/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	malloc.c,v $
 * Revision 1.2  90/02/23  04:54:19  bww
 * 	Corrected types in DEBUG printf.
 * 	[90/02/23  04:53:38  bww]
 * 
 * Revision 1.1  90/02/22  18:39:31  bww
 * 	Mach Release 2.5
 * 	[90/02/22  18:38:07  bww]
 * 
 * Revision 1.5  89/05/05  18:54:09  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 24-Mar-89  Michael Jones (mbj) at Carnegie-Mellon University
 *	Implement fork() for multi-threaded programs.
 *	
 *	Made MTASK version work correctly again.
 *
 *	Don't assume that more_memory always works.
 *	
 *	Coalesced more_memory_lock and page_lock which performed the
 *	same functions in different pieces of code into malloc_lock.
 *	
 *	Be sure to set input address to vm_allocate to 0 when we don't
 *	care where memory is allocated.
 *
 *	Work around the fact that vm inheritance currently breaks both the
 *	child's AND THE PARENT'S sharing relationships with other tasks when
 *	VM_INHERIT_COPY is specified.  See INHERITANCE_PRESERVES_SHARING.
 *
 * 21-Dec-88  Dean Thompson (dt) at Carnegie Mellon University
 *	Added a spin lock to more_memory() around the new memory
 *	allocation code.
 *
 *  9-Dec-88  Peter Stout (pds) at Carnegie Mellon University
 *	Modified more_memory() to allocate memory in larger chunks.
 *	This may reduce memory fragmentation.
 *	Replace ASSERTs with more informative fprintf's.
 *	Changed realloc() not to do anything, if the new and old
 *	sizes are in the same bucket.
 *	De-linted.
 * 
 * 13-Sep-88  Eric Cooper (ecc) at Carnegie Mellon University
 *	Changed realloc() to copy min(old size, new size) bytes.
 *	Bug found by Mike Kupfer at Olivetti.
 */
/*
 * malloc.c by Eric Cooper
 *
 * Memory allocator for use with multiple threads.
 */
#ifndef	lint
static char rcs_id[] = "$ Header: malloc.c,v 1.5 89/05/05 18:54:09 mrt Exp $";
#endif	not lint

#include <cthreads.h>
#include "cthread_internals.h"

#if	MTASK
#undef	task_self  /* Must call the function since the variable is shared */
#endif	MTASK

/*
 * C library imports:
 */
extern bcopy();

/*
 * Structure of memory block header.
 * When free, next points to next block on free list.
 * When allocated, fl points to free list.
 * Size of header is 4 bytes, so minimum usable block size is 8 bytes.
 */
typedef union header {
	union header *next;
	struct free_list *fl;
} *header_t;

#define MIN_SIZE	8	/* minimum block size */

typedef struct free_list {
	int lock;	/* spin lock for mutual exclusion */
	header_t head;	/* head of free list for this size */
#ifdef	DEBUG
	int in_use;	/* # mallocs - # frees */
#endif	DEBUG
} *free_list_t;

/*
 * Free list with index i contains blocks of size 2^(i+3) including header.
 * Smallest block size is 8, with 4 bytes available to user.
 * The largest block size allowed is 2^31, with 2^31 - 4 bytes available to user.
 */
#define NBUCKETS		29
#define MAX_USER_BLOCK_SIZE						    \
	((unsigned int) ((1 << (NBUCKETS + 2)) - sizeof(union header)))

private struct free_list malloc_free_list[NBUCKETS];

private int malloc_lock = 0;			/* unlocked */

#if	MTASK

#define DEFAULT_N_SHARED_PAGES	128
int n_shared_pages = 0;
private vm_address_t shared_pages = 0;
private vm_address_t next_shared_page = 0;

#else	MTASK

typedef struct pool {
	struct pool *next;
	vm_size_t size;
} *pool_t;

#define	POOL_NULL   ((pool_t) 0)
private pool_t pool = POOL_NULL;
private vm_size_t pool_size = 0;

#define DEFAULT_MALLOC_POOL_SIZE    32
unsigned int malloc_pool_size = DEFAULT_MALLOC_POOL_SIZE;

#endif	MTASK

#if	MTASK
#if	INHERITANCE_PRESERVES_SHARING
private
#endif	INHERITANCE_PRESERVES_SHARING
void
malloc_set_inheritance(shared)
	boolean_t shared;
{
	kern_return_t r;
	extern char etext[], end[];
	vm_inherit_t inheritance;

	inheritance = shared ? VM_INHERIT_SHARE : VM_INHERIT_COPY;

	/*
	 * Change existing global data inheritance.
	 */
	MACH_CALL(vm_inherit(task_self(), (vm_address_t) etext, (vm_size_t) (end - etext), inheritance), r);

	if (shared_pages != 0) {
		/*
		 * Change inheritance for pool of shared pages.
		 */
		MACH_CALL(vm_inherit(task_self(), shared_pages, n_shared_pages * vm_page_size, inheritance), r);
	}
}
#endif	MTASK

private
boolean_t
more_memory(size, fl)
	unsigned int size;
	register free_list_t fl;
{
	register vm_size_t amount;
	register unsigned int n;
	vm_address_t where;
	register header_t h;
	kern_return_t r;

#if	MTASK
	/*
	 * To simulate threads with multiple tasks,
	 * we pre-allocate a large pool of shared memory
	 * and allocate from it. This is unsatisfactory,
	 * but there is no easy way for a child to allocate
	 * memory that can be shared by its parent.
	 * Updating the free pointer (next_shared_page) is a
	 * critical section, protected by the malloc_lock semaphore.
	 */
	if (shared_pages == 0) {
		/*
		 * Allocate pool of shared pages.
		 */
		if (n_shared_pages <= 0)
			n_shared_pages = DEFAULT_N_SHARED_PAGES;
		r = vm_allocate(task_self(), &shared_pages, (vm_size_t) (n_shared_pages * vm_page_size), TRUE);
		if (r != KERN_SUCCESS) {
			mach_error("malloc.more_memory: vm_allocate failed", r);
			return FALSE;
		}

		/*
		 * Make existing global data and malloc area shared.
		 */
		malloc_set_inheritance(TRUE);
	}
#endif	MTASK

	if (size <= vm_page_size) {
		amount = vm_page_size;
		n = vm_page_size / size;
		/*
		 * We lose vm_page_size - n*size bytes here.
		 */
	} else {
		amount = (vm_size_t) size;
		n = 1;
	}

	spin_lock(&malloc_lock);
#if	MTASK
	ASSERT(next_shared_page <= n_shared_pages);
	where = shared_pages + (next_shared_page * vm_page_size);
	next_shared_page += (amount + vm_page_size - 1) / vm_page_size;
	if (next_shared_page >= n_shared_pages) {
		next_shared_page = n_shared_pages;
		spin_unlock(&malloc_lock);
		return FALSE;
	}
#else	MTASK
	if (amount <= pool_size) {
		pool_size -= amount;
		where = (vm_address_t) pool + pool_size;
		if (pool_size == 0) {
			pool = pool->next;
			if (pool != POOL_NULL) {
				pool_size = pool->size;
			}
		}
	} else {
		where = (vm_address_t) 0;
		if (pool != POOL_NULL) {
			register pool_t prev = pool;
			register pool_t cur;

			while ((cur = prev->next) != POOL_NULL) {
				if (amount <= cur->size) {
					cur->size -= amount;
					where = (vm_address_t) cur + cur->size;
					if (cur->size == 0) {
						prev->next = cur->next;
					}
					break;
				}
				prev = cur;
			}
		}
		if (where == (vm_address_t) 0) {
			if (amount >= (vm_page_size * malloc_pool_size)) {
				r = vm_allocate(task_self(), &where, amount, TRUE);
				if (r != KERN_SUCCESS) {
					mach_error("malloc.more_memory: vm_allocate failed", r);
					spin_unlock(&malloc_lock);
					return FALSE;
				}
			} else {
				pool_t new_pool = POOL_NULL;

				r = vm_allocate(task_self(), (vm_address_t *) &new_pool, (vm_page_size * malloc_pool_size), TRUE);
				if (r != KERN_SUCCESS) {
					mach_error("malloc.more_memory: vm_allocate failed", r);
					spin_unlock(&malloc_lock);
					return FALSE;
				}
				if (pool != POOL_NULL) {
					pool->size = pool_size;
				}
				new_pool->next = pool;
				pool = new_pool;
				pool_size = vm_page_size * malloc_pool_size - amount;
				where = (vm_address_t) pool + pool_size;
			}
		}
	}
#endif	MTASK
	spin_unlock(&malloc_lock);

	h = (header_t) where;
	do {
		h->next = fl->head;
		fl->head = h;
		h = (header_t) ((char *) h + size);
	} while (--n != 0);

	return TRUE;
}

char *
malloc(size)
	register unsigned int size;
{
	register unsigned int i, n;
	register free_list_t fl;
	register header_t h;

	size += sizeof(union header);
	if (size <= sizeof(union header)) {
#ifdef	DEBUG
		fprintf(stderr, "malloc: bad requested block size (%u)\n",
			size - sizeof(union header));
#endif	DEBUG
		return 0;
	}

	/*
	 * Find smallest power-of-two block size
	 * big enough to hold requested size plus header.
	 */
	i = 0;
	n = MIN_SIZE;
	while (n < size && i < NBUCKETS) {
		i += 1;
		n <<= 1;
	}
	if (i == NBUCKETS) {
#ifdef	DEBUG
		fprintf(stderr, "malloc: requested block too large (%u)\n",
			size - sizeof(union header));
#endif	DEBUG
		return 0;
	}
	

	fl = &malloc_free_list[i];
	spin_lock(&fl->lock);
	h = fl->head;
	if (h == 0) {
		/*
		 * Free list is empty; allocate more blocks.
		 */
		if (! more_memory(n, fl)) {
			spin_unlock(&fl->lock);
			return 0;
		}
		h = fl->head;
	}
	/*
	 * Pop block from free list.
	 */
	fl->head = h->next;
#ifdef	DEBUG
	fl->in_use += 1;
#endif	DEBUG
	spin_unlock(&fl->lock);
	/*
	 * Store free list pointer in block header
	 * so we can figure out where it goes
	 * at free() time.
	 */
	h->fl = fl;
	/*
	 * Return pointer past the block header.
	 */
	return ((char *) h) + sizeof(union header);
}

free(base)
	char *base;
{
	register header_t h;
	register free_list_t fl;
	register int i;

	if (base == 0) {
#ifdef	DEBUG
		fprintf(stderr, "free: freeing zero length block\n");
#endif	DEBUG
		return;
	}

	/*
	 * Find free list for block.
	 */
	h = (header_t) (base - sizeof(union header));
	/*
	 * If base is garbage this may seg fault, but such is life ...
	 */
	fl = h->fl;
	i = fl - malloc_free_list;
	/*
	 * Sanity checks.
	 */
	if (i < 0 || i >= NBUCKETS) {
#ifdef	DEBUG
		fprintf(stderr, 
			"free: object has bad free list pointer (0x%x)\n",
			fl);
#endif	DEBUG
		return;
	}
	if (fl != &malloc_free_list[i]) {
#ifdef	DEBUG
		fprintf(stderr, 
	"free: object's free list ptr != bucket free list ptr (0x%x != 0x%x)\n",
			fl, &malloc_free_list[i]);
#endif	DEBUG
		return;
	}
	/*
	 * Push block on free list.
	 */
	spin_lock(&fl->lock);
	h->next = fl->head;
	fl->head = h;
#ifdef	DEBUG
	fl->in_use -= 1;
#endif	DEBUG
	spin_unlock(&fl->lock);
	return;
}

char *
realloc(old_base, new_size)
	char *old_base;
	unsigned int new_size;
{
	register header_t h;
	register free_list_t fl;
	register int i;
	unsigned int old_size;
	char *new_base;

	if (old_base == 0)
		return malloc(new_size);
	/*
	 * Find size of old block.
	 */
	h = (header_t) (old_base - sizeof(union header));
	/*
	 * If old_base is garbage this may seg fault, but such is life ...
	 */
	fl = h->fl;
	i = fl - malloc_free_list;
	/*
	 * Sanity checks.
	 */
	if (i < 0 || i >= NBUCKETS) {
#ifdef	DEBUG
		fprintf(stderr, 
			"realloc: object has bad free list pointer (0x%x)\n",
			fl);
#endif	DEBUG
		return 0;
	}
	if (fl != &malloc_free_list[i]) {
#ifdef	DEBUG
		fprintf(stderr, 
	"realloc: object's free list ptr != bucket free list ptr (0x%x != 0x%x)\n",
			fl, &malloc_free_list[i]);
#endif	DEBUG
		return 0;
	}
	/*
	 * Free list with index i contains blocks of size 2^(i+3) including header.
	 */
	old_size = (1 << (i+3)) - sizeof(union header);

	/*
	 * If new_size is in the same bucket, then don't do anything.
	 */
	if (old_size >= new_size && (new_size + sizeof(union header)) > (1 << (i+2))) {
		return old_base;
	 }

	/*
	 * Allocate new block, copy old bytes, and free old block.
	 */
	new_base = malloc(new_size);
	if (new_base != 0)
		bcopy(old_base, new_base, (int) (old_size < new_size ? old_size : new_size));
	free(old_base);
	return new_base;
}

#ifdef	DEBUG
void
print_malloc_free_list()
{
  	register unsigned int i, size;
	register free_list_t fl;
	register unsigned int n;
  	register header_t h;
	int total_used = 0;
	int total_free = 0;

	fprintf(stderr, "      Size     In Use       Free      Total\n");
  	for (i = 0, size = MIN_SIZE, fl = malloc_free_list;
	     i < NBUCKETS;
	     i += 1, size <<= 1, fl += 1) {
		spin_lock(&fl->lock);
		if (fl->in_use != 0 || fl->head != 0) {
			total_used += fl->in_use * size;
			for (n = 0, h = fl->head; h != 0; h = h->next, n += 1)
				;
			total_free += n * size;
			fprintf(stderr, "%10u %10d %10u %10d\n",
				size, fl->in_use, n, fl->in_use + (int)n);
		}
		spin_unlock(&fl->lock);
  	}
  	fprintf(stderr, " all sizes %10d %10d %10d\n",
		total_used, total_free, total_used + total_free);
}
#endif	DEBUG

void malloc_fork_prepare()
/*
 * Prepare the malloc module for a fork by insuring that no thread is in a
 * malloc critical section.
 */
{
	register int i;

	for (i = 0; i < NBUCKETS; i++) {
		spin_lock(&malloc_free_list[i].lock);
	}

#if	MTASK
#if	INHERITANCE_PRESERVES_SHARING
	malloc_set_inheritance(FALSE);
#else	INHERITANCE_PRESERVES_SHARING
	/*
	 * Currently, the vm_inherit code is broken such that if a task does
	 * a fork() with a region marked for VM_INHERIT_COPY which it was
	 * already sharing with other tasks, both the parent and the child
	 * have the sharing relationship broken instead of just the child.
	 * We fork twice as a workaround -- once leaving sharing in place so
	 * the parent isn't disturbed, and the second time to break sharing
	 * in the child's child.  The first child just exits.
	 *			Mike Jones  --  22-Mar-1989
	 */
#endif	INHERITANCE_PRESERVES_SHARING
#endif	MTASK
}

void malloc_fork_parent()
/*
 * Called in the parent process after a fork() to resume normal operation.
 */
{
	register int i;

#if	MTASK
	malloc_set_inheritance(TRUE);
#endif	MTASK

	for (i = NBUCKETS-1; i >= 0; i--) {
		spin_unlock(&malloc_free_list[i].lock);
	}
}

void malloc_fork_child()
/*
 * Called in the child process after a fork() to resume normal operation.
 * In the MTASK case we also have to change memory inheritance so that the
 * child does not share memory with the parent.
 */
{
	register int i;

#if	MTASK
	malloc_set_inheritance(TRUE);
#endif	MTASK

	for (i = NBUCKETS-1; i >= 0; i--) {
		spin_unlock(&malloc_free_list[i].lock);
	}
}
