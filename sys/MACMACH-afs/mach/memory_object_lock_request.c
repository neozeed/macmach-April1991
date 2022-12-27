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


/* SimpleRoutine memory_object_lock_request */
mig_external kern_return_t memory_object_lock_request
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t size,
	boolean_t should_clean,
	boolean_t should_flush,
	vm_prot_t lock_value,
	port_t reply_to
)
#else
	(memory_control, offset, size, should_clean, should_flush, lock_value, reply_to)
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t size;
	boolean_t should_clean;
	boolean_t should_flush;
	vm_prot_t lock_value;
	port_t reply_to;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t sizeType;
		vm_size_t size;
		msg_type_t should_cleanType;
		boolean_t should_clean;
		msg_type_t should_flushType;
		boolean_t should_flush;
		msg_type_t lock_valueType;
		vm_prot_t lock_value;
		msg_type_t reply_toType;
		port_t reply_to;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 72;

#if	UseStaticMsgType
	static msg_type_t offsetType = {
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
	static msg_type_t sizeType = {
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
	static msg_type_t should_cleanType = {
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
	static msg_type_t should_flushType = {
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
	static msg_type_t lock_valueType = {
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
	static msg_type_t reply_toType = {
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
	InP->offsetType = offsetType;
#else	UseStaticMsgType
	InP->offsetType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->offsetType.msg_type_size = 32;
	InP->offsetType.msg_type_number = 1;
	InP->offsetType.msg_type_inline = TRUE;
	InP->offsetType.msg_type_longform = FALSE;
	InP->offsetType.msg_type_deallocate = FALSE;
	InP->offsetType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->offset /* offset */ = /* offset */ offset;

#if	UseStaticMsgType
	InP->sizeType = sizeType;
#else	UseStaticMsgType
	InP->sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->sizeType.msg_type_size = 32;
	InP->sizeType.msg_type_number = 1;
	InP->sizeType.msg_type_inline = TRUE;
	InP->sizeType.msg_type_longform = FALSE;
	InP->sizeType.msg_type_deallocate = FALSE;
	InP->sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->size /* size */ = /* size */ size;

#if	UseStaticMsgType
	InP->should_cleanType = should_cleanType;
#else	UseStaticMsgType
	InP->should_cleanType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->should_cleanType.msg_type_size = 32;
	InP->should_cleanType.msg_type_number = 1;
	InP->should_cleanType.msg_type_inline = TRUE;
	InP->should_cleanType.msg_type_longform = FALSE;
	InP->should_cleanType.msg_type_deallocate = FALSE;
	InP->should_cleanType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->should_clean /* should_clean */ = /* should_clean */ should_clean;

#if	UseStaticMsgType
	InP->should_flushType = should_flushType;
#else	UseStaticMsgType
	InP->should_flushType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->should_flushType.msg_type_size = 32;
	InP->should_flushType.msg_type_number = 1;
	InP->should_flushType.msg_type_inline = TRUE;
	InP->should_flushType.msg_type_longform = FALSE;
	InP->should_flushType.msg_type_deallocate = FALSE;
	InP->should_flushType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->should_flush /* should_flush */ = /* should_flush */ should_flush;

#if	UseStaticMsgType
	InP->lock_valueType = lock_valueType;
#else	UseStaticMsgType
	InP->lock_valueType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->lock_valueType.msg_type_size = 32;
	InP->lock_valueType.msg_type_number = 1;
	InP->lock_valueType.msg_type_inline = TRUE;
	InP->lock_valueType.msg_type_longform = FALSE;
	InP->lock_valueType.msg_type_deallocate = FALSE;
	InP->lock_valueType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->lock_value /* lock_value */ = /* lock_value */ lock_value;

#if	UseStaticMsgType
	InP->reply_toType = reply_toType;
#else	UseStaticMsgType
	InP->reply_toType.msg_type_name = MSG_TYPE_PORT;
	InP->reply_toType.msg_type_size = 32;
	InP->reply_toType.msg_type_number = 1;
	InP->reply_toType.msg_type_inline = TRUE;
	InP->reply_toType.msg_type_longform = FALSE;
	InP->reply_toType.msg_type_deallocate = FALSE;
	InP->reply_toType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->reply_to /* reply_to */ = /* reply_to */ reply_to;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_control;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2043;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}
