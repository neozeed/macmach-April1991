#include "mach_interface.h"
#include <mach/message.h>
#include <mach/mach_types.h>
#include <mach/mig_errors.h>
#include <mach/msg_type.h>
#if	!defined(KERNEL) && !defined(MIG_NO_STRINGS)
#include <strings.h>
#endif
/* LINTLIBRARY */

extern port_t mig_get_reply_port();
extern void mig_dealloc_reply_port();

#ifndef	mig_internal
#define	mig_internal	static
#endif

#ifndef	TypeCheck
#define	TypeCheck 1
#endif

#ifndef	UseExternRCSId
#ifdef	hc
#define	UseExternRCSId		1
#endif
#endif

#ifndef	UseStaticMsgType
#if	!defined(hc) || defined(__STDC__)
#define	UseStaticMsgType	1
#endif
#endif

#define msg_request_port	msg_remote_port
#define msg_reply_port		msg_local_port


/* Routine netipc_listen */
mig_external kern_return_t netipc_listen
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int src_addr,
	int dst_addr,
	int src_port,
	int dst_port,
	int protocol,
	port_t ipc_port
)
#else
	(request_port, src_addr, dst_addr, src_port, dst_port, protocol, ipc_port)
	port_t request_port;
	int src_addr;
	int dst_addr;
	int src_port;
	int dst_port;
	int protocol;
	port_t ipc_port;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t src_addrType;
		int src_addr;
		msg_type_t dst_addrType;
		int dst_addr;
		msg_type_t src_portType;
		int src_port;
		msg_type_t dst_portType;
		int dst_port;
		msg_type_t protocolType;
		int protocol;
		msg_type_t ipc_portType;
		port_t ipc_port;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	union {
		Request In;
		Reply Out;
	} Mess;

	register Request *InP = &Mess.In;
	register Reply *OutP = &Mess.Out;

	msg_return_t msg_result;

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size = 72;

#if	UseStaticMsgType
	static msg_type_t src_addrType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t dst_addrType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t src_portType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t dst_portType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t protocolType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t ipc_portType = {
		/* msg_type_name = */		MSG_TYPE_PORT,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t RetCodeCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->src_addrType = src_addrType;
#else	UseStaticMsgType
	InP->src_addrType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->src_addrType.msg_type_size = 32;
	InP->src_addrType.msg_type_number = 1;
	InP->src_addrType.msg_type_inline = TRUE;
	InP->src_addrType.msg_type_longform = FALSE;
	InP->src_addrType.msg_type_deallocate = FALSE;
	InP->src_addrType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->src_addr /* src_addr */ = /* src_addr */ src_addr;

#if	UseStaticMsgType
	InP->dst_addrType = dst_addrType;
#else	UseStaticMsgType
	InP->dst_addrType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->dst_addrType.msg_type_size = 32;
	InP->dst_addrType.msg_type_number = 1;
	InP->dst_addrType.msg_type_inline = TRUE;
	InP->dst_addrType.msg_type_longform = FALSE;
	InP->dst_addrType.msg_type_deallocate = FALSE;
	InP->dst_addrType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->dst_addr /* dst_addr */ = /* dst_addr */ dst_addr;

#if	UseStaticMsgType
	InP->src_portType = src_portType;
#else	UseStaticMsgType
	InP->src_portType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->src_portType.msg_type_size = 32;
	InP->src_portType.msg_type_number = 1;
	InP->src_portType.msg_type_inline = TRUE;
	InP->src_portType.msg_type_longform = FALSE;
	InP->src_portType.msg_type_deallocate = FALSE;
	InP->src_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->src_port /* src_port */ = /* src_port */ src_port;

#if	UseStaticMsgType
	InP->dst_portType = dst_portType;
#else	UseStaticMsgType
	InP->dst_portType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->dst_portType.msg_type_size = 32;
	InP->dst_portType.msg_type_number = 1;
	InP->dst_portType.msg_type_inline = TRUE;
	InP->dst_portType.msg_type_longform = FALSE;
	InP->dst_portType.msg_type_deallocate = FALSE;
	InP->dst_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->dst_port /* dst_port */ = /* dst_port */ dst_port;

#if	UseStaticMsgType
	InP->protocolType = protocolType;
#else	UseStaticMsgType
	InP->protocolType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->protocolType.msg_type_size = 32;
	InP->protocolType.msg_type_number = 1;
	InP->protocolType.msg_type_inline = TRUE;
	InP->protocolType.msg_type_longform = FALSE;
	InP->protocolType.msg_type_deallocate = FALSE;
	InP->protocolType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->protocol /* protocol */ = /* protocol */ protocol;

#if	UseStaticMsgType
	InP->ipc_portType = ipc_portType;
#else	UseStaticMsgType
	InP->ipc_portType.msg_type_name = MSG_TYPE_PORT;
	InP->ipc_portType.msg_type_size = 32;
	InP->ipc_portType.msg_type_number = 1;
	InP->ipc_portType.msg_type_inline = TRUE;
	InP->ipc_portType.msg_type_longform = FALSE;
	InP->ipc_portType.msg_type_deallocate = FALSE;
	InP->ipc_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->ipc_port /* ipc_port */ = /* ipc_port */ ipc_port;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2036;

	msg_result = msg_rpc(&InP->Head, MSG_OPTION_NONE, sizeof(Reply), 0, 0);
	if (msg_result != RPC_SUCCESS) {
		if (msg_result == RCV_INVALID_PORT)
			mig_dealloc_reply_port();
		return msg_result;
	}

#if	TypeCheck
	msg_size = OutP->Head.msg_size;
	msg_simple = OutP->Head.msg_simple;
#endif	TypeCheck

	if (OutP->Head.msg_id != 2136)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 32) || (msg_simple != TRUE)) &&
	    ((msg_size != sizeof(death_pill_t)) ||
	     (msg_simple != TRUE) ||
	     (OutP->RetCode == KERN_SUCCESS)))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->RetCodeType != * (int *) &RetCodeCheck)
#else	UseStaticMsgType
	if ((OutP->RetCodeType.msg_type_inline != TRUE) ||
	    (OutP->RetCodeType.msg_type_longform != FALSE) ||
	    (OutP->RetCodeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->RetCodeType.msg_type_number != 1) ||
	    (OutP->RetCodeType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	if (OutP->RetCode != KERN_SUCCESS)
		return OutP->RetCode;

	return OutP->RetCode;
}
