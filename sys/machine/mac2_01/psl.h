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
 * $Log:	psl.h,v $
 * Revision 2.2  90/08/30  11:02:51  bohman
 * 	Created.
 * 	[90/08/29  11:44:23  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/psl.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_PSL_H_
#define _MAC2_PSL_H_

/*
 * Definition of bits in the 68020/30
 * status register.
 */
#define	SR_T1		0x8000		/* trace on any instruction */
#define	SR_T0		0x4000		/* trace on change of flow */
#define	SR_SUPR		0x2000		/* supervisor mode */
#define	SR_MASTER	0x1000
#define	SR_IPL		0x0700		/* interrupt priority bits */
#define SR_IPL7		0x0700
#define SR_IPL6		0x0600
#define SR_IPL5		0x0500
#define SR_IPL4		0x0400
#define SR_IPL3		0x0300
#define SR_IPL2		0x0200
#define SR_IPL1		0x0100
#define SR_IPL0		0x0000
#define	SR_CC		0x001F		/* all condition code bits */
#define SR_X		0x0010		/* extended carry */
#define SR_N		0x0008		/* negative */
#define SR_Z		0x0004		/* zero */
#define SR_V		0x0002		/* overflow */
#define SR_C		0x0001		/* carry */

#define	SR_HIGH		0x2700		/* supervisor mode high pri */
#define	SR_LOW		0x2000		/* supervisor mode low pri */
#define	SR_USER		0x0000		/* user mode low priority */
#define	SR_USERCLR	0xFF00		/* system bits */
#define	SR_TRACE	SR_T1		/* trace mode mask - use T1 (any) */

#define	PSL_USERSET	SR_USER		/* set for user */
#define	PSL_USERCLR	SR_USERCLR	/* clear for user */
#define	PSL_ALLCC	SR_CC		/* condition codes */
#define	PSL_T		SR_TRACE	/* trace bit */

#endif	_MAC2_PSL_H_
