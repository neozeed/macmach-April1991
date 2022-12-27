#include "memory_object_user.h"
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

mig_external void init_memory_object
#if	(defined(__STDC__) || defined(c_plusplus))
	(port_t rep_port)
#else
	(rep_port)
	port_t rep_port;
#endif
{
#ifdef	lint
	rep_port++;
#endif
}

/* SimpleRoutine memory_object_init */
mig_external kern_return_t memory_object_init
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	memory_object_name_t memory_object_name,
	vm_size_t memory_object_page_size
)
#else
	(memory_object, memory_control, memory_object_name, memory_object_page_size)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	memory_object_name_t memory_object_name;
	vm_size_t memory_object_page_size;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t memory_object_nameType;
		memory_object_name_t memory_object_name;
		msg_type_t memory_object_page_sizeType;
		vm_size_t memory_object_page_size;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 48;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_t memory_object_nameType = {
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
	static msg_type_t memory_object_page_sizeType = {
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
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

#if	UseStaticMsgType
	InP->memory_object_nameType = memory_object_nameType;
#else	UseStaticMsgType
	InP->memory_object_nameType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_object_nameType.msg_type_size = 32;
	InP->memory_object_nameType.msg_type_number = 1;
	InP->memory_object_nameType.msg_type_inline = TRUE;
	InP->memory_object_nameType.msg_type_longform = FALSE;
	InP->memory_object_nameType.msg_type_deallocate = FALSE;
	InP->memory_object_nameType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_object_name /* memory_object_name */ = /* memory_object_name */ memory_object_name;

#if	UseStaticMsgType
	InP->memory_object_page_sizeType = memory_object_page_sizeType;
#else	UseStaticMsgType
	InP->memory_object_page_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->memory_object_page_sizeType.msg_type_size = 32;
	InP->memory_object_page_sizeType.msg_type_number = 1;
	InP->memory_object_page_sizeType.msg_type_inline = TRUE;
	InP->memory_object_page_sizeType.msg_type_longform = FALSE;
	InP->memory_object_page_sizeType.msg_type_deallocate = FALSE;
	InP->memory_object_page_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_object_page_size /* memory_object_page_size */ = /* memory_object_page_size */ memory_object_page_size;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2200;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_terminate */
mig_external kern_return_t memory_object_terminate
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	memory_object_name_t memory_object_name
)
#else
	(memory_object, memory_control, memory_object_name)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	memory_object_name_t memory_object_name;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t memory_object_nameType;
		memory_object_name_t memory_object_name;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 40;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
		/* msg_type_name = */		MSG_TYPE_PORT_ALL,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	TRUE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t memory_object_nameType = {
		/* msg_type_name = */		MSG_TYPE_PORT_ALL,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	TRUE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT_ALL;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = TRUE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

#if	UseStaticMsgType
	InP->memory_object_nameType = memory_object_nameType;
#else	UseStaticMsgType
	InP->memory_object_nameType.msg_type_name = MSG_TYPE_PORT_ALL;
	InP->memory_object_nameType.msg_type_size = 32;
	InP->memory_object_nameType.msg_type_number = 1;
	InP->memory_object_nameType.msg_type_inline = TRUE;
	InP->memory_object_nameType.msg_type_longform = FALSE;
	InP->memory_object_nameType.msg_type_deallocate = TRUE;
	InP->memory_object_nameType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_object_name /* memory_object_name */ = /* memory_object_name */ memory_object_name;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2201;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_copy */
mig_external kern_return_t memory_object_copy
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t old_memory_object,
	memory_object_control_t old_memory_control,
	vm_offset_t offset,
	vm_size_t length,
	memory_object_t new_memory_object
)
#else
	(old_memory_object, old_memory_control, offset, length, new_memory_object)
	memory_object_t old_memory_object;
	memory_object_control_t old_memory_control;
	vm_offset_t offset;
	vm_size_t length;
	memory_object_t new_memory_object;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t old_memory_controlType;
		memory_object_control_t old_memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t lengthType;
		vm_size_t length;
		msg_type_t new_memory_objectType;
		memory_object_t new_memory_object;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t old_memory_controlType = {
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
	static msg_type_t lengthType = {
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
	static msg_type_t new_memory_objectType = {
		/* msg_type_name = */		MSG_TYPE_PORT_ALL,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	TRUE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->old_memory_controlType = old_memory_controlType;
#else	UseStaticMsgType
	InP->old_memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->old_memory_controlType.msg_type_size = 32;
	InP->old_memory_controlType.msg_type_number = 1;
	InP->old_memory_controlType.msg_type_inline = TRUE;
	InP->old_memory_controlType.msg_type_longform = FALSE;
	InP->old_memory_controlType.msg_type_deallocate = FALSE;
	InP->old_memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->old_memory_control /* old_memory_control */ = /* old_memory_control */ old_memory_control;

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
	InP->lengthType = lengthType;
#else	UseStaticMsgType
	InP->lengthType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->lengthType.msg_type_size = 32;
	InP->lengthType.msg_type_number = 1;
	InP->lengthType.msg_type_inline = TRUE;
	InP->lengthType.msg_type_longform = FALSE;
	InP->lengthType.msg_type_deallocate = FALSE;
	InP->lengthType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->length /* length */ = /* length */ length;

#if	UseStaticMsgType
	InP->new_memory_objectType = new_memory_objectType;
#else	UseStaticMsgType
	InP->new_memory_objectType.msg_type_name = MSG_TYPE_PORT_ALL;
	InP->new_memory_objectType.msg_type_size = 32;
	InP->new_memory_objectType.msg_type_number = 1;
	InP->new_memory_objectType.msg_type_inline = TRUE;
	InP->new_memory_objectType.msg_type_longform = FALSE;
	InP->new_memory_objectType.msg_type_deallocate = TRUE;
	InP->new_memory_objectType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_memory_object /* new_memory_object */ = /* new_memory_object */ new_memory_object;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = old_memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2202;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_data_request */
mig_external kern_return_t memory_object_data_request
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length,
	vm_prot_t desired_access
)
#else
	(memory_object, memory_control, offset, length, desired_access)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t lengthType;
		vm_size_t length;
		msg_type_t desired_accessType;
		vm_prot_t desired_access;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_t lengthType = {
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
	static msg_type_t desired_accessType = {
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
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

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
	InP->lengthType = lengthType;
#else	UseStaticMsgType
	InP->lengthType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->lengthType.msg_type_size = 32;
	InP->lengthType.msg_type_number = 1;
	InP->lengthType.msg_type_inline = TRUE;
	InP->lengthType.msg_type_longform = FALSE;
	InP->lengthType.msg_type_deallocate = FALSE;
	InP->lengthType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->length /* length */ = /* length */ length;

#if	UseStaticMsgType
	InP->desired_accessType = desired_accessType;
#else	UseStaticMsgType
	InP->desired_accessType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->desired_accessType.msg_type_size = 32;
	InP->desired_accessType.msg_type_number = 1;
	InP->desired_accessType.msg_type_inline = TRUE;
	InP->desired_accessType.msg_type_longform = FALSE;
	InP->desired_accessType.msg_type_deallocate = FALSE;
	InP->desired_accessType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->desired_access /* desired_access */ = /* desired_access */ desired_access;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2203;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_data_unlock */
mig_external kern_return_t memory_object_data_unlock
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length,
	vm_prot_t desired_access
)
#else
	(memory_object, memory_control, offset, length, desired_access)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t lengthType;
		vm_size_t length;
		msg_type_t desired_accessType;
		vm_prot_t desired_access;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_t lengthType = {
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
	static msg_type_t desired_accessType = {
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
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

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
	InP->lengthType = lengthType;
#else	UseStaticMsgType
	InP->lengthType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->lengthType.msg_type_size = 32;
	InP->lengthType.msg_type_number = 1;
	InP->lengthType.msg_type_inline = TRUE;
	InP->lengthType.msg_type_longform = FALSE;
	InP->lengthType.msg_type_deallocate = FALSE;
	InP->lengthType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->length /* length */ = /* length */ length;

#if	UseStaticMsgType
	InP->desired_accessType = desired_accessType;
#else	UseStaticMsgType
	InP->desired_accessType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->desired_accessType.msg_type_size = 32;
	InP->desired_accessType.msg_type_number = 1;
	InP->desired_accessType.msg_type_inline = TRUE;
	InP->desired_accessType.msg_type_longform = FALSE;
	InP->desired_accessType.msg_type_deallocate = FALSE;
	InP->desired_accessType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->desired_access /* desired_access */ = /* desired_access */ desired_access;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2204;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_data_write */
mig_external kern_return_t memory_object_data_write
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt
)
#else
	(memory_object, memory_control, offset, data, dataCnt)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_long_t dataType;
		pointer_t data;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_long_t dataType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTERNAL_MEMORY,
		/* msg_type_long_size = */	8,
		/* msg_type_long_number = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

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
	InP->dataType = dataType;
#else	UseStaticMsgType
	InP->dataType.msg_type_long_name = MSG_TYPE_INTERNAL_MEMORY;
	InP->dataType.msg_type_long_size = 8;
	InP->dataType.msg_type_header.msg_type_inline = FALSE;
	InP->dataType.msg_type_header.msg_type_longform = TRUE;
	InP->dataType.msg_type_header.msg_type_deallocate = FALSE;
	InP->dataType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->data /* data */ = /* data */ data;

	InP->dataType.msg_type_long_number /* dataCnt */ = /* dataType.msg_type_long_number */ dataCnt;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2205;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_lock_completed */
mig_external kern_return_t memory_object_lock_completed
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length
)
#else
	(memory_object, memory_control, offset, length)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t lengthType;
		vm_size_t length;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 48;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_t lengthType = {
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
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

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
	InP->lengthType = lengthType;
#else	UseStaticMsgType
	InP->lengthType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->lengthType.msg_type_size = 32;
	InP->lengthType.msg_type_number = 1;
	InP->lengthType.msg_type_inline = TRUE;
	InP->lengthType.msg_type_longform = FALSE;
	InP->lengthType.msg_type_deallocate = FALSE;
	InP->lengthType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->length /* length */ = /* length */ length;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2206;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_supply_completed */
mig_external kern_return_t memory_object_supply_completed
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length,
	kern_return_t result,
	vm_offset_t error_offset
)
#else
	(memory_object, memory_control, offset, length, result, error_offset)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t result;
	vm_offset_t error_offset;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t lengthType;
		vm_size_t length;
		msg_type_t resultType;
		kern_return_t result;
		msg_type_t error_offsetType;
		vm_offset_t error_offset;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 64;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_t lengthType = {
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
	static msg_type_t resultType = {
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
	static msg_type_t error_offsetType = {
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
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

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
	InP->lengthType = lengthType;
#else	UseStaticMsgType
	InP->lengthType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->lengthType.msg_type_size = 32;
	InP->lengthType.msg_type_number = 1;
	InP->lengthType.msg_type_inline = TRUE;
	InP->lengthType.msg_type_longform = FALSE;
	InP->lengthType.msg_type_deallocate = FALSE;
	InP->lengthType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->length /* length */ = /* length */ length;

#if	UseStaticMsgType
	InP->resultType = resultType;
#else	UseStaticMsgType
	InP->resultType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->resultType.msg_type_size = 32;
	InP->resultType.msg_type_number = 1;
	InP->resultType.msg_type_inline = TRUE;
	InP->resultType.msg_type_longform = FALSE;
	InP->resultType.msg_type_deallocate = FALSE;
	InP->resultType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->result /* result */ = /* result */ result;

#if	UseStaticMsgType
	InP->error_offsetType = error_offsetType;
#else	UseStaticMsgType
	InP->error_offsetType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->error_offsetType.msg_type_size = 32;
	InP->error_offsetType.msg_type_number = 1;
	InP->error_offsetType.msg_type_inline = TRUE;
	InP->error_offsetType.msg_type_longform = FALSE;
	InP->error_offsetType.msg_type_deallocate = FALSE;
	InP->error_offsetType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->error_offset /* error_offset */ = /* error_offset */ error_offset;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2207;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_data_return */
mig_external kern_return_t memory_object_data_return
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt
)
#else
	(memory_object, memory_control, offset, data, dataCnt)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_long_t dataType;
		pointer_t data;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t memory_controlType = {
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
	static msg_type_long_t dataType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTERNAL_MEMORY,
		/* msg_type_long_size = */	8,
		/* msg_type_long_number = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->memory_controlType = memory_controlType;
#else	UseStaticMsgType
	InP->memory_controlType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_controlType.msg_type_size = 32;
	InP->memory_controlType.msg_type_number = 1;
	InP->memory_controlType.msg_type_inline = TRUE;
	InP->memory_controlType.msg_type_longform = FALSE;
	InP->memory_controlType.msg_type_deallocate = FALSE;
	InP->memory_controlType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control /* memory_control */ = /* memory_control */ memory_control;

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
	InP->dataType = dataType;
#else	UseStaticMsgType
	InP->dataType.msg_type_long_name = MSG_TYPE_INTERNAL_MEMORY;
	InP->dataType.msg_type_long_size = 8;
	InP->dataType.msg_type_header.msg_type_inline = FALSE;
	InP->dataType.msg_type_header.msg_type_longform = TRUE;
	InP->dataType.msg_type_header.msg_type_deallocate = FALSE;
	InP->dataType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->data /* data */ = /* data */ data;

	InP->dataType.msg_type_long_number /* dataCnt */ = /* dataType.msg_type_long_number */ dataCnt;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2208;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}
