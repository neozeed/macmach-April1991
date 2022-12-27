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
 * $Log:	ipc_in.c,v $
 * Revision 1.1  90/02/19  21:51:20  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.26  89/05/02  11:09:22  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.25  89/04/24  20:38:59  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:52:53  dpj]
 * 
 * 09-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Conditionalize encrypt code.
 *
 *  7-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for dispatcher with version number.
 *
 * 19-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added port statistics.
 *
 *  5-Aug-87  Daniel Julin (dpj) and Robert Sansom (rds) at Carnegie Mellon University
 *	Put in Camelot support.
 *
 * 30-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added NETPORT.
 *
 * 22-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed (void) cast from lk_lock calls.
 *
 * 29-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Use NPORT_EQUAL macro instead of pr_nport_equal function.
 *
 * 17-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified locking in ipc_inmsg again, to avoid races between the
 *	cleanup and the sending of the reply on a RPC server.
 *
 * 15-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified locking strategy in ipc_inmsg to avoid possible deadlock
 *	with the remote and local ports, and to avoid certain deadlock on
 *	the service port on the client side of an RPC.
 *
 * 10-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Adapted for use with VMTP.
 *
 *  3-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Allocate the ipc_in record on the stack in ipc_inmsg.
 *	Bug fix to IPC sequence number check in ipc_inmsg.
 *
 * 18-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Try and protect ipc_in_scan_swap from bogus messages.
 *	Use mem_dealloc to deallocate assembly buffers allocated using mem_alloc.
 *	Added some statistics gathering.
 *
 *  5-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Lock is now inline in port record.
 *
 * 20-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Call mem_alloc instead of vm_allocate.
 *
 * 16-Apr-87  Daniel Julin (dpj) at Carnegie Mellon University
 *	Added debugging macros.
 *
 * 15-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed a little bit of lint.
 *
 * 22-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Set the msg_type field of an outgoing message according to
 *	whether the network message was encrypted or not.
 *	If the network message was encrypted check its ipc_seq_no.
 *	Check that the receiver/owner of the reply port matches what we already know.
 *
 *  2-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Minor changes to integrate with other modules.
 *	Always set deallocate bit to FALSE.
 *
 * 13-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for locking of port records and for request-response
 *	operations.
 *
 * 15-Nov-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 *
 */
/*
 * ipc_in.c 
 *
 *
 */
#ifndef	lint
char ipc_in_rcsid[] = "$ Header: ipc_in.c,v 1.26 89/05/02 11:09:22 dpj Exp $";
#endif not lint
/*
 * Operations for incoming IPC messages. This module is responsible for
 * handling messages coming over the network (from the transport module), and
 * delivering them to local processes. 
 */

/*
 * Note: this code is augmented with comments indicating the lock status of
 * port records at various stages of the computation. The format is:
 *
 *  <port record ptr> LOCK <current lock>/<needed lock>
 *
 * where <current lock> and <needed lock> are one of "RW", "R" or "-".
 * A necessary condition for correctness is that 
 * <current lock> >= <needed lock>.
 */


#include	<cthreads.h>
#include	<mach.h>
#include	<mach/msg_type.h>
#include	<stdio.h>
#include	<mach/message.h>
#include	<mach/machine/vm_param.h>

#include	"config.h"
#include	"crypt.h"
#include	"debug.h"
#include	"dispatcher.h"
#include	"ipc.h"
#include	"ipc_hdr.h"
#include	"ipc_rec.h"
#include	"ipc_swap.h"
#include	"ls_defs.h"
#include	"mem.h"
#include	"netmsg.h"
#include	"network.h"
#include	"nm_defs.h"
#include	"nm_extra.h"
#include	"port_defs.h"
#include	"portops.h"
#include	"portrec.h"
#include	"portsearch.h"
#include	"rwlock.h"
#include	"transport.h"

#if	CAMELOT
#include "../camelot/cam.h"
extern struct mutex	camelot_lock;
#endif	CAMELOT


/*
 * ipc_inprobe -- 
 *
 * Examine an incoming message and decide if it can be accepted by the IPC
 * module. 
 *
 * Parameters: 
 *
 * trid: ID used by the transport protocol 
 *
 * pkt: an sbuf containing at least the IPC header for the incoming message 
 *
 * from: the address of the network server where the message originated 
 *
 * trmod: index of the transport module delivering this message.
 *
 * crypt_level: encryption level for this message. (IGNORED)
 *
 * broadcast: TRUE if the message was a broadcast. (IGNORED)
 *
 * Results: 
 *
 * cancel: a routine to be called by the transport module if a previously
 * accepted message cannot be delivered in full. 
 *
 * ipcid: an ID to be used in further references to this message in the IPC
 * module.
 *
 * returns a code indicating whether or not the message is accepted, and why.
 *
 * Side effects: 
 *
 * Allocates an ipc_iorec for the message if accepted. 
 *
 * Note: 
 *
 * This procedure will only work if the IPC netmsg header is fully contained in
 * the first non-empty segment of the data sbuf. 
 *
 * Part of this code is repeated in ipc_inmsg. If you change one, change
 * the other.
 *
 */
/* ARGSUSED */
EXPORT int
ipc_inprobe(IN trid, IN pkt, IN from, OUT cancel, IN trmod, 
				OUT ipcid, IN crypt_level, IN broadcast)
	int			trid;
	sbuf_ptr_t		pkt;
	netaddr_t		from;
	int			(*(*cancel)) ();
	int			trmod;
	int			*ipcid;
	long			crypt_level;
	boolean_t		broadcast;
BEGIN("ipc_inprobe")
	ipc_iorec_ptr_t		ior_ptr;	/* ipc_iorec for this message */
	port_rec_ptr_t		dp_ptr;		/* destination port record */
	ipc_netmsg_hdr_t	*nmh_ptr;	/* netmsg header */
	sbuf_seg_ptr_t		sb_ptr;		/* pointer into data sbuf */
	boolean_t		do_swap;	/* is byte-swapping necessary ? */

*ipcid = 0;
*cancel = 0;

DEBUG5(debug.ipc_in,0,2000,trid,(long)from,trmod,crypt_level,(long)broadcast);

/*
 * Get a pointer to the netmsg header 
 */
sb_ptr = pkt->segs;
while (sb_ptr->s == 0)		/* skip past empty segments */
	sb_ptr++;
nmh_ptr = (ipc_netmsg_hdr_t *) sb_ptr->p;
do_swap = DISP_CHECK_SWAP(nmh_ptr->disp_hdr.src_format);

/*
 * Check the acceptability of the message.
 */

/*
 * Check the destination port 
 */
dp_ptr = pr_nportlookup((network_port_ptr_t)&(nmh_ptr->remote_port));
/* dp_ptr LOCK RW/R */
if ((dp_ptr == 0) || (dp_ptr->portrec_network_port.np_receiver != my_host_id)) {
	/*
	 * Signal port_not_here 
	 */
	lk_unlock(&dp_ptr->portrec_lock);
	/* dp_ptr LOCK -/- */
	RETURN(IPC_PORT_NOT_HERE);
}

/*
 * If the message was encrypted check its ipc sequence number.
 */
#if	USE_CRYPT
if (crypt_level != CRYPT_DONT_ENCRYPT) {
	if (do_swap) {
		SWAP_DECLS;
		(void) SWAP_LONG(nmh_ptr->ipc_seq_no, nmh_ptr->ipc_seq_no);
	}
		
	if (!po_check_ipc_seq_no(dp_ptr, from, nmh_ptr->ipc_seq_no)) {
		/*
		 * Sequence number check failed, reject this message.
		 */
		lk_unlock(&dp_ptr->portrec_lock);
		/* dp_ptr LOCK -/- */
		RETURN(IPC_BAD_SEQ_NO);
	}
}	
#endif	USE_CRYPT

/*
 * Check for a blocked port.
 */
if (dp_ptr->portrec_info & PORT_INFO_BLOCKED) {
	/*
	 * Signal port_blocked and add the sender to the waiting list 
	 */
	ipc_in_block(dp_ptr,from);
	lk_unlock(&dp_ptr->portrec_lock);
	/* dp_ptr LOCK -/- */
	RETURN(IPC_PORT_BLOCKED);
}

/*
 * Refuse any further messages for the same port.
 */
/* dp_ptr LOCK RW/RW */
dp_ptr->portrec_info |= (PORT_INFO_BLOCKED | PORT_INFO_PROBED);

ior_ptr = (ipc_iorec_ptr_t) dp_ptr->portrec_current_ipcrec;

lk_unlock(&dp_ptr->portrec_lock);
/* dp_ptr LOCK -/- */

if (ior_ptr == 0) {
	/*
	 * Allocate an ipc_iorec.
	 */
	if ((ior_ptr = (ipc_iorec_ptr_t) mem_allocobj(MEM_IPCIOREC)) == 0) {
		panic("ipc_inprobe cannot get a new ipc_iorec");
	}
	ior_ptr->in.dest_port_ptr = dp_ptr;
	if (nmh_ptr->info & IPC_INFO_RPC)
		ior_ptr->type = IPC_IOREC_TYPE_SERVER;
	else
		ior_ptr->type = IPC_IOREC_TYPE_SINGLE;
}

*ipcid = (int) ior_ptr;
*cancel = ipc_in_cancel;

RETURN(IPC_SUCCESS);

END


/*
 * ipc_in_scan -- 
 *
 * Assemble an incoming message and translate ports and ool pointers. 
 *
 * Parameters: 
 *
 * ir_ptr: pointer to an ipc_inrec for the message to assemble, with a properly
 * allocated assembly buffer.
 *
 * crypt_level: the encryption level of the incoming message.
 *
 * Results: 
 *
 * none. 
 *
 * Side effects: 
 *
 * Fills the assembly buffer with the correct data. May call various Port
 * Operations functions if necessary. 
 *
 * Note: 
 *
 * Part of this code is repeated in ipc_in_scan_swap. If you change one, 
 * change the other.
 *
 */
PRIVATE void
ipc_in_scan(IN ir_ptr, IN crypt_level)
	ipc_inrec_ptr_t 	ir_ptr;
	int			crypt_level;
BEGIN("ipc_in_scan")
	ipc_netmsg_hdr_t	*nmh_ptr;
	sbuf_pos_t      	from;		/* current position in the incoming data */
	pointer_t       	to_ptr;		/* current position in the assembly buffer */
	pointer_t		npd_ptr;	/* Network Port Dictionary */
	pointer_t       	npd_cur;	/* current location in npd */
	msg_type_long_t		*scan_ptr;	/* pointer for scanning the msg */
	msg_type_long_t		*end_scan_ptr;	/* pointer for end of msg */

nmh_ptr = ir_ptr->netmsg_hdr_ptr;
SBUF_SEEK(*ir_ptr->msg, from, sizeof(ipc_netmsg_hdr_t));
to_ptr = (pointer_t) ir_ptr->assem_buff;

/*
 * Get the Network Port Dictionary if appropriate. 
 */
if (!(nmh_ptr->info & IPC_INFO_SIMPLE)) {
	if (nmh_ptr->npd_size) {
		npd_ptr = mem_alloc((int)nmh_ptr->npd_size, FALSE);
		npd_cur = npd_ptr;
		SBUF_EXTRACT(*ir_ptr->msg, from, npd_ptr, nmh_ptr->npd_size);
	}
}
/*
 * Get the inline section. 
 */
SBUF_EXTRACT(*ir_ptr->msg, from, to_ptr, nmh_ptr->inline_size);

#define	ADDSCAN(p,o)	(((char *)p + o))
scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff, sizeof(msg_header_t));
end_scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff, nmh_ptr->inline_size);

/*
 * Scan and translate. 
 */
while (scan_ptr < end_scan_ptr) {
	unsigned long		tn;	/* type of current data */
	unsigned long		elts;	/* number of elements in current descriptor */
	unsigned long		len;	/* length of current data */
	pointer_t		dptr;	/* pointer to current data */
#ifdef lint
	msg_type_t		mth;	/* current msg_type_header */
#else lint
	register msg_type_t	mth;	/* current msg_type_header */
#endif lint

	scan_ptr->msg_type_header.msg_type_deallocate = 0;
	mth = scan_ptr->msg_type_header;
	if (mth.msg_type_longform) {
		tn = scan_ptr->msg_type_long_name;
		elts = scan_ptr->msg_type_long_number;
#if	LongAlign
		len = (((scan_ptr->msg_type_long_size * elts) + 31) >> 5) << 2;
#else	LongAlign
		len = (((scan_ptr->msg_type_long_size * elts) + 15) >> 4) << 1;
#endif	LongAlign
		dptr = (pointer_t) ADDSCAN(scan_ptr, sizeof(msg_type_long_t));
	} else {
		tn = mth.msg_type_name;
		elts = mth.msg_type_number;
#if	LongAlign
		len = (((mth.msg_type_size * elts) + 31) >> 5) << 2;
#else	LongAlign
		len = (((mth.msg_type_size * elts) + 15) >> 4) << 1;
#endif	LongAlign
		dptr = (pointer_t) ADDSCAN(scan_ptr, sizeof(msg_type_t));
	}

	DEBUG6(debug.ipc_in, 3, 2030, mth.msg_type_inline, mth.msg_type_deallocate,
		mth.msg_type_longform, tn, len, elts);

	/*
	 * Enter out-of-line sections if necessary, and advance to the next
	 * descriptor. 
	 */
	if (mth.msg_type_inline) {
		scan_ptr = (msg_type_long_t *) ADDSCAN(dptr, len);
	} else {
		scan_ptr = (msg_type_long_t *) ADDSCAN(dptr, sizeof(char *));
		to_ptr = (pointer_t) round_page(to_ptr);
		*(pointer_t *) dptr = to_ptr;
		dptr = to_ptr;
		SBUF_EXTRACT(*ir_ptr->msg, from, to_ptr, len);
	}

	/*
	 * This is a good place to handle imaginary data (copy-on-reference). 
	 */

	/*
	 * Translate ports if needed 
	 */
	if (MSG_TYPE_PORT_ANY(tn)) {
		int	i;		/* index for iterating over elements */
		int	npd_entry_size;	/* size of new NPD entry */

		for (i = 1; i <= elts; i++) {
			npd_entry_size = po_translate_nport_rights(ir_ptr->from, npd_cur, crypt_level,
						(port_t *) dptr, (int *) &tn);
			npd_cur = (pointer_t) (((char *) npd_cur) + npd_entry_size);
			dptr = (pointer_t) ADDSCAN(dptr, sizeof(port_t));
		}
	}
}

#undef	ADDSCAN

RET;

END



/*
 * ipc_in_scan_swap -- 
 *
 * Assemble an incoming message and translate ports and ool pointers. Byte-swap
 * the message data as appropriate. 
 *
 * Parameters: 
 *
 * ir_ptr: pointer to an ipc_inrec for the message to assemble, with a properly
 * allocated assembly buffer. 
 *
 * crypt_level: the encryption level of the incoming message.
 *
 * Results: 
 *
 * none. 
 *
 * Side effects: 
 *
 * Fills the assembly buffer with the correct data. May call various Port
 * Operations functions if necessary. 
 *
 * Note: 
 *
 * Part of this code is repeated in ipc_in_scan. If you change one, change
 * the other.
 *
 */
PRIVATE void
ipc_in_scan_swap(IN ir_ptr, IN crypt_level)
	ipc_inrec_ptr_t		ir_ptr;
	int			crypt_level;
BEGIN("ipc_in_scan_swap")
	ipc_netmsg_hdr_t	*nmh_ptr;
	sbuf_pos_t		from;		/* current position in the incoming data */
	pointer_t		to_ptr;		/* current position in the assembly buffer */
	pointer_t		npd_ptr;	/* Network Port Dictionary */
	pointer_t		npd_cur;	/* current location in npd */
	msg_type_long_t		*scan_ptr;	/* pointer for scanning the msg */
	msg_type_long_t		*end_scan_ptr;	/* pointer for end of msg */

nmh_ptr = ir_ptr->netmsg_hdr_ptr;
SBUF_SEEK(*ir_ptr->msg, from, sizeof(ipc_netmsg_hdr_t));
to_ptr = (pointer_t) ir_ptr->assem_buff;

/*
 * Get the Network Port Dictionary if appropriate. 
 */
if (!(nmh_ptr->info & IPC_INFO_SIMPLE)) {
	if (nmh_ptr->npd_size) {
		npd_ptr = mem_alloc((int)nmh_ptr->npd_size, FALSE);
		npd_cur = npd_ptr;
		SBUF_EXTRACT(*ir_ptr->msg, from, npd_ptr, nmh_ptr->npd_size);
	}
}
/*
 * Get the inline section. 
 */
SBUF_EXTRACT(*ir_ptr->msg, from, to_ptr, nmh_ptr->inline_size);

{
	/*
	 * Byte-swap the msg header. 
	 */
	register msg_header_t *msg = (msg_header_t *) ir_ptr->assem_buff;
	SWAP_DECLS;

	(void) SWAP_LONG(msg->msg_size, msg->msg_size);
	(void) SWAP_LONG(msg->msg_type, msg->msg_type);
	(void) SWAP_LONG(msg->msg_id, msg->msg_id);
}

/*
 * Scan, translate and byte-swap. 
 */
#define	ADDSCAN(p,o)	(((char *)p + o))
scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff, sizeof(msg_header_t));
end_scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff, nmh_ptr->inline_size);

while (scan_ptr < end_scan_ptr) {
	unsigned short			tn;	/* type of current data */
	unsigned short			ts;	/* type size of current data */
	unsigned long			elts;	/* number of elements in current descriptor */
	unsigned long			len;	/* length of current data */
	pointer_t			dptr;	/* pointer to current data */
	msg_type_t			mth;	/* current msg_type_header */
	swap_msg_type_t			rmth;	/* reverse mth */
	SWAP_DECLS;				/* declarations for swapping macros */

	 /* XXX */ *(long *) &mth = 0;	/* make sure the dealloc bit stays 0 */
	 /* XXX */ *(long *) &rmth = *(long *) &(scan_ptr->msg_type_header);
	if (mth.msg_type_longform = rmth.msg_type_longform) {
		tn = SWAP_SHORT(scan_ptr->msg_type_long_name, scan_ptr->msg_type_long_name);
		elts = SWAP_LONG(scan_ptr->msg_type_long_number, scan_ptr->msg_type_long_number);
		ts = SWAP_SHORT(scan_ptr->msg_type_long_size, scan_ptr->msg_type_long_size);
#if	LongAlign
/*
		len = (((SWAP_SHORT(scan_ptr->msg_type_long_size, scan_ptr->msg_type_long_size)
				* elts) + 31) >> 5) << 2;
*/
		len = (((ts * elts) + 31) >> 5) << 2;
#else	LongAlign
/*
		len = (((SWAP_SHORT(scan_ptr->msg_type_long_size, scan_ptr->msg_type_long_size)
				* elts) + 15) >> 4) << 1;
*/
		len = (((ts * elts) + 31) >> 4) << 1;
#endif	LongAlign
		dptr = (pointer_t) ADDSCAN(scan_ptr, sizeof(msg_type_long_t));
	} else {
		tn = mth.msg_type_name = rmth.msg_type_name;
#if	VaxOrder
		elts = mth.msg_type_number = rmth.msg_type_numlow | (rmth.msg_type_numhigh << 4);
#else	VaxOrder
		elts = mth.msg_type_number = rmth.msg_type_numlow | (rmth.msg_type_numhigh << 8);
#endif	VaxOrder
#if	LongAlign
		len = ((((mth.msg_type_size = rmth.msg_type_size) * elts) + 31) >> 5) << 2;
#else	LongAlign
		len = ((((mth.msg_type_size = rmth_msg_type_size) * elts) + 15) >> 4) << 1;
#endif	LongAlign

		dptr = (pointer_t) ADDSCAN(scan_ptr, sizeof(msg_type_t));
	}

	DEBUG6(debug.ipc_in, 3, 2029, rmth.msg_type_inline, mth.msg_type_deallocate,
		mth.msg_type_longform, tn, len, elts);

	/*
	 * Byte-swap the data. Enter out-of-line sections if necessary, and
	 * advance to the next descriptor. 
	 */
	if (mth.msg_type_inline = rmth.msg_type_inline) {
		scan_ptr->msg_type_header = mth;
		scan_ptr = (msg_type_long_t *) ADDSCAN(dptr, len);
		if (scan_ptr > end_scan_ptr) {
			ERROR((msg, "ipc_in_scan_swap: too much data to swap."));
			break;
		}
		switch (tn) {
			/* case MSG_TYPE_BOOLEAN: XXX */
		case MSG_TYPE_INTEGER_16:
			SWAP_SHORT_ARRAY(dptr, elts);
			break;
		case MSG_TYPE_INTEGER_32:
			SWAP_LONG_ARRAY(dptr, elts);
			break;
		case MSG_TYPE_REAL:
			ERROR((msg, "ERROR: cannot byte-swap real numbers"));
			PRINT_ERROR;
			break;
		}
	} else {
		scan_ptr->msg_type_header = mth;
		scan_ptr = (msg_type_long_t *) ADDSCAN(dptr, sizeof(char *));
		if (nmh_ptr->info & IPC_INFO_SIMPLE) {
			/*
			 * Cannot really have OOL data.
			 */
			ERROR((msg, "ipc_in_scan_swap: unexpected out-of-line data."));
			continue;
		}
		to_ptr = (pointer_t) round_page(to_ptr);
		*(pointer_t *) dptr = to_ptr;
		dptr = to_ptr;
		switch (tn) {
			/* case MSG_TYPE_BOOLEAN: XXX */
		case MSG_TYPE_INTEGER_16:
			(void)swap_short_sbuf(ir_ptr->msg, &from, (unsigned short **)&to_ptr, (int)elts);
			break;
		case MSG_TYPE_INTEGER_32:
			(void)swap_long_sbuf(ir_ptr->msg, &from, (unsigned long **)&to_ptr, (int)elts);
			break;
		case MSG_TYPE_REAL:
			ERROR((msg, "ERROR: cannot byte-swap real numbers"));
			PRINT_ERROR;
			break;
		default:
			SBUF_EXTRACT(*ir_ptr->msg, from, to_ptr, len);
			break;
		}
	}

	/*
	 * This is a good place to handle imaginary data (copy-on-reference). 
	 */

	/*
	 * Translate ports if needed 
	 */
	if (MSG_TYPE_PORT_ANY(tn)) {
		int	i;		/* index for iterating over elements */
		int	npd_entry_size;	/* size of new NPD entry */

		if (nmh_ptr->info & IPC_INFO_SIMPLE) {
			/*
			 * Cannot really have port data.
			 */
			ERROR((msg, "ipc_in_scan_swap: unexpected port data."));
			continue;
		}
		for (i = 1; i <= elts; i++) {
			npd_entry_size = po_translate_nport_rights(ir_ptr->from, npd_cur, crypt_level,
						(port_t *) dptr, (int *) &tn);
			npd_cur = (pointer_t) (((char *) npd_cur) + npd_entry_size);
			dptr = (pointer_t) ADDSCAN(dptr, sizeof(port_t));
		}
	}
}

#undef	ADDSCAN

RET;

END




/*
 * ipc_inmsg -- 
 *
 * Process an incoming IPC message, and deliver it to the local port if
 * appropriate. 
 *
 * Parameters: 
 *
 * trid: ID used by the transport module 
 *
 * msg: an sbuf containing the incoming message 
 *
 * from: the address of the network server where the message originated 
 *
 * cleanup: procedure in the transport module, to be called when the space 
 * used by the message data can be released. 
 *
 * trmod: index of the transport module delivering this message.
 *
 * ipcid: ID used by the IPC module (from a previous probe or a 
 * request-response interaction), or 0 
 *
 * crypt_level: encryption level for this message.
 *
 * broadcast: TRUE if the message was a broadcast. (IGNORED)
 *
 * Results: 
 *
 * a code indicating whether or not the message is accepted, and why.
 *
 * Side effects: 
 *
 * May allocate an ipc_iorec if one does not already exist.
 *
 * Note: 
 *
 * Part of this code is repeated in ipc_inprobe. If you change one, change
 * the other.
 *
 * We use pr_ntran to translate the local port in the netmsg header.
 * Thus this port does not get handled securely.
 *
 */
/* ARGSUSED */
EXPORT int
ipc_inmsg(IN trid, IN msg, IN from, IN cleanup, IN trmod, 
				IN ipcid, IN crypt_level, IN broadcast)
	int			trid;
	sbuf_ptr_t		msg;
	netaddr_t		from;
	int			(*cleanup) ();
	int			trmod;
	int			ipcid;
	int			crypt_level;
	boolean_t		broadcast;
BEGIN("ipc_inmsg")
	ipc_iorec_t		ior_data;	/* ipc_iorec for this message */
	ipc_iorec_ptr_t		ior_ptr;	/* ipc_iorec for this message */
	ipc_inrec_ptr_t 	ir_ptr;		/* ipc_inrec for this message */
	port_rec_ptr_t		dp_ptr;		/* destination port record */
    	port_rec_ptr_t		rp_ptr;		/* port record for reply port */
	ipc_netmsg_hdr_t	*nmh_ptr;	/* netmsg header */
	sbuf_seg_ptr_t		sb_ptr;		/* pointer into data sbuf */
	sbuf_pos_t		cur_pos;	/* current position in the message */
	boolean_t		do_swap;	/* is byte-swapping necessary */
	boolean_t		dealloc_ior;	/* should we deallocate the ipc_iorec */
	msg_return_t		msg_ret;

DEBUG6(debug.ipc_in,0,2001,trid,(long)from,trmod,ipcid,crypt_level,(long)broadcast);

/*
 * Get a pointer to the netmsg header 
 */
sb_ptr = msg->segs;
while (sb_ptr->s == 0)		/* skip past empty segments */
	sb_ptr++;
nmh_ptr = (ipc_netmsg_hdr_t *) sb_ptr->p;
do_swap = DISP_CHECK_SWAP(nmh_ptr->disp_hdr.src_format);

/*
 * Check the acceptability of the message.
 */

/*
 * Check the destination port 
 */
if (ipcid == 0) {
	dp_ptr = (port_rec_ptr_t) pr_nportlookup(&nmh_ptr->local_port);
	/* dp_ptr LOCK RW/R */
	if ((dp_ptr == 0)
	    || (dp_ptr->portrec_network_port.np_receiver != my_host_id)
	    || (dp_ptr->portrec_info & PORT_INFO_DEAD))
	{
		/*
		 * Signal port_not_here 
		 */
		DEBUG1(debug.ipc_in,0,2002,(long)dp_ptr);
		DEBUG_NPORT(debug.ipc_in,0,nmh_ptr->local_port);
		if (dp_ptr != 0) lk_unlock(&dp_ptr->portrec_lock);
		/* dp_ptr LOCK -/- */
		(void)cleanup(trid);
		RETURN(IPC_PORT_NOT_HERE);
	}
	ior_ptr = (ipc_iorec_ptr_t) dp_ptr->portrec_current_ipcrec;

	/*
	 * If the message was encrypted check its ipc sequence number.
	 */
#if	USE_CRYPT
	if (crypt_level != CRYPT_DONT_ENCRYPT) {
		if (do_swap) {
			SWAP_DECLS;
			(void) SWAP_LONG(nmh_ptr->ipc_seq_no, nmh_ptr->ipc_seq_no);
		}
		
		if (!po_check_ipc_seq_no(dp_ptr, from, nmh_ptr->ipc_seq_no)) {
			/*
			 * Sequence number check failed, reject this message.
			 */
		    	DEBUG1(debug.ipc_in,0,2003,nmh_ptr->ipc_seq_no);
			lk_unlock(&dp_ptr->portrec_lock);
			/* dp_ptr LOCK -/- */
			(void)cleanup(trid);
			RETURN(IPC_BAD_SEQ_NO);
		}
	}	
#endif	USE_CRYPT
} else {
	ior_ptr = (ipc_iorec_ptr_t) ipcid;
	ir_ptr = &ior_ptr->in;
	dp_ptr = ior_ptr->in.dest_port_ptr;
	DEBUG1(debug.ipc_in,0,2043,dp_ptr);
	lk_lock(&dp_ptr->portrec_lock,PERM_READWRITE,TRUE);
	/* dp_ptr LOCK RW/R */
	DEBUG0(debug.ipc_in & 0xf000,0,2044);
	/*
	 * Check that the destination port in the actual message matches
	 * the one in the dp_ptr; reject the message is they do not match.
	 */
	if (!(NPORT_EQUAL(dp_ptr->portrec_network_port, nmh_ptr->local_port))) {
		ERROR((msg, "ipc_inmsg: network ports in message and port record differ.\n"));
		lk_unlock(&dp_ptr->portrec_lock);
		/* dp_ptr LOCK -/- */
		(void)cleanup(trid);
		RETURN(IPC_FAILURE);
	}
				
}

if (dp_ptr->portrec_info & PORT_INFO_PROBED) {
    	dp_ptr->portrec_info &= ~PORT_INFO_PROBED;
} else {
	/*
	 * Check for a blocked port.
	 */
	if (dp_ptr->portrec_info & PORT_INFO_BLOCKED) {
		/*
		 * Signal port_blocked and add the sender to the waiting list 
		 */
		DEBUG0(debug.ipc_in,0,2004);
		ipc_in_block(dp_ptr,from);
		lk_unlock(&dp_ptr->portrec_lock);
		/* dp_ptr LOCK -/- */
		(void)cleanup(trid);
		RETURN(IPC_PORT_BLOCKED);
	}

	/*
	 * Refuse any further messages for the same port.
	 */
	/* dp_ptr LOCK RW/RW */
	dp_ptr->portrec_info |= PORT_INFO_BLOCKED;
}

/*
 * Byte-swap the rest of the netmsg header if needed. 
 */
if (do_swap) {
	SWAP_DECLS;

	/* Worry about local_port and remote_port XXX */
	(void) SWAP_LONG(nmh_ptr->info, nmh_ptr->info);
	(void) SWAP_LONG(nmh_ptr->npd_size, nmh_ptr->npd_size);
	(void) SWAP_LONG(nmh_ptr->inline_size, nmh_ptr->inline_size);
	(void) SWAP_LONG(nmh_ptr->ool_size, nmh_ptr->ool_size);
	(void) SWAP_LONG(nmh_ptr->ool_num, nmh_ptr->ool_num);
}


/*
 * At this point, we are sure that we can accept the message.
 * We could switch to another thread, to free the network thread.
 */
DEBUG0(debug.ipc_in,0,2005);

if (ior_ptr == 0) {
	/*
	 * Use the statically allocate ipc_iorec.
	 */
	ior_ptr = &ior_data;
	ir_ptr = &ior_ptr->in;
	dealloc_ior = FALSE;
	ir_ptr->dest_port_ptr = dp_ptr;
	if (nmh_ptr->info & IPC_INFO_RPC)
		ior_ptr->type = IPC_IOREC_TYPE_SERVER;
	else
		ior_ptr->type = IPC_IOREC_TYPE_SINGLE;
}
else {
	ir_ptr = &ior_ptr->in;
	dealloc_ior = TRUE;
}

/*
 * Unlock the port record while assembling the message.
 */
lk_unlock(&dp_ptr->portrec_lock);
/* dp_ptr LOCK -/- */

ir_ptr->netmsg_hdr_ptr = nmh_ptr;
ir_ptr->msg = msg;
ir_ptr->from = from;
ir_ptr->cleanup = cleanup;
ior_ptr->trid = trid;
ior_ptr->trmod = trmod;

SBUF_SEEK(*msg, cur_pos, sizeof(ipc_netmsg_hdr_t));

/*
 * Allocate an assembly buffer. 
 */
if (nmh_ptr->info & IPC_INFO_SIMPLE) {
	if ((!do_swap) &&
	    (cur_pos.data_left >= nmh_ptr->inline_size)) {
		/*
		 * The whole message is entirely contained in one
		 * sbuf segment; it is not necessary to copy it. 
		 */
		ior_ptr->in.assem_buff = (msg_header_t *) cur_pos.data_ptr;
		ior_ptr->in.assem_len = -1;
	} else {
		/*
		 * For a simple message, a MEM_ASSEMBUFF is surely
		 * enough. Get one and copy the message into it. 
		 */
		ior_ptr->in.assem_buff = (msg_header_t *) mem_allocobj(MEM_ASSEMBUFF);
		if (ior_ptr->in.assem_buff == 0) {
			panic("ipc_inmsg cannot get an assembly buffer");
		}
		ior_ptr->in.assem_len = 0;
	}
} else {
	/*
	 * For a complex message, we must take care to have an
	 * assembly buffer big enough to contain all ool sections,
	 * with the proper page alignment. 
	 */
	int	assem_len;

	assem_len = (nmh_ptr->ool_num * vm_page_size) + nmh_ptr->inline_size + nmh_ptr->ool_size;
	if (assem_len > IPC_ASSEM_SIZE) {
		if ((ior_ptr->in.assem_buff = (msg_header_t *)mem_alloc(assem_len, FALSE)) == 0) {
			panic("ipc_inmsg cannot get an assembly buffer");
		}
		else {
			LOG2(debug.ipc_in,3,2006,
				(long)assem_len, (long)ior_ptr->in.assem_buff);
		}
		ior_ptr->in.assem_len = assem_len;
	} else {
		ior_ptr->in.assem_buff = (msg_header_t *) mem_allocobj(MEM_ASSEMBUFF);
		if (ior_ptr->in.assem_buff == 0) {
			panic("ipc_inmsg cannot get an assembly buffer");
		}
		ior_ptr->in.assem_len = 0;
	}

}

/*
 * Assemble the message and translate it if necessary. 
 *
 * Note: if the sbuf segment is used as assembly buffer, there is nothing to do. 
 */
if (ior_ptr->in.assem_len != -1) {
	if (do_swap) {
		ipc_in_scan_swap(ir_ptr, crypt_level);
	} else {
		if (nmh_ptr->info & IPC_INFO_SIMPLE) {
			pointer_t       to_ptr;

			to_ptr = (pointer_t) ior_ptr->in.assem_buff;
			SBUF_EXTRACT(*msg, cur_pos, to_ptr, nmh_ptr->inline_size);
		} else {
			ipc_in_scan(ir_ptr, crypt_level);
		}
	}
}
/*
 * Translate and exchange remote_port and local_port. 
 * Keep the reply portrec blocked for a RPC server, so that
 * the reply cannot be sent before we are finished dealing with the request.
 */
if ((rp_ptr = pr_nportlookup(&(nmh_ptr->remote_port))) == PORT_REC_NULL) {
	if ((rp_ptr = pr_ntran(&(nmh_ptr->remote_port))) != PORT_REC_NULL) {
		/*
		 * First time that we have seen this port - remember its security level.
		 */
		rp_ptr->portrec_security_level = crypt_level;
#if	NETPORT
		netport_enter(task_self(),rp_ptr->portrec_network_port,
			rp_ptr->portrec_local_port,
			(rp_ptr->portrec_network_port.np_receiver == my_host_id));
#endif	NETPORT
	}
}
/* rp_ptr LOCK RW/RW */
if (rp_ptr == PORT_REC_NULL) {
	ior_ptr->in.assem_buff->msg_local_port = PORT_NULL;
} else {
	ior_ptr->in.assem_buff->msg_local_port = rp_ptr->portrec_local_port;
	/*
	 * Check the receiver/owner of the reply port.
	 */
	if ((nmh_ptr->remote_port.np_receiver != rp_ptr->portrec_network_port.np_receiver)
	    || (nmh_ptr->remote_port.np_owner != rp_ptr->portrec_network_port.np_owner))
	{
		ps_do_port_search(rp_ptr, TRUE, &(nmh_ptr->remote_port), (int(*)())0);
	}
	if (ior_ptr->type != IPC_IOREC_TYPE_SERVER) {
		lk_unlock(&rp_ptr->portrec_lock);
		/* rp_ptr LOCK -/- */
	}
}
lk_lock(&dp_ptr->portrec_lock,PERM_READWRITE,TRUE);
/* dp_ptr LOCK RW/RW */
ior_ptr->in.assem_buff->msg_remote_port = dp_ptr->portrec_local_port;

/*
 * Set the msg_type field according to the crypt_level of the network message.
 */
#if	USE_CRYPT
if (crypt_level != CRYPT_DONT_ENCRYPT) ior_ptr->in.assem_buff->msg_type |= MSG_TYPE_ENCRYPTED;
#endif	USE_CRYPT

/*
 * Deliver the message, unblock the port if needed, and look for
 * other senders waiting. 
 */
#if	CAMELOT
if (Cam_Message(Cam_MsgHeader(ior_ptr->in.assem_buff))) {
	char *data;

	mutex_lock(&camelot_lock);
	data = Cam_Receive(Cam_MsgHeader(ior_ptr->in.assem_buff), from);

	if (data != NULL) {
		msg_ret = msg_send(ior_ptr->in.assem_buff, SEND_NOTIFY, 0);
/*
		Cam_Restore(Cam_MsgHeader(ior_ptr->in.assem_buff), data);
*/
	} else
		msg_ret = SEND_SUCCESS;	/* drop message on floor */
	mutex_unlock(&camelot_lock);
} else
#endif	CAMELOT
msg_ret = msg_send(ior_ptr->in.assem_buff, SEND_NOTIFY, 0);
INCSTAT(ipc_in_messages);
INCPORTSTAT(dp_ptr, messages_rcvd);
DEBUG2(debug.ipc_in,0,2007,ior_ptr->in.assem_buff->msg_id,msg_ret);
dp_ptr->portrec_current_ipcrec = 0;
if (msg_ret == SEND_WILL_NOTIFY) {
	/*
	 * The port remains blocked. Do nothing. 
	 */
} else {
	if (msg_ret != SEND_SUCCESS) {
		/*
		 * Something strange happened. Report it, then go on as usual. 
		 */
		ERROR((msg, "ipc_inmsg: cannot deliver the message: %d", msg_ret));
		PRINT_ERROR;
	}
	dp_ptr->portrec_info &= ~PORT_INFO_BLOCKED;
	if (dp_ptr->portrec_block_queue) {
		/*
		 * Signal port_unblocked to network servers on blocked queue. 
		 */
		ipc_msg_accepted(dp_ptr);
	}
}
lk_unlock(&dp_ptr->portrec_lock);
/* dp_ptr LOCK -/- */

/*
 * Deallocate all resources used for the incoming message. 
 */
(*ior_ptr->in.cleanup) (ior_ptr->trid);
switch (ior_ptr->in.assem_len) {
	case -1:
		break;
	case 0:
		mem_deallocobj((pointer_t)ior_ptr->in.assem_buff, MEM_ASSEMBUFF);
		break;
	default:
		mem_dealloc((pointer_t)ior_ptr->in.assem_buff, ior_ptr->in.assem_len);
		break;
}

DEBUG2(debug.ipc_in,0,2051,dp_ptr,rp_ptr);
/*
 * Deal with the ipc_iorec in accordance with the type of transaction in progress.
 */
switch (ior_ptr->type) {

    	case IPC_IOREC_TYPE_CLIENT:
		DEBUG1(debug.ipc_in,0,2045,ior_ptr);
		(void)ipc_out_cleanup((int) ior_ptr, IPC_SUCCESS);
		break;
		
    	case IPC_IOREC_TYPE_SINGLE:
		if (dealloc_ior) mem_deallocobj((pointer_t)ior_ptr, MEM_IPCIOREC);
		break;

	case IPC_IOREC_TYPE_SERVER:
		if (rp_ptr == PORT_REC_NULL) {
			ERROR((msg, "ipc_inmsg: got an RPC request, but there is no reply port."));
			if (dealloc_ior)
				mem_deallocobj((pointer_t)ior_ptr, MEM_IPCIOREC);
			/*
			 * Worry about the transport module. XXX
			 */
			break;
		}
		ior_ptr->status = IPC_IOREC_WAIT_LOCAL;
		ior_ptr->out.dest_port_ptr = rp_ptr;
		if (rp_ptr->portrec_current_ipcrec) {
		    	ERROR((msg, "ipc_inmsg: reply port already has a pending record"));
			if (dealloc_ior) mem_deallocobj((pointer_t)ior_ptr, MEM_IPCIOREC);
		}
		/*
		 * If the ipc_iorec was statically allocated we had better dynmically allocate one now.
		 */
		if (!dealloc_ior) {
			if ((ior_ptr = (ipc_iorec_ptr_t) mem_allocobj(MEM_IPCIOREC)) == 0) {
				panic("ipc_inmsg cannot get a new ipc_iorec");
			}
			else *ior_ptr = ior_data;
		}
		rp_ptr->portrec_current_ipcrec = (pointer_t)ior_ptr;
		lk_unlock(&rp_ptr->portrec_lock);
		/* rp_ptr LOCK -/- */
		break;

	default:
		ERROR((msg,"ipc_inmsg: unknown ipc_iorec type: %d\n", ior_ptr->type));
		PRINT_ERROR;
		break;
}

DEBUG0(debug.ipc_in,0,2008);
RETURN(IPC_SUCCESS);

END


/*
 * ipc_in_cancel --
 *
 * Cleanup after a message accepted by a ipc_inprobe has to be cancelled.
 *
 * Parameters:
 *
 * ipcid: ID generated by ipc_inprobe
 *
 * reason: code indicating why the message has to be cancelled
 *
 * Results:
 *
 * IPC_SUCCESS
 *
 * Side effects:
 *
 * Deallocates the ipc_iorec and cleans up the destination port record.
 * May signal port_unblocked if appropriate.
 *
 * Design:
 *
 * Note:
 *
 */
PUBLIC int
ipc_in_cancel(IN ipcid, IN reason)
	int		ipcid;
	int		reason;
BEGIN("ipc_in_cancel")
	ipc_iorec_ptr_t	ior_ptr;
	port_rec_ptr_t	dp_ptr;

	DEBUG2(debug.ipc_in,0,2009,ipcid,reason);

	ior_ptr = (ipc_iorec_ptr_t) ipcid;
	dp_ptr = ior_ptr->in.dest_port_ptr;

	/*
	 * Take care of a pending cleanup for an RPC.
	 */
	if (ior_ptr->type == IPC_IOREC_TYPE_CLIENT) {
	    	(void)ipc_out_cleanup(ipcid, TR_FAILURE);
	}

	/*
	 * Take care of the ipc_iorec.
	 */
	mem_deallocobj((pointer_t) ior_ptr, MEM_IPCIOREC);

	/*
	 * Take care of the destination port, and the people waiting for it.
	 */
	lk_lock(&dp_ptr->portrec_lock,PERM_READWRITE,TRUE);
	/* dp_ptr LOCK RW/RW */
	dp_ptr->portrec_info &= ~(PORT_INFO_BLOCKED | PORT_INFO_PROBED);
	dp_ptr->portrec_current_ipcrec = 0;
	if (dp_ptr->portrec_block_queue) {
		/*
		 * Signal port_unblocked to network servers on blocked queue. 
		 */
		ipc_msg_accepted(dp_ptr);
	}
	lk_unlock(&dp_ptr->portrec_lock);
	/* dp_ptr LOCK -/- */

RETURN(IPC_SUCCESS);

END


/*
 * ipc_in_init -- 
 *
 * Parameters: 
 *
 * none 
 *
 * Results: 
 *
 * none
 *
 * Side effects: 
 *
 * Initializes the dispatcher switch with the IPC dispatching functions.
 *
 * Note: 
 *
 */
PUBLIC void ipc_in_init()
BEGIN("ipc_in_init")

    dispatcher_switch[DISPE_IPC_MSG].disp_indata = ipc_inmsg;
    dispatcher_switch[DISPE_IPC_MSG].disp_inprobe = ipc_inprobe;
    dispatcher_switch[DISPE_IPC_UNBLOCK].disp_indata_simple = ipc_in_unblock;

    RET;

END
