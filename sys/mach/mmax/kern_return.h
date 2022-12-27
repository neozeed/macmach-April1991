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
 * $Log:	kern_return.h,v $
 * Revision 2.4  89/03/09  20:21:45  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:38:59  gm0w
 * 	Changes for cleanup.
 * 
 *  5-Mar-87  David L. Black (dlb) at Carnegie-Mellon University
 *	Added #ifndef ASSEMBLER for inclusion in assembly language files.
 *
 * 14-Oct-85  Michael Wayne Young (mwyoung) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	File:	mach/mmax/kern_return.h
 *	Author:	Avadis Tevanian, Jr., Michael Wayne Young
 *
 *	Machine-dependent kernel return definitions.
 */

#ifndef	_MACH_MMAX_KERN_RETURN_H_
#define _MACH_MMAX_KERN_RETURN_H_

#ifndef	ASSEMBLER
typedef	int		kern_return_t;
#endif	ASSEMBLER
#endif	_MACH_MMAX_KERN_RETURN_H_
