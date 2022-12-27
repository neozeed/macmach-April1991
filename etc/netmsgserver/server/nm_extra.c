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
 * $Log:	nm_extra.c,v $
 * Revision 1.2  90/02/25  17:24:37  bww
 * 	Pick up cthreads.h using <> rather than "".
 * 	[90/02/25  17:22:45  bww]
 * 
 * Revision 1.1  90/02/19  21:54:02  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.13  89/05/02  11:14:12  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replace netmsg_receive by netmsg_receive_locked which should
 *	only be called if we are locking threads.
 *
 * 30-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *
 * 13-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Try just calling the non-blocking msg_receive provided by cthreads.
 *	Replaced net_recursion_level by trace_recursion_level
 *	and net_trace tracing_on.  Removed net_debug_level.
 *
 * 25-Mar-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added include of config.h.
 *	Removed panic().
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed netmsg_send.
 *	Made debugging variables be lower case.
 *
 * 24-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nm_extra.c
 *
 *
 */
#ifndef	lint
char nm_extra_rcsid[] = "$ Header: nm_extra.c,v 1.13 89/05/02 11:14:12 dpj Exp $";
#endif not lint

/*
 * Things that do not belong anywhere else!
 */


#define NM_EXTRA_DEBUG	0

#include <mach.h>
#include <stdio.h>
#include <mach/message.h>

#include "config.h"
#include "debug.h"
#include "netmsg.h"
#include "nm_defs.h"
#include <cthreads.h>


/*
 * Tracing values.
 */
int		trace_recursion_level = 0;



/*
 * ipaddr_to_string
 *	Place an IP address into a string.
 *
 * Parameters:
 *	output_string	: the string to use
 *	input_address	: the address to used
 *
 */
EXPORT void ipaddr_to_string(output_string, input_address)
char		*output_string;
netaddr_t	input_address;
BEGIN("ipaddr_to_string")
    ip_addr_t	ip_address;

    ip_address.ia_netaddr = input_address;
    (void)sprintf(output_string, "%d.%d.%d.%d",
		ip_address.ia_bytes.ia_net_owner,
		ip_address.ia_bytes.ia_net_node_type,
		ip_address.ia_bytes.ia_host_high,
		ip_address.ia_bytes.ia_host_low);
    RET;

END



#if	LOCK_THREADS
/*
 * netmsg_receive_locked
 *	Does a non-blocking and unlocked receive on the local port contained in the message header.
 *
 * Parameters:
 *	msg_ptr		: pointer to a message buffer for the receive operation.
 *
 * Results:
 *	Whatever msg_receive returns.
 *
 * Note:
 *	Should only be called if lock_threads is turned on.
 *
 */
EXPORT netmsg_receive_locked(msg_ptr)
msg_header_t		*msg_ptr;
BEGIN("netmsg_receive_locked")
	register kern_return_t	mr;

	mutex_unlock(thread_lock);
	mr = msg_receive(msg_ptr, MSG_OPTION_NONE, 0);
	mutex_lock(thread_lock);
	RETURN(mr);

END
#endif	LOCK_THREADS
