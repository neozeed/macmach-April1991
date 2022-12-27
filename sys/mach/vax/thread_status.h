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
 * Revision 2.4  89/03/09  20:24:55  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:42:01  gm0w
 * 	Changes for cleanup.
 * 
 * 28-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Added flavor code for new thread_status interface.
 *
 * 27-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 *	File:	vax/thread_status.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for the thread
 *	state as applied to VAX processors.
 */

#ifndef	_MACH_VAX_THREAD_STATUS_H_
#define _MACH_VAX_THREAD_STATUS_H_

/*
 *	Two structures are defined:
 *
 *	vax_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	vax_saved_state		this structure corresponds to the state
 *				of the user registers as saved on the
 *				stack upon kernel entry.  This structure
 *				is used internally only.  Since this
 *				structure may change from version to
 *				version, it is hidden from the user.
 */

#define VAX_THREAD_STATE	(1)
				/* only one set of registers */

struct vax_thread_state {
	int	r0;		/* general registers */
	int	r1;
	int	r2;
	int	r3;
	int	r4;
	int	r5;
	int	r6;
	int	r7;
	int	r8;
	int	r9;
	int	r10;
	int	r11;
	int	ap;		/* user's arg pointer */
	int	fp;		/* user's frame pointer */
	int	sp;		/* user's stack pointer */
	int	pc;		/* user's program counter */
	int	ps;		/* user's processor status longword */
};

#define VAX_THREAD_STATE_COUNT	(sizeof(struct vax_thread_state)/sizeof(int))

#ifdef	KERNEL
struct vax_saved_state {
	int	ap;		/* user's arg pointer */
	int	fp;		/* user's frame pointer */
	int	pad0;		/* saved pc after calls */
	int	r0;		/* general registers */
	int	r1;
	int	r2;
	int	r3;
	int	r4;
	int	r5;
	int	r6;
	int	r7;
	int	r8;
	int	r9;
	int	r10;
	int	r11;
	int	nargs;		/* calls saves nargs here */
	int	sp;		/* user's stack pointer */
	int	pad1;
	int	pad2;
	int	pc;		/* user's program counter */
	int	ps;		/* user's processor status longword */
};

#define USER_REGS(thread)	(((struct vax_saved_state *)	\
					((thread)->kernel_stack	\
					 + KERNEL_STACK_SIZE)	\
					) - 1)
#endif	KERNEL
#endif	_MACH_VAX_THREAD_STATUS_H_
