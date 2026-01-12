/*****************************************************************************

	      External Declarations for Zap-A-Pal PAL Programmer

		  By Robert A. Freedman and RCL9

*****************************************************************************/

extern	unsigned seg,offset,off,word;

extern	int	s; 		/* s is Style of fuze-map print-out	*/

extern	int 	c,e;
extern	int 	cksum;		/* Check-Sum for JEDEC file	*/
extern	int 	devno;		/* 'D'nnnn read from Basic JEDEC file	*/
extern	int 	ci,chip_index;
extern	int 	mi,mfg_index;
extern	int     n,n_wr,i,j,sum,lr,ix,ino,af,T20,T24,fuzno,max;

extern	char    *fn, filename[], *p, fin[], fon[];
extern 	FILE 	*fopen();
extern	FILE    *fo,*fp;

extern	int fuzmap[];

/* For TEST program */
extern	int test_fuz;				

extern	uchar pins[];

extern	float 	a_slope,			/* Slopes of the Dacs */
		b_slope,
		c_slope,
		d_slope;

extern	float	vcc_want,			/* Used during Calibration testing */
		ver_want,
		vihh_want,
		c_want,
		d_want;

/* Autocalibration points */
extern	int 	vcclo, vcchi, vihlo, vihhi, c_hi, c_low, d_hi, d_low;	

/* Output value of current DAC in use */
extern	int 	dac;

/* Last value sent to each DAC in the calib() routine */
extern	int	dac_vcc, dac_vihh, dac_c_vcc, dac_d_vcc;

/* DAC settings */
extern	int 	ver, verlo, verhi, vcc, vihh, c_vcc, d_vcc;	

extern	float  act_10V;			/* Measured values of	*/
extern	float  act_2P5;			/* Voltage References	*/

extern	float  vcc_offset;		/* Compensate for diode drops	*/
extern	float  vihh_offset;		/* in Drivers & at GND Pin	*/

/* Pin # to verify, I/O adr, State */
extern	int    verpin,vad,fuse;	

/* Mux adr for Pins 14 - 23 */
extern	int    veradr[];

extern	uchar clear[];
extern	uchar odlo[];
extern	uchar odhi[];

/* Maps Pin numbers to Shift Register Position	*/
extern	int pind[];

/* PAL Manufacturer's Attributes */
extern struct	mfg_attr {
	char	*mfg_nam;
	float	verlo,verhi,vihh; 
} m[];

/* Chip Attributes: Name, Number of Pins and Number of Fuzes */
extern	struct	chip_type {
	char	*chip_nam;
	int	npins,fuzes; 
} t[];
