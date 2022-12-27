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
 * Revision 2.5  89/03/09  20:21:39  rpd
 * 	More cleanup.
 * 
 * Revision 2.4  89/02/25  18:38:54  gm0w
 * 	Changes for cleanup.
 * 
 * 14-Mar-88  Joseph Boykin (boykin) at Encore Computer Corporation
 *	Added floating point Operand error (type 7).  Clean up.
 *
 * 29-Sep-87  David Black (dlb) at Carnegie-Mellon University
 *	Created.
 *
 */

/*
 *	Codes and subcodes for ns32000 exceptions.
 */

#ifndef	_MACH_MMAX_EXCEPTION_H_
#define _MACH_MMAX_EXCEPTION_H_

/*
 *	EXC_BAD_INSTRUCTION
 */

#define EXC_NS32K_FPU			3
#define EXC_NS32K_ILL			4
#define EXC_NS32K_UND			10

/*
 *	EXC_ARITHMETIC
 */

#define EXC_NS32K_FPU			3
#define EXC_NS32K_DVZ			6

/*
 *	FPU subcodes
 */

#define EXC_NS32K_FPU_UNDERFLOW 	1
#define EXC_NS32K_FPU_OVERFLOW		2
#define EXC_NS32K_FPU_DVZ		3
#define EXC_NS32K_FPU_ILLEGAL 		4
#define EXC_NS32K_FPU_INVALID		5
#define EXC_NS32K_FPU_INEXACT		6
#define EXC_NS32K_FPU_OPERAND		7

/*
 *	EXC_SOFTWARE
 */

#define EXC_NS32K_FLG			7

/*
 *	EXC_BREAKPOINT
 */

#define EXC_NS32K_BPT			8
#define EXC_NS32K_TRC			9

#endif	_MACH_MMAX_EXCEPTION_H_
