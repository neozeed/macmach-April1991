#ifndef	_monitor
#define	_monitor

/* Module monitor */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_monitor
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <mach/kernel_event.h>

/* Routine monitor_create */
mig_external kern_return_t monitor_create
#if	defined(LINTLIBRARY)
    (this_task, new_monitor, requested_size)
	task_t this_task;
	port_t *new_monitor;
	int *requested_size;
{ return monitor_create(this_task, new_monitor, requested_size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t this_task,
	port_t *new_monitor,
	int *requested_size
);
#else
    ();
#endif
#endif

/* Routine monitor_terminate */
mig_external kern_return_t monitor_terminate
#if	defined(LINTLIBRARY)
    (this_monitor)
	monitor_t this_monitor;
{ return monitor_terminate(this_monitor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	monitor_t this_monitor
);
#else
    ();
#endif
#endif

/* Routine monitor_resume */
mig_external kern_return_t monitor_resume
#if	defined(LINTLIBRARY)
    (this_monitor)
	monitor_t this_monitor;
{ return monitor_resume(this_monitor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	monitor_t this_monitor
);
#else
    ();
#endif
#endif

/* Routine monitor_suspend */
mig_external kern_return_t monitor_suspend
#if	defined(LINTLIBRARY)
    (this_monitor)
	monitor_t this_monitor;
{ return monitor_suspend(this_monitor); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	monitor_t this_monitor
);
#else
    ();
#endif
#endif

/* Routine thread_monitor */
mig_external kern_return_t thread_monitor
#if	defined(LINTLIBRARY)
    (this_monitor, monitor_id, this_thread)
	monitor_t this_monitor;
	int monitor_id;
	thread_t this_thread;
{ return thread_monitor(this_monitor, monitor_id, this_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	monitor_t this_monitor,
	int monitor_id,
	thread_t this_thread
);
#else
    ();
#endif
#endif

/* Routine thread_unmonitor */
mig_external kern_return_t thread_unmonitor
#if	defined(LINTLIBRARY)
    (this_monitor, this_thread)
	monitor_t this_monitor;
	thread_t this_thread;
{ return thread_unmonitor(this_monitor, this_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	monitor_t this_monitor,
	thread_t this_thread
);
#else
    ();
#endif
#endif

/* Routine monitor_read */
mig_external kern_return_t monitor_read
#if	defined(LINTLIBRARY)
    (this_monitor, buffer, bufferCnt)
	monitor_t this_monitor;
	kern_mon_buffer_t buffer;
	unsigned int *bufferCnt;
{ return monitor_read(this_monitor, buffer, bufferCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	monitor_t this_monitor,
	kern_mon_buffer_t buffer,
	unsigned int *bufferCnt
);
#else
    ();
#endif
#endif

#endif	_monitor
