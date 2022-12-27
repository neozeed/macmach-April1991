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
 *	File: mac2emul/macuser.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2/asm.h>
#include <mac2/cpu.h>

#include <assym.s>

ENTRY(fetch_user_1_byte)
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	_active_threads,a1
	movl	sp@(4),a0
	moveq	#0,d0
	movl	#fsuerr,a1@(THREAD_RECOVER)
	movsb	a0@,d1
	clrl	a1@(THREAD_RECOVER)
	movb	d1,sp@(8)@(0)
	rts

ENTRY(fetch_user_2_byte)
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	_active_threads,a1
	movl	sp@(4),a0
	moveq	#0,d0
	movl	#fsuerr,a1@(THREAD_RECOVER)
	movsw	a0@,d1
	clrl	a1@(THREAD_RECOVER)
	movw	d1,sp@(8)@(0)
	rts

ENTRY(fetch_user_4_byte)
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	_active_threads,a1
	movl	sp@(4),a0
	moveq	#0,d0
	movl	#fsuerr,a1@(THREAD_RECOVER)
	movsl	a0@,d1
	clrl	a1@(THREAD_RECOVER)
	movl	d1,sp@(8)@(0)
	rts

ENTRY(store_user_1_byte)
	movl	#FC_UD,d0
	movc	d0,dfc
	movl	_active_threads,a1
	moveq	#0,d0
	movl	sp@(4),a0
	movb	sp@(8)@(0),d1
	movl	#fsuerr,a1@(THREAD_RECOVER)
	movsb	d1,a0@
	clrl	a1@(THREAD_RECOVER)
	rts

ENTRY(store_user_2_byte)
	movl	#FC_UD,d0
	movc	d0,dfc
	movl	_active_threads,a1
	moveq	#0,d0
	movl	sp@(4),a0
	movw	sp@(8)@(0),d1
	movl	#fsuerr,a1@(THREAD_RECOVER)
	movsw	d1,a0@
	clrl	a1@(THREAD_RECOVER)
	rts

ENTRY(store_user_4_byte)
	movl	#FC_UD,d0
	movc	d0,dfc
	movl	_active_threads,a1
	moveq	#0,d0
	movl	sp@(4),a0
	movl	sp@(8)@(0),d1
	movl	#fsuerr,a1@(THREAD_RECOVER)
	movsl	d1,a0@
	clrl	a1@(THREAD_RECOVER)
	rts
	
fsuerr:
	movl	#-1,d0
	clrl	a1@(THREAD_RECOVER)
	rts
