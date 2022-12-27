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
 * $Log:	vm_types.h,v $
 * Revision 2.4  89/03/09  20:23:06  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:40:11  gm0w
 * 	Changes for cleanup.
 * 
 *  8-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Copy of Vax version.  Works for ns32000 as well.
 *
 * 13-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	File:	vm_types.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	Header file for VM data types.  VAX version.
 */

#ifndef	_MACH_SQT_VM_TYPES_H_
#define _MACH_SQT_VM_TYPES_H_

#ifdef	ASSEMBLER
#else	ASSEMBLER
typedef	unsigned long	vm_offset_t;
typedef	unsigned long	vm_size_t;
#endif	ASSEMBLER
#endif	_MACH_SQT_VM_TYPES_H_
