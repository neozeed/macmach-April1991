/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netmemory_aux.c,v $
 * Revision 1.2  90/02/23  19:00:04  bww
 * 	Get netmemory.h from servers directory.
 * 	[90/02/23  18:59:40  bww]
 * 
 * Revision 1.1  90/02/19  21:48:17  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 2.1  89/08/24  17:51:59  mrt
 * Created.
 * 
 */
/*
 *	File:	netmemory_aux.c
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Easier-to-use interface to the netmemoryserver.
 *
 */

#include <mach.h>
#include <stdio.h>
#include <servers/netname.h>
#include <servers/netmemory.h>

#define	SERVERNAME	"netmemoryserver"
#define	SERVERDEBUGNAME	"netmemoryserver-debug"

int __use_debug_nmserver__ = 0;

kern_return_t
netmemory_look_up_server(nm_portp)
port_t *nm_portp;
{
	static port_t nm_port = PORT_NULL;
	kern_return_t status = KERN_SUCCESS;
	char *servername;

	if (__use_debug_nmserver__) {
		servername = SERVERDEBUGNAME;
	} else {
		servername = SERVERNAME;
	}
	if (nm_port == PORT_NULL) {
		netname_wait_for_server();
		status = netname_look_up(name_server_port, "",
			servername, &nm_port);
		if (status) nm_port = PORT_NULL;
	}
	*nm_portp = nm_port;
	if (status == NETNAME_NOT_CHECKED_IN) {
		status = KERN_FAILURE;
	}
	return status;
}

kern_return_t
netmemory_check_in(name, size, addrp, nm_objectp, nm_controlp, nm_localp,
		   control_name)
char *name;
int size;
char **addrp;
port_t *nm_objectp;
port_t *nm_controlp;
port_t *nm_localp;
char *control_name;
{
	port_t nm_server;
	port_t nm_object;
	port_t nm_control;
	port_t nm_local_object;
	kern_return_t status;

	status = netmemory_look_up_server(&nm_server);
	if (status) {
		return status;
	}
	status = netname_look_up(name_server_port, "", name, &nm_object);
	if (status == NETNAME_SUCCESS) return NETNAME_NOT_YOURS;
	status = netmemory_create(nm_server, size, &nm_object,
				  &nm_control);
	if (status) return status;
	if (name) {
		status = netname_check_in(name_server_port, name,
					  PORT_NULL, nm_object);
		if (status) return status;
	}
	if (control_name) {
		status = netname_check_in(name_server_port, control_name,
					  PORT_NULL, nm_control);
		if (status) return status;
	}
	if (*addrp != (char *)1 || nm_localp) {
		status = netmemory_cache(nm_server, nm_object,
					 &nm_local_object);
		if (status) return status;
	}
	if (*addrp != (char *)1) {
		boolean_t anywhere = (*addrp == 0);
		status = vm_map(task_self(), addrp, size, 0,
			anywhere, nm_local_object, 0, FALSE,
			VM_PROT_ALL, VM_PROT_ALL, VM_INHERIT_SHARE);
		if (status) return status;
	}
	if (nm_objectp) *nm_objectp = nm_object;
	if (nm_controlp) *nm_controlp = nm_control;
	if (nm_localp) *nm_localp = nm_local_object;
	return KERN_SUCCESS;
}

kern_return_t
netmemory_look_up(name, host, size, addrp, nm_objectp, nm_controlp, nm_localp,
	  control_name)
char *name;
char *host;
int size;
char **addrp;
port_t *nm_objectp;
port_t *nm_controlp;
port_t *nm_localp;
char *control_name;
{
	port_t nm_server;
	port_t nm_object;
	port_t nm_local_object;
	kern_return_t status;

	status = netmemory_look_up_server(&nm_server);
	if (status) return status;
	if (host == 0) host = ""; /* default to local host only */
	if (name) {
		status = netname_look_up(name_server_port, host, name,
					 &nm_object);
		if (status) return status;
	}
	if (control_name && nm_controlp) {
		status = netname_look_up(name_server_port, host,
					  control_name, nm_controlp);
		if (status) return status;
	}
	if (*addrp != (char *)1 || nm_localp) {
		status = netmemory_cache(nm_server, nm_object,
					 &nm_local_object);
		if (status) return status;
	}
	if (*addrp != (char *)1) {
		boolean_t anywhere = (*addrp == 0);
		status = vm_map(task_self(), addrp, size, 0,
			anywhere, nm_local_object, 0, FALSE,
			VM_PROT_ALL, VM_PROT_ALL, VM_INHERIT_SHARE);
		if (status) return status;
	}
	if (nm_objectp) *nm_objectp = nm_object;
	if (nm_localp) *nm_localp = nm_local_object;
	return KERN_SUCCESS;
}
