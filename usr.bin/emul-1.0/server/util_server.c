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
 *	File: emul/server/util_server.c
 *	Author: Chris Newman
 */

/* util_server.c -- utility server for mac emulator unix clients
 */

#include <mach.h>
#include <mach/message.h>
#include <mach/mig_errors.h>

/* reply message for environment request
 */
static struct outline {
    msg_header_t	h;
    msg_type_t		t;
    char		*data;
} environ_reply = {
    {
	0,			/* unused */
	0,			/* message is not simple */
	sizeof (struct outline),
	MSG_TYPE_NORMAL,
	PORT_NULL,		/* no reply needed */
	0,			/* remote port - to fill in */
	0,			/* msg_id */
    },
    {
	MSG_TYPE_CHAR,
	8,			/* 8 bits per char */
	0,			/* amount of data - to fill in */
	0,			/* data is not inline */
	0,			/* not long form */
	0,			/* deallocate */
	0,			/* unused */
    },
    0				/* data ptr -- to fill in */
};

extern char **environ;

void util_server(in, out)
    register msg_header_t *in, *out;
{
    kern_return_t	error;
    
    switch (in->msg_id) {
      case 0: /* environment */
	if (environ_reply.data == 0
	    && environ_reply.h.msg_size == sizeof (struct outline)) {
	    int size = 1;
	    char **scan, *buf;
	    
	    for (scan = environ; *scan; scan++) {
		size += strlen(*scan) + 1;
	    }
	    if ((error = vm_allocate(task_self(), (vm_address_t *) &buf,
				     size, TRUE)) != KERN_SUCCESS) {
		environ_reply.h.msg_size = sizeof (msg_header_t);
	    } else {
		register char *src, *dst;
		
		environ_reply.h.msg_id = scan - environ;
		environ_reply.t.msg_type_number = size;
		environ_reply.data = dst = buf;
		for (scan = environ; *scan; scan++) {
		    for (src = *scan; *src; *dst++ = *src++);
		    *dst++ = '\n';
		}
		*dst = '\0';
	    }
	}
	environ_reply.h.msg_remote_port = in->msg_remote_port;
	(void) msg_send(&environ_reply.h, MSG_OPTION_NONE, 0);
	((death_pill_t *) out)->RetCode = MIG_NO_REPLY;
	return;
	
      case 1: /* console ungrab */
	console_restore();
	break;
	
      case 2: /* console grab */
	console_redirect();
	break;
    }
    out->msg_remote_port = in->msg_remote_port;
    out->msg_local_port = PORT_NULL;
    out->msg_id = 1;
    out->msg_size = sizeof (msg_header_t);
    out->msg_type = MSG_TYPE_NORMAL;
    out->msg_simple = TRUE;
    ((death_pill_t *) out)->RetCode = KERN_SUCCESS;
}
