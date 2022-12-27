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
 * $Log:	act.h,v $
 * Revision 2.2  90/08/30  11:00:27  bohman
 * 	Created.
 * 	[90/08/29  10:57:07  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/act.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Copyright (C) 1985, 1986, 1987, 1988
 * by David E. Bohman II
 */

#ifndef _MAC2_ACT_H_
#define _MAC2_ACT_H_

#include <kern/zalloc.h>

/*
 * activity defines
 */

/*
 * Maximum number of entries
 * an activity can contain.
 */
#define ACT_MAX_NENTRIES	16

/*
 * An activity consists of a common
 * part which defines the code to
 * execute and processor ipl, and
 * one to ACT_MAX_NENTRIES activity
 * list entries, each which define a
 * call to the activity from a certain
 * activity list.
 */
struct act {
    int			(*a_func)();	/* function to execute */
    short		a_nentries;	/* number of entries available */
    unsigned short	a_ipl;		/* processor priority to exec at */
    unsigned short	a_flags;	/* activity flags */
#define	ACT_CUR		0x01		/* activity is running on a list */
    struct actentry {
	struct actentry	*ae_next;	/* forward in actlist */
	struct act	*ae_act;	/* pointer to this activity */
	struct actlist	*ae_alist;	/* actlist for entry */
	int		ae_arg;		/* argument to pass to function */
	unsigned short	ae_time;	/* timer value */
	unsigned short	ae_flags;	/* activity queue flags */
#define	ACT_Q_RUN	0x01		/* activity is scheduled for list */
#define	ACT_Q_QUE	0x02		/* activity is on list */
#define ACT_Q_OK	0x04		/* entry has been initialized */
    } a_alist[0];			/* actually 1 to ACT_MAX_NENTRIES */
};

/*
 * An activity list is a singly linked
 * list of activity list entries.  The
 * the list is fifo; activity entries are
 * added to the end and always taken off
 * the front.
 */
struct actlist {
    struct actentry	*al_front;
    struct actentry	*al_back;
};

/*
 * These next two macros are used to manipulate
 * an activity list.  CHECKACTLIST() returns TRUE
 * if the activity list should be run because it
 * contains entries.  DOACTLIST() removes each
 * queued entry from the front of the list,
 * decrements its timer value, and calls the
 * activity if the new timer value is zero.
 * It only processes activities that are queued
 * on the list when it begins so that an activity
 * can reschedule itself on a list.
 */
#define CHECKACTLIST(l)	((l).al_front != 0)

#define	DOACTLIST(l) \
{									\
    register struct actentry *ae, *ab;					\
    register struct act *ap;			       			\
    register s = splhigh();						\
\
    ab = (l).al_back;							\
    while ((l).al_front) {						\
	ae = (l).al_front;						\
	(l).al_front = ae->ae_next;					\
	if ((l).al_front == 0)						\
	    (l).al_back = 0;						\
	ae->ae_flags &= ~ACT_Q_QUE;					\
	ap = ae->ae_act;						\
	if (ae->ae_flags&ACT_Q_RUN) {					\
	    if (ae->ae_time == 0) {					\
		ae->ae_flags &= ~ACT_Q_RUN;				\
		ap->a_flags |= ACT_CUR;					\
		splx(ap->a_ipl);					\
		(*ap->a_func)(ae->ae_arg, (ae - ap->a_alist), ap);	\
		splhigh();						\
		ap->a_flags &= ~ACT_CUR;				\
	    }								\
	    else {							\
		ae->ae_time--;						\
		putact(ae);						\
	    }								\
	}								\
	if (ae == ab)							\
	    break;							\
    }									\
    splx(s);								\
}

/*
 * Cancel an activity from all lists.  The activity
 * is not dequeued from any activity list, that will
 * be done next time the activity list is processed.
 */
#define	cancelact(p) \
{									\
    register struct actentry *ae, *am = &(p)->a_alist[(p)->a_nentries];	\
    register s = spl7();						\
\
    for (ae = (p)->a_alist; ae < am; ae++) {				\
    	ae->ae_flags &= ~ACT_Q_RUN;					\
	ae->ae_time = 0;						\
    }									\
    splx(s);								\
}

/*
 * Place an activity entry on the corresponding
 * activity list.  Call at IPL7.
 */
#define	putact(pe) \
{								\
    register struct actlist *pl = (pe)->ae_alist;		\
\
    if ((pl)->al_front == 0)					\
    	(pl)->al_back = (pl)->al_front = (pe);			\
    else {							\
	(pl)->al_back->ae_next = (pe);				\
	(pl)->al_back = (pe);					\
    }								\
    (pe)->ae_next = 0;						\
    (pe)->ae_flags |= ACT_Q_QUE;				\
}

struct act *makeact();

extern zone_t actzones[];
#define ACTZONE(n)	(actzones[n - 1])

#endif	_MAC2_ACT_H_
