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
 * Revision 2.4  89/03/09  20:23:00  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:40:06  gm0w
 * 	Changes for cleanup.
 * 
 *  8-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Clone MMAX version (essentially ns32000 specific), mods for
 *	Sequent Balance (ns32000).
 *
 * 28-Apr-86  Jim Van Sciver (jvs) at Encore
 *	Modified for the Multimax.
 *
 * 20-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	File:	sqt/vm_param.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	Sequent Balance (ns32000) machine dependent virtual memory parameters.
 *	Most of the declarations are preceeded by NS32K_ (or ns32k_)
 *	which is OK because only Multimax specific code will be using
 *	them.  In almost all cases the declarations are the same as
 *	the VAX.  The names were changed to catch VAX parameter dependencies.
 */

#ifndef	_MACH_SQT_VM_PARAM_H_
#define _MACH_SQT_VM_PARAM_H_

#define BYTE_SIZE	8	/* byte size in bits */

#define NS32K_PGBYTES	512	/* bytes per ns32k page */
#define NS32K_PGSHIFT	9	/* number of bits to shift for pages */

/*
 *	Convert bytes to pages and convert pages to bytes.
 *	No rounding is used.
 */

#define ns32k_btop(x)		(((unsigned)(x)) >> NS32K_PGSHIFT)
#define ns32k_ptob(x)		((caddr_t)(((unsigned)(x)) << NS32K_PGSHIFT))

/*
 *	Round off or truncate to the nearest page.  These will work
 *	for either addresses or counts.  (i.e. 1 byte rounds to 1 page
 *	bytes.
 */

#define ns32k_round_page(x)	((((unsigned)(x)) + NS32K_PGBYTES - 1) & \
					~(NS32K_PGBYTES-1))
#define ns32k_trunc_page(x)	(((unsigned)(x)) & ~(NS32K_PGBYTES-1))

#define VM_MIN_ADDRESS	((vm_offset_t) 0)
#define VM_MAX_ADDRESS	((vm_offset_t) 0x1000000)

#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t) 0x0)
#define VM_MAX_KERNEL_ADDRESS	((vm_offset_t) 0x1000000)

#define KERNEL_STACK_SIZE	(8*1024)
#define INTSTACK_SIZE		(3*512)

/*
 *	Conversion between NS32000 pages and VM pages
 */

#define trunc_ns32k_to_vm(p)	(atop(trunc_page(ns32k_ptob(p))))
#define round_ns32k_to_vm(p)	(atop(round_page(ns32k_ptob(p))))
#define vm_to_ns32k(p)		(ns32k_btop(ptoa(p)))

#endif	_MACH_SQT_VM_PARAM_H_
