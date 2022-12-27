/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mig_support.c,v $
 * Revision 1.1  90/02/19  18:08:34  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.4  89/06/13  16:07:33  mrt
 * 	Replaced instances of task_data with thread_reply.
 * 	[89/05/28            mrt]
 * 
 * Revision 1.3  89/05/05  18:46:11  mrt
 * 	Cleanup for Mach 2.5
 * 
 *
 *  12-May-88	Mary Thompson (mrt) at Carnegie Mellon
 *	included mach_error.h to remove lint
 *  30-Jul-87	Mary Thompson (mrt) at Carnegie Mellon
 *	started
 */
/*
 *  Abstract:
 *	Routines to set and deallocate the mig reply port.
 *	They are called from mig generated interfaces.
 */

#include <mach.h>
#include <stdio.h>
#include <mach_error.h>

#ifndef mac2
static port_t	mig_reply_port;
#endif

void mig_init(first)
/*****************************************************
 *  Called by mach_init. This is necessary after
 *  a fork to get rid of bogus port  number.
 ****************************************************/
    int	first;
{
#ifdef mac2
	/* do nothing */
#else
	if (first == 0)
		mig_reply_port = PORT_NULL;
	else
	{   fprintf(stderr,"Calling non-threads version of mig_int\n");
	    fflush(stderr);
	}
#endif
}


port_t mig_get_reply_port()
/********************************************************
 *  Called by mig interfaces whenever they  need a reply port.
 *  Used to provide the same interface as multi-threaded tasks need.
 ********************************************************/
{
#ifdef mac2
	/* just return the thread reply port */
	return thread_reply();
#else
   kern_return_t	retcode;

 	if (mig_reply_port == PORT_NULL)
	{	mig_reply_port = thread_reply();
		retcode = port_allocate(task_self(), &mig_reply_port);
		if (retcode != KERN_SUCCESS)
		     mach_error("get_reply_port: failed allocating new reply port.",
				retcode);
	}
	return(mig_reply_port);
#endif
}

void mig_dealloc_reply_port()
/*************************************************************
 *  Called by mig interfaces after a timeout on the port.
 *  Could be called by user.
 ***********************************************************/
{
#ifdef mac2
	/* do nothing */
#else
   port_t		tmp_port;

	if (mig_reply_port != PORT_NULL)
	{	tmp_port = mig_reply_port;
		mig_reply_port = thread_reply();
		(void) port_deallocate(task_self(), tmp_port);
		mig_reply_port = PORT_NULL;
	}
#endif
}

