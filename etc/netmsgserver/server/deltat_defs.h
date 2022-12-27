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
 * $Log:	deltat_defs.h,v $
 * Revision 1.1  90/02/19  21:50:25  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.14  89/05/02  11:08:01  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  1-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new request-reply transport interface. (RPCMOD)
 *
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified to use lock_queue macros.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased the size of the deltat_pkt_data by 8 bytes to allow for crypt padding.
 *	ip_id in packet header is set in netipc_send.
 *	Replaced deltat_make_recv_event by deltat_get_recv_event.
 *	Added to a deltat event a pointer to the queue on which the event is placed.
 *
 * 27-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added external definition of deltat_recv_queue and added deltat_recv_event
 *	field to deltat_event structure to place an event on this queue.
 *	Allow for one inline sbuf segment in a deltat event.
 *	Merged in external definitions from deltat_handler.h
 *	Include a statically allocated packet within a deltat event.
 *
 * 18-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made some fields in deltat_pkt_t and deltat_uid_t unsigned quantities.
 *
 * 25-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made lock and timer inline in deltat event record.
 *
 * 13-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * deltat_defs.h
 *
 *
 * $ Header: deltat_defs.h,v 1.14 89/05/02 11:08:01 dpj Exp $
 *
 */

/*
 * Internal definitions for the delta-t transport protocol.
 */


#ifndef	_DELTAT_DEFS_
#define	_DELTAT_DEFS_

/*
 * Definitions for packets sent out over the network.
 */
#include "netipc.h"
#include "mem.h"

typedef struct deltat_uid_ {
    unsigned long		deltat_uid_incarnation;
    unsigned long		deltat_uid_sequence_no;
} deltat_uid_t;

#define HTON_DELTAT_UID(uid) {							\
    (uid).deltat_uid_incarnation = htonl((uid).deltat_uid_incarnation);		\
    (uid).deltat_uid_sequence_no = htonl((uid).deltat_uid_sequence_no);		\
}

#define NTOH_DELTAT_UID(uid) {							\
    (uid).deltat_uid_incarnation = ntohl((uid).deltat_uid_incarnation);		\
    (uid).deltat_uid_sequence_no = ntohl((uid).deltat_uid_sequence_no);		\
}


#define DELTAT_HEADER_SIZE	((4 * sizeof(short)) + (2 * sizeof(deltat_uid_t)))
#define DELTAT_MAX_DATA_SIZE	(NETIPC_MAX_DATA_SIZE - DELTAT_HEADER_SIZE)

typedef struct deltat_packet_ {
    netipc_header_t	deltat_pkt_header;
    char		deltat_pkt_format;
    char		deltat_pkt_type;
    char		deltat_pkt_msg_type;	/* The status of the higher-level messages. */
    char		deltat_pkt_spare;
    deltat_uid_t	deltat_pkt_uid;
    unsigned short	deltat_pkt_seq_no;
    unsigned short	deltat_pkt_pb_seq_no;	/* Piggy Backed Ack */
    deltat_uid_t	deltat_pkt_pb_uid;	/* Piggy Backed Ack */
    char		deltat_pkt_data[DELTAT_MAX_DATA_SIZE+8];
} deltat_pkt_t, *deltat_pkt_ptr_t;

#define NTOH_DELTAT_HEADER(pkt_ptr) {						\
    NTOH_DELTAT_UID((pkt_ptr)->deltat_pkt_uid);					\
    NTOH_DELTAT_UID((pkt_ptr)->deltat_pkt_pb_uid);				\
    (pkt_ptr)->deltat_pkt_seq_no = ntohs((pkt_ptr)->deltat_pkt_seq_no);		\
    (pkt_ptr)->deltat_pkt_pb_seq_no = ntohs((pkt_ptr)->deltat_pkt_pb_seq_no);	\
}

#define HTON_DELTAT_HEADER(pkt_ptr) {						\
    HTON_DELTAT_UID((pkt_ptr)->deltat_pkt_uid);					\
    HTON_DELTAT_UID((pkt_ptr)->deltat_pkt_pb_uid);				\
    (pkt_ptr)->deltat_pkt_seq_no = htons((pkt_ptr)->deltat_pkt_seq_no);		\
    (pkt_ptr)->deltat_pkt_pb_seq_no = htons((pkt_ptr)->deltat_pkt_pb_seq_no);	\
}
    

#define DELTAT_NULL_PACKET	((deltat_pkt_ptr_t)0)

/*
 * Delta-T Packet Types
 */
#define DELTAT_DATA		1
#define DELTAT_ACK		2
#define DELTAT_CRYPT_FAILURE	3
#define DELTAT_ABORT		4

/*
 * Delta-T Messages Types
 */
#define DELTAT_MSG_NULL		0
#define DELTAT_MSG_END		1
#define DELTAT_MSG_MIDDLE	2

/*
 * Template for deltat-t packet header.
 */
extern netipc_header_t		deltat_template;

/*
 * Macro for filling in an delta-t packet header.
 */

#define DELTAT_SET_PKT_HEADER(pkt_ptr,size,dest,crypt_level) {				\
    (pkt_ptr)->deltat_pkt_header = deltat_template;					\
    (pkt_ptr)->deltat_pkt_header.nih_msg_header.msg_size += (size);			\
    (pkt_ptr)->deltat_pkt_header.nih_ip_header.ip_len += (size);			\
    (pkt_ptr)->deltat_pkt_header.nih_ip_header.ip_dst.s_addr = (dest);			\
    (pkt_ptr)->deltat_pkt_header.nih_udp_header.uh_ulen = 				\
	htons((unsigned short)(size) + DELTAT_HEADER_SIZE + NETIPC_SWAPPED_HEADER_SIZE);\
    (pkt_ptr)->deltat_pkt_header.nih_crypt_header.ch_crypt_level =			\
	htonl((unsigned long)crypt_level);						\
    (pkt_ptr)->deltat_pkt_header.nih_crypt_header.ch_data_size =			\
	htons((unsigned short)(size) + DELTAT_HEADER_SIZE);				\
}




/*
 * Definitions of events which maintain the status of transmissions in progress.
 */
#include <cthreads.h>

#include "lock_queue.h"
#include "lock_queue_macros.h"
#include "nm_defs.h"
#include "sbuf.h"
#include "timer.h"

/*
 * Structure used to place event on the deltat_recv_queue.
 */
typedef struct deltat_recv_event_ {
    struct deltat_recv_event_	*link;
    struct deltat_event_	*dtre_event_ptr;
    boolean_t			dtre_active;
} deltat_recv_event_t, *deltat_recv_event_ptr_t;
    

/*
 * Event holding information when sending data to a remote host.
 */
typedef struct deltat_event_ {
    struct deltat_event_	*link;
    struct mutex		dte_lock;
    lock_queue_t		dte_event_q;
    netaddr_t			dte_host_id;
    deltat_uid_t		dte_uid;
    short			dte_pkt_seq_no;
    short			dte_status;
    int				dte_crypt_level;
    sbuf_t			dte_data;
    sbuf_pos_t			dte_data_pos;
    struct timer		dte_timer;	/* Only on the sending side. */
    int				dte_tries;	/* Only on the sending side. */
    int				dte_client_id;	/* Only on the sending side. */
#if	RPCMOD
    int				(*dte_reply)();	/* Only on the sending side. */
#else	RPCMOD
    int				(*dte_cleanup)();/* Only on the sending side. */
    int				dte_service;	/* Only on the sending side. */
#endif	RPCMOD
    pointer_t			dte_in_packets;	/* Only on the receiving side. */
    deltat_recv_event_t		dte_recv_event;	/* Only on the receiving side. */
    sbuf_seg_t			dte_seg;	/* Only on the receiving side. */
    deltat_pkt_t		dte_packet;
} deltat_event_t, *deltat_event_ptr_t;

#define DELTAT_NULL_EVENT	((deltat_event_ptr_t)0)

/*
 * Event status
 */
#define DELTAT_INACTIVE		1
#define DELTAT_SENDING		2
#define DELTAT_COMPLETED	3	/* The event can be destroyed. */
#define DELTAT_WAITING		4	/* The last packet of the event has been sent. */
#define DELTAT_RECEIVING	5

/*
 * Hash tables in which events are placed.
 */
#define	DELTAT_SEND_HASH_TABLE_ID	0
#define DELTAT_RECV_HASH_TABLE_ID	1

/*
 * Queue of received events.
 */
extern struct lock_queue	deltat_recv_queue;



/*
 * External definitions of functions provided by deltat_handler.c
 */

extern deltat_pkt_ptr_t deltat_handle_abort_pkt();
/*
deltat_pkt_ptr_t	in_packet_ptr;
*/

extern deltat_pkt_ptr_t deltat_handle_ack_pkt();
/*
deltat_pkt_ptr_t	in_packet_ptr;
*/

extern deltat_pkt_ptr_t deltat_handle_crypt_failure_pkt();
/*
deltat_pkt_ptr_t	in_packet_ptr;
*/

extern deltat_pkt_ptr_t deltat_handle_data_pkt();
/*
deltat_pkt_ptr_t	in_packet_ptr;
int			data_size;
int			crypt_level;
*/


/*
 * External definitions of functions provided by deltat_send.c
 */

extern void deltat_send_abort();
/*
deltat_event_ptr_t	event_ptr;
*/

extern void deltat_send_ack();
/*
deltat_event_ptr_t	event_ptr;
deltat_uid_t		ack_uid;
unsigned short		ack_seq_no;
int			crypt_level;
int			msg_status;
*/

void deltat_send_crypt_failure();
/*
deltat_pkt_ptr_t	in_packet_ptr;
int			data_size;
int			crypt_level;
*/

extern int deltat_send_next_packet();
/*
deltat_event_ptr_t	event_ptr;
*/


/*
 * External definitions of functions provided by deltat_utils.c
 */

extern void deltat_utils_init();

extern deltat_event_ptr_t deltat_event_lookup();
/*
netaddr_t		host_id
deltat_uid_t		event_uid;
int			hash_table_id;
*/

extern void deltat_destroy_recv_event();
/*
deltat_event_ptr_t	event_ptr;
*/

void deltat_destroy_send_event();
/*
deltat_event_ptr_t	event_ptr;
*/

extern deltat_event_ptr_t deltat_get_recv_event();
/*
netaddr_t		host_id;
deltat_uid_t		new_uid;
*/

extern deltat_event_ptr_t deltat_make_send_event();
/*
netaddr_t		host_id;
*/

/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_DTEVENT;

#endif	_DELTAT_DEFS_
