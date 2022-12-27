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
 * $Log:	deltat_send.c,v $
 * Revision 1.1  90/02/19  21:50:34  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.20  89/05/02  11:08:16  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.19  89/04/24  20:38:31  dpj
 * 	Dummy checkin to get the logs right
 * 	[89/04/19  23:42:10  dpj]
 * 
 * Revision 1.18  88/10/08  22:22:32  dpj
 * 	Added USE_DELTAT.
 * 
 * Revision 1.17.2.1  89/04/19  17:52:01  dpj
 * 	Changes from NeXT as of 88/09/30
 * 
 *  4-Sep-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added USE_DELTAT.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  3-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for debugging control under logstat.
 *
 * 13-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Set packet header explicitly in deltat_send_crypt_failure.
 *	Removed data_size parameter from deltat_send_crypt_failure.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed parameters to deltat_send_ack.
 *	Use timer_restart instead of timer_stop/timer_start.
 *	There is now a statically allocated packet within a deltat event.
 *	Added some statistics gathering.
 *
 * 25-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Timer in deltat event record is now inline.
 *	Replaced printf by ERROR and DEBUG/LOG macros.
 *
 * 20-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed for new definition of timer_stop.
 *
 * 20-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * deltat_send.c
 *
 *
 */
#ifndef	lint
char deltat_send_rcsid[] = "$ Header: deltat_send.c,v 1.20 89/05/02 11:08:16 dpj Exp $";
#endif not lint
/*
 * Functions that send various packets over the network
 * as part of the Delta-T transport protocol.
 */



#include	"netmsg.h"
#include	"nm_defs.h"

#if	USE_DELTAT

#define DELTAT_SEND_DEBUG	(debug.deltat & 0x4)

#include <mach.h>

#include "crypt.h"
#include "debug.h"
#include "deltat.h"
#include "deltat_defs.h"
#include "mem.h"
#include "netipc.h"
#include "netmsg.h"
#include "network.h"
#include "nm_defs.h"
#include "nm_extra.h"
#include "sbuf.h"


/*
 * deltat_send_abort
 *	Sends an abort packet to the remote network server in order to abort an event.
 *
 * Parameters:
 *	event_ptr	: the event to be aborted.
 *
 * Note:
 *	Use the packet in the event if it exists to send the abort.
 *	Assumes event is valid (and locked).
 *
 */
PUBLIC void deltat_send_abort(event_ptr)
register deltat_event_ptr_t	event_ptr;
BEGIN("deltat_send_abort")
    register deltat_pkt_ptr_t	pkt_ptr = &event_ptr->dte_packet;
    kern_return_t		kr;

    if (pkt_ptr == DELTAT_NULL_PACKET) {
	MEM_ALLOCOBJ(pkt_ptr,deltat_pkt_ptr_t,MEM_TRBUFF);
    }

    /*
     * Fill in the packet.
     */
    pkt_ptr->deltat_pkt_type = DELTAT_ABORT;
    pkt_ptr->deltat_pkt_uid = event_ptr->dte_uid;
    HTON_DELTAT_HEADER(pkt_ptr);
    DELTAT_SET_PKT_HEADER(pkt_ptr, 0, event_ptr->dte_host_id, event_ptr->dte_crypt_level);

    /*
     * Check to see if we should encrypt this packet.
     */
    if ((event_ptr->dte_crypt_level == CRYPT_DONT_ENCRYPT)
	|| ((crypt_encrypt_packet((netipc_ptr_t)pkt_ptr, event_ptr->dte_crypt_level)) == CRYPT_SUCCESS))
    {
	/*
	 * Actually try to send this packet.
	 */
	if ((kr = netipc_send((netipc_header_ptr_t)pkt_ptr)) != SEND_SUCCESS) {
	    ERROR((msg, "deltat_send_abort.netipc_send fails, kr = %d.", kr));
	}
	else INCSTAT(deltat_aborts_sent);
    }

    RET;

END



/*
 * deltat_send_ack
 *	Sends out an acknowledgement packet
 *
 * Parameters:
 *	event_ptr	: pointer to the event
 *	ack_uid		: the deltat uid to be used.
 *	ack_seq_no	: the deltat packet sequence number to be used.
 *	crypt_level	: the crypt_level to be used
 *	msg_status	: the status of the overall message
 *
 * Side effects:
 *	Sends a packet out over the network.
 *
 * Note:
 *	We send the ack using the packet contained in the event.
 *
 */
PUBLIC void deltat_send_ack(event_ptr, ack_uid, ack_seq_no, crypt_level, msg_status)
register deltat_event_ptr_t	event_ptr;
deltat_uid_t			ack_uid;
unsigned short			ack_seq_no;
int				crypt_level;
int				msg_status;
BEGIN("deltat_send_ack")
    kern_return_t		kr;
    register deltat_pkt_ptr_t	ack_pkt_ptr = &event_ptr->dte_packet;

    DEBUG3(DELTAT_SEND_DEBUG, 2, 1099, ack_uid.deltat_uid_incarnation,
			ack_uid.deltat_uid_sequence_no, ack_seq_no);
    /*
     * Set the Delta-t packet header.
     */
    ack_pkt_ptr->deltat_pkt_type = DELTAT_ACK;
    ack_pkt_ptr->deltat_pkt_msg_type = (char)msg_status;
    ack_pkt_ptr->deltat_pkt_uid = ack_uid;
    ack_pkt_ptr->deltat_pkt_seq_no = ack_seq_no;
    HTON_DELTAT_HEADER(ack_pkt_ptr);

    /*
     * Set the network packet header.
     */
    DELTAT_SET_PKT_HEADER(ack_pkt_ptr, 0, event_ptr->dte_host_id, crypt_level);

    /*
     * Check to see if we should encrypt this packet.
     */
    if ((crypt_level == CRYPT_DONT_ENCRYPT) ||
	((crypt_encrypt_packet((netipc_ptr_t)ack_pkt_ptr, crypt_level)) == CRYPT_SUCCESS))
    {
	/*
	 * Actually try to send this packet.
	 */
	if ((kr = netipc_send((netipc_header_ptr_t)ack_pkt_ptr)) != SEND_SUCCESS) {
	    ERROR((msg, "deltat_send_ack.netipc_send fails, kr = %d.", kr));
	}
	else INCSTAT(deltat_acks_sent);
    }

    RET;

END



/*
 * deltat_send_crypt_failure
 *	Send out a packet to inform a source host that we were unable to decrypt a packet.
 *
 * Parameters:
 *	in_packet_ptr	: the packet that caused the decryption failure
 *	crypt_level	: the encryption level of that packet
 *
 * Side effects:
 *	Send out a packet.
 *
 * Note:
 *	We use the in_packet_ptr to send out the decryption failure packet.
 *	We neither decrypt or encrypt it, instead just send all the data back.
 *	However we negate the crypt_level to inform the source of the failure.
 *
 */
PUBLIC void deltat_send_crypt_failure(in_packet_ptr, crypt_level)
register deltat_pkt_ptr_t	in_packet_ptr;
int				crypt_level;
BEGIN("deltat_send_crypt_failure")
    kern_return_t		kr;

    if (in_packet_ptr == DELTAT_NULL_PACKET) RET;

    in_packet_ptr->deltat_pkt_header.nih_msg_header.msg_local_port = PORT_NULL;
    in_packet_ptr->deltat_pkt_header.nih_msg_header.msg_remote_port = task_self();

    in_packet_ptr->deltat_pkt_header.nih_ip_header.ip_dst.s_addr =
				in_packet_ptr->deltat_pkt_header.nih_ip_header.ip_src.s_addr;
    in_packet_ptr->deltat_pkt_header.nih_ip_header.ip_src.s_addr = my_host_id;

    in_packet_ptr->deltat_pkt_header.nih_udp_header.uh_sum = 0;
    in_packet_ptr->deltat_pkt_header.nih_crypt_header.ch_crypt_level = htonl((unsigned long)-crypt_level);

    if ((kr = netipc_send((netipc_header_ptr_t)in_packet_ptr)) != SEND_SUCCESS) {
	ERROR((msg, "srr_send_crypt_failure.netipc_send fails, kr = %d.", kr));
    }
    else INCSTAT(deltat_cfailures_sent);

    RET;

END



/*
 * deltat_send_next_packet
 *
 * Parameters:
 *	event_ptr	: pointer to the event for which a new
 *			  packet should be constructed and sent
 *
 * Results:
 *	A result code (TR_SUCCESS, TR_SEND_FAILURE or TR_CRYPT_FAILURE).
 *
 * Side effects:
 *	Sets a timer for the retransmission of this packet.
 *
 * Design:
 *	Reset the packet header.
 *	Copy up to DELTAT_MAX_DATA from the sbuf into the packet.
 *	Send the packet.
 *	Call the timer module to schedule retransmission of this packet.
 *	
 *
 * Note:
 *	Assumes that the event is already locked.
 *
 */
PUBLIC deltat_send_next_packet(event_ptr)
register deltat_event_ptr_t	event_ptr;
BEGIN("deltat_send_next_packet")
    kern_return_t		kr;
    register deltat_pkt_ptr_t	pkt_ptr = &event_ptr->dte_packet;
    int				size;
    register pointer_t		temp_ptr;

    if (pkt_ptr == DELTAT_NULL_PACKET) {
	LOG0(TRUE, 3, 1070);
	RETURN(TR_SEND_FAILURE);
    }

    if (event_ptr->dte_status != DELTAT_SENDING) {
	LOG1(TRUE, 3, 1071, event_ptr->dte_status);
	RETURN(TR_SEND_FAILURE);
    }

    /*
     * Set the Delta-t packet header.
     */
    pkt_ptr->deltat_pkt_type = DELTAT_DATA;
    pkt_ptr->deltat_pkt_uid = event_ptr->dte_uid;
    pkt_ptr->deltat_pkt_seq_no = ++event_ptr->dte_pkt_seq_no;
    DEBUG3(DELTAT_SEND_DEBUG, 2, 1096, pkt_ptr->deltat_pkt_uid.deltat_uid_incarnation,
		pkt_ptr->deltat_pkt_uid.deltat_uid_sequence_no, pkt_ptr->deltat_pkt_seq_no);
    HTON_DELTAT_HEADER(pkt_ptr);

    /*
     * Copy some data from the sbuf into the packet.
     */
    size = DELTAT_MAX_DATA_SIZE;
    temp_ptr = (pointer_t)&pkt_ptr->deltat_pkt_data[0];
    SBUF_SAFE_EXTRACT(event_ptr->dte_data, event_ptr->dte_data_pos, temp_ptr, size);
    pkt_ptr->deltat_pkt_msg_type = (SBUF_POS_AT_END(event_ptr->dte_data, event_ptr->dte_data_pos)
					? DELTAT_MSG_END : DELTAT_MSG_MIDDLE);

    /*
     * Set the network packet header.
     */
    DELTAT_SET_PKT_HEADER(pkt_ptr, size, event_ptr->dte_host_id, event_ptr->dte_crypt_level);

    /*
     * Check to see if we should encrypt this packet.
     */
    if (event_ptr->dte_crypt_level != CRYPT_DONT_ENCRYPT) {
	kr = crypt_encrypt_packet((netipc_ptr_t)pkt_ptr, event_ptr->dte_crypt_level);
    }
    else kr = CRYPT_SUCCESS;

    if (kr == CRYPT_SUCCESS) {
	/*
	 * Actually try to send this packet.
	 */
	if ((kr = netipc_send((netipc_header_ptr_t)pkt_ptr)) != SEND_SUCCESS) {
	    ERROR((msg, "deltat_send_next_packet.netipc_send fails, kr = %d.", kr));
	    RETURN(TR_SEND_FAILURE);
	}
	else INCSTAT(deltat_dpkts_sent);
	event_ptr->dte_tries = 1;
	event_ptr->dte_status = (SBUF_POS_AT_END(event_ptr->dte_data, event_ptr->dte_data_pos)
					 ? DELTAT_WAITING : DELTAT_SENDING);
    }
    else {
	RETURN(TR_CRYPT_FAILURE);
    }

    /*
     * Queue this event up with the timer service.
     */
    if (event_ptr->dte_pkt_seq_no == 1) timer_start(&event_ptr->dte_timer);
    else timer_restart(&event_ptr->dte_timer);

    RETURN(TR_SUCCESS);

END


#else	USE_DELTAT
	/*
	 * Just a dummy to keep the loader happy.
	 */
static int	dummy;
#endif	USE_DELTAT

