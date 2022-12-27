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
 * Revision 2.4  89/03/09  20:25:02  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:42:06  gm0w
 * 	Changes for cleanup.
 * 
 * 16-Jan-87  David Golub (dbg) at Carnegie-Mellon University
 *	Made vax_ptob return 'unsigned' instead of caddr_t.
 *
 * 20-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 *	File:	vax/vm_param.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	VAX machine dependent virtual memory parameters.
 *	Most of the declarations are preceeded by VAX_ (or vax_)
 *	which is OK because only Vax specific code will be using
 *	them.
 */

#ifndef	_MACH_VAX_VM_PARAM_H_
#define _MACH_VAX_VM_PARAM_H_

#define BYTE_SIZE	8	/* byte size in bits */

#define VAX_PGBYTES	512	/* bytes per vax page */
#define VAX_PGSHIFT	9	/* number of bits to shift for pages */

/*
 *	Convert bytes to pages and convert pages to bytes.
 *	No rounding is used.
 */

#define vax_btop(x)		(((unsigned)(x)) >> VAX_PGSHIFT)
#define vax_ptob(x)		(((unsigned)(x)) << VAX_PGSHIFT)

/*
 *	Round off or truncate to the nearest page.  These will work
 *	for either addresses or counts.  (i.e. 1 byte rounds to 1 page
 *	bytes.
 */

#define vax_round_page(x)	((((unsigned)(x)) + VAX_PGBYTES - 1) & \
					~(VAX_PGBYTES-1))
#define vax_trunc_page(x)	(((unsigned)(x)) & ~(VAX_PGBYTES-1))

#define VM_MIN_ADDRESS	((vm_offset_t) 0)
#define VM_MAX_ADDRESS	((vm_offset_t) 0x80000000)

#define VM_MIN_KERNEL_ADDRESS	((vm_offset_t) 0x80000000)
#define VM_MAX_KERNEL_ADDRESS	((vm_offset_t) 0xffffffff)

#define KERNEL_STACK_SIZE	(8*512)	/* XXX we don't use the stack yet */
#define INTSTACK_SIZE		(3*512)		/* interrupt stack size */

/*
 *	Conversion between VAX pages and VM pages
 */

#define trunc_vax_to_vm(p)	(atop(trunc_page(vax_ptob(p))))
#define round_vax_to_vm(p)	(atop(round_page(vax_ptob(p))))
#define vm_to_vax(p)		(vax_btop(ptoa(p)))

#endif	_MACH_VAX_VM_PARAM_H_
