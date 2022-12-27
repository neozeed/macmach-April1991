/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: emul/patches/patches.s
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * patches for macintosh
 * rom code.
 */

_post_mouse:
	moveq	#0,d0
	movb	0x172,d0
	bne	0f
	movl	#1,a0
	bra	1f
0:	movl	#2,a0
1:	movl	#0x30000,d0
	.word	0xa02f		| PostEvent
	rts

	.data
_key_state:
	.long	0

	.text
	.globl	_key_reset
_key_reset:
	clrl	_key_state
	rts

_post_key:
	movl	d3,sp@-
	movl	#0x200,d3
	movb	_newkey,d3
	bsr	0f
	movl	sp@+,d3
	rts

0:
	lea	_KMAP_0,a0
	moveq	#0,d0
	movb	d3,d0
	andb	#0x7f,d0
    	movb	a0@(d0),d0
	orb	d0,d3	
	subql	#4,sp
	pea	_KCHR_0
	moveq	#0,d0
	movw	_modflg,d0
	rolw	#1,d0
	lslw	#8,d0
	movb	d3,d0
	movw	d0,sp@-
	pea	_key_state
	.word	0xa9c3		| KeyTrans
	movw	sp@+,d0
	beq	0f
	bsr	1f
0:	movw	sp@+,d0
	beq	2f
1:	rorw	#8,d0
	swap	d0
	movw	d3,d0
	roll	#8,d0
	tstb	d3
	bmi	3f
	movb	#2,0x218
	movb	#2,0x21e
	movw	#3,a0
	.word	0xa02f		| PostEvent
2:	rts
3:	movw	#4,a0
	bclr	#15,d0
	.word	0xa02f
	rts

    	.lcomm	trash, 8192

    	.data
    	.globl	_VBLtick
_VBLtick:
    	.long	60
    	.text

	.globl	_VBLintr,_VBLvec
_VBLvec:
	.long	_VBLintr
_VBLintr:
	movl	#cache_flush,0x6f4	| patch cache flush vector
	moveml	a0-a3/d0-d3,sp@-
	movb	0xcb2,sp@-
	beq	0f
	moveq	#0,d0
	movl	0xdbc,a0
	jsr	a0@
0:
    	subl	#1,_VBLtick
    	bne	0f
    	lea	trash,a1
	movl	0x192,a0
    	jsr	a0@
	movl	#60,_VBLtick
0:
    	lea	trash,a1
	movl	0x196,a0
    	jsr	a0@

	tstl	0xd94
	beq	0f
	movw	sp@(0x22),d0
	andw	#0x700,d0
	bne	0f
	jsr	0x40806300
0:
	movb	sp@+,d0
	beq	0f
	movl	0xdbc,a0
	jsr	a0@
0:
	moveml	sp@+,a0-a3/d0-d3
	rte

	.globl	_SLOTintr,_SLOTvec
_SLOTvec:
	.long	_SLOTintr
_SLOTintr:
	moveml	a0-a3/d0-d4,sp@-
	movb	0xcb2,sp@-
	beq	0f
	moveq	#0,d0
	movl	0xdbc,a0
	jsr	a0@
0:
	movb	0x50f03e00,d4
	notb	d4
0:
	bfffo	d4{#26:#6},d0
	beq	1f
	bfclr	d4{d0:#1}
	negb	d0
	addb	#40,d0
	movl	0xd28,a0
	jsr	a0@
	bra	0b
1:
	tstl	0xd94
	beq	0f
	movw	sp@(0x26),d0
	andw	#0x700,d0
	bne	0f
	jsr	0x40806300
0:

	movb	sp@+,d0
	beq	0f
	movl	0xdbc,a0
	jsr	a0@
0:
	moveml	sp@+,a0-a3/d0-d4
	rte

    	.globl	_INPintr,_INPvec
_INPvec:
    	.long	_INPintr
_INPintr:
	moveml	a0-a3/d0-d3,sp@-
	movb	0xcb2,sp@-
	beq	0f
	moveq	#0,d0
	movl	0xdbc,a0
	jsr	a0@
0:
	jsr	_input_events
    	tstl	d0
    	beq	0f
	movl	d0,d3
    	btst	#0,d3
    	beq	2f
    	jsr	_post_mouse
2:	btst	#2,d3
	beq	0f
	jsr	_post_key
0:
	movb	sp@+,d0
	beq	0f
	movl	0xdbc,a0
	jsr	a0@
0:
	moveml	sp@+,a0-a3/d0-d3
	rte

    	.globl	_TIM1intr,_TIM1vec
_TIM1vec:
    	.long	_TIM1intr
_TIM1intr:
	moveml	a0-a3/d0-d3,sp@-
	movb	0xcb2,sp@-
	beq	0f
	moveq	#0,d0
	movl	0xdbc,a0
	jsr	a0@
0:
	movl	0x1aa,a0
	jsr	a0@

	movb	sp@+,d0
	beq	0f
	movl	0xdbc,a0
	jsr	a0@
0:
	moveml	sp@+,a0-a3/d0-d3
	rte

    	.globl	_TIM2intr,_TIM2vec
_TIM2vec:
    	.long	_TIM2intr
_TIM2intr:
	moveml	a0-a3/d0-d3,sp@-
	movb	0xcb2,sp@-
	beq	0f
	moveq	#0,d0
	movl	0xdbc,a0
	jsr	a0@
0:
	jsr	_TimeIntr

	movb	sp@+,d0
	beq	0f
	movl	0xdbc,a0
	jsr	a0@
0:
	moveml	sp@+,a0-a3/d0-d3
	rte

	.globl	_SwapMMUMode
_SwapMMUMode:
	lea	swapmmumode,a0
	movl	a0,0xdbc
	movl	a0,0x574
	movl	sp@(8),d0
	jsr	swapmmumode
	movl	d0,sp@(8)
	rts

swapmmumode:
	cmpb	0xcb2,d0
	beq	0f
	movb	0xcb2,sp@-
	trap	#1
	movb	sp@+,d0
0:
	rts

cache_flush:
	trap	#2
	rts

	.globl	_splhigh
_splhigh:
	movw	sr,d0
	orw	#0x700,sr
	andw	#0x700,d0
	rts

	.globl	_splx
_splx:
	movw	sr,d0
	movl	sp@(4),d1
	andw	#0x700,d1
	movw	d1,sr
	andw	#0x700,d0
	rts

	.globl	_stop
_stop:
	movl	sp@(4),d0
	movl	sp@(8),a0
	trap	#15
	rts

	.globl	_RESETvec, _RESET
_RESETvec:
	.long	_start+0x80000
	.long	_RESET
_RESET:
	moveml	d0-d1/a0-a1,sp@-
	jsr	_reset
	moveml	sp@+,d0-d1/a0-a1
	movl	sp@+,sp
	jmp	@(_start+0x80000-4)@(0)

	.globl	_OSvec, _OStrap
_OSvec:
	.long	_OStrap
_OStrap:
	moveml	d0-d1/a0-a1,sp@-
	movl	sp@(16),a1
	moveq	#0,d1
	movw	a1@+,d1
	movl	a1,sp@(16)
	movl	d1,sp@-
	bfclr	d1{#16:#8}
	movl	@(_os_traps:l,d1:W:4),a1
	jsr	a1@
	addqw	#4,sp
	moveml	sp@+,d0-d1/a0-a1
	tstw	d0
	rts

	.globl	_TBOXvec, _TBOXtrap
_TBOXvec:	
	.long	_TBOXtrap
_TBOXtrap:
	trap	#15		| toolbox emulation NYI
	rts
