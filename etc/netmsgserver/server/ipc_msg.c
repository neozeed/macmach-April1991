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
 * $Log:	ipc_msg.c,v $
 * Revision 1.1  90/02/19  21:51:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.15  89/05/02  11:09:37  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.14  89/04/24  20:39:17  dpj
 * 	Changed a few variables from short to long in ipc_in_scan_swap(),
 * 	to track changes done earlier in ipc_in_scan(). This was causing
 * 	mysterious failures for Camelot.
 * 	Make sure to zero the correct fields in the netmsg_header when there
 * 	is no OOL segment.
 * 	[89/04/19  23:25:46  dpj]
 * 
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:53:07  dpj]
 * 	Added debugging output for ipc_outmsg.
 * 	[89/04/19  15:18:06  dpj]
 * 
 * 09-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Conditionalize encrypt code.
 *
 * 20-May-88  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Added \ escapes before newline in invocations of
 *	macros so that it would compile on the MMAX
 *
 *  2-Apr-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed print level in a DEBUG statement.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  5-Mar-88  Robert Sansom (rds) at Carnegie Mellon University
 *	Use one npd sbuf for the actual dictionary and for GC.
 *	GC assumes that all segs in the sbuf are of size PO_NPD_SEG_SIZE.
 *	Deallocate temporary NPD sbuf in ipc_in_scan and ipc_in_scan_swap.
 *
 * 28-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed computation of end_scan_ptr to work correctly with message
 *	sizes not multiple of 4 bytes.
 *
 * 19-Jan-88  Robert Sansom (rds) at Carnegie Mellon University
 *	Use libmach vm_deallocate now that it works for multi-threaded 
 *	applications.
 *
 * 18-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for byte-swapping of ipc sequence number.
 *	Added Camelot support.
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Started.
 *
 */
/*
 * ipc_msg.c 
 *
 * Translation of incoming and outgoing IPC messages. 
 *
 *
 */
#ifndef	lint
char ipc_msg_rcsid[] = "$ Header: ipc_msg.c,v 1.15 89/05/02 11:09:37 dpj Exp $";
#endif not lint



#include	"netmsg.h"
#include	"nm_defs.h"

#include	<mach.h>
#include	<mach/msg_type.h>
#include	<mach/message.h>
#include	<mach/machine/vm_param.h>

#include	"config.h"
#include	"debug.h"
#include	"ipc_hdr.h"
#include	"ipc_rec.h"
#include	"ipc_swap.h"
#include	"ipc_internal.h"
#include	"mem.h"
#include	"crypt.h"
#include	"port_defs.h"
#include	"portrec.h"
#include	"portops.h"

#if	RPCMOD
#else	RPCMOD
 ! You lose !
#endif	RPCMOD

extern unsigned long	ipc_sequence_number;

#if	CAMELOT
#include	"../camelot/cam.h"
extern struct mutex	camelot_lock;
#endif	CAMELOT

/*
 * ipc_in_scan -- 
 *
 * Assemble an incoming message and translate ports and ool pointers. 
 *
 * Parameters: 
 *
 * ir_ptr: pointer to an ipc_inrec for the message to assemble, with a properly
 * allocated assembly buffer. 
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
 * Part of this code is repeated in ipc_in_scan_swap. If you change one, change
 * the other. 
 *
 */
PRIVATE void
ipc_in_scan(IN ir_ptr, IN crypt_level)
	ipc_inrec_t	*ir_ptr;
	int             crypt_level;
BEGIN("ipc_in_scan")
	ipc_netmsg_hdr_t *nmh_ptr;
	sbuf_pos_t      from;	/* current position in the incoming data */
	pointer_t       to_ptr;	/* current position in the assembly buffer */
	pointer_t       npd_ptr;/* Network Port Dictionary */
	pointer_t       npd_cur;/* current location in npd */
	msg_type_long_t *scan_ptr;	/* pointer for scanning the msg */
	msg_type_long_t *end_scan_ptr;	/* pointer for end of msg */

	nmh_ptr = ir_ptr->netmsg_hdr_ptr;
	SBUF_SEEK(*ir_ptr->msg, from, sizeof(ipc_netmsg_hdr_t));
	to_ptr = (pointer_t) ir_ptr->assem_buff;

	/*
	 * Get the Network Port Dictionary if appropriate. 
	 */
	if (!(nmh_ptr->info & IPC_INFO_SIMPLE)) {
		if (nmh_ptr->npd_size) {
			MEM_ALLOC(npd_ptr,pointer_t,nmh_ptr->npd_size, FALSE);
			npd_cur = npd_ptr;
			SBUF_EXTRACT(*ir_ptr->msg, from, npd_ptr, nmh_ptr->npd_size);
			npd_ptr = npd_cur;
		}
	}
	/*
	 * Get the inline section. 
	 */
	SBUF_EXTRACT(*ir_ptr->msg, from, to_ptr, nmh_ptr->inline_size);

#define	ADDSCAN(p,o)	(((char *)p + o))
	scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff, sizeof(msg_header_t));
#if	LongAlign
	end_scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff,
					(((nmh_ptr->inline_size + 3) >> 2) << 2));
#else	LongAlign
	end_scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff,
					(((nmh_ptr->inline_size + 1) >> 1) << 1));
#endif	LongAlign

	/*
	 * Scan and translate. 
	 */
	while (scan_ptr < end_scan_ptr) {
		unsigned long   tn;	/* type of current data */
		unsigned long   elts;	/* number of elements in current
					 * descriptor */
		unsigned long   len;	/* length of current data */
		pointer_t       dptr;	/* pointer to current data */
#ifdef lint
		msg_type_t      mth;	/* current msg_type_header */
#else lint
		register msg_type_t mth;	/* current msg_type_header */
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

		DEBUG6(debug.ipc_in, 3, 2530, mth.msg_type_inline, mth.msg_type_deallocate,
		       mth.msg_type_longform, tn, len, elts);

		/*
		 * Enter out-of-line sections if necessary, and advance to
		 * the next descriptor. 
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
		 * This is a good place to handle imaginary data
		 * (copy-on-reference). 
		 */

		/*
		 * Translate ports if needed 
		 */
		if (MSG_TYPE_PORT_ANY(tn)) {
			int             i;	/* index for iterating over
						 * elements */
			int             npd_entry_size;	/* size of new NPD entry */

			for (i = 1; i <= elts; i++) {
				npd_entry_size = po_translate_nport_rights(ir_ptr->from, npd_cur, crypt_level,
					      (port_t *) dptr, (int *) &tn);
				npd_cur = (pointer_t) (((char *) npd_cur) + npd_entry_size);
				dptr = (pointer_t) ADDSCAN(dptr, sizeof(port_t));
			}
		}
	}

#undef	ADDSCAN

	MEM_DEALLOC(npd_ptr, (int)nmh_ptr->npd_size);
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
 * Part of this code is repeated in ipc_in_scan. If you change one, change the
 * other. 
 *
 */
PRIVATE void
ipc_in_scan_swap(IN ir_ptr, IN crypt_level)
	ipc_inrec_t	*ir_ptr;
	int             crypt_level;
BEGIN("ipc_in_scan_swap")
	ipc_netmsg_hdr_t *nmh_ptr;
	sbuf_pos_t      from;	/* current position in the incoming data */
	pointer_t       to_ptr;	/* current position in the assembly buffer */
	pointer_t       npd_ptr;/* Network Port Dictionary */
	pointer_t       npd_cur;/* current location in npd */
	msg_type_long_t *scan_ptr;	/* pointer for scanning the msg */
	msg_type_long_t *end_scan_ptr;	/* pointer for end of msg */

	nmh_ptr = ir_ptr->netmsg_hdr_ptr;
	SBUF_SEEK(*ir_ptr->msg, from, sizeof(ipc_netmsg_hdr_t));
	to_ptr = (pointer_t) ir_ptr->assem_buff;

	/*
	 * Get the Network Port Dictionary if appropriate. 
	 */
	if (!(nmh_ptr->info & IPC_INFO_SIMPLE)) {
		if (nmh_ptr->npd_size) {
			MEM_ALLOC(npd_ptr,pointer_t,nmh_ptr->npd_size, FALSE);
			npd_cur = npd_ptr;
			SBUF_EXTRACT(*ir_ptr->msg, from, npd_ptr, nmh_ptr->npd_size);
			npd_ptr = npd_cur;
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
#if	LongAlign
	end_scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff,
					(((nmh_ptr->inline_size + 3) >> 2) << 2));
#else	LongAlign
	end_scan_ptr = (msg_type_long_t *) ADDSCAN(ir_ptr->assem_buff,
					(((nmh_ptr->inline_size + 1) >> 1) << 1));
#endif	LongAlign

	while (scan_ptr < end_scan_ptr) {
		unsigned long  tn;	/* type of current data */
		unsigned long  ts;	/* type size of current data */
		unsigned long   elts;	/* number of elements in current
					 * descriptor */
		unsigned long   len;	/* length of current data */
		pointer_t       dptr;	/* pointer to current data */
		msg_type_t      mth;	/* current msg_type_header */
		swap_msg_type_t rmth;	/* reverse mth */
		SWAP_DECLS;	/* declarations for swapping macros */

		 /* XXX */ *(long *) &mth = 0;	/* make sure the dealloc bit
						 * stays 0 */
		 /* XXX */ *(long *) &rmth = *(long *) &(scan_ptr->msg_type_header);
		if (mth.msg_type_longform = rmth.msg_type_longform) {
			tn = SWAP_SHORT(scan_ptr->msg_type_long_name, scan_ptr->msg_type_long_name);
			elts = SWAP_LONG(scan_ptr->msg_type_long_number, scan_ptr->msg_type_long_number);
			ts = SWAP_SHORT(scan_ptr->msg_type_long_size, scan_ptr->msg_type_long_size);
#if	LongAlign
			/*
			 * len = (((SWAP_SHORT(scan_ptr->msg_type_long_size,
			 * scan_ptr->msg_type_long_size) elts) + 31) >> 5) <<
			 * 2; 
			 */
			len = (((ts * elts) + 31) >> 5) << 2;
#else	LongAlign
			/*
			 * len = (((SWAP_SHORT(scan_ptr->msg_type_long_size,
			 * scan_ptr->msg_type_long_size) elts) + 15) >> 4) <<
			 * 1; 
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

		DEBUG6(debug.ipc_in, 0, 2529, rmth.msg_type_inline, mth.msg_type_deallocate,
		       mth.msg_type_longform, tn, len, elts);

		/*
		 * Byte-swap the data. Enter out-of-line sections if
		 * necessary, and advance to the next descriptor. 
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
				(void) swap_short_sbuf(ir_ptr->msg, &from, (unsigned short **) &to_ptr, (int) elts);
				break;
			case MSG_TYPE_INTEGER_32:
				(void) swap_long_sbuf(ir_ptr->msg, &from, (unsigned long **) &to_ptr, (int) elts);
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
		 * This is a good place to handle imaginary data
		 * (copy-on-reference). 
		 */

		/*
		 * Translate ports if needed 
		 */
		if (MSG_TYPE_PORT_ANY(tn)) {
			int             i;	/* index for iterating over
						 * elements */
			int             npd_entry_size;	/* size of new NPD entry */

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

	MEM_DEALLOC(npd_ptr, (int)nmh_ptr->npd_size);
	RET;

END



/*
 * ipc_inmsg -- 
 *
 * Translate an incoming IPC message from the transport sbuf into a Mach IPC
 * message. 
 *
 * Parameters: 
 *
 * ipc_ptr: IPC record for this message transaction. 
 * data_ptr: sbuf containing the incoming message.
 * nmh_ptr: pointer to netmsg header in the incoming sbuf.
 * from: the address of the network server where the message originated. 
 * crypt_level: encryption level for this message.
 * do_swap: whether or not byte-swapping is necessary.
 *
 * Results: 
 *
 * none. 
 *
 * Side effects: 
 *
 * Allocates an assembly area, fills it and places it in the IPC record. 
 *
 * Note: 
 *
 * Must clear the MSG_TYPE_RPC bit.
 *
 * The ipc_seq_no in the netmsg header is already byte-swapped on entry.
 *
 */
/* ARGSUSED */
PUBLIC void
ipc_inmsg(ipc_ptr, data_ptr, nmh_ptr, from, crypt_level,do_swap)
	ipc_rec_ptr_t		ipc_ptr;
	sbuf_ptr_t		data_ptr;
	ipc_netmsg_hdr_t	*nmh_ptr;
	netaddr_t		from;
	int			crypt_level;
	boolean_t		do_swap;
BEGIN("ipc_inmsg")
	ipc_inrec_t		*ir_ptr;	/* ipc_inrec for this message */
	sbuf_pos_t		cur_pos;	/* current position in the msg */


	DEBUG5(debug.ipc_in,0,2560,ipc_ptr,data_ptr,nmh_ptr,from,crypt_level);

	/*
	 * Set up basic information.
	 */
	ir_ptr = &ipc_ptr->in;
	ir_ptr->netmsg_hdr_ptr = nmh_ptr;
	ir_ptr->from = from;
	ir_ptr->msg = data_ptr;
	ir_ptr->ipc_seq_no = nmh_ptr->ipc_seq_no;

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

	DEBUG5(debug.ipc_out, 0, 2586,
				nmh_ptr->info,
				nmh_ptr->npd_size,
				nmh_ptr->inline_size,
				nmh_ptr->ool_size,
				nmh_ptr->ool_num);

	SBUF_SEEK(*data_ptr, cur_pos, sizeof(ipc_netmsg_hdr_t));

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
			ir_ptr->assem_buff = (msg_header_t *) cur_pos.data_ptr;
			ir_ptr->assem_len = 0;
			ir_ptr->assem_type = IPC_REC_ASSEM_PKT;
		} else {
			/*
			 * For a simple message, a MEM_ASSEMBUFF is surely
			 * enough. Get one and copy the message into it. 
			 */
			MEM_ALLOCOBJ(ir_ptr->assem_buff,msg_header_t *,MEM_ASSEMBUFF);
			ir_ptr->assem_len = 0;
			ir_ptr->assem_type = IPC_REC_ASSEM_OBJ;
		}
	} else {
		/*
		 * For a complex message, we must take care to have an
		 * assembly buffer big enough to contain all ool sections,
		 * with the proper page alignment. 
		 */
		int             assem_len;

		assem_len = (nmh_ptr->ool_num * vm_page_size) + nmh_ptr->inline_size + nmh_ptr->ool_size;
		if (assem_len > IPC_ASSEM_SIZE) {
			MEM_ALLOC(ir_ptr->assem_buff,msg_header_t *,assem_len, FALSE);
			ir_ptr->assem_len = assem_len;
			ir_ptr->assem_type = IPC_REC_ASSEM_MEMALLOC;
		} else {
			MEM_ALLOCOBJ(ir_ptr->assem_buff,msg_header_t *,MEM_ASSEMBUFF);
			if (ir_ptr->assem_buff == 0) {
				panic("ipc_inmsg cannot get an assembly buffer");
			}
			ir_ptr->assem_len = 0;
			ir_ptr->assem_type = IPC_REC_ASSEM_OBJ;
		}
	}
	DEBUG3(debug.ipc_in,0,2506,ir_ptr->assem_type,ir_ptr->assem_len,
							ir_ptr->assem_buff);

	/*
	 * Assemble the message and translate it if necessary. 
	 *
	 * Note: if the sbuf segment is used as assembly buffer, there is
	 * nothing to do. 
	 */
	if (ir_ptr->assem_type != IPC_REC_ASSEM_PKT) {
		if (do_swap) {
			ipc_in_scan_swap(ir_ptr, crypt_level);
		} else {
			if (nmh_ptr->info & IPC_INFO_SIMPLE) {
				pointer_t       to_ptr;

				to_ptr = (pointer_t) ir_ptr->assem_buff;
				SBUF_EXTRACT(*data_ptr, cur_pos, to_ptr, nmh_ptr->inline_size);
			} else {
				ipc_in_scan(ir_ptr, crypt_level);
			}
		}
	}

	/*
	 * Set the msg_type field according to the crypt_level of the network
	 * message. 
	 */
#if	USE_CRYPT
	if (crypt_level != CRYPT_DONT_ENCRYPT)
		ir_ptr->assem_buff->msg_type |= MSG_TYPE_ENCRYPTED;
#endif	USE_CRYPT

	/*
	 * We can never deliver a message with the RPC bit set,
	 * because that could get us in endless troubles with forwarding.
	 */
	ir_ptr->assem_buff->msg_type &= ~ MSG_TYPE_RPC;

	DEBUG4(debug.ipc_in, 0, 2561,ir_ptr->assem_buff->msg_simple,
					ir_ptr->assem_buff->msg_type,
					ir_ptr->assem_buff->msg_id,
					ir_ptr->assem_buff->msg_size);

	RET;

END



/*
 * ipc_outmsg --
 *
 * Translate
 *
 * Parameters: 
 *
 * ipc_ptr: pointer to an ipc_rec for the message to transmit.
 * msg_ptr: pointer to a buffer containing the inline section of the msg.
 * destination:	network address of the destination for this msg.
 * crypt_level:	the encryption level of the msg.
 *
 * Results: 
 *
 * none 
 *
 * Side effects: 
 *
 * Initializes the ipc_outrec and translates the outgoing message into internal
 * format (sbuf). Translates the ports in the message if necessary. Arranges
 * for everything to be correctly garbage-collected when the ipc_rec is
 * destroyed (including the message buffer itself). 
 *
 * Note: 
 *
 * This procedure does everything that can be done without using the port record
 * for the destination of the message. It must be called only once for each
 * message, independent of the number of retransmissions this message needs.
 *
 */
PUBLIC
void
ipc_outmsg(ipc_ptr, msg_ptr, destination, crypt_level)
	ipc_rec_ptr_t		ipc_ptr;
	msg_header_t		*msg_ptr;
	netaddr_t		destination;
	int			crypt_level;
BEGIN("ipc_outmsg")
	ipc_outrec_t		*or_ptr;

	DEBUG4(debug.ipc_out,0,2510,ipc_ptr,msg_ptr,destination,crypt_level);

	or_ptr = &ipc_ptr->out;

	/*
	 * Fill in some important fields in the ipc_outrec. 
	 */
	or_ptr->ipcbuff = msg_ptr;
	or_ptr->dest = destination;
	or_ptr->crypt_level = crypt_level;
	or_ptr->ool_exists = FALSE;
	or_ptr->npd_exists = FALSE;

	/*
	 * Prepare the IPC netmsg header 
	 */
	or_ptr->netmsg_hdr.disp_hdr.disp_type = htons(DISP_IPC_MSG);
	or_ptr->netmsg_hdr.disp_hdr.src_format = conf_own_format;
	or_ptr->netmsg_hdr.info = 0;
	or_ptr->netmsg_hdr.ipc_seq_no = ipc_sequence_number++;
	or_ptr->netmsg_hdr.inline_size = msg_ptr->msg_size;

	/*
	 * This is a good place to do whatever the Camelot communications
	 * manager wants to do. 
	 */
#if	CAMELOT
	if (Cam_Message(Cam_MsgHeader(msg_ptr))) {
		mutex_lock(&camelot_lock);
		if (!Cam_Transmit(Cam_MsgHeader(msg_ptr),destination)) {
			ERROR((msg, "ipc_outmsg,Cam_Transmit returned FALSE"));
		}
		mutex_unlock(&camelot_lock);
	}
#endif	CAMELOT

	if (msg_ptr->msg_simple) {
		/*
		 * For a simple message, we have nothing much to do. 
		 */
		or_ptr->netmsg_hdr.info |= IPC_INFO_SIMPLE;
		or_ptr->netmsg_hdr.npd_size = 0;
		or_ptr->netmsg_hdr.ool_size = 0;
		or_ptr->netmsg_hdr.ool_num = 0;
		or_ptr->msg.end = or_ptr->msg.segs = &or_ptr->segs[0];
		or_ptr->msg.free = or_ptr->msg.size = IPC_OUT_NUM_SEGS;
		SBUF_APPEND(or_ptr->msg, 0, 0);	/* empty spare segment */
		SBUF_APPEND(or_ptr->msg, (pointer_t) & or_ptr->netmsg_hdr,
			    sizeof(ipc_netmsg_hdr_t));
		SBUF_APPEND(or_ptr->msg, (pointer_t) msg_ptr, msg_ptr->msg_size);
	} else {
		/*
		 * The message is not simple. We must scan it to find the
		 * out-of-line sections, and build a Network Port Dictionary
		 * for the embedded ports. 
		 */

		/*
		 * Variables for scanning the message 
		 */
		msg_type_long_t *scan_ptr;	/* pointer for scanning the
						 * msg */
		msg_type_long_t *end_scan_ptr;	/* pointer for end of msg */

		/*
		 * Variables for keeping track of out-of-line sections 
		 */
		unsigned long   ool_size = 0;	/* total size of ool stuff */
		unsigned long   ool_num = 0;	/* number of ool sections */

		/*
		 * Variables for building the Network Port Dictionary 
		 */
		pointer_t       npd_seg_start = 0;	/* start of current NPD segment */
		pointer_t       npd_seg_next = 0;	/* current position in NPD segment */
		int             npd_seg_free = 0;	/* bytes free in current NPD segment */
		int             npd_size = 0;		/* total size of NPD */

		DEBUG0(debug.ipc_out, 0, 2511);

		/*
		 * Scan the message by iterating over each descriptor in the
		 * inline section 
		 */
#define	ADDSCAN(p,o)	(((char *)p + o))
		scan_ptr = end_scan_ptr = (msg_type_long_t *) msg_ptr;
		scan_ptr = (msg_type_long_t *) ADDSCAN(msg_ptr, sizeof(msg_header_t));
		end_scan_ptr = (msg_type_long_t *) ADDSCAN(msg_ptr, msg_ptr->msg_size);

		while (scan_ptr < end_scan_ptr) {
			unsigned long   tn;	/* type of current data */
			unsigned long   elts;	/* number of elements in
						 * current descriptor */
			unsigned long   len;	/* length of current data */
			pointer_t       dptr;	/* pointer to current data */
#ifdef lint
			msg_type_t      mth;	/* current msg_type_header */
#else lint
			register msg_type_t mth;	/* current
							 * msg_type_header */
#endif lint

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

			DEBUG6(debug.ipc_out, 0, 2584, mth.msg_type_inline, mth.msg_type_deallocate,
		       mth.msg_type_longform, tn, len, elts);

			/*
			 * This is a good place to handle imaginary data
			 * (copy-on-reference). 
			 */

			/*
			 * Enter out-of-line sections in the sbufs if
			 * necessary and advance to the next descriptor. 
			 */
			if (mth.msg_type_inline) {
				scan_ptr = (msg_type_long_t *) ADDSCAN(dptr, len);
			} else {
				if (!or_ptr->ool_exists) {
					SBUF_SEG_INIT(or_ptr->ool, &or_ptr->ool_seg);
					or_ptr->ool_exists = TRUE;
				} else if (or_ptr->ool.size == 1) {
					/*
					 * Handle special case of growing a
					 * single inline sbuf segment. 
					 */
					sbuf_seg_ptr_t  new_segs;
					MEM_ALLOC(new_segs,sbuf_seg_ptr_t,
					10 * sizeof(struct sbuf_seg),FALSE);
					new_segs[0] = or_ptr->ool_seg;
					or_ptr->ool.end = or_ptr->ool.segs = new_segs;
					or_ptr->ool.end++;
					or_ptr->ool.size = 10;
					or_ptr->ool.free = 9;
				}
				scan_ptr = (msg_type_long_t *) ADDSCAN(dptr, sizeof(char *));
				dptr = *((pointer_t *) dptr);
				SBUF_APPEND(or_ptr->ool, dptr, len);
				ool_size += len;
				ool_num++;
			}

			/*
			 * Translate ports if needed. 
			 */
			if (MSG_TYPE_PORT_ANY(tn)) {
				int             i;		/* index for iterating over elements */
				int             npd_entry_size;	/* size of new NPD entry */

				if (!or_ptr->npd_exists) {
					SBUF_INIT(or_ptr->npd, 10);
					or_ptr->npd_exists = TRUE;
				}
				for (i = 1; i <= elts; i++) {
					/*
					 * Make sure that there is enough space in
					 * the NPD to put a new entry. If necessary,
					 * move the current segment to the sbufs and
					 * allocate a new one. 
					 */
					if (npd_seg_free < PO_MAX_NPD_ENTRY_SIZE) {
						if (npd_seg_start) {
							/*
							 * Only do the move if there
							 * is something to move. 
							 */
							SBUF_APPEND(or_ptr->npd, npd_seg_start,
								PO_NPD_SEG_SIZE - npd_seg_free);
						}
						MEM_ALLOC(npd_seg_start,
						pointer_t,PO_NPD_SEG_SIZE, FALSE);
						npd_seg_free = PO_NPD_SEG_SIZE;
						npd_seg_next = npd_seg_start;
					}
					npd_entry_size = po_translate_lport_rights((int)ipc_ptr,
							*(port_t *)dptr,
							(int)tn, crypt_level, destination,
							(pointer_t)npd_seg_next);
					npd_seg_free -= npd_entry_size;
					npd_size += npd_entry_size;
					npd_seg_next = (pointer_t)(((char *) npd_seg_next) + npd_entry_size);
					dptr = (pointer_t) ADDSCAN(dptr, sizeof(port_t));
				}
			}
		}

		/*
		 * The message has been completely scanned. Enter the last
		 * sbuf of the NPD and finish the netmsg header. 
		 */
		if (npd_seg_start) {
			SBUF_APPEND(or_ptr->npd, npd_seg_start, PO_NPD_SEG_SIZE - npd_seg_free);
		}
		or_ptr->netmsg_hdr.npd_size = npd_size;
		or_ptr->netmsg_hdr.ool_size = ool_size;
		or_ptr->netmsg_hdr.ool_num = ool_num;


		/*
		 * Assemble the msg to transmit all the constituent parts. 
		 */
		{
			int             num_segs = 3;	/* Spare segment, netmsg header, inline message. */
			if (or_ptr->npd_exists) num_segs += or_ptr->npd.size - or_ptr->npd.free;
			if (or_ptr->ool_exists) num_segs += or_ptr->ool.size - or_ptr->ool.free;
			if (num_segs > IPC_OUT_NUM_SEGS) {
				SBUF_INIT(or_ptr->msg, num_segs);
			} else {
				/*
				 * Use the statically allocated sbuf
				 * segments. 
				 */
				or_ptr->msg.end = or_ptr->msg.segs = &or_ptr->segs[0];
				or_ptr->msg.free = or_ptr->msg.size = IPC_OUT_NUM_SEGS;
			}
		}
		SBUF_APPEND(or_ptr->msg, 0, 0);	/* empty spare segment */
		SBUF_APPEND(or_ptr->msg, (pointer_t) & or_ptr->netmsg_hdr, sizeof(ipc_netmsg_hdr_t));
		if (or_ptr->npd_exists) SBUF_SB_APPEND(or_ptr->msg, or_ptr->npd);
		SBUF_APPEND(or_ptr->msg, (pointer_t) msg_ptr, msg_ptr->msg_size);
		if (or_ptr->ool_exists) SBUF_SB_APPEND(or_ptr->msg, or_ptr->ool);
	}

#undef	ADDSCAN

	DEBUG6(debug.ipc_out, 0, 2585, msg_ptr->msg_id,
				or_ptr->netmsg_hdr.info,
				or_ptr->netmsg_hdr.npd_size,
				or_ptr->netmsg_hdr.inline_size,
				or_ptr->netmsg_hdr.ool_size,
				or_ptr->netmsg_hdr.ool_num);

	RET;

END



/*
 * ipc_out_gc -- 
 *
 * Parameters: 
 *
 * ipc_ptr: pointer to ipc_rec to destroy 
 *
 * Results: 
 *
 * Side effects: 
 *
 * Deallocates the ipc_rec and all space associated with the outgoing message in
 * the IPC module: IPC buffer, out-of-line sections, headers, Network Port
 * Dictionary, and dynamically allocated sbuf (if it exists).
 *
 * Note:
 *
 * If the inrec section of the ipc_rec has been used, it must be cleaned up
 * before this procedure is called (see ipc_in_gc).
 *
 */
PUBLIC
ipc_out_gc(ipc_ptr)
	ipc_rec_ptr_t		ipc_ptr;
BEGIN("ipc_out_gc")
	sbuf_seg_ptr_t		cursb_ptr;	/* pointer in sbuf to GC */
	sbuf_seg_ptr_t		endsb_ptr;	/* last sbuf_seg to GC */
	ipc_outrec_t		*or_ptr;

	DEBUG2(debug.ipc_out, 3, 2512, ipc_ptr, ipc_ptr->reply_port_ptr);

	or_ptr = &ipc_ptr->out;

#if	CAMELOT
	if (Cam_Message(Cam_MsgHeader(or_ptr->ipcbuff))) {
		mutex_lock(&camelot_lock);
		Cam_Cleanup(Cam_MsgHeader(or_ptr->ipcbuff));
		mutex_unlock(&camelot_lock);
		/*
		 * Set the first ool segment to be null because Camelot freed it.
		 */
		or_ptr->ool.segs->p = 0;
		or_ptr->ool.segs->s = 0;
	}
#endif	CAMELOT

	MEM_DEALLOCOBJ(or_ptr->ipcbuff, MEM_IPCBUFF);

	if (or_ptr->ool_exists) {
		cursb_ptr = or_ptr->ool.segs;
		endsb_ptr = or_ptr->ool.end;
		while (cursb_ptr < endsb_ptr) {
			if (cursb_ptr->s != 0) {
				(void)vm_deallocate(task_self(), (vm_address_t) cursb_ptr->p,
							(vm_size_t) cursb_ptr->s);
			}
			cursb_ptr++;
		}
		if (or_ptr->ool.size > 1)
			MEM_DEALLOC((pointer_t) or_ptr->ool.segs, 
					(or_ptr->ool.size * sizeof(sbuf_seg_t)));
	}
	if (or_ptr->npd_exists) {
		cursb_ptr = or_ptr->npd.segs;
		endsb_ptr = or_ptr->npd.end;
		while (cursb_ptr < endsb_ptr) {
			if (cursb_ptr->s != 0) {
				MEM_DEALLOC(cursb_ptr->p, PO_NPD_SEG_SIZE);
			}
			cursb_ptr++;
		}
		MEM_DEALLOC((pointer_t) or_ptr->npd.segs, 
					(or_ptr->npd.size * sizeof(sbuf_seg_t)));
	}
	if (or_ptr->msg.size > IPC_OUT_NUM_SEGS) {
		/*
		 * Free the dynamically allocated sbuf. 
		 */
		SBUF_FREE(or_ptr->msg);
	}
	if (ipc_ptr->reply_port_ptr) {
    		lk_lock(&ipc_ptr->reply_port_ptr->portrec_lock,PERM_READWRITE,TRUE);
		pr_release(ipc_ptr->reply_port_ptr);
	}
	MEM_DEALLOCOBJ(ipc_ptr, MEM_IPCREC);

	RET;

END
