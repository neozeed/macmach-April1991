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
 * Revision 2.4  89/03/09  20:24:50  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:41:56  gm0w
 * 	Changes for cleanup.
 * 
 *  1-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created from mach_syscalls.h in the user library sources.
 *
 */

#ifndef	_MACH_VAX_SYSCALL_SW_H_
#define _MACH_VAX_SYSCALL_SW_H_

#define kernel_trap(trap_name, trap_number, number_args) \
	.globl	_/**/trap_name; \
	.align	2; \
_/**/trap_name: \
	.word	0; \
	chmk	$trap_number; \
	ret

#endif	_MACH_VAX_SYSCALL_SW_H_
