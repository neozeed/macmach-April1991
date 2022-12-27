/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * 30-Mar-90  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Added forward static declaration of sleep
 *
 * $Log:	scheduler.c,v $
 * Revision 1.1  90/02/19  21:48:38  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/08/24  19:20:34  mrt
 * 	Fixed various scheduling bugs.
 * 	[89/08/24            jsb]
 * 
 */
/*
 *	File:	scheduler.c
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Multiple page size scheduler for read/write access.
 *
 */
/*
 *  We do not wait for readers to acknowledge that they have been
 *  kicked off (in other words, we do not ask for pager_lock_completed
 *  messages when sending pager_lock_requests to readers).
 *  (This is an optimization.)
 *
 *  We do not automatically turn writers into readers when we grab
 *  their min_pages on behalf of readers. We also do not merge read
 *  requests as outlined by the comment in Add_reading_kernel.
 *  I am also not careful about cleaning vs flushing.
 *  (These are things that I will do later.)
 */


#include "object.h"

#define	SET_NULL		(0)
#define SET_ADD(set, elt)	((set) |= (1 << (elt)))
#define SET_DELETE(set, elt)	((set) &= ~(1 << (elt)))
#define	SET_MEMBER(set, elt)	(((set) & (1 << (elt))) != 0)
#define	SET_FORALL(elt, set) \
	for (elt = 0; elt < (8 * sizeof(kernel_id_set_t)); elt++) {\
	    if (! SET_MEMBER(set, elt)) continue;

#if	DEBUG
#define Z1(page)	((page) - object->min_pages)
#define	Z0()		Z1(min_page)
#define	ZZ()		Z1(first_page), Z1(last_page)
#define	X1(offset)	Z1(OFFSET_TO_PAGE(offset))
#define	XX()		X1(its_offset), X1(its_offset + its_size) - 1
#endif	DEBUG

#define PAGE_TO_OFFSET(page)	(((page) - object->min_pages)\
				    << MIN_PAGE_SHIFT)
#define OFFSET_TO_PAGE(offset)	(&object->min_pages[(offset)\
				    >> MIN_PAGE_SHIFT])

#define	SUPPLY_ALL	VM_PROT_NONE
#define	SUPPLY_READ	VM_PROT_WRITE
#define	SUPPLY_NONE	VM_PROT_ALL

static sleep();

#if	DEBUG
static char *prot_to_string(prot)
{
    if (prot == SUPPLY_ALL) {
	return "WRITE";
    } else if (prot == SUPPLY_READ) {
	return "READ";
    } else if (prot == SUPPLY_NONE) {
	return "NONE";
    } else {
	return "???";
    }
}

#define	PRINT_DATA_PROVIDED(prot)\
    syslog(LOG_DEBUG, "[%d..%d] pager_data_provided(%d) %s\n",\
	   ZZ(), kernel, prot_to_string(prot))

#define	PRINT_UNLOCK_WRITER()\
    syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) WRITE\n",\
	   ZZ(), kernel)

#else	DEBUG

#define	PRINT_DATA_PROVIDED(prot)

#define	PRINT_UNLOCK_WRITER()

#endif	DEBUG
/*
 *  Offset should be rounded to MIN_PAGE_SIZE boundary.
 */
#define	DATA_PROVIDED(object, kernel, offset, prot)\
{\
    vm_size_t size = (object)->kerninfo[kernel].real_page_size;\
    PRINT_DATA_PROVIDED(prot);\
    if (size < MIN_PAGE_SIZE) {\
	vm_offset_t stop = (offset) + MIN_PAGE_SIZE;\
	for (; (offset) < stop; (offset) += size) {\
	    pager_data_provided((object)->kerninfo[kernel].request_port,\
		(offset), (object)->storage + (offset), size, (prot));\
	}\
    } else {\
	pager_data_provided((object)->kerninfo[kernel].request_port, (offset),\
	    (object)->storage + (offset), size, (prot));\
    }\
}

#define	LOCK_READER(object, kernel, offset, length)\
pager_lock_request(object->kerninfo[kernel].request_port, offset, length,\
FALSE, TRUE, SUPPLY_NONE, PORT_NULL)

#define LOCK_WRITER(object, kernel, offset, length)\
pager_lock_request(object->kerninfo[kernel].request_port, offset, length,\
TRUE, TRUE, SUPPLY_NONE, object->paging_object)

#define DEMOTE_WRITER(object, kernel, offset, length)\
pager_lock_request(object->kerninfo[kernel].request_port, offset, length,\
TRUE, FALSE, SUPPLY_READ, object->paging_object)

#define UNLOCK_WRITER(object, kernel, offset, length)\
PRINT_UNLOCK_WRITER();\
pager_lock_request(object->kerninfo[kernel].request_port, offset, length,\
FALSE, FALSE, SUPPLY_ALL, PORT_NULL)

/*
 *  May want to get rid of malloc entirely. XXX
 *  We could also macro-ize the first part...
 */
static request_t *req_alloc(object)
object_t *object;
{
    register request_t *r = object->free_requests;

    if (r) {
	object->free_requests = r->follow;
	return r;
    }
    r = (request_t *) malloc(sizeof(request_t));
    if (r == 0) {
	syslog(LOG_ERR, "Can't allocate request in req_alloc\n");
	/* core dump anyway XXX */
    }
    r->next = object->all_requests;
    object->all_requests = r;
    return r;
}

#define	req_free(r)	(r->follow = object->free_requests, \
			object->free_requests = r)

#define req_mark(r, kernel)\
{\
    short mark = object->mark;\
\
    (r)->mark = mark;\
    object->outstanding[kernel][mark]++;\
    object->inactivity = 0; /* for hogtimer, eventually... */\
}

#define req_unmark(r, kernel)\
{\
    object->outstanding[kernel][(r)->mark]--;\
}

_timed_out(paging_object, object, kernel)
port_t paging_object;
object_t *object;
int kernel;
{
    register short newmark = ! object->mark; /* also oldmark */

#ifdef	lint
    paging_object++;
#endif	lint
    for (kernel = 0; kernel < MAX_NUM_KERNELS; kernel++) {
	if (object->outstanding[kernel][newmark] > 0) {
	    Kernel_close(object, kernel);
	}
    }
    object->mark = newmark;
}

periodic_routine(object_list)
object_t **object_list;
{
    object_t *object;

    for (;;) {
	sleep(TIMEOUT);
	for (object = *object_list; object; object = object->next) {
	    timed_out(object->paging_object, object, 0);
	}
    }
}

/*
 *  A replacement for sleep(3) which works with multiple threads.
 */
static sleep(s)
int s;
{
    struct timeval timeout;

    timeout.tv_sec = s;
    timeout.tv_usec = 0;
    select(0, 0, 0, 0, &timeout);
}


/******************************************************************************
 *  The internal routines:
 *
 *	add_reader
 *	add_writer
 *	add_expecting
 *	add_sleeping
 *	satisfy_request
 *
 ******************************************************************************
 */

/*
 *  Add_reader is called from Add_reading_kernel and from satisfy_request.
 */
static add_reader(first_page, last_page, object, kernel)
min_page_t *first_page, *last_page;
object_t *object;
kernel_id_t kernel;
{
    min_page_t *min_page;
    vm_offset_t offset;
    vm_size_t size, stop;

    /*
     *  Mark all the min_pages in our page as being used,
     *  and inform the kernel that it now has the page.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	if (min_page->use == USE_NONE) {
	    min_page->use = USE_READ;
	    min_page->user.readers = SET_NULL;
	}
	SET_ADD(min_page->user.readers, kernel);
    }
    offset = PAGE_TO_OFFSET(first_page);
    DATA_PROVIDED(object, kernel, offset, SUPPLY_READ);
}

/*
 *  Add_writer is called from Add_writing_kernel and from satisfy_request.
 */
static add_writer(first_page, last_page, object, kernel, needs_data)
min_page_t *first_page, *last_page;
object_t *object;
kernel_id_t kernel;
boolean_t needs_data;
{
    min_page_t *min_page;
    kernel_id_t reader;
    vm_offset_t offset, its_offset;
    vm_size_t size, its_size;
    int j, start, stop;
    boolean_t we_were_reading;

    offset = PAGE_TO_OFFSET(first_page);
    size = object->kerninfo[kernel].page_size;
    /*
     *  First, boot all the readers whose pages properly include our page.
     */
    if (first_page->use == USE_READ) {
	SET_FORALL(reader, first_page->user.readers)
	    its_size = object->kerninfo[reader].page_size;
	    if (its_size > size) {
		its_offset = offset ^ (its_size - 1);
		start = its_offset >> MIN_PAGE_SHIFT;
		stop  = its_offset + its_size >> MIN_PAGE_SHIFT;
		for (j = start; j < stop; j++) {
		    SET_DELETE(object->min_pages[j].user.readers,
			reader);
		}
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) NONE.a\n", start, stop - 1,
reader);
#endif	DEBUG
		LOCK_READER(object, reader, its_offset, its_size);
	    }
	}
    }
    /*
     *  Now, boot all other readers.
     *  (except for ourselves, if we were reading).
     */
    we_were_reading = FALSE;
    for (min_page = first_page; min_page <= last_page; min_page++) {
	if (min_page->use == USE_READ) {
	    SET_FORALL(reader, min_page->user.readers)
		if (reader == kernel) {
		    we_were_reading = TRUE;
		    continue;
		}
		its_offset = PAGE_TO_OFFSET(min_page);
		its_size = object->kerninfo[reader].page_size;
		start = its_offset >> MIN_PAGE_SHIFT;
		stop  = its_offset + its_size >> MIN_PAGE_SHIFT;
		for (j = start; j < stop; j++) {
		    SET_DELETE(object->min_pages[j].user.readers,
			reader);
		}
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) NONE.b\n", start, stop - 1,
reader);
#endif	DEBUG
		LOCK_READER(object, reader, its_offset, its_size);
	    }
	}
    }
    /*
     *  Finally, mark all the min_pages in our page as being used,
     *  and inform the kernel that it now has the page.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	min_page->use = USE_WRITE;
	min_page->user.writer = kernel;
    }
    if (needs_data || ! we_were_reading) {
	DATA_PROVIDED(object, kernel, offset, SUPPLY_ALL);
    } else {
	UNLOCK_WRITER(object, kernel, offset, size);
    }
}

/*
 *  add_expecting
 *
 *  Preconditions:
 *	There are no waiting requests in the range.
 *	There is at least one writer that we have to kick off.
 *
 *  WHY DO WE HAVE TO ENQUEUE ON EACH MIN_PAGE?
 *  So that others (with smaller pages) always know that there is
 *  an expecting request for each page!
 *  [so that subtract_kernel can find the request, really.]
 *
 *  Note that request->count is originally set to the number
 *  of seperate writers pointing to it, which is the number of
 *  pager_lock_completed messages which will be returned.
 */
static add_expecting(request, object)
request_t *request;
object_t *object;
{
    min_page_t *first_page, *last_page, *min_page;
    vm_offset_t its_offset;
    vm_size_t its_size;
    kernel_id_t writer;

    first_page = request->first_page;
    last_page = request->last_page;
    request->count = 0;
    request->follow = 0;
    request->sibling = 0;
    /*
     *  First, enqueue our request on each min_page tail.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	min_page->tail = request;
    }
    /*
     *  Now boot all writers.
     */
    if (first_page->use == USE_WRITE
    && (object->kerninfo[first_page->user.writer].page_size
    >   object->kerninfo[request->kernel].page_size)) {
	/*
	 *  We have a bigger writer to kick off.
	 */
	writer = first_page->user.writer;
	its_size = object->kerninfo[writer].page_size;
	its_offset = PAGE_TO_OFFSET(first_page) & ~(its_size - 1);
	first_page->head = request;
	req_mark(request, writer);
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) NONE/req\n", XX(), writer);
#endif	DEBUG
	LOCK_WRITER(object, writer, its_offset, its_size);
    } else {
	/*
	 *  None of the writers are bigger than the request.
	 */
	for (min_page = first_page; min_page <= last_page; ) {
	    if (min_page->use != USE_WRITE) {
		min_page++;
		continue;
	    }
	    writer = min_page->user.writer;
	    min_page->head = request;
	    req_mark(request, writer);
	    its_offset = PAGE_TO_OFFSET(min_page);
	    its_size = object->kerninfo[writer].page_size;
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) NONE/req\n", XX(), writer);
#endif	DEBUG
	    LOCK_WRITER(object, writer, its_offset, its_size);
	    request->count++;
	    min_page += object->kerninfo[writer].page_size
			>> MIN_PAGE_SHIFT;
	}
    }
}

/*
 *  Preconditions:
 *	There is at least one waiting request in the range.
 *
 *  Two truths are used here.
 *  First, if a request r1 has one edge properly inside the edges of
 *	another request r2, then the other edge of r1 cannot be
 *	properly outside the edges of r2.
 *  Second, sibling links are not shared; if a page is on a sibling
 *	chain, then it is a follow of a page bigger than it and thus
 *	is only pointed to by one page.
 *
 */
static add_sleeping(request)
request_t *request;
{
    min_page_t *first_page, *last_page, *min_page;
    request_t *r, *oldtail;

    first_page = request->first_page;
    last_page = request->last_page;
    request->count = 0;
    request->follow = 0;
    request->sibling = 0;
    /*
     *  Find all requests in our range which are not followed, and follow
     *  them. The set of requests in our range with no followers is the
     *  smallest set which we can use to wake us up at the correct time.
     *  Return if we can find at least one request to follow.
     *  Also, become the tail for each min_page.
     *  We set oldtail here because it would be too late below.
     */
    oldtail = first_page->tail;
    for (min_page = first_page; min_page <= last_page; min_page++) {
	if (min_page->tail != 0 && min_page->tail->follow == 0) {
	    min_page->tail->follow = request;
	    request->count++;
	}
	min_page->tail = request;
    }
    if (request->count > 0) {
	return;
    }
    /*
     *  No unfollowed requests were found. Given our assumptions, this can
     *  only be caused by a request which is larger then ours which is followed
     *  only by pages outside of our range. We must then follow that larger
     *  request by becoming a sibling of the other following requests of that
     *  request.
     */
    r = oldtail->follow;
    if (r == 0 || r->first_page > request->first_page) {
	oldtail->follow = request;
	request->sibling = r;
    } else if (! r->sibling) {
	r->sibling = request;
    } else {
	while (r->sibling->first_page < request->first_page) {
	    r = r->sibling;
	}
	request->sibling = r->sibling;
	r->sibling = request;
    }
}

/*
 *  Invariants:
 *  Request has count == 0 and all writers are gone.
 *  We are ready to:
 *	update head for each affected min_page

WE MUST update head and possibly tail (if head == tail).
head = head->follow +/- sibling stuff
tail = same unless head = tail in which case head = tail = 0.

 *
 *  Called by Subtract_kernel, and recursively by satisfy_request.
 *
 *  The only ugly slow things appear here.
 *  This code doesn't wake up people that it might want to wake up.
 *  In fact, this code really needs a lot of help.
 */
static satisfy_request(request, object)
request_t *request;
object_t *object;
{
    min_page_t *first_page, *last_page, *min_page;
    boolean_t needs_data;
    use_t use;
    request_t *r;
    vm_offset_t its_offset;
    vm_size_t its_size;
    kernel_id_t writer;

if (request->kernel != -1) {
/* XXXXXXXXXXXXXXXXXXXXXXXXXX
Note the fact that we never need to iter over all requests!
we just look at evicted!
--- no, that's not true.
not unless we have a count of requests remaing with dead kernels...

OR... we could keep a list of dead kernels and only evict when
run out of ids... but who cares...

how about an array[max_num_kernels] of request pointers...
XXXXXXXXXXXXXXXX*/
    /*
     *  First, find the use, user, and min_pages specified by the
     *  request, and call add_reader or add_writer as appropriate.
     */
    first_page = request->first_page;
    last_page = request->last_page;
    if (SET_MEMBER(first_page->write_waiters, request->kernel)) {
	SET_DELETE(first_page->write_waiters, request->kernel);
	use = USE_WRITE;
    } else {
	use = USE_READ;
    }
    if (SET_MEMBER(first_page->data_waiters, request->kernel)) {
	SET_DELETE(first_page->data_waiters, request->kernel);
	needs_data = TRUE;
    } else {
	needs_data = FALSE;
    }
    if (use == USE_WRITE) {
	add_writer(first_page, last_page, object, request->kernel, needs_data);
    } else {
	add_reader(first_page, last_page, object, request->kernel);
    }
}
    /*
     *  Eliminate references to us via tails.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	if (min_page->tail == request) {
	    min_page->tail = 0;
	}
    }
    /*
     *  If we are not followed by anyone, then return.
     */
    if (request->follow == 0) {
	return;
    }
    /*
     *  There is a following request. We consider two cases, depending
     *  upon whether it is bigger than the original request.
     */
    request = request->follow;
    if (request->first_page < first_page || request->last_page > last_page) {
	/*
	 *  The following request is bigger than the original.
	 *  First, if we were not the last request pointing to the following
	 *  request, then return.
	 *  Otherwise, kick off all writers in our range.
	 *  If there are no writers, then satisfy the request immediately;
	 *  otherwise, just return.
	 *
	 *  Note here the two uses of count, first to count the number of
	 *  other requests pointing to a request, and second the number
	 *  of outstanding subtract_kernels for a request.
 	 *
	 *  This code should strongly resemble that in add_expecting.
	 */
	if (--request->count > 0) {
	    return;
	}
	for (min_page = request->first_page; min_page <= request->last_page;) {
	    if (min_page->use != USE_WRITE) {
		min_page++;
		continue;
	    }
	    writer = min_page->user.writer;
	    its_offset = PAGE_TO_OFFSET(min_page);
	    its_size = object->kerninfo[writer].page_size;
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) NONE/req\n",
Z1(min_page),
Z1(min_page - 1 +(object->kerninfo[writer].page_size>>MIN_PAGE_SHIFT)),
writer);
#endif	DEBUG
	    min_page->head = request;
	    req_mark(request, writer);
	    min_page += object->kerninfo[writer].page_size
			>> MIN_PAGE_SHIFT;
	    LOCK_WRITER(object, writer, its_offset, its_size);
	    request->count++;
	}
	if (request->count == 0) {
	    satisfy_request(request, object);
	    req_free(request);
	}
    } else {
	/*
	 *  The following request is not bigger than the original.
	 *  The original request was therefore the last request pointing
	 *  to it; furthermore, it may have siblings.
	 *  Since we are the only user for the whole range for all of
	 *  the siblings, the value of 'use' determines whether the
	 *  following request(s) can be satisfied right now.
	 */
	if (use == USE_WRITE) {
	    writer = first_page->user.writer;
	    its_offset = PAGE_TO_OFFSET(first_page);
	    its_size = object->kerninfo[writer].page_size;
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] pager_lock_request(%d) NONE/req\n", XX(), writer);
#endif	DEBUG
	    LOCK_WRITER(object, writer, its_offset, its_size);
	    for (r = request; r; r = r->sibling) {
		r->first_page->head = r;
		req_mark(r, writer);
	    }
	} else {
	    while (request) {
		satisfy_request(request, object);
		r = request;
		request = request->sibling;
		req_free(r);
	    }
	}
    }
}

/******************************************************************************
 *  The steady-state exported routines:
 *
 *	Add_reading_kernel
 *	Add_writing_kernel
 *	Subtract_kernel
 *
 ******************************************************************************
 */

Add_reading_kernel(object, kernel, offset, length, needs_data)
object_t *object;
kernel_id_t kernel;
vm_offset_t offset;
vm_size_t length;
boolean_t needs_data;
{
    min_page_t *first_page, *last_page, *min_page;
    boolean_t have_writer = FALSE, have_expecting = FALSE;
    request_t *request;

    first_page = OFFSET_TO_PAGE(offset);
    last_page = OFFSET_TO_PAGE(offset + length) - 1;
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] add_reading_kernel(%d)\n", ZZ(), kernel);
#endif	DEBUG
    /*
     *	We might already be writing, but only if we are asking for data.
     *	This means that we paged out and did a read fault in. In this case
     *	we should return data to write (but only if we haven't already been
     *  evicted).
     *
     *  We might already be waiting to read or write. I don't quite understand
     *	how this happens, but if gave a writer data when it was just asking
     *	for an unlock (see add_writer for when we do this), it sometimes
     *	emits a data request anyway. If at this point we have already evicted
     *	the kernel, we put this spurious request on the queue, but the kernel
     *	forgets about it and thus we get called.
     *
     *	Thus the only thing we really complain about is a writer that
     *	has not been evicted asking for read permission (not data).
     */
    if (first_page->use == USE_WRITE && first_page->user.writer == kernel) {
	boolean_t writer_evicted = FALSE;

	for (min_page = first_page; min_page <= last_page; min_page++) {
	    if (min_page->tail != 0) {
		writer_evicted = TRUE;
		break;
	    }
	}
	if (! writer_evicted) {
	    if (needs_data) {
		DATA_PROVIDED(object, kernel, offset, SUPPLY_ALL);
	    } else {
		syslog(LOG_ERR, "Writer asks for read permission?\n");
		UNLOCK_WRITER(object, kernel, offset, length);
	    }
	    return;
	}
    }
    if (SET_MEMBER(first_page->write_waiters, kernel)) {
	return;
    }
    if (SET_MEMBER(first_page->data_waiters, kernel)) {
	return;
    }
    if (! needs_data) {
	syslog(LOG_ERR, "Reader doesn't need data?\n");
	return;
    }
    /*
     *  If we are already reading, then satisfy the request directly.
     */
    if (first_page->use == USE_READ
    && SET_MEMBER(first_page->user.readers, kernel)) {
	DATA_PROVIDED(object, kernel, offset, SUPPLY_READ);
	return;
    }
    /*
     *  Check to see whether there are any writers in our page, and
     *  if so, whether any of them have already been booted by a waiting
     *  kernel.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	if (min_page->use == USE_WRITE) {
	    have_writer = TRUE;
	    if (min_page->tail != 0) {
		have_expecting = TRUE;
		break;
	    }
	}
    }
    /*
     *  If there are no writers, then just grab the page and return.
     */
    if (! have_writer) {
	add_reader(first_page, last_page, object, kernel);
	return;
    }
    /*
     *  There are writers, and thus we must wait.
     */
    request = req_alloc(object);
    request->kernel = kernel;
    request->first_page = first_page;
    request->last_page = last_page;
    SET_ADD(first_page->data_waiters, kernel);
    /*
     *  We first look for other requests that we can keep company with.
     *
     *  This part of the test could be made much faster by keeping two
     *  arrays of kernel_id_set_t's, indexed by kernel_id; one array
     *  is the mask of all kernels with the same page size, and the
     *  other is the mask of all kernels with a bigger page size.
     *
     *  We probably want to handle same size page_size requests as
     *  special case, but that would require using sets for readers
     *  in reader requests, which is a global sort of change.
     *  We will probably get to it eventually.
     */
#if	TRICKY
    if (there are any reader requests with page size <= mine) {
	stick us in front of that request;
	readjust counts, as appropriate;
    } else ...
#endif	TRICKY
    /*
     *  If there are no expecting requests already, then call add_expecting;
     *  otherwise, call add_sleeping.
     */
    if (have_expecting) {
	add_sleeping(request);
    } else {
	add_expecting(request, object);
    }
}

Add_writing_kernel(object, kernel, offset, length, needs_data)
object_t *object;
kernel_id_t kernel;
vm_offset_t offset;
vm_size_t length;
boolean_t needs_data;
{
    min_page_t *first_page, *last_page, *min_page;
    boolean_t have_writer = FALSE, have_expecting = FALSE;
    request_t *request;

#ifdef lint
    length++;
#endif lint

    first_page = OFFSET_TO_PAGE(offset);
    last_page = OFFSET_TO_PAGE(offset + length) - 1;
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] add_writing_kernel(%d)\n", ZZ(), kernel);
#endif	DEBUG
    /*
     *  Even if we are already writing, we may page out and need it again.
     *  (However, first make sure that we have not already been evicted.)
     */
    if (first_page->use == USE_WRITE && first_page->user.writer == kernel) {
	boolean_t writer_evicted = FALSE;

	for (min_page = first_page; min_page <= last_page; min_page++) {
	    if (min_page->tail != 0) {
		writer_evicted = TRUE;
		break;
	    }
	}
	if (! writer_evicted) {
	    if (needs_data) {
		DATA_PROVIDED(object, kernel, offset, SUPPLY_ALL);
	    } else {
		syslog(LOG_ERR, "Writer asks for write permission?\n");
		UNLOCK_WRITER(object, kernel, offset, length);
	    }
	    return;
	}
    }
    if (SET_MEMBER(first_page->write_waiters, kernel)) {
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] add_writing_kernel(%d) (again)\n", ZZ(), kernel);
#endif	DEBUG
	if (needs_data) {
	    SET_ADD(first_page->data_waiters, kernel);
	}
        return;
    }
    /*
     *  If we are already waiting to read, then simply change
     *  the request to be a write request, and return.
     */
    if (SET_MEMBER(first_page->data_waiters, kernel)) {
	SET_ADD(first_page->write_waiters, kernel);
	return;
    }
    /*
     *  Check to see whether there are any writers in our page, and
     *  if so, whether any of them have already been booted by a waiting
     *  kernel.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	if (min_page->use == USE_WRITE) {
	    have_writer = TRUE;
	    if (min_page->tail != 0) {
		have_expecting = TRUE;
		break;
	    }
	}
    }
    /*
     *  If there are no writers, then just grab the page and return.
     */
    if (! have_writer) {
	add_writer(first_page, last_page, object, kernel, needs_data);
	return;
    }
    /*
     *  There are writers, and thus we must wait. If there are no
     *  expecting requests already, then call add_expecting; otherwise,
     *  call add_sleeping.
     */
    request = req_alloc(object);
    request->kernel = kernel;
    request->first_page = first_page;
    request->last_page = last_page;
    SET_ADD(first_page->write_waiters, kernel);
    if (needs_data) {
	SET_ADD(first_page->data_waiters, kernel);
    }
    if (have_expecting) {
	add_sleeping(request);
    } else {
	add_expecting(request, object);
    }
}

Subtract_kernel(object, kernel, offset, data, length)
object_t *object;
kernel_id_t kernel;
vm_offset_t offset;
char *data;
vm_size_t length;
{
    min_page_t *first_page, *last_page, *min_page;
    request_t *request;
    vm_offset_t round_offset;
    vm_size_t round_length;

    if (length < MIN_PAGE_SIZE) {
	round_offset = offset & ~(MIN_PAGE_SIZE - 1);
	round_length = MIN_PAGE_SIZE;
    } else {
	round_offset = offset;
	round_length = length;
    }
    first_page = OFFSET_TO_PAGE(round_offset);
    last_page = OFFSET_TO_PAGE(round_offset + round_length) - 1;
#if	DEBUG
syslog(LOG_DEBUG, "[%d..%d] subtract_kernel(%d)\n", ZZ(), kernel);
#endif	DEBUG
    /*
     *  If this is a pager_lock_completed message following
     *  a pager_data_write (ie, a pending_complete), then
     *  record its arrival, and return.
     */
    if (data == 0 && SET_MEMBER(first_page->pending_completes, kernel)) {
	SET_DELETE(first_page->pending_completes, kernel);
	return;
    }
    /*
     *  Make sure that the calling kernel actually is writing the page.
     */
    if (first_page->use != USE_WRITE || first_page->user.writer != kernel) {
	syslog(LOG_ERR, "subtract_kernel from wrong writer?\n");
	return;
    }
    /*
     *  If there's data, write it in. If we know that this is the last
     *  pager_data_write before the pager_lock_completed, then
     *  we can act now and just mark down that a pager_lock_completed
     *  is still pending. Luckily, only when kernel_page_size is less
     *  than MIN_PAGE_SIZE are we unsure of whether we are the last
     *  pager_data_write... (Even this could be fixed by a count...)

Using data_writes as request completions leads to races
so I am disabling. Joe 6/20/89

     */
    if (data) {
	bcopy(data, object->storage + offset, length);
#if 0
	if (length < MIN_PAGE_SIZE) {
#else
	if (1) {
#endif
	    return;
	}
	SET_ADD(first_page->pending_completes, kernel);
    }
    /*
     *  Mark each page as no longer in use.
     */
    for (min_page = first_page; min_page <= last_page; min_page++) {
	min_page->use = USE_NONE;
    }
    if (first_page->head != 0 && (first_page->head->first_page < first_page
    || first_page->head->last_page > last_page)) {
	/*
	 *  We are followed by a bigger request.
	 *  If we are the last subtract_kernel for the request,
	 *  then satisfy the request.
	 */
	request = first_page->head;
 	first_page->head = 0;
	req_unmark(request, kernel);
	if (--request->count == 0) {
	    satisfy_request(request, object);
	    req_free(request);
	}
    } else {
	/*
	 *  We are followed only by smaller requests.
	 *  Satisfy all requests that we point to.
	 */
	for (min_page = first_page; min_page <= last_page; min_page++) {
	    if (min_page->head != 0) {
		request = min_page->head;
		min_page->head = 0;
		req_unmark(request, kernel);
		min_page = request->last_page;
		satisfy_request(request, object);
		req_free(request);
	    }
	}
    }
}

/******************************************************************************
 *  The transient exported routines:
 *
 *	Object_create
 *	Object_destroy
 *	Kernel_open
 *	Kernel_close
 *
 ******************************************************************************
 */

/*
 *  A few of the beginning tests should probably be moved to object.c.
 */
kern_return_t Object_create(object)
object_t *object;
{
    unsigned int num_min_pages;

    num_min_pages = object->object_size >> MIN_PAGE_SHIFT;
    object->min_pages = (min_page_t *)
	calloc(num_min_pages, sizeof(min_page_t));
    if (object->min_pages == 0) {
	syslog(LOG_ERR, "scheduler cannot allocate %d min_pages\n",
		num_min_pages);
	return NETMEMORY_RESOURCE_SHORTAGE;
    }
    return NETMEMORY_SUCCESS;
}

Object_destroy(object)
object_t *object;
{
    int kernel;

    for (kernel = 0; kernel < MAX_NUM_KERNELS; kernel++) {
	if (object->kerninfo[kernel].request_port) {
	    pager_destroy();
#ifdef mac2
	    port_deallocate(task_self(), object->kerninfo[kernel].request_port);
#else
	    port_deallocate(object->kerninfo[kernel].request_port);
#endif
	}
    }
    free(object->min_pages);
}

boolean_t Kernel_open(object, kernel, page_size)
object_t *object;
kernel_id_t kernel;
vm_size_t page_size;
{
#ifdef lint
    page_size++;
#endif lint
    memory_object_set_attributes(object->kerninfo[kernel].request_port,
	TRUE, MAY_CACHE, 0);
}

/*
 *  Kernel_close removes all references to the kernel in requests
 *  and min_pages, subtracting from any minpages that the kernel
 *  was writing.
 */
Kernel_close(object, kernel)
object_t *object;
kernel_id_t kernel;
{
    register request_t *r;
    min_page_t *min_page, *start_page, *stop_page;
    vm_size_t length;

#if	DEBUG
syslog(LOG_DEBUG, "kernel_close(%d)\n", kernel);
#endif	DEBUG
    /*
     *  First, zap from all requests.
     *  Satisfy_request knows how to deal with -1 kernels.
     */
    for (r = object->all_requests; r; r = r->next) {
	if (r->kernel == kernel) {
	    r->kernel = -1;
	}
    }
    /*
     *  Next, zap from all min_pages.
     */
    length = object->kerninfo[kernel].page_size;
    start_page = &object->min_pages[0];
    stop_page = start_page + (object->object_size >> MIN_PAGE_SHIFT);
    for (min_page = start_page; min_page < stop_page; min_page++) {
	if (min_page->use == USE_READ) {
	    SET_DELETE(min_page->user.readers, kernel);
	} else if (min_page->use == USE_WRITE
	&& min_page->user.writer == kernel) {
	    Subtract_kernel(object, kernel, PAGE_TO_OFFSET(min_page), 0,
		length);
	}
	SET_DELETE(min_page->write_waiters, kernel);
	SET_DELETE(min_page->data_waiters, kernel);
	SET_DELETE(min_page->pending_completes, kernel);
    }
    object->kerninfo[kernel].request_port = PORT_NULL;
    object->outstanding[kernel][0] = 0;
    object->outstanding[kernel][1] = 0;
}

