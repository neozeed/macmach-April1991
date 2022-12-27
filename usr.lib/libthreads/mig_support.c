/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mig_support.c,v $
 * Revision 1.1  90/02/22  18:39:36  bww
 * 	Mach Release 2.5
 * 	[90/02/22  18:38:07  bww]
 * 
 * Revision 1.4  89/06/08  18:28:26  mrt
 * 	Changed task_data() to thread_reply()
 * 	[89/06/08            mrt]
 * 
 * Revision 1.3  89/05/05  18:54:23  mrt
 * 	Cleanup for Mach 2.5
 * 
 *
 * 24-Mar-89  Michael Jones (mbj) at Carnegie-Mellon University
 *	Made MTASK version work correctly again.
 *
 * 27-Aug-87  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed mig_support.c to avoid deadlock that can occur
 *	if tracing is turned on	during calls to mig_get_reply_port().
 *
 * 10-Aug-87  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed mig_support.c to use MACH_CALL.
 *	Changed "is_init" to "multithreaded" and reversed its sense.
 *
 * 30-Jul-87  Mary Thompson (mrt) at Carnegie Mellon University
 *	Created.
 */
/*
 * mig_support.c  - by Mary Thompson
 *
 * Routines to set and deallocate the mig reply port for the current thread.
 * Called from mig-generated interfaces.
 */

#ifndef	lint
static char rcs_id[] = "$ Header: mig_support.c,v 1.4 89/06/08 18:28:26 mrt Exp $";
#endif	not lint

#include <mach.h>
#include <cthreads.h>
#include "cthread_internals.h"

#if	MTASK
#undef	task_self  /* Must call the function since the variable is shared */
#endif	MTASK

private struct mutex reply_port_lock = MUTEX_INITIALIZER;
private int multithreaded = 0;

/*
 * Called by mach_init with 0 before cthread_init is
 * called and again with 1 at the end of cthread_init.
 */
void
mig_init(init_done)
	int init_done;
{
	multithreaded = init_done;
}

/*
 * Called by mig interface code whenever a reply port is needed.
 * Tracing is masked during this call; otherwise, a call to printf()
 * can result in a call to malloc() which eventually reenters
 * mig_get_reply_port() and deadlocks.
 */
port_t
mig_get_reply_port()
{
	register cproc_t self;
	register kern_return_t r;
	port_t port;
#ifdef	DEBUG
	int d = cthread_debug;
#endif	DEBUG

	if (! multithreaded)
		return thread_reply();
#ifdef	DEBUG
	cthread_debug = FALSE;
#endif	DEBUG
	self = cproc_self();
	if (self->reply_port == PORT_NULL) {
		mutex_lock(&reply_port_lock);
		self->reply_port = thread_reply();
		MACH_CALL(port_allocate(task_self(), &port), r);
		self->reply_port = port;
		mutex_unlock(&reply_port_lock);
	}
#ifdef	DEBUG
	cthread_debug = d;
#endif	DEBUG
	return self->reply_port;
}

/*
 * Called by mig interface code after a timeout on the reply port.
 * May also be called by user.
 */
void
mig_dealloc_reply_port()
{
	register cproc_t self;
	register port_t port;
#ifdef	DEBUG
	int d = cthread_debug;
#endif	DEBUG

	if (! multithreaded)
		return;
#ifdef	DEBUG
	cthread_debug = FALSE;
#endif	DEBUG
	self = cproc_self();
	ASSERT(self != NO_CPROC);
	port = self->reply_port;
	if (port != PORT_NULL && port != thread_reply()) {
		mutex_lock(&reply_port_lock);
		self->reply_port = thread_reply();
		(void) port_deallocate(task_self(), port);
		self->reply_port = PORT_NULL;
		mutex_unlock(&reply_port_lock);
	}
#ifdef	DEBUG
	cthread_debug = d;
#endif	DEBUG
}
