/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vm_types.h,v $
 * Revision 2.2  89/07/11  13:37:31  jjc
 * 	Copied for Sun 4.
 * 	[89/06/30            jjc]
 * 
 * 13-Jun-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Copied RT version for Sun 3.
 *
 * 13-Jun-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 *	File:	vm_types.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1985, Avadis Tevanian, Jr.
 *
 *	Header file for VM data types.  Sun version.
 *
 */
#ifndef	_MACH_SUN4_VM_TYPES_H_
#define _MACH_SUN4_VM_TYPES_H_

#ifndef	ASSEMBLER
#ifndef	_VM_TYPES_MACHINE_
#define	_VM_TYPES_MACHINE_	1

typedef	unsigned long	vm_offset_t;
typedef	unsigned long	vm_size_t;

#endif	_VM_TYPES_MACHINE_
#endif	ASSEMBLER

#endif	_MACH_SUN4_VM_TYPES_H_
