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
 *	File: mac2emul/macether_routines.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <mac2emul/macserver_hash.h>

#include "mac.h"

/*
 * The server side of the mac ether.
 */

kern_return_t
    mac_ether_send(port_t		request_port,
		   ether_packet_t	packet,
		   unsigned int		length)
{
    if (mac_ether_q_send(packet, length) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_ether_recv(port_t		request_port,
		   ether_packet_t	packet,
		   unsigned int		*length)
{
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (mac_ether_dq_receive(packet, length, entry->thread) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_ether_add_protocol(port_t	request_port,
			   short	protocol)
{
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (mac_ether_proto_hash_add(protocol, entry->thread) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_ether_delete_protocol(port_t	request_port,
			      short	protocol)
{
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (mac_ether_proto_hash_delete(protocol, entry->thread) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_ether_address(port_t		request_port,
		      ether_address_t	address)
{
    if (mac_ether_interface_address(address) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_ether_add_multicast(port_t		request_port,
			    ether_address_t	address)
{
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (mac_ether_interface_add_multi(address, entry->thread) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}

kern_return_t
    mac_ether_delete_multicast(port_t		request_port,
			       ether_address_t	address)
{
    register hash_entry_ptr_t	entry;

    lookup_port_hash_entry(request, request_port, entry);
    if (entry == HASH_ENTRY_NULL)
	return (KERN_FAILURE);

    if (mac_ether_interface_delete_multi(address, entry->thread) == FALSE)
	return (KERN_FAILURE);

    return (KERN_SUCCESS);
}
