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
 *	File: mac2emul/macether_hash.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <kern/queue.h>
#include <kern/zalloc.h>

/*
 * mac ethernet protocol hashing
 * defines.
 */

typedef struct {
    queue_chain_t	hash_link;
    queue_chain_t	emul_link;
    thread_t		thread;
    unsigned short	proto;
} hash_entry_t, *hash_entry_ptr_t;

#define HASH_ENTRY_NULL ((hash_entry_ptr_t)0)

typedef struct {
    queue_head_t	head;
} hash_bucket_t;

#define HASH_SIZE	(1 << 8)

#define HASH_FUNCT(n)	((n) & (HASH_SIZE-1))

#define hash_chain(proto)	(&macether_hash_table[HASH_FUNCT(proto)].head)

#define insert_hash_entry(entry)	\
{								\
    queue_enter(hash_chain((entry)->proto),			\
		(entry),					\
		hash_entry_ptr_t,			       	\
		hash_link);					\
    queue_enter(&(entry)->thread->pcb->pcb_emul->ether_proto,	\
		(entry),					\
		hash_entry_ptr_t,				\
		emul_link);					\
}

#define remove_hash_entry(entry)	\
{								\
    queue_remove(hash_chain((entry)->proto),			\
		 (entry),					\
		 hash_entry_ptr_t,				\
		 hash_link);					\
    queue_remove(&(entry)->thread->pcb->pcb_emul->ether_proto,	\
		 (entry),					\
		 hash_entry_ptr_t,				\
		 emul_link);					\
}

#define lookup_proto_hash_entry(proto_, entry)			\
{								\
    register queue_t		q;				\
    register hash_entry_ptr_t	h;				\
\
    (entry) = HASH_ENTRY_NULL;					\
    q = hash_chain(proto_);					\
    h = (hash_entry_ptr_t)queue_first(q);			\
    while (!queue_end(q, (queue_entry_t)h)) {			\
	if (h->proto == (proto_)) {				\
	    (entry) = h;					\
	    break;						\
	}							\
	h = (hash_entry_ptr_t)queue_next(&h->hash_link);	\
    }								\
}

#define is802_3(proto)	((proto) > 0 && (proto) <= 1500)

#define inuse802_3	(count802_3 > 0)

extern int		count802_3;

extern hash_bucket_t	*macether_hash_table;
extern zone_t		macether_hash_entry_zone;
