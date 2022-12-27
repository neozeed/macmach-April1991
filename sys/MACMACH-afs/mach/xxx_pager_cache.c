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


/* SimpleRoutine xxx_pager_cache */
mig_external kern_return_t xxx_pager_cache
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	boolean_t should_cache
)
#else
	(memory_control, should_cache)
	memory_object_control_t memory_control;
	boolean_t should_cache;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t should_cacheType;
		boolean_t should_cache;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 32;

#if	UseStaticMsgType
	static msg_type_t should_cacheType = {
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
	InP->should_cacheType = should_cacheType;
#else	UseStaticMsgType
	InP->should_cacheType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->should_cacheType.msg_type_size = 32;
	InP->should_cacheType.msg_type_number = 1;
	InP->should_cacheType.msg_type_inline = TRUE;
	InP->should_cacheType.msg_type_longform = FALSE;
	InP->should_cacheType.msg_type_deallocate = FALSE;
	InP->should_cacheType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->should_cache /* should_cache */ = /* should_cache */ should_cache;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_control;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2044;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}
