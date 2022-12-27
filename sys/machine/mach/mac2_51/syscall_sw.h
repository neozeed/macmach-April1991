/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_sw.h,v $
 * Revision 2.2  90/08/30  17:51:32  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mach/mac2/syscall_sw.h
 */

#ifndef	_MACH_MAC2_SYSCALL_SW_H_
#define _MACH_MAC2_SYSCALL_SW_H_

#define kernel_trap(trap_name, trap_number, number_args) \
	.globl	_##trap_name; \
 _##trap_name: \
	pea	trap_number; \
	trap	\#0; \
	rts

#endif	_MACH_MAC2_SYSCALL_SW_H_
