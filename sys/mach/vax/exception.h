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
 * $Log:	exception.h,v $
 * Revision 2.4  89/03/09  20:24:40  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:41:46  gm0w
 * 	Changes for cleanup.
 * 
 *  2-Nov-87  David Golub (dbg) at Carnegie-Mellon University
 *	Created.
 *
 */

/*
 *	Codes and subcodes for vax exceptions.
 */

/*
 *	EXC_BAD_INSTRUCTION
 */

#ifndef	_MACH_VAX_EXCEPTION_H_
#define _MACH_VAX_EXCEPTION_H_

#define EXC_VAX_PRIVINST		1
#define EXC_VAX_RESOPND			2
#define EXC_VAX_RESADDR			3
#define EXC_VAX_COMPAT			4

/*
 *	COMPAT subcodes
 */
#define EXC_VAX_COMPAT_RESINST		0
#define EXC_VAX_COMPAT_BPT		1
#define EXC_VAX_COMPAT_IOT		2
#define EXC_VAX_COMPAT_EMT		3
#define EXC_VAX_COMPAT_TRAP		4
#define EXC_VAX_COMPAT_RESOP		5
#define EXC_VAX_COMPAT_ODDADDR		6

/*
 *	EXC_ARITHMETIC
 */

#define EXC_VAX_INT_OVF			1
#define EXC_VAX_INT_DIV			2
#define EXC_VAX_FLT_OVF_T		3
#define EXC_VAX_FLT_DIV_T		4
#define EXC_VAX_FLT_UND_T		5
#define EXC_VAX_DEC_OVF			6

#define EXC_VAX_FLT_OVF_F		8
#define EXC_VAX_FLT_DIV_F		9
#define EXC_VAX_FLT_UND_F		10

/*
 *	EXC_SOFTWARE
 */

#define EXC_VAX_SUB_RNG			7

/*
 *	EXC_BREAKPOINT
 */

#define EXC_VAX_BPT			1
#define EXC_VAX_TRACE			2

#endif	_MACH_VAX_EXCEPTION_H_
