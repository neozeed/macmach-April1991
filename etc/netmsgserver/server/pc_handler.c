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
 * $Log:	pc_handler.c,v $
 * Revision 1.1  90/02/19  21:54:43  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.12  89/05/02  11:15:05  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.11  89/04/24  20:41:18  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:56:07  dpj]
 * 
 * 05-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Added USE_SRR, although it must be on for now.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 25-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for byte-swapping of the portcheck packets at the receiver.
 *	Do not take the initiative to mark a port as blocked; let the ipc
 *	module do that if needed.
 *
 * 18-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Use lq_cond_delete_from_queue instead of 
 *	lq_find_in_queue/lq_remove_from_queue.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added some statistics gathering.
 *
 * 29-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	pc_lock is now inline in pc_request_counter.
 *	Re-initialise the checkup timer before calling timer_start.
 *	Lock is now inline in port record.
 *	Re-initialise the checkup timer before calling timer_start.
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *
 *  5-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed to use pr_np_puid_lookup instead of pr_nportlookup.
 *	Handle PORT_INFO_DEAD.  Replaced SBUF_INIT by SBUF_SEG_INIT.
 *
 * 17-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started from the sources of Sanjay Agrawal.
 *
 */
/*
 * pc_handler.c
 *
 *
 */

#ifndef	lint
char pc_handler_rcsid[] = "$ Header: pc_handler.c,v 1.12 89/05/02 11:15:05 dpj Exp $";
#endif not lint

/*
 * Routines handling incoming requests and responses for the port checkups module.
 */


#include "netmsg.h"
#include "nm_defs.h"

#include	<sys/types.h>
#include	<netinet/in.h>

#include "debug.h"
#include "disp_hdr.h"
#include "ipc.h"
#include "ls_defs.h"
#include "network.h"
#include "nm_extra.h"
#include "pc_defs.h"
#include "portcheck.h"
#include "portrec.h"
#include "portsearch.h"
#include "srr.h"
#include "ipc_swap.h"



/*
 * pc_clientequal
 *	Checks to see whether the client_id of a host record matches the input client_id.
 *
 */
PRIVATE int pc_clientequal(hp, client_id)
pc_host_list_ptr_t	hp;
int		client_id;
BEGIN("pc_clientequal")
	RETURN(hp->pchl_client_id == client_id);
END

/*
 * pc_cleanup
 *	Called by the transport module if a request failed.
 *
 * Parameters:
 *	client_id	: the client_id of the failed request
 *	completion_code	: why the request failed
 *
 * Design:
 *	Just feeds this request into pc_handle_checkup_reply
 *	with all the port statuses set to be bad.
 *
 */
PUBLIC int pc_cleanup(client_id, completion_code)
int	client_id;
int	completion_code;
BEGIN("pc_cleanup")
	sbuf_t			msgbuf;
	sbuf_seg_t		msgbuf_seg;
	pc_host_list_ptr_t	hp;
	int			index;

	DEBUG2(PC_DEBUG, 3, 1160, client_id, completion_code);

	hp = (pc_host_list_ptr_t)lq_find_in_queue(&pc_request_queue, pc_clientequal, client_id);

	if (hp == NULL) {
		LOG1(TRUE, 5, 1161, client_id);
		RETURN(0);
	}

	for (index = 0; index < PC_MAX_ENTRIES; index++) {
		hp->pchl_portcheck->pc_status[index] = PORTCHECK_NOTOK;
	}

	SBUF_SEG_INIT(msgbuf, &msgbuf_seg);
#if	USE_SRR
	SBUF_APPEND(msgbuf, hp->pchl_portcheck, srr_max_data_size);
#endif	USE_SRR

	(void)pc_handle_checkup_reply(client_id, (sbuf_ptr_t)&msgbuf, hp->pchl_destination, FALSE, 0);
	RETURN(0);

END



/*
 * pc_handle_checkup_request
 *	Handles and replies to a port checkup request.
 *	It is called by the srr transport protocol.
 *
 * Parameters:
 *	request		: the incoming checkup request
 *	from		: the sender of the request (ignored)
 *	broadcast	: ignored
 *	crypt_level	: ignored
 *
 * Design:
 *	Compares the information about a network port contained in the request
 *	with the information about the port held by the port records module.
 *	The status byte is set accordingly.
 */
/* ARGSUSED */
PUBLIC int pc_handle_checkup_request(request, from, broadcast, crypt_level)
sbuf_ptr_t	request;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("pc_handle_checkup_request")
	network_port_t	nport;
	portcheck_ptr_t pc_ptr;
	int		num_entries, index, size;
	port_rec_ptr_t	port_rec_ptr;

	INCSTAT(pc_requests_rcvd);
	SBUF_GET_SEG(*request, pc_ptr, portcheck_ptr_t);
	SBUF_GET_SIZE(*request, size);
	if (DISP_CHECK_SWAP(pc_ptr->pc_disp_hdr.src_format)) {
		SWAP_DECLS;

		num_entries = SWAP_LONG(pc_ptr->pc_num_entries,
						pc_ptr->pc_num_entries);
	} else {
		num_entries = pc_ptr->pc_num_entries;
	}
	if (size < (num_entries * sizeof(pc_network_port_t))) {
		LOG2(TRUE, 5, 1162, size, num_entries);
		RETURN(DISP_FAILURE);
	}

	DEBUG1(PC_DEBUG, 3, 1163, num_entries);
	DEBUG_NETADDR(PC_DEBUG, 3, from);

	for(index = 0; index < num_entries; index++) {
		PC_EXTRACT_NPORT(nport, pc_ptr->pc_nports[index]);
		port_rec_ptr = pr_np_puid_lookup(pc_ptr->pc_nports[index].pc_np_puid);
		if (port_rec_ptr == PORT_REC_NULL) {
			pc_ptr->pc_status[index] = PORTCHECK_NOTOK;
		}
		else if (port_rec_ptr->portrec_info & PORT_INFO_DEAD) {
			pc_ptr->pc_status[index] = PORTCHECK_DEAD;
			lk_unlock(&port_rec_ptr->portrec_lock);
		}
		else {
			if ((port_rec_ptr->portrec_network_port.np_receiver != nport.np_receiver)
				|| (port_rec_ptr->portrec_network_port.np_owner != nport.np_owner))
			{
				pc_ptr->pc_status[index] |= PORTCHECK_O_R_CHANGED;
			}
			if (port_rec_ptr->portrec_network_port.np_receiver == my_host_id) {
				if ((pc_ptr->pc_status[index] & PORTCHECK_BLOCK)
					!= (port_rec_ptr->portrec_info & PORT_INFO_BLOCKED))
				{
					/*
					 * Either the requestor thought the port was blocked and it is not
					 * or it thought that the port was not blocked and it is.
					 * Reverse the value of the block status.
					 */
					pc_ptr->pc_status[index] ^= PORTCHECK_BLOCK;
				}
			}
			lk_unlock(&port_rec_ptr->portrec_lock);
		}
		DEBUG1(PC_DEBUG, 3, 1164, (unsigned int)pc_ptr->pc_status[index]);
		DEBUG_NPORT(PC_DEBUG, 3, nport);
	}

	/*
	 * Set the headers in preparation for responding to the request.
	 */
	pc_ptr->pc_disp_hdr.src_format = conf_own_format;
	RETURN(DISP_SUCCESS);
END



/*
 * pc_handle_checkup_reply
 *	Handles the results of a port checkup requests.
 *	It is called by the srr transport protocol.
 *
 * Parameters:
 *	client_id	: the id of the request which generated this reply
 *	reply		: pointer to the reply sbuf
 *	from		: host from which the reply was received
 *	broadcast	: ignored
 *	crypt_level	: ignored
 *
 * Design:
 *	Extract the corresponding request record from the pc_request_queue.
 *	Examine the returned status bits to determine:
 *		whether we should call port_search
 *		whether we should change the blocked status of the port
 *		whether we should call ipc_retry
 *	The outstanding request counter is decremented and if it reaches zero
 *	then we restart the checkups timer.
 *
 */
/* ARGSUSED */
PUBLIC int pc_handle_checkup_reply(client_id, reply, from, broadcast, crypt_level)
int		client_id;
sbuf_ptr_t	reply;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("pc_handle_checkup_reply")
	int			num_entries, index, size;
	unsigned char		status;
	portcheck_ptr_t 	pc_ptr;
	port_rec_ptr_t  	port_rec_ptr;
	pc_host_list_ptr_t	hp;

	INCSTAT(pc_replies_rcvd);

	if ((hp = (pc_host_list_ptr_t)lq_cond_delete_from_queue(&pc_request_queue,pc_clientequal,client_id))
		== (pc_host_list_ptr_t)0)
	{
		LOG1(TRUE, 5, 1165, client_id);
		RETURN(DISP_FAILURE);
	}
	if (from != hp->pchl_destination) {
		LOG1(TRUE, 5, 1166, client_id);
		LOG_NETADDR(TRUE, 5, from);
		LOG_NETADDR(TRUE, 5, hp->pchl_destination);
		/*
		 * Deallocate the memory used for this request.
		 */
		MEM_DEALLOCOBJ(hp->pchl_portcheck, MEM_TRBUFF);
		MEM_DEALLOCOBJ(hp, MEM_PCITEM);
		RETURN(DISP_FAILURE);
	}

	SBUF_GET_SEG(*reply, pc_ptr, portcheck_ptr_t);
	SBUF_GET_SIZE(*reply, size);
	if (DISP_CHECK_SWAP(pc_ptr->pc_disp_hdr.src_format)) {
		SWAP_DECLS;

		num_entries = SWAP_LONG(pc_ptr->pc_num_entries,
						pc_ptr->pc_num_entries);
	} else {
		num_entries = pc_ptr->pc_num_entries;
	}
	if (size < (num_entries * sizeof(pc_network_port_t))) {
		LOG2(TRUE, 4, 1167, size, num_entries);
		/*
		 * Deallocate the memory used for this request.
		 */
		MEM_DEALLOCOBJ(hp->pchl_portcheck, MEM_TRBUFF);
		MEM_DEALLOCOBJ(hp, MEM_PCITEM);
		RETURN(DISP_FAILURE);
	}

	DEBUG2(PC_DEBUG, 3, 1168, client_id, num_entries);
	DEBUG_NETADDR(PC_DEBUG, 3, from);

	for (index = 0; index < num_entries; index++) {
#if	PC_DEBUG
		network_port_t	nport;
		PC_EXTRACT_NPORT(nport, pc_ptr->pc_nports[index]);
#endif	PC_DEBUG
		port_rec_ptr = pr_np_puid_lookup(pc_ptr->pc_nports[index].pc_np_puid);
		if (port_rec_ptr == PORT_REC_NULL) {
			LOG0(PC_DEBUG, 3, 1169);
			LOG_NPORT(PC_DEBUG, 3, nport);
			continue;
		}
		/* port_rec_ptr LOCK RW/RW */

		status = pc_ptr->pc_status[index];
		DEBUG1(PC_DEBUG, 3, 1170, (unsigned int)status);
		DEBUG_NPORT(PC_DEBUG, 3, nport);
		if ((status & PORTCHECK_DEAD) || (status & PORTCHECK_NOTOK)
			|| (status & PORTCHECK_O_R_CHANGED))
		{
			ps_do_port_search(port_rec_ptr, FALSE, (network_port_ptr_t)0, (int(*)())0);
		}
		else {
			if ((status & PORTCHECK_BLOCK)
				!= (port_rec_ptr->portrec_info & PORT_INFO_BLOCKED))
			{
				if (port_rec_ptr->portrec_info & PORT_INFO_BLOCKED) {
					port_rec_ptr->portrec_info ^= PORT_INFO_BLOCKED;
					(void)ipc_retry(port_rec_ptr);
#if	0
				} else {
					port_rec_ptr->portrec_info |= PORT_INFO_BLOCKED;
#endif	0
				}
			}
			else {
				port_rec_ptr->portrec_aliveness = PORT_ACTIVE;
			}
		}
		/* port_rec_ptr LOCK -/- */
		lk_unlock(&port_rec_ptr->portrec_lock);
	}

	/*
	 * Deallocate the memory used for this request.
	 */
	MEM_DEALLOCOBJ(hp->pchl_portcheck, MEM_TRBUFF);
	MEM_DEALLOCOBJ(hp, MEM_PCITEM);

	/*
	 * If this was the last request outstanding, reschedule another round of checkups.
	 */
	mutex_lock(&pc_request_counter.pc_lock);
	pc_request_counter.pc_counter --;
	if (pc_request_counter.pc_counter < 0) {
		ERROR((msg, "pc_handle_checkup_reply: pc_counter = %d.", pc_request_counter.pc_counter));
	}
	else if (pc_request_counter.pc_counter == 0) {
		pc_timer.interval.tv_sec = param.pc_checkup_interval;
		pc_timer.interval.tv_usec = 0;
		timer_start(&pc_timer);
	}
	mutex_unlock(&pc_request_counter.pc_lock);

	RETURN(DISP_SUCCESS);
END

