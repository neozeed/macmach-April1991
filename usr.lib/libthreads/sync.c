/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	sync.c,v $
 * Revision 1.1  90/02/22  18:39:57  bww
 * 	Mach Release 2.5
 * 	[90/02/22  18:38:07  bww]
 * 
 * Revision 1.4  89/05/05  19:01:02  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*
 * sync.c - by Eric Cooper
 *
 * Spin locks and mutexes.
 */
#ifndef	lint
static char rcs_id[] = "$ Header: sync.c,v 1.4 89/05/05 19:01:02 mrt Exp $";
#endif	not lint


#include <cthreads.h>
#include "cthread_internals.h"

/*
 * Spin locks.
 * Use test and test-and-set logic on all architectures.
 */

void
spin_lock(p)
	register int *p;
{
	while (*p != 0 || !mutex_try_lock((mutex_t) p))
		;	/* spin */
}

void
spin_unlock(p)
	int *p;
{
	mutex_unlock((mutex_t) p);
}

/*
 * Mutex objects.
 *
 * Mutex_wait_lock() is implemented in terms of mutex_try_lock().
 * Mutex_try_lock() and mutex_unlock() are machine-dependent,
 * except in the COROUTINE implementation.
 *
 * Mutex_try_lock() relies on the fact that the first word pointed to
 * by its argument is the lock word, and does not reference any other fields;
 * hence it can be used by both spin_lock() (with an int * argument)
 * and mutex_wait_lock() (with a mutex_t argument).
 */

int mutex_spin_limit = 0;

void
mutex_wait_lock(m)
	register mutex_t m;
{
	register int i;

	TRACE(printf("[%s] lock(%s)\n", cthread_name(cthread_self()), mutex_name(m)));
	for (i = 0; i < mutex_spin_limit; i += 1)
		if (m->lock == 0 && mutex_try_lock(m))
			return;
		else
			/* spin */;
	for (;;)
		if (m->lock == 0 && mutex_try_lock(m))
			return;
		else
			cthread_yield();
}

#if	COROUTINE

int
mutex_try_lock(m)
	register mutex_t m;
{
	if (m->lock) {
		return FALSE;
	} else {
		m->lock = 1;
		return TRUE;
	}
}

void
mutex_unlock(m)
	register mutex_t m;
{
	m->lock = 0;
}

#endif	COROUTINE
