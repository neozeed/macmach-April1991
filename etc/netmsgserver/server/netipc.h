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
 * $Log:	netipc.h,v $
 * Revision 1.1  90/02/19  21:53:25  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.7  89/05/02  11:13:03  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 30-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Increased NETIPC_MAX_MSG_SIZE by 8 to account for rounding up when sending secure packets.
 *
 * 22-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added external definitions of netipc_send and netipc_receive.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * netipc.h
 *
 *
 * $ Header: netipc.h,v 1.7 89/05/02 11:13:03 dpj Exp $
 *
 */

/*
 * Definitions for IPC interface to the network.
 */


#ifndef	_NETIPC_
#define	_NETIPC_

#include <mach/message.h>
#include <sys/types.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include "crypt.h"

typedef struct {
    msg_header_t	nih_msg_header;
    struct ip		nih_ip_header;
    struct udphdr	nih_udp_header;
    crypt_header_t	nih_crypt_header;
} netipc_header_t, *netipc_header_ptr_t;

#define NETIPC_MAX_PACKET_SIZE		(1500)	/* Should be ETHERMTU???? */
#define NETIPC_PACKET_HEADER_SIZE	(sizeof(struct ip) + sizeof(struct udphdr) + CRYPT_HEADER_SIZE)
#define NETIPC_MAX_DATA_SIZE		(NETIPC_MAX_PACKET_SIZE - NETIPC_PACKET_HEADER_SIZE)
#define NETIPC_SWAPPED_HEADER_SIZE	(sizeof(struct udphdr) + CRYPT_HEADER_SIZE)
#define NETIPC_MAX_MSG_SIZE		(NETIPC_MAX_PACKET_SIZE + sizeof(msg_header_t) + 8)

typedef struct {
    netipc_header_t	ni_header;
    char		ni_data[NETIPC_MAX_DATA_SIZE];
} netipc_t, *netipc_ptr_t;

#define NETIPC_MSG_ID	1959

/*
 * Functions exported by netipc.c
 */

extern netipc_receive();
/*
netipc_header_ptr_t	pkt_ptr;
*/


extern netipc_send();
/*
netipc_header_ptr_t	pkt_ptr;
*/

#define NETIPC_BAD_UDP_CHECKSUM	-1

#endif	_NETIPC_
