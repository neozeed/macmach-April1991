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
 * $Log:	nn_handler.c,v $
 * Revision 1.1  90/02/19  21:54:31  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.17  89/05/02  11:14:48  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 31-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Work around a race in the chtreads library:
 *	the lock on the lookup record must be held during
 *	condition_signal(), to prevent the condition from being
 *	deallocated before condition_signal() has exited.
 *	condition_clear() DOES NOT do the job right now.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 28-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed locking and condition handling for lookup request records.
 *
 * 26-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added debugging around conditions.
 *
 * 15-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed a syntax error in the NETPORT option.
 *
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added NETPORT.
 *
 * 29-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Use NPORT_EQUAL macro instead of pr_nport_equal function.
 *
 * 15-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added some debugging output.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added some statistics gathering.
 *
 * 27-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Lock and condition are now inline in request record.
 *	Changed nn_table to consist of inline lock_queue records.
 *	Lock is now inline in port record.
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *
 * 16-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced fprintf by ERROR macro.
 *
 *  6-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced SBUF_INIT by SBUF_SEG_INIT.
 *
 * 27-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nn_handler.c
 *
 *
 */

#ifndef	lint
char nn_handler_rcsid[] = "$ Header: nn_handler.c,v 1.17 89/05/02 11:14:48 dpj Exp $";
#endif not lint

/*
 * Functions handling network request and replies for the network name service module.
 */


#include <cthreads.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>

#include "crypt.h"
#include "debug.h"
#include "dispatcher.h"
#include "mem.h"
#include "netmsg.h"
#include "netname_defs.h"
#include "nm_extra.h"
#include "nn_defs.h"
#include "portrec.h"
#include "port_defs.h"
#include "sbuf.h"
#include "transport.h"
#include "network.h"
#include "ipc.h"

/*
 * Flag for debugging conditions handling.
 */
#define	NN_COND		0x1000


/*
 * nn_cleanup
 *	Called by the transport module if a request has failed.
 *
 * Parameters:
 *	client_id	: a pointer to the original request record.
 *	completion_code	: the reason for calling cleanup
 *
 * Design:
 *	Sets the result in the request record to NETNAME_HOST_NOT_FOUND
 *	and signals on the condition in the record.
 *
 */
PRIVATE nn_cleanup(client_id, completion_code)
int	client_id;
int	completion_code;
BEGIN("nn_cleanup")
    nn_req_rec_ptr_t	req_rec_ptr;

#ifdef lint
    completion_code;
#endif lint

    DEBUG2(debug.netname,0,3013,NETNAME_HOST_NOT_FOUND,client_id);
    req_rec_ptr = (nn_req_rec_ptr_t)client_id;
    mutex_lock(&req_rec_ptr->nnrr_lock);
    req_rec_ptr->nnrr_lport = 0;
    req_rec_ptr->nnrr_result = NETNAME_HOST_NOT_FOUND;
    DEBUG2(debug.netname & NN_COND,0,3014,&req_rec_ptr->nnrr_condition,
				req_rec_ptr->nnrr_condition.queue.head);
    condition_signal(&req_rec_ptr->nnrr_condition);
    mutex_unlock(&req_rec_ptr->nnrr_lock);
    DEBUG2(debug.netname & NN_COND,0,3015,&req_rec_ptr->nnrr_condition,
				req_rec_ptr->nnrr_condition.queue.head);

    RETURN(0);

END



/*
 * nn_handle_reply
 *	Handles the results of a network name lookup request.
 *	It is called by the srr transport protocol.
 *
 * Parameters:
 *	client_id	: pointer to the request record which generated this reply
 *	reply		: pointer to the reply sbuf
 *	from		: host from which the reply was received - ignored.
 *	broadcast	: ignored
 *	crypt_level	: ignored
 *
 * Design:
 *	If the network port returned is not the null network port,
 *	call pr_ntran to get the corresponding local port.
 *	Fill in the fields of the request record to reflect the result.
 *	Signals on the condition in the request record.
 *
 */
/* ARGSUSED */
PUBLIC int nn_handle_reply(client_id, reply, from, broadcast, crypt_level)
int		client_id;
sbuf_ptr_t	reply;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("nn_handle_reply")
    nn_req_ptr_t	rep_ptr;
    nn_req_rec_ptr_t	req_rec_ptr;
    port_rec_ptr_t	port_rec_ptr;

    DEBUG1(debug.netname,0,3007,client_id);
    INCSTAT(nn_replies_rcvd);
    SBUF_GET_SEG(*reply, rep_ptr, nn_req_ptr_t);
    req_rec_ptr = (nn_req_rec_ptr_t)client_id;
    mutex_lock(&req_rec_ptr->nnrr_lock);

    if (NPORT_EQUAL(null_network_port, rep_ptr->nnr_nport)) {
	req_rec_ptr->nnrr_result = NETNAME_NOT_CHECKED_IN;
	req_rec_ptr->nnrr_lport = 0;
	DEBUG0(debug.netname,0,3008);
    }
    else {
	if ((port_rec_ptr = pr_ntran((network_port_ptr_t)&(rep_ptr->nnr_nport))) == PORT_REC_NULL) {
	    char nport_string[40];
	    pr_nporttostring(nport_string, (network_port_ptr_t)&(rep_ptr->nnr_nport));
	    ERROR((msg, "nn_handle_reply.pr_ntran fails, network port = %s.\n", nport_string));
	    req_rec_ptr->nnrr_result = NETNAME_HOST_NOT_FOUND;
	    req_rec_ptr->nnrr_lport = 0;
	}
	else {
	    req_rec_ptr->nnrr_result = NETNAME_SUCCESS;
	    req_rec_ptr->nnrr_lport = port_rec_ptr->portrec_local_port;
#if	NETPORT
	    ipc_netport_enter(port_rec_ptr->portrec_network_port,
			port_rec_ptr->portrec_local_port,
			(port_rec_ptr->portrec_network_port.np_receiver == my_host_id));
#endif	NETPORT
	    lk_unlock(&port_rec_ptr->portrec_lock);
	}
    }

    DEBUG2(debug.netname & NN_COND,0,3009,&req_rec_ptr->nnrr_condition,
				req_rec_ptr->nnrr_condition.queue.head);
    condition_signal(&req_rec_ptr->nnrr_condition);
    mutex_unlock(&req_rec_ptr->nnrr_lock);
    DEBUG2(debug.netname & NN_COND,0,3010,&req_rec_ptr->nnrr_condition,
				req_rec_ptr->nnrr_condition.queue.head);
    RETURN(DISP_SUCCESS);

END



/*
 * nn_handle_request
 *	Handles and replies to a name look request.
 *	It is called by the srr transport protocol.
 *
 * Parameters:
 *	request		: the incoming checkup request
 *	from		: the sender of the request (ignored)
 *	broadcast	: whether the request was broadcast
 *	crypt_level	: ignored
 *
 * Design:
 *	Looks to see if the name can be found in our local name table.
 *	If it can be found,
 *		call pr_ltran and place the corresponding local port into the request.
 *	If it cannot be found and the request was broadcast
 *	then return DISP_FAILURE else place the null network port into the request.
 *
 */
/* ARGSUSED */
PUBLIC int nn_handle_request(request, from, broadcast, crypt_level)
sbuf_ptr_t	request;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("nn_handle_request")
    int			hash_index;
    nn_entry_ptr_t	name_entry_ptr;
    netname_name_t	port_name;
    nn_req_ptr_t	req_ptr;
    port_rec_ptr_t	port_rec_ptr;

    INCSTAT(nn_requests_rcvd);
    SBUF_GET_SEG(*request, req_ptr, nn_req_ptr_t);

    /*
     * Set src_format to be our own source format.
     */
    req_ptr->nnr_disp_hdr.src_format = conf_own_format;

    (void)strcpy(port_name, req_ptr->nnr_name);
    NN_NAME_HASH(hash_index, port_name);
    name_entry_ptr = (nn_entry_ptr_t)lq_find_in_queue(&nn_table[hash_index], nn_name_test, (int)port_name);
    if (name_entry_ptr == (nn_entry_ptr_t)0) {
	if (broadcast) {
	    RETURN(DISP_IGNORE);
	}
	else {
	    req_ptr->nnr_nport = null_network_port;
	    RETURN(DISP_SUCCESS);
	}
    }
    else {
	if ((port_rec_ptr = pr_ltran(name_entry_ptr->nne_port)) == PORT_REC_NULL) {
	    ERROR((msg, "nn_handle_request.pr_lportlookup fails, port = %d.\n", name_entry_ptr->nne_port));
	    RETURN(DISP_FAILURE);
	}
	/* port_rec_ptr LOCK RW/R */
	req_ptr->nnr_nport = port_rec_ptr->portrec_network_port;
	lk_unlock(&port_rec_ptr->portrec_lock);
	RETURN(DISP_SUCCESS);
    }
END



/*
 * nn_network_look_up
 *	Perform a network name look up.
 *
 * Parameters:
 *	host_id		: the host to which the request should be directed
 *	port_name	: the name to be looked up
 *	port_ptr	: returns the local port found
 *
 * Results:
 *	NETNAME_SUCCESS
 *	NETNAME_NOT_CHECKED_IN
 *	NETNAME_HOST_NOT_FOUND
 *
 * Design:
 *	Construct a name request and send if off using the SRR transport protocol.
 *	Wait for the request to be answered (request record becomes unlocked).
 *
 */
PUBLIC nn_network_look_up(host_id, port_name, port_ptr)
netaddr_t	host_id;
netname_name_t	port_name;
port_t		*port_ptr;
BEGIN("nn_network_look_up")
    nn_req_rec_ptr_t	req_rec_ptr;
    nn_req_ptr_t	req_ptr;
    sbuf_t		req_buf;
    sbuf_seg_t		req_buf_seg;
    int			rc;

    DEBUG1(debug.netname,0,3000,host_id);
    DEBUG_STRING(debug.netname,0,3001,port_name);
    MEM_ALLOCOBJ(req_rec_ptr,nn_req_rec_ptr_t,MEM_NNREC);
    condition_init(&req_rec_ptr->nnrr_condition);
    mutex_init(&req_rec_ptr->nnrr_lock);
    mutex_lock(&req_rec_ptr->nnrr_lock);

    MEM_ALLOCOBJ(req_ptr,nn_req_ptr_t,MEM_TRBUFF);
    req_ptr->nnr_disp_hdr.src_format = conf_own_format;
    req_ptr->nnr_disp_hdr.disp_type = htons(DISP_NETNAME);
    (void)strcpy(req_ptr->nnr_name, port_name);
    req_rec_ptr->nnrr_result = NETNAME_PENDING;

    SBUF_SEG_INIT(req_buf, &req_buf_seg);
    SBUF_APPEND(req_buf, req_ptr, sizeof(nn_req_t));

    DEBUG1(debug.netname,0,3005, req_rec_ptr);
    mutex_unlock(&req_rec_ptr->nnrr_lock);
    if ((rc = transport_switch[TR_SRR_ENTRY].send((int)req_rec_ptr, (sbuf_ptr_t)&req_buf, host_id,
			TRSERV_NORMAL, CRYPT_DONT_ENCRYPT, nn_cleanup)) != TR_SUCCESS)
    {
	ERROR((msg, "nn_network_look_up.srr_send fails, rc = %d.\n", rc));
	*port_ptr = 0;
	rc = NETNAME_HOST_NOT_FOUND;
    }
    else {
	INCSTAT(nn_requests_sent);
	/*
	 * Wait for the request record to become unlocked.
	 */
#if	LOCK_THREADS
	mutex_unlock(thread_lock);
#endif	LOCK_THREADS
	mutex_lock(&req_rec_ptr->nnrr_lock);
	while (req_rec_ptr->nnrr_result == NETNAME_PENDING) {
		DEBUG2(debug.netname & NN_COND,0,3011,&req_rec_ptr->nnrr_condition,
					req_rec_ptr->nnrr_condition.queue.head);
		condition_wait(&req_rec_ptr->nnrr_condition, 
							&req_rec_ptr->nnrr_lock);
		DEBUG2(debug.netname & NN_COND,0,3012,&req_rec_ptr->nnrr_condition,
					req_rec_ptr->nnrr_condition.queue.head);
	}
	DEBUG0(debug.netname,0,3006);
#if	LOCK_THREADS
	mutex_lock(thread_lock);
#endif	LOCK_THREADS
	mutex_unlock(&req_rec_ptr->nnrr_lock);
	*port_ptr = req_rec_ptr->nnrr_lport;
	rc = req_rec_ptr->nnrr_result;
    }

    mutex_clear(&req_rec_ptr->nnrr_lock);
    condition_clear(&req_rec_ptr->nnrr_condition);
    MEM_DEALLOCOBJ(req_rec_ptr, MEM_NNREC);
    MEM_DEALLOCOBJ(req_ptr, MEM_TRBUFF);

    DEBUG1(debug.netname,0,3002,rc);
    RETURN(rc);

END
