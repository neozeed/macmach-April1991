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
 * Revision 2.4  89/03/09  20:22:49  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:39:54  gm0w
 * 	Changes for cleanup.
 * 
 *  3-Mar-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	Define kernel_trap() for the sequent.
 *	Define auxiliary macros "nsvc#" (where # is the number of args
 *	the svc is passed).  The n in nsvc implies that the call number
 *	is negative and needs to be sign extended.
 *	NB.  profiling stuff is untested.
 *
 *  8-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Copy of Vax version for now.  Needs update to ns32000/balance.
 *
 *  1-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created from mach_syscalls.h in the user library sources.
 *
 */

#ifndef	_MACH_SQT_SYSCALL_SW_H_
#define _MACH_SQT_SYSCALL_SW_H_

#ifdef	PROF
#define ENTRY(x)	.globl _/**/x; .align 2; _/**/x: \
			.data; 1:; .long 0; .text; addr 1b,r0; bsr mcount
#else
#define ENTRY(x)	.globl _/**/x; .align 2; _/**/x:
#endif	PROF


	.globl	cerror

#define nsvc0(name,n) \
	ENTRY(name)\
	enter	[], 0;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[];\
	ret 	0


#define nsvc1(name,n) \
	ENTRY(name)\
	enter	[], 0;\
	movd	8(fp), r1;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[];\
	ret 	0

#define nsvc2(name,n) \
	ENTRY(name)\
	enter	[], 0;\
	movd	8(fp), r1;\
	movd	0xc(fp), r2;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[];\
	ret 	0

#define nsvc3(name,n) \
	ENTRY(name)\
	enter	[r3], 0;\
	movd	8(fp), r1;\
	movd	0xc(fp), r2;\
	movd	0x10(fp), r3;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[r3];\
	ret 	0

#define nsvc4(name,n) \
	ENTRY(name)\
	enter	[r3,r4], 0;\
	movd	8(fp), r1;\
	movd	0xc(fp), r2;\
	movd	0x10(fp), r3;\
	movd	0x14(fp), r4;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[r3,r4];\
	ret 	0

#define nsvc5(name,n) \
	ENTRY(name)\
	enter	[r3,r4,r5], 0;\
	movd	8(fp), r1;\
	movd	0xc(fp), r2;\
	movd	0x10(fp), r3;\
	movd	0x14(fp), r4;\
	movd	0x18(fp), r5;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[r3,r4,r5];\
	ret 	0

#define nsvc6(name,n) \
	ENTRY(name)\
	enter	[r3,r4,r5,r6], 0;\
	movd	8(fp), r1;\
	movd	0xc(fp), r2;\
	movd	0x10(fp), r3;\
	movd	0x14(fp), r4;\
	movd	0x18(fp), r5;\
	movd	0x1c(fp), r6;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[r3,r4,r5,r6];\
	ret 	0

#define nsvc7(name,n) \
	ENTRY(name)\
	enter	[r3,r4,r5,r6,r7], 0;\
	movd	8(fp), r1;\
	movd	0xc(fp), r2;\
	movd	0x10(fp), r3;\
	movd	0x14(fp), r4;\
	movd	0x18(fp), r5;\
	movd	0x1c(fp), r6;\
	movd	0x20(fp), r7;\
	movxbd	n, r0;\
	svc;\
	bfc	1f;\
	bsr	cerror;\
1:	exit	[r3,r4,r5,r6,r7];\
	ret 	0


#define kernel_trap(trap_name, trap_number, number_args) \
	nsvc/**/number_args(trap_name,trap_number)

#endif	_MACH_SQT_SYSCALL_SW_H_
