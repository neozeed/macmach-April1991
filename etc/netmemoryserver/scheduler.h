/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	scheduler.h,v $
 * Revision 1.1  90/02/19  21:48:43  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/08/24  17:46:35  mrt
 * 	Cleanup
 * 
 */
/*
 *	File:	scheduler.h
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Definitions for read/write access scheduler.
 *
 */
typedef short kernel_id_t;
typedef short use_t;
typedef unsigned long kernel_id_set_t;
typedef union user user_t;
typedef struct min_page min_page_t;
typedef struct request request_t;

#define USE_NONE	0
#define USE_READ	1
#define USE_WRITE	2

union user {
    kernel_id_t writer;
    kernel_id_set_t readers;
};

/*
 *  Definition of min_page_t.
 *
 *  Min_page_t is the data structure associated with each minimal page.
 *  It contains information about what the page is currently
 *  being used for, as well as who is waiting for it.
 *
 *  This structure must either make sense when zero-filled,
 *  or code should be added to object_create to explicitly
 *  initialize each min_page in the object.
 *
 *  Use and user are currently set for each min_page in a
 *	range that is being used; e.g., if a writer uses
 *	a page which consists of four min_pages, each min_page
 *	has use and user set to USE_WRITE and the writer's
 *	kernel_id_t value, respectively.
 *  Use is one of USE_NONE, USE_READ, or USE_WRITE.
 *  User is a union. If use is USE_READ or USE_WRITE, look
 *	at user.readers or user.writer, respectively;
 *	if use is USE_NONE, user has no meaning.
XXXXXXXX
This is no longer correct!
Note the top note comment above labeled "SOMETHING I forgot...".
 *  Head and Tail:
 *	If use is not USE_WRITE, then neither head nor tail should be
 *	examined. Otherwise, if the writer has not been booted, then
 *	head and tail are null. Otherwise,
 *	If there is a writer who has been booted, then head
 *	points to the request which has booted the writer,
 *	and tail points to the last request which is waiting
 *	for the given min_page.
 *	Note that head and tail must be set for all min_pages of
 *	a booted writer so that any subset of those pages can
 *	be examined to determine whether that subset has been
 *	booted.
XXXXXXXX
 *  Write_waiters and data_waiters are used to detect duplicate
 *	requests (or ampl.) from a given kernel. They are used together
 *	to record that a kernel already has one of the following
 *	(mutually exclusive) requests waiting for a given page:
 *
 *	    none			: w_w false, d_w false
 *	    read, needs data		: w_w false, d_w true
 *	    write, doesn't need data	: w_w true,  d_w false
 *	    write, needs data		: w_w true,  d_w true
 *
 *	Note that all read requests will be sent data. This is because
 *	we never lock a reader out without forcing it to invalidate
 *	its data.
 *
 *	Write_waiters and data_waiters are set only in the first
 *	min_page of a request.
 */
struct min_page {
    use_t use;
    user_t user;
    request_t *head;
    request_t *tail;
    kernel_id_set_t write_waiters;	/* kernels waiting for write access */
    kernel_id_set_t data_waiters;	/* kernels waiting for data */
    kernel_id_set_t pending_completes;	/* pending lock_completes */
};

/*
 *  XXX explain this structure.
 *
 *  Requests are only added by add_sleeping or add_expecting and are only
 *  removed by satisfy_request.
 */
struct request {
    kernel_id_t kernel;		/* set before calling add_{sleep,expect}ing */
    min_page_t *first_page;	/* ditto				*/
    min_page_t *last_page;	/* ditto				*/
    request_t *follow;		/* request which follows me, 0 if none	*/
    request_t *sibling;		/* i and others follow a bigger page	*/
    request_t *next;		/* for req_kill				*/
    short count;		/* num requests or boots pointing to me */
    short mark;			/* for aging requests for kernel death	*/
};

char *malloc();
