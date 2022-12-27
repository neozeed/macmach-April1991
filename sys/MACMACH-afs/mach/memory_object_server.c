/* Module memory_object */

#define EXPORT_BOOLEAN
#include <mach/boolean.h>
#include <mach/message.h>
#include <mach/mig_errors.h>

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

/* Due to pcc compiler bug, cannot use void */
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(hc)
#define novalue void
#else
#define novalue int
#endif

#define msg_request_port	msg_local_port
#define msg_reply_port		msg_remote_port
#include <mach/std_types.h>
#include <mach/mach_types.h>

/* SimpleRoutine memory_object_init */
mig_internal novalue _Xmemory_object_init
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_init
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, memory_object_name_t memory_object_name, vm_size_t memory_object_page_size);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t memory_object_nameCheck = {
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
	static msg_type_t memory_object_page_sizeCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 48) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_object_nameType != * (int *) &memory_object_nameCheck)
#else	UseStaticMsgType
	if ((In0P->memory_object_nameType.msg_type_inline != TRUE) ||
	    (In0P->memory_object_nameType.msg_type_longform != FALSE) ||
	    (In0P->memory_object_nameType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_object_nameType.msg_type_number != 1) ||
	    (In0P->memory_object_nameType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_object_page_sizeType != * (int *) &memory_object_page_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->memory_object_page_sizeType.msg_type_inline != TRUE) ||
	    (In0P->memory_object_page_sizeType.msg_type_longform != FALSE) ||
	    (In0P->memory_object_page_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->memory_object_page_sizeType.msg_type_number != 1) ||
	    (In0P->memory_object_page_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_init(In0P->Head.msg_request_port, In0P->memory_control, In0P->memory_object_name, In0P->memory_object_page_size);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_terminate */
mig_internal novalue _Xmemory_object_terminate
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t memory_controlType;
		memory_object_control_t memory_control;
		msg_type_t memory_object_nameType;
		memory_object_name_t memory_object_name;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_terminate
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, memory_object_name_t memory_object_name);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t memory_object_nameCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT_ALL,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	TRUE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 40) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT_ALL) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_object_nameType != * (int *) &memory_object_nameCheck)
#else	UseStaticMsgType
	if ((In0P->memory_object_nameType.msg_type_inline != TRUE) ||
	    (In0P->memory_object_nameType.msg_type_longform != FALSE) ||
	    (In0P->memory_object_nameType.msg_type_name != MSG_TYPE_PORT_ALL) ||
	    (In0P->memory_object_nameType.msg_type_number != 1) ||
	    (In0P->memory_object_nameType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_terminate(In0P->Head.msg_request_port, In0P->memory_control, In0P->memory_object_name);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_copy */
mig_internal novalue _Xmemory_object_copy
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_copy
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t old_memory_object, memory_object_control_t old_memory_control, vm_offset_t offset, vm_size_t length, memory_object_t new_memory_object);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t old_memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
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
	static msg_type_t lengthCheck = {
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
	static msg_type_t new_memory_objectCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT_ALL,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	TRUE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 56) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->old_memory_controlType != * (int *) &old_memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->old_memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->old_memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->old_memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->old_memory_controlType.msg_type_number != 1) ||
	    (In0P->old_memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->lengthType != * (int *) &lengthCheck)
#else	UseStaticMsgType
	if ((In0P->lengthType.msg_type_inline != TRUE) ||
	    (In0P->lengthType.msg_type_longform != FALSE) ||
	    (In0P->lengthType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->lengthType.msg_type_number != 1) ||
	    (In0P->lengthType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->new_memory_objectType != * (int *) &new_memory_objectCheck)
#else	UseStaticMsgType
	if ((In0P->new_memory_objectType.msg_type_inline != TRUE) ||
	    (In0P->new_memory_objectType.msg_type_longform != FALSE) ||
	    (In0P->new_memory_objectType.msg_type_name != MSG_TYPE_PORT_ALL) ||
	    (In0P->new_memory_objectType.msg_type_number != 1) ||
	    (In0P->new_memory_objectType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_copy(In0P->Head.msg_request_port, In0P->old_memory_control, In0P->offset, In0P->length, In0P->new_memory_object);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_data_request */
mig_internal novalue _Xmemory_object_data_request
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_data_request
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, vm_offset_t offset, vm_size_t length, vm_prot_t desired_access);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
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
	static msg_type_t lengthCheck = {
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
	static msg_type_t desired_accessCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 56) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->lengthType != * (int *) &lengthCheck)
#else	UseStaticMsgType
	if ((In0P->lengthType.msg_type_inline != TRUE) ||
	    (In0P->lengthType.msg_type_longform != FALSE) ||
	    (In0P->lengthType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->lengthType.msg_type_number != 1) ||
	    (In0P->lengthType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->desired_accessType != * (int *) &desired_accessCheck)
#else	UseStaticMsgType
	if ((In0P->desired_accessType.msg_type_inline != TRUE) ||
	    (In0P->desired_accessType.msg_type_longform != FALSE) ||
	    (In0P->desired_accessType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->desired_accessType.msg_type_number != 1) ||
	    (In0P->desired_accessType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_data_request(In0P->Head.msg_request_port, In0P->memory_control, In0P->offset, In0P->length, In0P->desired_access);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_data_unlock */
mig_internal novalue _Xmemory_object_data_unlock
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_data_unlock
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, vm_offset_t offset, vm_size_t length, vm_prot_t desired_access);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
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
	static msg_type_t lengthCheck = {
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
	static msg_type_t desired_accessCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 56) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->lengthType != * (int *) &lengthCheck)
#else	UseStaticMsgType
	if ((In0P->lengthType.msg_type_inline != TRUE) ||
	    (In0P->lengthType.msg_type_longform != FALSE) ||
	    (In0P->lengthType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->lengthType.msg_type_number != 1) ||
	    (In0P->lengthType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->desired_accessType != * (int *) &desired_accessCheck)
#else	UseStaticMsgType
	if ((In0P->desired_accessType.msg_type_inline != TRUE) ||
	    (In0P->desired_accessType.msg_type_longform != FALSE) ||
	    (In0P->desired_accessType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->desired_accessType.msg_type_number != 1) ||
	    (In0P->desired_accessType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_data_unlock(In0P->Head.msg_request_port, In0P->memory_control, In0P->offset, In0P->length, In0P->desired_access);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_data_write */
mig_internal novalue _Xmemory_object_data_write
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_data_write
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, vm_offset_t offset, pointer_t data, unsigned int dataCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 56) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
	if ((In0P->dataType.msg_type_header.msg_type_inline != FALSE) ||
	    (In0P->dataType.msg_type_header.msg_type_longform != TRUE) ||
	    (In0P->dataType.msg_type_long_name != MSG_TYPE_BYTE) ||
	    (In0P->dataType.msg_type_long_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_data_write(In0P->Head.msg_request_port, In0P->memory_control, In0P->offset, In0P->data, In0P->dataType.msg_type_long_number);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_lock_completed */
mig_internal novalue _Xmemory_object_lock_completed
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_lock_completed
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, vm_offset_t offset, vm_size_t length);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
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
	static msg_type_t lengthCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 48) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->lengthType != * (int *) &lengthCheck)
#else	UseStaticMsgType
	if ((In0P->lengthType.msg_type_inline != TRUE) ||
	    (In0P->lengthType.msg_type_longform != FALSE) ||
	    (In0P->lengthType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->lengthType.msg_type_number != 1) ||
	    (In0P->lengthType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_lock_completed(In0P->Head.msg_request_port, In0P->memory_control, In0P->offset, In0P->length);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_supply_completed */
mig_internal novalue _Xmemory_object_supply_completed
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_supply_completed
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, vm_offset_t offset, vm_size_t length, kern_return_t result, vm_offset_t error_offset);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
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
	static msg_type_t lengthCheck = {
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
	static msg_type_t resultCheck = {
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
	static msg_type_t error_offsetCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 64) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->lengthType != * (int *) &lengthCheck)
#else	UseStaticMsgType
	if ((In0P->lengthType.msg_type_inline != TRUE) ||
	    (In0P->lengthType.msg_type_longform != FALSE) ||
	    (In0P->lengthType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->lengthType.msg_type_number != 1) ||
	    (In0P->lengthType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->resultType != * (int *) &resultCheck)
#else	UseStaticMsgType
	if ((In0P->resultType.msg_type_inline != TRUE) ||
	    (In0P->resultType.msg_type_longform != FALSE) ||
	    (In0P->resultType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->resultType.msg_type_number != 1) ||
	    (In0P->resultType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->error_offsetType != * (int *) &error_offsetCheck)
#else	UseStaticMsgType
	if ((In0P->error_offsetType.msg_type_inline != TRUE) ||
	    (In0P->error_offsetType.msg_type_longform != FALSE) ||
	    (In0P->error_offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->error_offsetType.msg_type_number != 1) ||
	    (In0P->error_offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_supply_completed(In0P->Head.msg_request_port, In0P->memory_control, In0P->offset, In0P->length, In0P->result, In0P->error_offset);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_data_return */
mig_internal novalue _Xmemory_object_data_return
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_data_return
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control, vm_offset_t offset, pointer_t data, unsigned int dataCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_controlCheck = {
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
	static msg_type_t offsetCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 56) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_controlType != * (int *) &memory_controlCheck)
#else	UseStaticMsgType
	if ((In0P->memory_controlType.msg_type_inline != TRUE) ||
	    (In0P->memory_controlType.msg_type_longform != FALSE) ||
	    (In0P->memory_controlType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_controlType.msg_type_number != 1) ||
	    (In0P->memory_controlType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->offsetType != * (int *) &offsetCheck)
#else	UseStaticMsgType
	if ((In0P->offsetType.msg_type_inline != TRUE) ||
	    (In0P->offsetType.msg_type_longform != FALSE) ||
	    (In0P->offsetType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->offsetType.msg_type_number != 1) ||
	    (In0P->offsetType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
	if ((In0P->dataType.msg_type_header.msg_type_inline != FALSE) ||
	    (In0P->dataType.msg_type_header.msg_type_longform != TRUE) ||
	    (In0P->dataType.msg_type_long_name != MSG_TYPE_BYTE) ||
	    (In0P->dataType.msg_type_long_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_data_return(In0P->Head.msg_request_port, In0P->memory_control, In0P->offset, In0P->data, In0P->dataType.msg_type_long_number);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

boolean_t memory_object_server
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	register msg_header_t *InP =  InHeadP;
	register death_pill_t *OutP = (death_pill_t *) OutHeadP;

#if	UseStaticMsgType
	static msg_type_t RetCodeType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = sizeof *OutP;
	OutP->Head.msg_type = InP->msg_type;
	OutP->Head.msg_local_port = PORT_NULL;
	OutP->Head.msg_remote_port = InP->msg_reply_port;
	OutP->Head.msg_id = InP->msg_id + 100;

#if	UseStaticMsgType
	OutP->RetCodeType = RetCodeType;
#else	UseStaticMsgType
	OutP->RetCodeType.msg_type_name = MSG_TYPE_INTEGER_32;
	OutP->RetCodeType.msg_type_size = 32;
	OutP->RetCodeType.msg_type_number = 1;
	OutP->RetCodeType.msg_type_inline = TRUE;
	OutP->RetCodeType.msg_type_longform = FALSE;
	OutP->RetCodeType.msg_type_deallocate = FALSE;
	OutP->RetCodeType.msg_type_unused = 0;
#endif	UseStaticMsgType
	OutP->RetCode = MIG_BAD_ID;

	if ((InP->msg_id > 2208) || (InP->msg_id < 2200))
		return FALSE;
	else {
		typedef novalue (*SERVER_STUB_PROC)
#if	(defined(__STDC__) || defined(c_plusplus))
			(msg_header_t *, msg_header_t *);
#else
			();
#endif
		static SERVER_STUB_PROC routines[] = {
			_Xmemory_object_init,
			_Xmemory_object_terminate,
			_Xmemory_object_copy,
			_Xmemory_object_data_request,
			_Xmemory_object_data_unlock,
			_Xmemory_object_data_write,
			_Xmemory_object_lock_completed,
			_Xmemory_object_supply_completed,
			_Xmemory_object_data_return,
		};

		if (routines[InP->msg_id - 2200])
			(routines[InP->msg_id - 2200]) (InP, &OutP->Head);
		 else
			return FALSE;
	}
	return TRUE;
}
