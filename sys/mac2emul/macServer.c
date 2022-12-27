/* Module mac */

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
#include <mac2emul/macserver_types.h>

/* Routine mac_create */
mig_internal novalue _Xmac_create
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_create
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t server_port, port_t thread_port, port_t *request_port);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t thread_portCheck = {
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
	static msg_type_t request_portType = {
		/* msg_type_name = */		MSG_TYPE_PORT,
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
	if ((msg_size != 32) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->thread_portType != * (int *) &thread_portCheck)
#else	UseStaticMsgType
	if ((In0P->thread_portType.msg_type_inline != TRUE) ||
	    (In0P->thread_portType.msg_type_longform != FALSE) ||
	    (In0P->thread_portType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->thread_portType.msg_type_number != 1) ||
	    (In0P->thread_portType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_create(In0P->Head.msg_request_port, In0P->thread_port, &OutP->request_port);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 40;

#if	UseStaticMsgType
	OutP->request_portType = request_portType;
#else	UseStaticMsgType
	OutP->request_portType.msg_type_name = MSG_TYPE_PORT;
	OutP->request_portType.msg_type_size = 32;
	OutP->request_portType.msg_type_number = 1;
	OutP->request_portType.msg_type_inline = TRUE;
	OutP->request_portType.msg_type_longform = FALSE;
	OutP->request_portType.msg_type_deallocate = FALSE;
	OutP->request_portType.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->Head.msg_simple = FALSE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_emulate */
mig_internal novalue _Xmac_emulate
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_emulate
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, vm_address_t reset_vector, vm_address_t os_vector, mac_os_map_t os_map, vm_address_t tbox_vector, mac_tbox_map_t tbox_map);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t reset_vectorCheck = {
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
	static msg_type_t os_vectorCheck = {
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
	static msg_type_t os_mapCheck = {
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
	static msg_type_t tbox_vectorCheck = {
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
	static msg_type_t tbox_mapCheck = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		128,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 216) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->reset_vectorType != * (int *) &reset_vectorCheck)
#else	UseStaticMsgType
	if ((In0P->reset_vectorType.msg_type_inline != TRUE) ||
	    (In0P->reset_vectorType.msg_type_longform != FALSE) ||
	    (In0P->reset_vectorType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->reset_vectorType.msg_type_number != 1) ||
	    (In0P->reset_vectorType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->os_vectorType != * (int *) &os_vectorCheck)
#else	UseStaticMsgType
	if ((In0P->os_vectorType.msg_type_inline != TRUE) ||
	    (In0P->os_vectorType.msg_type_longform != FALSE) ||
	    (In0P->os_vectorType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->os_vectorType.msg_type_number != 1) ||
	    (In0P->os_vectorType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->os_mapType != * (int *) &os_mapCheck)
#else	UseStaticMsgType
	if ((In0P->os_mapType.msg_type_inline != TRUE) ||
	    (In0P->os_mapType.msg_type_longform != FALSE) ||
	    (In0P->os_mapType.msg_type_name != MSG_TYPE_CHAR) ||
	    (In0P->os_mapType.msg_type_number != 32) ||
	    (In0P->os_mapType.msg_type_size != 8))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->tbox_vectorType != * (int *) &tbox_vectorCheck)
#else	UseStaticMsgType
	if ((In0P->tbox_vectorType.msg_type_inline != TRUE) ||
	    (In0P->tbox_vectorType.msg_type_longform != FALSE) ||
	    (In0P->tbox_vectorType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->tbox_vectorType.msg_type_number != 1) ||
	    (In0P->tbox_vectorType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->tbox_mapType != * (int *) &tbox_mapCheck)
#else	UseStaticMsgType
	if ((In0P->tbox_mapType.msg_type_inline != TRUE) ||
	    (In0P->tbox_mapType.msg_type_longform != FALSE) ||
	    (In0P->tbox_mapType.msg_type_name != MSG_TYPE_CHAR) ||
	    (In0P->tbox_mapType.msg_type_number != 128) ||
	    (In0P->tbox_mapType.msg_type_size != 8))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_emulate(In0P->Head.msg_request_port, In0P->reset_vector, In0P->os_vector, In0P->os_map, In0P->tbox_vector, In0P->tbox_map);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* SimpleRoutine mac_phys_insert */
mig_internal novalue _Xmac_phys_insert
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_phys_insert
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, memory_object_t memory_object, vm_offset_t offset, vm_size_t size, vm_prot_t perm, vm_offset_t physical);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t memory_objectCheck = {
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
	static msg_type_t sizeCheck = {
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
	static msg_type_t permCheck = {
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
	static msg_type_t physicalCheck = {
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
	if (* (int *) &In0P->memory_objectType != * (int *) &memory_objectCheck)
#else	UseStaticMsgType
	if ((In0P->memory_objectType.msg_type_inline != TRUE) ||
	    (In0P->memory_objectType.msg_type_longform != FALSE) ||
	    (In0P->memory_objectType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_objectType.msg_type_number != 1) ||
	    (In0P->memory_objectType.msg_type_size != 32))
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
	if (* (int *) &In0P->sizeType != * (int *) &sizeCheck)
#else	UseStaticMsgType
	if ((In0P->sizeType.msg_type_inline != TRUE) ||
	    (In0P->sizeType.msg_type_longform != FALSE) ||
	    (In0P->sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->sizeType.msg_type_number != 1) ||
	    (In0P->sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->permType != * (int *) &permCheck)
#else	UseStaticMsgType
	if ((In0P->permType.msg_type_inline != TRUE) ||
	    (In0P->permType.msg_type_longform != FALSE) ||
	    (In0P->permType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->permType.msg_type_number != 1) ||
	    (In0P->permType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->physicalType != * (int *) &physicalCheck)
#else	UseStaticMsgType
	if ((In0P->physicalType.msg_type_inline != TRUE) ||
	    (In0P->physicalType.msg_type_longform != FALSE) ||
	    (In0P->physicalType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->physicalType.msg_type_number != 1) ||
	    (In0P->physicalType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) mac_phys_insert(In0P->Head.msg_request_port, In0P->memory_object, In0P->offset, In0P->size, In0P->perm, In0P->physical);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* Routine mac_interrupt_vector */
mig_internal novalue _Xmac_interrupt_vector
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_interrupt_vector
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, int which_intr, vm_address_t new_vector);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t which_intrCheck = {
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
	static msg_type_t new_vectorCheck = {
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
	if ((msg_size != 40) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->which_intrType != * (int *) &which_intrCheck)
#else	UseStaticMsgType
	if ((In0P->which_intrType.msg_type_inline != TRUE) ||
	    (In0P->which_intrType.msg_type_longform != FALSE) ||
	    (In0P->which_intrType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->which_intrType.msg_type_number != 1) ||
	    (In0P->which_intrType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->new_vectorType != * (int *) &new_vectorCheck)
#else	UseStaticMsgType
	if ((In0P->new_vectorType.msg_type_inline != TRUE) ||
	    (In0P->new_vectorType.msg_type_longform != FALSE) ||
	    (In0P->new_vectorType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->new_vectorType.msg_type_number != 1) ||
	    (In0P->new_vectorType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_interrupt_vector(In0P->Head.msg_request_port, In0P->which_intr, In0P->new_vector);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_interrupt_enable */
mig_internal novalue _Xmac_interrupt_enable
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_interrupt_enable
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, int which_intr, boolean_t enable);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t which_intrCheck = {
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
	static msg_type_t enableCheck = {
		/* msg_type_name = */		MSG_TYPE_BOOLEAN,
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
	if ((msg_size != 40) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->which_intrType != * (int *) &which_intrCheck)
#else	UseStaticMsgType
	if ((In0P->which_intrType.msg_type_inline != TRUE) ||
	    (In0P->which_intrType.msg_type_longform != FALSE) ||
	    (In0P->which_intrType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->which_intrType.msg_type_number != 1) ||
	    (In0P->which_intrType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->enableType != * (int *) &enableCheck)
#else	UseStaticMsgType
	if ((In0P->enableType.msg_type_inline != TRUE) ||
	    (In0P->enableType.msg_type_longform != FALSE) ||
	    (In0P->enableType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (In0P->enableType.msg_type_number != 1) ||
	    (In0P->enableType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_interrupt_enable(In0P->Head.msg_request_port, In0P->which_intr, In0P->enable);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_interrupt_post */
mig_internal novalue _Xmac_interrupt_post
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_interrupt_post
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, int which_intr, boolean_t post);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t which_intrCheck = {
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
	static msg_type_t postCheck = {
		/* msg_type_name = */		MSG_TYPE_BOOLEAN,
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
	if ((msg_size != 40) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->which_intrType != * (int *) &which_intrCheck)
#else	UseStaticMsgType
	if ((In0P->which_intrType.msg_type_inline != TRUE) ||
	    (In0P->which_intrType.msg_type_longform != FALSE) ||
	    (In0P->which_intrType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->which_intrType.msg_type_number != 1) ||
	    (In0P->which_intrType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->postType != * (int *) &postCheck)
#else	UseStaticMsgType
	if ((In0P->postType.msg_type_inline != TRUE) ||
	    (In0P->postType.msg_type_longform != FALSE) ||
	    (In0P->postType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (In0P->postType.msg_type_number != 1) ||
	    (In0P->postType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_interrupt_post(In0P->Head.msg_request_port, In0P->which_intr, In0P->post);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_input_event */
mig_internal novalue _Xmac_input_event
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_input_event
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, mac_input_event_t *event);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t eventType = {
		/* msg_type_name = */		MSG_TYPE_CHAR,
		/* msg_type_size = */		8,
		/* msg_type_number = */		3,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	OutP->RetCode = mac_input_event(In0P->Head.msg_request_port, &OutP->event);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 40;

#if	UseStaticMsgType
	OutP->eventType = eventType;
#else	UseStaticMsgType
	OutP->eventType.msg_type_name = MSG_TYPE_CHAR;
	OutP->eventType.msg_type_size = 8;
	OutP->eventType.msg_type_number = 3;
	OutP->eventType.msg_type_inline = TRUE;
	OutP->eventType.msg_type_longform = FALSE;
	OutP->eventType.msg_type_deallocate = FALSE;
	OutP->eventType.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_vm_map_compat */
mig_internal novalue _Xmac_vm_map_compat
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_vm_map_compat
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, vm_address_t *address, vm_size_t size, vm_address_t mask, boolean_t anywhere, memory_object_t memory_object, vm_offset_t offset, boolean_t copy, vm_prot_t cur_protection, vm_prot_t max_protection, vm_inherit_t inheritance);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

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
	static msg_type_t sizeCheck = {
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
	static msg_type_t maskCheck = {
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
	static msg_type_t anywhereCheck = {
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
	static msg_type_t memory_objectCheck = {
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
	static msg_type_t copyCheck = {
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
	static msg_type_t cur_protectionCheck = {
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
	static msg_type_t max_protectionCheck = {
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
	static msg_type_t inheritanceCheck = {
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

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 104) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->addressType != * (int *) &addressCheck)
#else	UseStaticMsgType
	if ((In0P->addressType.msg_type_inline != TRUE) ||
	    (In0P->addressType.msg_type_longform != FALSE) ||
	    (In0P->addressType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->addressType.msg_type_number != 1) ||
	    (In0P->addressType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->sizeType != * (int *) &sizeCheck)
#else	UseStaticMsgType
	if ((In0P->sizeType.msg_type_inline != TRUE) ||
	    (In0P->sizeType.msg_type_longform != FALSE) ||
	    (In0P->sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->sizeType.msg_type_number != 1) ||
	    (In0P->sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->maskType != * (int *) &maskCheck)
#else	UseStaticMsgType
	if ((In0P->maskType.msg_type_inline != TRUE) ||
	    (In0P->maskType.msg_type_longform != FALSE) ||
	    (In0P->maskType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->maskType.msg_type_number != 1) ||
	    (In0P->maskType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->anywhereType != * (int *) &anywhereCheck)
#else	UseStaticMsgType
	if ((In0P->anywhereType.msg_type_inline != TRUE) ||
	    (In0P->anywhereType.msg_type_longform != FALSE) ||
	    (In0P->anywhereType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (In0P->anywhereType.msg_type_number != 1) ||
	    (In0P->anywhereType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->memory_objectType != * (int *) &memory_objectCheck)
#else	UseStaticMsgType
	if ((In0P->memory_objectType.msg_type_inline != TRUE) ||
	    (In0P->memory_objectType.msg_type_longform != FALSE) ||
	    (In0P->memory_objectType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->memory_objectType.msg_type_number != 1) ||
	    (In0P->memory_objectType.msg_type_size != 32))
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
	if (* (int *) &In0P->copyType != * (int *) &copyCheck)
#else	UseStaticMsgType
	if ((In0P->copyType.msg_type_inline != TRUE) ||
	    (In0P->copyType.msg_type_longform != FALSE) ||
	    (In0P->copyType.msg_type_name != MSG_TYPE_BOOLEAN) ||
	    (In0P->copyType.msg_type_number != 1) ||
	    (In0P->copyType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->cur_protectionType != * (int *) &cur_protectionCheck)
#else	UseStaticMsgType
	if ((In0P->cur_protectionType.msg_type_inline != TRUE) ||
	    (In0P->cur_protectionType.msg_type_longform != FALSE) ||
	    (In0P->cur_protectionType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->cur_protectionType.msg_type_number != 1) ||
	    (In0P->cur_protectionType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->max_protectionType != * (int *) &max_protectionCheck)
#else	UseStaticMsgType
	if ((In0P->max_protectionType.msg_type_inline != TRUE) ||
	    (In0P->max_protectionType.msg_type_longform != FALSE) ||
	    (In0P->max_protectionType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->max_protectionType.msg_type_number != 1) ||
	    (In0P->max_protectionType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->inheritanceType != * (int *) &inheritanceCheck)
#else	UseStaticMsgType
	if ((In0P->inheritanceType.msg_type_inline != TRUE) ||
	    (In0P->inheritanceType.msg_type_longform != FALSE) ||
	    (In0P->inheritanceType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->inheritanceType.msg_type_number != 1) ||
	    (In0P->inheritanceType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_vm_map_compat(In0P->Head.msg_request_port, &In0P->address, In0P->size, In0P->mask, In0P->anywhere, In0P->memory_object, In0P->offset, In0P->copy, In0P->cur_protection, In0P->max_protection, In0P->inheritance);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 40;

#if	UseStaticMsgType
	OutP->addressType = addressType;
#else	UseStaticMsgType
	OutP->addressType.msg_type_name = MSG_TYPE_INTEGER_32;
	OutP->addressType.msg_type_size = 32;
	OutP->addressType.msg_type_number = 1;
	OutP->addressType.msg_type_inline = TRUE;
	OutP->addressType.msg_type_longform = FALSE;
	OutP->addressType.msg_type_deallocate = FALSE;
	OutP->addressType.msg_type_unused = 0;
#endif	UseStaticMsgType

	/* 0 */ OutP->address = In0P->address;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}
#include <mac2emul/macether_types.h>

/* Routine mac_ether_send */
mig_internal novalue _Xmac_ether_send
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_send
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, ether_packet_t packet, unsigned int packetCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;
	unsigned int msg_size_delta;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size < 28) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if ((In0P->packetType.msg_type_inline != TRUE) ||
	    (In0P->packetType.msg_type_longform != FALSE) ||
	    (In0P->packetType.msg_type_name != MSG_TYPE_CHAR) ||
	    (In0P->packetType.msg_type_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
	msg_size_delta = (1 * In0P->packetType.msg_type_number)+3 &~ 3;
	if (msg_size != 28 + msg_size_delta)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_ether_send(In0P->Head.msg_request_port, In0P->packet, In0P->packetType.msg_type_number);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_ether_recv */
mig_internal novalue _Xmac_ether_recv
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_recv
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, ether_packet_t packet, unsigned int *packetCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;
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

	unsigned int packetCnt;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	packetCnt = 1518;

	OutP->RetCode = mac_ether_recv(In0P->Head.msg_request_port, OutP->packet, &packetCnt);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 36;

#if	UseStaticMsgType
	OutP->packetType = packetType;
#else	UseStaticMsgType
	OutP->packetType.msg_type_name = MSG_TYPE_CHAR;
	OutP->packetType.msg_type_size = 8;
	OutP->packetType.msg_type_inline = TRUE;
	OutP->packetType.msg_type_longform = FALSE;
	OutP->packetType.msg_type_deallocate = FALSE;
	OutP->packetType.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->packetType.msg_type_number /* packetCnt */ = /* packetType.msg_type_number */ packetCnt;

	msg_size_delta = (1 * packetCnt)+3 &~ 3;
	msg_size += msg_size_delta;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_ether_add_protocol */
mig_internal novalue _Xmac_ether_add_protocol
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_add_protocol
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, short protocol);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t protocolCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_16,
		/* msg_type_size = */		16,
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
	if ((msg_size != 32) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->protocolType != * (int *) &protocolCheck)
#else	UseStaticMsgType
	if ((In0P->protocolType.msg_type_inline != TRUE) ||
	    (In0P->protocolType.msg_type_longform != FALSE) ||
	    (In0P->protocolType.msg_type_name != MSG_TYPE_INTEGER_16) ||
	    (In0P->protocolType.msg_type_number != 1) ||
	    (In0P->protocolType.msg_type_size != 16))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_ether_add_protocol(In0P->Head.msg_request_port, In0P->protocol);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_ether_delete_protocol */
mig_internal novalue _Xmac_ether_delete_protocol
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_delete_protocol
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, short protocol);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t protocolCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_16,
		/* msg_type_size = */		16,
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
	if ((msg_size != 32) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->protocolType != * (int *) &protocolCheck)
#else	UseStaticMsgType
	if ((In0P->protocolType.msg_type_inline != TRUE) ||
	    (In0P->protocolType.msg_type_longform != FALSE) ||
	    (In0P->protocolType.msg_type_name != MSG_TYPE_INTEGER_16) ||
	    (In0P->protocolType.msg_type_number != 1) ||
	    (In0P->protocolType.msg_type_size != 16))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_ether_delete_protocol(In0P->Head.msg_request_port, In0P->protocol);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_ether_address */
mig_internal novalue _Xmac_ether_address
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_address
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, ether_address_t address);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

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

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	OutP->RetCode = mac_ether_address(In0P->Head.msg_request_port, OutP->address);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 44;

#if	UseStaticMsgType
	OutP->addressType = addressType;
#else	UseStaticMsgType
	OutP->addressType.msg_type_name = MSG_TYPE_CHAR;
	OutP->addressType.msg_type_size = 8;
	OutP->addressType.msg_type_number = 6;
	OutP->addressType.msg_type_inline = TRUE;
	OutP->addressType.msg_type_longform = FALSE;
	OutP->addressType.msg_type_deallocate = FALSE;
	OutP->addressType.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_ether_add_multicast */
mig_internal novalue _Xmac_ether_add_multicast
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_add_multicast
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, ether_address_t address);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

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

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 36) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->addressType != * (int *) &addressCheck)
#else	UseStaticMsgType
	if ((In0P->addressType.msg_type_inline != TRUE) ||
	    (In0P->addressType.msg_type_longform != FALSE) ||
	    (In0P->addressType.msg_type_name != MSG_TYPE_CHAR) ||
	    (In0P->addressType.msg_type_number != 6) ||
	    (In0P->addressType.msg_type_size != 8))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_ether_add_multicast(In0P->Head.msg_request_port, In0P->address);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

/* Routine mac_ether_delete_multicast */
mig_internal novalue _Xmac_ether_delete_multicast
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t mac_ether_delete_multicast
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t request_port, ether_address_t address);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

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

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 36) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->addressType != * (int *) &addressCheck)
#else	UseStaticMsgType
	if ((In0P->addressType.msg_type_inline != TRUE) ||
	    (In0P->addressType.msg_type_longform != FALSE) ||
	    (In0P->addressType.msg_type_name != MSG_TYPE_CHAR) ||
	    (In0P->addressType.msg_type_number != 6) ||
	    (In0P->addressType.msg_type_size != 8))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = mac_ether_delete_multicast(In0P->Head.msg_request_port, In0P->address);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 32;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

boolean_t mac_server
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

	if ((InP->msg_id > 15) || (InP->msg_id < 1))
		return FALSE;
	else {
		typedef novalue (*SERVER_STUB_PROC)
#if	(defined(__STDC__) || defined(c_plusplus))
			(msg_header_t *, msg_header_t *);
#else
			();
#endif
		static SERVER_STUB_PROC routines[] = {
			_Xmac_create,
			_Xmac_emulate,
			_Xmac_phys_insert,
			_Xmac_interrupt_vector,
			_Xmac_interrupt_enable,
			_Xmac_interrupt_post,
			_Xmac_input_event,
			_Xmac_vm_map_compat,
			_Xmac_ether_send,
			_Xmac_ether_recv,
			_Xmac_ether_add_protocol,
			_Xmac_ether_delete_protocol,
			_Xmac_ether_address,
			_Xmac_ether_add_multicast,
			_Xmac_ether_delete_multicast,
		};

		if (routines[InP->msg_id - 1])
			(routines[InP->msg_id - 1]) (InP, &OutP->Head);
		 else
			return FALSE;
	}
	return TRUE;
}
