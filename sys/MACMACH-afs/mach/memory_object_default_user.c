#include "memory_object_default.h"
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

mig_external void init_memory_object_default
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

/* SimpleRoutine memory_object_create */
mig_external kern_return_t memory_object_create
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t old_memory_object,
	memory_object_t new_memory_object,
	vm_size_t new_object_size,
	memory_object_control_t new_control_port,
	memory_object_name_t new_name,
	vm_size_t new_page_size
)
#else
	(old_memory_object, new_memory_object, new_object_size, new_control_port, new_name, new_page_size)
	memory_object_t old_memory_object;
	memory_object_t new_memory_object;
	vm_size_t new_object_size;
	memory_object_control_t new_control_port;
	memory_object_name_t new_name;
	vm_size_t new_page_size;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t new_memory_objectType;
		memory_object_t new_memory_object;
		msg_type_t new_object_sizeType;
		vm_size_t new_object_size;
		msg_type_t new_control_portType;
		memory_object_control_t new_control_port;
		msg_type_t new_nameType;
		memory_object_name_t new_name;
		msg_type_t new_page_sizeType;
		vm_size_t new_page_size;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 64;

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
	static msg_type_t new_object_sizeType = {
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
	static msg_type_t new_control_portType = {
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
	static msg_type_t new_nameType = {
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
	static msg_type_t new_page_sizeType = {
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

#if	UseStaticMsgType
	InP->new_object_sizeType = new_object_sizeType;
#else	UseStaticMsgType
	InP->new_object_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->new_object_sizeType.msg_type_size = 32;
	InP->new_object_sizeType.msg_type_number = 1;
	InP->new_object_sizeType.msg_type_inline = TRUE;
	InP->new_object_sizeType.msg_type_longform = FALSE;
	InP->new_object_sizeType.msg_type_deallocate = FALSE;
	InP->new_object_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_object_size /* new_object_size */ = /* new_object_size */ new_object_size;

#if	UseStaticMsgType
	InP->new_control_portType = new_control_portType;
#else	UseStaticMsgType
	InP->new_control_portType.msg_type_name = MSG_TYPE_PORT;
	InP->new_control_portType.msg_type_size = 32;
	InP->new_control_portType.msg_type_number = 1;
	InP->new_control_portType.msg_type_inline = TRUE;
	InP->new_control_portType.msg_type_longform = FALSE;
	InP->new_control_portType.msg_type_deallocate = FALSE;
	InP->new_control_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_control_port /* new_control_port */ = /* new_control_port */ new_control_port;

#if	UseStaticMsgType
	InP->new_nameType = new_nameType;
#else	UseStaticMsgType
	InP->new_nameType.msg_type_name = MSG_TYPE_PORT;
	InP->new_nameType.msg_type_size = 32;
	InP->new_nameType.msg_type_number = 1;
	InP->new_nameType.msg_type_inline = TRUE;
	InP->new_nameType.msg_type_longform = FALSE;
	InP->new_nameType.msg_type_deallocate = FALSE;
	InP->new_nameType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_name /* new_name */ = /* new_name */ new_name;

#if	UseStaticMsgType
	InP->new_page_sizeType = new_page_sizeType;
#else	UseStaticMsgType
	InP->new_page_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->new_page_sizeType.msg_type_size = 32;
	InP->new_page_sizeType.msg_type_number = 1;
	InP->new_page_sizeType.msg_type_inline = TRUE;
	InP->new_page_sizeType.msg_type_longform = FALSE;
	InP->new_page_sizeType.msg_type_deallocate = FALSE;
	InP->new_page_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_page_size /* new_page_size */ = /* new_page_size */ new_page_size;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = old_memory_object;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2250;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine memory_object_data_initialize */
mig_external kern_return_t memory_object_data_initialize
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control_port,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt
)
#else
	(memory_object, memory_control_port, offset, data, dataCnt)
	memory_object_t memory_object;
	memory_object_control_t memory_control_port;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_control_portType;
		memory_object_control_t memory_control_port;
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
	static msg_type_t memory_control_portType = {
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
	InP->memory_control_portType = memory_control_portType;
#else	UseStaticMsgType
	InP->memory_control_portType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_control_portType.msg_type_size = 32;
	InP->memory_control_portType.msg_type_number = 1;
	InP->memory_control_portType.msg_type_inline = TRUE;
	InP->memory_control_portType.msg_type_longform = FALSE;
	InP->memory_control_portType.msg_type_deallocate = FALSE;
	InP->memory_control_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_control_port /* memory_control_port */ = /* memory_control_port */ memory_control_port;

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
	InP->Head.msg_id = 2251;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}
