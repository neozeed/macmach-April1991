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
 * $Log:	cam_stuff.c,v $
 * Revision 1.1  90/02/19  21:49:22  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.8  89/05/02  11:06:45  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 25-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Removed an unused kr variable to avoid a compiler warning.
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	De-linted.
 *
 *  5-Aug-87  Daniel Julin (dpj) and Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * cam_stuff.c
 *
 *
 */

#ifndef	lint
char cam_stuff_rcsid[] = "$ Header: cam_stuff.c,v 1.8 89/05/02 11:06:45 dpj Exp $";
#endif not lint

/*
 * Main routines for the camelot support.
 */


#include "config.h"
#if	CAMELOT

#include <cthreads.h>
#include <mach.h>
#include <mach/boolean.h>
#include <mach/message.h>
#include <sys/types.h>
#include <mig_errors.h>

#include "debug.h"
#include "mem.h"
#include "netmsg.h"
#include "nm_extra.h"

#include "../camelot/cam.h"
#include "../camelot/global.h"


#define CAM_STUFF_MAX_MSG_SIZE	512
#define CAM_STUFF_NUM_THREADS	1

extern boolean_t ct_server();

/*
 * Global lock to prevent camelot routines from stepping on each other.
 */
struct mutex	camelot_lock;



/*
 * cam_stuff_main
 *	Main loop for ct service.
 *
 * Results:
 *	Should never return.
 *
 * Design:
 *	Wait for a message on the ct port.
 *	Call ct_server to process it.
 *	Send the reply message.
 *
 * Note:
 *	There may be multiple threads executing this main loop.
 *
 */
PRIVATE cam_stuff_main()
BEGIN("cam_stuff_main")
    msg_header_t	*req_msg_ptr, *rep_msg_ptr;
    kern_return_t	kr;
    boolean_t		send_reply;

extern char			my_host_name[40];
extern netaddr_t		my_host_id;

#if	LOCK_THREADS
    mutex_lock(thread_lock);
#endif	LOCK_THREADS

    MEM_ALLOC(req_msg_ptr,msg_header_t *,CAM_STUFF_MAX_MSG_SIZE, FALSE);
    MEM_ALLOC(rep_msg_ptr,msg_header_t *,CAM_STUFF_MAX_MSG_SIZE, FALSE);

    while (TRUE) {
	/*
	 * Wait for a name service request.
	 */
	req_msg_ptr->msg_size = CAM_STUFF_MAX_MSG_SIZE;
	req_msg_ptr->msg_local_port = ctPort;
	kr = netmsg_receive(req_msg_ptr);

	if (kr == RCV_SUCCESS) {
		mutex_lock(&camelot_lock);
		if (!(ct_server((caddr_t)req_msg_ptr, (caddr_t)rep_msg_ptr))) {
			ERROR((msg, "cam_stuff_main.ct_server fails, msg id = %d.", req_msg_ptr->msg_id));
			send_reply = FALSE;
		} else {
			send_reply = (((death_pill_t *)rep_msg_ptr)->RetCode != MIG_NO_REPLY);
		}
		mutex_unlock(&camelot_lock);
		if (send_reply) {
			/*
			 * Send the reply back.
			 */
			kr = msg_send(rep_msg_ptr, MSG_OPTION_NONE, 0);
			if (kr != SEND_SUCCESS) {
			    ERROR((msg, "cam_stuff_main.msg_send fails, kr = %d.", kr));
			}
		}
	} else {
	    ERROR((msg, "cam_stuff_main.netmsg_receive fails, kr = %d.", kr));
	}

	LOGCHECK;
    }

END



/*
 * cam_stuff_init
 *	Initialises the ct service.
 *
 * Results:
 *	TRUE or FALSE.
 *
 * Design:
 *	Call Cam_Initialize.
 *	Start up some number of threads to handle ct service requests.
 *
 * Note:
 *
 */
EXPORT boolean_t cam_stuff_init()
BEGIN("cam_stuff_init")
    int			i;
    cthread_t		new_thread;

    mutex_init(&camelot_lock);

    mutex_lock(&camelot_lock);
    Cam_Initialize();
    mutex_unlock(&camelot_lock);

    /*
     * ctPort starts by being enabled. If somebody
     * sends a message to it right now, you lose.
     */
    (void)port_disable(task_self(), ctPort);

    for (i = 0; i < CAM_STUFF_NUM_THREADS; i++) {
	new_thread = cthread_fork((cthread_fn_t)cam_stuff_main, (any_t)0);
	cthread_set_name(new_thread, "cam_stuff_main");
	cthread_detach(new_thread);
    }

    RETURN(TRUE);

END

#endif	CAMELOT
