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
 * $Log:	deltat.c,v $
 * Revision 1.1  90/02/19  21:50:16  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.25  89/05/02  11:07:50  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.24  89/04/24  20:38:12  dpj
 * 	Changes from NeXT as of 88/09/30
 * 
 * 	 13-Oct-87 Mary R. Thompson (mrt) at Carnegie Mellon
 * 	[89/04/19  17:51:34  dpj]
 * 
 * Revision 1.23  88/10/14  16:16:04  mrt
 * Changed queue_item_t to cthread_queue_item_t
 * 
 * Revision 1.22  88/10/08  22:19:05  dpj
 * 	Added USE_DELTAT.
 * 
 *	Changed queue_item_t to cthread_queue_item_t.
 *
 *  4-Sep-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added USE_DELTAT.
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  1-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new request-reply transport interface. (RPCMOD)
 *
 * 13-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Use lq_cond_delete_from_queue instead of lq_find_in_queue/lq_remove_from_queue.
 *	Do not pass data_size to deltat_send_crypt_failure.
 *	Swap the deltat header before calling deltat_handle_crypt_failure.
 *	Do not loop on joint local and remote crypt failure.
 *
 * 30-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added deltat_recv_queue, deltat_process_recv_queue/event and deltat_recv_timer.
 *	These are used to clean-up received events - whether completed or not.
 *	There is now a statically allocated packet within a deltat event.
 *	ip_id in packet header is set in netipc_send.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased the backlog on the deltat_listen_port.
 *	Added some statistics gathering.
 *
 *  5-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Obtain max tries value from param record.
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *	Added call to cthread_set_name.
 *	Lock and timer in deltat event record are now inline.
 *	Replaced printf by ERROR and DEBUG/LOG macros.
 *
 * 18-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed deltat_abort.
 *
 * 18-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * deltat.c
 *
 *
 */
#ifndef	lint
char deltat_rcsid[] = "$ Header: deltat.c,v 1.25 89/05/02 11:07:50 dpj Exp $";
#endif not lint
/*
 * Functions implementing Delta-T transport protocol.
 */


#include	"netmsg.h"
#include	"nm_defs.h"

#if	USE_DELTAT

#define DELTAT_DEBUG	(debug.deltat & 0x1)

#include <mach.h>
#include <mach/message.h>
#include <cthreads.h>

#include "crypt.h"
#include "debug.h"
#include "deltat.h"
#include "deltat_defs.h"
#include "lock_queue.h"
#include "ls_defs.h"
#include "mem.h"
#include "netipc.h"
#include "netmsg.h"
#include "network.h"
#include "nm_defs.h"
#include "nm_extra.h"
#include "sbuf.h"
#include "timer.h"
#include "transport.h"
#include "uid.h"


static port_t			deltat_listen_port;
static cthread_t		deltat_listen_thread;

PUBLIC struct lock_queue	deltat_recv_queue;
PUBLIC struct timer		deltat_recv_timer;

netipc_header_t			deltat_template;

extern deltat_main();
extern deltat_process_recv_queue();

/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_DTEVENT;



/*
 * deltat_init
 *	Initialises the delta-t transport protocol.
 *
 * Parameters:
 *
 * Results:
 *	FALSE : we failed to initialise the delta-t transport protocol.
 *	TRUE  : we were successful.
 *
 * Side effects:
 *	Initialises the delta-t protocol entry point in the switch array.
 *	Initialises the template for sending network messages.
 *	Allocates the listener port and creates a thread to listen to the network.
 *
 */
EXPORT boolean_t deltat_init()
BEGIN("deltat_init")
    kern_return_t	kr;

    /*
     * Initialize the memory management facilities.
     */
    mem_initobj(&MEM_DTEVENT,"Delta-T event",sizeof(deltat_event_t),
								FALSE,35,10);


    deltat_utils_init();

#if	RPCMOD
    transport_switch[TR_DELTAT_ENTRY].sendrequest = deltat_sendrequest;
#else	RPCMOD
    transport_switch[TR_DELTAT_ENTRY].send = deltat_send;
#endif	RPCMOD

    deltat_template.nih_msg_header.msg_simple = TRUE;
    deltat_template.nih_msg_header.msg_type = MSG_TYPE_NORMAL;
    deltat_template.nih_msg_header.msg_size = sizeof(netipc_header_t) + DELTAT_HEADER_SIZE;
    deltat_template.nih_msg_header.msg_id = NETIPC_MSG_ID;
    deltat_template.nih_msg_header.msg_local_port = PORT_NULL;
    deltat_template.nih_msg_header.msg_remote_port = task_self();

    deltat_template.nih_ip_header.ip_hl = sizeof(struct ip) >> 2; /* 32 bit words */
    deltat_template.nih_ip_header.ip_v = IPVERSION;
    deltat_template.nih_ip_header.ip_tos = 0;
    deltat_template.nih_ip_header.ip_len = NETIPC_PACKET_HEADER_SIZE + DELTAT_HEADER_SIZE;
    deltat_template.nih_ip_header.ip_id = 0;
    deltat_template.nih_ip_header.ip_off = 0;
    deltat_template.nih_ip_header.ip_ttl = 30;	/*UDP_TTL*/
    deltat_template.nih_ip_header.ip_p = IPPROTO_UDP;
    deltat_template.nih_ip_header.ip_sum = 0;
    deltat_template.nih_ip_header.ip_src.s_addr = my_host_id;

    deltat_template.nih_udp_header.uh_sport = htons(DELTAT_UDP_PORT);
    deltat_template.nih_udp_header.uh_dport = htons(DELTAT_UDP_PORT);
    deltat_template.nih_udp_header.uh_sum = 0;

    deltat_template.nih_crypt_header.ch_crypt_level = CRYPT_DONT_ENCRYPT;
    deltat_template.nih_crypt_header.ch_checksum = 0;

    /*
     * Initialise the IPC interface to the network.
     */
    if ((kr = port_allocate(task_self(), &deltat_listen_port)) != KERN_SUCCESS) {
	ERROR((msg, "deltat_init.port_allocate fails, kr = %d.", kr));
	RETURN(FALSE);
    }
    if ((kr = port_set_backlog(task_self(), deltat_listen_port, 16)) != KERN_SUCCESS) {
	ERROR((msg, "deltat_init.port_set_backlog fails, kr = %d.", kr));
	RETURN(FALSE);
    }
    if ((kr = netipc_listen(task_self(), 0, 0, 0, (int)(htons(DELTAT_UDP_PORT)),
				IPPROTO_UDP, deltat_listen_port)) != KERN_SUCCESS)
    {
	ERROR((msg, "deltat_init.netipc_listen fails, kr = %d.", kr));
	RETURN(FALSE);
    }

    /*
     * Initialise the receiver queue and timer.
     */
    lq_init(&deltat_recv_queue);
    deltat_recv_timer.interval.tv_sec = param.deltat_msg_life;
    deltat_recv_timer.interval.tv_usec = 0;
    deltat_recv_timer.action = deltat_process_recv_queue;
    deltat_recv_timer.info = (char *)0;
    timer_start(&deltat_recv_timer);

    /*
     * Now fork a thread to execute the receive loop of the deltat transport protocol.
     */
    deltat_listen_thread = cthread_fork((cthread_fn_t)deltat_main, (any_t)0);
    cthread_set_name(deltat_listen_thread, "deltat_main");
    cthread_detach(deltat_listen_thread);

    RETURN(TRUE);

END


/*
 * deltat_main
 *	The main reception loop for the Delta-T protocol.
 *	Allocates a reception buffer and waits for incoming messages.
 *	Calls appropriate packet handler procedure.
 *
 * Parameters:
 *	None
 *
 * Results:
 *	Should never exit.
 *
 * Note:
 *	A new buffer must be allocated
 *	if the handling routine does not return one that can be reused.
 *
 */
PRIVATE deltat_main()
BEGIN("deltat_main")
    kern_return_t		kr;
    register deltat_pkt_ptr_t	in_packet_ptr;
    register deltat_pkt_ptr_t	old_packet_ptr;
    int				crypt_level;
    int				data_size;
    boolean_t			crypt_remote_failure;

#if	LOCK_THREADS
    mutex_lock(thread_lock);
#endif	LOCK_THREADS

    MEM_ALLOCOBJ(in_packet_ptr,deltat_pkt_ptr_t,MEM_TRBUFF);

    while (TRUE) {
	/*
	 * Fill in the message header and try to receive a message.
	 */
	in_packet_ptr->deltat_pkt_header.nih_msg_header.msg_size = NETIPC_MAX_MSG_SIZE;
	in_packet_ptr->deltat_pkt_header.nih_msg_header.msg_local_port = deltat_listen_port;
	kr = netipc_receive((netipc_header_ptr_t)in_packet_ptr);
	if (kr != RCV_SUCCESS) {
	    ERROR((msg, "deltat_main.netipc_receive fails, kr = %d.", kr));
	}
	else {
	    crypt_level = ntohl(in_packet_ptr->deltat_pkt_header.nih_crypt_header.ch_crypt_level);
	    if (crypt_level < 0) {
		/*
		 * This is a remote crypt failure packet.
		 */
		crypt_remote_failure = TRUE;
		crypt_level = - crypt_level;
		in_packet_ptr->deltat_pkt_header.nih_crypt_header.ch_crypt_level =
				htonl((unsigned long)crypt_level);
	    }
	    else crypt_remote_failure = FALSE;
		
	    if (crypt_level != CRYPT_DONT_ENCRYPT) {
		kr = crypt_decrypt_packet((netipc_ptr_t)in_packet_ptr, crypt_level);
	    }
	    else kr = CRYPT_SUCCESS;

	    if (kr == CRYPT_SUCCESS) {
		/*
		 * Swap the deltat header and see what type of packet we got.
		 */
		NTOH_DELTAT_HEADER(in_packet_ptr);
		if (crypt_remote_failure) {
		    /*
		     * This packet was sent by us but could not be decrypted by the remote network server.
		     */
		    LOG0(TRUE, 5, 1093);
		    INCSTAT(deltat_cfailures_rcvd);
		    old_packet_ptr = deltat_handle_crypt_failure_pkt(in_packet_ptr);
		}
		else {
		    old_packet_ptr = DELTAT_NULL_PACKET;
		    data_size = ntohs(in_packet_ptr->deltat_pkt_header.nih_crypt_header.ch_data_size);
		    data_size -= DELTAT_HEADER_SIZE;
		    switch (in_packet_ptr->deltat_pkt_type) {
			case DELTAT_DATA: {
			    old_packet_ptr = deltat_handle_data_pkt(in_packet_ptr, data_size, crypt_level);
			    break;
			}
			case DELTAT_ACK: {
			    INCSTAT(deltat_acks_rcvd);
			    old_packet_ptr = deltat_handle_ack_pkt(in_packet_ptr);
			    break;
			}
			case DELTAT_ABORT: {
			    INCSTAT(deltat_aborts_rcvd);
			    old_packet_ptr = deltat_handle_abort_pkt(in_packet_ptr);
			    break;
			}
			default: {
			    LOG1(TRUE, 5, 1094, in_packet_ptr->deltat_pkt_type);
			    break;
			}
		    }
		}
	    }
	    else {
		LOG0(TRUE, 5, 1095);
		if (!crypt_remote_failure) deltat_send_crypt_failure(in_packet_ptr, crypt_level);
		old_packet_ptr = in_packet_ptr;
	    }

	    if (old_packet_ptr == DELTAT_NULL_PACKET) {
		/*
		 *Allocate a new buffer.
		 */
		MEM_ALLOCOBJ(in_packet_ptr,deltat_pkt_ptr_t,MEM_TRBUFF);
	    }
	    else in_packet_ptr = old_packet_ptr;
    	}
	LOGCHECK;
    }
END


/*
 * deltat_retry
 *	If event is marked as completed then destroys the event
 *	otherwise tries retransmitting a packet over the network for an event.
 *	The event is aborted if the packet has been retransmitted more than DELTAT_MAX_TRIES times.
 *
 * Parameters:
 *	timer	: a pointer to the timer which went off
 *
 * Results:
 *	TR_SUCCESS or TR_FAILURE
 *
 * Side effects:
 *	May send a packet off over the network.
 *	May delete the event and call cleanup to inform the client.
 *	May send an abort event to the remote network server.
 *
 */
EXPORT deltat_retry(timer)
register timer_t		timer;
BEGIN("deltat_retry")
    register deltat_event_ptr_t	event_ptr;
    kern_return_t		kr;

    event_ptr = (deltat_event_ptr_t)timer->info;

    /*
     * Lock the event before proceeding.
     */
    mutex_lock(&event_ptr->dte_lock);

    /*
     * Check to see if we should abort this event or retransmit the packet.
     */
    if (event_ptr->dte_status == DELTAT_COMPLETED) {
	deltat_destroy_send_event(event_ptr);
	RETURN(TR_SUCCESS);
    }

    if (event_ptr->dte_tries > param.deltat_max_tries) {
	/*
	 * Abort the event: call the client cleanup, send an abort packet and destroy the event.
	 */
#if	RPCMOD
	if (event_ptr->dte_reply) {
	    kr = event_ptr->dte_reply(event_ptr->dte_client_id, TR_SEND_FAILURE, NULL);
#else	RPCMOD
	if (event_ptr->dte_cleanup) {
	    kr = event_ptr->dte_cleanup(event_ptr->dte_client_id, TR_SEND_FAILURE);
#endif	RPCMOD
	}
	deltat_send_abort(event_ptr);
	deltat_destroy_send_event(event_ptr);
	RETURN(TR_FAILURE);
    }
    else {
	/*
	 * Send the packet off and reset the timer.
	 */
	if ((kr = netipc_send((netipc_header_ptr_t)&event_ptr->dte_packet)) != SEND_SUCCESS) {
	    ERROR((msg, "deltat_retry.netipc_send fails, kr = %d.", kr));
	}
	else {
	    INCSTAT(deltat_retries_sent);
	}
	event_ptr->dte_tries ++;
	timer_start(&event_ptr->dte_timer);
    }

    mutex_unlock(&event_ptr->dte_lock);

    RETURN(((kr == SEND_SUCCESS) ? TR_SUCCESS : TR_FAILURE));

END


/*
 * deltat_sendrequest
 *	Processes a send request by creating an event for it
 *	and sending off the first packet of the event.
 *
 * Parameters:
 *	client_id	: an identifier assigned by the client to this transaction
 *	data		: the data to be sent
 *	to		: the destination of the request
 *	crypt_level	: whether the data should be encrypted
 *	reply_proc	: a function to be called when this transaction has finished
 *
 * Returns:
 *	TR_SUCCESS or a specific failure code.
 *
 * Side effects:
 *	Creates an event record and queues it in the sending hash table.
 *	Creates the first packet of this event and sends it out over the network.
 *
 * Design:
 *	Construct an event record.
 *	Send the first packet of this event by calling deltat_send_next_packet.
 *
 * Note:
 *	This description applies to the RPCMOD version of this procedure.
 *	See archives for details of the old version.
 */
#if	RPCMOD
EXPORT int deltat_sendrequest(client_id,data,to,crypt_level,reply_proc)
#else	RPCMOD
EXPORT int deltat_send(client_id,data,to,service,crypt_level,cleanup)
#endif	RPCMOD
int		client_id;
sbuf_ptr_t	data;
netaddr_t	to;
#if	RPCMOD
#else	RPCMOD
int		service;
#endif	RPCMOD
int		crypt_level;
#if	RPCMOD
int		(*reply_proc)();
BEGIN("deltat_sendrequest")
#else	RPCMOD
int		(*cleanup)();
BEGIN("deltat_send")
#endif	RPCMOD
    register deltat_event_ptr_t	event_ptr;
    kern_return_t		kr;

    if ((event_ptr = deltat_make_send_event(to)) == DELTAT_NULL_EVENT) {
	ERROR((msg,"deltat_send.deltat_make_send_event fails."));
	RETURN(TR_SEND_FAILURE);
    }

    /*
     * Lock the event just in case.
     */
    mutex_lock(&event_ptr->dte_lock);

    /*
     * Fill in the fields of the event not already initialised.
     */
#if	RPCMOD
    event_ptr->dte_reply = reply_proc;
#else	RPCMOD
    event_ptr->dte_cleanup = cleanup;
#endif	RPCMOD
    event_ptr->dte_client_id = client_id;
    event_ptr->dte_crypt_level = crypt_level;
#if	RPCMOD
#else	RPCMOD
    event_ptr->dte_service = service;
#endif	RPCMOD

    /*
     * And the sbuf.
     */
    event_ptr->dte_data = *data;
    SBUF_SEEK(event_ptr->dte_data, event_ptr->dte_data_pos, 0)

    /*
     * Try sending the first packet.
     */
    if ((kr = deltat_send_next_packet(event_ptr)) != TR_SUCCESS) {
	/*
	 * We can safely destroy this event because we know that it is not on a timer queue.
	 */
	deltat_destroy_send_event(event_ptr);
	RETURN(kr);
    }

    mutex_unlock(&event_ptr->dte_lock);
    RETURN(TR_SUCCESS);
END



/*
 * deltat_check_recv_event
 *
 * Parameters:
 *	recv_event	: the queue item in question.
 *	arg		: ignored.
 *
 * Results:
 *	Whether the event is active or not.
 *
 */
/* ARGSUSED */
PRIVATE deltat_check_recv_event(queue_item, arg)
deltat_recv_event_ptr_t	queue_item;
int			arg;
BEGIN("deltat_check_recv_event")
    register boolean_t	result;

    mutex_lock(&queue_item->dtre_event_ptr->dte_lock);
    result = !(queue_item->dtre_active);
    mutex_unlock(&queue_item->dtre_event_ptr->dte_lock);

    RETURN(result);

END

/*
 * deltat_mark_recv_event
 *	marks an event as inactive.
 *
 * Parameters:
 *	recv_event	: the queue item in question.
 *	arg		: ignored.
 *
 * Results:
 *	Irrelevant.
 *
 */
/* ARGSUSED */
PRIVATE deltat_mark_recv_event(queue_item, arg)
deltat_recv_event_ptr_t	queue_item;
int			arg;
BEGIN("deltat_mark_recv_event")

    mutex_lock(&queue_item->dtre_event_ptr->dte_lock);
    queue_item->dtre_active = FALSE;
    mutex_unlock(&queue_item->dtre_event_ptr->dte_lock);
    RETURN(0);

END


/*
 * deltat_process_recv_queue
 *	Called by the timer routine for the queue of receive events.
 *
 * Parameters:
 *	timer	: the timer that went off
 *
 * Results:
 *	Irrelevant
 *
 * Design:
 *	Delete from the queue and destroy all those events not marked as active.
 *	Mark all events left in the queue as inactive.
 *	Restart the timer.
 *
 * Side effects:
 *	Will destroy some events.
 *
 * Notes:
 *	deltat_handle_data_pkt is responsible for marking an event as active.
 *
 *	Do not destroy the event if it is completed but deltat_cleanup has not yet been called -
 *	deltat_cleanup has been called if the dte_in_packets field has been cleared.
 *
 */
PUBLIC deltat_process_recv_queue(timer)
timer_t		timer;
BEGIN("deltat_recv_retry")
    register deltat_recv_event_ptr_t	recv_event_ptr;
    register deltat_event_ptr_t		event_ptr;

    DEBUG0(DELTAT_DEBUG, 3, 1089);

    while ((recv_event_ptr =
	(deltat_recv_event_ptr_t)lq_cond_delete_from_queue(&deltat_recv_queue, deltat_check_recv_event, 0))
			!= 0)
    {
	event_ptr = recv_event_ptr->dtre_event_ptr;
	mutex_lock(&event_ptr->dte_lock);

	switch (event_ptr->dte_status) {
	    case DELTAT_RECEIVING: {
		register pointer_t	old_pkt_ptr, next_pkt_ptr;
		/*
		 * Deallocate the packets received for this event.
		 */
		old_pkt_ptr = event_ptr->dte_in_packets;
		while (old_pkt_ptr != (pointer_t)0) {
		    /*
		     * Get the address of the next packet to be deleted and then delete the current one.
		     */
		    next_pkt_ptr = *(pointer_t *)old_pkt_ptr;
		    MEM_DEALLOCOBJ(old_pkt_ptr, MEM_TRBUFF);
		    old_pkt_ptr = next_pkt_ptr;
		}
		event_ptr->dte_in_packets = (pointer_t)0;
		LOG1(TRUE, 5, 1090, event_ptr);
		deltat_destroy_recv_event(event_ptr);
		break;
	    }
	    case DELTAT_COMPLETED: {
		if (event_ptr->dte_in_packets != (pointer_t)0) {
		    LOG1(TRUE, 3, 1091, event_ptr);
		    /*
		     * deltat_cleanup not yet called - requeue the event.
		     */
		    event_ptr->dte_recv_event.dtre_active = TRUE;
		    lq_prequeue(&deltat_recv_queue, (cthread_queue_item_t)&event_ptr->dte_recv_event.link);
		    mutex_unlock(&event_ptr->dte_lock);
		}
		else {
		    DEBUG1(DELTAT_DEBUG, 0, 1092, event_ptr);
		    deltat_destroy_recv_event(event_ptr);
		}
		break;
	    }
	    default: {
		ERROR((msg, "deltat_recv_retry: unexpected event status (%d).", event_ptr->dte_status));
		mutex_unlock(&event_ptr->dte_lock);
		deltat_destroy_recv_event(event_ptr);
	    }
	}
    }

    lq_map_queue(&deltat_recv_queue, deltat_mark_recv_event, 0);

    timer_start(timer);
    RET;

END


#else	USE_DELTAT
	/*
	 * Just a dummy to keep the loader happy.
	 */
static int	dummy;
#endif	USE_DELTAT

