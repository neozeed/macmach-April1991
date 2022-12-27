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
 * $Log:	thread.h,v $
 * Revision 2.2  90/08/30  11:03:16  bohman
 * 	Created.
 * 	[90/08/29  11:59:21  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/thread.h
 */

#ifndef	_MAC2_THREAD_H_
#define	_MAC2_THREAD_H_

#ifdef	KERNEL
extern	struct	u_address	U_ADDRESS;
#define	u			U_ADDRESS
#endif	KERNEL

#endif	_MAC2_THREAD_H_
