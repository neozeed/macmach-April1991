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


/* Routine thread_get_state */
mig_external kern_return_t thread_get_state
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread,
	int flavor,
	thread_state_t old_state,
	unsigned int *old_stateCnt
)
#else
	(target_thread, flavor, old_state, old_stateCnt)
	thread_t target_thread;
	int flavor;
	thread_state_t old_state;
	unsigned int *old_stateCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t flavorType;
		int flavor;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t old_stateType;
		int old_state[1024];
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = target_thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2065;

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

	if (OutP->Head.msg_id != 2165)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size < 44) || (msg_simple != TRUE)) &&
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
	if ((OutP->old_stateType.msg_type_header.msg_type_inline != TRUE) ||
	    (OutP->old_stateType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->old_stateType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->old_stateType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

#if	TypeCheck
	msg_size_delta = (4 * OutP->old_stateType.msg_type_long_number);
	if (msg_size != 44 + msg_size_delta)
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	if (OutP->old_stateType.msg_type_long_number / 1 > *old_stateCnt) {
		bcopy((char *) OutP->old_state, (char *) old_state, 4 * *old_stateCnt);
		*old_stateCnt /* old_stateType.msg_type_long_number 1 */ = /* *old_stateCnt */ OutP->old_stateType.msg_type_long_number / 1;
		return MIG_ARRAY_TOO_LARGE;
	}
	bcopy((char *) OutP->old_state, (char *) old_state, 4 * OutP->old_stateType.msg_type_long_number);

	*old_stateCnt /* old_stateType.msg_type_long_number */ = /* *old_stateCnt */ OutP->old_stateType.msg_type_long_number;

	OutP = &Mess.Out;
	return OutP->RetCode;
}
