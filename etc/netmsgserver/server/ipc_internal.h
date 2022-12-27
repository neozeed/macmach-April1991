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
 * $Log:	ipc_internal.h,v $
 * Revision 1.1  90/02/19  21:51:25  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.7  89/05/02  11:09:32  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added MEM_IPCBUFF allocation definitions.
 *
 * 22-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed a memory leak in ipc_in_gc, for mem_alloc()'ed assembly buffers.
 *
 * 14-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new ipc_rec states and outgoing queue.
 *
 *  5-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 */
/*
 * ipc_internal.h
 *
 *
 * $ Header: ipc_internal.h,v 1.7 89/05/02 11:09:32 dpj Exp $
 *
 */

/*
 * Macros internal to the ipc module.
 */


#ifndef	_IPC_INTERNAL_
#define	_IPC_INTERNAL_

#include	"ipc.h"
#include	"mem.h"

/*
 * Macros to test ipc/rpc status.
 */
#define	awaiting_local_reply(pr)						\
	((pr->portrec_reply_ipcrec != 0) &&					\
		(((ipc_rec_ptr_t)pr->portrec_reply_ipcrec)->status ==		\
							IPC_REC_REPLY))

#define	awaiting_remote_reply(pr)						\
	((pr->portrec_reply_ipcrec != 0)					\
		&&								\
	(((ipc_rec_ptr_t)pr->portrec_reply_ipcrec)->type == IPC_REC_TYPE_CLIENT)\
		&&								\
	(((ipc_rec_ptr_t)pr->portrec_reply_ipcrec)->status == IPC_REC_ACTIVE))

/*
 * Macro to clean up the resources used for processing an incoming message.
 */
#define	ipc_in_gc(ipc_ptr) {							\
	switch ((ipc_ptr)->in.assem_type) {					\
		case IPC_REC_ASSEM_PKT:						\
			break;							\
		case IPC_REC_ASSEM_OBJ:						\
			MEM_DEALLOCOBJ(((ipc_ptr)->in.assem_buff),	\
				MEM_ASSEMBUFF);					\
			break;							\
		case IPC_REC_ASSEM_MEMALLOC:					\
			MEM_DEALLOC((pointer_t)((ipc_ptr)->in.assem_buff),	\
						(ipc_ptr)->in.assem_len);	\
			break;							\
		default:							\
			ERROR((msg,						\
				"ipc_in_gc: unknown type for assembly area: %d",\
				(ipc_ptr)->in.assem_type));			\
			break;							\
	}									\
}


/*
 * Extern declarations for this module only.
 */
extern void	ipc_inmsg();
extern void	ipc_outmsg();
extern void	ipc_client_abort();
extern void	ipc_server_abort();

/*
 * Memory management for MEM_IPCBUFF.
 */
extern mem_objrec_t		MEM_IPCBUFF;


#endif	_IPC_INTERNAL_
