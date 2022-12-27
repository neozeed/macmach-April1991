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
 *	File: mac2emul/macserver_hash.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <kern/queue.h>
#include <kern/zalloc.h>

typedef struct {
    queue_chain_t	thread_link;
    port_t		thread_port;
    queue_chain_t	request_link;
    port_t		request_port;
    thread_t		thread;
    queue_head_t	phys_pages;
    port_t		memory_object;
} hash_entry_t, *hash_entry_ptr_t;

#define HASH_ENTRY_NULL	((hash_entry_ptr_t)0)

typedef struct {
    queue_head_t	head;
} hash_bucket_t;

#define HASH_SIZE	(1 << 8)

#define HASH_FUNCT(n)	((n) & (HASH_SIZE-1))

#define hash_chain(port)	(&macserver_hash_table[HASH_FUNCT(port)].head)

#define insert_hash_entry(entry)	\
{							\
    queue_enter(hash_chain((entry)->thread_port),	\
		(entry),				\
		hash_entry_ptr_t,			\
		thread_link);				\
    queue_enter(hash_chain((entry)->request_port),	\
		(entry),				\
		hash_entry_ptr_t,			\
		request_link);				\
}

#define remove_hash_entry(entry)	\
{							\
    queue_remove(hash_chain((entry)->thread_port),	\
		 (entry),				\
		 hash_entry_ptr_t,			\
		 thread_link);				\
    queue_remove(hash_chain((entry)->request_port),	\
		 (entry),				\
		 hash_entry_ptr_t,			\
		 request_link);				\
}

#define lookup_port_hash_entry(type, port, entry)		\
{								\
    register queue_t		q;				\
    register hash_entry_ptr_t	h;				\
\
    (entry) = HASH_ENTRY_NULL;					\
    q = hash_chain(port);					\
    h = (hash_entry_ptr_t)queue_first(q);			\
    while (!queue_end(q, (queue_entry_t)h)) {			\
	if (h->type##_port == (port)) {				\
	    (entry) = h;					\
	    break;						\
	}							\
	h = (hash_entry_ptr_t)queue_next(&h->type##_link);	\
    }								\
}

extern hash_bucket_t	*macserver_hash_table;
extern zone_t		macserver_hash_entry_zone;
