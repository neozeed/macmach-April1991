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
 *	File: mac2emul/macserver_routines.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <kern/kern_port.h>

#include <mac2emul/macserver_hash.h>

#include "mac.h"

kern_return_t
    mac_create(port_t		server_port,
	       port_t		thread_port,
	       port_t		*request_port)
{
    register kern_return_t	result;
    register task_t		self = current_task();
    register hash_entry_ptr_t	entry;
    register thread_t		thread;
    kern_port_t			kport;
    port_name_t			port;
    extern port_set_name_t	macserver_port_set;

    if (!port_translate(self, thread_port, &kport))
	return (KERN_INVALID_ARGUMENT);

    if (port_object_type(kport) == PORT_OBJECT_THREAD) {
	thread = (thread_t) port_object_get(kport);
	thread_reference(thread);
    }
    else
	thread = THREAD_NULL;

    port_unlock(kport);

    if (thread == THREAD_NULL)
	return (KERN_INVALID_ARGUMENT);

    result = mac_kern_create(thread);
    if (result != KERN_SUCCESS) {
	thread_deallocate(thread);
	return (result);
    }

    result = mac_compat_map_init(thread);
    if (result != KERN_SUCCESS) {
	mac_kern_destroy(thread);
	thread_deallocate(thread);
	return (result);
    }

    result = port_allocate(self, &port);
    if (result != KERN_SUCCESS) {
	mac_kern_destroy(thread);
	thread_deallocate(thread);
	return (result);
    }

    result = port_set_add(self, macserver_port_set, port);
    if (result != KERN_SUCCESS) {
	port_deallocate(self, port);
	mac_kern_destroy(thread);
	thread_deallocate(thread);
	return (result);
    }

    entry = (hash_entry_ptr_t)zalloc(macserver_hash_entry_zone);
    if (entry == HASH_ENTRY_NULL) {
	port_deallocate(self, port);
	mac_kern_destroy(thread);
	thread_deallocate(thread);
	return (KERN_RESOURCE_SHORTAGE);
    }

    bzero(entry, sizeof (hash_entry_t));

    entry->thread_port = thread_port;
    entry->request_port = port;
    entry->thread = thread;
    queue_init(&entry->phys_pages);
    insert_hash_entry(entry);

    *request_port = port;
    return (KERN_SUCCESS);
}

kern_return_t
    mac_emulate(port_t		request_port,
		vm_address_t	reset_vector,
		vm_address_t	os_vector,
		mac_os_map_t	os_map,
		vm_address_t	tbox_vector,
		mac_tbox_map_t	tbox_map)
{
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (mac_kern_emulate(entry->thread,
			 reset_vector,
			 os_vector, os_map,
			 tbox_vector, tbox_map) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_phys_insert(port_t		request_port,
		    memory_object_t	memory_object,
		    vm_offset_t		offset,
		    vm_size_t		size,
		    vm_prot_t		perm,
		    vm_offset_t		physical)
{
    register kern_return_t	result;
    register hash_entry_ptr_t	entry;
    kern_obj_t			obj;
    boolean_t			initialize = FALSE;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (entry->memory_object == PORT_NULL)
	initialize = TRUE;
    else if (entry->memory_object != memory_object)
	return (KERN_INVALID_ARGUMENT);

    if (object_copyin(current_task(),
		      memory_object,
		      MSG_TYPE_PORT, FALSE,
		      (kern_obj_t *) &obj) == TRUE) {
	if (initialize == TRUE)
	    entry->memory_object = memory_object;

	result = macserver_phys_pages_insert(obj,
					     offset,
					     size,
					     perm,
					     physical,
					     &entry->phys_pages);

	obj_release(obj);
    }
    else
	result = KERN_INVALID_ARGUMENT;

    return (result);
}

kern_return_t
    mac_interrupt_vector(port_t		request_port,
			 int		which,
			 vm_offset_t	new_vector)
{
    register kern_return_t	result;
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    result = mac_kern_interrupt_vector(entry->thread, which, new_vector);

    return (result);
}

kern_return_t
    mac_interrupt_enable(port_t		request_port,
			 int		which,
			 boolean_t	enable)
{
    register kern_return_t	result;
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    result = mac_kern_interrupt_enable(entry->thread, which, enable);

    return (result);
}

kern_return_t
    mac_interrupt_post(port_t		request_port,
		       int		which,
		       boolean_t	post)
{
    register kern_return_t	result;
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    result = mac_kern_interrupt_post(entry->thread, which, post);

    return (result);
}

kern_return_t
    mac_input_event(port_t		request_port,
		    mac_input_event_t	*event)
{
    register kern_return_t	result;
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    result = mac_kern_input_event(entry->thread, event);

    return (result);
}

kern_return_t
    mac_vm_map_compat(port_t		request_port,
		      vm_offset_t	*address,
		      vm_size_t		size,
		      vm_offset_t	mask,
		      boolean_t		anywhere,
		      memory_object_t	memory_object,
		      vm_offset_t	offset,
		      boolean_t		copy,
		      vm_prot_t		cur_protection,
		      vm_prot_t		max_protection,
		      vm_inherit_t	inheritance)
{
    register kern_return_t	result;
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    (void) object_copyin(current_task(),
			 memory_object,
			 MSG_TYPE_PORT, TRUE,
			 (kern_obj_t *) &memory_object);

    result = mac_kern_map_compat(entry->thread,
				 address, size,
				 mask, anywhere,
				 memory_object, offset,
				 copy,
				 cur_protection, max_protection,
				 inheritance);

    obj_release((kern_obj_t)memory_object);

    return (result);
}
