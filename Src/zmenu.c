/*****************************************************************************

	         Main Menu for Zap-A-Pal PAL Programmer

		By Robert A. Freedman and RCL9

*****************************************************************************/

#include	<stdio.h>
#include	"zapal.h"
#include	"zextern.h"

/* Display List of ZAPAL Commands */
menu()		
{
	int 	szct, szmt;		/* Number of entries in structures */
	int	menu_num;

	szct = sizeof(t) / sizeof(struct chip_type);
	szmt = sizeof(m) / sizeof(struct  mfg_attr);
	menu_num = 1;

	for(;;) {
		clr_screen();
		printf("      ZAP-A-PAL Programmer - Version 1.1\n");
		printf("(C) 1987 by R. A. Freedman and RCL9.");

		/* Set flag if this chip has 20 pins */
		T20 = (t[ci].npins == 20 ? 1 : 0 );

		printf("\n\n\t%PAL-%s %s has %2d pins and %4d fuzes        \n",
			t[ci].chip_nam, m[mi].mfg_nam, t[ci].npins, t[ci].fuzes);

		printf("\n\tSpacebar - Go to next menu\n");
		if (menu_num) {
		printf("\n\tT - Select next device type");
		printf("\n\tU - Select next chip manufacturer");
		printf("\n\tL - Load PAL chip into RAM");
		printf("\n\tR - Show fuze map from PAL Chip");
		printf("\n\tY - Show fuze map from RAM");
		printf("\n\tC - Compare PAL chip with RAM");
		printf("\n\tZ - Burn  a PAL from RAM");
		printf("\n\tV - Verify  PAL against RAM");
		printf("\n\tX - Quit to DOS");
		} else {
		printf("\n\tI - Set to READ  file %s        ",fin);
		printf("\n\tO - Set to WRITE file %s        ",fon);
		printf("\n\tJ - Load  JEDEC file into RAM");
		printf("\n\tW - Write JEDEC file from RAM");
		printf("\n\tS - Style of Fuze MAP is %s ",(s&1?"JEDEC":"PALASM"));
		printf("\n\tF - FUZEs are shown as %s",(s&2?"'1' or '0'":"'X' or '-'"));
		printf("\n\tG - %s GAP every 4th fuse",(s&4?"Insert":"Don't "));
		printf("\n\tM - Calibrate manually");	
		printf("\n\tA - Auto-Calibrate");
		printf("\n\tD - I/O port access");
		}
		printf("\n\n--->>> ");

		switch(tolower(get_key())) {
			case ' ':
				menu_num = (++menu_num) & 1;
				break;
			case 'd':
				/* Allow user access to the hardware ports */
				debug();
				break;
			case 'm': 
				calib();
				break;
			case 'a': 
				autocal(1);
				break;
			case 'x': 
				clr_screen();
				return(0);
				break;
			case 'r': 
				/* Show Fuz-Map from chip */
				shopal(1,s);
				break;	
			case 'y': 
				/* Show Fuz-Map from RAM  */
				shopal(0,s);
				break;	
			case 'e': 
				test();	
				break;
			case 'z': 
				zapal();
				break;
			case 'v': 
				mount(); 
				veripal();
				break;
			case 'l': 
				loadpal();
				break;
			case 'j': 
				if (loadjedec() == ERROR) {
					printf("ERROR - Press any key..."); 
					get_key();
				} 
				break;
			case 'w': 
				write_jedec();	
				break;
			case 'p': 
				paltype();	
				break;
			case 'c': 
				/* Compare PAL with RAM	*/
				shopal(2,s);	
				break;	
			case 's': 
				s = s ^ 1;
				break;
			case 'f': 
				s = s ^ 2;
				break;
			case 'g': 
				s = s ^ 4;
				break;
			case 't': 
				ci= (ci+1) % szct;
				break;
			case 'u': 
				mi= (mi+1) % szmt;
				break;
			case 'i': 
				getfn(fin);	
				break;
			case 'o': 
				getfn(fon);
				break;
			default :
				break;
		} 
		shutdn();
	}
}

