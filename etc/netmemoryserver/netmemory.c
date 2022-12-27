/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netmemory.c,v $
 * Revision 1.1  90/02/19  21:48:09  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/08/24  19:20:54  mrt
 * 	Added netmemory_cover routine. Removed MsgError definition. Made server
 * 	wait for netmsgserver before starting up. Added -debug switch which
 * 	registers server under different name. Random bug fixes.
 * 	[89/08/24            jsb]
 * 
 */
/*
 *	File:	netmemory.c
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Server loop for netmemory object creation and other operations.
 *
 */

#include "object.h"

static object_t *object_list = 0;

typedef struct registry registry_t;

struct registry {
    object_t *object;
    int kernel;
    port_t port;
    registry_t *next;
};

registry_t *registry;	/* owners without port deaths */
registry_t *unregistry;	/* port deaths without owners */

port_death_register(object, kernel, port)
object_t *object;
port_t port;
{
    registry_t *r, *r_prev;
    /*
     *  First check to see whether we have been waiting for someone to
     *  register this port.
     */
    for (r = unregistry, r_prev = 0; r; r_prev = r, r = r->next) {
	if (r->port == port) {
	    port_death(object->paging_object, object, kernel, port);
	    if (r_prev) {
		r_prev->next = r->next;
	    } else {
		unregistry = r->next;
	    }
	    free(r);
	    return;
	}
    }
    /*
     *  Nope, so just stick it in the registry.
     */
    r = (registry_t *) malloc(sizeof(registry_t));
    r->object = object;
    r->kernel = kernel;
    r->port = port;
    r->next = registry;
    registry = r;
}

handle_port_death(port)
port_t port;
{
    registry_t *r, *r_prev;

#if	DEBUG
	syslog(LOG_DEBUG, "$ port-death(0x%x)\n", port);
#endif	DEBUG
    /*
     *  First check to see whether it's in the registry.
     */
    for (r = registry, r_prev = 0; r; r_prev = r, r = r->next) {
	if (r->port == port) {
	    port_death(r->object->paging_object, r->object, r->kernel, port);
	    if (r_prev) {
		r_prev->next = r->next;
	    } else {
		unregistry = r->next;
	    }
	    free(r);
	    return;
	}
    }
    /*
     *  Nope, so just stick it in the unregistry.
     */
    r = (registry_t *) malloc(sizeof(registry_t));
    r->port = port;
    r->next = unregistry;
    unregistry = r;
}

free_object(object)
object_t *object;
{
    vm_deallocate(task_self(), object->faults_per_page,
	sizeof(struct fault_info) * (object->object_size >> MIN_PAGE_SHIFT));
    vm_deallocate(task_self(), object->storage, object->object_size);
    port_deallocate(task_self(), object->paging_object);
    free(object);
}

int netmemory_create(object_server, object_size, paging_object, control_port)
port_t object_server;
vm_size_t object_size;
port_t *paging_object;
port_t *control_port;
{
    return netmemory_cover(object_server, 0, object_size, paging_object,
			     control_port);
}

int netmemory_cover(object_server, object_data, object_size,
		      paging_object, control_port)
port_t object_server;
#ifdef __STDC__
pointer_t object_data;
unsigned int object_size;
#else
char *object_data;
vm_size_t object_size;
#endif
port_t *paging_object;
port_t *control_port;
{
    kern_return_t status;
    object_t *object;
    int object_server_routine();
    int kernel;
    kern_return_t pstatus;
    vm_size_t old_object_size;

    if (object_size <= 0) {
	return NETMEMORY_INVALID_ARGUMENT;
    }
    if (object_data && (object_size & (vm_page_size - 1))) {
	/* we know msg_receive actually gave us whole pages */
	object_size = round_page(object_size);
    }
    old_object_size = object_size;
    if (object_size & (MAX_PAGE_SIZE - 1)) {
	/* silently round up */
	object_size = (1 + (object_size >> MAX_PAGE_SHIFT)) << MAX_PAGE_SHIFT;
    }
    object = (object_t *) malloc(sizeof(object_t));
    if (object == 0) {
	syslog(LOG_ERR, "Could not allocate structure for new object\n");
	pstatus = NETMEMORY_RESOURCE_SHORTAGE;
	goto e0;
    }
    status = port_allocate(task_self(), &object->paging_object);
    if (status != KERN_SUCCESS) {
	syslog(LOG_ERR, "port_allocate in object server: %s\n", STATUS);
	pstatus = NETMEMORY_RESOURCE_SHORTAGE;
	goto e1;
    }
    status = port_allocate(task_self(), &object->control_port);
    if (status != KERN_SUCCESS) {
	syslog(LOG_ERR, "port_allocate in object server: %s\n", STATUS);
	pstatus = NETMEMORY_RESOURCE_SHORTAGE;
	goto e2;
    }
    port_set_backlog(task_self(), object->paging_object, PORT_BACKLOG_MAX);
    if (object_data) {
	object->storage = object_data;
	if (old_object_size < object_size) {
	    /* have to pad */
	    char *padding = object_data + old_object_size;
	    status = vm_allocate(task_self(), &padding,
			    object_size - old_object_size, FALSE);
	    if (status != KERN_SUCCESS) {
		/* couldn't pad; have to copy! */
		status = vm_allocate(task_self(), &object->storage,
				     object_size, TRUE);
		if (status != KERN_SUCCESS) {
		    syslog(LOG_ERR, "vm_allocate in object server: %s\n",
			   STATUS);
		    pstatus = NETMEMORY_RESOURCE_SHORTAGE;
		    goto e3;
		}
		status = vm_copy(task_self(), object_data, old_object_size,
				 object->storage);
		if (status != KERN_SUCCESS) {
		    syslog(LOG_ERR, "vm_copy in object server: %s\n", STATUS);
		    pstatus = status;
		    goto e3;
		}
		status = vm_deallocate(task_self(), object_data,
				       old_object_size);
		if (status != KERN_SUCCESS) {
		    syslog(LOG_ERR, "vm_deallocate in object server: %s\n",
			   STATUS);
		}
	    }
	}
    } else {
	status = vm_allocate(task_self(), &object->storage, object_size, TRUE);
	if (status != KERN_SUCCESS) {
	    syslog(LOG_ERR, "vm_allocate in object server: %s\n", STATUS);
	    pstatus = NETMEMORY_RESOURCE_SHORTAGE;
	    goto e3;
	}
    }
/* XXX set it to zero? and MAX, or MIN? and MAX_PAGE_SHIFT? */
    status = vm_allocate(task_self(), &object->faults_per_page,
	sizeof(struct fault_info) * (object_size >> MIN_PAGE_SHIFT), TRUE);
    if (status != KERN_SUCCESS) {
	syslog(LOG_ERR, "vm_allocate in object server: %s\n", STATUS);
	pstatus = NETMEMORY_RESOURCE_SHORTAGE;
	goto e4;
    }
    object->object_size = object_size;
/*
 *  XXXXXXXXXX DO THIS RIGHT? (Ie, a seperate thread?)
 */
    port_enable(task_self(), object->control_port);
/*
 *  XXXX
 */
    for (kernel = 0; kernel < MAX_NUM_KERNELS; kernel++) {
	object->kerninfo[kernel].request_port = PORT_NULL;
    }
    pstatus = Object_create(object);
    if (pstatus != NETMEMORY_SUCCESS) {
	goto e5;
    }
    cthread_detach(cthread_fork(object_server_routine, object));
    *paging_object = object->paging_object;
    *control_port = object->control_port;
    object->next = object_list;
    object_list = object;
    return NETMEMORY_SUCCESS;

e5: vm_deallocate(task_self(), object->faults_per_page,
    sizeof(struct fault_info) * (object_size >> MIN_PAGE_SHIFT));
e4: vm_deallocate(task_self(), object->storage, object_size);
e3: port_deallocate(task_self(), object->control_port);
e2: port_deallocate(task_self(), object->paging_object);
e1: free(object);
e0: *paging_object = PORT_NULL;
    return pstatus;
}

kern_return_t netmemory_destroy(control_port)
port_t control_port;
{
    object_t *object;
    object_t *previous_object;

    for (object = object_list, previous_object = 0;;) {
	if (object == 0) {
	    return NETMEMORY_INVALID_ARGUMENT;
	}
	if (control_port == object->control_port) {
	    break;
	}
	previous_object = object;
	object = object->next;
    }
    if (previous_object) {
	previous_object->next = object->next;
    } else {
	object_list = object->next;
    }
    object_thread_terminate(object->paging_object);
    Object_destroy(object);
    free_object(object);
    return NETMEMORY_SUCCESS;
}

kern_return_t netmemory_cache(object_server, paging_object,
				   local_paging_object)
port_t object_server;
port_t paging_object;
port_t *local_paging_object;
{
    /* eventually we will use this for amazingly clever performance gains */
    *local_paging_object = paging_object;
    return NETMEMORY_SUCCESS;
}

boolean_t paging_object_server_statistics(object_server, rusage, r_count,
					  faults_total, ft_count)
port_t object_server;
pointer_t *rusage, *faults_total;
unsigned int *r_count, *ft_count;
{
    object_t *object;
    static fault_info_t _total;
    static struct rusage _rusage;

    *rusage       = (pointer_t) &_rusage;
    *faults_total = (pointer_t) &_total;
    *r_count  = sizeof(_rusage);
    *ft_count = sizeof(_total);
    getrusage(RUSAGE_SELF, &_rusage);
    _total.read_faults = 0;
    _total.write_faults = 0;
    _total.protection_faults = 0;
    for (object = object_list; object; object = object->next) {
	_total.read_faults       += object->faults_total.read_faults;
	_total.write_faults      += object->faults_total.write_faults;
	_total.protection_faults += object->faults_total.protection_faults;
    }
    return TRUE;
}

sanity()
{
    if (MAX_NUM_KERNELS > 32 || MAX_NUM_KERNELS < 1) {
	syslog(LOG_ERR, "bad value of MAX_NUM_KERNELS in config.h\n");
	exit(1);
    }
    if (MIN_PAGE_SIZE != (1 << MIN_PAGE_SHIFT)) {
	syslog(LOG_ERR, "MIN_PAGE_SHIFT wrong in config.h\n");
	exit(1);
    }
    if (MAX_PAGE_SIZE != (1 << MAX_PAGE_SHIFT)) {
	syslog(LOG_ERR, "MAX_PAGE_SHIFT wrong in config.h\n");
	exit(1);
    }
}

create_nanny()
{
    int pid;
    union wait status;

    for (;;) {
	pid = fork();
	if (pid == 0) {
	    return;
	}
	while (wait(&status) != pid) {
	    ;
	}
	if (CORESIGNAL(status.w_termsig)) {
	    syslog(LOG_ERR,
		"netmemoryserver faulted (signal = %d) -- restarting\n",
		status.w_termsig);
	    sleep(1);	/* give time for cleanup to finish! */
	    continue;
	}
	if (status.w_termsig != 0) {
	    syslog(LOG_ERR, "netmemoryserver killed (signal = %d)\n",
		status.w_termsig);
	} else if (status.w_retcode == 0) {
	    syslog(LOG_ERR, "netmemoryserver exited normally.\n");
	} else {
	    syslog(LOG_ERR,
		"netmemoryserver exited abnormally (status = %d)\n",
		status.w_retcode);
	}
	if (status.w_termsig) {
	    kill(getpid(), status.w_termsig);
	} else {
	    exit(status.w_retcode);
	}
    }
}

main(argc, argv)
int argc;
char **argv;
{
    max_msg_t in_msg, out_msg;
    port_t notify_port;
    kern_return_t status;
    char *servername;

    if (argc > 1 && !strcmp(argv[1], "-debug")) {
	printf("debugging version of netmemoryserver\n");
	servername = SERVERDEBUGNAME;
    } else {
	servername = SERVERNAME;
    }
    netname_wait_for_server();
    status = netname_look_up(name_server_port, "", servername, &server_port);
    if (status == NETNAME_SUCCESS) {
	fprintf(stderr, "An instance of %s is already running.\n", servername);
	exit(1);
    }
    opensyslog();
    sanity();
    create_nanny();
    status = port_allocate(task_self(), &server_port);
    if (status != KERN_SUCCESS) {
	syslog(LOG_ERR, "port_allocate in object server: %s\n", STATUS);
	exit(1);
    }
    status = netname_check_in(name_server_port, servername, task_self(),
	server_port);
    if (status != NETNAME_SUCCESS) {
	syslog(LOG_ERR, "netname_check_in in object server: %s\n", STATUS);
	exit(1);
    }
    cthread_detach(cthread_fork(periodic_routine, &object_list));
    notify_port = task_notify();
    port_enable(task_self(), server_port);
    port_enable(task_self(), notify_port);
    for (;;) {
	in_msg.header.msg_local_port = PORT_ENABLED;
	in_msg.header.msg_size = sizeof(in_msg);
	status = msg_receive(&in_msg.header, MSG_OPTION_NONE, 0);
	if (status != RCV_SUCCESS) {
	    syslog(LOG_ERR, "msg_receive in object server: %s\n", STATUS);
	    exit(1);
	}
	if (in_msg.header.msg_local_port == notify_port) {
	    if (in_msg.header.msg_id == NOTIFY_PORT_DELETED) {
		handle_port_death(((notification_t *) &in_msg)->notify_port);
	    } else {
		syslog(LOG_ERR,
		  "object server could not identify notification message %d\n",
		  in_msg.header.msg_id);
	    }
	} else {
	    if (netmemory_server(&in_msg.header, &out_msg.header)) {
		if (((reply_t *) &out_msg)->return_code != MIG_NO_REPLY) {
		    msg_send(&out_msg.header, MSG_OPTION_NONE, 0);
		}
		port_deallocate(task_self(), in_msg.header.msg_remote_port);
	    } else if (internal_server(&in_msg.header, &out_msg.header)) {
		if (((reply_t *) &out_msg)->return_code != MIG_NO_REPLY) {
		    msg_send(&out_msg.header, MSG_OPTION_NONE, 0);
		}
	    } else {
		syslog(LOG_ERR, "object server received unrecognized rpc\n");
	    }
	}
    }
}
