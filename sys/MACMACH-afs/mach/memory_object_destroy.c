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


/* SimpleRoutine memory_object_destroy */
mig_external kern_return_t memory_object_destroy
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	kern_return_t reason
)
#else
	(memory_control, reason)
	memory_object_control_t memory_control;
	kern_return_t reason;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t reasonType;
		kern_return_t reason;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 32;

#if	UseStaticMsgType
	static msg_type_t reasonType = {
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
	InP->reasonType = reasonType;
#else	UseStaticMsgType
	InP->reasonType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->reasonType.msg_type_size = 32;
	InP->reasonType.msg_type_number = 1;
	InP->reasonType.msg_type_inline = TRUE;
	InP->reasonType.msg_type_longform = FALSE;
	InP->reasonType.msg_type_deallocate = FALSE;
	InP->reasonType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->reason /* reason */ = /* reason */ reason;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_control;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2092;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}
