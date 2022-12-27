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


/* Routine memory_object_get_attributes */
mig_external kern_return_t memory_object_get_attributes
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	boolean_t *object_ready,
	boolean_t *may_cache,
	memory_object_copy_strategy_t *copy_strategy
)
#else
	(memory_control, object_ready, may_cache, copy_strategy)
	memory_object_control_t memory_control;
	boolean_t *object_ready;
	boolean_t *may_cache;
	memory_object_copy_strategy_t *copy_strategy;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t object_readyType;
		boolean_t object_ready;
		msg_type_t may_cacheType;
		boolean_t may_cache;
		msg_type_t copy_strategyType;
		memory_object_copy_strategy_t copy_strategy;
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

	unsigned int msg_size = 24;

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
	static msg_type_t object_readyCheck = {
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
	static msg_type_t may_cacheCheck = {
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
	static msg_type_t copy_strategyCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = memory_control;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2040;

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

	if (OutP->Head.msg_id != 2140)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 56) || (msg_simple != TRUE)) &&
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
	if (* (int *) &OutP->object_readyType != * (int *) &object_readyCheck)
#else	UseStaticMsgType
	if ((OutP->object_readyType.msg_type_inline != TRUE) ||
	    (OutP->object_readyType.msg_type_longform != FALSE) ||
	    (OutP->object_readyType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (OutP->object_readyType.msg_type_number != 1) ||
	    (OutP->object_readyType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*object_ready /* object_ready */ = /* *object_ready */ OutP->object_ready;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->may_cacheType != * (int *) &may_cacheCheck)
#else	UseStaticMsgType
	if ((OutP->may_cacheType.msg_type_inline != TRUE) ||
	    (OutP->may_cacheType.msg_type_longform != FALSE) ||
	    (OutP->may_cacheType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (OutP->may_cacheType.msg_type_number != 1) ||
	    (OutP->may_cacheType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*may_cache /* may_cache */ = /* *may_cache */ OutP->may_cache;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->copy_strategyType != * (int *) &copy_strategyCheck)
#else	UseStaticMsgType
	if ((OutP->copy_strategyType.msg_type_inline != TRUE) ||
	    (OutP->copy_strategyType.msg_type_longform != FALSE) ||
	    (OutP->copy_strategyType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->copy_strategyType.msg_type_number != 1) ||
	    (OutP->copy_strategyType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*copy_strategy /* copy_strategy */ = /* *copy_strategy */ OutP->copy_strategy;

	return OutP->RetCode;
}
