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
 * $Log:	portcheck.c,v $
 * Revision 1.1  90/02/19  21:55:20  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.20  89/05/02  11:15:56  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.19  89/04/24  20:41:44  dpj
 * 	Changes from NeXT as of 88/09/30
 * 
 * 	 13-Oct-87 Mary R. Thompson (mrt) at Carnegie Mellon
 * 	[89/04/19  17:56:44  dpj]
 * 
 *	Changed queue_item_t to cthread_queue_item_t.
 *
 * 05-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Added USE_SRR, although it must be on for now.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  7-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for dispatcher with version number.
 *
 * 25-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for byte-swapping of the portcheck packets at the receiver.
 *	RPCMOD: abort pending RPC's when checking their reply port.
 *
 *  2-Oct-87  Daniel Julin (dpj) at Carnegie Mellon University
 *	Modified to work with no network.
 *
 * 21-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Update the receiver and owner field in the port statistics
 *	record when we check up on a port.
 *
 * 10-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Call mem_deallocobj to free the port list queue.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Do not check a port if it is marked as active (ie. has a message in transit).
 *	Added some statistics gathering.
 *
 *  6-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Checkup timer interval obtained from param record.
 *	Re-initialise the checkup timer before calling timer_start.
 *
 * 16-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Do not check the status of a port if we have all rights to it.
 *	Statically allocate pc_timer, pc_request_queue and pc_request_counter.pc_lock.
 *	Do a LOGCHECK in pc_do_checkups.
 *	Lock is now inline in port record.
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *
 *  5-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Check for PORT_INFO_DEAD when generating requests. 
 *	Replaced SBUF_INIT by SBUF_SEG_INIT.
 *	Replace DISP_PC_REPLY and DISP_PC_REQUEST by DISP_PORTCHECK.
 *	Call pc_send_startup_message at end of pc_init.
 *	Deallocate the lock_queue created by pr_list.
 *	Check the local status of a port if we are both the owner and receiver.
 *
 * 17-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started from the sources of Healfdene Goguen and Sanjay Agrawal.
 *	Use a TRBUFF for a checkup request.
 *	Check the portrec_aliveness field when deciding whether to make a checkup request.
 *
 */
/*
 * portcheck.c
 *
 *
 */

/*
 * Main routines of the port checkups module.
 */

#ifndef	lint
char portcheck_rcsid[] = "$ Header: portcheck.c,v 1.20 89/05/02 11:15:56 dpj Exp $";
#endif not lint


#include	"netmsg.h"
#include	"nm_defs.h"

#include	<cthreads.h>
#include	<mach.h>
#include	<stdio.h>
#include	<mach/boolean.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<netinet/in.h>


#include	"debug.h"
#include	"dispatcher.h"
#include	"lock_queue.h"
#include	"ls_defs.h"
#include	"mem.h"
#include	"network.h"
#include	"nm_extra.h"
#include	"pc_defs.h"
#include	"portcheck.h"
#include	"portops.h"
#include	"portrec.h"
#include	"sbuf.h"
#include	"srr.h"
#include	"timer.h"
#include	"ipc.h"

PRIVATE int		pc_last_client_id = 0;

struct lock_queue	pc_request_queue;

struct timer		pc_timer;

pc_count_t		pc_request_counter;

PRIVATE int pc_do_checkups();

/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_PCITEM;



/*
 * pc_init
 *	Initialises the port checkups module.
 *
 */
PUBLIC boolean_t pc_init()
BEGIN("pc_init")

	/*
	 * Initialize the memory management facilities.
	 */
	mem_initobj(&MEM_PCITEM,"PC item",sizeof(pc_host_list_t),
								FALSE,250,50);

	/*
	 * Set up the queue which will hold outstanding requests.
	 */
	lq_init(&pc_request_queue);

	/*
	 * Set up the timer which will call pc_do_checkups.
 	 */		
	pc_timer.interval.tv_sec = param.pc_checkup_interval;
	pc_timer.interval.tv_usec = 0;
	pc_timer.action = pc_do_checkups;
	pc_timer.info = (char *)0;
	timer_start(&pc_timer);

	/*
	 * Set up the counter which will count outstanding requests.
	 */
	mutex_init(&pc_request_counter.pc_lock);
	pc_request_counter.pc_counter = 0;

	/*
	 * Fill in our entries in the dispatcher switch.
 	 */
	dispatcher_switch[DISPE_PORTCHECK].disp_indata_simple = pc_handle_checkup_reply;
	dispatcher_switch[DISPE_PORTCHECK].disp_rr_simple = pc_handle_checkup_request;
	dispatcher_switch[DISPE_STARTUP].disp_indata_simple = pc_handle_startup_hint;

	/*
	 * Send out a startup hint.
	 */
	if (param.conf_network) {
		pc_send_startup_hint();
	}

	RETURN(TRUE);
END


/*
 * pc_hostequal
 *	Checks to see whether the host in a queued record equals the second host.
 *
 */
PRIVATE int pc_hostequal(hp, netaddr)
pc_host_list_ptr_t	hp;
netaddr_t		netaddr;
BEGIN("pc_hostequal")
	RETURN(hp->pchl_destination == netaddr);
END


/*
 * pc_add_host
 *	Adds a host which is a new destination for checkup requests
 *	to the queue of host requests.
 *
 * Parameters:
 *	destination	: the new host
 *
 * Returns:
 *	Pointer to the queue entry for the host.
 *
 * Notes:
 *	The new record is queued at the head of the host queue
 *	so that if it is a duplicate request (needed because the
 *	first request has reached maximum size) it will be found
 *	first by another call to pc_gen_request.
 *
 *
 */
PRIVATE pc_host_list_ptr_t pc_add_host(destination)
netaddr_t	destination;
BEGIN("pc_add_host")
	pc_host_list_ptr_t	newp;

	/*
	 * Update the number of requests outstanding.
	 */
	mutex_lock(&pc_request_counter.pc_lock);
	++pc_request_counter.pc_counter;
	mutex_unlock(&pc_request_counter.pc_lock);

	/*
	 * Create and fill in a record for this destination.
	 */
	MEM_ALLOCOBJ(newp,pc_host_list_ptr_t,MEM_PCITEM);
	newp->pchl_destination = destination;
	newp->pchl_client_id = pc_last_client_id++;
	MEM_ALLOCOBJ(newp->pchl_portcheck,portcheck_ptr_t,MEM_TRBUFF);
	newp->pchl_portcheck->pc_num_entries = 0;

	DEBUG1(PC_DEBUG, 1, 1210, pc_request_counter.pc_counter);
 	DEBUG_NETADDR(PC_DEBUG, 1, destination);

	lq_prequeue(&pc_request_queue, (cthread_queue_item_t)newp);

	RETURN(newp);
END



/*
 * pc_add_request
 *	Adds a checkup request to the requests destined for a particular host.
 *	If there are no requests for this host, or the number of requests has
 *	grown too large, then a new request packet is made for this host.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to the port record
 *	destination	: host to be queried
 *
 */
PRIVATE void pc_add_request(port_rec_ptr, destination)
port_rec_ptr_t	port_rec_ptr;
netaddr_t	destination;
BEGIN("pc_add_request")
	pc_host_list_ptr_t	hp;

	/*
	 * See if we already have requests queued for the receiver.
	 */
	hp = (pc_host_list_ptr_t)lq_find_in_queue(&pc_request_queue, pc_hostequal, (int)destination);
	if ((hp == (pc_host_list_ptr_t)NULL)
		|| (hp->pchl_portcheck->pc_num_entries == PC_MAX_ENTRIES))
	{
		hp = pc_add_host(destination);
	}

	/*
	 * Add our information to the request.
	 */
	PC_ASSIGN_NPORT(hp->pchl_portcheck->pc_nports[hp->pchl_portcheck->pc_num_entries],
				port_rec_ptr->portrec_network_port);
	
	hp->pchl_portcheck->pc_status[hp->pchl_portcheck->pc_num_entries] = 0;
	if (port_rec_ptr->portrec_info & PORT_INFO_BLOCKED) {
		hp->pchl_portcheck->pc_status[hp->pchl_portcheck->pc_num_entries] = PORTCHECK_BLOCK;
	}
	hp->pchl_portcheck->pc_num_entries++;

	DEBUG1(PC_DEBUG, 1, 1211, port_rec_ptr->portrec_local_port);
	DEBUG_NETADDR(PC_DEBUG, 1, destination);

	RET;
END


/*
 * pc_gen_request
 *	Generate the requests for a particular port.
 *
 * Parameters:
 *	pi_ptr		: pointer to a record generated by pr_list.
 *
 * Design:
 *	Look up the port.
 *	Queue a request for the receiver if it is not this network server.
 *	Queue a request for the owner if it is different from the receiver and not this network server.
 *	Deallocate the record created by pr_list.
 *
 */
PRIVATE void pc_gen_request(pi_ptr)
port_item_ptr_t		pi_ptr;
BEGIN("pc_gen_request")
	register port_rec_ptr_t		port_rec_ptr;

	/*
	 * Find the port record corresponding to this port item.
	 */
	if ((port_rec_ptr = pr_lportlookup(pi_ptr->pi_port)) == PORT_REC_NULL) {
		MEM_DEALLOCOBJ(pi_ptr, MEM_PORTITEM);
		RET;
	}
	/* port_rec_ptr LOCK RW/R */

#if	PORTSTAT
	/*
	 * Update the receiver & owner fields in the port statistics record.
	 */
	port_rec_ptr->portrec_stat->nport_receiver = port_rec_ptr->portrec_network_port.np_receiver;
	port_rec_ptr->portrec_stat->nport_owner = port_rec_ptr->portrec_network_port.np_owner;
#endif	PORTSTAT

	/*
	 * Check the portrec_aliveness field.
	 */
	if (NPORT_HAVE_ALL_RIGHTS(port_rec_ptr->portrec_network_port)
		|| (port_rec_ptr->portrec_info & (PORT_INFO_SUSPENDED | PORT_INFO_DEAD | PORT_INFO_ACTIVE))
		|| ((--port_rec_ptr->portrec_aliveness) > PORT_INACTIVE))
	{
		lk_unlock(&port_rec_ptr->portrec_lock);
		MEM_DEALLOCOBJ(pi_ptr, MEM_PORTITEM);
		RET;
	}

	DEBUG2(PC_DEBUG, 3, 1212, pi_ptr->pi_port, port_rec_ptr->portrec_aliveness);
	DEBUG_NPORT(PC_DEBUG, 3, port_rec_ptr->portrec_network_port);

#if	RPCMOD
	/*
	 * As a precaution, we kick the port in the RPC module to cause
	 * an abort of a pending RPC.
	 */
	ipc_port_moved(port_rec_ptr);
#endif	RPCMOD

	if (PORT_REC_RECEIVER(port_rec_ptr) != my_host_id) {
		pc_add_request(port_rec_ptr, PORT_REC_RECEIVER(port_rec_ptr));
	}

	/*
	 * If the owner is not the same as the receiver do the same for the owner.
	 */
	if ((PORT_REC_OWNER(port_rec_ptr) != my_host_id)
		&& (PORT_REC_RECEIVER(port_rec_ptr) != PORT_REC_OWNER(port_rec_ptr)))
	{
		pc_add_request(port_rec_ptr, PORT_REC_OWNER(port_rec_ptr));
	}

#if	0
	if (NPORT_HAVE_ALL_RIGHTS(port_rec_ptr->portrec_network_port)) {
		/*
		 * Check the local status of this port.
		 */
		kern_return_t	kr;
		boolean_t	enabled, ownership, receive_rights;
		int		num_msgs, backlog;

		port_rec_ptr->portrec_aliveness = PORT_ACTIVE;

		kr = port_status(task_self(), pi_ptr->pi_port, &enabled, &num_msgs,
				&backlog, &ownership, &receive_rights);
#ifdef lint
		enabled; num_msgs; backlog;
#endif lint
		if (kr != KERN_SUCCESS) {
			ERROR((msg, "pc_gen_request.port_status fails, kr = %d.", kr));
		}
		else if (ownership && receive_rights) {
			/*
			 * Should destroy this port.
			 */
			LOG1(TRUE, 3, 1213, po_ptr->pi_port);
			po_notify_port_death(port_rec_ptr);
		}
	}
#endif	0

	lk_unlock(&port_rec_ptr->portrec_lock);
	MEM_DEALLOCOBJ(pi_ptr, MEM_PORTITEM);
	RET;

END


/*
 * pc_send_request
 *	Actually send out a request using srr_send.
 *
 * Parameters:
 *	hp	: pointer to an entry in the host queue.
 *
 * Notes:
 *	The value returned is ignored.
 *
 */
PRIVATE int pc_send_request(hp)
pc_host_list_ptr_t	hp;
BEGIN("pc_send_request")
	sbuf_t		msgbuf;
	sbuf_seg_t	msgbuf_seg;
	int		rc;

	DEBUG1(PC_DEBUG, 1, 1214, hp->pchl_portcheck->pc_num_entries);
	DEBUG_NETADDR(PC_DEBUG, 1, hp->pchl_destination);

	hp->pchl_portcheck->pc_disp_hdr.disp_type = htons(DISP_PORTCHECK);
	hp->pchl_portcheck->pc_disp_hdr.src_format = conf_own_format;
	SBUF_SEG_INIT(msgbuf, &msgbuf_seg);
#if	USE_SRR
	SBUF_APPEND(msgbuf, hp->pchl_portcheck, srr_max_data_size);
#endif	USE_SRR
	if ((rc = transport_switch[TR_SRR_ENTRY].send(hp->pchl_client_id,
				(sbuf_ptr_t)&msgbuf, hp->pchl_destination,
				TRSERV_NORMAL, CRYPT_DONT_ENCRYPT, pc_cleanup)) != TR_SUCCESS)
	{
		ERROR((msg, "pc_send_request.transport_send fails, rc = %d.", rc));
	}
	else INCSTAT(pc_requests_sent);
	RETURN(1);
END



/*
 * pc_do_checkups
 *	Called from the timer module.
 *	Generates checkup requests and then sends them out.
 *	If no requests were generated then we restart the checkup timer.
 *
 */
PRIVATE int pc_do_checkups()
BEGIN("pc_do_checkups")
	lock_queue_t	port_queue;
	port_item_ptr_t	pi_ptr;
	
	DEBUG0(PC_DEBUG, 3, 1215);

	port_queue = pr_list();
	while ((pi_ptr = (port_item_ptr_t)lq_dequeue(port_queue)) != (port_item_ptr_t)0) {
		pc_gen_request(pi_ptr);
	}
	MEM_DEALLOCOBJ(port_queue, MEM_LQUEUE);

	mutex_lock(&pc_request_counter.pc_lock);
	if (pc_request_counter.pc_counter == 0) {
		pc_timer.interval.tv_sec = param.pc_checkup_interval;
		pc_timer.interval.tv_usec = 0;
		timer_start(&pc_timer);
	}
	else {
		lq_map_queue(&pc_request_queue, pc_send_request, 0);
	}
	mutex_unlock(&pc_request_counter.pc_lock);

	LOGCHECK;

	RETURN(0);
END
