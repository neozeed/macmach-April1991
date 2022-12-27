/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	object.c,v $
 * Revision 1.1  90/02/19  21:48:31  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/08/24  19:21:07  mrt
 * 	Fixed paging_object_statistics.
 * 	[89/08/24            jsb]
 * 
 * Revision 1.3  89/05/03  17:43:51  mrt
 * 	Added stub for memory_object_copy
 * 	[89/05/03            jsb]
 * 
 *
 *  22-May-88  Alessandro Forin (af) at Carnegie-Mellon University
 *	Various minor mods, moved management of memory_object_data_provided
 *	messages inside the scheduler(s).
 *
 *   5-Nov-87 Joseph Barrera (jsb) at Carnegie-Mellon University
 *	Created.
 */
/*
 *	File:	object.c
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Server for memory_object.defs and internal.defs.
 *
 */

#include "object.h"

object_server_routine(object)
object_t *object;
{
    max_msg_t in_msg, out_msg;
    kern_return_t status;

    cthread_set_data(cthread_self(), (any_t) object);
    for (;;) {
	in_msg.header.msg_local_port = object->paging_object;
	in_msg.header.msg_size = sizeof(in_msg);
	status = msg_receive(&in_msg.header, MSG_OPTION_NONE, 0);
	if (status != RCV_SUCCESS) {
	    syslog(LOG_ERR, "msg_receive from object: %s\n", STATUS);
	    cthread_exit(0);
	}
	if (memory_object_server(&in_msg.header, &out_msg.header)) {
	    if (((reply_t *) &out_msg)->return_code != MIG_NO_REPLY) {
		msg_send(&out_msg.header, MSG_OPTION_NONE, 0);
	    }
	} else if (internal_server(&in_msg.header, &out_msg.header)) {
	    if (((reply_t *) &out_msg)->return_code != MIG_NO_REPLY) {
		msg_send(&out_msg.header, MSG_OPTION_NONE, 0);
	    }
	} else if (netmemstat_server(&in_msg.header, &out_msg.header)) {
	    if (((reply_t *) &out_msg)->return_code != MIG_NO_REPLY) {
		msg_send(&out_msg.header, MSG_OPTION_NONE, 0);
	    }
	} else {
	    syslog(LOG_ERR, "paging object received unrecognized rpc\n");
	}
    }
}

/*
 *  Since cthread_self is currently a linear operation on the number
 *  of cthreads, we may want to do something faster ourselves.
 *
 *  (There is always the sleaze option: replace the port with
 *  the object before the call to memory_object_server...)
 *
 *  This is already pretty sleazy because it does not reallly do
 *  what it claims to do. But at least it avoids locking.
 */
object_t *port_to_object(port)
port_t port;
{
#ifdef	lint
    port++;
#endif	lint
    return (object_t *) cthread_data(cthread_self());
}

/*
 *  Make sure that this translates PORT_NULL into a free slot, or -1
 *  if there is none.
 *
 *  A kernel slot is used iff the request_port field of that slot
 *  is not PORT_NULL. The port_death routine understands this.
 */
int port_to_kernel(object, port)
object_t *object;
port_t port;
{
    int kernel;

    for (kernel = 0; kernel < MAX_NUM_KERNELS; kernel++) {
	if (object->kerninfo[kernel].request_port == port) {
	    return kernel;
	}
    }
    return -1;
}

/******************************************************************************
 *  Routines from pager.defs:
 *	memory_object_init
 *	memory_object_data_request
 *	memory_object_data_unlock
 *	memory_object_data_write
 *	memory_object_lock_completed
 ******************************************************************************
 */

memory_object_init(paging_object, memory_control, memory_object_name, page_size)
paging_object_t paging_object;
port_t memory_control, memory_object_name;
vm_size_t page_size;
{
    object_t *object;
    int kernel;

#if	DEBUG
syslog(LOG_DEBUG, "$ memory_object_init(0x%x, 0x%x, 0x%x, %d)\n",
	paging_object, memory_control, memory_object_name, page_size);
#endif	DEBUG
    if (memory_object_name != PORT_NULL) {
	port_deallocate(task_self(), memory_object_name);
    }
    if (memory_control == PORT_NULL) {
	syslog(LOG_ERR, "null memory_control in memory_object_init\n");
    	return;
    }
    object = port_to_object(paging_object);
    if (page_size > MAX_PAGE_SIZE) {
	syslog(LOG_ERR, "memory_object_init with out of range page size %d\n",
	    page_size);
	goto fail;
    }
    if (! is_power_of_two(page_size)) {
	syslog(LOG_ERR, "memory_object_init with invalid page size %d\n", page_size);
	goto fail;
    }
    if (port_to_kernel(object, memory_control) != -1) {
	syslog(LOG_ERR, "duplicate memory_object_init call from same kernel\n");
	goto fail;
    }
    kernel = port_to_kernel(object, PORT_NULL);
    if (kernel == -1) {
	syslog(LOG_ERR, "memory_object_init with too many kernels\n");
	goto fail;
    }
    object->kerninfo[kernel].request_port = memory_control;
    object->kerninfo[kernel].real_page_size = page_size;
    if (page_size < MIN_PAGE_SIZE) {
	object->kerninfo[kernel].page_size = MIN_PAGE_SIZE;
    } else {
	object->kerninfo[kernel].page_size = page_size;
    }
    Kernel_open(object, kernel, page_size);
    port_death(server_port, object, kernel, memory_control);
    return;

fail:
    port_deallocate(task_self(), memory_control);
    pager_destroy();
}

memory_object_data_request(paging_object, memory_control,
		   offset, length, desired_access)
port_t paging_object, memory_control;
vm_offset_t offset;
vm_size_t length;
vm_prot_t desired_access;
{
#if	DEBUG
syslog(LOG_DEBUG, "$ memory_object_data_request(0x%x, 0x%x, %d, %d, %d)\n",
	paging_object, memory_control, offset, length, desired_access);
#endif	DEBUG
    memory_object_request(paging_object, memory_control, offset, length,
		  desired_access, TRUE);
}

memory_object_data_unlock(paging_object, memory_control,
		   offset, length, desired_access)
port_t paging_object, memory_control;
vm_offset_t offset;
vm_size_t length;
vm_prot_t desired_access;
{
#if	DEBUG
syslog(LOG_DEBUG, "$ memory_object_data_unlock(0x%x, 0x%x, %d, %d, %d)\n",
	paging_object, memory_control, offset, length, desired_access);
#endif	DEBUG
    memory_object_request(paging_object, memory_control, offset, length,
		  desired_access, FALSE);
}

memory_object_request(paging_object, memory_control, offset, length,
	      desired_access, needs_data)
port_t paging_object, memory_control;
vm_offset_t offset;
vm_size_t length;
vm_prot_t desired_access;
boolean_t needs_data;
{
    object_t *object;
    int j, count, kernel;
    vm_size_t real_page_size;
    if (memory_control == PORT_NULL) {
	syslog(LOG_ERR, "null memory_control in memory_object_request\n");
    	return;
    }
    object = port_to_object(paging_object);
    kernel = port_to_kernel(object, memory_control);
    if (kernel == -1) {
	syslog(LOG_ERR, "unrecognized kernel in memory_object_request\n");
	memory_object_data_error(memory_control, offset, length,
	    KERN_MEMORY_FAILURE);
	return;
    }
    real_page_size = object->kerninfo[kernel].real_page_size;
    if (offset & (real_page_size - 1) != 0) {
	syslog(LOG_ERR, "invalid offset in memory_object_request (%d)\n",
	       offset);
	goto fail;
    }
    if (length != real_page_size) {
	syslog(LOG_ERR, "invalid length in memory_object_request (%d)\n",
	       offset);
	goto fail;
    }
    if (offset >= object->object_size) {
	goto fail;
    }
    if (real_page_size < MIN_PAGE_SIZE) {
	offset = offset & ~(MIN_PAGE_SIZE - 1);
	length = MIN_PAGE_SIZE;
    }
    j = offset >> MIN_PAGE_SHIFT;
    count = length >> MIN_PAGE_SHIFT;
    if (! needs_data) {
	object->faults_total.protection_faults += count;
	for (; count; count--, j++) {
	    object->faults_per_page[j].protection_faults++;
	}
    } else if (desired_access & VM_PROT_WRITE) {
	object->faults_total.write_faults += count;
	for (; count; count--, j++) {
	    object->faults_per_page[j].write_faults++;
	}
    } else {
	object->faults_total.read_faults += count;
	for (; count; count--, j++) {
	    object->faults_per_page[j].read_faults++;
	}
    }
    if (desired_access & VM_PROT_WRITE) {
	Add_writing_kernel(object, kernel, offset, length, needs_data);
    } else {
	Add_reading_kernel(object, kernel, offset, length, needs_data);
    }
    return;

fail:
    memory_object_data_error(memory_control, offset, length,
	KERN_MEMORY_FAILURE);
}

memory_object_data_write(paging_object, memory_control, offset, data, data_count)
port_t paging_object, memory_control;
vm_offset_t offset;
char *data;
int data_count;
{
    object_t *object;
    int kernel;
    vm_size_t real_page_size;

#if	DEBUG
syslog(LOG_DEBUG, "$ memory_object_data_write(0x%x, 0x%x, %d, 0x%x, %d)\n",
	paging_object, memory_control, offset, data, data_count);
#endif	DEBUG
    object = port_to_object(paging_object);
    if (memory_control == PORT_NULL) {
#if	DEBUG
	/* xxx this should be an error! */
	syslog(LOG_DEBUG, "null memory_control in memory_object_data_write\n");
#endif	DEBUG
    	return;
    }
    kernel = port_to_kernel(object, memory_control);
    if (kernel == -1) {
	syslog(LOG_ERR, "unrecognized kernel in memory_object_data_write\n");
	return;
    }
    real_page_size = object->kerninfo[kernel].real_page_size;
    if (data_count != real_page_size) {
	syslog(LOG_ERR, "invalid length in memory_object_data_write (%d)\n",
	    data_count);
	return;
    }
    if (offset & (real_page_size - 1) != 0) {
	syslog(LOG_ERR, "invalid offset in memory_object_data_write (%d)\n", offset);
	return;
    }
    if (offset >= object->object_size) {
	syslog(LOG_ERR, "illegal memory_object_data_write beyond size of object\n");
	return;
    }
    Subtract_kernel(object, kernel, offset, data, data_count);
    vm_deallocate(task_self(), data, data_count);
}

memory_object_lock_completed(paging_object, memory_control, offset, length)
paging_object_t paging_object;
port_t memory_control;
vm_offset_t offset;
vm_size_t length;
{
    object_t *object;
    int kernel;
    vm_size_t lock_page_size;

#if	DEBUG
syslog(LOG_DEBUG, "$ memory_object_lock_completed(0x%x, 0x%x, %d, %d)\n",
	paging_object, memory_control, offset, length);
#endif	DEBUG
    object = port_to_object(paging_object);
    if (memory_control == PORT_NULL) {
#if	DEBUG
	/* this should be an error! */
	syslog(LOG_DEBUG, "null memory_control in memory_object_lock_completed\n");
#endif	DEBUG
    	return;
    }
    kernel = port_to_kernel(object, memory_control);
    if (kernel == -1) {
	syslog(LOG_ERR, "unrecognized kernel in memory_object_lock_completed\n");
	return;
    }
    lock_page_size = object->kerninfo[kernel].page_size;
    if (length != lock_page_size) {
	syslog(LOG_ERR, "invalid length in memory_object_lock_completed (%d)\n",
	    length);
	return;
    }
    if (offset & (lock_page_size - 1) != 0) {
	syslog(LOG_ERR, "invalid offset in memory_object_lock_completed (%d)\n",
	    offset);
	return;
    }
    if (offset >= object->object_size) {
	syslog(LOG_ERR,"illegal memory_object_lock_completed beyond size of object\n");
	return;
    }
    Subtract_kernel(object, kernel, offset, 0, length);
}

/******************************************************************************
 *  Routines from internal.defs: (_timed_out is in scheduler.c)
 *	_port_death
 *	_object_thread_terminate
 ******************************************************************************
 */

_port_death(paging_object, object, kernel, port)
paging_object_t paging_object;
object_t *object;
int kernel;
port_t port;
{
    if (paging_object == server_port) {
	port_death_register(object, kernel, port);
    } else {
	Kernel_close(object, kernel);
    }
}

_object_thread_terminate(paging_object)
port_t paging_object;
{
#ifdef	lint
    paging_object++;
#endif	lint
    cthread_exit(0);
}


memory_object_copy()
{
	fprintf(stderr, "We laugh at your memory_object_copy!\n");
}

memory_object_terminate()
{
	fprintf(stderr, "We laugh at your memory_object_terminate!\n");
}

/******************************************************************************
 *  Routines from pager.defs:
 *	paging_object_statistics
 ******************************************************************************
 */

paging_object_statistics(paging_object, faults_per_page, fpp_size,
				    faults_total, ft_size)
paging_object_t paging_object;
pointer_t	*faults_per_page;
unsigned int	*fpp_size;
pointer_t	*faults_total;
unsigned int	*ft_size;
{
    object_t *object;

    object = port_to_object(paging_object);
    *faults_per_page = (pointer_t) object->faults_per_page;
    *fpp_size	     = (object->object_size >> MIN_PAGE_SHIFT) *
			sizeof(fault_info_t);
    *faults_total    = (pointer_t) &object->faults_total;
    *ft_size	     = sizeof(fault_info_t);
    return 0;
}
