/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_sw.h,v $
 * Revision 2.5  89/03/09  20:18:48  rpd
 * 	More cleanup.
 * 
 * Revision 2.4  89/02/25  18:12:39  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.3  89/02/07  00:55:30  mwyoung
 * Relocated from ca/syscall_sw.h
 * 
 * Revision 2.2  88/10/11  10:07:49  rpd
 * 	Major modifications to kernel_trap:
 * 	  The _trap_name symbol is defined correctly now,
 * 	  so pointers to system calls should work.
 * 	
 * 	  Now traps pass 6 arguments in registers.
 * 	
 * 	  errno processing is no longer done.
 * 	[88/10/06  12:14:43  rpd]
 * 
 *  1-Apr-87  William Bolosky (bolosky) at Carnegie-Mellon University
 *	Fixed to always pass in 5 parameters (in r2-r6).
 *	If you need to send in more than 5, then you must fix
 *	kernel_trap and also fix ../ca/trap.c.
 *
 *  1-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created from mach_syscalls.h in the user library sources.
 */

#ifndef	_MACH_CA_SYSCALL_SW_H_
#define _MACH_CA_SYSCALL_SW_H_

#define kernel_trap(trap_name, trap_number, number_args)	\
	.set	.oVpcc, 0					\
	.set	.oVncs, 0					\
	.globl	_/**/trap_name;					\
	.globl	_./**/trap_name;				\
								\
	.data;							\
_/**/trap_name:							\
	.long _./**/trap_name;					\
								\
	.text;							\
_./**/trap_name:						\
	ai	r1,r1,-8;					\
	st	r6,0(r1);					\
	st	r7,4(r1);					\
	l	r6,8(r1);					\
	l	r7,12(r1);					\
	svc	trap_number(r0);				\
	l	r6,0(r1);					\
	l	r7,4(r1);					\
	brx	r15;						\
	ai	r1,r1,8;					\
								\
	.ltorg;							\
	.align	2

#endif	_MACH_CA_SYSCALL_SW_H_
