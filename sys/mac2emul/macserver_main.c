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
 *	File: mac2emul/macserver_main.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <kern/ipc_pobj.h>
#include <kern/kern_port.h>

#include <mac2emul/macserver_hash.h>

port_set_name_t		macserver_port_set;

zone_t			macserver_hash_entry_zone;

hash_bucket_t		*macserver_hash_table;

#define	kern_failure(s, r)	\
{						\
    printf("%s returns %d\n", (s), (r));	\
    panic(SERVER_NAME);				\
}

#define	SERVER_NAME		"mac kern server"
#define SERVER_LOOP		macserver_loop

void	SERVER_LOOP();

void
macserver_init(server, init)
register task_t		server, init;
{
    register kern_return_t	result;
    register			i;
    port_t			port;
    kern_port_t			kport;

    macserver_hash_entry_zone = zinit(sizeof (hash_entry_t),
				      256*sizeof (hash_entry_t),
				      0, FALSE, "macserver hash entry zone");

    if (macserver_hash_entry_zone == ZONE_NULL)
	panic("zinit macserver hash entry zone");

    macserver_hash_table =
	(hash_bucket_t *)kalloc(HASH_SIZE * sizeof (hash_bucket_t));

    if (macserver_hash_table == 0)
	panic("kalloc macserver hash table");

    for (i = 0; i < HASH_SIZE; i++)
	queue_init(hash_chain(i));

    macserver_phys_pages_initialize();

    result = port_set_allocate(server, &macserver_port_set);
    if (result != KERN_SUCCESS)
	kern_failure("port_set_allocate", result);

    result = port_allocate(server, &port);
    if (result != KERN_SUCCESS)
	kern_failure("port_allocate server_port", result);

    result = port_set_add(server, macserver_port_set, port);
    if (result != KERN_SUCCESS)
	kern_failure("port_set_add server_port", result);

/* XXX */
    (void) object_copyin(server, port,
			 MSG_TYPE_PORT, FALSE,
			 (kern_obj_t *) &kport);
    init->ipc_ports_registered[TASK_PORT_REGISTER_MAX - 1] = (port_t)kport;
/* XXX */
}

void
macserver()
{
    task_name(SERVER_NAME);

    (void) SERVER_LOOP(macserver_port_set, TRUE);
}

macserver_terminate(port)
register port_t		port;
{
    register hash_entry_ptr_t	entry;
    kern_obj_t			obj;

    lookup_port_hash_entry(thread, port, entry);

    if (entry != HASH_ENTRY_NULL) {
	remove_hash_entry(entry);
	if (entry->memory_object != PORT_NULL) {
	    if (object_copyin(current_task(),
			      entry->memory_object,
			      MSG_TYPE_PORT, TRUE,
			      &obj) == TRUE) {
		macserver_phys_pages_terminate(obj,
					       &entry->phys_pages);
		obj_release(obj);
	    }
	    else
		macserver_phys_pages_terminate(KERN_OBJ_NULL,
					       &entry->phys_pages);
		
	}
	mac_kern_destroy(entry->thread);
	thread_deallocate(entry->thread);
	port_deallocate(current_task(), entry->request_port);
	zfree(macserver_hash_entry_zone, entry);
    }
}

/* DO NOT ADD ANY CODE BELOW HERE, UNLESS YOU KNOW WHAT YOU ARE DOING */

#include <mach/mach_user_internal.h>

#define TERMINATE_FUNCTION	macserver_terminate
#define SERVER_DISPATCH		mac_server

#include <kern/server_loop.c>
