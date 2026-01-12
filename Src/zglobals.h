/*****************************************************************************

	      Variable Declarations for Zap-A-Pal PAL Programmer

		   By Robert A. Freedman and RCL9

*****************************************************************************/

unsigned seg,offset,off,word;

int 	s;				/* s is Style of fuze-map print-out	*/

int 	c,e;
int 	cksum;				/* Check-Sum for JEDEC file	*/
int 	devno;				/* 'D'nnnn read from Basic JEDEC file	*/
int 	ci,chip_index;
int 	mi,mfg_index;
int     n,n_wr,i,j,sum,lr,ix,ino,af,T20,T24,fuzno,max;

char    *fn, filename[64], *p, fin[32], fon[32];
extern 	FILE *fopen();
FILE    *fo,*fp;

int fuzmap[4096];

/* For TEST program */
int test_fuz;				

uchar pins[32];

float 	a_slope,			/* Slopes of the Dacs */
	b_slope,
	c_slope,
	d_slope;

float	vcc_want,			/* Used during Calibration testing */
	ver_want,
	vihh_want,
	c_want,
	d_want;

/* Autocalibration points */
int 	vcclo, vcchi, vihlo, vihhi, c_hi, c_low, d_hi, d_low;	

/* Output value of current DAC in use */
int 	dac;

/* Last value sent to each DAC in the calib() routine */
int	dac_vcc, dac_vihh, dac_c_vcc, dac_d_vcc;

/* DAC settings */
int 	ver, verlo, verhi, vcc, vihh, c_vcc, d_vcc;	

float  act_10V = 9.945;			/* Measured values of	*/
float  act_2P5 =  2.465;		/* Voltage References	*/
float  vcc_offset = 0.000 + 0.7;	/* Compensate for diode drops	*/
float  vihh_offset = 1.130 + 0.7;	/* in Drivers & at GND Pin	*/

/* Pin # to verify, I/O adr, State */
int    verpin,vad,fuse;	

/* Mux adr for Pins 14 - 23 */
int    veradr[10] = {9,7,6,5,4,3,2,1,0,8};	
 

/*		Outputs		 Control		Inputs
	 2 2 2 1 1 1 1 1 2 1    1                              1 1
         2 1 0 9 8 7 6 5 3 4  1 3              2 3 4 5 6 7 8 9 0 1	*/
uchar clear[28] =
	{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}; /* Clear */
uchar odlo[28] =
	{0,0,0,0,0,0,0,0,0,0, 2,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}; /* OD lo */
uchar odhi[28] =
	{0,0,0,0,0,0,0,0,0,0, 1,2,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}; /* OD hi */

/* Maps Pin numbers to Shift Register Position	*/
int pind[24] = {10,18,19,20,21,22,23,24,25,26,27,28,11,9,7,6,5,4,3,2,1,0,8};

/* PAL Manufacturer's Attributes */
struct	mfg_attr {
	char	*mfg_nam;
	float	verlo,verhi,vihh; 
} m[] = {
	{ {" MMI"},	5.0, 5.5, 11.75 },
	{ {"-B MMI"},	6.0, 6.0, 10.00 },
	{ {" TI "},	5.0, 5.0, 10.50 },
	{ {" NSC"},	5.0, 5.5, 11.75 }
	};

/* Chip Attributes: Name, Number of Pins and Number of Fuzes */
struct	chip_type {
	char	*chip_nam;
	int	npins,fuzes; 
} t[] = {
	{ {"16L8"},	20, 2048 },
	{ {"16R8"},	20, 2048 },
	{ {"16R6"},	20, 2048 },
	{ {"16R4"},	20, 2048 },
	{ {"10H8"},	20, 2048 },
	{ {"10L8"},	20, 2048 },
	{ {"12H6"},	20, 2048 },
	{ {"12L6"},	20, 2048 },
	{ {"14H4"},	20, 2048 },
	{ {"14L4"},	20, 2048 },
	{ {"16H2"},	20, 2048 },
	{ {"16L2"},	20, 2048 },
	{ {"16C1"},	20, 2048 },
	{ {"16X4"},	20, 2048 },
	{ {"16A4"},	20, 2048 },
	{ {"12L10"},	24, 3200 },
	{ {"14L8"},	24, 3200 },
	{ {"16L6"},	24, 3200 },
	{ {"18L4"},	24, 3200 },
	{ {"20L2"},	24, 3200 },
	{ {"20C1"},	24, 3200 },
	{ {"20L10"},	24, 3200 },
	{ {"20X10"},	24, 3200 },
	{ {"20X8"},	24, 3200 },
	{ {"20X4"},	24, 3200 },
	{ {"20L8"},	24, 3200 },
	{ {"20R8"},	24, 3200 },
	{ {"20R6"},	24, 3200 },
	{ {"20R4"},	24, 3200 }
	};
