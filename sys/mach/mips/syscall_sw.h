/*
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	syscall_sw.h,v $
 * Revision 2.2  89/11/20  11:34:11  mja
 * 	Changed include of <regdef.h> to mips/asm.h
 * 	[89/11/10            mrt]
 * 
 * Revision 2.1  89/05/30  16:55:46  rvb
 * Created.
 * 
 * 12-Jan-89  Alessandro Forin (af) at Carnegie-Mellon University
 *	Created for Mips.
 *
 *  1-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created from mach_syscalls.h in the user library sources.
 */

#ifndef	_MACH_MIPS_SYSCALL_SW_H_
#define	_MACH_MIPS_SYSCALL_SW_H_	1

#include <mips/asm.h>

/*
 * The Unix kernel expects arguments to be passed with the normal C calling
 * sequence, and v0 should contain the system call number.
 * On Mach we pass all the arguments in registers, the trap number is in v0
 * and the return value is placed in v0.  There are no awful hacks for
 * returning multiple values from a trap.
 *
 * Performance: a trap with up to 4 args takes 4 cycles in user mode,
 * with an unfortunate and unavoidable nop instruction and no memory
 * accesses. Any arg after the fourth takes 1 more cycle to load
 * from the cache (which cannot possibly miss) into a register.
 */

/*
 * A simple trap is one with up to 4 args. Args are passed to us
 * in registers, and we keep them there.
 */
#define simple_kernel_trap(trap_name, trap_number)	 \
	.globl	trap_name;	 			 \
	.ent	trap_name,0;				 \
trap_name:;						 \
	.frame	sp,0,ra;				 \
	li	v0,trap_number;				 \
	syscall;					 \
	j	ra;					 \
	.end trap_name

#define kernel_trap_0(trap_name,trap_number)		 \
	simple_kernel_trap(trap_name,trap_number)
#define kernel_trap_1(trap_name,trap_number)		 \
	simple_kernel_trap(trap_name,trap_number)
#define kernel_trap_2(trap_name,trap_number)		 \
	simple_kernel_trap(trap_name,trap_number)
#define kernel_trap_3(trap_name,trap_number)		 \
	simple_kernel_trap(trap_name,trap_number)
#define kernel_trap_4(trap_name,trap_number)		 \
	simple_kernel_trap(trap_name,trap_number)

/*
 * A trap with more than 4 args requires popping of args
 * off the stack, where they are placed by the compiler.
 */
#define kernel_trap_5(trap_name, trap_number)	 	 \
	.globl	trap_name; 				 \
	.ent	trap_name,0;				 \
trap_name:;						 \
	.frame	sp,0,ra;				 \
	lw	t0,16(sp);				 \
	li	v0,trap_number;				 \
	syscall;					 \
	j	ra;					 \
	.end trap_name

#define kernel_trap_6(trap_name, trap_number)	 	 \
	.globl	trap_name; 				 \
	.ent	trap_name,0;				 \
trap_name:;						 \
	.frame	sp,0,ra;				 \
	lw	t0,16(sp);				 \
	lw	t1,20(sp);				 \
	li	v0,trap_number;				 \
	syscall;					 \
	j	ra;					 \
	.end trap_name

/*
 * There are no Mach traps with more than 6 args.
 * If that changes just add more macros, using
 * registers t2-t7.  And fix the kernel.
 */

#define kernel_trap(trap_name,trap_number,nargs)	 \
	kernel_trap_/**/nargs(trap_name,trap_number)

#endif	_MACH_MIPS_SYSCALL_SW_H_
