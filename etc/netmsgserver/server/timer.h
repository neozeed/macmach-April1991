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
 * $Log:	timer.h,v $
 * Revision 1.1  90/02/19  21:57:22  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.13  89/05/02  11:18:33  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 10-Oct-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added timer_wake_up extern declaration.
 *
 * 26-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed timer_start so that it assumes that the given timer is not on the timer queue.
 *	Added extern definition of timer_restart.
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made TIMEVAL_CMP take struct timevals as arguments instead of pointers.
 *
 * 13-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Cleaned up once more!  Added TIMER_NULL.
 *
 * 16-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Cleaned up.
 *
 * 30-Jan-87  Thomas Newton (tdn) at Carnegie Mellon University
 *	Created minor variant of this file for use in Ada+ compiler;
 *		see HISTORY comments in timer.c
 *
 *  2-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Make timer_queue and time_to_wake_up externs.
 *	Made all routine definitions extern (and included types where relevant).
 *
 * 10-Nov-86  Sanjay Agrawal (agrawal) at Carnegie Mellon University
 *	Started
 */
/*
 * timer.h
 *
 *
 * $ Header: timer.h,v 1.13 89/05/02 11:18:33 dpj Exp $
 *
 */

/*
 * Definitions for the timer package.
 */


#ifndef _TIMER_
#define _TIMER_

#include <mach/boolean.h>
#include <sys/time.h>
#include "mem.h"

/*
 * TIMEVAL_CMP
 *	Compare two timeval structures.
 *	Result is <, ==, or > 0, according to whether t1 is <, ==, or > t2.
 */
#define TIMEVAL_CMP(t1, t2)		\
	((t1).tv_sec == (t2).tv_sec ? (t1).tv_usec - (t2).tv_usec : (t1).tv_sec - (t2).tv_sec)

/*
 * Timer structure.
 */
typedef struct timer {
	struct timer	*link;		/* used by queue package */
	struct timeval	interval;	/* timer interval */
	int		(*action)();	/* action on timer expiration */
	char 		*info;		/* arbitrary client information */
	struct timeval	deadline;	/* used by timer package */
} *timer_t;

#define TIMER_NULL	((timer_t)0)


/*
 * TIMER_WAKE_UP
 *	wakes up the timer thread immediately.
 *
 */
extern void timer_wake_up();
/*
*/


/*
 * TIMER_START:
 *	If timer t is not already on the timer queue
 *	then the absolute deadline of t is computed and t is inserted in the timer queue.
 *
 *	Assumes that the timer is not on the timer queue - timer_restart should be used if it is.
 */
extern void timer_start(/*t*/);
/*
timer_t		t;
*/


/*
 * TIMER_STOP:
 *	If timer t is present on the queue then it is removed from the queue.
 *	Returns whether there was a timer to be removed or not.
 */
extern boolean_t timer_stop(/*t*/);
/*
timer_t		t;
*/


/*
 * TIMER_RESTART:
 *	Starts timer T Whether it was already queued on the timer queue or not.
 */
extern void timer_restart(/*t*/);
/*
timer_t		t;
*/


/*
 * TIMER_ALLOC:
 *	allocates space for a timer and initialises it.
 */
extern timer_t timer_alloc();


/*
 * TIMER_INIT:
 *	Initilizes the timer package.
 *	Creates a thread which waits for timers to expire.
 */
extern boolean_t timer_init();


/*
 * TIMER_KILL:
 *	Terminates the background timer thread by tricking it into suicide.
 *	Note that the timer thread may not be terminated upon return --
 *	the purpose of this routine is to do cleanup when a program is
 *	ready to terminate so that the threads package will not dump core.
 */
extern void timer_kill();


/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_TIMER;


#endif _TIMER_
