/*****************************************************************************

	      Calibration Routines for Zap-A-Pal PAL Programmer

		 By Robert A. Freedman and RCL9

*****************************************************************************/

#include	<stdio.h>
#include	"zapal.h"
#include	"zextern.h"

/* Calibrate Programmable Voltage Sources Manually      */
/* This is useful for Debugging during construction	*/

calib()
{	
	int 	d;			/* Current DAC being calibrated */
	int	dac[4]; 		/* Last output value of each DAC */

	/* Start at Dac-A */
	d = 0; 
	/* Zero out initial DAC values */
	dac[0] = dac[1] = dac[2] = dac[3] = 0;

	clr_screen();
	printf("-------------------------->>>>  Calibrate  <<<<-----------------------------\n\n");

	printf("Type 'C' to Calibrate, anything else to use defaults...");
	if (tolower(get_key()) != 'c' ) 
		return(0);

	printf("\n\nCommands:\n");
	printf("\tx - Exit and use new DAC values computed (all four must be set!)\n");
	printf("\tn - Go on to next DAC\n");
	printf("\tu - Increment DAC value\n");
	printf("\td - Decrement DAC value\n\n");

	outport(RESET,0);	/* Reset all of the latches */

	outport(EVCC,0);	/* Disable Vihh power to Vcc pin */
	outport(EOP,1);		/* Disble output Bimos drivers */
	outport(ENAB,0)»	/ª Disablå BiMOÓ driveró 	*/
	outport(ENCL,1);	/* Disable BiMOS drivers CLOCK	*/
	outport(ENCH,1);	/* Disable BiMOS drivers OD	*/
        outport(VINHIB,1);     	/* Turn ON 497 Booster		*/
	outport(VLH,0);		/* Set Booster to Low (15V)	*/
	/* outport(TRIG,0);*/	/* Make sure TRIG is low	*/
 
	outport(DAC_A,0);	/* Load DAC-A = 0 V		*/
	outport(DAC_B,0);	/* Load DAC-B = 0 V		*/
	outport(DAC_C,0);	/* Load DAC-C = 0 V		*/
	outport(DAC_D,0);	/* Load DAC-D = 0 V		*/

	for(;;) {
		/* Load DAC-A, B, C or D */
		outport(map_dac(d), dac[d]);    

	switch(d) {
		case 0:	printf("DAC-A VCC output value = %d. ", dac[0]); break;
		case 1:	printf("DAC-B VIHH output value = %d. ", dac[1]); break;
		case 2:	printf("DAC-C C_VCC output value = %d. ", dac[2]); break;
		case 3:	printf("DAC-D D_VCC output value = %d. ", dac[3]); break;
	}
	printf("Voltage is ");
	if (CAL_2P5_REF)
		printf(" > 2.5v ");
	else
		printf(" < 2.5v ");
	if (CAL_10V_REF)
		printf("and > 10v\n");
	else
		printf("and < 10v\n");
 
		/* Press Key for each step */
		c = tolower(get_key());	
		if (c == 'x' ) {
				/* Store last value sent to each DAC */
				dac_vcc = dac[0]; 
				dac_vihh = dac[1]; 
				dac_c_vcc = dac[2];
				dac_d_vcc = dac[3];
				break; 
		}
		/* Switch to next DAC	*/
		if (c == 'n') { 
			/* Zero out current DAC output */
			outport(map_dac(d), 0);
			 d = (d + 1) & 3;	
		} 
		/* Up */
		if (c == 'u') 
			dac[d] = ++dac[d] & 0xFF;
		/* Down */
 		if (c == 'd') 
			dac[d] = --dac[d] & 0xFF;
 	}
}


/* Automatic Calibration of Voltage Generators	*/
/* ar = 0 = Don't do voltage testing along various points */

autocal(ar) 
	int ar;	
{	
	float 	a;

	clr_screen();
	printf("-------------------------->>>>  Auto-Calibration  <<<<------------------------\n\n");

	printf("Turn on the board's power and remove any PALs. Press any key.");
	get_key();
	printf("\n\n");

	outport(DAC_A,0);	/* Load DAC-A = 0 V	*/
	outport(DAC_B,0);	/* Load DAC-B = 0 V	*/
	outport(DAC_C,0);	/* Load DAC-C = 0 V	*/
	outport(DAC_D,0);	/* Load DAC-D = 0 V	*/

	outport(RESET,0);	/* Reset all of the latches */

	outport(EVCC,0);	/* Disable Vihh power to Vcc pin */
	outport(EOP,1);		/* Disble output Bimos drivers */
	outport(ENAB,0)»	/ª Disablå BiMOÓ driveró 	*/
	outport(ENCL,1);	/* Disable BiMOS drivers CLOCK	*/
	outport(ENCH,1);	/* Disable BiMOS drivers OD	*/
        outport(VINHIB,1);     	/* Turn ON 497 Booster		*/
	outport(VLH,0);		/* Set Booster to Low (15V)	*/
	/* outport(TRIG,0);*/	/* Make sure TRIG is low	*/
 
	/* --------------  Start at Dac-A  -------------- */
	/* Start Dac output value at 0 */
	dac = 0;
	/* Get 2.5v transition point for DAC-A */
	if ((vcclo = slew(0, TEST_LOW)) == ERROR) return(ERROR);
	/* Get 10v transition point for DAC-A */
	if ((vcchi = slew(0, TEST_HI)) == ERROR) return(ERROR);
	/* Set DAC output voltage to point where it just crossed 10V */
	dac--;	
	slew(0, TEST_HI);
	outport(DAC_A,0);	/* Load DAC-A = 0 V	*/

	/* ----------------  Do DAC-B Next  ---------------- */
	dac = 0;
	printf("\n");
	/* Get 2.5v transition point for DAC-B */
	if ((vihlo = slew(1, TEST_LOW)) == ERROR) return(ERROR);
	/* Get 10v transition point for DAC-B */
	if ((vihhi = slew(1, TEST_HI)) == ERROR) return(ERROR);
	/* Set DAC output voltage to point where it just crossed 10V */
	dac--;
	slew(1, TEST_HI);
	outport(DAC_B,0);	/* Load DAC-B = 0 V	*/

	/* ----------------  Do DAC-C Next  ---------------- */
	dac = 0;
	printf("\n");
	/* Get 2.5v transition point for DAC-C */
	if ((c_low = slew(2, TEST_LOW)) == ERROR) return(ERROR);
	/* Get 10v transition point for DAC-C */
	if ((c_hi = slew(2, TEST_HI)) == ERROR) return(ERROR);
	/* Set DAC output voltage to point where it just crossed 10V */
	dac--;
	slew(2, TEST_HI);
	outport(DAC_C,0);	/* Load DAC-C = 0 V	*/

	/* ----------------  Do DAC-D Next  ---------------- */
	dac = 0;
	printf("\n");
	/* Get 2.5v transition point for DAC-D */
	if ((d_low = slew(3, TEST_LOW)) == ERROR) return(ERROR);
	/* Get 10v transition point for DAC-D */
	if ((d_hi = slew(3, TEST_HI)) == ERROR) return(ERROR);
	/* Set DAC output voltage to point where it just crossed 10V */
	dac--;
	slew(3, TEST_HI);
	outport(DAC_D,0);	/* Load DAC-D = 0 V	*/

	/* Calculate slopes of each DAC response */
	a_slope = (act_10V - act_2P5) / (vcchi - vcclo);
	b_slope = (act_10V - act_2P5) / (vihhi - vihlo);
	c_slope = (act_10V - act_2P5) / (c_hi - c_low);
	d_slope = (act_10V - act_2P5) / (d_hi - d_low);

	if (!ar) {
		printf("\n\nPress any key to continue... "); 
		get_key(); 
		return(0); 
	}
	printf("\n\nSlope_A = %.4f. Slope_B = %.4f. Slope_C = %.4f. Slope_D = %.4f.\n",
		a_slope, b_slope, c_slope, d_slope);

	printf("\nThe following lists output DAC values needed to set the desired voltage:\n");
	for (a=1.0; a <= 16.0; a+=0.5) { 
		vcc_want = vihh_want = 	c_want = d_want = a;
		vcc  = vcclo + (vcc_want - act_2P5) / a_slope;
		vihh = vihlo + (vihh_want - act_2P5) / b_slope;
		c_vcc  = c_low + (c_want - act_2P5) / c_slope;
		d_vcc  = d_low + (d_want - act_2P5) / d_slope;
		printf ("\n%4.2fv =>  VCC = %4d. ", a, vcc);
		printf ("VIHH = %4d. ", vihh);
		printf ("C_VCC = %4d. ", c_vcc);
		printf ("D_VCC = %4d.", d_vcc);
	} 
	printf("\n\nPress any key to continue...");
	get_key(); 
}


/ª Scaî foò Thresholdó */

int slew(d,test) 
	int 	d;			/* DAC number to test (0-3) */
	int	test;			/* Test against 10v or 2.5v */
{
	do { 
		if (dac > 255) {
			printf("\n\nCalibration Failure: DAC-%c off Scale", (d + 'A'));
			printf("\nProbable hardware failure or wrong port address.");
			printf("\nPress any key to continue...");
			get_key();
			return(ERROR); 
		}
		/* Load the DAC with the next test value */
		outport(map_dac(d), dac);
	     	dac++;
	} while (!(test ? CAL_10V_REF : CAL_2P5_REF));

	printf("\r");
	printf("(DAC-%c) ", 'A' + d);
	switch(d) {
		case 0:	printf("VCC   = ");	break;
		case 1:	printf("VIHH  = ");	break;
		case 2:	printf("C_VCC = ");break;
		case 3:	printf("D_VCC = ");break;
	}

	switch(d) {
		case 0:	printf("%3d", vcclo);	break;
		case 1:	printf("%3d", vihlo);	break;
		case 2:	printf("%3d", c_low);	break;
		case 3:	printf("%3d", d_low);	break;
	}
	printf(" @ 2.5 Volt Point and ");
	switch(d) {
		case 0:	printf("%3d", vcchi);	break;
		case 1:	printf("%3d", vihhi);	break;
		case 2:	printf("%3d", c_hi);	break;
		case 3:	printf("%3d", d_hi);	break;
	}
	printf(" @ 10.0V Point. DAC value = %d", dac);
	return(--dac);		
}

/* Map DAC number (0-3) to DAC port address */

map_dac(d)
	int d;
{
	switch(d) {
		case 0:
			return(DAC_A);
		case 1:
			return(DAC_B);
		case 2:
			return(DAC_C);
		case 3:
			return(DAC_D);
	}
}
