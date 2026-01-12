/*****************************************************************************

	     Miscellaneous Routines for Zap-A-Pal PAL Programmer

		  By Robert A. Freedman and RCL9

*****************************************************************************/

#include	<stdio.h>
#include	"zapal.h"
#include	"zextern.h"

int ani(c) 
	char c; 
{ 
	return(c >= 'A' ? (c-'7') : (c-'0')); 
}


/* Load from PAL chip into RAM	*/

loadpal()	
{
	mount();  
	max = (T20 ? 2048 : 3200);
	for (fuzno=0; fuzno < max; fuzno++) {
		if (fuzno % 256 == 0) 
			printf("\rLoading %3d %%",percent(fuzno,max) );
		fuzmap[fuzno] = (~readfuz( fuzno )) & 1;
	} 
	shutdn();
}

mount()
{	
	printf("\nPlease mount PAL in ZIF Socket, Watch Pin 1. Press any key.");
	get_key();
	revup();	/* Turn ON the Power	*/
}


shopal(f,s) 
	int f,s;
/*	  f = source:	0 = from RAM
			1 = from PAL chip
			2 = show differences

	  s =	Style:	0
			1 = JEDEC vs PALASM numbering
	  bits		2 = "1" & '0'vs 'X' & `-`
			4 = Space every 4 bits
			8 = Gap every 8 lines		*/

{	
	int fooz,fuz,ill,j,m;

	if (f != 0) 
		mount(); 
	clr_screen();
	s = s & 7;
	fuzno=0;  
	while ( fuzno < (T20 ? 2048 : 3200) ) {
		ill = (T20 ? 32 : 40);
		if (check_key()) 
			break;	/* Escape from long listing */
		if ((fuzno / ill) % 8 && s & 4) 
			printf("\n");
		printf ((s&1?"\n*L%04d ":"\n%4d "),(s&1?fuzno:fuzno/(T20?32:40)));
		for (j=0; j < (T20?32:40); j++) {
			f = f & 3; 
			m = (~fuzmap[fuzno]) & 1;
			if (f == 0) 
				fooz = m; 
			if (f == 1) 
				fooz = readfuz(fuzno); 
			if (f == 2) 
				fooz = m ^ readfuz(fuzno); 
			printf("%c",(f==2 ? (!fooz ? '.' : (m ? 'O':'U') ):
				(s&2?(fooz?'1':'0'):(fooz?'X':'-'))));
			fuzno++;
			if (!(fuzno % 4)) 
				printf((s&4?" ":""));
		}
	} 
	shutdn();	
	printf("\t\nPress any key..."); 
	get_key(); 
	clr_screen();
}

/* Invert n bits of val */

int bitinv(val,bits) 
	int val,bits;      
{       
	int res; 

	res = 0;
	while (bits) { 
		res = res + res + (val % 2); 
		val = val / 2; 
		bits--;
	}  
	return(res);
}

int percent(n,max) 
	int n,max;
{	
	float nn,maxx; 

	nn = n; 
	maxx = max;	
	return(nn * 100.0 / maxx);	
}

erra(e,p,q) 
	int e; 
	char *p,*q;
{	
	printf("\rERROR: %d %s%s, Hit any key.",e,p,q); 
	get_key();
}

/* Input a 2 digit hex number. Returns -2 if error or -1 if RETURN pressed. */

get_2hex()
{
	int	num1, num2;

	if ((num1 = get_1hex()) < 0)
		return(num1);
	if ((num2 = get_1hex()) < 0)
		return(num2);
	return(num1 * 16 + num2);
}

/* Input a 1 digit hex number. Return -1 if error */

get_1hex()
{
	int	a, num;

	a = toupper(get_key());
	if (a == '\r')
		return(-1);
	if (a >= 'A' && a < 'G') {
		num = (a - 'A' + 10);
		printf("%c", a);
	} else if (a >= '0' && a <= '9') {
		num = (a - '0');
		printf("%c", a);
	} else
		return(-2);
	return(num);
}

/* Allow Raw I/O To the hardware I/O Ports on the ZAP-A-PAL Board */

debug()
{
	int	port, data;

	for (;;) {
		clr_screen();
		printf("----------------------->>>>  Hardware Port I/O  <<<<--------------------------\n\n");
		printf("This allows accessing any one of the 16 ports on the ZAP-A-PAL board.\n\n");
		printf("1) Input from port\n");
		printf("2) Output to port\n");
		printf("X) Exit\n");
		printf("\n--->>> ");
		switch(tolower(get_key())) {
			case '1':
				clr_screen();
				printf("<RETURN> exits...\n");
				for (;;) {
					printf("\nPort (0-f) --->>> ");
					if ((port = get_1hex()) == -1)
						break;
					if (port == -2)
						continue;
					printf(". Value = %-04x", inport(port));
				}
				break;
			case '2':
				clr_screen();
				printf("<RETURN> exits...\n");
				for (;;) {
					printf("\nPort (0-f) --->>> ");
					if ((port = get_1hex()) == -1)
						break;
					if (port == -2)
						continue;
					printf(". Data value (00-ff) --->>> ");
					if ((data = get_2hex()) == -1)
						break;
					if (data == -2)
						continue;
					outport(port, data);
				}
				break;
			case 'x':
				return;
		}
	}
}
