/* NOTE: All references to TRIG have been commented out - RCL */

/*****************************************************************************

		Main Line Code for Zap-A-Pal PAL Programmer

		 By Robert A. Freedman and RCL9

*****************************************************************************

NOTE: 	Comment out 'M - Calibrate Manually' line when done.

History (in reverse order)

1.1	Jan. 15, 1988	- Modularized code.
			- Wrote Z80 device dependent routines

1.0	Aug. 3, 1987	- Derived from original BYTE version 2.0
			- Written by Robert A. Freedman, (c) 1986
 
******************************************************************************/

#include <stdio.h>
#include "zapal.h"
#include "zglobals.h"		/* Variable declarations */

/******************************  MAIN  ********************************/

main(argc,argv) 
	int argc; 
	char **argv;
{
	/* Init Dummy file.names */
	strcpy(fin,"file.jed");	
	strcpy(fon,"fmap.jed");

	T20 = 1;	/* Do 20-pin PALs */
	ci = mi = 0;
	test_fuz = 0;
	s = 5;

/*	dac_vcc  = 131;	*/	/* DAC-A 131 = 5.02 Volts	*/
/*	dac_vihh = 154;	*/	/* DAC-B 154 = 11.75 Volts: 13.00 @ VIHH */

	/* Initialize low level hardware */
	init();
	/* Go calibrate the 4 DACs */
	autocal(0);	
	shutdn();
	menu();		
	shutdn();	
	exit(0); 
 }

