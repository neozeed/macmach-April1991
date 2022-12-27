/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rx_clock.h,v $
 * Revision 2.4  90/08/30  11:48:51  bohman
 * 	Removed unused parameter from macro declaration of
 * 	clock_NewTime().
 * 	[90/08/28            bohman]
 * 
 * Revision 2.3  89/06/03  15:38:56  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:24:10  jsb]
 * 
 * Revision 2.2  89/04/22  15:28:42  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

/*
****************************************************************************
*        Copyright IBM Corporation 1988, 1989 - All Rights Reserved        *
*                                                                          *
* Permission to use, copy, modify, and distribute this software and its    *
* documentation for any purpose and without fee is hereby granted,         *
* provided that the above copyright notice appear in all copies and        *
* that both that copyright notice and this permission notice appear in     *
* supporting documentation, and that the name of IBM not be used in        *
* advertising or publicity pertaining to distribution of the software      *
* without specific, written prior permission.                              *
*                                                                          *
* IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL IBM *
* BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY      *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER  *
* IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING   *
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.    *
****************************************************************************
*/

/* Elapsed time package */
/* This package maintains a clock which is independent of the time of day.  It uses the 4.3BSD interval timer (getitimer/setitimer) in TIMER_REAL mode.  Any other use of the timer voids this package's warranty. */

#ifndef _CLOCK_
#define _CLOCK_

#ifdef	KERNEL
#if defined(AFS_AIX_ENV) || defined(AFS_AUX_ENV)
#include <sys/systm.h>
#include <sys/time.h>
#endif	/* System V */
#else	KERNEL
#ifndef ITIMER_REAL
#include <sys/time.h>
#endif	ITIMER_REAL
#endif	KERNEL

/* Some macros to make macros more reasonable (this allows a block to be used within a macro which does not cause if statements to screw up).   That is, you can use "if (...) macro_name(); else ...;" without having things blow up on the semi-colon. */

#ifndef BEGIN
#define BEGIN do {
#define END } while(0)
#endif

/* A clock value is the number of seconds and microseconds that have elapsed since calling clock_Init. */
struct clock {
    long sec;	    /* Seconds since clock_Init */
    long usec;	    /* Microseconds since clock_Init */
};

#ifndef	KERNEL

/* For internal use.  The last value returned from clock_GetTime() */
extern struct clock clock_now;

/* For internal use:  this flag, if set, indicates a new time should be read by clock_getTime() */
extern int clock_haveCurrentTime;

/* For external use: the number of times the clock value is actually updated */
extern int clock_nUpdates;

/* Initialize the clock package */
extern clock_Init();

#define	clock_NewTime()	(clock_haveCurrentTime = 0)

/* Update the value to be returned by gettime */
extern void clock_UpdateTime();

/* Return the current clock time.  If the clock value has not been updated since the last call to clock_NewTime, it is updated now */
#define	clock_GetTime(cv)				\
    BEGIN						\
	if (!clock_haveCurrentTime) clock_UpdateTime(); \
	(cv)->sec = clock_now.sec;			\
	(cv)->usec = clock_now.usec;			\
    END

/* Current clock time, truncated to seconds */
#define	clock_Sec() ((!clock_haveCurrentTime)? clock_UpdateTime(), clock_now.sec:clock_now.sec)

#else	KERNEL

#define clock_Init()
#define clock_GetTime(cv) osi_GetTime((struct timeval *)cv) /* Bogus--uses TOD clock */
#define clock_Sec() osi_Time()
#define	clock_NewTime()    /* don't do anything; clock is fast enough in kernel */

#endif	KERNEL

/* Returns the elapsed time in milliseconds between clock values (*cv1) and (*cv2) */
#define clock_ElapsedTime(cv1, cv2) \
    (((cv2)->sec - (cv1)->sec)*1000 + ((cv2)->usec - (cv1)->usec)/1000)

/* Advance the known value of the current clock time (clock_now) by the specified clock value */
#define clock_Advance(cv) clock_Add(&clock_now, cv)

/* Some comparison operators for clock values */
#define	clock_Gt(a, b)	((a)->sec>(b)->sec || (a)->sec==(b)->sec && (a)->usec>(b)->usec)
#define	clock_Ge(a, b)	((a)->sec>(b)->sec || (a)->sec==(b)->sec && (a)->usec>=(b)->usec)
#define	clock_Eq(a, b)	((a)->sec==(b)->sec && (a)->usec==(b)->usec)
#define	clock_Le(a, b)	((a)->sec<(b)->sec || (a)->sec==(b)->sec && (a)->usec<=(b)->usec)
#define	clock_Lt(a, b)	((a)->sec<(b)->sec || (a)->sec==(b)->sec && (a)->usec<(b)->usec)

/* Is the clock value zero? */
#define	clock_IsZero(c)	((c)->sec == 0 && (c)->usec == 0)

/* Set the clock value to zero */
#define	clock_Zero(c)	((c)->sec = (c)->usec = 0)

/* Add time c2 to time c1.  Both c2 and c1 must be positive times. */
#define	clock_Add(c1, c2)					\
    BEGIN							\
	if (((c1)->usec += (c2)->usec) >= 1000000) {		\
	    (c1)->usec -= 1000000;				\
	    (c1)->sec++;					\
	}							\
	(c1)->sec += (c2)->sec;					\
    END

/* Subtract time c2 from time c1.  c2 should be less than c1 */
#define	clock_Sub(c1, c2)					\
    BEGIN							\
	if (((c1)->usec	-= (c2)->usec) < 0) {			\
	    (c1)->usec += 1000000;				\
	    (c1)->sec--;					\
	}							\
	(c1)->sec -= (c2)->sec;					\
    END

#define clock_Float(c) ((c)->sec + (c)->usec/1e6)

#endif _CLOCK_
