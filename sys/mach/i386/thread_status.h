/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	thread_status.h,v $
 * Revision 1.3  89/03/09  20:19:59  rpd
 * 	More cleanup.
 * 
 * Revision 1.2  89/02/26  13:01:07  gm0w
 * 	Changes for cleanup.
 * 
 * 24-Feb-89  Robert Baron (rvb) at Carnegie-Mellon University
 *	May need some work.
 */

/*
 *	File:	thread_status.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for the thread
 *	state as applied to I386 processors.
 */

#ifndef	_MACH_I386_THREAD_STATUS_H_
#define _MACH_I386_THREAD_STATUS_H_

/*
 *	Two structures are defined:
 *
 *	i386_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	i386_saved_state	this structure corresponds to the state
 *				of the user registers as saved on the
 *				stack upon kernel entry.  This structure
 *				is used internally only.  Since this
 *				structure may change from version to
 *				version, it is hidden from the user.
 */

#define i386_THREAD_STATE	(1)
				/* only one set of registers */

struct i386_thread_state {
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	esp;
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;
	unsigned int	eip;
	unsigned int	cs;
	unsigned int	efl;
	unsigned int	uesp;
	unsigned int	ss;
};

#define i386_THREAD_STATE_COUNT	(sizeof(struct i386_thread_state)/sizeof(unsigned int))

#ifdef	KERNEL
struct i386_saved_state {
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	esp;
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;
	unsigned int	trapno;
	unsigned int	err;
	unsigned int	eip;
	unsigned int	cs;
	unsigned int	efl;
	unsigned int	uesp;
	unsigned int	ss;
};

/* This macro is probably not right... must be fixed ... wsn */

#define USER_REGS(thread)	(((struct i386_saved_state *)	\
					((thread)->kernel_stack	\
					 + KERNEL_STACK_SIZE)	\
					) - 1)
#endif	KERNEL
#endif	_MACH_I386_THREAD_STATUS_H_
