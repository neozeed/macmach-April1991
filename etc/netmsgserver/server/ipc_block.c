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
 * $Log:	ipc_block.c,v $
 * Revision 1.1  90/02/19  21:51:06  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.11  89/05/02  11:09:04  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 14-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Return DISP_SUCCESS in ipc_in_unblock().
 *
 *  3-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Changed a few debugging codes for RPCMOD.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Bug fix to parameters of ipc_in_unblock.
 *	Added some statistics gathering.
 *
 * 28-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *	Lock is now inline in port record.
 *
 *  5-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Split off from ipc_in.c.  Added ipc_msg_accept.
 *
 */
/*
 * ipc_block.c
 *
 *
 */

#ifndef	lint
char ipc_block_rcsid[] = "$ Header: ipc_block.c,v 1.11 89/05/02 11:09:04 dpj Exp $";
#endif not lint

/*
 * Functions handling blocking and unblocking of network ports.
 */


#include <sys/types.h>
#include <netinet/in.h>

#include "config.h"
#include "crypt.h"
#include "debug.h"
#include "disp_hdr.h"
#include "ipc.h"
#include "ipc_rec.h"
#include "mem.h"
#include "netmsg.h"
#include "nm_defs.h"
#include "nm_extra.h"
#include "port_defs.h"
#include "portrec.h"
#include "sbuf.h"
#include "transport.h"

typedef struct {
	disp_hdr_t	ipc_unblock_disp_hdr;
	np_uid_t	ipc_unblock_np_puid;
} ipc_unblock_t, *ipc_unblock_ptr_t;

#define IPC_MAX_UNBLOCKS	3



/*
 * ipc_in_block
 *	Add a host to the waiting list for a blocked port.
 *
 * Parameters:
 *	dp_ptr	: port record for the blocked port.
 *	from	: address of host to add to the waiting list.
 *
 * Results:
 *	none
 *
 * Side effects:
 *	should add an entry to the waiting list.
 *
 * Note:
 *	the dp_ptr should be locked and it is left locked at exit.
 *
 */
PUBLIC void ipc_in_block(IN dp_ptr, IN from)
	port_rec_ptr_t	dp_ptr;
	netaddr_t	from;
BEGIN("ipc_in_block")
	ipc_block_ptr_t		blk_ptr, current, prev;

	/* dp_ptr LOCK RW/RW */
	/*
	 * Find where to put the new block entry.
	 */
	prev = current = (ipc_block_ptr_t)dp_ptr->portrec_block_queue;
	while (current != IPC_BLOCK_NULL) {
		if (current->addr == from) {
			/*
			 * No point in storing another block record for this host.
			 */
			RET;
		}
		prev = current;
		current = current->next;
	}

	MEM_ALLOCOBJ(blk_ptr,ipc_block_ptr_t,MEM_IPCBLOCK);
	blk_ptr->addr = from;
	blk_ptr->next = IPC_BLOCK_NULL;

	if (prev == IPC_BLOCK_NULL) {
		dp_ptr->portrec_block_queue = (pointer_t)blk_ptr;
	}
	else {
		prev->next = blk_ptr;
	}

	/* dp_ptr LOCK RW/- */

	RET;

END



/*
 * ipc_in_unblock --
 *	accepts an unblock packet from over the network.
 *
 * Parameters:
 *	client_id	: ignored.
 *	data		: the unblock data.
 *	from		: the host sending the unblock.
 *	broadcast	: ignored.
 *	crypt_level	: ignored.
 *
 * Results:
 *	DISP_SUCCESS.
 *
 * Side effects:
 *	Calls ipc_retry to try resending a message to the unblocked port.
 *
 * Note:
 *	The unblock data just contains a network port PUID.
 *
 */
/* ARGSUSED */
PUBLIC ipc_in_unblock(client_id,data, from, broadcast, crypt_level)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("ipc_in_unblock")
	ipc_unblock_ptr_t	unblock_ptr;
	port_rec_ptr_t		port_rec_ptr;

	INCSTAT(ipc_unblocks_rcvd);
	SBUF_GET_SEG(*data, unblock_ptr, ipc_unblock_ptr_t);

	if ((port_rec_ptr = pr_np_puid_lookup(unblock_ptr->ipc_unblock_np_puid)) == PORT_REC_NULL) {
#if	RPCMOD
		LOG0(TRUE, 3, 2540)
#else	RPCMOD
		LOG0(TRUE, 3, 2040)
#endif	RPCMOD
		RETURN(DISP_SUCCESS);
	}

	/* port_rec_ptr LOCK RW/RW */
	if (port_rec_ptr->portrec_network_port.np_receiver != from) {
#if	RPCMOD
		LOG0(TRUE, 3, 2541);
#else	RPCMOD
		LOG0(TRUE, 3, 2041);
#endif	RPCMOD
		LOG_NETADDR(TRUE, 3, port_rec_ptr->portrec_network_port.np_receiver);
		LOG_NETADDR(TRUE, 3, from);
	}
	else {
		port_rec_ptr->portrec_info &= ~PORT_INFO_BLOCKED;
		(void)ipc_retry(port_rec_ptr);
	}
	lk_unlock(&port_rec_ptr->portrec_lock);
	/* port_rec_ptr LOCK -/- */

	RETURN(DISP_SUCCESS);

END




/*
 * ipc_msg_accepted
 *	called when we have received a message accepted notification from the kernel.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to record for relevant port.
 *
 * Design:
 *	Sends a port unblock datagram to some blocked senders.
 *
 * Notes:
 *	Should not send an unblock to too many senders.
 *	Assumes that the port record is locked.
 *
 */
EXPORT void ipc_msg_accepted(port_rec_ptr)
port_rec_ptr_t	port_rec_ptr;
BEGIN("ipc_msg_accepted")
	sbuf_t		sbuf;
	sbuf_seg_t	sbuf_seg;
	ipc_unblock_t	message;
	int		tr, i;
	ipc_block_ptr_t	block_ptr;

	/* port_rec_ptr LOCK RW/RW */
	if (port_rec_ptr->portrec_block_queue == (pointer_t)0) {
		/*
		 * No unblocks to send.
		 */
		RET;
	}

	SBUF_SEG_INIT(sbuf, &sbuf_seg);
	SBUF_APPEND(sbuf, &message, sizeof(ipc_unblock_t));
	message.ipc_unblock_disp_hdr.disp_type = htons(DISP_IPC_UNBLOCK);
	message.ipc_unblock_disp_hdr.src_format = conf_own_format;
	message.ipc_unblock_np_puid = port_rec_ptr->portrec_network_port.np_puid;

	for (i = 0; i < IPC_MAX_UNBLOCKS; i++) {
		block_ptr = (ipc_block_ptr_t)port_rec_ptr->portrec_block_queue;
		if (block_ptr == IPC_BLOCK_NULL) {
		    	break;
		}
		port_rec_ptr->portrec_block_queue = (pointer_t)block_ptr->next;

		tr = transport_switch[TR_DATAGRAM_ENTRY].send(0, &sbuf, block_ptr->addr,
					TRSERV_NORMAL, CRYPT_DONT_ENCRYPT, 0);
		if (tr != TR_SUCCESS) {
			ERROR((msg, "ipc_msg_accepted.send fails, tr = %d.", tr));
		}
		else INCSTAT(ipc_unblocks_sent);

		MEM_DEALLOCOBJ(block_ptr, MEM_IPCBLOCK);
	}

	RET;

END
