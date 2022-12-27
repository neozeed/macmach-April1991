/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	fork.c,v $
 * Revision 1.1  90/02/22  18:39:23  bww
 * 	Mach Release 2.5
 * 	[90/02/22  18:38:07  bww]
 * 
 * Revision 1.2  89/05/07  11:38:42  mrt
 * 	Cleanup for Mach 2.5
 * 
 *
 * 13-Mar-89  Michael Jones (mbj) at Carnegie-Mellon University
 *	Wrote it.
 */
/*
 * fork.c
 *
 * Implement fork() for programs using the C Threads package.
 *
 * Copyright (C) 1989 Michael B. Jones
 */

#ifndef	lint
static char rcs_id[] = "$ Header: fork.c,v 1.2 89/05/07 11:38:42 mrt Exp $";
#endif	not lint

#include <mach/mach_types.h>
#include <mach.h>
#include <cthreads.h>
#include "cthread_internals.h"

#include <sys/syscall.h>

extern int mach_init();
extern int syscall(), getpid();

extern void
	cthread_fork_prepare(),
	cthread_fork_parent(),
	cthread_fork_child();

#if	MTASK
/*
 * Remember the last pid of a child process.  This is imperfect, but good
 * enough to prevent most of the "[unknown] (process %d) Exited" messages.
 */
int cthread_last_child_pid;
#endif	MTASK

int unix_fork()
/*
 * Implement a straight unix fork() call even though we overload fork().
 */
{
    int parent_pid = getpid();
    int fork_return = syscall(SYS_fork);
    int our_pid = getpid();

    if (fork_return < 0) return fork_return;	/* Fork error */

    if (parent_pid == our_pid) {
	/*
	 * Parent.
	 */
	return fork_return;		/* Parent returns child pid */
    } else {
	/*
	 * Child.
	 */
	(void) mach_init(); /* Reinitialize mach ports, etc. as per libmach */
	return 0;	/* Syscall(SYS_fork) doesn't return 0 to the child.
			   Returning fork_return would be incorrect. */
    }
}

int fork()
/*
 * Fork a multi-threaded process, cleaning up the cthread state in the child.
 * The child will come up with only one thread running.
 */
{
    int fork_return;

    cthread_fork_prepare();		/* Grab cthread locks to insure a
    					   consistent state after the fork */

    fork_return = unix_fork();		/* Do an actual fork (and mach_init) */

    if (fork_return != 0) {
	/*
	 * Parent and error cases.
	 */

#if	MTASK
	if (fork_return > 0) cthread_last_child_pid = fork_return;
#endif	MTASK

	cthread_fork_parent();		/* Release locks grabbed by prepare */
    } else {
	/*
	 * Child.
	 */
#if	INHERITANCE_PRESERVES_SHARING || ! MTASK
	cthread_fork_child();		/* Rebuild a consistent state */
#else	INHERITANCE_PRESERVES_SHARING || ! MTASK
	/*
	 * Currently, the vm_inherit code is broken such that if a task does
	 * a fork() with a region marked for VM_INHERIT_COPY which it was
	 * already sharing with other tasks, both the parent and the child
	 * have the sharing relationship broken instead of just the child.
	 * We fork twice as a workaround -- once leaving sharing in place so
	 * the parent isn't disturbed, and the second time to break sharing
	 * in the child's child.  The first child just exits.
	 *			Mike Jones  --  22-Mar-1989
	 */
	void malloc_set_inheritance();

	/*
	 * Since we're still sharing with our parent, go ahead and tell it
	 * who we are in case we manage to exit before he runs again.
	 */
	cthread_last_child_pid = getpid();

	malloc_set_inheritance(FALSE);	/* Do next fork inheriting copies */
	fork_return = unix_fork();
	if (fork_return != 0) {
	    /*
	     * Parent and error cases.  Throw away extra task which may or may
	     * not still be sharing memory with the parent, depending upon
	     * whether or not inheritance has been fixed.
	     */
	    _exit(0);
	} else {
	    /*
	     * Child (of child) case.  We've definitely broken the sharing.
	     */
	    cthread_fork_child();	/* Rebuild a consistent state */
	}
#endif	INHERITANCE_PRESERVES_SHARING || ! MTASK
    }
    return fork_return;
}

int vfork()
{
    return fork();
}
