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
 * $Log:	syscall_sw.h,v $
 * Revision 2.4  89/03/09  20:21:50  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:39:04  gm0w
 * 	Changes for cleanup.
 * 
 * 24-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Complete rewrite due to different calling sequences inside and
 *	outside kernel.
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	MMax version -- modified from vax version.
 *
 *  1-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created from mach_syscalls.h in the user library sources.
 *
 */

#ifndef	_MACH_MMAX_SYSCALL_SW_H_
#define _MACH_MMAX_SYSCALL_SW_H_

/*
 *	This code must convert between the user procedure call protocol
 *	(all parameters on stack) and the kernel procedure call protocol
 *	(first two parameters in registers).  Just to make life interesting,
 *	r0 is reserved for the trap code number.  In hopes of optimizing
 *	calls with few arguments we put the first two arguments into r1 and
 *	r2 (in that order); the ACALL handler in locore.s gets them from
 *	there.  The remaining arguments are to be found starting at 12(sp)
 *	since the rsb address and first two arguments must be skipped at
 *	4 bytes each.
 */

#define kernel_trap(trap_name, trap_number, number_args) \
	.globl	_/**/trap_name ;\
_/**/trap_name: \
	movd	8(sp),r2	;\
	movd	4(sp),r1	;\
	addr	trap_number,r0	;\
	svc	;\
	ret	$0

#endif	_MACH_MMAX_SYSCALL_SW_H_
