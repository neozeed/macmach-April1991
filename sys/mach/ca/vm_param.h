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
 * $Log:	vm_param.h,v $
 * Revision 2.4  89/03/09  20:19:01  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:12:51  gm0w
 * 	Changes for cleanup.
 * 
 * 23-May-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	KERNEL_STACK_SIZE -> 8K.
 *
 * 11-Jun-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Created (a while ago) from Avie's vax version.
 *
 */
/*
 *	File:	mach/ca/vm_param.h
 *	Author:	William J. Bolosky
 *	Copyright (C) 1986, William J. Bolosky
 *
 *	imbrt machine dependent virtual memory parameters.
 *
 */

#ifndef	_MACH_CA_VM_PARAM_H_
#define _MACH_CA_VM_PARAM_H_

#define BYTE_SIZE	8	/* byte size in bits */

#define INTSTACK_SIZE	512 * 3	/* This is not used anywhere, since there is
				   no interrupt stack.  But there might be 
				   someday.
				 */

#define VM_MIN_ADDRESS	((vm_offset_t) 0)
#define VM_MAX_ADDRESS	((vm_offset_t) 0xe0000000)

#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t) 0xe0000000)
#define VM_MAX_KERNEL_ADDRESS	((vm_offset_t) 0xf0000000)

#define KERNEL_STACK_SIZE	(8*1024)

#endif	_MACH_CA_VM_PARAM_H_
