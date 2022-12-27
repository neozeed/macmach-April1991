/*
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	thread_status.h,v $
 * Revision 2.4  89/03/09  20:21:56  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:39:09  gm0w
 * 	Changes for cleanup.
 * 
 * 12-Feb-88  David Black (dlb) at Carnegie-Mellon University
 *	Added definitions for new status interfaces.
 *
 * 16-Jun-87  David Black (dlb) at Carnegie-Mellon University
 *	Changed from cputypes.h to cpp symbols for balance/multimax.
 *	ns32032 --> ns32000.
 *
 *  2-May-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	Well, the sqt state has one more parameter, the ipl level, sigh.
 *
 *  6-Apr-87  David Black (dlb) at Carnegie-Mellon University
 *	Created based on vax file.
 *
 */
/*
 *	File:	{mmax,sqt}/thread_status.h
 *	Author:	David L. Black
 *
 *	This file contains the structure definitions for the user-visible
 *	thread state as applied to ns32000 processors.  NOTE: this file
 *	is shared between multiple ns32000 implementations (Balance and
 *	Multimax).
 *
 */

#ifndef	_MACH_MMAX_THREAD_STATUS_H_
#define _MACH_MMAX_THREAD_STATUS_H_

/*
 *	Two structures are defined:
 *
 *	ns32000_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	ns32000_saved_state	this structure corresponds to the state
 *				of the user registers as saved on the
 *				stack upon kernel entry.  This structure
 *				is used internally only.  Since this
 *				structure may change from version to
 *				version, it is hidden from the user.
 */

#define NS32000_THREAD_STATE	(1)
				/* only one set of registers */

struct ns32000_thread_state {
	int	r0;
	int	r1;
	int	r2;
	int	r3;
	int	r4;
	int	r5;
	int	r6;
	int	r7;
	int	fp;
	int	sp;
	int	pc;
	short	mod;
	short	psr;
	int	fsr;
	int	f0;
	int	f1;
	int	f2;
	int	f3;
	int	f4;
	int	f5;
	int	f6;
	int	f7;
};

#define NS32000_THREAD_STATE_COUNT \
		(sizeof(struct ns32000_thread_state)/sizeof(int))

#ifdef	KERNEL
/*
 *	NOTE:	Floating point registers are never saved on the kernel
 *		stack.  They have to be altered by hand or in the pcb.
 */
struct ns32000_saved_state {
	int	sp;
	int	r7;
	int	r6;
	int	r5;
	int	r4;
	int	r3;
	int	r2;
	int	r1;
	int	r0;
#ifdef	balance
	int	ipl;
#endif	balance
	int	fp;
	int	pc;
	short	mod;
	short	psr;
};

#endif	KERNEL
#endif	_MACH_MMAX_THREAD_STATUS_H_
