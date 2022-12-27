/* Module monitor */

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
#include <kern/type_conversion.h>
#include <mach/mach_types.h>
#include <mach/kernel_event.h>

/* Routine monitor_create */
mig_internal novalue _Xmonitor_create
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t requested_sizeType;
		int requested_size;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
		msg_type_t new_monitorType;
		port_t new_monitor;
		msg_type_t requested_sizeType;
		int requested_size;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t monitor_create
#if	(defined(__STDC__) || defined(c_plusplus))
		(task_t this_task, port_t *new_monitor, int *requested_size);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t requested_sizeCheck = {
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
	static msg_type_t new_monitorType = {
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
	static msg_type_t requested_sizeType = {
		/* msg_type_name = */		MSG_TYPE_INTEGER_32,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	task_t this_task;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 32) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_task /* convert_port_to_task 0 Head.msg_request_port */ = /* this_task */ convert_port_to_task(In0P->Head.msg_request_port);

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->requested_sizeType != * (int *) &requested_sizeCheck)
#else	UseStaticMsgType
	if ((In0P->requested_sizeType.msg_type_inline != TRUE) ||
	    (In0P->requested_sizeType.msg_type_longform != FALSE) ||
	    (In0P->requested_sizeType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->requested_sizeType.msg_type_number != 1) ||
	    (In0P->requested_sizeType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt1; }
#define	label_punt1
#endif	TypeCheck

	OutP->RetCode = monitor_create(this_task, &OutP->new_monitor, &In0P->requested_size);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	task_deallocate(this_task);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 48;

#if	UseStaticMsgType
	OutP->new_monitorType = new_monitorType;
#else	UseStaticMsgType
	OutP->new_monitorType.msg_type_name = MSG_TYPE_PORT;
	OutP->new_monitorType.msg_type_size = 32;
	OutP->new_monitorType.msg_type_number = 1;
	OutP->new_monitorType.msg_type_inline = TRUE;
	OutP->new_monitorType.msg_type_longform = FALSE;
	OutP->new_monitorType.msg_type_deallocate = FALSE;
	OutP->new_monitorType.msg_type_unused = 0;
#endif	UseStaticMsgType

#if	UseStaticMsgType
	OutP->requested_sizeType = requested_sizeType;
#else	UseStaticMsgType
	OutP->requested_sizeType.msg_type_name = MSG_TYPE_INTEGER_32;
	OutP->requested_sizeType.msg_type_size = 32;
	OutP->requested_sizeType.msg_type_number = 1;
	OutP->requested_sizeType.msg_type_inline = TRUE;
	OutP->requested_sizeType.msg_type_longform = FALSE;
	OutP->requested_sizeType.msg_type_deallocate = FALSE;
	OutP->requested_sizeType.msg_type_unused = 0;
#endif	UseStaticMsgType

	/* 0 */ OutP->requested_size = In0P->requested_size;

	OutP->Head.msg_simple = FALSE;
	OutP->Head.msg_size = msg_size;
}

/* Routine monitor_terminate */
mig_internal novalue _Xmonitor_terminate
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
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t monitor_terminate
#if	(defined(__STDC__) || defined(c_plusplus))
		(monitor_t this_monitor);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

	monitor_t this_monitor;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_monitor /* convert_port_to_monitor 0 Head.msg_request_port */ = /* this_monitor */ convert_port_to_monitor(In0P->Head.msg_request_port);

	OutP->RetCode = monitor_terminate(this_monitor);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	monitor_deallocate(this_monitor);
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

/* Routine monitor_resume */
mig_internal novalue _Xmonitor_resume
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
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t monitor_resume
#if	(defined(__STDC__) || defined(c_plusplus))
		(monitor_t this_monitor);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

	monitor_t this_monitor;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_monitor /* convert_port_to_monitor 0 Head.msg_request_port */ = /* this_monitor */ convert_port_to_monitor(In0P->Head.msg_request_port);

	OutP->RetCode = monitor_resume(this_monitor);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	monitor_deallocate(this_monitor);
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

/* Routine monitor_suspend */
mig_internal novalue _Xmonitor_suspend
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
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t monitor_suspend
#if	(defined(__STDC__) || defined(c_plusplus))
		(monitor_t this_monitor);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

	monitor_t this_monitor;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_monitor /* convert_port_to_monitor 0 Head.msg_request_port */ = /* this_monitor */ convert_port_to_monitor(In0P->Head.msg_request_port);

	OutP->RetCode = monitor_suspend(this_monitor);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	monitor_deallocate(this_monitor);
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

/* Routine thread_monitor */
mig_internal novalue _Xthread_monitor
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t monitor_idType;
		int monitor_id;
		msg_type_t this_threadType;
		port_t this_thread;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t thread_monitor
#if	(defined(__STDC__) || defined(c_plusplus))
		(monitor_t this_monitor, int monitor_id, thread_t this_thread);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t monitor_idCheck = {
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
	static msg_type_t this_threadCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	monitor_t this_monitor;
	thread_t this_thread;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 40) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_monitor /* convert_port_to_monitor 0 Head.msg_request_port */ = /* this_monitor */ convert_port_to_monitor(In0P->Head.msg_request_port);

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->monitor_idType != * (int *) &monitor_idCheck)
#else	UseStaticMsgType
	if ((In0P->monitor_idType.msg_type_inline != TRUE) ||
	    (In0P->monitor_idType.msg_type_longform != FALSE) ||
	    (In0P->monitor_idType.msg_type_name != MSG_TYPE_INTEGER_32) ||
	    (In0P->monitor_idType.msg_type_number != 1) ||
	    (In0P->monitor_idType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt1; }
#define	label_punt1
#endif	TypeCheck

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->this_threadType != * (int *) &this_threadCheck)
#else	UseStaticMsgType
	if ((In0P->this_threadType.msg_type_inline != TRUE) ||
	    (In0P->this_threadType.msg_type_longform != FALSE) ||
	    (In0P->this_threadType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->this_threadType.msg_type_number != 1) ||
	    (In0P->this_threadType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt1; }
#define	label_punt1
#endif	TypeCheck

	this_thread /* convert_port_to_thread 0 this_thread */ = /* this_thread */ convert_port_to_thread(In0P->this_thread);

	OutP->RetCode = thread_monitor(this_monitor, In0P->monitor_id, this_thread);
#ifdef	label_punt2
#undef	label_punt2
punt2:
#endif	label_punt2
	thread_deallocate(this_thread);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	monitor_deallocate(this_monitor);
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

/* Routine thread_unmonitor */
mig_internal novalue _Xthread_unmonitor
#if	(defined(__STDC__) || defined(c_plusplus))
	(msg_header_t *InHeadP, msg_header_t *OutHeadP)
#else
	(InHeadP, OutHeadP)
	msg_header_t *InHeadP, *OutHeadP;
#endif
{
	typedef struct {
		msg_header_t Head;
		msg_type_t this_threadType;
		port_t this_thread;
	} Request;

	typedef struct {
		msg_header_t Head;
		msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t thread_unmonitor
#if	(defined(__STDC__) || defined(c_plusplus))
		(monitor_t this_monitor, thread_t this_thread);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;

#if	UseStaticMsgType
	static msg_type_t this_threadCheck = {
		/* msg_type_name = */		MSG_TYPE_PORT,
		/* msg_type_size = */		32,
		/* msg_type_number = */		1,
		/* msg_type_inline = */		TRUE,
		/* msg_type_longform = */	FALSE,
		/* msg_type_deallocate = */	FALSE,
		/* msg_type_unused = */	0
	};
#endif	UseStaticMsgType

	monitor_t this_monitor;
	thread_t this_thread;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 32) || (msg_simple != FALSE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_monitor /* convert_port_to_monitor 0 Head.msg_request_port */ = /* this_monitor */ convert_port_to_monitor(In0P->Head.msg_request_port);

#if	TypeCheck
#if	UseStaticMsgType
	if (* (int *) &In0P->this_threadType != * (int *) &this_threadCheck)
#else	UseStaticMsgType
	if ((In0P->this_threadType.msg_type_inline != TRUE) ||
	    (In0P->this_threadType.msg_type_longform != FALSE) ||
	    (In0P->this_threadType.msg_type_name != MSG_TYPE_PORT) ||
	    (In0P->this_threadType.msg_type_number != 1) ||
	    (In0P->this_threadType.msg_type_size != 32))
#endif	UseStaticMsgType
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; goto punt1; }
#define	label_punt1
#endif	TypeCheck

	this_thread /* convert_port_to_thread 0 this_thread */ = /* this_thread */ convert_port_to_thread(In0P->this_thread);

	OutP->RetCode = thread_unmonitor(this_monitor, this_thread);
#ifdef	label_punt2
#undef	label_punt2
punt2:
#endif	label_punt2
	thread_deallocate(this_thread);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	monitor_deallocate(this_monitor);
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

/* Routine monitor_read */
mig_internal novalue _Xmonitor_read
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
		msg_type_long_t bufferType;
		kern_mon_data_t buffer[320];
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	extern kern_return_t monitor_read
#if	(defined(__STDC__) || defined(c_plusplus))
		(monitor_t this_monitor, kern_mon_buffer_t buffer, unsigned int *bufferCnt);
#else
		();
#endif

#if	TypeCheck
	boolean_t msg_simple;
#endif	TypeCheck

	unsigned int msg_size;
	unsigned int msg_size_delta;

#if	UseStaticMsgType
	static msg_type_long_t bufferType = {
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
		/* msg_type_long_number = */	1920
	};
#endif	UseStaticMsgType

	monitor_t this_monitor;
	unsigned int bufferCnt;

#if	TypeCheck
	msg_size = In0P->Head.msg_size;
	msg_simple = In0P->Head.msg_simple;
	if ((msg_size != 24) || (msg_simple != TRUE))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	TypeCheck

	this_monitor /* convert_port_to_monitor 0 Head.msg_request_port */ = /* this_monitor */ convert_port_to_monitor(In0P->Head.msg_request_port);

	bufferCnt = 320;

	OutP->RetCode = monitor_read(this_monitor, OutP->buffer, &bufferCnt);
#ifdef	label_punt1
#undef	label_punt1
punt1:
#endif	label_punt1
	monitor_deallocate(this_monitor);
#ifdef	label_punt0
#undef	label_punt0
punt0:
#endif	label_punt0
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msg_size = 44;

#if	UseStaticMsgType
	OutP->bufferType = bufferType;
#else	UseStaticMsgType
	OutP->bufferType.msg_type_long_name = MSG_TYPE_INTEGER_32;
	OutP->bufferType.msg_type_long_size = 32;
	OutP->bufferType.msg_type_header.msg_type_inline = TRUE;
	OutP->bufferType.msg_type_header.msg_type_longform = TRUE;
	OutP->bufferType.msg_type_header.msg_type_deallocate = FALSE;
	OutP->bufferType.msg_type_header.msg_type_unused = 0;
#endif	UseStaticMsgType

	OutP->bufferType.msg_type_long_number /* 6 bufferCnt */ = /* bufferType.msg_type_long_number */ 6 * bufferCnt;

	msg_size_delta = (24 * bufferCnt);
	msg_size += msg_size_delta;

	OutP->Head.msg_simple = TRUE;
	OutP->Head.msg_size = msg_size;
}

boolean_t monitor_server
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

	if ((InP->msg_id > 2806) || (InP->msg_id < 2800))
		return FALSE;
	else {
		typedef novalue (*SERVER_STUB_PROC)
#if	(defined(__STDC__) || defined(c_plusplus))
			(msg_header_t *, msg_header_t *);
#else
			();
#endif
		static SERVER_STUB_PROC routines[] = {
			_Xmonitor_create,
			_Xmonitor_terminate,
			_Xmonitor_resume,
			_Xmonitor_suspend,
			_Xthread_monitor,
			_Xthread_unmonitor,
			_Xmonitor_read,
		};

		if (routines[InP->msg_id - 2800])
			(routines[InP->msg_id - 2800]) (InP, &OutP->Head);
		 else
			return FALSE;
	}
	return TRUE;
}
