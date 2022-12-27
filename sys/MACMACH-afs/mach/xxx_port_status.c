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


/* Routine xxx_port_status */
mig_external kern_return_t xxx_port_status
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t my_port,
	boolean_t *enabled,
	int *num_msgs,
	int *backlog,
	boolean_t *ownership,
	boolean_t *receive_rights
)
#else
	(target_task, my_port, enabled, num_msgs, backlog, ownership, receive_rights)
	task_t target_task;
	port_t my_port;
	boolean_t *enabled;
	int *num_msgs;
	int *backlog;
	boolean_t *ownership;
	boolean_t *receive_rights;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t my_portType;
		port_t my_port;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t enabledType;
		boolean_t enabled;
		msg_type_t num_msgsType;
		int num_msgs;
		msg_type_t backlogType;
		int backlog;
		msg_type_t ownershipType;
		boolean_t ownership;
		msg_type_t receive_rightsType;
		boolean_t receive_rights;
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

	unsigned int msg_size = 32;

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
	static msg_type_t enabledCheck = {
		/* msg_type_name = */		MSG_TYPE_BOOLEAN,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t num_msgsCheck = {
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
	static msg_type_t backlogCheck = {
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
	static msg_type_t ownershipCheck = {
		/* msg_type_name = */		MSG_TYPE_BOOLEAN,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t receive_rightsCheck = {
		/* msg_type_name = */		MSG_TYPE_BOOLEAN,
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

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = target_task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2006;

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

	if (OutP->Head.msg_id != 2106)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 72) || (msg_simple != TRUE)) &&
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

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->enabledType != * (int *) &enabledCheck)
#else	UseStaticMsgType
	if ((OutP->enabledType.msg_type_inline != TRUE) ||
	    (OutP->enabledType.msg_type_longform != FALSE) ||
	    (OutP->enabledType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (OutP->enabledType.msg_type_number != 1) ||
	    (OutP->enabledType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*enabled /* enabled */ = /* *enabled */ OutP->enabled;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->num_msgsType != * (int *) &num_msgsCheck)
#else	UseStaticMsgType
	if ((OutP->num_msgsType.msg_type_inline != TRUE) ||
	    (OutP->num_msgsType.msg_type_longform != FALSE) ||
	    (OutP->num_msgsType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->num_msgsType.msg_type_number != 1) ||
	    (OutP->num_msgsType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*num_msgs /* num_msgs */ = /* *num_msgs */ OutP->num_msgs;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->backlogType != * (int *) &backlogCheck)
#else	UseStaticMsgType
	if ((OutP->backlogType.msg_type_inline != TRUE) ||
	    (OutP->backlogType.msg_type_longform != FALSE) ||
	    (OutP->backlogType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->backlogType.msg_type_number != 1) ||
	    (OutP->backlogType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*backlog /* backlog */ = /* *backlog */ OutP->backlog;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->ownershipType != * (int *) &ownershipCheck)
#else	UseStaticMsgType
	if ((OutP->ownershipType.msg_type_inline != TRUE) ||
	    (OutP->ownershipType.msg_type_longform != FALSE) ||
	    (OutP->ownershipType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (OutP->ownershipType.msg_type_number != 1) ||
	    (OutP->ownershipType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*ownership /* ownership */ = /* *ownership */ OutP->ownership;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->receive_rightsType != * (int *) &receive_rightsCheck)
#else	UseStaticMsgType
	if ((OutP->receive_rightsType.msg_type_inline != TRUE) ||
	    (OutP->receive_rightsType.msg_type_longform != FALSE) ||
	    (OutP->receive_rightsType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (OutP->receive_rightsType.msg_type_number != 1) ||
	    (OutP->receive_rightsType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*receive_rights /* receive_rights */ = /* *receive_rights */ OutP->receive_rights;

	return OutP->RetCode;
}
