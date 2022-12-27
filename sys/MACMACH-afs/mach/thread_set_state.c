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


/* Routine thread_set_state */
mig_external kern_return_t thread_set_state
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread,
	int flavor,
	thread_state_t new_state,
	unsigned int new_stateCnt
)
#else
	(target_thread, flavor, new_state, new_stateCnt)
	thread_t target_thread;
	int flavor;
	thread_state_t new_state;
	unsigned int new_stateCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t flavorType;
		int flavor;
		msg_type_long_t new_stateType;
		int new_state[1024];
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

	unsigned int msg_size = 44;
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_t flavorType = {
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
	static msg_type_long_t new_stateType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTEGER_32,
		/* msg_type_long_size = */	32,
		/* msg_type_long_number = */	1024
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
	InP->flavorType = flavorType;
#else	UseStaticMsgType
	InP->flavorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->flavorType.msg_type_size = 32;
	InP->flavorType.msg_type_number = 1;
	InP->flavorType.msg_type_inline = TRUE;
	InP->flavorType.msg_type_longform = FALSE;
	InP->flavorType.msg_type_deallocate = FALSE;
	InP->flavorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->flavor /* flavor */ = /* flavor */ flavor;

#if	UseStaticMsgType
	InP->new_stateType = new_stateType;
#else	UseStaticMsgType
	InP->new_stateType.msg_type_long_name = MSG_TYPE_INTEGER_32;
	InP->new_stateType.msg_type_long_size = 32;
	InP->new_stateType.msg_type_header.msg_type_inline = TRUE;
	InP->new_stateType.msg_type_header.msg_type_longform = TRUE;
	InP->new_stateType.msg_type_header.msg_type_deallocate = FALSE;
	InP->new_stateType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	if (new_stateCnt > 1024)
		return MIG_ARRAY_TOO_LARGE;
	bcopy((char *) new_state, (char *) InP->new_state, 4 * new_stateCnt);

	InP->new_stateType.msg_type_long_number /* new_stateCnt */ = /* new_stateType.msg_type_long_number */ new_stateCnt;

	msg_size_delta = (4 * new_stateCnt);
	msg_size += msg_size_delta;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = target_thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2066;

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

	if (OutP->Head.msg_id != 2166)
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
