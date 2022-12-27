/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netname_aux.c,v $
 * Revision 1.1  90/02/19  21:48:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 */
/*
 *	File:	netname_aux.c
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Routine to call to wait for netmsgserver to start up.
 *
 */
#include <mach.h>
#include <servers/netname.h>

netname_wait_for_server()
{
	kern_return_t status;
	port_t port;

	for (;;) {
		status = netname_look_up(name_server_port, "", "NM_ACTIVE",
					 &port);
		if (status == KERN_SUCCESS) {
			/* port_deallocate(task_self, port); ??? */
			return;
		}
		sleep(30);
	}
}
