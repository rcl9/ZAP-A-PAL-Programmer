; *************************************************************************
; *									  *
; *          DEVICE.ASM - Device Dependent Routines for Zapal.c		  *
; *									  *
; *  			   By RCL9				  *
; *									  *
; *			    January 15, 1987				  *
; *									  *
; *************************************************************************

	cseg

; Routines:
;	outport(port,data)	- send byte to cartridge port
;	inport(port)		- get byte from cartridge port
;	clr_screen()		- clear the screen (VT52)
;	get_key()		- get keypress from keyboard (CP/M)
;	check_key()		- return NZ if key pressed. Doesn't wait for key.
;	init()			- initialize low level hardware

; General Equates

ESC	equ	01bh		; Escape character
bdos	equ	0005h		; CP/M BDOS entry point
cr	equ	0dh		; Carriage return
lf	equ	0ah		; Line feed

IN_MODE		equ	1	; Port C is currently input
OUT_MODE 	equ	0	; Port C is currently output

; Bit mapped Port C Control Latch Equates

; NOTE: When Port C changes direction Port A gets reset to 0!!! This
;	causes the STROBE* pin to go low and strobe a false value to
;	the PAL board. This was fixed by inverting the Strobe pin
;	on the conversion board.

WRITE		equ	0	; R/W* line
READ		equ	80h

STROBE		equ	40h	; Remote board Strobe*
NO_STROBE	equ	0

LOW		equ	0	; R/W from LSB latch
HIGH		equ	20h	; R/W from MSB latch

PORT_STROBE	equ	0	; Local board Port_Strobe*
NO_PORT_STROBE	equ	10h

; Computer types for the 'computer' type variable

sorcerer	equ	1	; Adm 3A, 2mhz, S100 I/O 8255 interface
piedpiper	equ	2	; Hazeltine, 4Mhz, Expansion board 8255
colossus	equ	3	; 4 mhz, lcd screen (presently VT52 clear screen)
cypher		equ	4	; 4 Mhz, VT52, Expansion Board 8255

; This is port base+4 for the 8255 I/O chip. Ie, if it is addressed for
; 252, then base will be 248.

base:	db	192		; Sorcerer
	db	248		; Pied Piper
	db	20h+4		; Colossus
	db	29		; Cypher (on odd bytes)

; ******************************* OutPort()  *********************************
;
; 	Syntax:  	outport(Port, Data)
;
; 	Input:		Port# = 0..0F (8 bit addr for Zapal Hardware)
;			Data  = 0..FFFF
;
; ****************************************************************************

	public	outport_

outport_:
	db	0edh, 043h	; ld (temp),bc
	dw	temp
	pop	b		; get return address
	pop	h		; get Port
	pop	d		; get data
	push	d		; and restore the stack..
	push	h
	push	b		

	mov	a,l		; Get the port (address) number
	ani	0fh		; make 0-15
	sta	address		; and update the new port address

; Make sure the 8255 and conversion latches are in Output mode

	lda	inout		; Port C in Output mode?
	cpi	OUT_MODE
	cnz	output		; No, change C bus to output

; Latch the MSB into the conversion latches

	mov	a,d		; Send MSB value out to 8255 port C
	call	putout
	db	6

	lda	address		; Strobe the MSB of data into MSB latch
	push	psw
	ori	WRITE + NO_STROBE + PORT_STROBE + HIGH
	call	putout		; Send A to control port A
	db	4

	pop	psw		; De-assert Port_Strobe*
	ori	WRITE + NO_STROBE + NO_PORT_STROBE + HIGH
	call	putout		
	db	4

; Now Latch the LSB into the conversion latches

	mov	a,e		; Send LSB Data value out to 8255 Port C
	call	putout
	db	6

	lda	address		; Strobe the LSB of data into LSB latch
	push	psw
	ori	WRITE + NO_STROBE + PORT_STROBE + LOW
	call	putout		; Send A to control port A
	db	4

	pop	psw		; De-assert Port_Strobe*
	ori	WRITE + NO_STROBE + NO_PORT_STROBE + LOW
	call	putout		
	db	4

; Now latch the 16 bit data word into the remote board via Strobe*

	lda	address		; Strobe the LSB of data into LSB latch
	push	psw
	ori	WRITE + STROBE + NO_PORT_STROBE + LOW
	call	putout		; Send A to control port A
	db	4

	pop	psw		; De-assert Strobe*
	ori	WRITE + NO_STROBE + NO_PORT_STROBE + LOW
	call	putout		
	db	4

	db	0edh, 04bh	; ld bc,(temp)
	dw	temp
	ret


; ******************************** InPort()  *********************************
;
; 	Syntax:  	inport(Port)
;
; 	Input:		Port# = 0..0F (8 bit addr for Zapal Hardware)
;	Output:		8 bit data from Zapal I/O Ports (Upper 8 masked off)
;
; ****************************************************************************

	public	inport_

inport_:
	pop	h		; get return address
	pop	d		; get Port
	push	d
	push	h		
	push	b		; Save it

	mov	a,e		; Get the port number
	ani	0fh		; make 0-15
	sta	address		; and update the new port address

; Make sure the 8255 and conversion latches are in Input mode

	lda	inout		; Port C in Input mode?
	cpi	IN_MODE
	cnz	input		; No, change C bus to input

; Read the MSB from remote board (at specified 4 bit address)

	lda	address		; Send address to Remote board & assert Strobe*
	ori	READ + STROBE + NO_PORT_STROBE + HIGH
	call	putout		; Send A to control port A
	db	4

	call	getinp		; Pick up MSB from Port C
	db	6
	mov	d,a		; And save it

; Now read LSB from remote board (at specified 4 bit address)

	lda	address		; Send address to Remote board & assert Strobe*
	push	psw
	ori	READ + STROBE + NO_PORT_STROBE + LOW
	call	putout		; Send A to control port A
	db	4

	calì	getinð		» Picë uð LSÂ froí Porô C
 	db	6
	mov	e,a		; And save it

; And de-assert the Strobe* signal so remote board stops sending

	pop	psw		; De-assert Strobe*
	ori	READ + NO_STROBE + NO_PORT_STROBE + LOW
	call	putout		
	db	4

	mvi	h,0		; Mask off upper 8 bits
	mov	l,e		; Move LSB into L
	mov	a,e
	ora	a		; Set Z flag
	pop	b		; Restore it
	ret

; ******************************** get_Key()  *******************************
;
; 	Syntax:  	get_key()
;
; 	Input:		None
;	Output:		Wait for character from the keyboard
;
; ****************************************************************************

	public	get_key_

get_key_:
	push	b
nochar:	
	mvi	c,6
	mvi	e,0ffh
	call	bdos
	ora	a
	jz	nochar
	mvi	h,0		; Put in HL for Aztec C
	mov	l,a
	pop	b
	ret


; ***************************** check_key()  *********************************
;
; 	Syntax:  	check_key()
;
; 	Input:		None
;	Output:		NZ if key pressed. Doesn't wait for keypress.
;
; ****************************************************************************

	public	check_key_

check_key_:
	push	b
	mvi	c,6
	mvi	e,0ffh
	call	bdos
	ora	a		; Returns NZ in A if key pressed
	mvi	h,0		; Put in HL for Aztec C
	mov	l,a
	pop	b
	ret


; ******************************* clr_screen() ******************************
;
; 	Syntax:  	clr_screen()
;
; 	Input:		None
;	Output:		Clears the screen
;
; ****************************************************************************

	public	clr_screen_

clr_screen_:
	push	psw			; Sorcerer clear screen
	lda	computer
	cpi	sorcerer
	jnz	clrtt2
	mvi	a,0ch			
	call	type
	pop	psw
	ret

clrtt2:	cpi	colossus
	jz	col
	cpi	cypher			; Cypher clear screen
	jnz	ppcls
col:	mvi	a,ESC
	call	type
	mvi	a,'E'			; VT52 clear screen
	call	type
	pop	psw
	ret
	
ppcls:	mvi	a,07eh			; Pied Piper Clear Screen
	call	type
	mvi	a,01ch
	call	type
	pop	psw
	ret


; ********************************* Zot()  *********************************
;
; 	Syntax:  	zot();
;
; 	Input:		None
;	Output:		Performs all the PAL programming timing 
;
; ****************************************************************************

	public	zot_

; TRIGger the timing PAL to zap the fuze 

zot_:

; zot()	
; {		while (  BUSY ) { outport(TRIG,0); }
;		while ( !BUSY ) { outport(TRIG,1); }
;		while (  BUSY ) { outport(TRIG,0); }
;		return(0);
; }
	ret


; ********************************* init()  *********************************
;
; 	Syntax:  	init();
;
; 	Input:		None
;	Output:		Initializes the low level hardware
;
; ****************************************************************************

	public	init_

init_:	push	b
	lxi	d,askmsg
	mvi	c,9
	call	bdos
pr1:	call	get_key_
	cpi	'1'
	jc	pr1
	cpi	'4'+1
	jnc	pr1
	sui	'0'		; make selection binary
	sta	computer
	dcr	a
	lxi	h,base
	mov	e,a
	mvi	d,0
	dad	d
	db	07eh		; ld 	a, (hl) - Get 8255 baseport address
	sta	baseport
	pop	b
	ret

askmsg:	db	cr,lf
	db	'Please select the host computer:'
	db	cr,lf,lf
	db	'     1) Sorcerer (S100 parallel connector).'
	db	cr,lf
	db	'     2) Pied Piper (Expansion board parallel port).'
	db	cr,lf
	db	'     3) Colossus + VT52 Terminal.'
	db	cr,lf
	db	'     4) Cypher + VT52 Terminal.'
	db	cr,lf,lf
	db	'--->>> $'

; ----------->>>>  8255 dependent subroutines  <<<<--------------

; Initialize I/O hardware. Make 8255 Port A output &  C to input.

init:	mvi	a,IN_MODE	; set in/out flag to input
	sta	inout
	mvi	a,137
	call	putout		; make 8255 port A & C input
	db	7
	call	input		; Init Port C's attached latch
	ret

; Make Port C of 8255 output and programmer buss to output

output:	push	psw
	push	d
	mvi	a,OUT_MODE	; signify output mode
	sta	inout
	lda	address
	ori	NO_STROBE + NO_PORT_STROBE + WRITE	; make drivers output
	call	putout		; output to Port A
	db	4
	mvi	a,128
	call	putout		; set 8255 Port A & C to output
	db	7
	pop	d
	pop	psw
	ret

; Make Port C of 8255 input and programmer buss to input

input:	push	psw
	push	d
	mvi	a,IN_MODE	; signify input mode
	sta	inout
	mvi	a,137
	call	putout		; set 8255 Port A to output, C to input
	db	7		; enable programmer buss to input
	lda	address
	ori	NO_STROBE + NO_PORT_STROBE + READ	; make drivers input
	call	putout		; output to Port A
	db	4
	pop	d
	pop	psw
	ret

; Input byte from port specified by offset. This gets around hardware
; related I/O problems encountered with the 8255.
;
;   getinp
;   db 	'4'		- port offset

getinp:	xthl				; ex hl, (sp)
	mov	a,m			; ld	a,(hl)
	inx	h
	xthl				; ex	(sp),hl
	push	b
	mov	c,a
	lda	baseport
	mov	b,a
	lda	computer
	cpi	cypher
	mov	a,b
	jnz	notcyp

	mov	a,c			; translate for cypher odd byte
	sui	4			; addressing
	db	0cbh, 027h		; sla	a
	add	b
	adi	4
	jmp	cont7

notcyp:	add	c
cont7:	mov	c,a
	db	0edh, 078h		; in	a,(c)
	pop	b
	ret

; Output byte from port specified by offset
;
;   putout
;   db 	'4'		- port offset

putout:	db	08h			; ex af,af' - Save char to send
	xthl				; ex hl, (sp)
	mov	a,m			; ld	a,(hl)
	inx	h
	xthl				; ex	(sp),hl
	push	b
	mov	c,a
	lda	baseport
	mov	b,a
	lda	computer
	cpi	cypher
	mov	a,b
	jnz	notcy2

	mov	a,c			; translate for cypher odd byte
	sui	4			; addressing
	db	0cbh, 027h		; sla	a
	add	b
	adi	4
	jmp	cont8

notcy2:	add	c
cont8:	mov	c,a
	db	08h			; ex af,af' - Save char to send
	db	0edh, 079h		; out	(c),a
	pop	b
	ret

; delay d*(.5ms)

dly:	push	psw
	push	b		; save
	mov	b,d		; # of .5 ms.
	lda	computer
	cpi	sorcerer
	jnz	dly3

; T cycles = 7 + 65*(4+12) +7 of inner loop

dly1:	mvi	c,65			; 7      | must add to .5ms
dly2:	dcr	c			; 4	 |
	db	20h	; jr	nz,dly2 ; 12/7   |
	db	0fdh
	db	10h	; djnz	dly1	; 13 (b<>0), 8 (b=0)
	db	0f9h
	pop	b
	pop	psw
	ret

dly3:	mvi	c,124			; 7	 | must add to 0.5ms
dly4:	dcr	c			; 4	 |
	db	20h	; jr	nz,dly4 ; 12/7   |
	db	0fdh
	db	010h	; djnz	dly3	; 13 (b<>0), 8 (b=0)
	db	0f9h
	pop	b
	pop	psw
	ret

; Send A out to the terminal

type:	push	b
	mov	e,a
	mvi	c,6
	call	bdos
	pop	b
	ret

; ------------------------->>>>  Data Segment  <<<<---------------------------

	dseg

computer:db	00		; type of computer in use.
baseport:db	00		; holds 8255 baseport address
address: db	00		; current output port address for remote board
inout:	 db	00		; Current direction of Port C
temp:	 dw	0000		; temporarily holds BC

	end
