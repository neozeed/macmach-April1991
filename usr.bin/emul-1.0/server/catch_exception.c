/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: emul/server/catch_exception.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"

#include <mach/exception.h>

kern_return_t
catch_exception_raise(port_t	exception_port,
		      port_t	thread_,
		      port_t	task_,
		      int	exception,
		      int	code,
		      int	subcode)
{
    kern_return_t	result;

    if (task_ == task && thread_ == thread)
	switch (exception) {
	  case EXC_BAD_ACCESS:
	  case EXC_BAD_INSTRUCTION:
	  case EXC_BREAKPOINT:
	    debugger();
	    return (KERN_SUCCESS);
	}
    else {
	fprintf(stderr, "catch exception: wrong task/thread\n");
	return (KERN_FAILURE);
    }

    fprintf(stderr, "unhandled exception %x code %x subcode %x\n",
	    exception,
	    code,
	    subcode);
    debugger();

    return (KERN_FAILURE);
}
