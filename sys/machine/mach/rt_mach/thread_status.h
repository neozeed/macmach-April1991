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
 * Revision 2.4  89/03/09  20:18:54  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:12:45  gm0w
 * 	Changes for cleanup.
 * 
 * 29-Jan-88  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Further modifications for the new thread status interface.
 *
 *  8-Jan-88  David Golub (dbg) at Carnegie-Mellon University
 *	Added flavor code for new thread status interface.
 *
 * 29-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Updated register layout for APC/SGP compatibility.
 *
 * 01-Apr-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 *	File:	mach/ca/thread_status.h
 *	Author:	Avadis Tevanian, Jr.
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for the thread
 *	state as applied to ROMP processors.
 *
 */

#ifndef	_MACH_CA_THREAD_STATUS_H_
#define _MACH_CA_THREAD_STATUS_H_

/*
 *	Two structures are defined:
 *
 *	romp_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	romp_saved_state	this structure corresponds to the state
 *				of the user registers as saved on the
 *				stack upon kernel entry.  This structure
 *				is used internally only.  Since this
 *				structure may change from version to
 *				version, it is hidden from the user.
 */

#define ROMP_THREAD_STATE	1
#define ROMP_EXCEPT_STATE	2
#define ROMP_FLOAT_STATE	3

struct romp_thread_state {
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
	int	r12;
	int	r13;
	int	r14;
	int	r15;
	int	iar;		/* user's instruction address register */
	int	icscs;		/* user's ICSCS*/
	int	mq;
};

/*
 *	We also export the hardware generated exception packets
 *	which occur on various types of exceptions for the RT.
 *
 *	Consult the IBMRT Technical Reference Manual for full
 *	documentation about the uses of these fields.
 */

struct romp_except_state {
	int	ecr_count;	/* exception count */
	int 	ex1_ctl;	/* exception 1 control */
	int	ex1_addr;	/* exception 1 address */
	int 	ex1_data;	/* exception 1 data (valid if store) */
	int	ex1_rsv;	/* reserved */	
	int	ex2_ctl;	/* exception 2 control */
	int	ex2_addr;	/* exception 2 address */
	int	ex2_data;	/* exception 2 data (valid if store) */
	int	ex2_rsv;	/* reserved */
	int	mcspcs;		/* machine-check status/program-check status*/
	int	info;		/* valid for certain machine-dependent */
				/* values of mcspcs */
};

/*
 *	Rather than define yet another floating point
 *	union, we will use the one defined in "float.h".  We
 *	add the float_type field to allow the kernel to specify to the
 *	user what floating point register type is contained in the
 *	floatsave structure.  The values for float_type are
 *	one of (FLOAT_MC881 | FLOAT_FPA | FLOAT_AFPA | FLOAT_EMUL) which
 *	are defined in "machine/float.h".  
 */

#include <mach/ca/float.h>

struct romp_float_state {
    	int 	float_type;	/* what type of floating point the following*/
				/* structure contains. */
	struct  floatsave fpregs;
				/* this is a union of the various types */
				/* of floating point used on the APC/RT. */
};

#define ROMP_THREAD_STATE_COUNT	(sizeof(struct romp_thread_state)/sizeof(int))
#define ROMP_EXCEPT_STATE_COUNT (sizeof(struct romp_except_state)/sizeof(int))
#define ROMP_FLOAT_STATE_COUNT 	(sizeof(struct romp_float_state)/sizeof(int))

#ifdef	KERNEL
struct romp_saved_state {
	int	r0;
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
	int	r12;
	int	r13;
	int	r14;
	int	r15;
	int	iar;
	int	mq;
	int	ecr_count;	/* exception count */
	int 	ex1_ctl;	/* exception 1 control */
	int	ex1_addr;	
	int 	ex1_data;
	int	ex1_rsv;	
	int	ex2_ctl;
	int	ex2_addr;
	int	ex2_data;
	int	ex2_rsv;
	int	ex_gsr1;
	int	ex_rep1;
	int	ex_gsr2;
	int	ex_rep2;
	int	mcspcs;
	int	info;
	int	icscs;		/* user's ICSCS*/
};
#define USER_REGS(thread)	(((struct romp_saved_state *)	\
					((thread)->kernel_stack	\
					 + KERNEL_STACK_SIZE)	\
					) - 1)
#endif	KERNEL
#endif	_MACH_CA_THREAD_STATUS_H_
