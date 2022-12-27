/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	trap.h,v $
 * Revision 2.2  90/08/30  11:03:32  bohman
 * 	Created.
 * 	[90/08/29  12:00:35  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/trap.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_TRAP_H_
#define _MAC2_TRAP_H_

/*
 * Trap type values
 */
#define TRAP_BAD_ACCESS		0
#define TRAP_TRACE		1
#define TRAP_EMULATION_1010	2
#define TRAP_EMULATION_1111	3
#define TRAP_BAD_INSTRUCTION	4
#define TRAP_PRIV_INSTRUCTION	5
#define TRAP_BREAKPOINT		6
#define TRAP_ARITHMETIC		7
#define TRAP_SOFTWARE		8
#define TRAP_ERROR		9

#endif	_MAC2_TRAP_H_
