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


/* Routine port_insert_send */
mig_external kern_return_t port_insert_send
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_t my_port,
	port_name_t his_name
)
#else
	(task, my_port, his_name)
	task_t task;
	port_t my_port;
	port_name_t his_name;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t my_portType;
		port_t my_port;
		msg_type_t his_nameType;
		port_name_t his_name;
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

	unsigned int msg_size = 40;

#if	UseStaticMsgType
	static msg_type_t my_portType = {
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
	static msg_type_t his_nameType = {
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
	InP->my_portType = my_portType;
#else	UseStaticMsgType
	InP->my_portType.msg_type_name = MSG_TYPE_PORT;
	InP->my_portType.msg_type_size = 32;
	InP->my_portType.msg_type_number = 1;
	InP->my_portType.msg_type_inline = TRUE;
	InP->my_portType.msg_type_longform = FALSE;
	InP->my_portType.msg_type_deallocate = FALSE;
	InP->my_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->my_port /* my_port */ = /* my_port */ my_port;

#if	UseStaticMsgType
	InP->his_nameType = his_nameType;
#else	UseStaticMsgType
	InP->his_nameType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->his_nameType.msg_type_size = 32;
	InP->his_nameType.msg_type_number = 1;
	InP->his_nameType.msg_type_inline = TRUE;
	InP->his_nameType.msg_type_longform = FALSE;
	InP->his_nameType.msg_type_deallocate = FALSE;
	InP->his_nameType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->his_name /* his_name */ = /* his_name */ his_name;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2085;

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

	if (OutP->Head.msg_id != 2185)
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