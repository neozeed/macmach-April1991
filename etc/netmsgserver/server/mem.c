/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mem.c,v $
 * Revision 1.1  90/02/19  21:53:01  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.31  89/05/02  11:12:33  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.30  89/04/24  20:40:46  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:55:11  dpj]
 * 
 * 09-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Use malloc/free to do memory allocations when a good malloc
 *	is available.
 *
 * 22-Aug-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added mem_list().
 *
 * 17-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 */
/*
 * mem.c
 *
 *
 */

#ifndef	lint
char mem_rcsid[] = "$ Header: mem.c,v 1.31 89/05/02 11:12:33 dpj Exp $";
#endif not lint

/*
 * Memory management module.
 */


#include	"netmsg.h"
#include	"nm_defs.h"

#include "mem.h"

#include <mach/machine/vm_param.h>
#include <cthreads.h>

/*
 * Chain of all the different objects managed by this module.
 */
PRIVATE struct mutex		mem_lock;
EXPORT mem_objrec_ptr_t		mem_chain;


/*
 * mem_init --
 *	Initialise the memory module.
 *
 * Results:
 *	TRUE or FALSE.
 *
 * Design:
 *	Initialise the mem_chain list.
 *
 */
EXPORT boolean_t mem_init()
BEGIN("mem_init")
	mutex_init(&mem_lock);
	mem_chain = NULL;

	RETURN(TRUE);

END


/*
 * mem_clean --
 *	Clean up the memory allocated to reduce paging activity.
 *
 * Results:
 *	KERN_SUCCESS or KERN_FAILURE
 *
 * Side effects:
 *	May invalidate memory.
 *
 * Design:
 *
 * Note:
 *	Does not do anything yet!
 *
 */
EXPORT mem_clean()
BEGIN("mem_clean")

	ERROR((msg, "mem_clean called unexpectedly."));
	RETURN(KERN_SUCCESS);

END



/*
 * mem_initobj --
 *
 * Parameters:
 *	or: pointer to the object record to initialize.
 *	name: name fotr this type of object.
 *	obj_size: size in bytes of one object.
 *	aligned: TRUE if objects must be aligned on a page boundary.
 *	full_num: number of objects in a full bucket.
 *	reuse_num: minimum number of free objects before reusing a bucket.
 *
 * Results:
 *	none.
 *
 * Side effects:
 *	Fill in the object record. Allocate the first bucket.
 *
 */
EXPORT void mem_initobj(or,name,obj_size,aligned,full_num,reuse_num)
	mem_objrec_ptr_t	or;
	char			*name;
	unsigned int		obj_size;
	boolean_t		aligned;
	int			full_num;
	int			reuse_num;
BEGIN("mem_initobj")
	int			shift;
	unsigned int		bucket_size;

#if	GOOD_MALLOC
	or->name = name;
	if (aligned) {
		or->obj_size = round_page(obj_size);
	} else {
		or->obj_size = obj_size;
	}
#else	GOOD_MALLOC
	/*
	 * Sanity checks.
	 */
	if (full_num < 2) {
		ERROR((msg,
			"mem_initobj(%s): full_num too small, resetting it to 2",
			name));
		full_num = 2;
	}
	if (reuse_num >= full_num) {
		ERROR((msg,
	"mem_initobj(%s): reuse_num too large, resetting it to full_num - 1",
			name));
		reuse_num = full_num - 1;
	}

	/*
	 * Initialize the record.
	 */
	mutex_init(&or->lock);
	or->name = name;
	or->aligned = aligned;
	or->reuse_num = reuse_num;
	or->full_num = full_num;
	or->cur_buckets = 0;
	or->max_buckets = 0;
	or->first = NULL;
	or->last = NULL;
	or->list_first = NULL;
	or->list_last = NULL;

	/*
	 * Make sure that the objects are big enough and well aligned
	 * to hold a chaining pointer.
	 */
	if (obj_size >= sizeof(pointer_t)) {
		or->obj_size = (obj_size + sizeof(pointer_t) - 1) &
							~(sizeof(pointer_t) - 1);
	} else {
		or->obj_size = sizeof(pointer_t);
	}

	/*
	 * Each bucket must be big enough to hold all the objects with
	 * the correct alignment, plus the bucket header.
	 *
	 * Since we use vm_allocate(), buckets occupy entire VM pages.
	 */
	if (aligned) {
		or->bucket_size = (full_num * round_page(or->obj_size)) +
							vm_page_size;
	} else {
		or->bucket_size = round_page(
			(full_num * or->obj_size) + sizeof(mem_objbucket_t));
	}

	/*
	 * Fix the bucket size to be a power of two, so that we can
	 * always the bucket that an object belongs to by simple masking.
	 */
	shift = 0;
	bucket_size = or->bucket_size;
	while (bucket_size != 0) {
		bucket_size >>= 1;
		shift++;
	}
	if (or->bucket_size != ( 1 << (shift - 1))) {
		or->bucket_size = 1 << shift;
	}
	or->mask = ~(or->bucket_size - 1);

	/*
	 * Place the record on the chain of objects.
	 */
	mutex_lock(&mem_lock);
	or->mem_chain = mem_chain;
	mem_chain = or;
	mutex_unlock(&mem_lock);

	DEBUG4(MEM_DEBUG_LO,0,1105,or,or->obj_size,or->bucket_size,or->mask);
	DEBUG_STRING(MEM_DEBUG_LO,0,1106,or->name);

#endif	GOOD_MALLOC
	RET;
END


/*
 * mem_allocobj_proc --
 *
 * Parameters:
 *	or: pointer to the object record.
 *
 * Results:
 *	Pointer to a new object.
 *
 * Side effects:
 *	Allocates memory for a new bucket.
 *	May call panic() if the allocation fails.
 *
 * Note:
 *	The object record must be locked throughout this procedure.
 *
 */
PUBLIC pointer_t mem_allocobj_proc(or)
	mem_objrec_ptr_t	or;
BEGIN("mem_allocobj_proc")
#if	GOOD_MALLOC
	RETURN((pointer_t) malloc(or->obj_size));
#else	GOOD_MALLOC
	kern_return_t		kr;
	vm_offset_t		data_ptr = 0;
	unsigned int		data_size;
	vm_offset_t		bucket_ptr;
	vm_offset_t		end_ptr;
	mem_objbucket_ptr_t	ob;
	unsigned int		step;
	vm_offset_t		cur_ptr;
	vm_offset_t		next_ptr;
	int			count;

	/*
	 * Allocate space for the bucket.
	 */
	if (or->bucket_size == vm_page_size) {
		/*
		 * For single-page buckets, there is no alignment problem.
		 */
		kr = vm_allocate(task_self(),&data_ptr,vm_page_size, TRUE);
		if (kr != KERN_SUCCESS) {
			ERROR((msg,
				"mem_allocobj_proc.vm_allocate failed, kr=%d",
				kr));
			panic("mem_allocobj_proc.vm_allocate");
		}
		bucket_ptr = data_ptr;
	} else {
		/*
		 * Get enough space to be able to choose the alignment,
		 * then discard what we dont use before and after the bucket.
		 */
		data_size = (or->bucket_size * 2) - vm_page_size;
		kr = vm_allocate(task_self(),&data_ptr,data_size, TRUE);
		if (kr != KERN_SUCCESS) {
			ERROR((msg,
				"mem_allocobj_proc.vm_allocate failed, kr=%d",
				kr));
			panic("mem_allocobj_proc.vm_allocate");
		}
		bucket_ptr= (data_ptr + or->bucket_size - 1) & or->mask;
		if (bucket_ptr != data_ptr) {
			kr = vm_deallocate(task_self(),data_ptr,
					bucket_ptr - data_ptr);
			if (kr != KERN_SUCCESS) {
				ERROR((msg,
			"mem_allocobj_proc.vm_deallocate failed, kr=%d",
					kr));
				panic("mem_allocobj_proc.vm_deallocate");
			}
		}
		end_ptr = bucket_ptr + or->bucket_size;
		if (end_ptr != (data_ptr + data_size)) {
			kr = vm_deallocate(task_self(),end_ptr,
					data_ptr + data_size - end_ptr);
			if (kr != KERN_SUCCESS) {
				ERROR((msg,
			"mem_allocobj_proc.vm_deallocate failed, kr=%d",
					kr));
				panic("mem_allocobj_proc.vm_deallocate");
			}
		}
	}

	/*
	 * Initialize the bucket.
	 */
	ob = (mem_objbucket_ptr_t) bucket_ptr;
	ob->num = or->full_num - 1;
	ob->info = (pointer_t) or;
	if (or->aligned) {
		cur_ptr = bucket_ptr + vm_page_size;
		step = round_page(or->obj_size);
	} else {
		cur_ptr = bucket_ptr + sizeof(mem_objbucket_t);
		step = or->obj_size;
	}
	ob->freehead = (pointer_t) cur_ptr;
	for (count = 2; count < or->full_num; count++) {
		*(pointer_t *) cur_ptr = next_ptr = cur_ptr + step;
		cur_ptr = next_ptr;
	}
	*(pointer_t *) cur_ptr = NULL;
	next_ptr = cur_ptr + step;

	/*
	 * Place the bucket at the head of the queue.
	 *
	 * Normally, there are no other buckets on the queue,
	 * but let us be careful.
	 */
	ob->avail = TRUE;
	ob->next = or->first;
	ob->prev = NULL;
	if (ob->next) {
		ob->next->prev = ob;
	} else {
		or->last = ob;
	}
	or->first = ob;
	or->cur_buckets++;
	if (or->cur_buckets > or->max_buckets) {
		or->max_buckets = or->cur_buckets;
	}

	ob->list_next = or->list_first;
	ob->list_prev = NULL;
	if (ob->list_next) {
		ob->list_next->list_prev = ob;
	} else {
		or->list_last = ob;
	}
	or->list_first = ob;

	DEBUG6(MEM_DEBUG_HI,3,1107,ob,or->cur_buckets,or->first,or->last,
						ob->freehead,next_ptr);

	RETURN((pointer_t) next_ptr);
#endif	GOOD_MALLOC
END



/*
 * mem_deallocobj_proc --
 *
 * Parameters:
 *	or: pointer to the object record.
 *	ob: pointer to a bucket that may need special action.
 *
 * Side effects:
 *	May place the bucket on the allocation queue, or may
 *	deallocate it.
 *
 * Note:
 *	The object record must be locked throughout this procedure.
 *
 */
PUBLIC void mem_deallocobj_proc(or,ob)
	mem_objrec_ptr_t	or;
#if	GOOD_MALLOC
#else	GOOD_MALLOC
	mem_objbucket_ptr_t	ob;
#endif	GOOD_MALLOC
BEGIN("mem_deallocobj_proc")
#if	GOOD_MALLOC
	free(ob);
#else	GOOD_MALLOC
	kern_return_t	kr;

	/*
	 * If the bucket is totally free, deallocate it unless
	 * it is the first one on the queue.
	 * We can be sure that a free bucket is on the allocation
	 * queue, because reuse_num < full_num.
	 */
	if ((ob->num == or->full_num) && (ob != or->first)) {
		if (ob->prev) {
			/*
			 * Always true because we never free the head 
			 * of the queue
			 */
			ob->prev->next = ob->next;
		}
		if (ob->next) {
			ob->next->prev = ob->prev;
		} else {
			or->last = ob->prev;
		}
		if (ob->list_prev) {
			ob->list_prev->list_next = ob->list_next;
		} else {
			or->list_first = ob->list_next;
		}
		if (ob->list_next) {
			ob->list_next->list_prev = ob->list_prev;
		} else {
			or->list_last = ob->list_prev;
		}
		kr = vm_deallocate(task_self(),(vm_offset_t)ob,or->bucket_size);
		if (kr != KERN_SUCCESS) {
			ERROR((msg,
				"mem_deallocobj_proc.vm_deallocate failed, kr=%d",
				kr));
			panic("mem_deallocobj_proc.vm_deallocate");
		}
		or->cur_buckets--;
		DEBUG4(MEM_DEBUG_HI,3,1108,ob,or->cur_buckets,or->first,or->last);
		RET;
	}

	/*
	 * If the bucket has reached the reuse threshold, see about
	 * putting it back on the queue.
	 *
	 * We will put it at the tail of the queue, in the hope that
	 * after a few more deallocations, it will become totally free.
	 */
	if ((ob->num == or->reuse_num) && (ob->avail == FALSE)) {
		ob->avail = TRUE;
		ob->next = NULL;
		ob->prev = or->last;
		if (or->last) {
			or->last->next = ob;
		} else {
			or->first = ob;
		}
		or->last = ob;
		DEBUG6(MEM_DEBUG_HI,3,1109,ob,ob->num,or->cur_buckets,or->first,
							or->last,ob->freehead);
		RET;
	}

	ERROR((msg,"mem_deallocobj_proc(%s) could not find anything to do",
								or->name));
#endif	GOOD_MALLOC
	RET;
END




/*
 * mem_list --
 *
 *   List all buckets for all classes into the specified buffers.
 *
 * Parameters:
 *
 * Side effects:
 *
 * Note:
 *
 */
PUBLIC void mem_list(class_ptr,nam_ptr,bucket_ptr,class_max,nam_max,bucket_max)
	mem_objrec_ptr_t	class_ptr;
	char			*nam_ptr;
	int			*bucket_ptr;
	int			*class_max;		/* inout */
	int			*nam_max;		/* inout */
	int			*bucket_max;		/* inout */
BEGIN("mem_list")
#if	GOOD_MALLOC
	/* think about returning malloc statistics */
#else	GOOD_MALLOC
	int			class_size = 0;
	int			nam_size = 0;
	int			bucket_size = 0;
	mem_objrec_ptr_t	cur_class;
	mem_objbucket_ptr_t	cur_bucket;
	int			real_bucket_num;
	

	mutex_lock(&mem_lock);

	cur_class = mem_chain;
	while (cur_class != NULL) {
		class_size += sizeof(mem_objrec_t);
		if (class_size > *class_max) {
			mutex_unlock(&mem_lock);
			*class_max =0;
			*nam_max = 0;
			bucket_max = 0;
			RET;
		}

		mutex_lock(&cur_class->lock);
		bcopy(cur_class,class_ptr,sizeof(mem_objrec_t));
		class_ptr++;

		nam_size += 80;
		if (nam_size > *nam_max) {
			mutex_unlock(&cur_class->lock);
			mutex_unlock(&mem_lock);
			*class_max =0;
			*nam_max = 0;
			bucket_max = 0;
			RET;
		}

		strncpy(nam_ptr,cur_class->name,79);
		nam_ptr[79] = '\0';
		nam_ptr += 80;

		real_bucket_num = 0;
		cur_bucket = cur_class->list_first;
		while (cur_bucket != NULL) {
			bucket_size += sizeof(int);
			if (bucket_size > *bucket_max) {
				mutex_unlock(&cur_class->lock);
				mutex_unlock(&mem_lock);
				*class_max =0;
				*nam_max = 0;
				bucket_max = 0;
				RET;
			}

			*bucket_ptr = cur_bucket->num;
			bucket_ptr++;
			real_bucket_num++;

			cur_bucket = cur_bucket->list_next;
		}

		if (real_bucket_num > cur_class->cur_buckets) {
			ERROR((msg,"mem_list: too many buckets for %s",cur_class->name));
		}

		mutex_unlock(&cur_class->lock);

		cur_class = cur_class->mem_chain;
	}
	
	mutex_unlock(&mem_lock);

	*class_max = class_size;
	*nam_max = nam_size;
	*bucket_max = bucket_size;

#endif	GOOD_MALLOC
	RET;
END
