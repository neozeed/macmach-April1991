/* Module memory_object_default */

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

/* SimpleRoutine memory_object_create */
mig_internal novalue _Xmemory_object_create
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_create
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t old_memory_object, memory_object_t new_memory_object, vm_size_t new_object_size, memory_object_control_t new_control_port, memory_object_name_t new_name, vm_size_t new_page_size);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

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

#if	UseStaticMsgType
	static msg_type_t new_object_sizeCheck = {
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
	static msg_type_t new_control_portCheck = {
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
	static msg_type_t new_nameCheck = {
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
	static msg_type_t new_page_sizeCheck = {
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

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->new_object_sizeType != * (int *) &new_object_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->new_object_sizeType.msg_type_inline != TRUE) ||
	    (In0P->new_object_sizeType.msg_type_longform != FALSE) ||
	    (In0P->new_object_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->new_object_sizeType.msg_type_number != 1) ||
	    (In0P->new_object_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->new_control_portType != * (int *) &new_control_portCheck)
#else	UseStaticMsgType
	if ((In0P->new_control_portType.msg_type_inline != TRUE) ||
	    (In0P->new_control_portType.msg_type_longform != FALSE) ||
	    (In0P->new_control_portType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->new_control_portType.msg_type_number != 1) ||
	    (In0P->new_control_portType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->new_nameType != * (int *) &new_nameCheck)
#else	UseStaticMsgType
	if ((In0P->new_nameType.msg_type_inline != TRUE) ||
	    (In0P->new_nameType.msg_type_longform != FALSE) ||
	    (In0P->new_nameType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->new_nameType.msg_type_number != 1) ||
	    (In0P->new_nameType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->new_page_sizeType != * (int *) &new_page_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->new_page_sizeType.msg_type_inline != TRUE) ||
	    (In0P->new_page_sizeType.msg_type_longform != FALSE) ||
	    (In0P->new_page_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->new_page_sizeType.msg_type_number != 1) ||
	    (In0P->new_page_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) memory_object_create(In0P->Head.msg_request_port, In0P->new_memory_object, In0P->new_object_size, In0P->new_control_port, In0P->new_name, In0P->new_page_size);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine memory_object_data_initialize */
mig_internal novalue _Xmemory_object_data_initialize
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t memory_object_data_initialize
#if	(defined(__STDC__) || defined(c_plusplus))
		(memory_object_t memory_object, memory_object_control_t memory_control_port, vm_offset_t offset, pointer_t data, unsigned int dataCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_control_portCheck = {
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
	if (* (int *) &In0P->memory_control_portType != * (int *) &memory_control_portCheck)
#else	UseStaticMsgType
	if ((In0P->memory_control_portType.msg_type_inline != TRUE) ||
	    (In0P->memory_control_portType.msg_type_longform != FALSE) ||
	    (In0P->memory_control_portType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_control_portType.msg_type_number != 1) ||
	    (In0P->memory_control_portType.msg_type_size != 32))
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

	(void) memory_object_data_initialize(In0P->Head.msg_request_port, In0P->memory_control_port, In0P->offset, In0P->data, In0P->dataType.msg_type_long_number);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

boolean_t memory_object_default_server
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

	if ((InP->msg_id > 2252) || (InP->msg_id < 2250))
		return FALSE;
	else {
		typedef novalue (*SERVER_STUB_PROC)
#if	(defined(__STDC__) || defined(c_plusplus))
			(msg_header_t *, msg_header_t *);
#else
			();
#endif
		static SERVER_STUB_PROC routines[] = {
			_Xmemory_object_create,
			_Xmemory_object_data_initialize,
			0,
		};

		if (routines[InP->msg_id - 2250])
			(routines[InP->msg_id - 2250]) (InP, &OutP->Head);
		 else
			return FALSE;
	}
	return TRUE;
}
