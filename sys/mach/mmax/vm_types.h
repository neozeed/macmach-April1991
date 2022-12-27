/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vm_types.h,v $
 * Revision 2.4  89/03/09  20:22:12  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:39:21  gm0w
 * 	Changes for cleanup.
 * 
 * 13-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */
/*
 *
 *	File:	mach/mmax/vm_types.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Header file for VM data types.  Multimax version.
 */

#ifndef	_MACH_MMAX_VM_TYPES_H_
#define _MACH_MMAX_VM_TYPES_H_

#ifdef	ASSEMBLER
#else	ASSEMBLER
typedef	unsigned long	vm_offset_t;
typedef	unsigned long	vm_size_t;
#endif	ASSEMBLER
#endif	_MACH_MMAX_VM_TYPES_H_
