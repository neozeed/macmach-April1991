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
 * $Log:	mach_init.c,v $
 * Revision 1.1  90/02/19  18:08:20  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.3  89/06/13  16:07:25  mrt
 * 	Changed references to task_data to thread_reply as task_data() is
 * 	no longer exported from the kernel. Removed setting of
 * 	NameServerPort, the correct name is name_server_port.
 * 	[89/05/28            mrt]
 * 
 * Revision 1.2  89/05/05  18:45:30  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 23-Nov-87  Mary Thompson (mrt) at Carnegie Mellon
 *	removed includes of <servers/msgn.h> and <servers/netname.h>
 *	as they are no longer used.
 *
 * 5-Oct-87   Mary Thompson (mrt) at Carnegie Mellon
 *	Added an extern void definition of mig_init to keep
 *	lint happy
 *
 * 30-Jul-87  Mary Thompson (mrt) at Carnegie Mellon
 *	Changed the intialization of the mig_reply_port to be
 *	a call to mig_init instead init_mach.
 *
 */

#define	MACH_INIT_SLOTS		1
#include <mach_init.h>
#include <mach.h>

extern void init_reply_port();
extern void mig_init();

#ifndef mac2_mpw_drvr
port_t		task_self_ =  PORT_NULL;
port_t		task_notify_ =  PORT_NULL;
port_t		thread_reply_ = PORT_NULL;

port_t		name_server_port = PORT_NULL;
port_t		environment_port = PORT_NULL;
port_t		service_port = PORT_NULL;

vm_size_t	vm_page_size;


port_array_t	mach_init_ports;
unsigned int	mach_init_ports_count;

#endif /* mac2_mpw_drvr */

int		mach_init()
{
#ifndef mac2_mpw_drvr
	vm_statistics_data_t vm_stat;

	/*
	 * undefine the macros defined in mach_init.h so that we
	 * can make the real kernel calls
	 */

#undef task_self()
#undef thread_reply()
#undef task_notify()

	/*
	 *	Get the important ports into the cached values,
	 *	as required by "mach_init.h".
	 */
	 
	task_self_ = task_self();
	thread_reply_ = thread_reply();
	task_notify_ = task_notify();

	/*
	 *	Initialize the single mig reply port
	 */

	mig_init(0);

	/*
	 *	Cache some other valuable system constants
	 */

	vm_statistics(task_self_, &vm_stat);
	vm_page_size = vm_stat.pagesize;
	
	/*
	 *	Find those ports important to every task.
	 */
	 
	if (mach_ports_lookup(task_self_, &mach_init_ports, &mach_init_ports_count) != KERN_SUCCESS)
		mach_init_ports_count = 0;

	name_server_port = mach_init_ports_count > (unsigned int)NAME_SERVER_SLOT 
			? mach_init_ports[NAME_SERVER_SLOT] : PORT_NULL;
	environment_port = mach_init_ports_count > ENVIRONMENT_SLOT ? mach_init_ports[ENVIRONMENT_SLOT] : PORT_NULL;
	service_port     = mach_init_ports_count > SERVICE_SLOT ? mach_init_ports[SERVICE_SLOT] : PORT_NULL;
	

	/* get rid of out-of-line data so brk has a chance of working */
	(void) vm_deallocate(task_self(),(vm_address_t)mach_init_ports,
				vm_page_size);


#endif /* mac2_mpw_drvr */
	return(0);
}

#ifndef mac2_mpw_drvr
int		(*mach_init_routine)() = mach_init;
#endif /* mac2_mpw_drvr */

#ifndef	lint
/*
 *	Routines which our library must suck in, to avoid
 *	a later library from referencing them and getting
 *	the wrong version.
 */
static replacements()
{
	sbrk(0);
	fork();
}
#endif	lint
