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
 * $Log:	keyman.c,v $
 * Revision 1.1  90/02/19  21:52:04  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.13  89/05/02  11:10:36  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.12  89/04/24  20:40:12  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:53:51  dpj]
 * 
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 10-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Pass down network name service port to key distribution server.
 *	Initialise DES module.
 *
 *  5-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	The key returned by km_get_key depends on the current crypt algorithm.
 *	Replaced km_get_ikey by km_get_dkey.
 *	Add exec of kds (conditional on NM_USE_KDS);
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *	Added call to cthread_set_name.
 *	Replaced fprintf by ERROR macro.
 *
 * 25-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added km_get_ikey.
 *
 *  9-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * keyman.c
 *
 *
 */

#ifndef	lint
char keyman_rcsid[] = "$ Header: keyman.c,v 1.13 89/05/02 11:10:36 dpj Exp $";
#endif not lint

/*
 * Main routines of Key Management module.
 */


#include "netmsg.h"
#include "nm_defs.h"

#if	USE_KEYMAN

#if	NM_USE_KDS
#define MACH_INIT_SLOTS	1
#endif	NM_USE_KDS

#include <cthreads.h>
#include <mach.h>
#include <mach_init.h>
#include <mach/boolean.h>
#include <mach/mach_param.h>
#include <mach/message.h>
#include <sys/types.h>

#include <config.h>
#include "crypt.h"
#include "debug.h"
#include "key_defs.h"
#include "keyman.h"
#include "km_defs.h"
#include "ls_defs.h"
#include "mem.h"
#include "nm_extra.h"
#if	NM_USE_KDS
#include "sm_init_defs.h"
#endif	NM_USE_KDS

PRIVATE port_t		km_service_port;

#define KM_MAX_MSG_SIZE	64

extern boolean_t km_server();
extern port_t netname_port;


/*
 * km_main
 *	Waits for messages on the key management service port.
 *
 * Results:
 *	Ignored.
 *
 */
PRIVATE km_main()
BEGIN("km_main")
    kern_return_t	kr;
    msg_header_t	*in_msg_ptr, *rep_msg_ptr;

#if	LOCK_THREADS
    mutex_lock(thread_lock);
#endif	LOCK_THREADS

    MEM_ALLOC(in_msg_ptr,msg_header_t *,KM_MAX_MSG_SIZE, FALSE);
    MEM_ALLOC(rep_msg_ptr,msg_header_t *,KM_MAX_MSG_SIZE, FALSE);

    while (TRUE) {
	in_msg_ptr->msg_size = KM_MAX_MSG_SIZE;
	in_msg_ptr->msg_local_port = km_service_port;

	kr = netmsg_receive(in_msg_ptr);

	if (kr == RCV_SUCCESS) {
	    if (km_server((caddr_t)in_msg_ptr, (caddr_t)rep_msg_ptr)) {
		/*
		 * Send the reply back.
		 */
		if ((kr = msg_send(rep_msg_ptr, MSG_OPTION_NONE, 0)) != SEND_SUCCESS) {
		    ERROR((msg, "km_main.msg_send fails, kr = %d.", kr));
		}
	    }
	    else {
		ERROR((msg, "km_main.km_server fails, msg_id = %d.", in_msg_ptr->msg_id));
	    }
	}
	else {
	    ERROR((msg, "km_main.netmsg_receive fails, kr = %d.", kr));
	}
	LOGCHECK;
    }

END



/*
 * km_init
 *	Initialises the key management module.
 *	Allocates and registers the key management service port.
 *	Starts up the kds.
 *	Starts up a thread to execute the km_main loop.
 *
 * Results:
 *	TRUE or FALSE.
 *
 * Design:
 *	Initialises the key management utility functions.
 *
 */
EXPORT boolean_t km_init()
BEGIN("km_init")
    cthread_t		km_thread;
    kern_return_t	kr;
#if	NM_USE_KDS
    port_array_t	ports;
    u_int		ports_cnt;
    int			pid;
#endif	NM_USE_KDS

    if ((desinit(0)) != 0) {
	ERROR((msg, "km_init.desinit fails."));
	RETURN(FALSE);
    }
    km_procs_init();
    km_utils_init();

    if ((kr = port_allocate(task_self(), &km_service_port)) != KERN_SUCCESS) {
	ERROR((msg, "km_init.port_allocate fails, kr = %d.", kr));
	RETURN(FALSE);
    }
    LOG1(TRUE, 3, 1154, km_service_port);

#if	NM_USE_KDS

    if ((kr = mach_ports_lookup(task_self(), &ports, &ports_cnt)) != KERN_SUCCESS) {
	ERROR((msg, "km_init.mach_ports_lookup fails, kr = %d.", kr));
	RETURN(FALSE);
    }
    if (ports_cnt < TASK_PORT_REGISTER_MAX) {
	ERROR((msg, "km_init: not enough registered ports (%d).", ports_cnt));
	RETURN(FALSE);
    }
    /*
     * Fill in the ports array with the km_service_port and the netname_port.
     */
    ports[NETNAME_SLOT] = netname_port;
    ports[KM_SERVICE_SLOT] = km_service_port;
    if ((kr = mach_ports_register(task_self(), ports, ports_cnt)) != KERN_SUCCESS) {
	ERROR((msg, "km_init.mach_ports_register fails, kr = %d.", kr));
	RETURN(FALSE);
    }

    /*
     * Try to fork and exec the KDS.
     */
    pid = fork();
    if (pid < 0) {
	ERROR((msg, "km_init.fork fails."));
	RETURN(FALSE);
    }
    else if (pid == 0) {
	if (execl("kds", "kds", (char *)0) < 0) {
	    fprintf(stderr, "km_init.exec of kds failed.\n");
	}
	_exit(0);
	/*
	 * Should not return from here.
	 */
    }
    else {
	LOG1(TRUE, 3, 1152, pid);
    }

#else	NM_USE_KDS

    if ((kr = _netname_check_in(PORT_NULL, KM_SERVICE_NAME, task_self(), km_service_port)) != KERN_SUCCESS)
    {
	ERROR((msg, "km_init._netname_check_in fails, kr = %d.", kr));
	RETURN(FALSE);
    }

#endif	NM_USE_KDS

    km_thread = cthread_fork((cthread_fn_t)km_main, (any_t)0);
    cthread_set_name(km_thread, "km_main");
    cthread_detach(km_thread);

    RETURN(TRUE);

END



/*
 * km_get_key
 *
 * Parameters:
 *	host_id		: the host for which a key is required.
 *	key_ptr		: pointer to space to put the key found.
 *
 * Results:
 *	TRUE if there exists a key for that host, FALSE otherwise.
 *
 * Design:
 *	Just calls km_host_lookup.
 *
 */
EXPORT boolean_t km_get_key(host_id, key_ptr)
netaddr_t	host_id;
key_ptr_t	key_ptr;
BEGIN("km_get_key")
    key_rec_ptr_t	key_rec_ptr;

    if (param.crypt_algorithm == CRYPT_NULL) {
	RETURN(TRUE);
    }
    else if ((key_rec_ptr = km_host_lookup(host_id)) == KEY_REC_NULL) {
	RETURN(FALSE);
    }
    else {
	if (param.crypt_algorithm == CRYPT_MULTPERM) *key_ptr = key_rec_ptr->kr_mpkey;
	else *key_ptr = key_rec_ptr->kr_key;
	RETURN(TRUE);
    }

END



/*
 * km_get_dkey
 *	returns the decryption key for a particular host.
 *
 * Parameters:
 *	host_id		: the host for which a key is required.
 *	dkey_ptr	: pointer to space to put the decryption key found.
 *
 * Results:
 *	TRUE if there exists a key for that host, FALSE otherwise.
 *
 * Design:
 *	Just calls km_host_lookup.
 *
 */
EXPORT boolean_t km_get_dkey(host_id, dkey_ptr)
netaddr_t	host_id;
key_ptr_t	dkey_ptr;
BEGIN("km_get_dkey")
    key_rec_ptr_t	key_rec_ptr;

    if (param.crypt_algorithm == CRYPT_NULL) {
	RETURN(TRUE);
    }
    else if ((key_rec_ptr = km_host_lookup(host_id)) == KEY_REC_NULL) {
	RETURN(FALSE);
    }
    else {
	if (param.crypt_algorithm == CRYPT_MULTPERM) *dkey_ptr = key_rec_ptr->kr_mpikey;
	else *dkey_ptr = key_rec_ptr->kr_key;
	RETURN(TRUE);
    }

END

#else	USE_KEYMAN

EXPORT boolean_t km_get_key()
BEGIN("km_get_key")
	RETURN(FALSE);
END


EXPORT boolean_t km_get_dkey()
BEGIN("km_get_dkey")
	RETURN(FALSE);
END


#endif	USE_KEYMAN
