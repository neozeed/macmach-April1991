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


/* Routine vm_map */
mig_external kern_return_t vm_map
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
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
	(target_task, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance)
	vm_task_t target_task;
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
	InP->Head.msg_request_port = target_task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2089;

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

	if (OutP->Head.msg_id != 2189)
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
