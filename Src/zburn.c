/*****************************************************************************

	      Programming Routines for Zap-A-Pal PAL Programmer

		By Robert A. Freedman and RCL9

*****************************************************************************/

#include	<stdio.h>
#include	"zapal.h"
#include	"zextern.h"

/* Set up DACs and Turn On Power	*/

revup()		
{
	outport(ENAB,0);	/* Disable BiMOS drivers 	*/
	outport(ENCL,1);	/* Disable BiMOS drivers CLOCK	*/
	outport(ENCH,1);	/* Disable BiMOS drivers OD	*/

        outport(VINHIB,1);     	/* Turn ON 497 Booster		*/
	outport(VLH,0);		/* Set Booster to Low (15V)	*/
	/* outport(TRIG,0);*/	/* Make sure TRIG is low	*/
 
	vcc_want = m[mi].verlo +  vcc_offset;
	ver_want = m[mi].verhi +  vcc_offset;	
	vihh_want = m[mi].vihh  + vihh_offset;

	verlo = vcc  = vcclo + ( vcc_want - act_2P5) / a_slope;
	verhi = ver  = vcclo + ( ver_want - act_2P5) / a_slope;
        vihh = vihlo + (vihh_want - act_2P5) / b_slope;

        outport(DAC_A,vcc);	/* Load DAC-A = vcc Volts	*/
        outport(DAC_B,vihh);	/* Load DAC-B = VIHH Volts	*/
/*	printf ("\nvcc = %3d - vihh = %3d",vcc,vihh);	*/
}


/* Turn it all off */

shutdn()	
{
	outport(ENAB,0);	/* Disable BiMOS drivers 	*/
	outport(ENCL,1);	/* Disable BiMOS drivers CLOCK	*/
	outport(ENCH,1);	/* Disable BiMOS drivers OD	*/

        outport(VINHIB,0);     	/* Turn OFF 497 Booster		*/
        outport(DAC_A,0);	/* Load DAC-A  =  0 V		*/
        outport(DAC_B,0);	/* Load DAC-B  =  0 V		*/
}


/* Burn a PAL from RAM image */

zapal() 	
{
	mount();
        outport(DAC_A,verlo);		/* Load DAC-A = vcc Volts	*/
	pass(3);			/* Programm Lo			*/
        outport(DAC_A,verhi);		/* Load DAC-A = vcc Volts	*/
	pass(2);			/* Programm Hi			*/
        outport(DAC_A,verlo);		/* Load DAC-A = vcc Volts	*/
	pass(1);			/* Programm Lo			*/
	veripal();			/* Verify			*/
	shutdn();
}


pass(n) 
	int n;
{	
	int m,x;

	printf("\rBurning-%1d  ",n);	
	for (fuzno=0; fuzno <(T20?2048:3200); fuzno++) {
	/*	if (fuzno % (T20?32:40) == 0) printf(".");	*/
		m = fuzmap[fuzno] & 3;
		if (m >= 2) 
			continue;	/* Skip Phantom Fuze	*/
		if (m == 0) 
			continue;	/* Skip Wanted  Fuze	*/
		if (m == 1) {	/* for 16L8 fuze is low if blown	*/
			/* Set-up and see if fuze is already blown	*/
			x = readfuz(fuzno);	/* Read Fuze State	*/
			if (!x) 
				continue;	/* Skip if blown */
			/* All Reprieves Exhausted, Blast Fuze to Oblivion */
			zot();
		} 
	}	
	return(0);
}


/* Verify PAL chip versus RAM	*/

veripal()	
{
	for (fuzno=0; fuzno < (T20?2048:3200); fuzno++) {
		if (fuzno%64 == 0) 
			printf("\rVerifying-%4d ",fuzno);	
		if ((fuzmap[fuzno] & 1) ^ ((~readfuz(fuzno) & 1))) {
			shutdn(); 
			erra(fuzno,"Verify FAILed","");
			return(ERROR); 
		}
	} 
	shutdn(); 
	printf("\nVerify PASSed");
}


/* Be Caraful with this one */

test()	
{	
	revup();
	while (readfuz(test_fuz))
		zot(); 
	test_fuz = ++test_fuz & 2048-1;	
	shutdn();
}


/* Set up to read or write a fuze */

int readfuz(fuzno) 
	int fuzno;	
{	
	int half,pln;		/* Return state of selected fuse  */

	pln = (fuzno / ( T20 ? 32 : 40 ));
	if (pln > (T20 ? 63: 79)) 
		return(ERROR);

	outport(ENAB,0);	/* Disable BiMOS drivers 	*/
	outport(ENCL,1);	/* Disable BiMOS drivers CLOCK	*/
	outport(ENCH,1);	/* Disable BiMOS drivers OD	*/

	half = ( T20 ? 32 : 40 );	/* Set OD and CLOCK pins	*/
	pin( 1, (pln >= half?1:2));	
	pin(13, (pln >= half?2:1));

	(pln >= half ? ldsr(odhi) : ldsr(odlo) );	/* Shift OD & Clock */

	outport(ENCL,0);	/* Enable BiMOS drivers	CLOCK	*/
	outport(ENCH,0);	/* Enable BiMOS drivers	OD	*/

	selfuz(fuzno);		ldsr(pins);
	outport(ENAB,1);	/* Enable BiMOS drivers		*/
 	return(verifuz());	/* Return state of addressed fuze	*/
}

/* Return state of fuze */

int verifuz()	
{
	/* Assume the shift-registers are all set up by selfuz(fuzno)	*/
	outport(ENCL,1);	/* Pulse CLOCK pin by floating	*/
	outport(ENCL,0);	/* CLOCK to Z momentarily	*/

	vad = veradr[verpin-14] + base;	/* Compute Mux adr of Pin	*/
	fuse = inport(vad) & 1;	/* Read the state of the fuse	*/
	return(fuse);		/* 0 = Blown, 1 = Intact fuse	*/
}


/* Analyzes fuze-number, and sets up all pins */

selfuz(fuzno) 
	int fuzno; 
{       
	int an,half,of,ox,lino,pl,pln,i;

	half = ( T20 ? 32 : 40 );
	ldsr(clear);	/* Clear out old fuze info	*/
	/*	Compute and place input pins	*/
	lino =( fuzno % half );
	pln = ( fuzno / half );
	if (pln > (T20?63:79)) 
		return(ERROR);

	lr = 0; 
	if (lino & 2)  
		lr = 2;        /* Find which half       */
        ix = 0; 
	if (!(lino & 1)) 
		ix = 1;        /* Find the state of Pin x */
 
        /* Now find where to put the selected pin, ie [I0..I9]           */
        for (i=0;i <10; i++) 
		pin(2+i,2); /* Pull all input pins to VIHH   */
 
	ino = lino / 4; 
	pin(2+ino,ix);	/* Then set Selected pin to TTL  */
 
	/*	Compute and Place Output Pins	*/
 
	/*	Set OD and CLOCK pins		*/
	pin( 1, (pln >= half ? 1:2));	
	pin(13, (pln >= half ? 2:1));

        pl = pln; 
	if (pln >= half) 
		pl = pln-half;
        an =  pl % 8;				/* A0..An = pl mod 8	*/
        ox = (pl / 8) & (T20 ? 0xF:0x1F);	/* Select Outp Pin to pulse */
        for (i=14; i<=23; i++) { 
		pin(i,0); 			/* Clear all Outputs	*/
	}     
               
	af = (T20?16:15 ); 
	of = (T20?22:23 ); 
	if ( pln >= half ) { 
		af = (T20?19:19 ); 
		of = (T20?18:18 ); 
	}
	if ((pln <  half) && !T20 ) 
		an = bitinv(an,4);	
	an = an & (T20? 7 : 0xF );

	for ( i = (T20?2:3); i >= 0; i--) {  /* Set Address bits  */
		pin(af+i,( an % 2 ? 2 : 0 )); 
		an = an / 2;
	}
	pin(of-ox,4);   /* Set Output Pin to Pulse      */
	verpin = of-ox;	/* Save pin to verify fuse state	*/
	pin((pln < half ? (T20?15:14) : (T20?22:23) ),lr); /* Set L/R	*/
	/* Now all the pins are set up for programming or verification	*/
}


/* Read or Store value of a pin */

int pin(n,val) 
	int n,val;       
{
	int v; uchar *p;       

	if (n == 0 || n > 24 ) 
		val = 0xE;
	p = pins + *(pind + n - 1 );
	v = *p; 
	*p = val; 
	return(v);
}


shopin()
{	printf("\n");
	for (i=0;i<28;i++) {	
		if( i == 10 || i == 18 ) 
			printf(" -");
		if( i ==  4 || i ==  8 || i == 23 ) 
			printf(" " );
		printf (" %1x",pins[i] & 0xF ); 
	}
}


copin(p) 
	uchar *p;
{	
	int i; 

	for (i=0;i<28;i++) { 
		pins[i] = p[i]; 
	}  
}


/* Load pins into Hardware Shift Register */

ldsr(p) 
	char *p;        
{	
	int i; 

	i=27; 
	while ( i >= 0 )
		outport(SCLK,p[i--] ); 
        outport(STROBE,1);	/* Strobe all bits into BiMOS latches	*/
        outport(STROBE,0);
}
