/*****************************************************************************

		Definitions for Zap-A-Pal PAL Programmer.

		By Robert A. Freedman and RCL9

*****************************************************************************/

#define	TRUE	1
#define	FALSE	0
#define uchar unsigned char
#define ERROR -1
#define base  0x00	/* Zap-A-PAL Address Space is from 0 -> F hex */

/* Hardware related routines in Zdevice */
extern	char	inport();	/* Return one byte from Zapal Input port */
extern	char	get_key();	/* Input any key from Keybd without wait */
extern	char	check_key();	/* Returns NZ if key pressed. Doesn't wait for key */
extern	outport();		/* Send a byte to an output port	*/
extern	clr_screen();		/* Clear the screen			*/
extern	zot();			/* Do the programming pulse		*/
extern	init();			/* Initialize low level hardware 	*/

/* Hardware Output Ports */

#define 	DAC_A 		base + 0x0	/* Load Dac A			*/
#define 	DAC_B 		base + 0x1	/* Load Dac B			*/
#define 	SCLK  		base + 0x2	/* Write data to shift regs	*/
#define		AUX_REG		base + 0x3	/* 8 bit auxliary register	*/
#define 	DAC_C 		base + 0x4	/* Load Dac C			*/
#define 	DAC_D 		base + 0x5	/* Load Dac D			*/
#define		RESET		base + 0x7	/* Reset line on board		*/
#define 	STROBE 		base + 0x8	/* Strobe data into latches	*/
#define 	ENAB   		base + 0x9	/* Enable BiMOS drivers		*/
#define 	ENCH   		base + 0xA	/* Enable OD pin (pin 1)	*/
#define 	ENCL   		base + 0xB	/* Enable clock pin (pin 13)	*/
#define		EVCC		base + 0xC	/* Enable Vihh power to Vcc pin	*/
#define 	VLH    		base + 0xD	/* Power booster, 0=low,1 =high */
#define 	VINHIB 		base + 0xE	/* 0=inhibit booster, 1=enable  */
#define 	EOP   		base + 0xF	/* Enable output bimos drivers	*/

/* Hardware Input Ports */
 
#define P23 base+0x8				/* PAL pin 23 			*/
#define P22 base+0x0				/* PAL pin 22 			*/
#define P21 base+0x1				/* PAL pin 21 			*/
#define P20 base+0x2				/* PAL pin 20 			*/
#define P19 base+0x3				/* PAL pin 19 			*/
#define P18 base+0x4				/* PAL pin 18 			*/
#define P17 base+0x5				/* PAL pin 17 			*/
#define P16 base+0x6				/* PAL pin 16 			*/
#define P15 base+0x7				/* PAL pin 15 			*/
#define P14 base+0x9				/* PAL pin 14 			*/
#define CAL_10V_REF       inport(base+0xA) & 1	/* 10v transition test pin 	*/
#define	CAL_2P5_REF       inport(base+0xB) & 1	/* 2.5v transition test pin	*/

#define	TEST_LOW	0			/* Used in Auto-calibration routine */
#define	TEST_HI		1
