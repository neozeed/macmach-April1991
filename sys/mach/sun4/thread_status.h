/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	thread_status.h,v $
 * Revision 2.2  89/07/11  13:37:15  jjc
 * 	Created.
 * 	[88/07/03            jjc]
 * 
 */
/*
 *	File:	sun4/thread_status.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for the thread
 *	state as applied to Sun 4 processor (SPARC).
 *
 */

#ifndef	_MACH_SUN4_THREAD_STATUS_H_
#define	_MACH_SUN4_THREAD_STATUS_H_

#include <machine/reg.h>

/*
 *	Two structures are defined:
 *
 *	sun_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	sun_saved_state	this structure corresponds to the state
 *				of the user registers as saved on the
 *				stack upon kernel entry.  This structure
 *				is used internally only.  Since this
 *				structure may change from version to
 *				version, it is hidden from the user.
 */

#define	SUN_THREAD_STATE_REGS	1

struct sun_thread_state {
	int	psr;		/* processor status register */
	int	pc;		/* program counter */
	int	npc;		/* next program counter */
	int	y;		/* the y register */
	int	g1;		/* user global regs */
	int	g2;
	int	g3;
	int	g4;
	int	g5;
	int	g6;
	int	g7;
	int	o0;
	int	o1;
	int	o2;
	int	o3;
	int	o4;
	int	o5;
	int	o6;
	int	o7;
	struct fp_status fp;	/* floating point registers/status */
};
#define	SUN_THREAD_STATE_REGS_COUNT \
			(sizeof(struct sun_thread_state) / sizeof(int))

#ifdef	KERNEL
struct sun_saved_state {
	int	fr_local[8];		/* saved locals */
	int	fr_arg[6];		/* saved arguments [0 - 5] */
	struct frame	*fr_savfp;	/* saved frame pointer */
	int	fr_savpc;		/* saved program counter */
	char	*fr_stret;		/* struct return addr */
	int	fr_argd[6];		/* arg dump area */
	int	psr;		/* processor status register */
	int	pc;		/* program counter */
	int	npc;		/* next program counter */
	int	y;		/* the y register */
	int	g1;		/* user global regs */
	int	g2;
	int	g3;
	int	g4;
	int	g5;
	int	g6;
	int	g7;
	int	o0;
	int	o1;
	int	o2;
	int	o3;
	int	o4;
	int	o5;
	int	o6;
	int	o7;
};

#define sp	o6
#define	USER_REGS(thread)	((thread)->pcb->user_regs)

#endif	KERNEL
/*
 *	The saved floating-point registers are found in the PCB.
 */

#endif	_MACH_SUN4_THREAD_STATUS_H_
