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
 * Revision 2.4  89/03/09  20:25:08  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:42:11  gm0w
 * 	Changes for cleanup.
 * 
 * 23-Apr-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed things to "unsigned int" to appease the user community :-).
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
 *	Header file for VM data types.  VAX version.
 */

#ifndef	_MACH_VAX_VM_TYPES_H_
#define _MACH_VAX_VM_TYPES_H_

#ifdef	ASSEMBLER
#else	ASSEMBLER
typedef	unsigned int	vm_offset_t;
typedef	unsigned int	vm_size_t;
#endif	ASSEMBLER
#endif	_MACH_VAX_VM_TYPES_H_
