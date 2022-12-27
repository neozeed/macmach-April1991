#include "mach_host.h"
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

mig_external void init_mach_host
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

/* Routine host_processors */
mig_external kern_return_t host_processors
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_priv_t host_priv,
	processor_array_t *processor_list,
	unsigned int *processor_listCnt
)
#else
	(host_priv, processor_list, processor_listCnt)
	host_priv_t host_priv;
	processor_array_t *processor_list;
	unsigned int *processor_listCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t processor_listType;
		processor_array_t processor_list;
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
	InP->Head.msg_request_port = host_priv;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2600;

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

	if (OutP->Head.msg_id != 2700)
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
	if ((OutP->processor_listType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->processor_listType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->processor_listType.msg_type_long_name != MSG_TYPE_PORT) ||
	    (OutP->processor_listType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*processor_list /* processor_list */ = /* *processor_list */ OutP->processor_list;

	*processor_listCnt /* processor_listType.msg_type_long_number */ = /* *processor_listCnt */ OutP->processor_listType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine host_info */
mig_external kern_return_t host_info
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_t host,
	int flavor,
	host_info_t host_info_out,
	unsigned int *host_info_outCnt
)
#else
	(host, flavor, host_info_out, host_info_outCnt)
	host_t host;
	int flavor;
	host_info_t host_info_out;
	unsigned int *host_info_outCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t flavorType;
		int flavor;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t host_info_outType;
		int host_info_out[1024];
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
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_t flavorType = {
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
	InP->flavorType = flavorType;
#else	UseStaticMsgType
	InP->flavorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->flavorType.msg_type_size = 32;
	InP->flavorType.msg_type_number = 1;
	InP->flavorType.msg_type_inline = TRUE;
	InP->flavorType.msg_type_longform = FALSE;
	InP->flavorType.msg_type_deallocate = FALSE;
	InP->flavorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->flavor /* flavor */ = /* flavor */ flavor;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = host;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2601;

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

	if (OutP->Head.msg_id != 2701)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size < 44) || (msg_simple != TRUE)) &&
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
	if ((OutP->host_info_outType.msg_type_header.msg_type_inline != TRUE) ||
	    (OutP->host_info_outType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->host_info_outType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->host_info_outType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

#if	TypeCheck
	msg_size_delta = (4 * OutP->host_info_outType.msg_type_long_number);
	if (msg_size != 44 + msg_size_delta)
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	if (OutP->host_info_outType.msg_type_long_number / 1 > *host_info_outCnt) {
		bcopy((char *) OutP->host_info_out, (char *) host_info_out, 4 * *host_info_outCnt);
		*host_info_outCnt /* host_info_outType.msg_type_long_number 1 */ = /* *host_info_outCnt */ OutP->host_info_outType.msg_type_long_number / 1;
		return MIG_ARRAY_TOO_LARGE;
	}
	bcopy((char *) OutP->host_info_out, (char *) host_info_out, 4 * OutP->host_info_outType.msg_type_long_number);

	*host_info_outCnt /* host_info_outType.msg_type_long_number */ = /* *host_info_outCnt */ OutP->host_info_outType.msg_type_long_number;

	OutP = &Mess.Out;
	return OutP->RetCode;
}

/* Routine processor_info */
mig_external kern_return_t processor_info
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_t processor,
	int flavor,
	host_t *host,
	processor_info_t processor_info_out,
	unsigned int *processor_info_outCnt
)
#else
	(processor, flavor, host, processor_info_out, processor_info_outCnt)
	processor_t processor;
	int flavor;
	host_t *host;
	processor_info_t processor_info_out;
	unsigned int *processor_info_outCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t flavorType;
		int flavor;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t hostType;
		host_t host;
		msg_type_long_t processor_info_outType;
		int processor_info_out[1024];
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
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_t flavorType = {
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
	static msg_type_t hostCheck = {
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
	InP->flavorType = flavorType;
#else	UseStaticMsgType
	InP->flavorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->flavorType.msg_type_size = 32;
	InP->flavorType.msg_type_number = 1;
	InP->flavorType.msg_type_inline = TRUE;
	InP->flavorType.msg_type_longform = FALSE;
	InP->flavorType.msg_type_deallocate = FALSE;
	InP->flavorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->flavor /* flavor */ = /* flavor */ flavor;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = processor;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2602;

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

	if (OutP->Head.msg_id != 2702)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size < 52) || (msg_simple != FALSE)) &&
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
	if (* (int *) &OutP->hostType != * (int *) &hostCheck)
#else	UseStaticMsgType
	if ((OutP->hostType.msg_type_inline != TRUE) ||
	    (OutP->hostType.msg_type_longform != FALSE) ||
	    (OutP->hostType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->hostType.msg_type_number != 1) ||
	    (OutP->hostType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*host /* host */ = /* *host */ OutP->host;

#if	TypeCheck
	if ((OutP->processor_info_outType.msg_type_header.msg_type_inline != TRUE) ||
	    (OutP->processor_info_outType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->processor_info_outType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->processor_info_outType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

#if	TypeCheck
	msg_size_delta = (4 * OutP->processor_info_outType.msg_type_long_number);
	if (msg_size != 52 + msg_size_delta)
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	if (OutP->processor_info_outType.msg_type_long_number / 1 > *processor_info_outCnt) {
		bcopy((char *) OutP->processor_info_out, (char *) processor_info_out, 4 * *processor_info_outCnt);
		*processor_info_outCnt /* processor_info_outType.msg_type_long_number 1 */ = /* *processor_info_outCnt */ OutP->processor_info_outType.msg_type_long_number / 1;
		return MIG_ARRAY_TOO_LARGE;
	}
	bcopy((char *) OutP->processor_info_out, (char *) processor_info_out, 4 * OutP->processor_info_outType.msg_type_long_number);

	*processor_info_outCnt /* processor_info_outType.msg_type_long_number */ = /* *processor_info_outCnt */ OutP->processor_info_outType.msg_type_long_number;

	OutP = &Mess.Out;
	return OutP->RetCode;
}

/* Routine processor_start */
mig_external kern_return_t processor_start
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_t processor
)
#else
	(processor)
	processor_t processor;
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
	InP->Head.msg_request_port = processor;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2603;

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

	if (OutP->Head.msg_id != 2703)
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

/* Routine processor_exit */
mig_external kern_return_t processor_exit
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_t processor
)
#else
	(processor)
	processor_t processor;
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
	InP->Head.msg_request_port = processor;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2604;

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

	if (OutP->Head.msg_id != 2704)
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

/* Routine processor_control */
mig_external kern_return_t processor_control
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_t processor,
	processor_info_t processor_cmd,
	unsigned int processor_cmdCnt
)
#else
	(processor, processor_cmd, processor_cmdCnt)
	processor_t processor;
	processor_info_t processor_cmd;
	unsigned int processor_cmdCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_long_t processor_cmdType;
		int processor_cmd[1024];
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
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_long_t processor_cmdType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTEGER_32,
		/* msg_type_long_size = */	32,
		/* msg_type_long_number = */	1024
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
	InP->processor_cmdType = processor_cmdType;
#else	UseStaticMsgType
	InP->processor_cmdType.msg_type_long_name = MSG_TYPE_INTEGER_32;
	InP->processor_cmdType.msg_type_long_size = 32;
	InP->processor_cmdType.msg_type_header.msg_type_inline = TRUE;
	InP->processor_cmdType.msg_type_header.msg_type_longform = TRUE;
	InP->processor_cmdType.msg_type_header.msg_type_deallocate = FALSE;
	InP->processor_cmdType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	if (processor_cmdCnt > 1024)
		return MIG_ARRAY_TOO_LARGE;
	bcopy((char *) processor_cmd, (char *) InP->processor_cmd, 4 * processor_cmdCnt);

	InP->processor_cmdType.msg_type_long_number /* processor_cmdCnt */ = /* processor_cmdType.msg_type_long_number */ processor_cmdCnt;

	msg_size_delta = (4 * processor_cmdCnt);
	msg_size += msg_size_delta;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = processor;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2605;

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

	if (OutP->Head.msg_id != 2705)
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

/* Routine processor_set_default */
mig_external kern_return_t processor_set_default
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_t host,
	processor_set_name_t *default_set
)
#else
	(host, default_set)
	host_t host;
	processor_set_name_t *default_set;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t default_setType;
		processor_set_name_t default_set;
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
	static msg_type_t default_setCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
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
	InP->Head.msg_request_port = host;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2606;

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

	if (OutP->Head.msg_id != 2706)
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
	if (* (int *) &OutP->default_setType != * (int *) &default_setCheck)
#else	UseStaticMsgType
	if ((OutP->default_setType.msg_type_inline != TRUE) ||
	    (OutP->default_setType.msg_type_longform != FALSE) ||
	    (OutP->default_setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->default_setType.msg_type_number != 1) ||
	    (OutP->default_setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*default_set /* default_set */ = /* *default_set */ OutP->default_set;

	return OutP->RetCode;
}

/* Routine xxx_processor_set_default_priv */
mig_external kern_return_t xxx_processor_set_default_priv
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_priv_t host,
	processor_set_t *default_set
)
#else
	(host, default_set)
	host_priv_t host;
	processor_set_t *default_set;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t default_setType;
		processor_set_t default_set;
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
	static msg_type_t default_setCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
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
	InP->Head.msg_request_port = host;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2607;

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

	if (OutP->Head.msg_id != 2707)
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
	if (* (int *) &OutP->default_setType != * (int *) &default_setCheck)
#else	UseStaticMsgType
	if ((OutP->default_setType.msg_type_inline != TRUE) ||
	    (OutP->default_setType.msg_type_longform != FALSE) ||
	    (OutP->default_setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->default_setType.msg_type_number != 1) ||
	    (OutP->default_setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*default_set /* default_set */ = /* *default_set */ OutP->default_set;

	return OutP->RetCode;
}

/* Routine processor_set_create */
mig_external kern_return_t processor_set_create
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_t host,
	port_t *new_set,
	port_t *new_name
)
#else
	(host, new_set, new_name)
	host_t host;
	port_t *new_set;
	port_t *new_name;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t new_setType;
		port_t new_set;
		msg_type_t new_nameType;
		port_t new_name;
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
	static msg_type_t new_setCheck = {
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

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = host;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2608;

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

	if (OutP->Head.msg_id != 2708)
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
#if	UseStaticMsgType
	if (* (int *) &OutP->new_setType != * (int *) &new_setCheck)
#else	UseStaticMsgType
	if ((OutP->new_setType.msg_type_inline != TRUE) ||
	    (OutP->new_setType.msg_type_longform != FALSE) ||
	    (OutP->new_setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->new_setType.msg_type_number != 1) ||
	    (OutP->new_setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*new_set /* new_set */ = /* *new_set */ OutP->new_set;

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &OutP->new_nameType != * (int *) &new_nameCheck)
#else	UseStaticMsgType
	if ((OutP->new_nameType.msg_type_inline != TRUE) ||
	    (OutP->new_nameType.msg_type_longform != FALSE) ||
	    (OutP->new_nameType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->new_nameType.msg_type_number != 1) ||
	    (OutP->new_nameType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*new_name /* new_name */ = /* *new_name */ OutP->new_name;

	return OutP->RetCode;
}

/* Routine processor_set_destroy */
mig_external kern_return_t processor_set_destroy
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_t set
)
#else
	(set)
	processor_set_t set;
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
	InP->Head.msg_request_port = set;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2609;

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

	if (OutP->Head.msg_id != 2709)
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

/* Routine processor_set_info */
mig_external kern_return_t processor_set_info
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_name_t set_name,
	int flavor,
	host_t *host,
	processor_set_info_t info_out,
	unsigned int *info_outCnt
)
#else
	(set_name, flavor, host, info_out, info_outCnt)
	processor_set_name_t set_name;
	int flavor;
	host_t *host;
	processor_set_info_t info_out;
	unsigned int *info_outCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t flavorType;
		int flavor;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t hostType;
		host_t host;
		msg_type_long_t info_outType;
		int info_out[1024];
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
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_t flavorType = {
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
	static msg_type_t hostCheck = {
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
	InP->flavorType = flavorType;
#else	UseStaticMsgType
	InP->flavorType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->flavorType.msg_type_size = 32;
	InP->flavorType.msg_type_number = 1;
	InP->flavorType.msg_type_inline = TRUE;
	InP->flavorType.msg_type_longform = FALSE;
	InP->flavorType.msg_type_deallocate = FALSE;
	InP->flavorType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->flavor /* flavor */ = /* flavor */ flavor;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = set_name;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2610;

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

	if (OutP->Head.msg_id != 2710)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size < 52) || (msg_simple != FALSE)) &&
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
	if (* (int *) &OutP->hostType != * (int *) &hostCheck)
#else	UseStaticMsgType
	if ((OutP->hostType.msg_type_inline != TRUE) ||
	    (OutP->hostType.msg_type_longform != FALSE) ||
	    (OutP->hostType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->hostType.msg_type_number != 1) ||
	    (OutP->hostType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*host /* host */ = /* *host */ OutP->host;

#if	TypeCheck
	if ((OutP->info_outType.msg_type_header.msg_type_inline != TRUE) ||
	    (OutP->info_outType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->info_outType.msg_type_long_name != MSG_TYPE_INTEGER_32) ||
	    (OutP->info_outType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

#if	TypeCheck
	msg_size_delta = (4 * OutP->info_outType.msg_type_long_number);
	if (msg_size != 52 + msg_size_delta)
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	if (OutP->info_outType.msg_type_long_number / 1 > *info_outCnt) {
		bcopy((char *) OutP->info_out, (char *) info_out, 4 * *info_outCnt);
		*info_outCnt /* info_outType.msg_type_long_number 1 */ = /* *info_outCnt */ OutP->info_outType.msg_type_long_number / 1;
		return MIG_ARRAY_TOO_LARGE;
	}
	bcopy((char *) OutP->info_out, (char *) info_out, 4 * OutP->info_outType.msg_type_long_number);

	*info_outCnt /* info_outType.msg_type_long_number */ = /* *info_outCnt */ OutP->info_outType.msg_type_long_number;

	OutP = &Mess.Out;
	return OutP->RetCode;
}

/* Routine processor_assign */
mig_external kern_return_t processor_assign
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_t processor,
	processor_set_t new_set,
	boolean_t wait
)
#else
	(processor, new_set, wait)
	processor_t processor;
	processor_set_t new_set;
	boolean_t wait;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t new_setType;
		processor_set_t new_set;
		msg_type_t waitType;
		boolean_t wait;
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
	static msg_type_t new_setType = {
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
	static msg_type_t waitType = {
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
	InP->new_setType = new_setType;
#else	UseStaticMsgType
	InP->new_setType.msg_type_name = MSG_TYPE_PORT;
	InP->new_setType.msg_type_size = 32;
	InP->new_setType.msg_type_number = 1;
	InP->new_setType.msg_type_inline = TRUE;
	InP->new_setType.msg_type_longform = FALSE;
	InP->new_setType.msg_type_deallocate = FALSE;
	InP->new_setType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_set /* new_set */ = /* new_set */ new_set;

#if	UseStaticMsgType
	InP->waitType = waitType;
#else	UseStaticMsgType
	InP->waitType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->waitType.msg_type_size = 32;
	InP->waitType.msg_type_number = 1;
	InP->waitType.msg_type_inline = TRUE;
	InP->waitType.msg_type_longform = FALSE;
	InP->waitType.msg_type_deallocate = FALSE;
	InP->waitType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->wait /* wait */ = /* wait */ wait;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = processor;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2611;

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

	if (OutP->Head.msg_id != 2711)
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

/* Routine processor_get_assignment */
mig_external kern_return_t processor_get_assignment
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_t processor,
	processor_set_name_t *assigned_set
)
#else
	(processor, assigned_set)
	processor_t processor;
	processor_set_name_t *assigned_set;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t assigned_setType;
		processor_set_name_t assigned_set;
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
	static msg_type_t assigned_setCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
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
	InP->Head.msg_request_port = processor;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2612;

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

	if (OutP->Head.msg_id != 2712)
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
	if (* (int *) &OutP->assigned_setType != * (int *) &assigned_setCheck)
#else	UseStaticMsgType
	if ((OutP->assigned_setType.msg_type_inline != TRUE) ||
	    (OutP->assigned_setType.msg_type_longform != FALSE) ||
	    (OutP->assigned_setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->assigned_setType.msg_type_number != 1) ||
	    (OutP->assigned_setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*assigned_set /* assigned_set */ = /* *assigned_set */ OutP->assigned_set;

	return OutP->RetCode;
}

/* Routine thread_assign */
mig_external kern_return_t thread_assign
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	processor_set_t new_set
)
#else
	(thread, new_set)
	thread_t thread;
	processor_set_t new_set;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t new_setType;
		processor_set_t new_set;
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
	static msg_type_t new_setType = {
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
	InP->new_setType = new_setType;
#else	UseStaticMsgType
	InP->new_setType.msg_type_name = MSG_TYPE_PORT;
	InP->new_setType.msg_type_size = 32;
	InP->new_setType.msg_type_number = 1;
	InP->new_setType.msg_type_inline = TRUE;
	InP->new_setType.msg_type_longform = FALSE;
	InP->new_setType.msg_type_deallocate = FALSE;
	InP->new_setType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_set /* new_set */ = /* new_set */ new_set;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2613;

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

	if (OutP->Head.msg_id != 2713)
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

/* Routine thread_assign_default */
mig_external kern_return_t thread_assign_default
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread
)
#else
	(thread)
	thread_t thread;
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
	InP->Head.msg_request_port = thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2614;

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

	if (OutP->Head.msg_id != 2714)
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

/* Routine thread_get_assignment */
mig_external kern_return_t thread_get_assignment
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	processor_set_name_t *assigned_set
)
#else
	(thread, assigned_set)
	thread_t thread;
	processor_set_name_t *assigned_set;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t assigned_setType;
		processor_set_name_t assigned_set;
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
	static msg_type_t assigned_setCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
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
	InP->Head.msg_request_port = thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2615;

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

	if (OutP->Head.msg_id != 2715)
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
	if (* (int *) &OutP->assigned_setType != * (int *) &assigned_setCheck)
#else	UseStaticMsgType
	if ((OutP->assigned_setType.msg_type_inline != TRUE) ||
	    (OutP->assigned_setType.msg_type_longform != FALSE) ||
	    (OutP->assigned_setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->assigned_setType.msg_type_number != 1) ||
	    (OutP->assigned_setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*assigned_set /* assigned_set */ = /* *assigned_set */ OutP->assigned_set;

	return OutP->RetCode;
}

/* Routine task_assign */
mig_external kern_return_t task_assign
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	processor_set_t new_set,
	boolean_t assign_threads
)
#else
	(task, new_set, assign_threads)
	task_t task;
	processor_set_t new_set;
	boolean_t assign_threads;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t new_setType;
		processor_set_t new_set;
		msg_type_t assign_threadsType;
		boolean_t assign_threads;
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
	static msg_type_t new_setType = {
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
	static msg_type_t assign_threadsType = {
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
	InP->new_setType = new_setType;
#else	UseStaticMsgType
	InP->new_setType.msg_type_name = MSG_TYPE_PORT;
	InP->new_setType.msg_type_size = 32;
	InP->new_setType.msg_type_number = 1;
	InP->new_setType.msg_type_inline = TRUE;
	InP->new_setType.msg_type_longform = FALSE;
	InP->new_setType.msg_type_deallocate = FALSE;
	InP->new_setType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->new_set /* new_set */ = /* new_set */ new_set;

#if	UseStaticMsgType
	InP->assign_threadsType = assign_threadsType;
#else	UseStaticMsgType
	InP->assign_threadsType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->assign_threadsType.msg_type_size = 32;
	InP->assign_threadsType.msg_type_number = 1;
	InP->assign_threadsType.msg_type_inline = TRUE;
	InP->assign_threadsType.msg_type_longform = FALSE;
	InP->assign_threadsType.msg_type_deallocate = FALSE;
	InP->assign_threadsType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->assign_threads /* assign_threads */ = /* assign_threads */ assign_threads;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2616;

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

	if (OutP->Head.msg_id != 2716)
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

/* Routine task_assign_default */
mig_external kern_return_t task_assign_default
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	boolean_t assign_threads
)
#else
	(task, assign_threads)
	task_t task;
	boolean_t assign_threads;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t assign_threadsType;
		boolean_t assign_threads;
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
	static msg_type_t assign_threadsType = {
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
	InP->assign_threadsType = assign_threadsType;
#else	UseStaticMsgType
	InP->assign_threadsType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->assign_threadsType.msg_type_size = 32;
	InP->assign_threadsType.msg_type_number = 1;
	InP->assign_threadsType.msg_type_inline = TRUE;
	InP->assign_threadsType.msg_type_longform = FALSE;
	InP->assign_threadsType.msg_type_deallocate = FALSE;
	InP->assign_threadsType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->assign_threads /* assign_threads */ = /* assign_threads */ assign_threads;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2617;

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

	if (OutP->Head.msg_id != 2717)
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

/* Routine task_get_assignment */
mig_external kern_return_t task_get_assignment
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	processor_set_name_t *assigned_set
)
#else
	(task, assigned_set)
	task_t task;
	processor_set_name_t *assigned_set;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t assigned_setType;
		processor_set_name_t assigned_set;
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
	static msg_type_t assigned_setCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
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
	InP->Head.msg_request_port = task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2618;

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

	if (OutP->Head.msg_id != 2718)
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
	if (* (int *) &OutP->assigned_setType != * (int *) &assigned_setCheck)
#else	UseStaticMsgType
	if ((OutP->assigned_setType.msg_type_inline != TRUE) ||
	    (OutP->assigned_setType.msg_type_longform != FALSE) ||
	    (OutP->assigned_setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->assigned_setType.msg_type_number != 1) ||
	    (OutP->assigned_setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*assigned_set /* assigned_set */ = /* *assigned_set */ OutP->assigned_set;

	return OutP->RetCode;
}

/* Routine host_kernel_version */
mig_external kern_return_t host_kernel_version
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_t host,
	kernel_version_t kernel_version
)
#else
	(host, kernel_version)
	host_t host;
	kernel_version_t kernel_version;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t kernel_versionType;
		kernel_version_t kernel_version;
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
	InP->Head.msg_request_port = host;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2619;

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

	if (OutP->Head.msg_id != 2719)
		return MIG_REPLY_MISMATCH;

#if	TypeCheck
	if (((msg_size != 556) || (msg_simple != TRUE)) &&
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
	if ((OutP->kernel_versionType.msg_type_header.msg_type_inline != TRUE) ||
	    (OutP->kernel_versionType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->kernel_versionType.msg_type_long_name != MSG_TYPE_STRING) ||
	    (OutP->kernel_versionType.msg_type_long_number != 1) ||
	    (OutP->kernel_versionType.msg_type_long_size != 4096))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	(void) mig_strncpy(kernel_version /* kernel_version */, /* kernel_version */ OutP->kernel_version, 512);
	kernel_version /* kernel_version */[511] = '\0';

	return OutP->RetCode;
}

/* Routine thread_priority */
mig_external kern_return_t thread_priority
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	int priority,
	boolean_t set_max
)
#else
	(thread, priority, set_max)
	thread_t thread;
	int priority;
	boolean_t set_max;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t priorityType;
		int priority;
		msg_type_t set_maxType;
		boolean_t set_max;
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
	static msg_type_t priorityType = {
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
	static msg_type_t set_maxType = {
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
	InP->priorityType = priorityType;
#else	UseStaticMsgType
	InP->priorityType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->priorityType.msg_type_size = 32;
	InP->priorityType.msg_type_number = 1;
	InP->priorityType.msg_type_inline = TRUE;
	InP->priorityType.msg_type_longform = FALSE;
	InP->priorityType.msg_type_deallocate = FALSE;
	InP->priorityType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->priority /* priority */ = /* priority */ priority;

#if	UseStaticMsgType
	InP->set_maxType = set_maxType;
#else	UseStaticMsgType
	InP->set_maxType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->set_maxType.msg_type_size = 32;
	InP->set_maxType.msg_type_number = 1;
	InP->set_maxType.msg_type_inline = TRUE;
	InP->set_maxType.msg_type_longform = FALSE;
	InP->set_maxType.msg_type_deallocate = FALSE;
	InP->set_maxType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->set_max /* set_max */ = /* set_max */ set_max;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2620;

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

	if (OutP->Head.msg_id != 2720)
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

/* Routine thread_max_priority */
mig_external kern_return_t thread_max_priority
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	processor_set_t processor_set,
	int max_priority
)
#else
	(thread, processor_set, max_priority)
	thread_t thread;
	processor_set_t processor_set;
	int max_priority;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t processor_setType;
		processor_set_t processor_set;
		msg_type_t max_priorityType;
		int max_priority;
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
	static msg_type_t processor_setType = {
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
	static msg_type_t max_priorityType = {
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
	InP->processor_setType = processor_setType;
#else	UseStaticMsgType
	InP->processor_setType.msg_type_name = MSG_TYPE_PORT;
	InP->processor_setType.msg_type_size = 32;
	InP->processor_setType.msg_type_number = 1;
	InP->processor_setType.msg_type_inline = TRUE;
	InP->processor_setType.msg_type_longform = FALSE;
	InP->processor_setType.msg_type_deallocate = FALSE;
	InP->processor_setType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->processor_set /* processor_set */ = /* processor_set */ processor_set;

#if	UseStaticMsgType
	InP->max_priorityType = max_priorityType;
#else	UseStaticMsgType
	InP->max_priorityType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->max_priorityType.msg_type_size = 32;
	InP->max_priorityType.msg_type_number = 1;
	InP->max_priorityType.msg_type_inline = TRUE;
	InP->max_priorityType.msg_type_longform = FALSE;
	InP->max_priorityType.msg_type_deallocate = FALSE;
	InP->max_priorityType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->max_priority /* max_priority */ = /* max_priority */ max_priority;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2621;

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

	if (OutP->Head.msg_id != 2721)
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

/* Routine task_priority */
mig_external kern_return_t task_priority
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	int priority,
	boolean_t change_threads
)
#else
	(task, priority, change_threads)
	task_t task;
	int priority;
	boolean_t change_threads;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t priorityType;
		int priority;
		msg_type_t change_threadsType;
		boolean_t change_threads;
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
	static msg_type_t priorityType = {
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
	static msg_type_t change_threadsType = {
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
	InP->priorityType = priorityType;
#else	UseStaticMsgType
	InP->priorityType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->priorityType.msg_type_size = 32;
	InP->priorityType.msg_type_number = 1;
	InP->priorityType.msg_type_inline = TRUE;
	InP->priorityType.msg_type_longform = FALSE;
	InP->priorityType.msg_type_deallocate = FALSE;
	InP->priorityType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->priority /* priority */ = /* priority */ priority;

#if	UseStaticMsgType
	InP->change_threadsType = change_threadsType;
#else	UseStaticMsgType
	InP->change_threadsType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->change_threadsType.msg_type_size = 32;
	InP->change_threadsType.msg_type_number = 1;
	InP->change_threadsType.msg_type_inline = TRUE;
	InP->change_threadsType.msg_type_longform = FALSE;
	InP->change_threadsType.msg_type_deallocate = FALSE;
	InP->change_threadsType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->change_threads /* change_threads */ = /* change_threads */ change_threads;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = task;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2622;

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

	if (OutP->Head.msg_id != 2722)
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

/* Routine processor_set_max_priority */
mig_external kern_return_t processor_set_max_priority
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_t processor_set,
	int max_priority,
	boolean_t change_threads
)
#else
	(processor_set, max_priority, change_threads)
	processor_set_t processor_set;
	int max_priority;
	boolean_t change_threads;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t max_priorityType;
		int max_priority;
		msg_type_t change_threadsType;
		boolean_t change_threads;
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
	static msg_type_t max_priorityType = {
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
	static msg_type_t change_threadsType = {
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
	InP->max_priorityType = max_priorityType;
#else	UseStaticMsgType
	InP->max_priorityType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->max_priorityType.msg_type_size = 32;
	InP->max_priorityType.msg_type_number = 1;
	InP->max_priorityType.msg_type_inline = TRUE;
	InP->max_priorityType.msg_type_longform = FALSE;
	InP->max_priorityType.msg_type_deallocate = FALSE;
	InP->max_priorityType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->max_priority /* max_priority */ = /* max_priority */ max_priority;

#if	UseStaticMsgType
	InP->change_threadsType = change_threadsType;
#else	UseStaticMsgType
	InP->change_threadsType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->change_threadsType.msg_type_size = 32;
	InP->change_threadsType.msg_type_number = 1;
	InP->change_threadsType.msg_type_inline = TRUE;
	InP->change_threadsType.msg_type_longform = FALSE;
	InP->change_threadsType.msg_type_deallocate = FALSE;
	InP->change_threadsType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->change_threads /* change_threads */ = /* change_threads */ change_threads;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = processor_set;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2623;

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

	if (OutP->Head.msg_id != 2723)
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

/* Routine thread_policy */
mig_external kern_return_t thread_policy
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	int policy,
	int data
)
#else
	(thread, policy, data)
	thread_t thread;
	int policy;
	int data;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t policyType;
		int policy;
		msg_type_t dataType;
		int data;
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
	static msg_type_t policyType = {
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
	static msg_type_t dataType = {
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
	InP->policyType = policyType;
#else	UseStaticMsgType
	InP->policyType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->policyType.msg_type_size = 32;
	InP->policyType.msg_type_number = 1;
	InP->policyType.msg_type_inline = TRUE;
	InP->policyType.msg_type_longform = FALSE;
	InP->policyType.msg_type_deallocate = FALSE;
	InP->policyType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->policy /* policy */ = /* policy */ policy;

#if	UseStaticMsgType
	InP->dataType = dataType;
#else	UseStaticMsgType
	InP->dataType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->dataType.msg_type_size = 32;
	InP->dataType.msg_type_number = 1;
	InP->dataType.msg_type_inline = TRUE;
	InP->dataType.msg_type_longform = FALSE;
	InP->dataType.msg_type_deallocate = FALSE;
	InP->dataType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->data /* data */ = /* data */ data;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = thread;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2624;

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

	if (OutP->Head.msg_id != 2724)
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

/* Routine processor_set_policy_enable */
mig_external kern_return_t processor_set_policy_enable
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_t processor_set,
	int policy
)
#else
	(processor_set, policy)
	processor_set_t processor_set;
	int policy;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t policyType;
		int policy;
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
	static msg_type_t policyType = {
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
	InP->policyType = policyType;
#else	UseStaticMsgType
	InP->policyType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->policyType.msg_type_size = 32;
	InP->policyType.msg_type_number = 1;
	InP->policyType.msg_type_inline = TRUE;
	InP->policyType.msg_type_longform = FALSE;
	InP->policyType.msg_type_deallocate = FALSE;
	InP->policyType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->policy /* policy */ = /* policy */ policy;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = processor_set;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2625;

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

	if (OutP->Head.msg_id != 2725)
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

/* Routine processor_set_policy_disable */
mig_external kern_return_t processor_set_policy_disable
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_t processor_set,
	int policy,
	boolean_t change_threads
)
#else
	(processor_set, policy, change_threads)
	processor_set_t processor_set;
	int policy;
	boolean_t change_threads;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t policyType;
		int policy;
		msg_type_t change_threadsType;
		boolean_t change_threads;
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
	static msg_type_t policyType = {
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
	static msg_type_t change_threadsType = {
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
	InP->policyType = policyType;
#else	UseStaticMsgType
	InP->policyType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->policyType.msg_type_size = 32;
	InP->policyType.msg_type_number = 1;
	InP->policyType.msg_type_inline = TRUE;
	InP->policyType.msg_type_longform = FALSE;
	InP->policyType.msg_type_deallocate = FALSE;
	InP->policyType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->policy /* policy */ = /* policy */ policy;

#if	UseStaticMsgType
	InP->change_threadsType = change_threadsType;
#else	UseStaticMsgType
	InP->change_threadsType.msg_type_name = MSG_TYPE_BOOLEAN;
	InP->change_threadsType.msg_type_size = 32;
	InP->change_threadsType.msg_type_number = 1;
	InP->change_threadsType.msg_type_inline = TRUE;
	InP->change_threadsType.msg_type_longform = FALSE;
	InP->change_threadsType.msg_type_deallocate = FALSE;
	InP->change_threadsType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->change_threads /* change_threads */ = /* change_threads */ change_threads;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = processor_set;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2626;

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

	if (OutP->Head.msg_id != 2726)
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

/* Routine processor_set_tasks */
mig_external kern_return_t processor_set_tasks
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_t processor_set,
	task_array_t *task_list,
	unsigned int *task_listCnt
)
#else
	(processor_set, task_list, task_listCnt)
	processor_set_t processor_set;
	task_array_t *task_list;
	unsigned int *task_listCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t task_listType;
		task_array_t task_list;
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
	InP->Head.msg_request_port = processor_set;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2627;

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

	if (OutP->Head.msg_id != 2727)
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
	if ((OutP->task_listType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->task_listType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->task_listType.msg_type_long_name != MSG_TYPE_PORT) ||
	    (OutP->task_listType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*task_list /* task_list */ = /* *task_list */ OutP->task_list;

	*task_listCnt /* task_listType.msg_type_long_number */ = /* *task_listCnt */ OutP->task_listType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine processor_set_threads */
mig_external kern_return_t processor_set_threads
#if	(defined(__STDC__) || defined(c_plusplus))
(
	processor_set_t processor_set,
	thread_array_t *thread_list,
	unsigned int *thread_listCnt
)
#else
	(processor_set, thread_list, thread_listCnt)
	processor_set_t processor_set;
	thread_array_t *thread_list;
	unsigned int *thread_listCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t thread_listType;
		thread_array_t thread_list;
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
	InP->Head.msg_request_port = processor_set;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2628;

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

	if (OutP->Head.msg_id != 2728)
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
	if ((OutP->thread_listType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->thread_listType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->thread_listType.msg_type_long_name != MSG_TYPE_PORT) ||
	    (OutP->thread_listType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*thread_list /* thread_list */ = /* *thread_list */ OutP->thread_list;

	*thread_listCnt /* thread_listType.msg_type_long_number */ = /* *thread_listCnt */ OutP->thread_listType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine host_processor_sets */
mig_external kern_return_t host_processor_sets
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_t host,
	processor_set_name_array_t *processor_sets,
	unsigned int *processor_setsCnt
)
#else
	(host, processor_sets, processor_setsCnt)
	host_t host;
	processor_set_name_array_t *processor_sets;
	unsigned int *processor_setsCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t processor_setsType;
		processor_set_name_array_t processor_sets;
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
	InP->Head.msg_request_port = host;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2629;

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

	if (OutP->Head.msg_id != 2729)
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
	if ((OutP->processor_setsType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->processor_setsType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->processor_setsType.msg_type_long_name != MSG_TYPE_PORT) ||
	    (OutP->processor_setsType.msg_type_long_size != 32))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*processor_sets /* processor_sets */ = /* *processor_sets */ OutP->processor_sets;

	*processor_setsCnt /* processor_setsType.msg_type_long_number */ = /* *processor_setsCnt */ OutP->processor_setsType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine host_processor_set_priv */
mig_external kern_return_t host_processor_set_priv
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_priv_t host_priv,
	processor_set_name_t set_name,
	processor_set_t *set
)
#else
	(host_priv, set_name, set)
	host_priv_t host_priv;
	processor_set_name_t set_name;
	processor_set_t *set;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t set_nameType;
		processor_set_name_t set_name;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t setType;
		processor_set_t set;
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
	static msg_type_t set_nameType = {
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
	static msg_type_t setCheck = {
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
	InP->set_nameType = set_nameType;
#else	UseStaticMsgType
	InP->set_nameType.msg_type_name = MSG_TYPE_PORT;
	InP->set_nameType.msg_type_size = 32;
	InP->set_nameType.msg_type_number = 1;
	InP->set_nameType.msg_type_inline = TRUE;
	InP->set_nameType.msg_type_longform = FALSE;
	InP->set_nameType.msg_type_deallocate = FALSE;
	InP->set_nameType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->set_name /* set_name */ = /* set_name */ set_name;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = host_priv;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2630;

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

	if (OutP->Head.msg_id != 2730)
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
	if (* (int *) &OutP->setType != * (int *) &setCheck)
#else	UseStaticMsgType
	if ((OutP->setType.msg_type_inline != TRUE) ||
	    (OutP->setType.msg_type_longform != FALSE) ||
	    (OutP->setType.msg_type_name != MSG_TYPE_PORT) ||
	    (OutP->setType.msg_type_number != 1) ||
	    (OutP->setType.msg_type_size != 32))
#endif	UseStaticMsgType
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*set /* set */ = /* *set */ OutP->set;

	return OutP->RetCode;
}
