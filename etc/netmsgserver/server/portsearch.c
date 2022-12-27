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
 * $Log:	portsearch.c,v $
 * Revision 1.1  90/02/19  21:55:49  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.16  89/05/02  11:16:34  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.15  89/04/24  20:42:12  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:57:23  dpj]
 * 
 * 09-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Conditionalize encrypt code.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  7-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for dispatcher with version number.
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new style of NETPORT calls.
 *
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added NETPORT.
 *
 * 22-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed (void) cast from lk_lock calls.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Set the aliveness of a port to PORT_ACTIVE on successful
 *	termination of a port search.
 *	Added some statistics gathering.
 *
 * 29-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *	Lock is now inline in port record.
 *	Added some debugging - controlled by PS_DEBUG.
 *
 * 18-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * portsearch.c
 *
 *
 */

#ifndef	lint
char portsearch_rcsid[] = "$ Header: portsearch.c,v 1.16 89/05/02 11:16:34 dpj Exp $";
#endif not lint

/*
 * Main functions of port search module.
 */


#include <mach/boolean.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "config.h"
#include "crypt.h"
#include "debug.h"
#include "disp_hdr.h"
#include "dispatcher.h"
#include "ipc.h"
#include "keyman.h"
#include "netmsg.h"
#include "network.h"
#include "nm_defs.h"
#include "nm_extra.h"
#include "nn.h"
#include "port_defs.h"
#include "portops.h"
#include "portrec.h"
#include "portsearch.h"
#include "ps_defs.h"
#include "rwlock.h"
#include "sbuf.h"
#include "transport.h"

PRIVATE ps_handle_reply();

/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_PSEVENT;


/*
 * ps_do_port_search
 *	initiates a port search.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to port record for this network port.
 *	new_information	: whether the caller has new information about the network port.
 *	new_nport_ptr	: pointer to the new information (in the form of a network port).
 *	retry		: function to call if the port search is successful
 *
 * Results:
 *	none.
 *
 * Design:
 *	Create a new port search event.
 *	Send out a query to, by default, the receiver for the port.
 *	However if the new information says that the receiver has moved, query the owner.
 *
 * Notes:
 *	The port record should be locked.
 *
 */
EXPORT void ps_do_port_search(port_rec_ptr, new_information, new_nport_ptr, retry)
port_rec_ptr_t		port_rec_ptr;
boolean_t		new_information;
network_port_ptr_t	new_nport_ptr;
int			(*retry)();
BEGIN("ps_do_port_search")
    ps_event_ptr_t	event_ptr;

    /*
     * Check for bogus case.
     */
    if ((port_rec_ptr->portrec_network_port.np_receiver == my_host_id)
	&& (port_rec_ptr->portrec_network_port.np_owner == my_host_id))
    {
	LOG0(TRUE, 4, 1220);
	LOG_NPORT(TRUE, 3, port_rec_ptr->portrec_network_port);
	RET;
    }

    DEBUG1(PS_DEBUG, 3, 1221, new_information);
    DEBUG_NPORT(PS_DEBUG, 3, port_rec_ptr->portrec_network_port);

    MEM_ALLOCOBJ(event_ptr,ps_event_ptr_t,MEM_PSEVENT);
    event_ptr->pse_lport = port_rec_ptr->portrec_local_port;
    event_ptr->pse_retry = retry;
    event_ptr->pse_state = 0;

#if	NETPORT
    ipc_netport_remove(port_rec_ptr->portrec_network_port);
#endif	NETPORT

    port_rec_ptr->portrec_info |= PORT_INFO_SUSPENDED;
    if ((port_rec_ptr->portrec_network_port.np_receiver == my_host_id)
	|| ((new_information)
	    && (port_rec_ptr->portrec_network_port.np_receiver != new_nport_ptr->np_receiver)
	    && (port_rec_ptr->portrec_network_port.np_owner != my_host_id)))
    {
	/*
	 * Query the owner.
	 */
	event_ptr->pse_state = PS_OWNER_QUERIED;
	event_ptr->pse_destination = port_rec_ptr->portrec_network_port.np_owner;
	DEBUG0(PS_DEBUG, 3, 1222)
	ps_send_query(event_ptr, port_rec_ptr);
    }
    else {
	/*
	 * Query the receiver.
	 */
	event_ptr->pse_state = PS_RECEIVER_QUERIED;
	event_ptr->pse_destination = port_rec_ptr->portrec_network_port.np_receiver;
	DEBUG0(PS_DEBUG, 3, 1223)
	ps_send_query(event_ptr, port_rec_ptr);
    }

    RET;

END



/*
 * ps_cleanup
 *	called if a transmission failed for some reason.
 *
 * Parameters:
 *	event_ptr	: pointer to the event for which the transmission failed.
 *	reason		: reason why the transmission failed.
 *
 * Results:
 *	meaningless.
 *
 * Design:
 *	If the reason was a crypt_failure call km_do_key_exchange.
 *	Otherwise feed ps_handle_reply a PS_PORT_UNKNOWN response.
 *
 */
PUBLIC ps_cleanup(event_ptr, reason)
ps_event_ptr_t	event_ptr;
int		reason;
BEGIN("ps_cleanup")
    ps_data_t		data;
    sbuf_t		sbuf;
    sbuf_seg_t		sbuf_seg;
    port_rec_ptr_t	port_rec_ptr;

    if (reason == TR_CRYPT_FAILURE) {
	km_do_key_exchange((int)event_ptr, ps_retry, event_ptr->pse_destination);
    }
    else {
	if ((port_rec_ptr = pr_lportlookup(event_ptr->pse_lport)) == PORT_REC_NULL) {
	    LOG1(TRUE, 4, 1224, event_ptr->pse_lport);
	    RETURN(0);
	}
	/* port_rec_ptr LOCK RW/RW */

	SBUF_SEG_INIT(sbuf, &sbuf_seg);
	SBUF_APPEND(sbuf, &data, sizeof(ps_data_t));
	data.psd_status = htonl((unsigned long)PS_PORT_UNKNOWN);
	data.psd_puid = port_rec_ptr->portrec_network_port.np_puid;
	data.psd_owner = port_rec_ptr->portrec_network_port.np_owner;
	data.psd_receiver = port_rec_ptr->portrec_network_port.np_receiver;

	lk_unlock(&port_rec_ptr->portrec_lock);
	/* port_rec_ptr LOCK -/- */

	DEBUG1(PS_DEBUG, 3, 1225, event_ptr->pse_lport);
	(void)ps_handle_reply((int)event_ptr, &sbuf, event_ptr->pse_destination, FALSE, 0);
    }

    RETURN(0);

END



/*
 * ps_handle_reply
 *	handles a reply to a port search query.
 *
 * Parameters:
 *	client_id	: pointer to port search event.
 *	reply		: the data of the reply.
 *	from		: the host that sent the reply.
 *	broadcast	: ignored.
 * 	crypt_level	: ignored.
 *
 * Design:
 *	If we got back good information stop the search.
 *	Otherwise proceed with the next phase of the search.
 *
 * Returns:
 *	DISP_SUCCESS
 *
 * Side effects:
 *	May destroy the port.
 *	May decide that we have receive/ownership rights to the port.
 *
 */
/* ARGSUSED */
PRIVATE ps_handle_reply(client_id, reply, from, broadcast, crypt_level)
int		client_id;
sbuf_ptr_t	reply;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("ps_handle_reply")
    port_rec_ptr_t	port_rec_ptr;
    ps_data_ptr_t	reply_ptr;
    ps_event_ptr_t	event_ptr;
    int			status;

    INCSTAT(ps_replies_rcvd);
    SBUF_GET_SEG(*reply, reply_ptr, ps_data_ptr_t);
    status = ntohl(reply_ptr->psd_status);
    event_ptr = (ps_event_ptr_t)client_id;

    if ((port_rec_ptr = pr_np_puid_lookup(reply_ptr->psd_puid)) == PORT_REC_NULL) {
	LOG2(PS_DEBUG, 3, 1227, reply_ptr->psd_puid.np_uid_high, reply_ptr->psd_puid.np_uid_low);
	MEM_DEALLOCOBJ(event_ptr, MEM_PSEVENT);
	RETURN(DISP_SUCCESS);
    }
    /* port_rec_ptr LOCK RW/RW */

    DEBUG1(PS_DEBUG, 3, 1228, status)
    DEBUG_NPORT(PS_DEBUG, 3, port_rec_ptr->portrec_network_port)

    switch (status) {
	case PS_PORT_HERE: {
	    port_rec_ptr->portrec_network_port.np_receiver = reply_ptr->psd_receiver;
	    port_rec_ptr->portrec_network_port.np_owner = reply_ptr->psd_owner;
#if	USE_CRYPT
	    if ((event_ptr->pse_destination == broadcast_address)
		&& (port_rec_ptr->portrec_security_level != CRYPT_DONT_ENCRYPT))
	    {
		event_ptr->pse_destination = reply_ptr->psd_receiver;
		ps_authenticate_port(event_ptr, port_rec_ptr);
	    }
	    else {
#endif	USE_CRYPT
		/*
		 * We can terminate the port search successfully.
		 */
		DEBUG0(PS_DEBUG, 3, 1229);
		port_rec_ptr->portrec_info &= ~PORT_INFO_SUSPENDED;
		port_rec_ptr->portrec_aliveness = PORT_ACTIVE;
#if	NETPORT
		ipc_netport_enter(port_rec_ptr->portrec_network_port,
			port_rec_ptr->portrec_local_port,
			(port_rec_ptr->portrec_network_port.np_receiver == my_host_id));
#endif	NETPORT
		if (event_ptr->pse_retry) (void)event_ptr->pse_retry(port_rec_ptr);
		MEM_DEALLOCOBJ(event_ptr, MEM_PSEVENT);

#if	USE_CRYPT
	    }
#endif	USE_CRYPT
	    break;
	}

	case PS_PORT_DEAD: {
	    /*
	     * Can safely destroy this port.
	     */
	    DEBUG0(PS_DEBUG, 3, 1230);
	    ipc_port_dead(port_rec_ptr);
	    nn_remove_entries(port_rec_ptr->portrec_local_port);
	    pr_destroy(port_rec_ptr);
	    MEM_DEALLOCOBJ(event_ptr, MEM_PSEVENT);
	    RETURN(DISP_SUCCESS);
	}

	case PS_PORT_UNKNOWN: {
	    /*
	     * If we have made a broadcast search already,
	     *     then destroy the port if we have send rights to it
	     *     otherwise we must now be both receiver and owner.
	     * If we have queried either the receiver or the owner then query the other.
	     * If we have queried both (or if they are the same) resort to a broadcast search./
	     */
	    if (event_ptr->pse_state & PS_DONE_BROADCAST) {
		if (NPORT_HAVE_SEND_RIGHTS(port_rec_ptr->portrec_network_port)) {
		    DEBUG0(PS_DEBUG, 3, 1231);
		    ipc_port_dead(port_rec_ptr);
		    nn_remove_entries(port_rec_ptr->portrec_local_port);
		    pr_destroy(port_rec_ptr);
		    MEM_DEALLOCOBJ(event_ptr, MEM_PSEVENT);
		    RETURN(DISP_SUCCESS);
		}
		else {
		    DEBUG0(PS_DEBUG, 3, 1232);
		    PORT_REC_RECEIVER(port_rec_ptr) = PORT_REC_OWNER(port_rec_ptr) = my_host_id;
		    po_port_deallocate(port_rec_ptr->portrec_local_port);
		    port_rec_ptr->portrec_info &= ~PORT_INFO_SUSPENDED;
#if	NETPORT
		    ipc_netport_enter(port_rec_ptr->portrec_network_port,
			port_rec_ptr->portrec_local_port,
			(port_rec_ptr->portrec_network_port.np_receiver == my_host_id));
#endif	NETPORT
		    if (event_ptr->pse_retry) (void)event_ptr->pse_retry(port_rec_ptr);
		    MEM_DEALLOCOBJ(event_ptr, MEM_PSEVENT);
		}
	    }
	    else if ((PORT_REC_RECEIVER(port_rec_ptr) == PORT_REC_OWNER(port_rec_ptr))
		    || ((event_ptr->pse_state & PS_RECEIVER_QUERIED)
			&& (event_ptr->pse_state & PS_OWNER_QUERIED)))
	    {
		/*
		 * Resort to a broadcast search.
		 */
		DEBUG0(PS_DEBUG, 3, 1233);
		event_ptr->pse_state |= PS_DONE_BROADCAST;
		event_ptr->pse_destination = broadcast_address;
		ps_send_query(event_ptr, port_rec_ptr);
	    }
	    else {
		/*
		 * Query which ever one of the receiver and owner we have not queried yet.
		 */
		if (event_ptr->pse_state & PS_RECEIVER_QUERIED) {
		    event_ptr->pse_state |= PS_OWNER_QUERIED;
		    event_ptr->pse_destination = PORT_REC_OWNER(port_rec_ptr);
		    DEBUG0(PS_DEBUG, 3, 1222);
		}
		else {
		    event_ptr->pse_state |= PS_RECEIVER_QUERIED;
		    event_ptr->pse_destination = PORT_REC_RECEIVER(port_rec_ptr);
		    DEBUG0(PS_DEBUG, 3, 1223);
		}
		ps_send_query(event_ptr, port_rec_ptr);
	    }
	    break;
	}
	
	case PS_OWNER_MOVED: {
	    /*
	     * We must have queried the old owner, try querying the new owner.
	     */
	    event_ptr->pse_destination = PORT_REC_OWNER(port_rec_ptr) = reply_ptr->psd_owner;
	    DEBUG0(PS_DEBUG, 3, 1234);
	    ps_send_query(event_ptr, port_rec_ptr);
	    break;
	}

	case (PS_OWNER_MOVED | PS_PORT_HERE): {
	    PORT_REC_RECEIVER(port_rec_ptr) = reply_ptr->psd_receiver;
	    PORT_REC_OWNER(port_rec_ptr) = reply_ptr->psd_owner;
#if	USE_CRYPT
	    if ((event_ptr->pse_destination == broadcast_address)
		&& (port_rec_ptr->portrec_security_level != CRYPT_DONT_ENCRYPT))
	    {
		event_ptr->pse_destination = reply_ptr->psd_receiver;
		ps_authenticate_port(event_ptr, port_rec_ptr);
	    }
	    else {
#endif	USE_CRYPT
		/*
		 * We must have queried the receiver, try querying the new owner.
		 */
		event_ptr->pse_state |= PS_OWNER_QUERIED;
		event_ptr->pse_destination = PORT_REC_OWNER(port_rec_ptr) = reply_ptr->psd_owner;
		DEBUG0(PS_DEBUG, 3, 1234);
		ps_send_query(event_ptr, port_rec_ptr);
#if	USE_CRYPT
	    }
#endif	USE_CRYPT
	    break;
	}

	case PS_RECEIVER_MOVED: {
	    /*
	     * We must have queried the old receiver, try querying the new receiver.
	     */
	    event_ptr->pse_destination = PORT_REC_RECEIVER(port_rec_ptr) = reply_ptr->psd_receiver;
	    DEBUG0(PS_DEBUG, 3, 1235);
	    ps_send_query(event_ptr, port_rec_ptr);
	    break;
	}

	case (PS_RECEIVER_MOVED | PS_PORT_HERE): {
	    PORT_REC_OWNER(port_rec_ptr) = reply_ptr->psd_owner;
	    PORT_REC_RECEIVER(port_rec_ptr) = reply_ptr->psd_receiver;
#if	USE_CRYPT
	    if ((event_ptr->pse_destination == broadcast_address)
		&& (port_rec_ptr->portrec_security_level != CRYPT_DONT_ENCRYPT))
	    {
		event_ptr->pse_destination = reply_ptr->psd_owner;
		ps_authenticate_port(event_ptr, port_rec_ptr);
	    }
	    else {
#endif	USE_CRYPT
		/*
		 * We must have queried the owner, try querying the new receiver.
		 */
		event_ptr->pse_state |= PS_RECEIVER_QUERIED;
		event_ptr->pse_destination = reply_ptr->psd_receiver;
		DEBUG0(PS_DEBUG, 3, 1235);
		ps_send_query(event_ptr, port_rec_ptr);
#if	USE_CRYPT
	    }
#endif	USE_CRYPT
	    break;
	}

	case (PS_OWNER_MOVED | PS_RECEIVER_MOVED): {
	    /*
	     * Just keep on querying with the new information.
	     */
	    PORT_REC_OWNER(port_rec_ptr) = reply_ptr->psd_owner;
	    PORT_REC_RECEIVER(port_rec_ptr) = reply_ptr->psd_receiver;
	    event_ptr->pse_destination = (event_ptr->pse_state & PS_RECEIVER_QUERIED)
					? reply_ptr->psd_receiver : reply_ptr->psd_owner;
	    ps_send_query(event_ptr, port_rec_ptr);
	    break;
	}

	default: {
	    LOG1(TRUE, 5, 1226, status);
	}
    }

    lk_unlock(&port_rec_ptr->portrec_lock);
    RETURN(DISP_SUCCESS);

END



/*
 * po_handle_query
 *	handles an incoming port search query.
 *
 * Parameters:
 *	request		: the data of the query.
 *	from		: the host that sent the query (ignored).
 *	broadcast	: was the query broadcast.
 * 	crypt_level	: ignored.
 *
 * Design:
 *	Respond with the information we know about this port.
 *
 * Returns:
 *	DISP_IGNORE if the request was broadcast and we are not the owner/receiver for this port.
 *	DISP_SUCCESS otherwise.
 *
 */
/* ARGSUSED */
PRIVATE ps_handle_query(query, from, broadcast, crypt_level)
sbuf_ptr_t	query;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("ps_handle_query")
    port_rec_ptr_t	port_rec_ptr;
    ps_data_ptr_t	query_ptr;
    boolean_t		ignore = FALSE;

    INCSTAT(ps_requests_rcvd);
    SBUF_GET_SEG(*query, query_ptr, ps_data_ptr_t);
    query_ptr->psd_disp_hdr.src_format = conf_own_format;

    DEBUG2(PS_DEBUG, 3, 1237, query_ptr->psd_puid.np_uid_high, query_ptr->psd_puid.np_uid_low);

    if ((port_rec_ptr = pr_np_puid_lookup(query_ptr->psd_puid)) == PORT_REC_NULL) {
	if (broadcast) {
	    RETURN(DISP_IGNORE);
	}
	else {
	    query_ptr->psd_status = htonl((unsigned long)PS_PORT_UNKNOWN);
	    RETURN(DISP_SUCCESS);
	}
    }
    /* port_rec_ptr LOCK RW/R */

    if (port_rec_ptr->portrec_info & PORT_INFO_DEAD) {
	query_ptr->psd_status = htonl((unsigned long)PS_PORT_DEAD);
    }
    else if ((port_rec_ptr->portrec_network_port.np_receiver == my_host_id)
	    && (port_rec_ptr->portrec_network_port.np_receiver == my_host_id))
    {
	/*
	 * We hold all rights to this network port.
	 */
	query_ptr->psd_receiver = my_host_id;
	query_ptr->psd_owner = my_host_id;
	query_ptr->psd_status = htonl((unsigned long)PS_PORT_HERE);
    }
    else if (port_rec_ptr->portrec_network_port.np_receiver == my_host_id) {
	/*
	 * We are just the receiver.
	 */
	query_ptr->psd_receiver = my_host_id;
	if (query_ptr->psd_owner != port_rec_ptr->portrec_network_port.np_owner) {
	    /*
	     * The owner has moved.
	     */
	    query_ptr->psd_owner = port_rec_ptr->portrec_network_port.np_owner;
	    query_ptr->psd_status = htonl((unsigned long)PS_OWNER_MOVED | PS_PORT_HERE);
	}
	else {
	    query_ptr->psd_status = htonl((unsigned long)PS_PORT_HERE);
	}
    }
    else if (port_rec_ptr->portrec_network_port.np_owner == my_host_id) {
	/*
	 * We are just the owner.
	 */
	query_ptr->psd_owner = my_host_id;
	if (query_ptr->psd_receiver != port_rec_ptr->portrec_network_port.np_receiver) {
	    /*
	     * The receiver has moved.
	     */
	    query_ptr->psd_receiver = port_rec_ptr->portrec_network_port.np_receiver;
	    query_ptr->psd_status = htonl((unsigned long)PS_RECEIVER_MOVED | PS_PORT_HERE);
	}
	else {
	    query_ptr->psd_status = htonl((unsigned long)PS_PORT_HERE);
	}
    }
    else {
	/*
	 * We only have send rights to this port.
	 */
	if (broadcast) {
	    ignore = TRUE;
	}
	else {
	    if ((query_ptr->psd_receiver != port_rec_ptr->portrec_network_port.np_receiver)
		&& (query_ptr->psd_owner != port_rec_ptr->portrec_network_port.np_owner))
	    {
		query_ptr->psd_status = htonl((unsigned long)PS_OWNER_MOVED | PS_RECEIVER_MOVED);
	    }
	    else if (query_ptr->psd_receiver != port_rec_ptr->portrec_network_port.np_receiver) {
		query_ptr->psd_status = htonl((unsigned long)PS_RECEIVER_MOVED);
	    }
	    else if (query_ptr->psd_owner != port_rec_ptr->portrec_network_port.np_owner) {
		query_ptr->psd_status = htonl((unsigned long)PS_OWNER_MOVED);
	    }
	    query_ptr->psd_receiver = port_rec_ptr->portrec_network_port.np_receiver;
	    query_ptr->psd_owner = port_rec_ptr->portrec_network_port.np_owner;
	}
    }

    DEBUG2(PS_DEBUG, 3, 1238, ignore, ntohl(query_ptr->psd_status));

    lk_unlock(&port_rec_ptr->portrec_lock);
    if (ignore) {
	RETURN(DISP_IGNORE);
    }
    else {
	RETURN(DISP_SUCCESS);
    }

END



/*
 * ps_retry
 *	retries a port search query.
 *
 * Parameters:
 *	event_ptr	: pointer to port search event to be retried.
 *
 * Design:
 *	just calls ps_send_query.
 *
 * Results:
 *	meaningless.
 *
 */
PUBLIC ps_retry(event_ptr)
ps_event_ptr_t	event_ptr;
BEGIN("ps_retry")
    port_rec_ptr_t	port_rec_ptr;

    if ((port_rec_ptr = pr_lportlookup(event_ptr->pse_lport)) == PORT_REC_NULL) {
	LOG1(TRUE, 5, 1239, event_ptr->pse_lport);
	RETURN(0);
    }
    /* port_rec_ptr LOCK RW/RW */

    ps_send_query(event_ptr, port_rec_ptr);

    lk_unlock(&port_rec_ptr->portrec_lock);
    RETURN(0);

END



/*
 * ps_send_query
 *	sends out a port search query.
 *
 * Parameters:
 *	event_ptr	: pointer to the event for which this query is to be made.
 *	port_rec_ptr	: pointer to the port record for the port to be queried.
 *
 * Design:
 *	constructs a port search query.
 *	Sends it at the appropriate security level to the named destination.
 *
 * Side Effects:
 *	calls km_do_key_exchange or ps_cleanup if the transport send fails.
 *
 * Notes:
 *	the port_rec_ptr should be locked.
 *	the destination may be the broadcast address in which case we send the query unencrypted.
 *
 */
PUBLIC void ps_send_query(event_ptr, port_rec_ptr)
    ps_event_ptr_t	event_ptr;
    port_rec_ptr_t	port_rec_ptr;
BEGIN("ps_send_query")
    sbuf_t	sbuf;
    sbuf_seg_t	sbuf_seg;
    ps_data_t	data;
    int		tr, crypt_level;

    DEBUG0(PS_DEBUG, 3, 1236);
    DEBUG_NETADDR(PS_DEBUG, 3, event_ptr->pse_destination);
    DEBUG_NPORT(PS_DEBUG, 3, port_rec_ptr->portrec_network_port);

    SBUF_SEG_INIT(sbuf, &sbuf_seg);
    SBUF_APPEND(sbuf, &data, sizeof(ps_data_t));

    data.psd_disp_hdr.disp_type = htons(DISP_PORTSEARCH);
    data.psd_disp_hdr.src_format = conf_own_format;
    data.psd_puid = port_rec_ptr->portrec_network_port.np_puid;
    data.psd_owner = port_rec_ptr->portrec_network_port.np_owner;
    data.psd_receiver = port_rec_ptr->portrec_network_port.np_receiver;

    crypt_level = (event_ptr->pse_destination == broadcast_address)
			? CRYPT_DONT_ENCRYPT : port_rec_ptr->portrec_security_level;

    tr = transport_switch[TR_SRR_ENTRY].send(event_ptr, &sbuf, event_ptr->pse_destination, TRSERV_NORMAL,
						crypt_level, ps_cleanup);
    if (tr != TR_SUCCESS) {
	ERROR((msg, "ps_send_query.send fails, tr = %d.", tr));
	if (tr == TR_CRYPT_FAILURE) {
	    km_do_key_exchange((int)event_ptr, ps_retry, event_ptr->pse_destination);
	}
	else {
	    lk_unlock(&port_rec_ptr->portrec_lock);
	    (void)ps_cleanup(event_ptr, tr);
	    lk_lock(&port_rec_ptr->portrec_lock, PERM_READWRITE, TRUE);
	}
    }
    else INCSTAT(ps_requests_sent);

    RET;
END



/*
 * ps_init
 *	initialises the port search module by setting up the dispatcher entries.
 *
 * Returns:
 *	TRUE
 *
 */
EXPORT boolean_t ps_init()
BEGIN("ps_init")

    /*
     * Initialize the memory management facilities.
     */
    mem_initobj(&MEM_PSEVENT,"PS event",sizeof(ps_event_t),FALSE,250,5);


    dispatcher_switch[DISPE_PORTSEARCH].disp_indata_simple = ps_handle_reply;
    dispatcher_switch[DISPE_PORTSEARCH].disp_rr_simple = ps_handle_query;

#if	USE_MULTPERM
    dispatcher_switch[DISPE_PS_AUTH].disp_indata_simple = ps_handle_auth_reply;
    dispatcher_switch[DISPE_PS_AUTH].disp_rr_simple = ps_handle_auth_request;
#endif	USE_MULTPERM

    RETURN(TRUE);

END

