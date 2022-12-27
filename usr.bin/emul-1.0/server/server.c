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
 *	File: emul/server/server.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"

#include <cthreads.h>

#include <mig_errors.h>

#include <mach/notify.h>

#include "mac.h"

static port_set_name_t	except_port_set, pager_port_set;
static port_name_t	exception_port;
static port_name_t	util_port;
port_name_t		kernel_request;
memory_object_t		memory_object;

any_t	server_loop();

void
server_main()
{
    port_name_t		old_exception_port;
    kern_return_t	result;

    result = port_set_allocate(task_self(), &except_port_set);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_allocate", result);

    result = port_set_add(task_self(), except_port_set, task_notify());
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_add notify port", result);

    result = port_set_allocate(task_self(), &pager_port_set);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_allocate", result);

    result = port_set_add(task_self(), pager_port_set, task_notify());
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_add notify port", result);

    result = task_get_exception_port(task, &old_exception_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("task_get_special_port exception port", result);
    result = env_set_port(environment_port, "except", old_exception_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("env_set_port except port", result);

    result = port_allocate(task_self(), &exception_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_allocate exception port", result);
    result = port_set_add(task_self(), except_port_set, exception_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_add exception port", result);
    result = task_set_exception_port(task, exception_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("task_set_special_port exception port", result);

    result = port_allocate(task_self(), &util_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_allocate util port", result);
    result = port_set_add(task_self(), except_port_set, util_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_add util port", result);
    result = env_set_port(environment_port, "util", util_port);
    if (result != KERN_SUCCESS)
	mach_error_exit("env_set_port util port", result);

    result = port_allocate(task_self(), &memory_object);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_allocate memory_object", result);
    result = port_set_add(task_self(), pager_port_set, memory_object);
    if (result != KERN_SUCCESS)
	mach_error_exit("port_set_add memory_object", result);

    {
	port_array_t	mach_ports;
	unsigned 	mach_ports_count;
	port_t		server_port;

	result = mach_ports_lookup(task_self(),
				   &mach_ports,
				   &mach_ports_count);
	if (result != KERN_SUCCESS)
	    mach_error_exit("mach_ports_lookup", result);

	server_port = mach_ports[3];	/* XXX */

	mach_ports[3] = PORT_NULL;
	result = mach_ports_register(task,
				     mach_ports,
				     mach_ports_count);

	vm_deallocate(task_self(),
		      (vm_offset_t)mach_ports,
		      mach_ports_count * sizeof (port_t));

	result = mac_create(server_port, thread, &kernel_request);
	if (result != KERN_SUCCESS)
	    mach_error_exit("mac_create", result);

	result = env_set_port(environment_port,
			      "kernel_request",
			      kernel_request);
	if (result != KERN_SUCCESS)
	    mach_error_exit("env_set_port kernel request port", result);
    }

    setup_mappings();

    console_redirect();

    open_input();

    cthread_detach(cthread_fork(server_loop,
				(any_t) except_port_set)
		   );

    cthread_detach(cthread_fork(server_loop,
				(any_t) pager_port_set)
		   );

    task_resume(task);

    for (;;)
	sigpause(0);
}

any_t
server_loop(port_set)
port_set_name_t		port_set;
{
    msg_return_t	result;
    static struct {
	msg_header_t	head;
	char	body[MSG_SIZE_MAX - sizeof (msg_header_t)];
    } recv_buf, reply_buf;
    msg_header_t	*recv_msg = &recv_buf.head;
    death_pill_t	*reply_msg = (death_pill_t *)&reply_buf.head;

    for (;;) {
	recv_msg->msg_size = sizeof (recv_buf);
	recv_msg->msg_local_port = port_set;

	result = msg_receive(recv_msg, MSG_OPTION_NONE, 0);
	if (result != RCV_SUCCESS)
	    other_error_exit("msg_receive error");

	if (recv_msg->msg_local_port == exception_port)
	    (void) exc_server(recv_msg, reply_msg);
	else if (recv_msg->msg_local_port == task_notify())
	    (void) notify_(recv_msg, reply_msg);
	else if (recv_msg->msg_local_port == memory_object)
	    (void) memory_object_server(recv_msg, reply_msg);
	else if (recv_msg->msg_local_port == util_port)
	    (void) util_server(recv_msg, reply_msg);
	else
	    other_error_exit("msg_receive unknown local port");

	if (reply_msg->RetCode != MIG_NO_REPLY)
	    (void) msg_send(&reply_msg->Head, MSG_OPTION_NONE, 0);
    }
}

notify_(notification, reply)
notification_t	*notification;
death_pill_t	*reply;
{
    switch (notification->notify_header.msg_id) {
      case NOTIFY_PORT_DELETED:
	if (notification->notify_port == task)
	    normal_exit();
	/* FALL THROUGH */

      default:
	fprintf(stderr, "notification type %o port %x\n",
		notification->notify_header.msg_id,
		notification->notify_port);
    }

    reply->RetCode = MIG_NO_REPLY;
}
