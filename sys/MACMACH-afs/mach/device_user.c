#include "device.h"
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

mig_external void init_device
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

/* SimpleRoutine block_write */
mig_external kern_return_t block_write
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	int block_size,
	io_buf_t block
)
#else
	(port, device, block_number, block_size, block)
	port_t port;
	dev_t device;
	daddr_t block_number;
	int block_size;
	io_buf_t block;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t deviceType;
		dev_t device;
		char devicePad[2];
		msg_type_t block_numberType;
		daddr_t block_number;
		msg_type_t block_sizeType;
		int block_size;
		msg_type_t blockType;
		io_buf_t block;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t deviceType = {
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
	static msg_type_t block_numberType = {
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
	static msg_type_t block_sizeType = {
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
	static msg_type_t blockType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->deviceType = deviceType;
#else	UseStaticMsgType
	InP->deviceType.msg_type_name = MSG_TYPE_INTEGER_16;
	InP->deviceType.msg_type_size = 16;
	InP->deviceType.msg_type_number = 1;
	InP->deviceType.msg_type_inline = TRUE;
	InP->deviceType.msg_type_longform = FALSE;
	InP->deviceType.msg_type_deallocate = FALSE;
	InP->deviceType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->device /* device */ = /* device */ device;

#if	UseStaticMsgType
	InP->block_numberType = block_numberType;
#else	UseStaticMsgType
	InP->block_numberType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_numberType.msg_type_size = 32;
	InP->block_numberType.msg_type_number = 1;
	InP->block_numberType.msg_type_inline = TRUE;
	InP->block_numberType.msg_type_longform = FALSE;
	InP->block_numberType.msg_type_deallocate = FALSE;
	InP->block_numberType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_number /* block_number */ = /* block_number */ block_number;

#if	UseStaticMsgType
	InP->block_sizeType = block_sizeType;
#else	UseStaticMsgType
	InP->block_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_sizeType.msg_type_size = 32;
	InP->block_sizeType.msg_type_number = 1;
	InP->block_sizeType.msg_type_inline = TRUE;
	InP->block_sizeType.msg_type_longform = FALSE;
	InP->block_sizeType.msg_type_deallocate = FALSE;
	InP->block_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_size /* block_size */ = /* block_size */ block_size;

#if	UseStaticMsgType
	InP->blockType = blockType;
#else	UseStaticMsgType
	InP->blockType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->blockType.msg_type_size = 32;
	InP->blockType.msg_type_number = 1;
	InP->blockType.msg_type_inline = FALSE;
	InP->blockType.msg_type_longform = FALSE;
	InP->blockType.msg_type_deallocate = FALSE;
	InP->blockType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block /* block */ = /* block */ block;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = port;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2500;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* SimpleRoutine block_write_async */
mig_external kern_return_t block_write_async
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	int block_size,
	io_buf_t block
)
#else
	(port, device, block_number, block_size, block)
	port_t port;
	dev_t device;
	daddr_t block_number;
	int block_size;
	io_buf_t block;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t deviceType;
		dev_t device;
		char devicePad[2];
		msg_type_t block_numberType;
		daddr_t block_number;
		msg_type_t block_sizeType;
		int block_size;
		msg_type_t blockType;
		io_buf_t block;
	} Request;

	union {
		Request In;
	} Mess;

	register Request *InP = &Mess.In;

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t deviceType = {
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
	static msg_type_t block_numberType = {
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
	static msg_type_t block_sizeType = {
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
	static msg_type_t blockType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

#if	UseStaticMsgType
	InP->deviceType = deviceType;
#else	UseStaticMsgType
	InP->deviceType.msg_type_name = MSG_TYPE_INTEGER_16;
	InP->deviceType.msg_type_size = 16;
	InP->deviceType.msg_type_number = 1;
	InP->deviceType.msg_type_inline = TRUE;
	InP->deviceType.msg_type_longform = FALSE;
	InP->deviceType.msg_type_deallocate = FALSE;
	InP->deviceType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->device /* device */ = /* device */ device;

#if	UseStaticMsgType
	InP->block_numberType = block_numberType;
#else	UseStaticMsgType
	InP->block_numberType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_numberType.msg_type_size = 32;
	InP->block_numberType.msg_type_number = 1;
	InP->block_numberType.msg_type_inline = TRUE;
	InP->block_numberType.msg_type_longform = FALSE;
	InP->block_numberType.msg_type_deallocate = FALSE;
	InP->block_numberType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_number /* block_number */ = /* block_number */ block_number;

#if	UseStaticMsgType
	InP->block_sizeType = block_sizeType;
#else	UseStaticMsgType
	InP->block_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_sizeType.msg_type_size = 32;
	InP->block_sizeType.msg_type_number = 1;
	InP->block_sizeType.msg_type_inline = TRUE;
	InP->block_sizeType.msg_type_longform = FALSE;
	InP->block_sizeType.msg_type_deallocate = FALSE;
	InP->block_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_size /* block_size */ = /* block_size */ block_size;

#if	UseStaticMsgType
	InP->blockType = blockType;
#else	UseStaticMsgType
	InP->blockType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->blockType.msg_type_size = 32;
	InP->blockType.msg_type_number = 1;
	InP->blockType.msg_type_inline = FALSE;
	InP->blockType.msg_type_longform = FALSE;
	InP->blockType.msg_type_deallocate = FALSE;
	InP->blockType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block /* block */ = /* block */ block;

	InP->Head.msg_simple = FALSE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL;
	InP->Head.msg_request_port = port;
	InP->Head.msg_reply_port = PORT_NULL;
	InP->Head.msg_id = 2501;

	return msg_send(&InP->Head, MSG_OPTION_NONE, 0);
}

/* Routine block_read */
mig_external kern_return_t block_read
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	int block_size,
	io_buf_ptr_t *block,
	unsigned int *blockCnt
)
#else
	(port, device, block_number, block_size, block, blockCnt)
	port_t port;
	dev_t device;
	daddr_t block_number;
	int block_size;
	io_buf_ptr_t *block;
	unsigned int *blockCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t deviceType;
		dev_t device;
		char devicePad[2];
		msg_type_t block_numberType;
		daddr_t block_number;
		msg_type_t block_sizeType;
		int block_size;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t blockType;
		io_buf_ptr_t block;
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

	unsigned int msg_size = 48;

#if	UseStaticMsgType
	static msg_type_t deviceType = {
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
	static msg_type_t block_numberType = {
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
	static msg_type_t block_sizeType = {
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
	InP->deviceType = deviceType;
#else	UseStaticMsgType
	InP->deviceType.msg_type_name = MSG_TYPE_INTEGER_16;
	InP->deviceType.msg_type_size = 16;
	InP->deviceType.msg_type_number = 1;
	InP->deviceType.msg_type_inline = TRUE;
	InP->deviceType.msg_type_longform = FALSE;
	InP->deviceType.msg_type_deallocate = FALSE;
	InP->deviceType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->device /* device */ = /* device */ device;

#if	UseStaticMsgType
	InP->block_numberType = block_numberType;
#else	UseStaticMsgType
	InP->block_numberType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_numberType.msg_type_size = 32;
	InP->block_numberType.msg_type_number = 1;
	InP->block_numberType.msg_type_inline = TRUE;
	InP->block_numberType.msg_type_longform = FALSE;
	InP->block_numberType.msg_type_deallocate = FALSE;
	InP->block_numberType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_number /* block_number */ = /* block_number */ block_number;

#if	UseStaticMsgType
	InP->block_sizeType = block_sizeType;
#else	UseStaticMsgType
	InP->block_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_sizeType.msg_type_size = 32;
	InP->block_sizeType.msg_type_number = 1;
	InP->block_sizeType.msg_type_inline = TRUE;
	InP->block_sizeType.msg_type_longform = FALSE;
	InP->block_sizeType.msg_type_deallocate = FALSE;
	InP->block_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_size /* block_size */ = /* block_size */ block_size;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2502;

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

	if (OutP->Head.msg_id != 2602)
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
	if ((OutP->blockType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->blockType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->blockType.msg_type_long_name != MSG_TYPE_INTEGER_8) ||
	    (OutP->blockType.msg_type_long_size != 8))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*block /* block */ = /* *block */ OutP->block;

	*blockCnt /* blockType.msg_type_long_number */ = /* *blockCnt */ OutP->blockType.msg_type_long_number;

	return OutP->RetCode;
}

/* Routine block_read_ahead */
mig_external kern_return_t block_read_ahead
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t port,
	dev_t device,
	daddr_t block_number,
	daddr_t ra_block_number,
	int block_size,
	io_buf_ptr_t *block,
	unsigned int *blockCnt
)
#else
	(port, device, block_number, ra_block_number, block_size, block, blockCnt)
	port_t port;
	dev_t device;
	daddr_t block_number;
	daddr_t ra_block_number;
	int block_size;
	io_buf_ptr_t *block;
	unsigned int *blockCnt;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t deviceType;
		dev_t device;
		char devicePad[2];
		msg_type_t block_numberType;
		daddr_t block_number;
		msg_type_t ra_block_numberType;
		daddr_t ra_block_number;
		msg_type_t block_sizeType;
		int block_size;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_long_t blockType;
		io_buf_ptr_t block;
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

	unsigned int msg_size = 56;

#if	UseStaticMsgType
	static msg_type_t deviceType = {
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
	static msg_type_t block_numberType = {
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
	static msg_type_t ra_block_numberType = {
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
	static msg_type_t block_sizeType = {
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
	InP->deviceType = deviceType;
#else	UseStaticMsgType
	InP->deviceType.msg_type_name = MSG_TYPE_INTEGER_16;
	InP->deviceType.msg_type_size = 16;
	InP->deviceType.msg_type_number = 1;
	InP->deviceType.msg_type_inline = TRUE;
	InP->deviceType.msg_type_longform = FALSE;
	InP->deviceType.msg_type_deallocate = FALSE;
	InP->deviceType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->device /* device */ = /* device */ device;

#if	UseStaticMsgType
	InP->block_numberType = block_numberType;
#else	UseStaticMsgType
	InP->block_numberType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_numberType.msg_type_size = 32;
	InP->block_numberType.msg_type_number = 1;
	InP->block_numberType.msg_type_inline = TRUE;
	InP->block_numberType.msg_type_longform = FALSE;
	InP->block_numberType.msg_type_deallocate = FALSE;
	InP->block_numberType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_number /* block_number */ = /* block_number */ block_number;

#if	UseStaticMsgType
	InP->ra_block_numberType = ra_block_numberType;
#else	UseStaticMsgType
	InP->ra_block_numberType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->ra_block_numberType.msg_type_size = 32;
	InP->ra_block_numberType.msg_type_number = 1;
	InP->ra_block_numberType.msg_type_inline = TRUE;
	InP->ra_block_numberType.msg_type_longform = FALSE;
	InP->ra_block_numberType.msg_type_deallocate = FALSE;
	InP->ra_block_numberType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->ra_block_number /* ra_block_number */ = /* ra_block_number */ ra_block_number;

#if	UseStaticMsgType
	InP->block_sizeType = block_sizeType;
#else	UseStaticMsgType
	InP->block_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	InP->block_sizeType.msg_type_size = 32;
	InP->block_sizeType.msg_type_number = 1;
	InP->block_sizeType.msg_type_inline = TRUE;
	InP->block_sizeType.msg_type_longform = FALSE;
	InP->block_sizeType.msg_type_deallocate = FALSE;
	InP->block_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	InP->block_size /* block_size */ = /* block_size */ block_size;

	InP->Head.msg_simple = TRUE;
	InP->Head.msg_size = msg_size;
	InP->Head.msg_type = MSG_TYPE_NORMAL | MSG_TYPE_RPC;
	InP->Head.msg_request_port = port;
	InP->Head.msg_reply_port = mig_get_reply_port();
	InP->Head.msg_id = 2503;

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

	if (OutP->Head.msg_id != 2603)
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
	if ((OutP->blockType.msg_type_header.msg_type_inline != FALSE) ||
	    (OutP->blockType.msg_type_header.msg_type_longform != TRUE) ||
	    (OutP->blockType.msg_type_long_name != MSG_TYPE_INTEGER_8) ||
	    (OutP->blockType.msg_type_long_size != 8))
		return MIG_TYPE_ERROR;
#endif	TypeCheck

	*block /* block */ = /* *block */ OutP->block;

	*blockCnt /* blockType.msg_type_long_number */ = /* *blockCnt */ OutP->blockType.msg_type_long_number;

	return OutP->RetCode;
}
