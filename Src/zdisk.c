/*****************************************************************************

	      Disk Related Routines for Zap-A-Pal PAL Programmer

		  By Robert A. Freedman and RCL9

*****************************************************************************/

#include	<stdio.h>
#include	<ctype.h>		/* Needed for isspace() */
#include	"zapal.h"
#include	"zextern.h"

/* Get a file name */

getfn(fn) 
	char *fn;	
{
	char *p; 
	int l;

	printf(" Enter File.nam ");
	fgets(fn,30,stdin); 
	l = strlen(fn);
	p = fn + l - 1;  
	*p = 0; 		/* Kill \n */
}


paltype()
{}


/* Load a JEDEC file into RAM */

loadjedec()	
{
	fp = fopen(fin,"r");
	if (!fp) { 
		erra(fp,"\nCan't OPEN file ",fin); 
		return(0); 
	}

	for (fuzno=0; fuzno < (T20?2048:3200); fuzno++)
		fuzmap[fuzno] = 2;	/* Mark as Empty	*/

	if (star() == ERROR) 
		return(ERROR);	/* scan for '*'	*/

	for(;;) {
		if (white() == ERROR) 
			return(ERROR);

		/* c must have command letter, Q G F L C #	*/
		if (c == 'Q') { 
			c = fgetc(fp);
			if (c == 'P')	/* max # of pins on chip   */
				{				
				}
			if (c == 'F')	/* max # of fuses in array */
				{				
				}
			if (star() == ERROR) 
				return(ERROR); /* scan for '*' */
			continue;
		}

		if (c == 'G')	/* State of Security Fuze	*/
			fgetc(fp); 

		if (c == 'F')	/* Default for undefined fuses	*/
			fgetc(fp); 

		if (c == 'C') {  		/*   CheckSum	*/
			fclose(fp); 
			return(0); 
		}

		if (c == 03) {  /*   other kind of CheckSum	*/
			fclose(fp); 
			return(0); 
		}

		if (c == 'D') {
			devno = 0; 
			for (i=0; i<4; i++) { 
				c = fgetc(fp);
				devno = devno * 10 + ani(c);	
			}
		}

		if (c == 'L') {
			fuzno = 0; 
			for (i=0; i<4; i++) {
				 c = fgetc(fp);
				fuzno = fuzno * 10 + ani(c);	
			}
			while ((c = white()) == '1' || c == '0')
				{ 
				fuzmap[fuzno] = c & 1; 
				fuzno++; 
			}
		}
	}
}


/* Eat chars till c is no longer whitespace	*/

white()	
{	
	do {	
		if ((c = fgetc(fp)) == ERROR) { 
			fclose(fp);
			erra(c,"while eating white-space","");
			return(ERROR); 
		}
	} 
	while (isspace(c)); 
	return(c);
}


/* Scan til you find an '*' */

star()	
{	
	while ((c = fgetc(fp)) != '*') {
		if (c == ERROR) { 
			fclose(fp);
			erra(c,"looking for '*'",""); 
			return(ERROR); 
		}
	} 
	return(c);
}


/* Write Fuze-Map to disk in JEDEC Format */

write_jedec()	
{
	int f;
	fp = fopen(fon,"w");
	if (!fp) { 
		erra(fp,"\nCan't OPEN file ",fon); 
		return(0); 
	}

	for (j=0; j < (T20 ? 2048:3200); j++) {
		f = fuzmap[j];

		if (j % (T20 ? 32:40) == 0 ) {
			if (f == 2) { 
				j = j + (T20?32:40) - 1; 
				continue; 
			} 
			fprintf(fp,"\n*L%04d ",j);
		}

		fprintf(fp,"%c",((f & 1) ? '1':'0'));
	}	
	fprintf(fp,"\n*C%04d\n",cksum);
	fclose (fp);
}
