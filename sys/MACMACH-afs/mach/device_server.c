/* Module device */

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
#include <sys/types.h>
#include <mach/device_types.h>

/* SimpleRoutine block_write */
mig_internal novalue _Xblock_write
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t block_write
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t port, dev_t device, daddr_t block_number, int block_size, io_buf_t block);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t deviceCheck = {
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
	static msg_type_t block_numberCheck = {
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
	static msg_type_t block_sizeCheck = {
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
	static msg_type_t blockCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		FALSE,
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
	if (* (int *) &In0P->deviceType != * (int *) &deviceCheck)
#else	UseStaticMsgType
	if ((In0P->deviceType.msg_type_inline != TRUE) ||
	    (In0P->deviceType.msg_type_longform != FALSE) ||
	    (In0P->deviceType.msg_type_name != MSG_TYPE_INTEGER_16) ||
	    (In0P->deviceType.msg_type_number != 1) ||
	    (In0P->deviceType.msg_type_size != 16))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_numberType != * (int *) &block_numberCheck)
#else	UseStaticMsgType
	if ((In0P->block_numberType.msg_type_inline != TRUE) ||
	    (In0P->block_numberType.msg_type_longform != FALSE) ||
	    (In0P->block_numberType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_numberType.msg_type_number != 1) ||
	    (In0P->block_numberType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_sizeType != * (int *) &block_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->block_sizeType.msg_type_inline != TRUE) ||
	    (In0P->block_sizeType.msg_type_longform != FALSE) ||
	    (In0P->block_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_sizeType.msg_type_number != 1) ||
	    (In0P->block_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->blockType != * (int *) &blockCheck)
#else	UseStaticMsgType
	if ((In0P->blockType.msg_type_inline != FALSE) ||
	    (In0P->blockType.msg_type_longform != FALSE) ||
	    (In0P->blockType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->blockType.msg_type_number != 1) ||
	    (In0P->blockType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) block_write(In0P->Head.msg_request_port, In0P->device, In0P->block_number, In0P->block_size, In0P->block);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* SimpleRoutine block_write_async */
mig_internal novalue _Xblock_write_async
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t block_write_async
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t port, dev_t device, daddr_t block_number, int block_size, io_buf_t block);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t deviceCheck = {
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
	static msg_type_t block_numberCheck = {
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
	static msg_type_t block_sizeCheck = {
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
	static msg_type_t blockCheck = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		FALSE,
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
	if (* (int *) &In0P->deviceType != * (int *) &deviceCheck)
#else	UseStaticMsgType
	if ((In0P->deviceType.msg_type_inline != TRUE) ||
	    (In0P->deviceType.msg_type_longform != FALSE) ||
	    (In0P->deviceType.msg_type_name != MSG_TYPE_INTEGER_16) ||
	    (In0P->deviceType.msg_type_number != 1) ||
	    (In0P->deviceType.msg_type_size != 16))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_numberType != * (int *) &block_numberCheck)
#else	UseStaticMsgType
	if ((In0P->block_numberType.msg_type_inline != TRUE) ||
	    (In0P->block_numberType.msg_type_longform != FALSE) ||
	    (In0P->block_numberType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_numberType.msg_type_number != 1) ||
	    (In0P->block_numberType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_sizeType != * (int *) &block_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->block_sizeType.msg_type_inline != TRUE) ||
	    (In0P->block_sizeType.msg_type_longform != FALSE) ||
	    (In0P->block_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_sizeType.msg_type_number != 1) ||
	    (In0P->block_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->blockType != * (int *) &blockCheck)
#else	UseStaticMsgType
	if ((In0P->blockType.msg_type_inline != FALSE) ||
	    (In0P->blockType.msg_type_longform != FALSE) ||
	    (In0P->blockType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->blockType.msg_type_number != 1) ||
	    (In0P->blockType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	(void) block_write_async(In0P->Head.msg_request_port, In0P->device, In0P->block_number, In0P->block_size, In0P->block);
	OutP->RetCode = MIG_NO_REPLY;
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	;
}

/* Routine block_read */
mig_internal novalue _Xblock_read
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t block_read
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t port, dev_t device, daddr_t block_number, int block_size, io_buf_ptr_t *block, unsigned int *blockCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t deviceCheck = {
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
	static msg_type_t block_numberCheck = {
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
	static msg_type_t block_sizeCheck = {
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
	static msg_type_long_t blockType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTEGER_8,
		/* msg_type_long_size = */	8,
		/* msg_type_long_number = */	0
	};
#endif	UseStaticMsgType

	unsigned int blockCnt;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 48) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->deviceType != * (int *) &deviceCheck)
#else	UseStaticMsgType
	if ((In0P->deviceType.msg_type_inline != TRUE) ||
	    (In0P->deviceType.msg_type_longform != FALSE) ||
	    (In0P->deviceType.msg_type_name != MSG_TYPE_INTEGER_16) ||
	    (In0P->deviceType.msg_type_number != 1) ||
	    (In0P->deviceType.msg_type_size != 16))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_numberType != * (int *) &block_numberCheck)
#else	UseStaticMsgType
	if ((In0P->block_numberType.msg_type_inline != TRUE) ||
	    (In0P->block_numberType.msg_type_longform != FALSE) ||
	    (In0P->block_numberType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_numberType.msg_type_number != 1) ||
	    (In0P->block_numberType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_sizeType != * (int *) &block_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->block_sizeType.msg_type_inline != TRUE) ||
	    (In0P->block_sizeType.msg_type_longform != FALSE) ||
	    (In0P->block_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_sizeType.msg_type_number != 1) ||
	    (In0P->block_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = block_read(In0P->Head.msg_request_port, In0P->device, In0P->block_number, In0P->block_size, &OutP->block, &blockCnt);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 48;

#if	UseStaticMsgType
	OutP->blockType = blockType;
#else	UseStaticMsgType
	OutP->blockType.msg_type_long_name = MSG_TYPE_INTEGER_8;
	OutP->blockType.msg_type_long_size = 8;
	OutP->blockType.msg_type_header.msg_type_inline = FALSE;
	OutP->blockType.msg_type_header.msg_type_longform = TRUE;
	OutP->blockType.msg_type_header.msg_type_deallocate = FALSE;
	OutP->blockType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->blockType.msg_type_long_number /* blockCnt */ = /* blockType.msg_type_long_number */ blockCnt;

	OutP->Head.msg_simple = FALSE;
	OutP->Head.msg_size = msg_size;
}

/* Routine block_read_ahead */
mig_internal novalue _Xblock_read_ahead
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
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

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t block_read_ahead
#if	(defined(__STDC__) || defined(c_plusplus))
		(port_t port, dev_t device, daddr_t block_number, daddr_t ra_block_number, int block_size, io_buf_ptr_t *block, unsigned int *blockCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t deviceCheck = {
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
	static msg_type_t block_numberCheck = {
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
	static msg_type_t ra_block_numberCheck = {
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
	static msg_type_t block_sizeCheck = {
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
	static msg_type_long_t blockType = {
	{
		/* msg_type_name = */		0,
		/* msg_type_size = */		0,
		/* msg_type_number = */		0,
		/* msg_type_inline = */		FALSE,
		/* msg_type_longform = */	TRUE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	},
		/* msg_type_long_name = */	MSG_TYPE_INTEGER_8,
		/* msg_type_long_size = */	8,
		/* msg_type_long_number = */	0
	};
#endif	UseStaticMsgType

	unsigned int blockCnt;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 56) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->deviceType != * (int *) &deviceCheck)
#else	UseStaticMsgType
	if ((In0P->deviceType.msg_type_inline != TRUE) ||
	    (In0P->deviceType.msg_type_longform != FALSE) ||
	    (In0P->deviceType.msg_type_name != MSG_TYPE_INTEGER_16) ||
	    (In0P->deviceType.msg_type_number != 1) ||
	    (In0P->deviceType.msg_type_size != 16))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_numberType != * (int *) &block_numberCheck)
#else	UseStaticMsgType
	if ((In0P->block_numberType.msg_type_inline != TRUE) ||
	    (In0P->block_numberType.msg_type_longform != FALSE) ||
	    (In0P->block_numberType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_numberType.msg_type_number != 1) ||
	    (In0P->block_numberType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->ra_block_numberType != * (int *) &ra_block_numberCheck)
#else	UseStaticMsgType
	if ((In0P->ra_block_numberType.msg_type_inline != TRUE) ||
	    (In0P->ra_block_numberType.msg_type_longform != FALSE) ||
	    (In0P->ra_block_numberType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->ra_block_numberType.msg_type_number != 1) ||
	    (In0P->ra_block_numberType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->block_sizeType != * (int *) &block_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->block_sizeType.msg_type_inline != TRUE) ||
	    (In0P->block_sizeType.msg_type_longform != FALSE) ||
	    (In0P->block_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->block_sizeType.msg_type_number != 1) ||
	    (In0P->block_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt0; }
#define	label_punt0
#endif	TypeCheck

	OutP->RetCode = block_read_ahead(In0P->Head.msg_request_port, In0P->device, In0P->block_number, In0P->ra_block_number, In0P->block_size, &OutP->block, &blockCnt);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 48;

#if	UseStaticMsgType
	OutP->blockType = blockType;
#else	UseStaticMsgType
	OutP->blockType.msg_type_long_name = MSG_TYPE_INTEGER_8;
	OutP->blockType.msg_type_long_size = 8;
	OutP->blockType.msg_type_header.msg_type_inline = FALSE;
	OutP->blockType.msg_type_header.msg_type_longform = TRUE;
	OutP->blockType.msg_type_header.msg_type_deallocate = FALSE;
	OutP->blockType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->blockType.msg_type_long_number /* blockCnt */ = /* blockType.msg_type_long_number */ blockCnt;

	OutP->Head.msg_simple = FALSE;
	OutP->Head.msg_size = msg_size;
}

boolean_t device_server
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

	if ((InP->msg_id > 2503) || (InP->msg_id < 2500))
		return FALSE;
	else {
		typedef novalue (*SERVER_STUB_PROC)
#if	(defined(__STDC__) || defined(c_plusplus))
			(msg_header_t *, msg_header_t *);
#else
			();
#endif
		static SERVER_STUB_PROC routines[] = {
			_Xblock_write,
			_Xblock_write_async,
			_Xblock_read,
			_Xblock_read_ahead,
		};

		if (routines[InP->msg_id - 2500])
			(routines[InP->msg_id - 2500]) (InP, &OutP->Head);
		 else
			return FALSE;
	}
	return TRUE;
}
