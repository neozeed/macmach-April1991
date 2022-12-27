/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	exception.h,v $
 * Revision 2.3  90/02/19  15:31:08  jjc
 * 	Added more exceptions.
 * 	[89/07/24            jjc]
 * 
 * Revision 2.2  89/07/11  13:37:05  jjc
 *      Created.
 *      [88/07/03            jjc]
 * 
 */
/*
 *	Machine dependent exception definitions for Sun 4
 */
#ifndef	_MACH_SUN4_EXCEPTION_H_
#define _MACH_SUN4_EXCEPTION_H_

/*
 *	EXC_BAD_INSTRUCTION
 */
#define	EXC_SUN4_UNIMPLEMENTED		0x2
#define	EXC_SUN4_PRIVILEGE_VIOLATION	0x3
#define	EXC_SUN4_WINDOW_OVERFLOW	0x5
#define	EXC_SUN4_ALIGNMENT		0x7

/*
 *	EXC_ARITHMETIC
 */
#define EXC_SUN4_FLT_EXCEPTION		0x8
#define EXC_SUN4_ZERO_DIVIDE		0x82

/*
 *	EXC_EMULATION
 */
#define	EXC_SUN4_TAG_OVERFLOW		0xa

/*
 *	EXC_BREAKPOINT
 */
#define	EXC_SUN4_BREAKPOINT	0x81	/* T_BREAKPOINT from sun4/trap.h */

#endif	_MACH_SUN4_EXCEPTION_H_
