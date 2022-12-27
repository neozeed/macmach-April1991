/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vm_param.h,v $
 * Revision 2.2  89/07/11  13:37:25  jjc
 * 	Created from Sun 3 version.
 * 	[88/06/03            jjc]
 * 
 */
#ifndef	_MACH_SUN4_VM_PARAM_H_
#define _MACH_SUN4_VM_PARAM_H_

#include <machine/machparam.h>
#include <mon/sunromvec.h>

#define BYTE_SIZE	8	/* byte size in bits */

#define VM_MIN_ADDRESS	((vm_offset_t)0)
#define VM_MAX_ADDRESS	((vm_offset_t)KERNELBASE)

#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t)KERNELBASE)
#define VM_MAX_KERNEL_ADDRESS	((vm_offset_t)MONSTART)

#define sun_round_page(x) ((((unsigned)(x)) + NBPG - 1) & ~(NBPG-1))
#define sun_trunc_page(x) ((unsigned)(x) & ~(NBPG-1))

#define	KERNEL_STACK_SIZE	(NBPG)	/* must be at least one SUN page */
#define INTSTACK_SIZE		(3*512)
#endif	_MACH_SUN4_VM_PARAM_H_
