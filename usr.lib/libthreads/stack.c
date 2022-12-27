/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	stack.c,v $
 * Revision 1.1  90/02/22  18:39:48  bww
 * 	Mach Release 2.5
 * 	[90/02/22  18:38:07  bww]
 * 
 * Revision 1.5  89/05/19  13:09:50  mbj
 * 	Don't deallocate the signal trampoline code, the args or the initial
 * 	environment even though they're on a stack.  They're actually per-task
 * 	rather than per-thread data.
 * 
 * Revision 1.4  89/05/18  11:17:15  mbj
 * 	Add an assertion to catch wraparound on stack allocation.
 * 	[89/05/04            mbj]
 * 
 * 	Use external definition of cthread_stack_base as starting
 * 	address for stack allocation.  Normally defined as 0, but
 * 	defined externally to allow for other values.
 * 	[89/03/31            mbj]
 * 
 * Revision 1.3  89/05/05  18:59:51  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 24-Mar-89  Michael Jones (mbj) at Carnegie-Mellon University
 *	Implement fork() for multi-threaded programs.
 *	Made MTASK version work correctly again.
 *
 * 01-Dec-87  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed cthread stack allocation to use aligned stacks
 *	and store self pointer at base of stack.
 *	Added inline expansion for cthread_sp() function.
 */
/*
 * stack.c  - by Eric Cooper
 *
 * C Thread stack allocation.
 */

#ifndef	lint
static char rcs_id[] = "$ Header: stack.c,v 1.5 89/05/19 13:09:50 mbj Exp $";
#endif	not lint

#include <cthreads.h>
#include "cthread_internals.h"

#include <sys/time.h>
#include <sys/resource.h>

#if	MTASK
#undef	task_self  /* Must call the function since the variable is shared */
#endif	MTASK

/*
 * C library imports:
 */
extern getrlimit(), perror();

#define	BYTES_TO_PAGES(b)	(((b) + vm_page_size - 1) / vm_page_size)

int cthread_stack_mask;

private vm_size_t cthread_stack_size = 0;

#if	MTHREAD || COROUTINE

extern vm_address_t cthread_stack_base;	/* Base for stack allocation */

private vm_address_t next_stack_base;

vm_address_t initial_stack_boundary;
private vm_address_t initial_stack_deallocated = 0;
private int initial_stack_lock = 0;

#endif	MTHREAD || COROUTINE

/*
 * Set up a stack segment for a thread.
 * Segment has a red zone (invalid page)
 * for early detection of stack overflow.
 * The cproc_self pointer is stored at the base.
 *
 *	--------- (high address)
 *	|	|
 *	|  ...	|
 *	|	|
 *	| stack	|
 *	|	|
 *	|  ...	|
 *	|	|
 *	--------- (stack base)
 *	|	|
 *	|invalid|
 *	|	|
 *	---------
 *	|	|
 *	|	|
 *	| self	|
 *	--------- (low address)
 */

private void
setup_stack(p, base)
	register cproc_t p;
	register vm_address_t base;
{
	register kern_return_t r;

	/*
	 * Check alignment.
	 */
	ASSERT((base & cthread_stack_mask) == base);
	ASSERT(((base + cthread_stack_size - 1) & cthread_stack_mask) == base);
	/*
	 * Stack base is two pages from bottom.
	 */
	p->stack_base = (unsigned int) (base + 2*vm_page_size);
	/*
	 * Stack size is segment size minus two pages.
	 */
#if	MTHREAD || COROUTINE
	if (p->flags & CPROC_INITIAL_STACK) {
		p->stack_size = initial_stack_boundary - p->stack_base;
	} else {
		p->stack_size = cthread_stack_size - 2*vm_page_size;
	}
#else	MTHREAD || COROUTINE
	p->stack_size = cthread_stack_size - 2*vm_page_size;
#endif	MTHREAD || COROUTINE
	/*
	 * Protect red zone.
	 */
	MACH_CALL(vm_protect(task_self(), base + vm_page_size, vm_page_size, FALSE, VM_PROT_NONE), r);
	/*
	 * Store self pointer.
	 */
	*((cproc_t *) base) = p;
}

void
stack_init(p)
	cproc_t p;
{
	struct rlimit rlim;

	/*
	 * Set cthread stack size from UNIX stack limit.
	 */
	if (getrlimit(RLIMIT_STACK, &rlim) != 0) {
		perror("getrlimit");
		exit(1);
	}
#if	MTASK
	/* 
	 * Stacks must all be the same size.  Check this for multiple tasks.
	 */
	ASSERT(cthread_stack_size == BYTES_TO_PAGES(rlim.rlim_cur) * vm_page_size || cthread_stack_size == 0);
#endif	MTASK

	cthread_stack_size = BYTES_TO_PAGES(rlim.rlim_cur) * vm_page_size;
	cthread_stack_mask = ~(cthread_stack_size - 1);

#if	MTHREAD || COROUTINE
	/*
	 * Guess at first available region for stack.
	 */
	next_stack_base = (cthread_stack_base + cthread_stack_size-1) &
				cthread_stack_mask;

	/*
	 * This is a pessimistic approximation of the top of the initial stack
	 * not including the thread trampoline code, the arguments and the
	 * initial environment.  It's pessimistic in that it contains a few
	 * stack frames which will be wasted when the stack is reallocated.
	 */
	initial_stack_boundary = (vm_address_t) cthread_sp();
	p->flags |= CPROC_INITIAL_STACK;	/* We're "special" */

#endif	MTHREAD || COROUTINE

	/*
	 * Set up stack for main thread.
	 */
	setup_stack(p, (vm_address_t) (cthread_sp() & cthread_stack_mask));
}

int
cthread_sp()
{
	int x;

	return (int) &x;
}

#if	MTHREAD || COROUTINE

/*
 * Allocate a stack segment for a thread.  Stacks are never deallocated
 * except by the child process of a fork() during re-initialization.
 *
 * The variable next_stack_base is used to align stacks.
 * It may be updated by several threads in parallel,
 * but mutual exclusion is unnecessary: at worst,
 * the vm_allocate will fail and the thread will try again.
 */

void
alloc_stack(p)
	cproc_t p;
{
	vm_address_t base;

	if (initial_stack_deallocated) {
		vm_address_t boundary_page;
		register kern_return_t r;

		spin_lock(&initial_stack_lock);
		if (! initial_stack_deallocated) {
			spin_unlock(&initial_stack_lock);
			goto normal_alloc_stack;
		}
		initial_stack_deallocated = 0;
		spin_unlock(&initial_stack_lock);

		p->flags |= CPROC_INITIAL_STACK;

		boundary_page = initial_stack_boundary &~ (vm_page_size - 1);
		base = initial_stack_boundary & cthread_stack_mask;

		if (initial_stack_boundary != boundary_page)
			bzero(boundary_page,
				initial_stack_boundary - boundary_page);

		MACH_CALL(vm_deallocate(task_self(), base, boundary_page - base), r);
		MACH_CALL(vm_allocate(task_self(), &base, boundary_page - base, FALSE), r);

		setup_stack(p, base);
		return;
	}

normal_alloc_stack:
	for (base = next_stack_base; ; ) {
	    ASSERT(cthread_stack_base == 0 || base != 0);
	    if (vm_allocate(task_self(), &base, cthread_stack_size, FALSE)
		== KERN_SUCCESS) break;
	    base += cthread_stack_size;
	}

	next_stack_base = base + cthread_stack_size;
	setup_stack(p, base);
}

void dealloc_stack(p)
	cproc_t p;
{
	register kern_return_t r;

	if (p->flags & CPROC_INITIAL_STACK) {
		/*
		 * Don't deallocate the signal trampoline code, the args or
		 * the initial environment even though they're on a stack.
		 * They're actually per-task rather than per-thread data.
		 */
		initial_stack_deallocated = p->stack_base;
	} else {
		MACH_CALL(vm_deallocate(task_self(), p->stack_base & cthread_stack_mask, cthread_stack_size), r);
	}
}

#endif	MTHREAD || COROUTINE

void stack_fork_child()
/*
 * Called in the child after a fork().  Resets stack data structures to
 * coincide with the reality that we now have a single cproc and cthread.
 */
{
#if	MTHREAD || COROUTINE
	/*
	 * Allocate stacks from the beginning of cthread_stack_base again.
	 */
	next_stack_base = (cthread_stack_base + cthread_stack_size-1) &
				cthread_stack_mask;
#endif	MTHREAD || COROUTINE
}
