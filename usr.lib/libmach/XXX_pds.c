/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	XXX_pds.c,v $
 * Revision 1.2  90/02/24  16:57:38  bww
 * 	Changed obsolete uses of "hc" symbol to "__HC__".
 * 	[90/02/24  16:57:05  bww]
 * 
 * Revision 1.1  90/02/19  18:07:24  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:39:08  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*  
 * Abstract:
 *	Write around for the obsolete calls to the default pager
 */

#include <mach/message.h>
#include <mig_errors.h>

#ifndef	mig_internal
#define	mig_internal	static
#endif	mig_internal

#ifndef	TypeCheck
#define	TypeCheck 1
#endif	TypeCheck

#ifndef	UseExternRCSId
#if	__HC__
#define	UseExternRCSId		1
#endif	/* __HC__ */
#endif	UseExternRCSId

#ifndef	UseStaticMsgType
#if	!__HC__
#define	UseStaticMsgType	1
#endif	/* __HC__ */
#endif	UseStaticMsgType

/* Due to C compiler bug, cannot use void */
#if	__HC__ || defined(c_plusplus)
#define novalue void
#else	/* __HC__ */
#define novalue int
#endif	/* __HC__ */

#define EXPORT_BOOLEAN
#include <mach/mach_types.h>

/* SimpleRoutine pager_create */
mig_internal novalue _Xpager_create(
#ifdef c_plusplus
	msg_header_t* InHeadP, msg_header_t* OutHeadP)
#else
	InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t new_paging_objectType;
		port_all_t new_paging_object;
		msg_type_t new_request_portType;
		port_t new_request_port;
		msg_type_t new_nameType;
		port_t new_name;
		msg_type_t new_page_sizeType;
		vm_size_t new_page_size;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *InP = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t pager_create(
#ifdef c_plusplus
		paging_object_t old_paging_object,
		port_all_t new_paging_object,
		port_t new_request_port,
		port_t new_name,
		vm_size_t new_page_size
#endif
	);

#if	TypeCheck
	if (InP->Head.msg_size != sizeof(Request))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->new_paging_objectType.msg_type_name != MSG_TYPE_PORT_ALL)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->new_request_portType.msg_type_name != MSG_TYPE_PORT)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->new_nameType.msg_type_name != MSG_TYPE_PORT)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->new_page_sizeType.msg_type_name != MSG_TYPE_INTEGER_32)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	(void) pager_create(InP->Head.msg_local_port, InP->new_paging_object, InP->new_request_port, InP->new_name, InP->new_page_size);

	OutP->RetCode = MIG_NO_REPLY;
}

/* SimpleRoutine pager_data_initialize */
mig_internal novalue _Xpager_data_initialize(
#ifdef c_plusplus
	msg_header_t* InHeadP, msg_header_t* OutHeadP)
#else
	InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t pager_request_portType;
		port_t pager_request_port;
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

	register Request *InP = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t pager_data_initialize(
#ifdef c_plusplus
		paging_object_t paging_object,
		port_t pager_request_port,
		vm_offset_t offset,
		pointer_t data,
		unsigned int dataCnt
#endif
	);

#if	TypeCheck
	if (InP->Head.msg_size != sizeof(Request))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->pager_request_portType.msg_type_name != MSG_TYPE_PORT)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->offsetType.msg_type_name != MSG_TYPE_INTEGER_32)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
	if (InP->dataType.msg_type_long_name != MSG_TYPE_INTEGER_8)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	(void) pager_data_initialize(InP->Head.msg_local_port, InP->pager_request_port, InP->offset, InP->data, InP->dataType.msg_type_long_number);

	OutP->RetCode = MIG_NO_REPLY;
}

boolean_t pager_default_server(
#ifdef c_plusplus
	msg_header_t* InHeadP, msg_header_t* OutHeadP)
#else
	InHeadP, OutHeadP)
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
	};
#endif	UseStaticMsgType

	OutP->Head.msg_local_port = InP->msg_local_port;
	OutP->Head.msg_remote_port = InP->msg_remote_port;
	OutP->Head.msg_id = InP->msg_id + 100;
	OutP->Head.msg_type = InP->msg_type;
	OutP->Head.msg_size = sizeof *OutP;

#if	UseStaticMsgType
	OutP->RetCodeType = RetCodeType;
#else	UseStaticMsgType
	OutP->RetCodeType.msg_type_name = MSG_TYPE_INTEGER_32;
	OutP->RetCodeType.msg_type_size = 32;
	OutP->RetCodeType.msg_type_number = 1;
	OutP->RetCodeType.msg_type_inline = TRUE;
	OutP->RetCodeType.msg_type_longform = FALSE;
	OutP->RetCodeType.msg_type_deallocate = FALSE;
#endif	UseStaticMsgType
	OutP->RetCode = MIG_BAD_ID;

	if ((InP->msg_id > 2251) || (InP->msg_id < 2250))
		return FALSE;
	 else {
		typedef novalue (*SERVER_STUB_PROC)(
#ifdef c_plusplus
		msg_header_t*, msg_header_t*
#endif
		);
		static SERVER_STUB_PROC routines[] = {
			_Xpager_create,
			_Xpager_data_initialize,
		};

		if (routines[InP->msg_id - 2250])
			(routines[InP->msg_id - 2250]) (InP, &OutP->Head);
		 else
			return FALSE;
	}
	return TRUE;
}
