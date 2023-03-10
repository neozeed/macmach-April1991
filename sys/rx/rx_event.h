/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rx_event.h,v $
 * Revision 2.3  89/06/03  15:39:18  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:24:31  jsb]
 * 
 * Revision 2.2  89/04/22  15:28:55  gm0w
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

/* Event package */ 

#ifndef _EVENT_
#define _EVENT_

#ifdef	KERNEL
#include <rx/rx_queue.h>
#include <rx/rx_clock.h>
#else	KERNEL
#include "rx_queue.h"
#include "rx_clock.h"
#endif	KERNEL

/* An event is something that will happen at (or after) a specified clock time, unless cancelled prematurely.  The user routine (*func)() is called with arguments (event, arg, arg1) when the event occurs.  Warnings:  (1) The user supplied routine should NOT cause process preemption.   (2) The event passed to the user is still on the event queue at that time.  The user must not remove (event_Cancel) it explicitly, but the user may remove or schedule any OTHER event at this time. */

struct rxevent {
    struct queue junk;	    /* Events are queued */
    struct clock eventTime; /* When this event times out (in clock.c units) */
    int	(*func)();	    /* Function to call when this expires */
    char *arg;		    /* Argument to the function */
    char *arg1;		    /* Another argument */
};

/* Some macros to make macros more reasonable (this allows a block to be used within a macro which does not cause if statements to screw up).   That is, you can use "if (...) macro_name(); else ...;" without having things blow up on the semi-colon. */

#ifndef BEGIN
#define BEGIN do {
#define END } while(0)
#endif

/* This routine must be called to initialize the event package.  nEvents is the number of events to allocate in a batch whenever more are needed.  If this is 0, a default number (10) will be allocated. */
extern rxevent_Init(/* nEvents, scheduler */);

/* Arrange for the indicated event at the appointed time.  When is a "struct clock", in the clock.c time base */
extern struct rxevent *rxevent_Post(/* when, func, arg, arg1 */);

/* Remove the indicated event from the event queue.  The event must be pending.  Also see the warning, above.  The event pointer supplied is zeroed. */
extern rxevent_Cancel_1(/* event_ptr */);
#define	rxevent_Cancel(event_ptr)			    \
	BEGIN					    \
	    if (event_ptr) {			    \
		rxevent_Cancel_1(event_ptr);	    \
		event_ptr = (struct rxevent *) 0;	    \
	    }					    \
	END

/* The actions specified for each event that has reached the current clock time will be taken.  The current time returned by GetTime is used (warning:  this may be an old time if the user has not called clock_NewTime) */
extern rxevent_RaiseEvents();

/* This returns the time between the current clock value and the the next event's expiry time, in *interval.  The function value returned is 0 if there are no scheduled events, 1 if there are one or more scheduled events.  In the event that 0 is returned, *interval is not updated. */
extern rxevent_TimeToNextEvent(/* interval */);
#endif _EVENT_
