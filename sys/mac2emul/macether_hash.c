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
 *	File: mac2emul/macether_hash.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macether_hash.h>

hash_bucket_t	*macether_hash_table;
zone_t		macether_hash_entry_zone;

int		count802_3;

/*
 * mac ethernet protocol hashing
 * routines.
 */

boolean_t
mac_ether_proto_hash_add(proto, thread)
unsigned short	proto;
thread_t	thread;
{
    register hash_entry_ptr_t	entry;

    if (macether_hash_table == (hash_bucket_t *)0) {
	register i;

	macether_hash_table =
	    (hash_bucket_t *)kalloc(HASH_SIZE * sizeof (hash_bucket_t));

	if (macether_hash_table == (hash_bucket_t *)0)
	    return (FALSE);

	for (i = 0; i < HASH_SIZE; i++)
	    queue_init(hash_chain(i));

	macether_hash_entry_zone = zinit(sizeof (hash_entry_t),
					 256*sizeof (hash_entry_t),
					 0, FALSE, "macether hash entry zone");

	if (macether_hash_entry_zone == ZONE_NULL)
	    return (FALSE);
    }

    if (proto == 0 || is802_3(proto)) {
	lookup_proto_hash_entry(0, entry);
	if (entry != HASH_ENTRY_NULL)
	    return (FALSE);

	if (proto == 0 && inuse802_3)
	    return (FALSE);
    }

    lookup_proto_hash_entry(proto, entry);
    if (entry != HASH_ENTRY_NULL)
	return (FALSE);

    entry = (hash_entry_ptr_t)zalloc(macether_hash_entry_zone);
    if (entry == HASH_ENTRY_NULL)
	return (FALSE);

    entry->proto = proto;
    entry->thread = thread;
    {
	register s = splimp();

	insert_hash_entry(entry);
	splx(s);
    }

    if (is802_3(proto))
	count802_3++;

    return (TRUE);
}

boolean_t
mac_ether_proto_hash_delete(proto, thread)
unsigned short	proto;
thread_t	thread;
{
    register hash_entry_ptr_t	entry;

    if (macether_hash_table == (hash_bucket_t *)0)
	return (FALSE);

    lookup_proto_hash_entry(proto, entry);

    if (entry == HASH_ENTRY_NULL)
	return (FALSE);

    if (entry->thread != thread)
	return (FALSE);

    {
	register s = splimp();

	remove_hash_entry(entry);
	splx(s);
    }

    if (is802_3(proto))
	count802_3--;

    zfree(macether_hash_entry_zone, entry);
}

thread_t
mac_ether_proto_hash_lookup(proto)
unsigned short proto;
{
    register hash_entry_ptr_t	entry;

    if (macether_hash_table == (hash_bucket_t *)0)
	return (THREAD_NULL);

    lookup_proto_hash_entry(proto, entry);

    if (entry == HASH_ENTRY_NULL)
	return (THREAD_NULL);

    return (entry->thread);
}

mac_ether_proto_hash_terminate(thread)
thread_t	thread;
{
    register queue_t		q = &thread->pcb->pcb_emul->ether_proto;
    register hash_entry_ptr_t	entry;
    register s;

    entry = (hash_entry_ptr_t)queue_first(q);
    while (!queue_end(q, (queue_entry_t)entry)) {
	{
	    register s = splimp();

	    remove_hash_entry(entry);
	    splx(s);
	}

	if (is802_3(entry->proto))
	    count802_3--;

	zfree(macether_hash_entry_zone, entry);

	entry = (hash_entry_ptr_t)queue_first(q);
    }
}
