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
 * $Log:	datagram.c,v $
 * Revision 1.1  90/02/19  21:49:59  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.17  89/05/02  11:07:33  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.16  89/04/24  20:38:06  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:51:23  dpj]
 * 
 * 05-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Added USE_DATAGRAM
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	ip_id in packet header is set in netipc_send.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased the backlog on the datagram_listen_port.
 *	Added some statistics gathering.
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *	Added call to cthread_set_name.
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *
 *  6-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced SBUF_INIT by SBUF_SEG_INIT.
 *
 * 17-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added datagram_max_data_size; initialised from DATAGRAM_MAX_DATA_SIZE.
 *	Removed datagram_abort.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * datagram.c
 *
 *
 */
#ifndef	lint
char datagram_rcsid[] = "$ Header: datagram.c,v 1.17 89/05/02 11:07:33 dpj Exp $";
#endif not lint

/*
 * Datagram interface to the network.
 */


#include "netmsg.h"
#include "nm_defs.h"

#if	USE_DATAGRAM

#include <mach.h>
#include <mach/message.h>
#include <cthreads.h>

#include "crypt.h"
#include "debug.h"
#include "datagram.h"
#include "disp_hdr.h"
#include "mem.h"
#include "netipc.h"
#include "network.h"
#include "nm_extra.h"
#include "sbuf.h"
#include "transport.h"
#include "uid.h"

static netipc_header_t		datagram_template;
static port_t			datagram_listen_port;
static cthread_t		datagram_listen_thread;
static netipc_ptr_t		datagram_out_message;

extern datagram_main();

int			datagram_max_data_size = NETIPC_MAX_DATA_SIZE;


/*
 * datagram_init
 *	Initialises the datagram transport protocol.
 *
 * Results:
 *	FALSE : we failed to initialise the datagram transport protocol.
 *	TRUE  : we were successful.
 *
 * Side effects:
 *	Initialises the datagram protocol entry point in the switch array.
 *	Initialises the template for sending datagrams.
 *	Allocates the listener port and creates a thread to listen to the network.
 *
 */
EXPORT boolean_t datagram_init()
BEGIN("datagram_init")
    kern_return_t	kr;

    transport_switch[TR_DATAGRAM_ENTRY].send = datagram_send;

    datagram_template.nih_msg_header.msg_simple = TRUE;
    datagram_template.nih_msg_header.msg_type = MSG_TYPE_NORMAL;
    datagram_template.nih_msg_header.msg_size = sizeof(netipc_header_t);
    datagram_template.nih_msg_header.msg_id = NETIPC_MSG_ID;
    datagram_template.nih_msg_header.msg_local_port = PORT_NULL;
    datagram_template.nih_msg_header.msg_remote_port = task_self();

    datagram_template.nih_ip_header.ip_hl = sizeof(struct ip) >> 2; /* 32 bit words */
    datagram_template.nih_ip_header.ip_v = IPVERSION;
    datagram_template.nih_ip_header.ip_tos = 0;
    datagram_template.nih_ip_header.ip_len = NETIPC_PACKET_HEADER_SIZE;
    datagram_template.nih_ip_header.ip_id = 0;
    datagram_template.nih_ip_header.ip_off = 0;
    datagram_template.nih_ip_header.ip_ttl = 30;	/*UDP_TTL*/
    datagram_template.nih_ip_header.ip_p = IPPROTO_UDP;
    datagram_template.nih_ip_header.ip_sum = 0;
    datagram_template.nih_ip_header.ip_src.s_addr = my_host_id;

    datagram_template.nih_udp_header.uh_sport = htons(DATAGRAM_UDP_PORT);
    datagram_template.nih_udp_header.uh_dport = htons(DATAGRAM_UDP_PORT);
    datagram_template.nih_udp_header.uh_sum = 0;

    datagram_template.nih_crypt_header.ch_crypt_level = CRYPT_DONT_ENCRYPT;
    datagram_template.nih_crypt_header.ch_checksum = 0;

    /*
     * Initialise the IPC interface to the network.
     */
    if ((kr = port_allocate(task_self(), &datagram_listen_port)) != KERN_SUCCESS) {
	ERROR((msg, "datagram_init.port_allocate fails, kr = %d.", kr));
	RETURN(FALSE);
    }
    if ((kr = port_set_backlog(task_self(), datagram_listen_port, 16)) != KERN_SUCCESS) {
	ERROR((msg, "deltat_init.port_set_backlog fails, kr = %d.", kr));
	RETURN(FALSE);
    }
    if ((kr = netipc_listen(task_self(), 0, 0, 0, (int)(htons(DATAGRAM_UDP_PORT)),
				IPPROTO_UDP, datagram_listen_port)) != KERN_SUCCESS)
    {
	ERROR((msg, "datagram_init.netipc_listen fails, kr = %d.", kr));
	RETURN(FALSE);
    }

    /*
     * Pre-allocate a buffer for use when sending datagrams.
     */
    MEM_ALLOCOBJ(datagram_out_message,netipc_ptr_t,MEM_TRBUFF);

    /*
     * Now fork a thread to execute the receive loop of the datagram transport protocol.
     */
    datagram_listen_thread = cthread_fork((cthread_fn_t)datagram_main, (any_t)0);
    cthread_set_name(datagram_listen_thread, "datagram_main");
    cthread_detach(datagram_listen_thread);

    RETURN(TRUE);

END


/*
 * datagram_main
 *	Main loop of datagram transport protocol
 *	Waits for incoming packets contained in IPC messages
 *	and calls the dispatcher to handle them.
 *
 * Note:
 *	It is assumed that the data in the incoming packet is no longer
 *	needed by the higher-level routine to which it was given by the
 *	disp_inmsg_simple function after disp_inmsg_simple returns.
 *	In other words we can reuse the buffer for the next datagram.
 */
PRIVATE datagram_main()
BEGIN("datagram_main")
    netipc_ptr_t	in_pkt_ptr;
    kern_return_t	kr;
    sbuf_t		in_sbuf;
    sbuf_seg_t		in_sbuf_seg;

#if	LOCK_THREADS
    mutex_lock(thread_lock);
#endif	LOCK_THREADS

    MEM_ALLOCOBJ(in_pkt_ptr,netipc_ptr_t,MEM_TRBUFF);

    SBUF_SEG_INIT(in_sbuf, &in_sbuf_seg);

    while (TRUE) {
	in_pkt_ptr->ni_header.nih_msg_header.msg_size = NETIPC_MAX_MSG_SIZE;
	in_pkt_ptr->ni_header.nih_msg_header.msg_local_port = datagram_listen_port;
	kr = netipc_receive((netipc_header_ptr_t)in_pkt_ptr);
	if (kr != RCV_SUCCESS) {
	    ERROR((msg, "datagram_main.netipc_receive fails, kr = %d.", kr));
	}
	else {
	    int		data_size;
	    netaddr_t	source_host;
	    boolean_t	broadcast;
	    int		crypt_level;

	    INCSTAT(datagram_pkts_rcvd);
	    source_host = in_pkt_ptr->ni_header.nih_ip_header.ip_src.s_addr;
	    broadcast = (in_pkt_ptr->ni_header.nih_ip_header.ip_dst.s_addr == broadcast_address);
	    crypt_level = ntohl(in_pkt_ptr->ni_header.nih_crypt_header.ch_crypt_level);

	    if (crypt_level != CRYPT_DONT_ENCRYPT) {
		kr = crypt_decrypt_packet(in_pkt_ptr, crypt_level);
	    }
	    else kr = CRYPT_SUCCESS;

	    if (kr == CRYPT_SUCCESS) {
		data_size = ntohs(in_pkt_ptr->ni_header.nih_crypt_header.ch_data_size);
		SBUF_REINIT(in_sbuf);
		SBUF_APPEND(in_sbuf, in_pkt_ptr->ni_data, data_size);
		kr = disp_indata_simple(0, (sbuf_ptr_t)&(in_sbuf), source_host, broadcast, crypt_level);
		if (kr != DISP_SUCCESS) {
		    LOG1(TRUE, 5, 1101, kr);
		}
	    }
	}
	LOGCHECK;
    }
END


/*
 * datagram_send
 *	Sends a datagram over the network.
 *
 * Parameters:
 *	to		: the host to which the datagram is to be sent.
 *	crypt_level	: whether this packet should be encrypted.
 *
 * Returns:
 *	TR_SUCCESS or a specific failure code.
 *
 * Note:
 *	cleanup is never called as this routine is guaranteed to have done
 *	with the input sbuf after it returns.
 *
 */
/* ARGSUSED */
EXPORT int datagram_send(client_id, data, to, service, crypt_level, cleanup)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	to;
int		service;
int		crypt_level;
int		(*cleanup)();
BEGIN("datagram_send")
    kern_return_t	kr;
    unsigned short	size;

    /*
     * Copy the input sbuf into our local buffer (datagram_out_message).
     */
    SBUF_FLATTEN(data, datagram_out_message->ni_data, size);

    /*
     * Sanity check.
     */
    if (size > NETIPC_MAX_DATA_SIZE) {
	LOG1(TRUE, 5, 1100, size);
	RETURN(DATAGRAM_TOO_LARGE);
    }

    /*
     * Fill in the netipc header.
     */
    datagram_out_message->ni_header = datagram_template;
    datagram_out_message->ni_header.nih_msg_header.msg_size += size;
    datagram_out_message->ni_header.nih_ip_header.ip_len += size;
    datagram_out_message->ni_header.nih_ip_header.ip_dst.s_addr = to;
    datagram_out_message->ni_header.nih_udp_header.uh_ulen = htons(size + NETIPC_SWAPPED_HEADER_SIZE);
    datagram_out_message->ni_header.nih_crypt_header.ch_crypt_level = htonl((unsigned long)crypt_level);
    datagram_out_message->ni_header.nih_crypt_header.ch_data_size = htons(size);

    /*
     * Check to see whether we should encrypt this datagram.
     */
    if (crypt_level != CRYPT_DONT_ENCRYPT) {
	kr = crypt_encrypt_packet(datagram_out_message, crypt_level);
    }
    else kr = CRYPT_SUCCESS;

    if (kr == CRYPT_SUCCESS) {
	/*
	 * Send the message to the kernel.
	 */
	if ((kr = netipc_send((netipc_header_ptr_t)datagram_out_message)) != SEND_SUCCESS) {
	    ERROR((msg, "datagram_send.netipc_send fails, kr = %1d.", kr));
	    RETURN(TR_SEND_FAILURE);
	}
	else INCSTAT(datagram_pkts_sent);
    }
    else RETURN(TR_CRYPT_FAILURE);

    RETURN(TR_SUCCESS);
END

#else	USE_DATAGRAM
	/*
	 * Just a dummy to keep the loader happy.
	 */
static int	dummy;
#endif	USE_DATAGRAM
