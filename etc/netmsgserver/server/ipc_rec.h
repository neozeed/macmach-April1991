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
 * $Log:	ipc_rec.h,v $
 * Revision 1.1  90/02/19  21:51:40  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.14  89/05/02  11:09:58  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 14-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new ipc_rec states and outgoing queue.
 *
 *  3-Sep-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for RPCMOD. Fixed the IPC return codes.
 *
 *  3-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Statically allocate some sbuf segments in an ipc_outrec for use in
 *	simple messages and for messages with one piece of out-of-line data.
 *
 * 29-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added booleans to an ipc_outrec to say whether there is
 *	out-of-line data or a NPD to be garbage collected.
 *
 *  2-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added crypt_level field to an out record.
 *	Added extern definitions internal to the IPC module.
 *
 * 13-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Defined the return codes for IPC operations.
 *
 *  9-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Defined ipc_iorec_t, and modified a few fields in the other
 *	record types for request-response support.
 *
 * 15-Nov-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 *
 */
/*
 * ipc_rec.h 
 *
 *
 * $ Header: ipc_rec.h,v 1.14 89/05/02 11:09:58 dpj Exp $ 
 *
 */

/*
 * Definitions for records used by the IPC module to register 
 * messages in transit. 
 */


#ifndef	_IPC_REC_
#define	_IPC_REC_

#include <mach/boolean.h>
#include <mach/message.h>

#if	RPCMOD
#include	"sys_queue.h"
#endif	RPCMOD

#include	"ipc_hdr.h"
#include	"sbuf.h"
#include	"mem.h"

#define IPC_OUT_NUM_SEGS	5


typedef struct {
#if	RPCMOD
#else	RPCMOD
	pointer_t	re_send_q;	/* link in queue for ip re-send thread */
	port_rec_ptr_t	dest_port_ptr;	/* port record for destination */
#endif	RPCMOD
	msg_header_t	*ipcbuff;	/* IPC receive buffer */
	sbuf_t		msg;		/* complete msg to transmit */
	sbuf_seg_t	segs[IPC_OUT_NUM_SEGS];	/* statically alloc'd sbuf segs. */
	boolean_t	ool_exists;	/* is there out-of-line data */
	sbuf_t		ool;		/* list of out-of-line sections (for GC) */
	sbuf_seg_t	ool_seg;	/* optimisation for message with 1 ool item */
	boolean_t	npd_exists;	/* is there a NPD */
	sbuf_t		npd;		/* Network Port Dictionary (for GC) */
	ipc_netmsg_hdr_t netmsg_hdr;	/* IPC netmsg header */
	netaddr_t	dest;		/* destination address (machine) */
	int		crypt_level;	/* The encryption level of the message. */
} ipc_outrec_t;

#if	RPCMOD
#else	RPCMOD
typedef ipc_outrec_t *ipc_outrec_ptr_t;
#endif	RPCMOD

typedef struct {
#if	RPCMOD
#else	RPCMOD
	port_rec_ptr_t	dest_port_ptr;	/* port record for destination */
#endif	RPCMOD
	sbuf_ptr_t	msg;		/* data from the transport level */
	msg_header_t	*assem_buff;	/* buffer for assembly */
	int		assem_len;	/* length of assem_buff to dealloc */
			/* or 0 if MEM_ASSEMBUF, -1 if no dealloc needed */
#if	RPCMOD
	int		assem_type;	/* type of assembly area */
#endif	RPCMOD
	netaddr_t	from;		/* message origin (machine) */
#if	RPCMOD
	unsigned long	ipc_seq_no;	/* sequence number */
#endif	RPCMOD
	ipc_netmsg_hdr_t *netmsg_hdr_ptr;	/* IPC netmsg header */
#if	RPCMOD
#else	RPCMOD
	int		(*cleanup) ();	/* cleanup routine in transport */
#endif	RPCMOD
} ipc_inrec_t;

#if	RPCMOD
#else	RPCMOD
typedef ipc_inrec_t *ipc_inrec_ptr_t;
#endif	RPCMOD

typedef struct {
	int			type;		/* type of transaction */
	int			status;		/* current progress status */
	int			trid;		/* transport-level ID */
	int			trmod;		/* index of transport mod used */
#if	RPCMOD
	int			retry_level;	/* port info at last xmit */
	sys_queue_chain_t	re_send_q;	/* link in queue for re-send */
	sys_queue_chain_t	out_q;		/* link in queue of out reqs */
	port_rec_ptr_t		server_port_ptr;
	port_rec_ptr_t		reply_port_ptr;
#endif	RPCMOD
	ipc_outrec_t		out;
	ipc_inrec_t		in;
#if	RPCMOD
} ipc_rec_t;
#else	RPCMOD
} ipc_iorec_t;
#endif	RPCMOD

#if	RPCMOD
typedef	ipc_rec_t *ipc_rec_ptr_t;
#else	RPCMOD
typedef	ipc_iorec_t *ipc_iorec_ptr_t;
#endif	RPCMOD


/*
 * Structure for queue of senders waiting on a blocked port.
 */
typedef struct ipc_block {
	struct ipc_block	*next;
	netaddr_t		addr;
} ipc_block_t, *ipc_block_ptr_t;

#define IPC_BLOCK_NULL		((ipc_block_ptr_t)0)

#if	RPCMOD
/*
 * Values for assem_type field in ipc_outrec.
 */
#define	IPC_REC_ASSEM_OBJ	0	/* MEM_ALLOCOBJ(MEM_ASSEMBUFF) */
#define	IPC_REC_ASSEM_MEMALLOC	1	/* mem_alloc(assem_len) */
#define	IPC_REC_ASSEM_PKT	2	/* (virtual) copy of packet buffer */
#endif	RPCMOD

/*
 * Values for type field in ipc_rec
 */
#if	RPCMOD
#define	IPC_REC_TYPE_UNKNOWN	0	/* not yet specified */
#define	IPC_REC_TYPE_SINGLE	1	/* single message */
#define	IPC_REC_TYPE_CLIENT	2	/* RPC, client side */
#define	IPC_REC_TYPE_SERVER	3	/* RPC, server side */
#else	RPCMOD
#define	IPC_IOREC_TYPE_SINGLE	1	/* single message */
#define	IPC_IOREC_TYPE_CLIENT	2	/* RPC, client side */
#define	IPC_IOREC_TYPE_SERVER	3	/* RPC, server side */
#endif	RPCMOD

/*
 * Values for status field in ipc_rec
 */
#if	RPCMOD
#define	IPC_REC_ACTIVE		1	/* waiting for ack or reply */
#define	IPC_REC_READY		2	/* ready to be transmitted */
#define	IPC_REC_WAITING		3	/* waiting for new information */
#define	IPC_REC_REPLY		4	/* waiting for local reply */
#else	RPCMOD
#define	IPC_IOREC_WAIT_CLEANUP	1	/* msg sent, waiting for cleanup */
#define	IPC_IOREC_WAIT_REPLY	2	/* req sent, waiting for reply */
#define	IPC_IOREC_WAIT_LOCAL	3	/* req delivered, wtng for local reply */
#define	IPC_IOREC_WAIT_BLOCK	4	/* msg not sent, port (b)locked */
#endif	RPCMOD


/*
 * Standard size for an assembly buffer 
 */
#define	IPC_ASSEM_SIZE		(16384)


/*
 * Return codes for IPC operations. These codes are in the same space as
 * the DISP_* and TR_* codes.
 */
#define	IPC_SUCCESS		1
#define	IPC_FAILURE		2
#define	IPC_PORT_BLOCKED	3
#define	IPC_PORT_NOT_HERE	4
#define IPC_BAD_SEQ_NO		5
#define	IPC_REQUEST		6
#define	IPC_ABORT_REPLY		7
#define	IPC_ABORT_REQUEST	8
#define	IPC_PORT_BUSY		9


#if	RPCMOD
/*
 * Packet used for client abort.
 */
typedef struct {
	struct abort_pkt {
		disp_hdr_t	disp_hdr;
		np_uid_t	np_puid;
		unsigned long	ipc_seq_no;
	} abort_pkt;
	sbuf_t		msg;
	sbuf_seg_t	segs[5];
} ipc_abort_rec_t, *ipc_abort_rec_ptr_t;

#endif	RPCMOD


/*
 * Extern definitions internal to the IPC module.
 */
extern void ipc_in_init();

extern void ipc_in_block();
/*
port_rec_ptr	dp_ptr;
netaddr_t	from;
*/

extern int ipc_in_cancel();
/*
int		ipcid;
int		reason;
*/

extern ipc_in_unblock();
/*
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
*/

extern void ipc_out_init();


/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_ASSEMBUFF;
extern mem_objrec_t		MEM_IPCBLOCK;
#if	RPCMOD
extern mem_objrec_t		MEM_IPCREC;
extern mem_objrec_t		MEM_IPCABORT;
#endif	RPCMOD


#endif	_IPC_REC_
