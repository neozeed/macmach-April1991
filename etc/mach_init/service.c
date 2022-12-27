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
 * 17-Oct-90  David Bohman (bohman) at Carnegie-Mellon University
 *	Slots above MACH_PORTS_SLOTS_USED which already exist after
 *	the initial mach_ports_lookup() are no longer thrown away.  Instead
 *	they are marked as 'taken'.  This is so that a kernel mode server
 *	can pass a port to tasks below /etc/init.  A backup port is still
 *	created and added to the receive port set.  Presumably, if the
 *	kernel server port went away, this would revert to the
 *	original behavior.
 *
 * $Log:	service.c,v $
 * Revision 1.1  90/02/19  18:45:56  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:45:49  bww]
 * 
 * Revision 1.3  89/04/29  13:34:43  mrt
 * 	Fixed includes for Mach 2.5
 * 
 *  7-Feb-89  Richard P. Draves (rpd) at Carnegie-Mellon University
 *	Rewritten to use backup ports instead of splitting
 *	receive_rights and ownwership_rights between servers
 *	and the service server.
 *
 */
/*
 *	Program:	Service server
 *
 *	Purpose:
 *		Create ports for globally interesting services,
 *		and hand the receive rights to those ports (i.e.
 *		the ability to serve them) to whoever asks.
 *
 *	Why we need it:
 *		We need to get the service ports into the
 *		very top of the task inheritance structure,
 *		but the currently available system startup
 *		mechanism doesn't allow the actual servers
 *		to be started up from within the initial task
 *		itself.  We start up as soon as we can, and
 *		force the service ports back up the task tree,
 *		and let servers come along later to handle them.
 *
 *		In the event that a server dies, a new instantiation
 *		can grab the same service port.
 *
 *	Can be run before /etc/init instead, if desired.
 */

/*
 * We must define MACH_INIT_SLOTS and include <mach_init.h> first,
 * to make sure SERVICE_SLOT gets defined.
 */
#define MACH_INIT_SLOTS		1
#include <mach_init.h>

#include <stdio.h>
#include <mach/boolean.h>
#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>

extern boolean_t debug;

extern boolean_t service_server();

void process_rcv_notify();
void process_backup_notify();
void msg_destroy();

/*
 * We provide backwards compatibility for old kernels and old clients.
 * Old kernels don't implement port_set_backup.  Old clients expect
 * to get receive rights (MSG_TYPE_PORT_RECEIVE), not MSG_TYPE_PORT_ALL.
 *
 * On an old kernel, compat_mode will get set to true.  Old clients
 * (on old and new kernels) will use old_service_checkin.
 */

boolean_t compat_mode = FALSE;

/*
 * Each service port has a separate backup port.  This is more secure
 * than using the same backup port (say, task_notify()) for all.
 * This is because a task that picks up receive rights to a service port
 * can use port_set_backup to extract send rights to the backup.
 */

typedef struct service_record {
	port_name_t service;
	port_name_t backup;
	boolean_t taken;
} service_record_t;

service_record_t *services;
int services_count;

void
serv_init(parent)
	task_t parent;
{
	port_name_t *ports;
	unsigned int portsCnt;

	int i;
	kern_return_t kr;

	/*
	 *	See what our parent had.  We are interested
	 *	in the count, not the ports themselves.
	 *	However, we save the memory for use below.
	 */

	kr = mach_ports_lookup(parent, &ports, &portsCnt);
	if (kr != KERN_SUCCESS)
		barf("can't lookup ports from parent");

	services_count = portsCnt;

	if (debug)
		printf("Got %d ports from lookup.\n", services_count);

	if (services_count <= SERVICE_SLOT)
		barf("no room for SERVICE_SLOT???");

	/*
	 *	Create ports for the various services.
	 *	We will have receive rights for these ports.
	 */

	services = (service_record_t *)
			malloc((unsigned) services_count,
			       (unsigned) sizeof(service_record_t));
	if (services == NULL)
		barf("can't allocate services array");

	for (i = 0; i < services_count; i++) {
		port_name_t service, backup;

		services[i].taken = FALSE;

		if (i >= MACH_PORTS_SLOTS_USED && ports[i] != PORT_NULL) {
		    service = ports[i];
		    services[i].taken = TRUE;
		}
		else {
		    kr = port_allocate(task_self(), &service);
		    if (kr != KERN_SUCCESS)
			barf("can't allocate service port %d", i);
		}

		kr = port_allocate(task_self(), &backup);
		if (kr != KERN_SUCCESS)
			barf("can't allocate backup port %d", i);

		if (debug)
			printf("Slot %d: service = %d, backup = %d.\n",
			       i, service, backup);

		if (!compat_mode) {
			port_name_t previous;

			kr = port_set_backup(task_self(), service,
					     backup, &previous);
			if (kr == KERN_SUCCESS) {
				if (previous != PORT_NULL)
					barf("previous not NULL???");
			} else
				compat_mode = TRUE;
		}

		ports[i] = service;
		services[i].service = service;
		services[i].backup = backup;
	}

	/*
	 *	Remember our service port, and mark it as taken.
	 */

	service_port = services[SERVICE_SLOT].service;
	services[SERVICE_SLOT].taken = TRUE;

	/*
	 *	Check the ports into our parent task.
	 */

	kr = mach_ports_register(parent, ports, (unsigned) services_count);
	if (kr != KERN_SUCCESS)
		barf("can't register ports in parent");

	kr = vm_deallocate(task_self(), (vm_address_t) ports,
			   (vm_size_t) (services_count*sizeof(port_name_t)));
	if (kr != KERN_SUCCESS)
		barf("can't deallocate memory");
}

void
serv_loop()
{
	int i;
	port_set_name_t pset;
	kern_return_t kr;

	if (compat_mode) {
		pset = PORT_ENABLED;

		kr = port_enable(task_self(), task_notify());
		if (kr != KERN_SUCCESS)
			barf("couldn't add notify port");

		kr = port_enable(task_self(), service_port);
		if (kr != KERN_SUCCESS)
			barf("couldn't add service port");

		/* Deallocate the unused backup ports. */

		if (debug)
			printf("Compat Mode: deallocating backups.\n");

		for (i = 0; i < services_count; i++) {
			kr = port_deallocate(task_self(), services[i].backup);
			if (kr != KERN_SUCCESS)
				barf("couldn't deallocate backup port %d", i);
			services[i].backup = PORT_NULL;
		}
	} else {
		kr = port_set_allocate(task_self(), &pset);
		if (kr != KERN_SUCCESS)
			barf("couldn't allocate port set");

		kr = port_set_add(task_self(), pset, task_notify());
		if (kr != KERN_SUCCESS)
			barf("couldn't add notify port");

		kr = port_set_add(task_self(), pset, service_port);
		if (kr != KERN_SUCCESS)
			barf("couldn't add service port");

		for (i = 0; i < services_count; i++) {
			kr = port_set_add(task_self(),
					  pset, services[i].backup);
			if (kr != KERN_SUCCESS)
				barf("couldn't add backup port %d", i);
		}
	}

	/*
	 *	Wait for requests to serve these ports
	 */

	for (;;) {
		union {
			msg_header_t	msg_header;
			char		msg_body[MSG_SIZE_MAX];
		} message, reply;
		port_name_t lport;

		/*
		 *	Pick a message, any message.
		 */

		message.msg_header.msg_local_port = pset;
		message.msg_header.msg_size = sizeof(message);

		kr = msg_receive(&message.msg_header, MSG_OPTION_NONE, 0);
		if (kr != KERN_SUCCESS) {
			barf("receive error %d", kr);
			/* should exit */
			continue;
		}

		lport  = message.msg_header.msg_local_port;

		if (debug)
			printf("Received message on %d.\n", lport);

		if (lport == task_notify())
			process_rcv_notify((notification_t *) &message);
		else if (lport == service_port) {
			if (service_server(&message.msg_header,
					   &reply.msg_header))
				if (debug)
					printf("Handled request.\n");

			if (reply.msg_header.msg_remote_port != PORT_NULL) {
				(void) msg_send(&reply.msg_header,
						SEND_TIMEOUT, 0);
				if (debug)
					printf("Reply sent.\n");
			}
		} else {
			for (i = 0; i < services_count; i++)
				if (services[i].backup == lport)
					break;

			if (i == services_count)
				barf("unknown local port %d", lport);
			else
				process_backup_notify(i,
					(notification_t *) &message);
		}

		msg_destroy(&message.msg_header);
	}
}

void
process_rcv_notify(not)
	notification_t *not;
{
	port_name_t returned;
	int i;

	switch (not->notify_header.msg_id) {
	    case NOTIFY_RECEIVE_RIGHTS:
		break;

	    case NOTIFY_PORT_DELETED:
		return;

	    case NOTIFY_PORT_DESTROYED:
	    case NOTIFY_MSG_ACCEPTED:
	    case NOTIFY_OWNERSHIP_RIGHTS:
		barf("strange notification %d", not->notify_header.msg_id);
		return;

	    default:
		barf("unknown notification %d", not->notify_header.msg_id);
		return;
	}

	returned = not->notify_port;

	for (i = 0; i < services_count; i++)
		if (services[i].service == returned)
			break;

	if (i == services_count) {
		barf("strange port %d returned in rcv notification", returned);
		return;
	}

	if (debug)
		printf("Port %d (slot %d) is returned via rcv notify.\n",
		       returned, i);

	services[i].taken = FALSE;
}

void
process_backup_notify(i, not)
	int i;
	notification_t *not;
{
	kern_return_t kr;
	port_name_t returned, previous;

	if (compat_mode)
		barf("Backup port in compat_mode???");

	/*
	 * Must be paranoid and check the notification carefully,
	 * because an evil user could try to spoof us.
	 */

	if ((not->notify_header.msg_simple != FALSE) ||
	    (not->notify_header.msg_id != NOTIFY_PORT_DESTROYED) ||
	    (not->notify_header.msg_size != sizeof(notification_t)) ||
	    (not->notify_type.msg_type_longform != FALSE) ||
	    (not->notify_type.msg_type_inline != TRUE) ||
	    (not->notify_type.msg_type_name != MSG_TYPE_PORT_ALL) ||
	    (not->notify_type.msg_type_size != 32) ||
	    (not->notify_type.msg_type_number != 1) ||
	    (not->notify_port != services[i].service)) {
		if (debug)
			printf("Something funny sent to backup port.\n");
		return;
	}

	returned = not->notify_port;

	if (debug)
		printf("Port %d (slot %d) is returned via backup.\n",
		       returned, i);

	services[i].taken = FALSE;

	kr = port_set_backup(task_self(), returned,
			     services[i].backup, &previous);
	if (kr != KERN_SUCCESS)
		barf("couldn't reinstall backup for %d", returned);

	if (previous != PORT_NULL)
		barf("backup already in place??");
}

kern_return_t
do_checkin(requested, granted)
	port_name_t requested;
	port_name_t *granted;
{
	int i;

	*granted = PORT_NULL;

	for (i = 0; i < services_count; i++)
		if (services[i].service == requested)
			break;

	if (i == services_count)
		return KERN_FAILURE;

	if (services[i].taken)
		return KERN_FAILURE;

	if (debug)
		printf("Port %d (slot %d) is taken.\n",
		       requested, i);

	services[i].taken = TRUE;
	*granted = requested;
	return KERN_SUCCESS;
}

/*
 *	Routine:	service_checkin
 *	Function:
 *		This is the routine the MiG interface calls...
 *		it merely returns the requested port (as granted).
 */
kern_return_t
service_checkin(request, requested, granted)
	port_name_t request;
	port_name_t requested;
	port_name_t *granted;
{
	/*
	 * If we are on an old kernel, then if the client dies,
	 * we won't get this service port back; it will die.
	 */

	if (compat_mode && debug)
		printf("Warning: new client, old kernel.");

	return do_checkin(requested, granted);
}

/*
 *	Routine:	oservice_checkin
 *	Function:
 *		This is the routine the old MiG interface calls...
 *		it merely returns the requested port (as granted).
 */
kern_return_t
old_service_checkin(request, requested, granted)
	port_name_t request;
	port_name_t requested;
	port_name_t *granted;
{
	return do_checkin(requested, granted);
}

void
msg_destroy_port(p)
	port_name_t p;
{
	int i;

	if ((p == PORT_NULL) ||
	    (p == task_self()) ||
	    (p == task_notify()))
		return;
	for (i = 0; i < services_count; i++)
		if ((p == services[i].service) ||
		    (p == services[i].backup))
			return;
	if (debug)
		printf("Deallocating port %d.\n", p);
	(void) port_deallocate(task_self(), p);
}

void
msg_destroy(m)
	msg_header_t *m;
{
	msg_destroy_port(m->msg_local_port);
	msg_destroy_port(m->msg_remote_port);

	/* should check for other ports, memory in message */
}
