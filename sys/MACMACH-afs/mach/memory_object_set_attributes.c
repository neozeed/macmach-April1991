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


/* SimpleRoutine memory_object_set_attributes */
mig_external kern_return_t memory_object_set_attributes
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	boolean_t object_ready,
	boolean_t may_cache,
	memory_object_copy_strategy_t copy_strategy
)
#else
	(memory_control, object_ready, may_cache, copy_strategy)
	memory_object_control_t memory_control;
	boolean_t object_ready;
	boolean_t may_cache;
	memory_object_copy_strategy_t copy_strategy;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t object_readyType;
		boolean_t object_ready;
		msg_type_t may_cacheType;
		boolean_t may_cache;
		msg_type_t copy_strategyType;
		memory_object_copy_strategy_t copy_strategy;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 48;

#if	UseStaticMsgType
	static msg_type_t object_readyType = {
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
	static msg_type_t may_cacheType = {
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
	static msg_type_t copy_strategyType = {
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
	InP->object_readyType = object_readyType;
#else	UseStaticMsgType
	InP->object_readyType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->object_readyType.msg_type_size = 32;
	InP->object_readyType.msg_type_number = 1;
	InP->object_readyType.msg_type_inline = TRUE;
	InP->object_readyType.msg_type_longform = FALSE;
	InP->object_readyType.msg_type_deallocate = FALSE;
	InP->object_readyType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->object_ready /* object_ready */ = /* object_ready */ object_ready;

#if	UseStaticMsgType
	InP->may_cacheType = may_cacheType;
#else	UseStaticMsgType
	InP->may_cacheType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->may_cacheType.msg_type_size = 32;
	InP->may_cacheType.msg_type_number = 1;
	InP->may_cacheType.msg_type_inline = TRUE;
	InP->may_cacheType.msg_type_longform = FALSE;
	InP->may_cacheType.msg_type_deallocate = FALSE;
	InP->may_cacheType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->may_cache /* may_cache */ = /* may_cache */ may_cache;

#if	UseStaticMsgType
	InP->copy_strategyType = copy_strategyType;
#else	UseStaticMsgType
	InP->copy_strategyType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->copy_strategyType.msg_type_size = 32;
	InP->copy_strategyType.msg_type_number = 1;
	InP->copy_strategyType.msg_type_inline = TRUE;
	InP->copy_strategyType.msg_type_longform = FALSE;
	InP->copy_strategyType.msg_type_deallocate = FALSE;
	InP->copy_strategyType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->copy_strategy /* copy_strategy */ = /* copy_strategy */ copy_strategy;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_control;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2091;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}
