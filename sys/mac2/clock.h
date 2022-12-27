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
 * $Log:	clock.h,v $
 * Revision 2.2  90/08/30  11:01:00  bohman
 * 	Created.
 * 	[90/08/29  11:30:58  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/clock.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_CLOCK_H_
#define _MAC2_CLOCK_H_

/*
 * The delta (in seconds) between MacOS T(0) and U**X T(0)
 */
#define T0_DELTA (((365*(1970-1904))+((1970-1904)/4)+1)*24*60*60)

extern struct actlist	actclock;

#endif _MAC2_CLOCK_H_
