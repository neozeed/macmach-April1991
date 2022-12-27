/*
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	thread_status.h,v $
 * Revision 2.3  90/03/08  23:11:05  af
 * 	Added single-stepping interface, via setstatus of the exception
 * 	flavor.  Sounded like an "exception" state is the closest thing
 * 	to a vax with a T bit set.  Added relevant defs.
 * 	[89/12/13            af]
 * 
 * Revision 2.2  89/07/14  15:26:37  rvb
 * 	Added floating point flavor, fixed mips_thread_state to export
 * 	the pc and hi/lo registers.
 * 	[89/07/07            af]
 * 
 * Revision 2.1  89/05/30  16:55:47  rvb
 * Created.
 * 
 *  3-Jan-89  Alessandro Forin (af) at Carnegie-Mellon University
 *	Created.
 */

/*
 *	File:	mips/thread_status.h
 *
 *	This file contains the structure definitions for the thread
 *	state as applied to Mips processors.
 *
 */

#ifndef	_MACH_MIPS_THREAD_STATE_
#define	_MACH_MIPS_THREAD_STATE_

/*
 *	Two basic structures are defined:
 *
 *	mips_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	mips_saved_state	this structure corresponds to the state
 *				of the user registers as saved on the
 *				stack upon kernel entry.  This structure
 *				is used internally only.  Since this
 *				structure may change from version to
 *				version, it is hidden from the user.
 *
 * 	Other definitions (flavors) cover alternate register sets as
 *	needed.  For instance, for the FP coprocessor.
 *	On MIPS, we also define an "exception" flavor which is used
 *	for getting more status information about exceptions and for
 *	putting a thread into special exceptional conditions such as
 *	single-stepping (since there is no T bit on MIPS ).
 */

#define	MIPS_THREAD_STATE	(1)
#define MIPS_FLOAT_STATE	(2)
#define	MIPS_COPROC_STATE	(3)
#define MIPS_EXC_STATE		(4)

struct mips_thread_state {
	int	r1;		/* at:  assembler temporary */
	int	r2;		/* v0:  return value 0 */
	int	r3;		/* v1:  return value 1 */
	int	r4;		/* a0:  argument 0 */
	int	r5;		/* a1:  argument 1 */
	int	r6;		/* a2:  argument 2 */
	int	r7;		/* a3:  argument 3 */
	int	r8;		/* t0:  caller saved 0 */
	int	r9;		/* t1:  caller saved 1 */
	int	r10;		/* t2: caller saved 2 */
	int	r11;		/* t3: caller saved 3 */
	int	r12;		/* t4: caller saved 4 */
	int	r13;		/* t5: caller saved 5 */
	int	r14;		/* t6: caller saved 6 */
	int	r15;		/* t7: caller saved 7 */
	int	r16;		/* s0: callee saved 0 */
	int	r17;		/* s1: callee saved 1 */
	int	r18;		/* s2: callee saved 2 */
	int	r19;		/* s3: callee saved 3 */
	int	r20;		/* s4: callee saved 4 */
	int	r21;		/* s5: callee saved 5 */
	int	r22;		/* s6: callee saved 6 */
	int	r23;		/* s7: callee saved 7 */
	int	r24;		/* t8: code generator 0 */
	int	r25;		/* t9: code generator 1 */
	int	r26;		/* k0: kernel temporary 0 */
	int	r27;		/* k1: kernel temporary 1 */
	int	r28;		/* gp: global pointer */
	int	r29;		/* sp: stack pointer */
	int	r30;		/* fp: frame pointer */
	int	r31;		/* ra: return address */
	int	mdlo;		/* low mult result */
	int	mdhi;		/* high mult result */
	int	pc;		/* user-mode PC */
};

#define	MIPS_THREAD_STATE_COUNT	(sizeof(struct mips_thread_state)/sizeof(int))

struct mips_coproc_state {
	int	r0;	/* 32 general coprocessor registers */
	int	r1;
	int	r2;
	int	r3;
	int	r4;
	int	r5;
	int	r6;
	int	r7;
	int	r8;
	int	r9;
	int	r10;
	int	r11;
	int	r12;
	int	r13;
	int	r14;
	int	r15;
	int	r16;
	int	r17;
	int	r18;
	int	r19;
	int	r20;
	int	r21;
	int	r22;
	int	r23;
	int	r24;
	int	r25;
	int	r26;
	int	r27;
	int	r28;
	int	r29;
	int	r30;
	int	r31;
	int	csr;	/* status register */
	int	esr;	/* exception status register */
};

#define	MIPS_COPROC_STATE_COUNT	(sizeof(struct mips_coproc_state)/sizeof(int))

#define mips_float_state	mips_coproc_state
#define		eir		esr	/* exception instruction reg */

#define	MIPS_FLOAT_STATE_COUNT	(sizeof(struct mips_float_state)/sizeof(int))


struct mips_exc_state {
	unsigned	cause;		/* machine-level trap code */
#define EXC_SST		0x00000044
	unsigned	address;	/* last invalid virtual address */
	unsigned	coproc_state;	/* which coprocessors thread used */
#define MIPS_STATUS_USE_COP0	1	/* (by definition) */
#define MIPS_STATUS_USE_COP1	2	/* FPA */
#define MIPS_STATUS_USE_COP2	4
#define MIPS_STATUS_USE_COP3	8
};

#define	MIPS_EXC_STATE_COUNT	(sizeof(struct mips_exc_state)/sizeof(int))


#ifdef	KERNEL
struct mips_saved_state {
	int	arg0;		/* arg save for c calling seq */
	int	arg1;		/* arg save for c calling seq */
	int	arg2;		/* arg save for c calling seq */
	int	arg3;		/* arg save for c calling seq */
	int	r1;		/* at:  assembler temporary */
	int	r2;		/* v0:  return value 0 */
	int	r3;		/* v1:  return value 1 */
	int	r4;		/* a0:  argument 0 */
	int	r5;		/* a1:  argument 1 */
	int	r6;		/* a2:  argument 2 */
	int	r7;		/* a3:  argument 3 */
	int	r8;		/* t0:  caller saved 0 */
	int	r9;		/* t1:  caller saved 1 */
	int	r10;		/* t2: caller saved 2 */
	int	r11;		/* t3: caller saved 3 */
	int	r12;		/* t4: caller saved 4 */
	int	r13;		/* t5: caller saved 5 */
	int	r14;		/* t6: caller saved 6 */
	int	r15;		/* t7: caller saved 7 */
	int	r16;		/* s0: callee saved 0 */
	int	r17;		/* s1: callee saved 1 */
	int	r18;		/* s2: callee saved 2 */
	int	r19;		/* s3: callee saved 3 */
	int	r20;		/* s4: callee saved 4 */
	int	r21;		/* s5: callee saved 5 */
	int	r22;		/* s6: callee saved 6 */
	int	r23;		/* s7: callee saved 7 */
	int	r24;		/* t8: code generator 0 */
	int	r25;		/* t9: code generator 1 */
	int	r26;		/* k0: kernel temporary 0 */
	int	r27;		/* k1: kernel temporary 1 */
	int	r28;		/* gp: global pointer */
	int	r29;		/* sp: stack pointer */
	int	r30;		/* fp: frame pointer */
	int	r31;		/* ra: return address */
	int	sr;		/* status register */
	int	mdlo;		/* low mult result */
	int	mdhi;		/* high mult result */
	int	badvaddr;	/* bad virtual address */
	int	cause;		/* cause register */
	int	pc;		/* program counter */
};


#define	USER_REGS(_th)	(((struct mips_saved_state *)	\
					((_th)->kernel_stack	\
					 + KERNEL_STACK_SIZE)	\
					) - 1)
#define	USER_REG(x)		((unsigned int*)USER_REGS(current_thread()))[(x)]

#endif	KERNEL

#endif	_MACH_MIPS_THREAD_STATE_
