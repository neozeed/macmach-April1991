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
 * $Log:	lock_queue.c,v $
 * Revision 1.1  90/02/19  21:52:30  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.21  89/05/02  11:11:07  dpj
 * 	13-Oct-87 Mary R. Thompson (mrt) at Carnegie Mellon
 * 
 *	Changed queue_item_t to cthread_queue_item_t.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *	Created lock_queue_init().
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added versions of most routines that do not do the locking
 *	internally.
 *
 * 14-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Use vax_fast_lock macros if possible.
 *
 * 10-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Go back to using mem_allocobj in lq_alloc.
 *
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	NO_CONDITION: Removed the use of a condition in queues, since only
 *	lq_blocking_dequeue() needs it, and nobody uses that routine.
 *	Disabled that routine. (from rfr).
 *
 * 18-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed lq_cond_delete_from_queue to return the item deleted.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added some register declarations.
 *	Changed lq_remove_from_queue to directly delete the item from the queue.
 *
 * 15-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed the explict queue_item_t parameter from lq_cond_delete_from_queue.
 *	The test function for it is now the same as for lq_find_in_queue.
 *	Replaced fprintf by ERROR macro.
 *
 * 19-Mar-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Revamped for better use outside the network server:
 *		use cthreads malloc instead of mem_allocobj;
 *		debugging macros from trace.h;
 *		mutex and condition are contained directly within a lock_queue_t.
 *
 * 20-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Bug fix from Tom Newton to lq_insert_in_queue.
 *
 * 18-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Use mem_allocobj to allocate lock queues.
 *	Fixed bug in lq_cond_delete_from_queue.
 *
 *  3-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Change lq_alloc to actually allocate space for the queue and to return a lock_queue_t.
 *	Changed to use BEGIN, END, RET and RETURN macros.
 *
 * 12-Oct-86  Sanjay Agrawal (agrawal) at Carnegie Mellon University
 *	Started
 */
/*
 * lock_queue.c
 *
 *
 */
#ifndef	lint
char lock_queue_rcsid[] = "$ Header: lock_queue.c,v 1.21 89/05/02 11:11:07 dpj Exp $";
#endif not lint

/*
 * The functions contained in this file are
 * lq_alloc();
 * lq_init();
 * lq_prequeue();
 * lq_enqueue();
 * lq_insert_in_queue();
 * lq_dequeue();
 * lq_blocking_dequeue();
 * lq_remove_from_queue();
 * lq_cond_delete_from_queue();
 * lq_on_queue();
 * lq_find_in_queue();
 * lq_map_queue();
 */


#define DEBUGOFF	1
#define	NO_CONDITION	1

#include <cthreads.h>
#include <stdio.h>
#include <mach/boolean.h>

#include "netmsg.h"
#include "debug.h"
#include "trace.h"
#include "mem.h"
#include "lock_queue.h"

#if	NO_CONDITION
#ifdef	condition_signal
#undef	condition_signal
#endif	condition_signal
#define condition_signal(foo)
#endif	NO_CONDITION

#ifdef	vax
#undef	VAX_FAST_LOCK
#define	VAX_FAST_LOCK	1
#else	vax
#undef	VAX_FAST_LOCK
#define	VAX_FAST_LOCK	0
#endif	vax

#if	VAX_FAST_LOCK
#include "vax_fast_lock.h"
#endif	VAX_FAST_LOCK

/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_LQUEUE;


/*
 * LQ_ALLOC:
 *	Allocates data space for the lock and the nonempty condition field.
 *	Calls lq_init to initialize the queue.
 */
lock_queue_t lq_alloc()
BEGIN("lq_alloc")
	register lock_queue_t	q;

	MEM_ALLOCOBJ(q,lock_queue_t,MEM_LQUEUE);
	lq_init(q);
	RETURN(q);
END


/*
 * LQ_INIT:
 *	Initializes the head and the tail of the queue to nil.
 *	Initialises the lock and condition of the queue.
 */
void lq_init(q)
	register lock_queue_t	q;
BEGIN("lq_init")
        q->head = 0;
        q->tail = 0;
        condition_init(&(q->nonempty));
	mutex_init(&(q->lock));
	RET;
END



/*
 * LQ_PREQUEUE:
 *	Inserts queue_item at the head of the queue.
 *	Locks queue while accessing queue.
 *	Signals queue nonempty.
 */
void lq_prequeue(q, x)
	register lock_queue_t	q;
	register cthread_queue_item_t	x;
BEGIN("lq_prequeue")
    	mutex_lock(&(q->lock));
	if (q->tail == 0) {
		q->head = q->tail = x;
		x->next = 0;
	} else {
		x->next = q->head;
		q->head = x;
	}
    	mutex_unlock(&(q->lock));
	condition_signal(&(q->nonempty));
	RET;
END

void lqn_prequeue(q, x)
	register lock_queue_t	q;
	register cthread_queue_item_t	x;
BEGIN("lqn_prequeue")
	if (q->tail == 0) {
		q->head = q->tail = x;
		x->next = 0;
	} else {
		x->next = q->head;
		q->head = x;
	}
/*	condition_signal(&(q->nonempty)); */
	RET;
END


/*
 * LQ_ENQUEUE:
 *	Enters queue item at the tail of the queue.
 *	Locks queue while accessing queue.
 */
void lq_enqueue(q, x)
	register lock_queue_t	q;
	register cthread_queue_item_t	x;	
BEGIN("lq_enqueue")
	x->next = 0;
	mutex_lock(&(q->lock));
	if (q->tail == 0)
		q->head = q->tail = x;
	else {
		q->tail->next = x;
		q->tail = x;
	}
	mutex_unlock(&(q->lock));
	condition_signal(&(q->nonempty));
	RET;
END


/*
 * LQ_INSERT_IN_QUEUE:
 *	Inserts queue_item in the correct positiion on the queue.
 *	Does so by calling a test function to do a comparison.
 *	The parameters passed to the test function are:
 *		current	- the item in the queue that is being looked at,
 *		x	- the queue_item passed in, and
 *		args	- the argument value passed in.
 *	Locks queue while accessing queue.
 *	Signals queue nonempty.
 */
void lq_insert_in_queue(q, test, x, args)
	register lock_queue_t	q;
	int			(*test)();
	register cthread_queue_item_t	x;
	int			args;
BEGIN("lq_insert_in_queue")
	register cthread_queue_item_t	prev, cur;

	mutex_lock(&(q->lock)); 
	cur = q->head;
	if (!cur){
		q->head = q->tail = x;
		x->next = 0;
		mutex_unlock(&(q->lock)); 
		condition_signal(&(q->nonempty));
		RET;
	}
	if ((*test)(cur, x,args)) {
		x->next = q->head;
		q->head = x;
		mutex_unlock(&(q->lock)); 
		condition_signal(&(q->nonempty));
		RET;
	}
	do{
		prev = cur;
		cur = cur->next;
		if ((cur) && ((*test)(cur, x, args))) {
			prev->next = x;
			x->next = cur;
			mutex_unlock(&(q->lock)); 
			condition_signal(&(q->nonempty));
			RET;
		}
	} while (cur != 0);
	ASSERT(prev == q->tail);
	prev->next = x;
	q->tail = x;
	x->next = 0;

	mutex_unlock(&(q->lock)); 
	condition_signal(&(q->nonempty));
	RET;
END



/*
 * LQ_DEQUEUE:
 *	If queue is not empty then removes item from the head of the queue.
 *	Locks queue while accessing queue.
 */
cthread_queue_item_t lq_dequeue(q)
	register lock_queue_t	q;
BEGIN("lq_dequeue")
	register cthread_queue_item_t	x;

	mutex_lock(&(q->lock));
	x = q->head;
	if (x != 0) {
		if ((q->head = x->next) == 0)
			q->tail = 0;
		else
			x->next = 0;
	}
	mutex_unlock(&(q->lock));
	RETURN(x);
END


#if	NO_CONDITION
#else	NO_CONDITION
/*
 * LQ_BLOCKING_DEQUEUE:
 *	If the queue is empty, a wait is done on the nonempty condition.
 *	Removes item from the head of the queue.
 *	Locks queue while accessing queue
 */
cthread_queue_item_t lq_blocking_dequeue(q)
	register lock_queue_t	q;
BEGIN("lq_blocking_dequeue")
	register cthread_queue_item_t	x;

	mutex_lock(&(q->lock));

	while ((x = q->head) == 0){
		condition_wait(&(q->nonempty),&(q->lock));
		}
	if ((q->head = x->next) == 0)
		q->tail = 0;
	else
		x->next = 0;

	mutex_unlock(&(q->lock));
	RETURN(x);
END
#endif	NO_CONDITION


/*
 * LQ_REMOVE_FROM_QUEUE:
 *	Removes the queue_item from the queue if it is present on the queue.
 *	Returns whether the item was deleted from the queue.
 */
boolean_t lq_remove_from_queue(q,x)
	register lock_queue_t	q;
	register cthread_queue_item_t	x;
BEGIN("lq_remove_from_queue")
	register cthread_queue_item_t	prev, cur;

	mutex_lock(&(q->lock)); 
	for (prev = cur = q->head; cur != 0; prev = cur, cur = cur->next)
		if (x == cur) {
			if (cur == q->head){
				if ((q->head = cur->next) == 0)
					q->tail = 0;
				else
					cur->next = 0;
			}
			else{
				if ((prev->next = cur->next) == 0) {
					ASSERT(cur == q->tail);
					q->tail = prev;
				} else
					cur->next = 0;
			}
			mutex_unlock(&(q->lock));	
			RETURN(TRUE);
		}
	mutex_unlock(&(q->lock));	
	RETURN(FALSE);
END
			

/*
 * LQ_COND_DELETE_FROM_QUEUE:
 *	Performs the test function with each element of the queue, 
 *	until the function returns true, or the tail of the queue is reached.
 *	The parameters passed to the test function are:
 *		current	- the item in the queue that is being looked at,
 *		args	- the argument value passed in.
 *	The item is then removed from the queue.
 *	Locks queue while accessing queue.
 *	Returns the item that was deleted from the queue.
 */
cthread_queue_item_t lq_cond_delete_from_queue(q, test, args)
	register lock_queue_t	q;
	int			(*test)();
	int			args;
BEGIN("lq_cond_delete_from_queue")
	register cthread_queue_item_t	prev, cur;

	mutex_lock(&(q->lock)); 
	for (prev = cur = q->head; cur != 0; prev = cur, cur = cur->next)
		if ((*test)(cur, args)) {
			if (cur == q->head){
				if ((q->head = cur->next) == 0)
					q->tail = 0;
				else
					cur->next = 0;
			}
			else{
				if ((prev->next = cur->next) == 0) {
					ASSERT(cur == q->tail);
					q->tail = prev;
				} else
					cur->next = 0;
			}
			mutex_unlock(&(q->lock));	
			RETURN(cur);
		}
	mutex_unlock(&(q->lock));
	RETURN((cthread_queue_item_t)0);
END

cthread_queue_item_t lqn_cond_delete_from_queue(q, test, args)
	register lock_queue_t	q;
	int			(*test)();
	int			args;
BEGIN("lqn_cond_delete_from_queue")
	register cthread_queue_item_t	prev, cur;

	for (prev = cur = q->head; cur != 0; prev = cur, cur = cur->next)
		if ((*test)(cur, args)) {
			if (cur == q->head){
				if ((q->head = cur->next) == 0)
					q->tail = 0;
				else
					cur->next = 0;
			}
			else{
				if ((prev->next = cur->next) == 0) {
					ASSERT(cur == q->tail);
					q->tail = prev;
				} else
					cur->next = 0;
			}
			RETURN(cur);
		}
	RETURN((cthread_queue_item_t)0);
END



/*
 * LQ_ON_QUEUE:
 *	Locks queue while accessing queue.
 *	Checks to see if the cthread_queue_item_t is on the queue,
 *	if so returns true else returns false.
 */
boolean_t lq_on_queue(q,x)
	register lock_queue_t	q;
	register cthread_queue_item_t	x;
BEGIN("lq_on_queue")
	register cthread_queue_item_t	cur;

	mutex_lock(&(q->lock));	
	for (cur = q->head; cur != 0;cur = cur->next){
		if (cur == x){
			mutex_unlock(&(q->lock));
			RETURN(TRUE);
		}
	}
	mutex_unlock(&(q->lock));
	RETURN(FALSE);
END


/*
 * LQ_FIND_IN_QUEUE:
 *	Returns a cthread_queue_item_t which is found by the function test.
 *	The parameters passed to the test function are:
 *		current	- the item in the queue that is being looked at,
 *		args	- the argument value passed in.
 *	If no cthread_queue_item_t is found returns nil.
 *	Locks queue while accessing queue
 */
cthread_queue_item_t lq_find_in_queue(q, test, args)
	register lock_queue_t	q;
	register int		(*test)();
	register int		args;
BEGIN("lq_find_in_queue")
	register cthread_queue_item_t	cur;

	mutex_lock(&(q->lock));
	for (cur = q->head; cur != 0; cur = cur->next)
		if ((*test)(cur, args)){
			mutex_unlock(&(q->lock));
			RETURN(cur);
		}
	mutex_unlock(&(q->lock));
	RETURN((cthread_queue_item_t)0);
END

cthread_queue_item_t lqn_find_in_queue(q, test, args)
	register lock_queue_t	q;
	register int		(*test)();
	register int		args;
BEGIN("lqn_find_in_queue")
	register cthread_queue_item_t	cur;

	for (cur = q->head; cur != 0; cur = cur->next)
		if ((*test)(cur, args)){
			RETURN(cur);
		}
	RETURN((cthread_queue_item_t)0);
END


/*
 * LQ_MAP_QUEUE:
 *	Maps fn() onto each item on the queue;
 *	The parameters passed to the map function are:
 *		current	- the item in the queue that is being looked at,
 *		args	- the argument value passed in.
 *	Locks queue while accessing queue
 */
void lq_map_queue(q, fn, args)
	register lock_queue_t	q;
	register int		(*fn)();
	register int		args;
BEGIN("lq_map_queue")
	register cthread_queue_item_t	cur;

	mutex_lock(&(q->lock));
	for (cur = q->head; cur != 0; cur = cur->next)
		(*fn)(cur, args);
	mutex_unlock(&(q->lock));
	RET;
END


/*
 * lock_queue_init --
 *
 * Initialize the lock_queue package.
 */
EXPORT boolean_t lock_queue_init()
BEGIN("lock_queue_init")

	/*
	 * Initialize the memory management facilities.
	 */
	mem_initobj(&MEM_LQUEUE,"Locked queue head",sizeof(struct lock_queue),
								FALSE,125,50);

	RETURN(TRUE);
END
