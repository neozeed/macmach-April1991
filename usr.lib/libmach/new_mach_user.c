/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	new_mach_user.c,v $
 * Revision 1.2  90/02/19  18:31:07  bww
 * 	#undef LINTLIBRARY from Mary Thompson.
 * 	[90/02/19  18:29:33  bww]
 * 
 * Revision 1.1  90/02/19  18:08:40  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:46:41  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*
 *	Each of the following routines is replaced by a pretty
 *	compatible version.  For these, we try the new interface
 *	routine, then the old one.
 */

#undef LINTLIBRARY

#include <mach/mach_types.h>
#include <mig_errors.h>


kern_return_t port_allocate(task, port_name)
	task_t		task;
	port_name_t	*port_name;
{
	kern_return_t	result;
	extern
	kern_return_t	NEW_port_allocate();
	extern
	kern_return_t	xxx_port_allocate();

	if ((result = NEW_port_allocate(task, port_name)) == MIG_BAD_ID)
		result = xxx_port_allocate(task, port_name);
	return(result);
}

kern_return_t port_deallocate(task, port_name)
	task_t		task;
	port_name_t	port_name;
{
	kern_return_t	result;
	extern
	kern_return_t	NEW_port_deallocate();
	extern
	kern_return_t	xxx_port_deallocate();


	if ((result = NEW_port_deallocate(task, port_name)) == MIG_BAD_ID)
		result = xxx_port_deallocate(task, port_name);
	return(result);
}

kern_return_t port_set_backlog(task, port_name, backlog)
	task_t		task;
	port_name_t	port_name;
	int		backlog;
{
	kern_return_t	result;
	extern
	kern_return_t	NEW_port_set_backlog();
	extern
	kern_return_t	xxx_port_set_backlog();


	if ((result = NEW_port_set_backlog(task, port_name, backlog)) == MIG_BAD_ID)
		result = xxx_port_set_backlog(task, port_name, backlog);
	return(result);
}

#define	port_allocate	NEW_port_allocate
#define	port_deallocate	NEW_port_deallocate
#define	port_set_backlog	NEW_port_set_backlog

/*
 *	These should be replaced by user-level support.  We should
 *	try the new replacement code first, then back off to the
 *	old interface routine.
 *
 *	Before we can really enable this change, we have to get
 *	the include files with the new declaration of PORT_ENABLED and
 *	PORT_DEFAULT out.
 *
 *	NOTES:
 *		There's no synchronization among multiple uses of these routines.
 *		The values returned by the new port_set routines may be
 *		 unexpected by programs expecting the old semantics.  This
 *		 can probably be corrected if anyone cares.
 */

port_t		PORT_ENABLED = (-1);

kern_return_t	port_enable(target_task, port)
	task_t		target_task;
	port_t		port;
{
	extern
	kern_return_t	port_set_add();
	extern
	kern_return_t	xxx_port_enable();

#if	PORT_ENABLED_IS_A_VARIABLE
	extern
	kern_return_t	port_set_allocate();

	if (PORT_ENABLED == (-1))
		(void) port_set_allocate(target_task, &PORT_ENABLED);
#endif	PORT_ENABLED_IS_A_VARIABLE

	return((PORT_ENABLED == (-1)) ?
		xxx_port_enable(target_task, port) :
	 	port_set_add(target_task, port, PORT_ENABLED)
		);
}

kern_return_t	port_disable(target_task, port)
	task_t		target_task;
	port_t		port;
{
	extern
	kern_return_t	port_set_remove();
	extern
	kern_return_t	xxx_port_disable();

#if	PORT_ENABLED_IS_A_VARIABLE
	extern
	kern_return_t	port_set_allocate();

	if (PORT_ENABLED == (-1))
		(void) port_set_allocate(target_task, &PORT_ENABLED);
#endif	PORT_ENABLED_IS_A_VARIABLE

	return((PORT_ENABLED == (-1)) ?
		xxx_port_disable(target_task, port) :
	 	port_set_remove(target_task, port)
		);
}

/*
 *	This routine exists in the new library, but has
 *	different calling sequence and semantics.
 *
 *	Any old users lose totally.
 */

/* port_status */

/*
 *	This routine can't even be easily emulated in
 *	the new library.  If old users complain loudly,
 *	we may be able to return it to service.
 */

/* port_status */


#include "mach_user.c"
