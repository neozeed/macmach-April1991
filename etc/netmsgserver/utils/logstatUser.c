#include "logstat.h"
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

mig_external void init_logstat
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

/* Routine ls_sendlog */
mig_external kern_return_t ls_sendlog
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	log_ptr_t *old_log_ptr,
	unsigned int *old_log_ptrCnt,
	log_ptr_t *cur_log_ptr,
	unsigned int *cur_log_ptrCnt
)
#else
	(server_port, old_log_ptr, old_log_ptrCnt, cur_log_ptr, cur_log_ptrCnt)
	port_t server_port;
	log_ptr_t *old_log_ptr;
	unsigned int *old_log_ptrCnt;
	log_ptr_t *cur_log_ptr;
	unsigned int *cur_log_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t old_log_ptrType;
		log_ptr_t old_log_ptr;
		msg_type_long_t cur_log_ptrType;
		log_ptr_t cur_log_ptr;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1200;

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

	if (OutP->Head.msg_id != 1300)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 64) || (msg_simple != FALSE)) &&
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
	if ((OutP->old_log_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->old_log_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->old_log_ptrType.msg_type_long_name != MSG_TYPE_UNSTRUCTURED) ||
	    (OutP->old_log_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*old_log_ptr /* old_log_ptr */ = /* *old_log_ptr */ OutP->old_log_ptr;

	*old_log_ptrCnt /* old_log_ptrType.msg_type_long_number */ = /* *old_log_ptrCnt */ OutP->old_log_ptrType.msg_type_long_number;

#if	TypeCheck
	if ((OutP->cur_log_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->cur_log_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->cur_log_ptrType.msg_type_long_name != MSG_TYPE_UNSTRUCTURED) ||
	    (OutP->cur_log_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*cur_log_ptr /* cur_log_ptr */ = /* *cur_log_ptr */ OutP->cur_log_ptr;

	*cur_log_ptrCnt /* cur_log_ptrType.msg_type_long_number */ = /* *cur_log_ptrCnt */ OutP->cur_log_ptrType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine ls_resetlog */
mig_external kern_return_t ls_resetlog
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port
)
#else
	(server_port)
	port_t server_port;
#endif
{
	typedef struct {
		msg_header_t Head;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1201;

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

	if (OutP->Head.msg_id != 1301)
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

/* Routine ls_writelog */
mig_external kern_return_t ls_writelog
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port
)
#else
	(server_port)
	port_t server_port;
#endif
{
	typedef struct {
		msg_header_t Head;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1202;

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

	if (OutP->Head.msg_id != 1302)
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

/* Routine ls_sendstat */
mig_external kern_return_t ls_sendstat
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	stat_ptr_t *stat_ptr,
	unsigned int *stat_ptrCnt
)
#else
	(server_port, stat_ptr, stat_ptrCnt)
	port_t server_port;
	stat_ptr_t *stat_ptr;
	unsigned int *stat_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t stat_ptrType;
		stat_ptr_t stat_ptr;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1203;

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

	if (OutP->Head.msg_id != 1303)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 48) || (msg_simple != FALSE)) &&
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
	if ((OutP->stat_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->stat_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->stat_ptrType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->stat_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*stat_ptr /* stat_ptr */ = /* *stat_ptr */ OutP->stat_ptr;

	*stat_ptrCnt /* stat_ptrType.msg_type_long_number */ = /* *stat_ptrCnt */ OutP->stat_ptrType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine ls_resetstat */
mig_external kern_return_t ls_resetstat
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port
)
#else
	(server_port)
	port_t server_port;
#endif
{
	typedef struct {
		msg_header_t Head;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1204;

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

	if (OutP->Head.msg_id != 1304)
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

/* Routine ls_senddebug */
mig_external kern_return_t ls_senddebug
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	debug_ptr_t *debug_ptr,
	unsigned int *debug_ptrCnt
)
#else
	(server_port, debug_ptr, debug_ptrCnt)
	port_t server_port;
	debug_ptr_t *debug_ptr;
	unsigned int *debug_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t debug_ptrType;
		debug_ptr_t debug_ptr;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1205;

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

	if (OutP->Head.msg_id != 1305)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 48) || (msg_simple != FALSE)) &&
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
	if ((OutP->debug_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->debug_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->debug_ptrType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->debug_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*debug_ptr /* debug_ptr */ = /* *debug_ptr */ OutP->debug_ptr;

	*debug_ptrCnt /* debug_ptrType.msg_type_long_number */ = /* *debug_ptrCnt */ OutP->debug_ptrType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine ls_setdebug */
mig_external kern_return_t ls_setdebug
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	debug_ptr_t debug_ptr,
	unsigned int debug_ptrCnt
)
#else
	(server_port, debug_ptr, debug_ptrCnt)
	port_t server_port;
	debug_ptr_t debug_ptr;
	unsigned int debug_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_long_t debug_ptrType;
		debug_ptr_t debug_ptr;
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
	static msg_type_long_t debug_ptrType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTEGER_32,
		/* msg_type_long_size = */	32,
		/* msg_type_long_number = */	0
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
	InP->debug_ptrType = debug_ptrType;
#else	UseStaticMsgType
	InP->debug_ptrType.msg_type_long_name = MSG_TYPE_INTEGER_32;
	InP->debug_ptrType.msg_type_long_size = 32;
	InP->debug_ptrType.msg_type_header.msg_type_inline = FALSE;
	InP->debug_ptrType.msg_type_header.msg_type_longform = TRUE;
	InP->debug_ptrType.msg_type_header.msg_type_deallocate = FALSE;
	InP->debug_ptrType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->debug_ptr /* debug_ptr */ = /* debug_ptr */ debug_ptr;

	InP->debug_ptrType.msg_type_long_number /* debug_ptrCnt */ = /* debug_ptrType.msg_type_long_number */ debug_ptrCnt;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1206;

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

	if (OutP->Head.msg_id != 1306)
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

/* Routine ls_sendparam */
mig_external kern_return_t ls_sendparam
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	param_ptr_t *param_ptr,
	unsigned int *param_ptrCnt
)
#else
	(server_port, param_ptr, param_ptrCnt)
	port_t server_port;
	param_ptr_t *param_ptr;
	unsigned int *param_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t param_ptrType;
		param_ptr_t param_ptr;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1207;

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

	if (OutP->Head.msg_id != 1307)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 48) || (msg_simple != FALSE)) &&
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
	if ((OutP->param_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->param_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->param_ptrType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->param_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*param_ptr /* param_ptr */ = /* *param_ptr */ OutP->param_ptr;

	*param_ptrCnt /* param_ptrType.msg_type_long_number */ = /* *param_ptrCnt */ OutP->param_ptrType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine ls_setparam */
mig_external kern_return_t ls_setparam
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	param_ptr_t param_ptr,
	unsigned int param_ptrCnt
)
#else
	(server_port, param_ptr, param_ptrCnt)
	port_t server_port;
	param_ptr_t param_ptr;
	unsigned int param_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_long_t param_ptrType;
		param_ptr_t param_ptr;
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
	static msg_type_long_t param_ptrType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTEGER_32,
		/* msg_type_long_size = */	32,
		/* msg_type_long_number = */	0
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
	InP->param_ptrType = param_ptrType;
#else	UseStaticMsgType
	InP->param_ptrType.msg_type_long_name = MSG_TYPE_INTEGER_32;
	InP->param_ptrType.msg_type_long_size = 32;
	InP->param_ptrType.msg_type_header.msg_type_inline = FALSE;
	InP->param_ptrType.msg_type_header.msg_type_longform = TRUE;
	InP->param_ptrType.msg_type_header.msg_type_deallocate = FALSE;
	InP->param_ptrType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->param_ptr /* param_ptr */ = /* param_ptr */ param_ptr;

	InP->param_ptrType.msg_type_long_number /* param_ptrCnt */ = /* param_ptrType.msg_type_long_number */ param_ptrCnt;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1208;

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

	if (OutP->Head.msg_id != 1308)
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

/* Routine ls_sendportstat */
mig_external kern_return_t ls_sendportstat
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	log_ptr_t *port_stat_ptr,
	unsigned int *port_stat_ptrCnt
)
#else
	(server_port, port_stat_ptr, port_stat_ptrCnt)
	port_t server_port;
	log_ptr_t *port_stat_ptr;
	unsigned int *port_stat_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t port_stat_ptrType;
		log_ptr_t port_stat_ptr;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1209;

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

	if (OutP->Head.msg_id != 1309)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 48) || (msg_simple != FALSE)) &&
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
	if ((OutP->port_stat_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->port_stat_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->port_stat_ptrType.msg_type_long_name != MSG_TYPE_UNSTRUCTURED) ||
	    (OutP->port_stat_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*port_stat_ptr /* port_stat_ptr */ = /* *port_stat_ptr */ OutP->port_stat_ptr;

	*port_stat_ptrCnt /* port_stat_ptrType.msg_type_long_number */ = /* *port_stat_ptrCnt */ OutP->port_stat_ptrType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine ls_mem_list */
mig_external kern_return_t ls_mem_list
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	mem_class_ptr_t *class_ptr,
	unsigned int *class_ptrCnt,
	mem_nam_ptr_t *nam_ptr,
	unsigned int *nam_ptrCnt,
	mem_bucket_ptr_t *bucket_ptr,
	unsigned int *bucket_ptrCnt
)
#else
	(server_port, class_ptr, class_ptrCnt, nam_ptr, nam_ptrCnt, bucket_ptr, bucket_ptrCnt)
	port_t server_port;
	mem_class_ptr_t *class_ptr;
	unsigned int *class_ptrCnt;
	mem_nam_ptr_t *nam_ptr;
	unsigned int *nam_ptrCnt;
	mem_bucket_ptr_t *bucket_ptr;
	unsigned int *bucket_ptrCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t class_ptrType;
		mem_class_ptr_t class_ptr;
		msg_type_long_t nam_ptrType;
		mem_nam_ptr_t nam_ptr;
		msg_type_long_t bucket_ptrType;
		mem_bucket_ptr_t bucket_ptr;
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = server_port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 1210;

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

	if (OutP->Head.msg_id != 1310)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 80) || (msg_simple != FALSE)) &&
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
	if ((OutP->class_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->class_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->class_ptrType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->class_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*class_ptr /* class_ptr */ = /* *class_ptr */ OutP->class_ptr;

	*class_ptrCnt /* class_ptrType.msg_type_long_number */ = /* *class_ptrCnt */ OutP->class_ptrType.msg_type_long_number;

#if	TypeCheck
	if ((OutP->nam_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->nam_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->nam_ptrType.msg_type_long_name != MSG_TYPE_CHAR) ||
	    (OutP->nam_ptrType.msg_type_long_size != 8))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*nam_ptr /* nam_ptr */ = /* *nam_ptr */ OutP->nam_ptr;

	*nam_ptrCnt /* nam_ptrType.msg_type_long_number */ = /* *nam_ptrCnt */ OutP->nam_ptrType.msg_type_long_number;

#if	TypeCheck
	if ((OutP->bucket_ptrType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->bucket_ptrType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->bucket_ptrType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->bucket_ptrType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*bucket_ptr /* bucket_ptr */ = /* *bucket_ptr */ OutP->bucket_ptr;

	*bucket_ptrCnt /* bucket_ptrType.msg_type_long_number */ = /* *bucket_ptrCnt */ OutP->bucket_ptrType.msg_type_long_number;

	return OutP->RetCode;
}
