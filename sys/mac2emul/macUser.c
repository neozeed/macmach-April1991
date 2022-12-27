#include "mac.h"
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

mig_external void init_mac
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

/* Routine mac_create */
mig_external kern_return_t mac_create
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	port_t thread_port,
	port_t *request_port
)
#else
	(server_port, thread_port, request_port)
	port_t server_port;
	port_t thread_port;
	port_t *request_port;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t thread_portType;
		port_t thread_port;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t request_portType;
		port_t request_port;
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
	static msg_type_t thread_portType = {
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
	static msg_type_t request_portCheck = {
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
	InP->thread_portType = thread_portType;
#else	UseStaticMsgType
	InP->thread_portType.msg_type_name = MSG_TYPE_PORT;
	InP->thread_portType.msg_type_size = 32;
	InP->thread_portType.msg_type_number = 1;
	InP->thread_portType.msg_type_inline = TRUE;
	InP->thread_portType.msg_type_longform = FALSE;
	InP->thread_portType.msg_type_deallocate = FALSE;
	InP->thread_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->thread_port /* thread_port */ = /* thread_port */ thread_port;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1;

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

	if (OutP->Head.msg_id != 101)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 40) || (msg_simple != FALSE)) &&
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
	if (* (int *) &OutP->request_portType != * (int *) &request_portCheck)
#else	UseStaticMsgType
	if ((OutP->request_portType.msg_type_inline != TRUE) ||
	    (OutP->request_portType.msg_type_longform != FALSE) ||
	    (OutP->request_portType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->request_portType.msg_type_number != 1) ||
	    (OutP->request_portType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*request_port /* request_port */ = /* *request_port */ OutP->request_port;

	return OutP->RetCode;
}

/* Routine mac_emulate */
mig_external kern_return_t mac_emulate
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	vm_address_t reset_vector,
	vm_address_t os_vector,
	mac_os_map_t os_map,
	vm_address_t tbox_vector,
	mac_tbox_map_t tbox_map
)
#else
	(request_port, reset_vector, os_vector, os_map, tbox_vector, tbox_map)
	port_t request_port;
	vm_address_t reset_vector;
	vm_address_t os_vector;
	mac_os_map_t os_map;
	vm_address_t tbox_vector;
	mac_tbox_map_t tbox_map;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t reset_vectorType;
		vm_address_t reset_vector;
		msg_type_t os_vectorType;
		vm_address_t os_vector;
		msg_type_t os_mapType;
		mac_os_map_t os_map;
		msg_type_t tbox_vectorType;
		vm_address_t tbox_vector;
		msg_type_t tbox_mapType;
		mac_tbox_map_t tbox_map;
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

	unsigned int msg_size = 216;

#if	UseStaticMsgType
	static msg_type_t reset_vectorType = {
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
	static msg_type_t os_vectorType = {
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
	static msg_type_t os_mapType = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		32,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	static msg_type_t tbox_vectorType = {
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
	static msg_type_t tbox_mapType = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		128,
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
	InP->reset_vectorType = reset_vectorType;
#else	UseStaticMsgType
	InP->reset_vectorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->reset_vectorType.msg_type_size = 32;
	InP->reset_vectorType.msg_type_number = 1;
	InP->reset_vectorType.msg_type_inline = TRUE;
	InP->reset_vectorType.msg_type_longform = FALSE;
	InP->reset_vectorType.msg_type_deallocate = FALSE;
	InP->reset_vectorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->reset_vector /* reset_vector */ = /* reset_vector */ reset_vector;

#if	UseStaticMsgType
	InP->os_vectorType = os_vectorType;
#else	UseStaticMsgType
	InP->os_vectorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->os_vectorType.msg_type_size = 32;
	InP->os_vectorType.msg_type_number = 1;
	InP->os_vectorType.msg_type_inline = TRUE;
	InP->os_vectorType.msg_type_longform = FALSE;
	InP->os_vectorType.msg_type_deallocate = FALSE;
	InP->os_vectorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->os_vector /* os_vector */ = /* os_vector */ os_vector;

#if	UseStaticMsgType
	InP->os_mapType = os_mapType;
#else	UseStaticMsgType
	InP->os_mapType.msg_type_name = MSG_TYPE_CHAR;
	InP->os_mapType.msg_type_size = 8;
	InP->os_mapType.msg_type_number = 32;
	InP->os_mapType.msg_type_inline = TRUE;
	InP->os_mapType.msg_type_longform = FALSE;
	InP->os_mapType.msg_type_deallocate = FALSE;
	InP->os_mapType.msg_type_unused = 0;
#endif	UseStaticMsgType

	{ typedef struct { char data[32]; } *sp; * (sp) InP->os_map /* os_map */ = * (sp) /* os_map */ os_map; }

#if	UseStaticMsgType
	InP->tbox_vectorType = tbox_vectorType;
#else	UseStaticMsgType
	InP->tbox_vectorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->tbox_vectorType.msg_type_size = 32;
	InP->tbox_vectorType.msg_type_number = 1;
	InP->tbox_vectorType.msg_type_inline = TRUE;
	InP->tbox_vectorType.msg_type_longform = FALSE;
	InP->tbox_vectorType.msg_type_deallocate = FALSE;
	InP->tbox_vectorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->tbox_vector /* tbox_vector */ = /* tbox_vector */ tbox_vector;

#if	UseStaticMsgType
	InP->tbox_mapType = tbox_mapType;
#else	UseStaticMsgType
	InP->tbox_mapType.msg_type_name = MSG_TYPE_CHAR;
	InP->tbox_mapType.msg_type_size = 8;
	InP->tbox_mapType.msg_type_number = 128;
	InP->tbox_mapType.msg_type_inline = TRUE;
	InP->tbox_mapType.msg_type_longform = FALSE;
	InP->tbox_mapType.msg_type_deallocate = FALSE;
	InP->tbox_mapType.msg_type_unused = 0;
#endif	UseStaticMsgType

	{ typedef struct { char data[128]; } *sp; * (sp) InP->tbox_map /* tbox_map */ = * (sp) /* tbox_map */ tbox_map; }

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2;

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

	if (OutP->Head.msg_id != 102)
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

/* SimpleRoutine mac_phys_insert */
mig_external kern_return_t mac_phys_insert
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	memory_object_t memory_object,
	vm_offset_t offset,
	vm_size_t size,
	vm_prot_t perm,
	vm_offset_t physical
)
#else
	(request_port, memory_object, offset, size, perm, physical)
	port_t request_port;
	memory_object_t memory_object;
	vm_offset_t offset;
	vm_size_t size;
	vm_prot_t perm;
	vm_offset_t physical;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_objectType;
		memory_object_t memory_object;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t sizeType;
		vm_size_t size;
		msg_type_t permType;
		vm_prot_t perm;
		msg_type_t physicalType;
		vm_offset_t physical;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 64;

#if	UseStaticMsgType
	static msg_type_t memory_objectType = {
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
	static msg_type_t permType = {
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
	static msg_type_t physicalType = {
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
	InP->memory_objectType = memory_objectType;
#else	UseStaticMsgType
	InP->memory_objectType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_objectType.msg_type_size = 32;
	InP->memory_objectType.msg_type_number = 1;
	InP->memory_objectType.msg_type_inline = TRUE;
	InP->memory_objectType.msg_type_longform = FALSE;
	InP->memory_objectType.msg_type_deallocate = FALSE;
	InP->memory_objectType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_object /* memory_object */ = /* memory_object */ memory_object;

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
	InP->permType = permType;
#else	UseStaticMsgType
	InP->permType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->permType.msg_type_size = 32;
	InP->permType.msg_type_number = 1;
	InP->permType.msg_type_inline = TRUE;
	InP->permType.msg_type_longform = FALSE;
	InP->permType.msg_type_deallocate = FALSE;
	InP->permType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->perm /* perm */ = /* perm */ perm;

#if	UseStaticMsgType
	InP->physicalType = physicalType;
#else	UseStaticMsgType
	InP->physicalType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->physicalType.msg_type_size = 32;
	InP->physicalType.msg_type_number = 1;
	InP->physicalType.msg_type_inline = TRUE;
	InP->physicalType.msg_type_longform = FALSE;
	InP->physicalType.msg_type_deallocate = FALSE;
	InP->physicalType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->physical /* physical */ = /* physical */ physical;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 3;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* Routine mac_interrupt_vector */
mig_external kern_return_t mac_interrupt_vector
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int which_intr,
	vm_address_t new_vector
)
#else
	(request_port, which_intr, new_vector)
	port_t request_port;
	int which_intr;
	vm_address_t new_vector;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t which_intrType;
		int which_intr;
		msg_type_t new_vectorType;
		vm_address_t new_vector;
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

	unsigned int msg_size = 40;

#if	UseStaticMsgType
	static msg_type_t which_intrType = {
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
	static msg_type_t new_vectorType = {
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
	InP->which_intrType = which_intrType;
#else	UseStaticMsgType
	InP->which_intrType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->which_intrType.msg_type_size = 32;
	InP->which_intrType.msg_type_number = 1;
	InP->which_intrType.msg_type_inline = TRUE;
	InP->which_intrType.msg_type_longform = FALSE;
	InP->which_intrType.msg_type_deallocate = FALSE;
	InP->which_intrType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->which_intr /* which_intr */ = /* which_intr */ which_intr;

#if	UseStaticMsgType
	InP->new_vectorType = new_vectorType;
#else	UseStaticMsgType
	InP->new_vectorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->new_vectorType.msg_type_size = 32;
	InP->new_vectorType.msg_type_number = 1;
	InP->new_vectorType.msg_type_inline = TRUE;
	InP->new_vectorType.msg_type_longform = FALSE;
	InP->new_vectorType.msg_type_deallocate = FALSE;
	InP->new_vectorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_vector /* new_vector */ = /* new_vector */ new_vector;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 4;

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

	if (OutP->Head.msg_id != 104)
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

/* Routine mac_interrupt_enable */
mig_external kern_return_t mac_interrupt_enable
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int which_intr,
	boolean_t enable
)
#else
	(request_port, which_intr, enable)
	port_t request_port;
	int which_intr;
	boolean_t enable;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t which_intrType;
		int which_intr;
		msg_type_t enableType;
		boolean_t enable;
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

	unsigned int msg_size = 40;

#if	UseStaticMsgType
	static msg_type_t which_intrType = {
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
	static msg_type_t enableType = {
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
	InP->which_intrType = which_intrType;
#else	UseStaticMsgType
	InP->which_intrType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->which_intrType.msg_type_size = 32;
	InP->which_intrType.msg_type_number = 1;
	InP->which_intrType.msg_type_inline = TRUE;
	InP->which_intrType.msg_type_longform = FALSE;
	InP->which_intrType.msg_type_deallocate = FALSE;
	InP->which_intrType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->which_intr /* which_intr */ = /* which_intr */ which_intr;

#if	UseStaticMsgType
	InP->enableType = enableType;
#else	UseStaticMsgType
	InP->enableType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->enableType.msg_type_size = 32;
	InP->enableType.msg_type_number = 1;
	InP->enableType.msg_type_inline = TRUE;
	InP->enableType.msg_type_longform = FALSE;
	InP->enableType.msg_type_deallocate = FALSE;
	InP->enableType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->enable /* enable */ = /* enable */ enable;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 5;

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

	if (OutP->Head.msg_id != 105)
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

/* Routine mac_interrupt_post */
mig_external kern_return_t mac_interrupt_post
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int which_intr,
	boolean_t post
)
#else
	(request_port, which_intr, post)
	port_t request_port;
	int which_intr;
	boolean_t post;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t which_intrType;
		int which_intr;
		msg_type_t postType;
		boolean_t post;
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

	unsigned int msg_size = 40;

#if	UseStaticMsgType
	static msg_type_t which_intrType = {
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
	static msg_type_t postType = {
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
	InP->which_intrType = which_intrType;
#else	UseStaticMsgType
	InP->which_intrType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->which_intrType.msg_type_size = 32;
	InP->which_intrType.msg_type_number = 1;
	InP->which_intrType.msg_type_inline = TRUE;
	InP->which_intrType.msg_type_longform = FALSE;
	InP->which_intrType.msg_type_deallocate = FALSE;
	InP->which_intrType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->which_intr /* which_intr */ = /* which_intr */ which_intr;

#if	UseStaticMsgType
	InP->postType = postType;
#else	UseStaticMsgType
	InP->postType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->postType.msg_type_size = 32;
	InP->postType.msg_type_number = 1;
	InP->postType.msg_type_inline = TRUE;
	InP->postType.msg_type_longform = FALSE;
	InP->postType.msg_type_deallocate = FALSE;
	InP->postType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->post /* post */ = /* post */ post;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 6;

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

	if (OutP->Head.msg_id != 106)
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

/* Routine mac_input_event */
mig_external kern_return_t mac_input_event
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	mac_input_event_t *event
)
#else
	(request_port, event)
	port_t request_port;
	mac_input_event_t *event;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t eventType;
		mac_input_event_t event;
		char eventPad[1];
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
	static msg_type_t eventCheck = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		3,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 7;

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

	if (OutP->Head.msg_id != 107)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 40) || (msg_simple != TRUE)) &&
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
	if (* (int *) &OutP->eventType != * (int *) &eventCheck)
#else	UseStaticMsgType
	if ((OutP->eventType.msg_type_inline != TRUE) ||
	    (OutP->eventType.msg_type_longform != FALSE) ||
	    (OutP->eventType.msg_type_name != MSG_TYPE_CHAR) ||
	    (OutP->eventType.msg_type_number != 3) ||
	    (OutP->eventType.msg_type_size != 8))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*event /* event */ = /* *event */ OutP->event;

	return OutP->RetCode;
}

/* Routine mac_vm_map_compat */
mig_external kern_return_t mac_vm_map_compat
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	vm_address_t *address,
	vm_size_t size,
	vm_address_t mask,
	boolean_t anywhere,
	memory_object_t memory_object,
	vm_offset_t offset,
	boolean_t copy,
	vm_prot_t cur_protection,
	vm_prot_t max_protection,
	vm_inherit_t inheritance
)
#else
	(request_port, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance)
	port_t request_port;
	vm_address_t *address;
	vm_size_t size;
	vm_address_t mask;
	boolean_t anywhere;
	memory_object_t memory_object;
	vm_offset_t offset;
	boolean_t copy;
	vm_prot_t cur_protection;
	vm_prot_t max_protection;
	vm_inherit_t inheritance;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t addressType;
		vm_address_t address;
		msg_type_t sizeType;
		vm_size_t size;
		msg_type_t maskType;
		vm_address_t mask;
		msg_type_t anywhereType;
		boolean_t anywhere;
		msg_type_t memory_objectType;
		memory_object_t memory_object;
		msg_type_t offsetType;
		vm_offset_t offset;
		msg_type_t copyType;
		boolean_t copy;
		msg_type_t cur_protectionType;
		vm_prot_t cur_protection;
		msg_type_t max_protectionType;
		vm_prot_t max_protection;
		msg_type_t inheritanceType;
		vm_inherit_t inheritance;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t addressType;
		vm_address_t address;
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

	unsigned int msg_size = 104;

#if	UseStaticMsgType
	static msg_type_t addressType = {
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
	static msg_type_t maskType = {
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
	static msg_type_t anywhereType = {
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
	static msg_type_t memory_objectType = {
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
	static msg_type_t copyType = {
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
	static msg_type_t cur_protectionType = {
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
	static msg_type_t max_protectionType = {
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
	static msg_type_t inheritanceType = {
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
	static msg_type_t addressCheck = {
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
	InP->addressType = addressType;
#else	UseStaticMsgType
	InP->addressType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->addressType.msg_type_size = 32;
	InP->addressType.msg_type_number = 1;
	InP->addressType.msg_type_inline = TRUE;
	InP->addressType.msg_type_longform = FALSE;
	InP->addressType.msg_type_deallocate = FALSE;
	InP->addressType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->address /* *address */ = /* address */ *address;

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
	InP->maskType = maskType;
#else	UseStaticMsgType
	InP->maskType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->maskType.msg_type_size = 32;
	InP->maskType.msg_type_number = 1;
	InP->maskType.msg_type_inline = TRUE;
	InP->maskType.msg_type_longform = FALSE;
	InP->maskType.msg_type_deallocate = FALSE;
	InP->maskType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->mask /* mask */ = /* mask */ mask;

#if	UseStaticMsgType
	InP->anywhereType = anywhereType;
#else	UseStaticMsgType
	InP->anywhereType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->anywhereType.msg_type_size = 32;
	InP->anywhereType.msg_type_number = 1;
	InP->anywhereType.msg_type_inline = TRUE;
	InP->anywhereType.msg_type_longform = FALSE;
	InP->anywhereType.msg_type_deallocate = FALSE;
	InP->anywhereType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->anywhere /* anywhere */ = /* anywhere */ anywhere;

#if	UseStaticMsgType
	InP->memory_objectType = memory_objectType;
#else	UseStaticMsgType
	InP->memory_objectType.msg_type_name = MSG_TYPE_PORT;
	InP->memory_objectType.msg_type_size = 32;
	InP->memory_objectType.msg_type_number = 1;
	InP->memory_objectType.msg_type_inline = TRUE;
	InP->memory_objectType.msg_type_longform = FALSE;
	InP->memory_objectType.msg_type_deallocate = FALSE;
	InP->memory_objectType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->memory_object /* memory_object */ = /* memory_object */ memory_object;

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
	InP->copyType = copyType;
#else	UseStaticMsgType
	InP->copyType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->copyType.msg_type_size = 32;
	InP->copyType.msg_type_number = 1;
	InP->copyType.msg_type_inline = TRUE;
	InP->copyType.msg_type_longform = FALSE;
	InP->copyType.msg_type_deallocate = FALSE;
	InP->copyType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->copy /* copy */ = /* copy */ copy;

#if	UseStaticMsgType
	InP->cur_protectionType = cur_protectionType;
#else	UseStaticMsgType
	InP->cur_protectionType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->cur_protectionType.msg_type_size = 32;
	InP->cur_protectionType.msg_type_number = 1;
	InP->cur_protectionType.msg_type_inline = TRUE;
	InP->cur_protectionType.msg_type_longform = FALSE;
	InP->cur_protectionType.msg_type_deallocate = FALSE;
	InP->cur_protectionType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->cur_protection /* cur_protection */ = /* cur_protection */ cur_protection;

#if	UseStaticMsgType
	InP->max_protectionType = max_protectionType;
#else	UseStaticMsgType
	InP->max_protectionType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->max_protectionType.msg_type_size = 32;
	InP->max_protectionType.msg_type_number = 1;
	InP->max_protectionType.msg_type_inline = TRUE;
	InP->max_protectionType.msg_type_longform = FALSE;
	InP->max_protectionType.msg_type_deallocate = FALSE;
	InP->max_protectionType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->max_protection /* max_protection */ = /* max_protection */ max_protection;

#if	UseStaticMsgType
	InP->inheritanceType = inheritanceType;
#else	UseStaticMsgType
	InP->inheritanceType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->inheritanceType.msg_type_size = 32;
	InP->inheritanceType.msg_type_number = 1;
	InP->inheritanceType.msg_type_inline = TRUE;
	InP->inheritanceType.msg_type_longform = FALSE;
	InP->inheritanceType.msg_type_deallocate = FALSE;
	InP->inheritanceType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->inheritance /* inheritance */ = /* inheritance */ inheritance;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 8;

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

	if (OutP->Head.msg_id != 108)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 40) || (msg_simple != TRUE)) &&
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
	if (* (int *) &OutP->addressType != * (int *) &addressCheck)
#else	UseStaticMsgType
	if ((OutP->addressType.msg_type_inline != TRUE) ||
	    (OutP->addressType.msg_type_longform != FALSE) ||
	    (OutP->addressType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->addressType.msg_type_number != 1) ||
	    (OutP->addressType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*address /* address */ = /* *address */ OutP->address;

	return OutP->RetCode;
}

/* Routine mac_ether_send */
mig_external kern_return_t mac_ether_send
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_packet_t packet,
	unsigned int packetCnt
)
#else
	(request_port, packet, packetCnt)
	port_t request_port;
	ether_packet_t packet;
	unsigned int packetCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t packetType;
		char packet[1518];
		char packetPad[2];
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

	unsigned int msg_size = 28;
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_t packetType = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		1518,
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
	InP->packetType = packetType;
#else	UseStaticMsgType
	InP->packetType.msg_type_name = MSG_TYPE_CHAR;
	InP->packetType.msg_type_size = 8;
	InP->packetType.msg_type_inline = TRUE;
	InP->packetType.msg_type_longform = FALSE;
	InP->packetType.msg_type_deallocate = FALSE;
	InP->packetType.msg_type_unused = 0;
#endif	UseStaticMsgType

	if (packetCnt > 1518)
		return MIG_ARRAY_TOO_LARGE;
	bcopy((char *) packet, (char *) InP->packet, 1 * packetCnt);

	InP->packetType.msg_type_number /* packetCnt */ = /* packetType.msg_type_number */ packetCnt;

	msg_size_delta = (1 * packetCnt)+3 &~ 3;
	msg_size += msg_size_delta;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 9;

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

	if (OutP->Head.msg_id != 109)
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

/* Routine mac_ether_recv */
mig_external kern_return_t mac_ether_recv
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_packet_t packet,
	unsigned int *packetCnt
)
#else
	(request_port, packet, packetCnt)
	port_t request_port;
	ether_packet_t packet;
	unsigned int *packetCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t packetType;
		char packet[1518];
		char packetPad[2];
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
	unsigned int msg_size_delta;

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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 10;

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

	if (OutP->Head.msg_id != 110)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size < 36) || (msg_simple != TRUE)) &&
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
	if ((OutP->packetType.msg_type_inline != TRUE) ||
	    (OutP->packetType.msg_type_longform != FALSE) ||
	    (OutP->packetType.msg_type_name != MSG_TYPE_CHAR) ||
	    (OutP->packetType.msg_type_size != 8))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

#if	TypeCheck
	msg_size_delta = (1 * OutP->packetType.msg_type_number)+3 &~ 3;
	if (msg_size != 36 + msg_size_delta)
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	if (OutP->packetType.msg_type_number / 1 > *packetCnt) {
		bcopy((char *) OutP->packet, (char *) packet, 1 * *packetCnt);
		*packetCnt /* packetType.msg_type_number 1 */ = /* *packetCnt */ OutP->packetType.msg_type_number / 1;
		return MIG_ARRAY_TOO_LARGE;
	}
	bcopy((char *) OutP->packet, (char *) packet, 1 * OutP->packetType.msg_type_number);

	*packetCnt /* packetType.msg_type_number */ = /* *packetCnt */ OutP->packetType.msg_type_number;

	OutP = &Mess.Out;
	return OutP->RetCode;
}

/* Routine mac_ether_add_protocol */
mig_external kern_return_t mac_ether_add_protocol
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	short protocol
)
#else
	(request_port, protocol)
	port_t request_port;
	short protocol;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t protocolType;
		short protocol;
		char protocolPad[2];
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

	unsigned int msg_size = 32;

#if	UseStaticMsgType
	static msg_type_t protocolType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_16,
		/* msg_type_size = */		16,
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
	InP->protocolType = protocolType;
#else	UseStaticMsgType
	InP->protocolType.msg_type_name = MSG_TYPE_INTEGER_16;
	InP->protocolType.msg_type_size = 16;
	InP->protocolType.msg_type_number = 1;
	InP->protocolType.msg_type_inline = TRUE;
	InP->protocolType.msg_type_longform = FALSE;
	InP->protocolType.msg_type_deallocate = FALSE;
	InP->protocolType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->protocol /* protocol */ = /* protocol */ protocol;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 11;

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

	if (OutP->Head.msg_id != 111)
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

/* Routine mac_ether_delete_protocol */
mig_external kern_return_t mac_ether_delete_protocol
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	short protocol
)
#else
	(request_port, protocol)
	port_t request_port;
	short protocol;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t protocolType;
		short protocol;
		char protocolPad[2];
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

	unsigned int msg_size = 32;

#if	UseStaticMsgType
	static msg_type_t protocolType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_16,
		/* msg_type_size = */		16,
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
	InP->protocolType = protocolType;
#else	UseStaticMsgType
	InP->protocolType.msg_type_name = MSG_TYPE_INTEGER_16;
	InP->protocolType.msg_type_size = 16;
	InP->protocolType.msg_type_number = 1;
	InP->protocolType.msg_type_inline = TRUE;
	InP->protocolType.msg_type_longform = FALSE;
	InP->protocolType.msg_type_deallocate = FALSE;
	InP->protocolType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->protocol /* protocol */ = /* protocol */ protocol;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 12;

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

	if (OutP->Head.msg_id != 112)
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

/* Routine mac_ether_address */
mig_external kern_return_t mac_ether_address
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_address_t address
)
#else
	(request_port, address)
	port_t request_port;
	ether_address_t address;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t addressType;
		ether_address_t address;
		char addressPad[2];
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
	static msg_type_t addressCheck = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		6,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 13;

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

	if (OutP->Head.msg_id != 113)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 44) || (msg_simple != TRUE)) &&
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
	if (* (int *) &OutP->addressType != * (int *) &addressCheck)
#else	UseStaticMsgType
	if ((OutP->addressType.msg_type_inline != TRUE) ||
	    (OutP->addressType.msg_type_longform != FALSE) ||
	    (OutP->addressType.msg_type_name != MSG_TYPE_CHAR) ||
	    (OutP->addressType.msg_type_number != 6) ||
	    (OutP->addressType.msg_type_size != 8))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	{ typedef struct { char data[6]; } *sp; * (sp) address /* address */ = * (sp) /* address */ OutP->address; }

	return OutP->RetCode;
}

/* Routine mac_ether_add_multicast */
mig_external kern_return_t mac_ether_add_multicast
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_address_t address
)
#else
	(request_port, address)
	port_t request_port;
	ether_address_t address;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t addressType;
		ether_address_t address;
		char addressPad[2];
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

	unsigned int msg_size = 36;

#if	UseStaticMsgType
	static msg_type_t addressType = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		6,
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
	InP->addressType = addressType;
#else	UseStaticMsgType
	InP->addressType.msg_type_name = MSG_TYPE_CHAR;
	InP->addressType.msg_type_size = 8;
	InP->addressType.msg_type_number = 6;
	InP->addressType.msg_type_inline = TRUE;
	InP->addressType.msg_type_longform = FALSE;
	InP->addressType.msg_type_deallocate = FALSE;
	InP->addressType.msg_type_unused = 0;
#endif	UseStaticMsgType

	{ typedef struct { char data[6]; } *sp; * (sp) InP->address /* address */ = * (sp) /* address */ address; }

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 14;

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

	if (OutP->Head.msg_id != 114)
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

/* Routine mac_ether_delete_multicast */
mig_external kern_return_t mac_ether_delete_multicast
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_address_t address
)
#else
	(request_port, address)
	port_t request_port;
	ether_address_t address;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t addressType;
		ether_address_t address;
		char addressPad[2];
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

	unsigned int msg_size = 36;

#if	UseStaticMsgType
	static msg_type_t addressType = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		6,
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
	InP->addressType = addressType;
#else	UseStaticMsgType
	InP->addressType.msg_type_name = MSG_TYPE_CHAR;
	InP->addressType.msg_type_size = 8;
	InP->addressType.msg_type_number = 6;
	InP->addressType.msg_type_inline = TRUE;
	InP->addressType.msg_type_longform = FALSE;
	InP->addressType.msg_type_deallocate = FALSE;
	InP->addressType.msg_type_unused = 0;
#endif	UseStaticMsgType

	{ typedef struct { char data[6]; } *sp; * (sp) InP->address /* address */ = * (sp) /* address */ address; }

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = request_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 15;

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

	if (OutP->Head.msg_id != 115)
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
