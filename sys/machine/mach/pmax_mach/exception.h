/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	exception.h,v $
 * Revision 2.1  89/05/30  16:55:45  rvb
 * Created.
 * 
 *  3-Jan-89  Alessandro Forin (af) at Carnegie-Mellon University
 *	Created.
 */

#ifndef	_MACH_MIPS_EXCEPTION_H_
#define	_MACH_MIPS_EXCEPTION_H_

/*
 *	Codes and subcodes for Mips exceptions.
 */


/*
 *	Hardware level exceptions
 */

#define	EXC_MIPS_INT		0	/* interrupt */
#define	EXC_MIPS_MOD		1	/* TLB mod */
#define	EXC_MIPS_RMISS		2	/* Read TLB Miss */
#define	EXC_MIPS_WMISS		3	/* Write TLB Miss */
#define	EXC_MIPS_RADE		4	/* Read Address Error */
#define	EXC_MIPS_WADE		5	/* Write Address Error */
#define	EXC_MIPS_IBE		6	/* Instruction Bus Error */
#define	EXC_MIPS_DBE		7	/* Data Bus Error */
#define	EXC_MIPS_SYSCALL	8	/* SYSCALL */
#define	EXC_MIPS_BREAK		9	/* BREAKpoint */
#define	EXC_MIPS_II		10	/* Illegal Instruction */
#define	EXC_MIPS_CPU		11	/* CoProcessor Unusable */
#define	EXC_MIPS_OV		12	/* OVerflow */

/*
 *	Software exception codes
 */
#define	EXC_MIPS_SOFT_SEGV	16	/* Software detected seg viol */
#define	EXC_MIPS_SOFT_CPU	19	/* coprocessor unusable */


/*
 *	Bad instruction subcodes
 */

#define	EXC_MIPS_PRIVINST		1
#define	EXC_MIPS_RESOPND		2
#define	EXC_MIPS_RESADDR		3

/*
 *	EXC_ARITHMETIC subcodes
 */

#define	EXC_MIPS_FLT_UNIMP	1
#define	EXC_MIPS_FLT_INVALID	2
#define	EXC_MIPS_FLT_DIVIDE0	3
#define	EXC_MIPS_FLT_OVERFLOW	4
#define	EXC_MIPS_FLT_UNDERFLOW	5
#define	EXC_MIPS_FLT_INEXACT	6

/*
 *	EXC_BREAKPOINT subcodes
 */

#define	EXC_MIPS_BPT			1
#define EXC_MIPS_TRACE			2


#endif	_MACH_MIPS_EXCEPTION_H_
