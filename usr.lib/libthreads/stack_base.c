/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * stack_base.c
 *
 * $Source: /afs/.mtxinu.com/mach/rcs/usr/lib/libthreads/stack_base.c,v $
 *
 * C Thread stack allocation base address.
 *
 * HISTORY
 * $Log:	stack_base.c,v $
 * Revision 1.1  90/02/22  18:39:53  bww
 * 	Mach Release 2.5
 * 	[90/02/22  18:38:07  bww]
 * 
 * Revision 2.1  89/05/18  11:17:25  mbj
 * Created.
 * 
 * 31-Mar-89  Michael Jones (mbj) at Carnegie-Mellon University
 *	Provide external definition of cthread_stack_base as starting
 *	address for stack allocation.  Normally defined as 0, but
 *	another definition could be loaded to allow for other values.
 */

#ifndef	lint
static char rcs_id[] = "$ Header: stack_base.c,v 2.1 89/05/18 11:17:25 mbj Exp $";
#endif	not lint

#include <cthreads.h>
#include "cthread_internals.h"

#if	MTHREAD || COROUTINE

vm_address_t cthread_stack_base = 0;	/* Base for stack allocation */

#endif	MTHREAD || COROUTINE
