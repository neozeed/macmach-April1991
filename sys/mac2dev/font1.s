/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	font1.s,v $
 * Revision 2.2  90/08/30  11:06:09  bohman
 * 	Created.
 * 	[90/08/29  12:45:48  bohman]
 * 
 */

/*
 * (C) 1986 UniSoft Corp. of Berkeley CA
 *
 * UniPlus Source Code. This program is proprietary
 * with Unisoft Corporation and is not to be reproduced
 * or used in any manner except as authorized in
 * writing by Unisoft.
 */
 #
 #	Warning!!! ----> this only runs on 68020s ......
 #
	
#define	ffp	a6@(8)
#define	x	a6@(12)
#define	y	a6@(16)
#define	c	a6@(20)

#define firstChar	2
#define lastChar	4


#define	font_pnt 	0
#define	font_height 	4
#define	font_width 	8
#define	font_leading 	12
#define	font_rowwords 	16
#define	font_bitimage 	20
#define	font_loctable 	24
#define	font_owtable 	28
#define	font_linewidth 	32
#define	font_maxx 	36
#define	font_maxy 	40
#define	font_inverse 	44
#define	font_screen 	48
#define	font_theight 	52
#define	font_offset 	56


#define	video_addr	0
#define	video_mem_x	4
#define	video_mem_y	8
#define	video_scr_x	12
#define	video_scr_y	16

	.globl font_char
	.globl _font_char
font_char:
_font_char:
	link	a6, #0
	moveml	#0x3f38, sp@-
	movl	ffp, a3		/* font pointer */
	movl	a3@(font_pnt), a2	/* fontrec pointer */
	movl	c, d0			/* character to output */
	cmpw	a2@(firstChar),d0	/* make sure it is in range */
	bge	L1
bad:	clrl	d0			/* return a 0 on an error */
	moveml	sp@+, #0x1cfc
	unlk	a6
	rts
L1:	cmpw	a2@(lastChar),d0
	bgt	bad
	movl	x, d1			/* now test the x position for
					   validity */
	blt	bad
	cmpl	a3@(font_maxx),d1
	bge	bad
	movl	y, d2			/* and then the y */
	blt	bad
	cmpl	a3@(font_maxy),d2
	bge	bad
	subw	a2@(firstChar), d0	/* adjust the character index */
	movl	a3@(font_loctable), a2	/* find the font offset */
	lea	a2@(0,d0:W:2), a2
	clrl	d3
	movw	a2@, d3
	clrl	d4			/* get the width (if 0 return) */
	movw	a2@(2), d4
	subl	d3, d4
	bgt	cnt			/* only valid 0 length is ' ' */
	blt	bad
	movl	c, d0
	cmpb	#0x20,d0
	bne	bad
cnt:	movl	a3@(font_width), d7	/* get the output width */
	mulu	d7, d1		/* get the screen index */
	mulul	a3@(font_linewidth), d2
	addl	d1, d2
	addl	a1@(font_offset), d2	/* get the pixel offset from the start*/
	movl	a3@(font_screen), a4	/* get the screen base */
	movl	a4@(video_mem_x), a2	/* get the line increment */
	movl	a4@(video_addr), a4
	movl	a3@(font_rowwords), a1	/* font row width */
	movl	a3@(font_bitimage), a0	/* font info pointer */
	movl	a3@(font_inverse), d5
	movl	d7, d6		/* get the center offset */
	subl	d4, d6
	blt	chk
	lsrl	#1, d6
	bra	cont
chk:	clrl	d6
cont:	movl	a3@(font_height), d1	/* loop for every line */
	subql	#1, d1
 	tstl	d4
	beq	blank			/* blank is a special case */
loop:
		bfextu	a0@{d3:d4}, d0 /* get the character row */
		addl	a1, d3	/* increment the row index */
 		lsll	d6, d0	/* center the character */
		tstb	d5
		bne	inv
		notl	d0		/* invert it if required */
inv:
		bfins	d0, a4@{d2:d7} /* insert it in the bitmap */
		addl	a2, d2	/* move to the next line */
	dbra	d1, loop		/* loop through each row */
	movl	a3@(font_leading), d1	/* do the last empty rows */
	beq	xit
blankit:
	subql	#1, d1
	tstb	d5			/* get the fill value */
	bne	invL
	movl	#0xffffffff, d0
	bra	loopL
invL:
	clrl	d0
loopL:
		bfins	d0, a4@{d2:d7} /* insert it */
		addl	a2, d2
	dbra	d1, loopL
xit:	movl	#1, d0			/* return 1 */
	moveml	sp@+, #0x1cfc
	unlk	a6
	rts
blank:	
	addl	a3@(font_leading), d1	/* just fill with background */
	addql	#1, d1
	bra	blankit

/*
 *	Code to do an upward scroll
 */
#define	ffq	a6@(8)
#define	top	a6@(12)
#define	bottom	a6@(16)
#define	ll	a6@(20)
	.globl	font_scrollup
	.globl	_font_scrollup
font_scrollup:
_font_scrollup:
	link	a6, #0
	moveml	#0x3f3c, sp@-
	movl	ffq, a1			/* Font pointer */
	movl	bottom, d0			/* Last line */
	blt	retn
	cmpl	a1@(font_maxy),d0		/* Check it against the screen*/
	blt	nxt				/*	bounds */
	movl	a1@(font_maxy), d0
	subql	#1, d0
nxt:
	addql	#1, d0				/* Add one to get next line */
	movl	d0, d3
	movl	ll, d1				/* Line count */
	ble	retn
	subl	top, d3			/* Get the scroll height */
	ble	retn
	cmpl	d3, d1			/* Set the line count to a max*/
	ble	nxt2				/*	of the scroll height */
	movl	d3, d1
nxt2:

/* d1 = scroll count (ll) */
/* d0 = bottom line (bottom) */
/* d3 = window width (bottom - top) */

	movl	a1@(font_screen), a0		/* Get the screen info */

	movl	a0@(video_addr), a2		/* get the screen address */

	movl	d3, d4			/* get a count of the number of */
	subl	d1, d4			/* scanlines to be shifted up */
	mulul	a1@(font_theight), d4

	movl	a1@(font_width), d5		/* calc the number of pixels/line */
	mulul	a1@(font_maxx), d5

	movl	a1@(font_offset), d7		/* get the pixel offset from the start*/

	movl	d7, d6			/* figure it out in bytes */
	asrl	#5, d6	
	asll	#2, d6

	addl	d6, a2			/* adjust the screen pointer */

	andl	#0x1f, d7			/* get the inc in bits */

	movl	d7, d2			/* figure out how far to the first */
	subl	#32, d2			/* longword boundary */
	negl	d2
	movl	d2, a3

	movl	d5, d6			/* figure out how many longs to move */
	subl	a3, d6
	lsrl	#5, d6
	subql	#1, d6

	movl	a1@(font_offset), d0		/* see how much remainder is left */
	addl	d5, d0
	andl	#0x1f, d0
	movl	d0, a5

	movl	a0@(video_mem_x), d0		/* now see how many bytes to the next */
	subl	d5, d0			/* line */
	addl	a5, d0
	subl	d7, d0
	asrl	#3, d0

	movl	top, d2			/* calculate the byte address of the */
	mulul	a1@(font_linewidth), d2	/* top of the screen */
	asrl	#3, d2
	movl	d2, a4
	addl	a2, a4

	subql	#1, d4				/* any lines to shift ? */
	ble	fill_up				/* nope, then proceed to filling */

	movl	d1, d2			/* get the byte address of the */
	addl	top, d2
	mulul	a1@(font_linewidth), d2	/* topmost valid data */
	asrl	#3, d2
	addl	d2, a2

loop90:						/* loop copying data up */

		movl	a3, d5		/* if there is less than a longword at */
		beq	skip91			/*    the beginning, copy it up */

			bfextu	a2@{d7:d5}, d2
			bfins	d2, a4@{d7:d5}

			addql	#4, a4		/* increment the addresses */
			addql	#4, a2
skip91:

		movl	d6, d5		/* loop copying out longs */
loop91:
			movl	a2@+, a4@+

		dbra	d5, loop91

		movl	a5, d5		/* if there is some left - copy it out */
		beq	skip92

			bfextu	a2@{#0:d5}, d2
			bfins	d2, a4@{#0:d5}
skip92:

		addl	d0, a2		/* move the byte address to the next */
		addl	d0, a4		/* 	lines */

	dbra	d4, loop90

fill_up:
	tstl	a1@(font_inverse)		/* get the fill value */
	bne	skip93
	movl	#0xffffffff, d2
	bra	skip94
skip93:	clrl	d2
skip94:

	movl	d1, d4			/* find out how many scan lines to fill*/
	mulul	a1@(font_theight), d4
	subql	#1, d4

loop95:						/* loop filling them */

		movl	a3, d5		/* do the first bit */
		beq	skip96

			bfins	d2, a4@{d7:d5}
			addql	#4, a4
skip96:
		movl	d6, d5		/* loop moving longs */
loop97:
			movl	d2, a4@+
		dbra	d5, loop97

		movl	a5, d5		/* do the last bit */
		beq	skip98

			bfins	d2, a4@{#0:d5}
skip98:
		addl	d0, a4		/* increment the byte address */
	dbra	d4, loop95


retn:	moveml	sp@+, #0x3cfc
	unlk	a6
	rts

/*
 *	Code to do a downward scroll
 */
	.globl	font_scrolldown
	.globl	_font_scrolldown
_font_scrolldown:
font_scrolldown:
	link	a6, #0
	moveml	#0x3f3c, sp@-
	movl	ffq, a1		/* font pointer */
	movl	top, d0		/* start line */
	blt	retn2
	movl	bottom, d3		/* ending line */
	cmpl	d3, d0
	bgt	retn2
	addql	#1, d3			/* actaually make it the next one */
	movl	ll, d1			/* line count */
	ble	retn2
	movl	d3, bottom		/* save it for later */
	subl	d0, d3		/* check the length */
	cmpl	d3, d1
	blt	nxt4
	movl	d3, d1		/* make it the max */
	beq	retn2
nxt4:
/* d1 = scroll count (ll) */
/* d0 = bottom line (bottom) */
/* d3 = window width (bottom - top) */

	movl	a1@(font_screen), a0		/* Get the screen info */

	movl	a0@(video_addr), a2		/* get the screen address */

	movl	d3, d4			/* get a count of the number of */
	subl	d1, d4			/* scanlines to be shifted down */
	mulul	a1@(font_theight), d4

	movl	a1@(font_width), d5		/* calc the number of pixels/line */
	mulul	a1@(font_maxx), d5

	movl	a1@(font_offset), d7		/* get the pixel offset from the start*/

	movl	d7, d6			/* figure it out in bytes */
	asrl	#5, d6	
	asll	#2, d6

	addl	d6, a2			/* adjust the screen pointer */

	andl	#0x1f, d7			/* get the inc in bits */

	movl	d7, d2			/* figure out how far to the first */
	subl	#32, d2			/* longword boundary */
	negl	d2
	movl	d2, a3

	movl	d5, d6			/* figure out how many longs to move */
	subl	a3, d6
	lsrl	#5, d6
	subql	#1, d6

	movl	a1@(font_offset), d0		/* see how much remainder is left */
	addl	d5, d0
	andl	#0x1f, d0
	movl	d0, a5

	movl	a0@(video_mem_x), d0 		/* now see how many bytes to the next */
	negl	d0				/* line */
	subl	d5, d0
	addl	a5, d0
	subl	d7, d0
	asrl	#3, d0

	movl	bottom, d2			/* calculate the byte address of the */
	mulul	a1@(font_linewidth), d2	/* bottom of the screen */
	subl	a0@(video_mem_x), d2
	asrl	#3, d2
	movl	d2, a4
	addl	a2, a4

	subql	#1, d4				/* any lines to shift ? */
	ble	fill_down			/* nope, then proceed to filling */

	movl	bottom, d2			/* get the byte address of the */
	subl	d1, d2
	mulul	a1@(font_linewidth), d2	/* bottommost valid data */
	subl	a0@(video_mem_x), d2
	asrl	#3, d2
	addl	d2, a2

loop80:						/* loop copying data up */

		movl	a3, d5		/* if there is less than a longword at */
		beq	skip81			/*    the beginning, copy it up */

			bfextu	a2@{d7:d5}, d2
			bfins	d2, a4@{d7:d5}

			addql	#4, a4		/* increment the addresses */
			addql	#4, a2
skip81:

		movl	d6, d5		/* loop copying out longs */
loop81:
			movl	a2@+, a4@+

		dbra	d5, loop81

		movl	a5, d5		/* if there is some left - copy it out */
		beq	skip82

			bfextu	a2@{#0:d5}, d2
			bfins	d2, a4@{#0:d5}
skip82:

		addl	d0, a2		/* move the byte address to the next */
		addl	d0, a4		/* 	lines */

	dbra	d4, loop80

fill_down:
	tstl	a1@(font_inverse)		/* get the fill value */
	bne	skip83
	movl	#0xffffffff, d2
	bra	skip84
skip83:	clrl	d2
skip84:

	movl	d1, d4			/* find out how many scan lines to fill*/
	mulul	a1@(font_theight), d4
	subql	#1, d4

loop85:						/* loop filling them */

		movl	a3, d5		/* do the first bit */
		beq	skip86

			bfins	d2, a4@{d7:d5}
			addql	#4, a4
skip86:
		movl	d6, d5		/* loop moving longs */
loop87:
			movl	d2, a4@+
		dbra	d5, loop87

		movl	a5, d5		/* do the last bit */
		beq	skip88

			bfins	d2, a4@{#0:d5}
skip88:
		addl	d0, a4		/* increment the byte address */
	dbra	d4, loop85


retn2:	moveml	sp@+, #0x3cfc
	unlk	a6
	rts

/*
 *	erase the whole screen
 */
	.globl	font_clear
font_clear:
	link	a6, #0
	moveml	#0x3f3c, sp@-
	movl	ffq, a1			/* Font pointer */

	movl	a1@(font_screen), a0		/* Get the screen info */

	movl	a0@(video_addr), a2		/* get the screen address */

	movl	a1@(font_width), d5		/* calc the number of pixels/line */
	mulul	a1@(font_maxx), d5

	movl	a1@(font_offset), d7		/* get the pixel offset from the start*/

	movl	d7, d6			/* figure it out in bytes */
	asrl	#5, d6	
	asll	#2, d6

	addl	d6, a2			/* adjust the screen pointer */

	andl	#0x1f, d7			/* get the inc in bits */

	movl	d7, d2			/* figure out how far to the first */
	subl	#32, d2			/* longword boundary */
	negl	d2
	movl	d2, a3

	movl	d5, d6			/* figure out how many longs to move */
	subl	a3, d6
	lsrl	#5, d6
	subql	#1, d6

	movl	a1@(font_offset), d0		/* see how much remainder is left */
	addl	d5, d0
	andl	#0x1f, d0
	movl	d0, a5

	movl	a0@(video_mem_x), d0		/* now see how many bytes to the next */
	subl	d5, d0			/* line */
	addl	a5, d0
	subl	d7, d0
	asrl	#3, d0

	movl	a1@(font_maxy), d4		/* find out how many scan lines to fill*/

_font_clear:					
/*
 *	a1 = font structure pointer
 *	a2 = pointer to current screen byte
 *	d0 = bytes per line
 *	d4 = scan lines to fill	
 *	d7 = the inc in bits	
 */
	tstl	a1@(font_inverse)
	bne	inv5
	movl	#0xffffffff, d2
	bra	lll
inv5:	clrl	d2
lll:
	mulul	a1@(font_theight), d4

loop75:						/* loop filling them */

		movl	a3, d5		/* do the first bit */
		beq	skip76

			bfins	d2, a2@{d7:d5}
			addql	#4, a2
skip76:
		movl	d6, d5		/* loop moving longs */
loop77:
			movl	d2, a2@+
		dbra	d5, loop77

		movl	a5, d5		/* do the last bit */
		beq	skip78

			bfins	d2, a2@{#0:d5}
skip78:
		addl	d0, a2		/* increment the byte address */
	dbra	d4, loop75


	moveml	sp@+, #0x3cfc
	unlk	a6
	rts

/*
 *	invert the whole screen
 */
	.globl	font_invertall
font_invertall:
	link	a6, #0
	moveml	#0x3f3c, sp@-
	movl	ffq, a1			/* Font pointer */

	movl	a1@(font_screen), a0		/* Get the screen info */

	movl	a0@(video_addr), a2		/* get the screen address */

	movl	a1@(font_width), d5		/* calc the number of pixels/line */
	mulul	a1@(font_maxx), d5

	movl	a1@(font_offset), d7		/* get the pixel offset from the start*/

	movl	d7, d6			/* figure it out in bytes */
	asrl	#5, d6	
	asll	#2, d6

	addl	d6, a2			/* adjust the screen pointer */

	andl	#0x1f, d7			/* get the inc in bits */

	movl	d7, d2			/* figure out how far to the first */
	subl	#32, d2			/* longword boundary */
	negl	d2
	movl	d2, a3

	movl	d5, d6			/* figure out how many longs to move */
	subl	a3, d6
	lsrl	#5, d6
	subql	#1, d6

	movl	a1@(font_offset), d0		/* see how much remainder is left */
	addl	d5, d0
	andl	#0x1f, d0
	movl	d0, a5

	movl	a0@(video_mem_x), d0		/* now see how many bytes to the next */
	subl	d5, d0			/* line */
	addl	a5, d0
	subl	d7, d0
	asrl	#3, d0

	movl	a1@(font_maxy), d4		/* find out how many scan lines to fill*/
	mulul	a1@(font_theight), d4

loop65:						/* loop filling them */

		movl	a3, d5		/* do the first bit */
		beq	skip66

			bfextu	a2@{d7:d5}, d2
			notl	d2
			bfins	d2, a2@{d7:d5}
			addql	#4, a2
skip66:
		movl	d6, d5		/* loop moving longs */
loop67:
			movl	a2@, d2
			notl	d2
			movl	d2, a2@+
		dbra	d5, loop67

		movl	a5, d5		/* do the last bit */
		beq	skip68

			bfextu	a2@{#0:d5}, d2
			notl	d2
			bfins	d2, a2@{#0:d5}
skip68:
		addl	d0, a2		/* increment the byte address */
	dbra	d4, loop65


	moveml	sp@+, #0x3cfc
	unlk	a6
	rts

#define	xs	a6@(12)
#define	ys	a6@(16)

/*
 *	font_invert(fp, x, y)	invert a character
 */

	.globl	font_invert
	.globl	_font_invert
font_invert:
_font_invert:
	link	a6, #0
	moveml	#0x3000, sp@-
	movl	ffp, a1
	movl	ys, d0
	mulul	a1@(font_linewidth), d0
	movl	xs, d1
	movl	a1@(font_width), d2
	mulul	d2, d1
	addl	d1, d0
	addl	a1@(font_offset), d0
	movl	a1@(font_screen), a0
	movl	a0@(video_mem_x), d1
	movl	a0@(video_addr), a0
	movl	a1@(font_theight), d3
	subql	#1, d3
loop5:
		bfchg	a0@{d0:d2}
		addl	d1, d0
		dbra	d3, loop5
	moveml	sp@+, #0x000c
	unlk	a6
	rts
