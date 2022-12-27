/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vm_param.h,v $
 * Revision 2.7  90/07/03  16:39:06  mrt
 * 	Make VM_MIN_KERNEL_SPACE 64Meg on APC.
 * 	[90/05/21            dlb]
 * 
 * Revision 2.6  89/03/10  02:18:01  rpd
 * 	When not KERNEL, make MMAX_APC be true by default.
 * 
 * Revision 2.5  89/03/09  20:22:05  rpd
 * 	More cleanup.
 * 
 * Revision 2.4  89/02/25  18:39:16  gm0w
 * 	Changes for cleanup.
 * 
 * 12-Apr-88  David Black (dlb) at Carnegie-Mellon University
 *	Modify variable kernel address changes to elilminate impact on
 *	machine-independent code.
 *
 * 19-Feb-88  Joseph Boykin (boykin) at Encore Computer Corporation
 *	Added definition of VM_MIN_KERNEL_SPACE (minimum virtual address space)
 *	we'll accept as a boot parameter.
 *
 * 21-May-87  David Kirschen (kirschen) at Encore Computer Corporation
 *      Changes for NS32332 (APC)
 *
 *  6-Apr-87  David Black (dlb) at Carnegie-Mellon University
 *	KERNEL_STACK_SIZE --> 8k because they're now kmem_alloc'd.
 *
 * 28-Apr-86  Jim Van Sciver (jvs) at Encore
 *	Modified for the Multimax.
 *
 * 20-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	File:	mach/mmax/vm_param.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	NS32000 machine dependent virtual memory parameters.
 *	Most of the declarations are preceeded by NS32K_ (or ns32k_)
 *	which is OK because only Multimax specific code will be using
 *	them.  In almost all cases the declarations are the same as
 *	the VAX.  The names were changed to catch VAX parameter dependencies.
 */

#ifndef	_MACH_MMAX_VM_PARAM_H_
#define _MACH_MMAX_VM_PARAM_H_

#ifdef	KERNEL
#include <mmax_apc.h>
#else	KERNEL
#ifndef	MMAX_APC
#define MMAX_APC	1
#endif	MMAX_APC
#endif	KERNEL

#include <mach/mmax/vm_types.h>

#define BYTE_SIZE	8	/* byte size in bits */

#if	MMAX_APC
#define NS32K_PGBYTES  4096     /* bytes per ns32k page */
#define NS32K_PGSHIFT    12     /* number of bits to shift for pages */
#else	MMAX_APC
#define NS32K_PGBYTES	512	/* bytes per ns32k page */
#define NS32K_PGSHIFT	9	/* number of bits to shift for pages */
#endif	MMAX_APC

/*
 *	Convert bytes to pages and convert pages to bytes.
 *	No rounding is used.
 */

#define ns32k_btop(x)		(((unsigned)(x)) >> NS32K_PGSHIFT)
#define ns32k_ptob(x)		((char *)(((unsigned)(x)) << NS32K_PGSHIFT))

/*
 *	Round off or truncate to the nearest page.  These will work
 *	for either addresses or counts.  (i.e. 1 byte rounds to 1 page
 *	bytes.
 */

#define ns32k_round_page(x)	((((unsigned)(x)) + NS32K_PGBYTES - 1) & \
					~(NS32K_PGBYTES-1))
#define ns32k_trunc_page(x)	(((unsigned)(x)) & ~(NS32K_PGBYTES-1))


/*	VM_MAX_KERNEL_ADDRESS does not allow the SCC to be mapped.
 */
#if	MMAX_APC
#define VM_MIN_ADDRESS	((vm_offset_t) 0)
#define VM_MAX_ADDRESS	((vm_offset_t) 0x7fffe000)
#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t) 0x0)
vm_offset_t	vm_max_kernel_address;
#define VM_MAX_KERNEL_ADDRESS	vm_max_kernel_address
#define VM_MIN_KERNEL_SPACE	((vm_offset_t) 0x4000000)
#else	MMAX_APC
#define VM_MIN_ADDRESS	((vm_offset_t) 0)
#define VM_MAX_ADDRESS	((vm_offset_t) 0x1000000)
#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t) 0x0)
vm_offset_t	vm_max_kernel_address;
#define VM_MAX_KERNEL_ADDRESS	vm_max_kernel_address
#define VM_MIN_KERNEL_SPACE	((vm_offset_t) 0xfc0000)
#endif	MMAX_APC

#define KERNEL_STACK_SIZE	(8*1024)
#define INTSTACK_SIZE		(3*512)

/*
 *	Conversion between NS32000 pages and VM pages
 */

#define trunc_ns32k_to_vm(p)	(atop(trunc_page(ns32k_ptob(p))))
#define round_ns32k_to_vm(p)	(atop(round_page(ns32k_ptob(p))))
#define vm_to_ns32k(p)		(ns32k_btop(ptoa(p)))

#endif	_MACH_MMAX_VM_PARAM_H_
