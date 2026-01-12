/ ****************************************************************************
/ *									     *
/ *          DEVICE.S - Atari ST Cartridge Low Level I/O for ZAPAL.C	     *
/ *									     *
/ *  			     By RCL9				     *
/ *									     *
/ *				August 5, 1987				     *
/ *									     *
/ ****************************************************************************

/ 	Routines:
/		outport(port,data)	- send byte to cartridge port
/		inport(port)		- get byte from cartridge port
/		wipe()			- clear the screen
/		get_key()		- get keypress from keyboard

/ ******************************* OutPort()  *********************************
/
/ 	Syntax:  	outport(Port, Data)
/
/ 	Input:		Port# = 0..0F (8 bit addr for Zapal Hardware)
/			Data  = 0..FF (8 bit output only to Zapal board)
/
/ ****************************************************************************

	.shri
	.globl outport_

outport_:
	link	a6, $0
	move.w	10(a6), d0		/ Get our data byte
	andi.l	$0x0FF, d0		/ We only need lower 8 bits of data
	addi.l	$0xFA0000, d0		/ Get ready for the data latch (ROM4)
	movea.l d0, a0			 
	move.b	(a0), d0		/ And latch the data into the latches
	clr.l	d0
	move.w	8(a6), d0		/ Next pick up the Port address
	asl.l	#1,d0			/ Make the Port # into a work offset
	addi.l	$0xFBFF00, d0		/ Index to special addr latch address
	movea.l d0, a0			
	move	(a0), d0		/ Latch the Port address into the latches
	unlk	a6
	rts				


/ ******************************** InPort()  *********************************
/
/ 	Syntax:  	inport(Port)
/
/ 	Input:		Port# = 0..0F (8 bit addr for Zapal Hardware)
/	Output:		8 bit data from Zapal I/O Ports
/
/ ****************************************************************************

	.shri
	.globl inport_

inport_:
	link	a6, $0
	clr.l	d0
	move.w	8(a6), d0		/ Get Port address from C code
	asl.l	$1,d0			/ Make the Port # into a work offset
	addi.l	$0xFBFF20, d0		/ Index into special read zone
	movea.l d0, a0			
	move.w	(a0), d0		/ Read data from Zapal board
	unlk	a6
	rts				


/ ******************************** get_Key()  *******************************
/
/ 	Syntax:  	get_key()
/
/ 	Input:		None
/	Output:		Character from the keyboard
/
/ ****************************************************************************

	.shri
	.globl get_key_

get_key_:
	move.w	$1,-(sp)		/ Go get a keyboard character
	trap	$1			/ GemDos call
	addq.l	$2,sp			/ Fix up the stack
	and.w	$0xff,d0		/ Keep only the lower 8 bits
	rts


/ ********************************* Wipe()  *********************************
/
/ 	Syntax:  	wipe()
/
/ 	Input:		None
/	Output:		Clears the screen
/
/ ****************************************************************************

	.shri
	.globl wipe_

wipe_:	move.w	$0x1b,-(sp)		/ Send ESCape
	move.w	$2,-(sp)
	trap	$1			/ GemDos call
	addq.l	$4,sp		
	move.w	$'E,-(sp)		/ Send 'E'
	move.w	$2,-(sp)
	trap	$1			/ GemDos call
	addq.l	$4,sp		
	rts


/ ********************************* Zot()  *********************************
/
/ 	Syntax:  	zot();

/ 	Input:		None
/	Output:		Performs all the PAL programming timing 
/
/ ****************************************************************************

	.shri
	.globl zot_

zot_:

/* TRIGger the timing PAL to zap the fuze */
zot()	
{		while (  BUSY ) { outport(TRIG,0); }
		while ( !BUSY ) { outport(TRIG,1); }
		while (  BUSY ) { outport(TRIG,0); }
		return(0);
}

	rts

	end
