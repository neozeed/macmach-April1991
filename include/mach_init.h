/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY 
 * $Log:	mach_init.h,v $
 * Revision 1.1  90/02/18  17:48:11  bww
 * 	Mach Release 2.5
 * 	[90/02/18  17:47:49  bww]
 * 
 * Revision 1.3  89/06/13  16:45:00  mrt
 * 	Defined macros for thread_reply and made task_data be another
 * 	name for thread_reply, as task_data() is no longer exported from
 * 	the kernel.
 * 	[89/05/28            mrt]
 * 
 * 	Moved definitions of round_page and trunc_page to
 * 	here from mach/vm_param.h
 * 	[89/05/18            mrt]
 * 
 * Revision 1.2  89/05/05  18:45:39  mrt
 * 	Cleanup and change includes for Mach 2.5
 * 	[89/04/28            mrt]
 * 
 */
/*
 *	Items provided by the Mach environment initialization.
 */

#ifndef	_MACH_INIT_H_
#define	_MACH_INIT_H_	1

#include <mach/mach_types.h>

/*
 *	Kernel-related ports; how a task/thread controls itself
 */

#ifndef mac2_mpw_drvr

extern	port_t	task_self_;
extern	port_t	task_notify_;
extern  port_t	thread_reply_;
#define task_data_	thread_reply_

#define	task_self()	task_self_
#define	task_data()	thread_reply_
#define	thread_reply()	thread_reply_
#define	task_notify()	task_notify_

#endif /* mac2_mpw_drvr */

#define	current_task()	task_self()

/*
 *	Other important ports in the Mach user environment
 */

#define	NameServerPort	name_server_port	/* compatibility */

#ifndef mac2_mpw_drvr
extern	port_t	name_server_port;
extern	port_t	environment_port;
extern	port_t	service_port;
#endif /* mac2_mpw_drvr */

/*
 *	Where these ports occur in the "mach_ports_register"
 *	collection... only servers or the runtime library need know.
 */

#if	MACH_INIT_SLOTS
#define	NAME_SERVER_SLOT	0
#define	ENVIRONMENT_SLOT	1
#define SERVICE_SLOT		2

#define	MACH_PORTS_SLOTS_USED	3

#ifndef mac2_mpw_drvr
extern	port_array_t	mach_init_ports;
extern	unsigned int	mach_init_ports_count;
#endif /* mac2_mpw_drvr */
#endif	/* MACH_INIT_SLOTS */

/*
 *	Globally interesting numbers
 */

#ifndef mac2_mpw_drvr
extern	vm_size_t	vm_page_size;

#define round_page(x)	((((vm_offset_t)(x) + (vm_page_size - 1)) / vm_page_size) * vm_page_size)
#define trunc_page(x)	((((vm_offset_t)(x)) / vm_page_size) * vm_page_size)
#endif /* mac2_mpw_drvr */

#endif	/* _MACH_INIT_H_ */
