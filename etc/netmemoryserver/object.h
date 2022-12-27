/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	object.h,v $
 * Revision 1.1  90/02/19  21:48:35  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.3  89/08/24  17:46:27  mrt
 * 	Cleanup
 * 
 * Revision 1.2  89/04/29  13:24:32  mrt
 * 	Changed includes for Mach 2.5
 * 
 */
/*
 *	File:	object.h
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Definitions for object.c module.
 *
 */
#include <mach.h>
#include <mach/mach_param.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <servers/netname.h>
#include <mig_errors.h>
#include <cthreads.h>
#include <stdio.h>
#include "internal.h"
#include "netmemory.h"
#include "scheduler.h"
#include "config.h"

#if	DEBUG
#define	USE_SYSLOG	0
#endif	DEBUG

#if	USE_SYSLOG
#include <syslog.h>
#define	opensyslog() openlog("netmemoryserver", LOG_PID | LOG_CONS, LOG_DAEMON)
#else	USE_SYSLOG
#define	opensyslog() setlinebuf(stdout), setlinebuf(stderr)
#define	syslog	fprintf
#define LOG_ERR		stderr
#define LOG_DEBUG	stdout
#endif	USE_SYSLOG

#if	CACHE_OBJECTS
#define	MAY_CACHE	TRUE
#else	CACHE_OBJECTS
#define	MAY_CACHE	FALSE
#endif	CACHE_OBJECTS

#define	LOG2(x)			(ffs(x) - 1) /* only correct if power of two */
#define is_power_of_two(x)	((x) == (1 << LOG2(x))) /* always correct    */

#define	SERVERNAME	"netmemoryserver"
#define	SERVERDEBUGNAME	"netmemoryserver-debug"
#define	STATUS		mach_error_string(status)

#define	CORESIGNAL(x) ((x) > 2 && (x) < 13 && (x) != 9)

#define	pager_destroy()		/* XXX we need one! */

typedef struct schedule schedule_t;

typedef struct max_msg {
    msg_header_t header;
    char data[MSG_SIZE_MAX - sizeof(msg_header_t)];
} max_msg_t;

typedef struct reply {
    msg_header_t header;
    msg_type_t return_code_type;
    msg_return_t return_code;
} reply_t;

typedef struct kernel {
    port_t request_port;
    short page_size;
    short real_page_size;
} kernel_t;

typedef struct object {
    char *storage;
    port_t paging_object;
    port_t control_port;
    vm_size_t object_size;
    kernel_t kerninfo[MAX_NUM_KERNELS];
    schedule_t *schedinfo;
    fault_info_t *faults_per_page;
    fault_info_t faults_total;
    struct object *next;
    min_page_t *min_pages;	/* array[num_min_pages] */
    request_t *free_requests;
    request_t *all_requests;
    short mark;
    short outstanding[MAX_NUM_KERNELS][2];
    int inactivity;		/* XXX not yet used */
} object_t;

/*
 *  Kernels are labeled with integers from 0 to (max_num_kernels - 1).
 *  -1 designates null kernel.
 *  An empty slot is recognized by request_port = PORT_NULL.
 */
#define	KERNEL_NULL	-1

/*
 *  Routines provided by scheduler.c.
 */
extern int periodic_routine();
extern kern_return_t Object_create();
extern Object_destroy();
extern Kernel_open();
extern Kernel_close();
extern Add_reading_kernel();
extern Add_writing_kernel();
extern Subtract_kernel();

port_t server_port;	/* the netmemory server port */
