/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_sw.h,v $
 * Revision 2.2  90/02/19  15:11:48  jjc
 * 	Created.
 * 	[89/07/23            jjc]
 * 
 */
#ifndef	_MACH_SUN4_SYSCALL_SW_H_
#define _MACH_SUN4_SYSCALL_SW_H_

#define kernel_trap(trap_name, trap_number, number_args) \
	.globl	_/**/trap_name; \
_/**/trap_name: \
	mov	trap_number, %g1; \
	ta	0; \
	retl; \
	nop;

#endif	_MACH_SUN4_SYSCALL_SW_H_
