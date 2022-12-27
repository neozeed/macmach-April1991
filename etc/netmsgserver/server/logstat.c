/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	logstat.c,v $
 * Revision 1.1  90/02/19  21:52:38  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.26  89/05/02  11:11:28  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.25  89/04/24  20:40:30  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:54:50  dpj]
 * 
 * Revision 1.24  88/10/08  22:25:21  dpj
 * 	Reduce size of logs.
 * 
 * 01-Sep-88  Avadis Tevanian (avie) at NeXT
 *	Reduce size of logs.
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *	Only check in NMMONITOR if param.old_nmmonitor is true.
 *
 * 31-May-88  Daniel Julin (dpj) at Carnegie-Mellon University
 * 	Added an extra port, checked-in as NM_ACTIVE, to be looked-up
 *	by people who want to get a port death notification when the
 *	network server crashes. This special port should be used intead
 *	of the NM_LOGSTAT/NMMonitor port used previously for that purpose,
 *	so that the LOGSTAT service can be re-organized transparently when
 *	needed.
 *
 * 12-Apr-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	COMPAT: Added a handler to kill the compatibility server before
 *	exiting whenever possible.
 *
 * 31-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Put back the checkin of NMMonitor, because lcp depends on it (GRR!).
 *	
 * 29-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Removed syslog initialization: it now happens in nmserver.c.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 17-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Removed checkin of NMMonitor to avoid conflicts in compatibility mode.
 *
 * 15-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added code to read a configuration file. Added syslog.
 *
 * 27-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added code to enable compatibility mode from the command line.
 *	Also kill the compatibility netmsgserver in panic(). (COMPAT)
 *
 * 14-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Do not append to previous log file when dumping a new log.
 *
 * 18-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added port statistics handling.
 *
 * 17-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	LOG_CORE: place the log in data space, so that it will show up
 *	in the core.
 *
 *  5-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Check in "nmmonitor" as well as "nm_logstat" so lcpserver can function.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added include of nm_extra.h.
 *	Use two logs - an old and a current log.
 *	ls_sendlog returns both logs and ls_writelog writes them both.
 *	Lock the logs when resetting them.
 *
 * 23-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Conditionally use thread_lock - ensures only one thread is executing.
 *	Use malloc to allocate the log storage.
 *	Statically allocate the log_lock.
 *	Added call to cthread_set_name.
 *
 * 16-Apr-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Split ls_init into two phases (ls_init_1 and ls_init_2) to allow use
 *	of the log services early in the network server initialisation sequence.
 *
 * 15-Apr-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added main routine for the logstat service. Fixed ls_writelog
 *	not to crash if the log has not been initialised.
 *
 * 15-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed lint.
 *
 * 13-Mar-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 * logstat.c
 *
 *
 */
#ifndef	lint
char logstat_rcsid[] = "$ Header: logstat.c,v 1.26 89/05/02 11:11:28 dpj Exp $";
#endif not lint
/*
 * Handling of the activity log and statistics.
 */


#define	LOG_CORE	1

#include	<string.h>
#include	<cthreads.h>
#include	<mach.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<mach/boolean.h>
#include	<sys/file.h>
#include	<mach/message.h>
#include	<sys/signal.h>
#include	<syslog.h>

#include	"debug.h"
#include	"logstat.h"
#include	"ls_defs.h"
#include	"mem.h"
#include	"netmsg.h"
#include	"nm_extra.h"

extern char	*malloc();

/*
 * Size of each of the current and the old log.
 */
#if	LOG_CORE
#if	NeXT
#define	LOG_SIZE	400
#define	LOG_LIMIT	300
#else	NeXT
#define	LOG_SIZE	4000
#define	LOG_LIMIT	3000
#endif	NeXT

log_rec_t	log1[LOG_SIZE];
log_rec_t	log2[LOG_SIZE];

#else	LOG_CORE
#define	LOG_SIZE	10000
#define	LOG_LIMIT	9000
#endif	LOG_CORE

/*
 * Log variables.
 */
EXPORT	log_rec_t	*log_cur_ptr;	/* next free record in the log */
EXPORT	log_rec_t	*log_end_ptr;	/* end of the log */
EXPORT	mutex_t		log_lock;	/* lock for the log */
PUBLIC	struct mutex	log_lock_data;

PRIVATE	log_rec_t	*old_log_start_ptr = 0;	/* first record of the old log */
PRIVATE	log_rec_t	*cur_log_start_ptr = 0;	/* first record of the current log */
PRIVATE int		old_log_length = 0;	/* size of old log */

/*
 * Port statistics variables.
 */
#define PORT_STAT_COUNT	256			/* Number of port stat records allocated. */
#if	PORTSTAT
EXPORT	port_stat_ptr_t	port_stat_cur;		/* Next free port stat record. */
EXPORT	port_stat_ptr_t	port_stat_end;		/* Last free port stat record. */
EXPORT	struct mutex	port_stat_lock;		/* Lock for allocating port stat records. */
PRIVATE	port_stat_ptr_t	port_stat_start;	/* First port stat record. */
#endif	PORTSTAT


/*
 * logstat service port.
 */
PRIVATE	port_t	ls_service_port;

#define	LS_MAX_MSG_SIZE		100


/*
 * Default file name for configuration file.
 */
#define	DEFAULT_CONFIG_FILE	"NETMSG_CONFIG"


/*
 * Names for debug and param fields.
 */
extern char	*debug_names[];
extern char	*param_names[];


/*
 * ls_main
 *	Waits for messages on the logstat service port.
 *
 * Results:
 *	Ignored.
 *
 */
PRIVATE ls_main()
BEGIN("ls_main")
    kern_return_t	kr;
    msg_header_t	*in_msg_ptr, *rep_msg_ptr;

#if	LOCK_THREADS
    mutex_lock(thread_lock);
#endif	LOCK_THREADS

    MEM_ALLOC(in_msg_ptr,msg_header_t *,LS_MAX_MSG_SIZE, FALSE);
    MEM_ALLOC(rep_msg_ptr,msg_header_t *,LS_MAX_MSG_SIZE, FALSE);

    while (TRUE) {
	in_msg_ptr->msg_size = LS_MAX_MSG_SIZE;
	in_msg_ptr->msg_local_port = ls_service_port;

	kr = netmsg_receive(in_msg_ptr);

	if (kr == RCV_SUCCESS) {
	    if (logstat_server((caddr_t)in_msg_ptr, (caddr_t)rep_msg_ptr)) {
		/*
		 * Send the reply back.
		 */
		if ((kr = msg_send(rep_msg_ptr, MSG_OPTION_NONE, 0)) != SEND_SUCCESS) {
		    ERROR((msg, "ls_main.msg_send fails, kr = %d.\n", kr));
		}
	    }
	    else {
		ERROR((msg, "ls_main.ls_server fails, msg_id = %d.\n", in_msg_ptr->msg_id));
	    }
	}
	else {
	    ERROR((msg, "ls_main.netmsg_receive fails, kr = %d.\n", kr));
	}
	LOGCHECK;
    }

END


/*
 * ls_init_1
 *
 * Firt phase of logstat initialisation.
 *
 * Parameters: none
 *
 * Results:
 *
 * TRUE in case of success, FALSE otherwise.
 *
 * Side effects:
 *
 * Allocates space for the log and initializes all the relevant variables.
 * Allocates space for the port statistics.
 * Initializes the statistics record. 
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT boolean_t ls_init_1()
{
#if	!LOG_CORE
	vm_size_t	size;
#endif	!LOG_CORE

#if	LOG_CORE
	cur_log_start_ptr = log1;
	old_log_start_ptr = log2;
#else	LOG_CORE
	size = LOG_SIZE * sizeof(log_rec_t);
	if ((cur_log_start_ptr = (log_rec_t *)malloc((unsigned int)size)) == (log_rec_t *)0) {
		panic("ls_init_1.malloc");
	}
	if ((old_log_start_ptr = (log_rec_t *)malloc((unsigned int)size)) == (log_rec_t *)0) {
		panic("ls_init_1.malloc");
	}
#endif	LOG_CORE
	log_cur_ptr = cur_log_start_ptr;
	log_end_ptr = cur_log_start_ptr + LOG_LIMIT;
	log_lock = &log_lock_data;
	mutex_init(log_lock);
	mutex_set_name(log_lock, "NM_log_lock");

#if	PORTSTAT
	/*
	 * Initialise the port statistics.
	 */
	if ((port_stat_start = (port_stat_ptr_t)malloc(PORT_STAT_COUNT * sizeof(port_stat_t))) == 0) {
		panic("ls_init_1.malloc");
	}
	port_stat_cur = port_stat_start;
	port_stat_end = port_stat_start + (PORT_STAT_COUNT - 1);
	mutex_init(&port_stat_lock);
	mutex_set_name(&port_stat_lock, "port_stat_lock");
#endif	PORTSTAT

	/*
	 * Initialize statistics record.
	 */
	bzero((char *)&nmstat,sizeof(stat_t));

	return TRUE;
}


/*
 * ls_init_2
 *
 * Second phase of logstat initialisation.
 *
 * Parameters: none
 *
 * Results:
 *
 * TRUE in case of success, FALSE otherwise.
 *
 * Side effects:
 *
 * Starts up a thread to execute the logstat server main loop.
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT boolean_t ls_init_2()
BEGIN("ls_init_2")
	kern_return_t	ret;
	cthread_t	ls_thread;
	port_t		active_port;

	/*
	 * Allocate a service port.
	 */
	if ((ret = port_allocate(task_self(), &ls_service_port)) != KERN_SUCCESS) {
		ERROR((msg, "ls_init_2.port_allocate fails, ret = %d.\n", ret));
		RETURN(FALSE);
	}
	ret = port_disable(task_self(), ls_service_port);
	if ((ret = _netname_check_in(PORT_NULL, "NM_LOGSTAT", task_self(), ls_service_port))
		!= KERN_SUCCESS)
	{
		ERROR((msg, "ls_init_2._netname_check_in fails, ret = %d.\n", ret));
		RETURN(FALSE);
	}

	if (param.old_nmmonitor) {
		(void)_netname_check_in(PORT_NULL, "NMMONITOR", task_self(), ls_service_port);
	}

	/*
	 * Create and checkin a port as a token of the network server's health.
	 */
	if ((ret = port_allocate(task_self(), &active_port)) != KERN_SUCCESS) {
		ERROR((msg, "ls_init_2.port_allocate fails, ret = %d.\n", ret));
		RETURN(FALSE);
	}
	ret = port_disable(task_self(), active_port);
	if ((ret = _netname_check_in(PORT_NULL, "NM_ACTIVE", task_self(), active_port))
		!= KERN_SUCCESS)
	{
		ERROR((msg, "ls_init_2._netname_check_in fails, ret = %d.\n", ret));
		RETURN(FALSE);
	}

	/*
	 * Fire up the server thread.
	 */
	ls_thread = cthread_fork((cthread_fn_t)ls_main, (any_t)0);
	cthread_set_name(ls_thread, "ls_main");
    	cthread_detach(ls_thread);

	RETURN(TRUE);
END


/*
 * ls_sendlog
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * OUT old_log_ptr: pointer to the beginning of the old log.
 * OUT old_log_size: size of the old log in 32-bit words.
 * OUT cur_log_ptr: pointer to the beginning of the current log.
 * OUT cur_log_size: size of the current log in 32-bit words.
 *
 * Results: none
 *
 * Side effects:
 *
 * Resets the logs to be empty.
 *
 * Note:
 *	Locks the log_lock whilst manipulating the logs.
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_sendlog(ServPort,OUT old_log_ptr,OUT old_log_size,OUT cur_log_ptr,OUT cur_log_size)
port_t		ServPort;
log_ptr_t	*old_log_ptr;
unsigned int	*old_log_size;
log_ptr_t	*cur_log_ptr;
unsigned int	*cur_log_size;
BEGIN("ls_sendlog")

mutex_lock(log_lock);
*old_log_ptr = old_log_start_ptr;
*old_log_size = old_log_length * (sizeof(log_rec_t) >> 2);
*cur_log_ptr = cur_log_start_ptr;
*cur_log_size = (log_cur_ptr - cur_log_start_ptr) * (sizeof(log_rec_t) >> 2);
log_cur_ptr = cur_log_start_ptr;
old_log_length = 0;
mutex_unlock(log_lock);

RETURN(0);

END


/*
 * ls_resetlog
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 *
 * Results: none
 *
 * Side effects:
 *
 * Swaps the current and old logs.  Reinitialises the new current log.
 *
 * Note:
 *	Locks the log_lock whilst manipulating the logs.
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_resetlog(ServPort)
port_t		ServPort;
BEGIN("ls_resetlog")
register log_rec_t	*temp_ptr;

mutex_lock(log_lock);

old_log_length = log_cur_ptr - cur_log_start_ptr;

temp_ptr = cur_log_start_ptr;
cur_log_start_ptr = old_log_start_ptr;
old_log_start_ptr = temp_ptr;

log_cur_ptr = cur_log_start_ptr;
log_end_ptr = cur_log_start_ptr + LOG_LIMIT;

mutex_unlock(log_lock);

RETURN(0);

END


/*
 * ls_writelog
 *
 * Parameters: 
 *
 * ServPort: server port for RPC.
 *
 * Results: none
 *
 * Side effects:
 *
 * Writes both the old and the current logs into a file named "NMLOG".
 * Resets the logs.
 *
 * Note:
 *	Locks the log_lock whilst manipulating the logs.
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_writelog(ServPort)
port_t		ServPort;
BEGIN("ls_writelog")
int	fd;
extern int errno;

fd = open("NMLOG", O_WRONLY|O_CREAT, 0644);
if (fd < 0) {
	fprintf(stderr, "fd = %d, errno = %d.\n", fd, errno);
    	perror("Cannot open file to dump the log");
	_exit(1);
}
	mutex_lock(log_lock);

if (old_log_length)
	(void)write(fd, (char *)old_log_start_ptr, old_log_length * sizeof(log_rec_t));
(void)write(fd, (char *)cur_log_start_ptr, (log_cur_ptr - cur_log_start_ptr) * sizeof(log_rec_t));
(void)close(fd);

log_cur_ptr = cur_log_start_ptr;
old_log_length = 0;

mutex_unlock(log_lock);

RETURN(0);

END


/*
 * ls_sendstat
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * OUT stat_ptr: pointer to the statistics record.
 * OUT stat_size: size of the statistics record in 32-bit words.
 *
 * Results: none
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_sendstat(ServPort,OUT stat_ptr,OUT stat_size)
port_t		ServPort;
stat_ptr_t	*stat_ptr;
unsigned int	*stat_size;
BEGIN("ls_sendstat")

*stat_ptr = (stat_ptr_t) &nmstat;
*stat_size = sizeof(stat_t) >> 2;

RETURN(0);

END


/*
 * ls_resetstat
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 *
 * Results: none
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_resetstat(ServPort)
port_t		ServPort;
BEGIN("ls_resetstat")

bzero((char *)&nmstat,sizeof(stat_t));

RETURN(0);

END


/*
 * ls_senddebug
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * OUT debug_ptr: pointer to the debug record.
 * OUT debug_size: size of the debug record in 32-bit words.
 *
 * Results: none
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_senddebug(ServPort,OUT debug_ptr,OUT debug_size)
port_t		ServPort;
debug_ptr_t	*debug_ptr;
unsigned int	*debug_size;
BEGIN("ls_senddebug")

*debug_ptr = (debug_ptr_t) &debug;
*debug_size = sizeof(debug_t) >> 2;

RETURN(0);

END


/*
 * ls_setdebug
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * debug_ptr: pointer to the new debug record.
 * debug_size: size of the debug record in 32-bit words.
 *
 * Results: none
 *
 * Side effects:
 *
 * Updates the global debug record.
 *
 * Design:
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_setdebug(ServPort, debug_ptr, debug_size)
port_t		ServPort;
debug_ptr_t	debug_ptr;
unsigned int	debug_size;
BEGIN("ls_senddebug")
bcopy((char *)debug_ptr,(char *)&debug,(int)(debug_size << 2));
RETURN(0);

END


/*
 * ls_sendparam
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * OUT param_ptr: pointer to the param record.
 * OUT param_size: size of the param record in 32-bit words.
 *
 * Results: none
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_sendparam(ServPort,OUT param_ptr,OUT param_size)
port_t		ServPort;
param_ptr_t	*param_ptr;
unsigned int	*param_size;
BEGIN("ls_sendparam")

*param_ptr = (param_ptr_t) &param;
*param_size = sizeof(param_t) >> 2;

RETURN(0);

END


/*
 * ls_setparam
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * param_ptr: pointer to the new param record.
 * param_size: size of the param record in 32-bit words.
 *
 * Results: none
 *
 * Side effects:
 *
 * Updates the global param record.
 *
 * Design:
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_setparam(ServPort, param_ptr, param_size)
port_t		ServPort;
param_ptr_t	param_ptr;
unsigned int	param_size;
BEGIN("ls_sendparam")

bcopy((char *)param_ptr,(char *)&param,(int)(param_size << 2));

RETURN(0);

END



/*
 * ls_sendportstat
 *
 * Parameters:
 *
 * ServPort: server port for RPC.
 * OUT port_stat_ptr: pointer to the beginning of the port statistics.
 * OUT port_stat_size: size of the port statistics in 32-bit words.
 *
 * Results: none
 *
 * Note:
 *	locks the port_stat_lock while counting the size of port stat data.
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_sendportstat(ServPort,OUT port_stat_ptr,OUT port_stat_size)
port_t		ServPort;
log_ptr_t	*port_stat_ptr;
unsigned int	*port_stat_size;
BEGIN("ls_sendportstat")

#if	PORTSTAT
*port_stat_ptr = (log_ptr_t)port_stat_start;
mutex_lock(&port_stat_lock);
*port_stat_size = ((port_stat_cur - port_stat_start) * (sizeof(port_stat_t))) >> 2;
mutex_unlock(&port_stat_lock);
#else	PORTSTAT
*port_stat_ptr = 0;
*port_stat_size = 0;
#endif	PORTSTAT

RETURN(0);

END


/*
 * ls_mem_list --
 *
 * Parameters:
 *
 * Results: none
 *
 * Note:
 *
 */
/*ARGSUSED*/
EXPORT kern_return_t ls_mem_list(ServPort, OUT class_ptr, OUT class_ptr_size,
					OUT nam_ptr, OUT nam_ptr_size,
					OUT bucket_ptr, OUT bucket_ptr_size)
#ifdef mac2
port_t			ServPort;
mem_class_ptr_t		*class_ptr;
unsigned int		*class_ptr_size;
mem_nam_ptr_t		*nam_ptr;
unsigned int		*nam_ptr_size;
mem_bucket_ptr_t	*bucket_ptr;
unsigned int		*bucket_ptr_size;
#else
port_t			ServPort;
mem_objrec_ptr_t	*class_ptr;
int			*class_ptr_size;
char			**nam_ptr;
int			*nam_ptr_size;
int			**bucket_ptr;
int			*bucket_ptr_size;
#endif
BEGIN("ls_mem_list")
	kern_return_t	kr;
	int		class_max;
	int		nam_max;
	int		bucket_max;

	class_max = 100 * sizeof(mem_objrec_t);
	nam_max = 100 * 80;
	bucket_max = 5000 * sizeof(int);

	*class_ptr = NULL;
	*nam_ptr = NULL;
	*bucket_ptr = NULL;

	kr = vm_allocate(task_self(),class_ptr,class_max,TRUE);
	kr = vm_allocate(task_self(),nam_ptr,nam_max,TRUE);
	kr = vm_allocate(task_self(),bucket_ptr,bucket_max,TRUE);

	*class_ptr_size = class_max;
	*nam_ptr_size = nam_max;
	*bucket_ptr_size = bucket_max;

	mem_list(*class_ptr,*nam_ptr,*bucket_ptr,
			class_ptr_size,nam_ptr_size,bucket_ptr_size);

	RETURN(KERN_SUCCESS);
END


/*
 * panic
 *
 * Parameters:
 *
 * m: message to print on the error stream
 *
 * Results: none
 *
 * Side effects:
 *
 * Prints a message on stderr, dumps the activity log into a file
 * named "log" and exits.
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT void panic(m)
char	*m;
BEGIN("panic")

#if	COMPAT
	extern int	compat_pid;

	if (compat_pid > 0) {
		signal(SIGCHLD,SIG_IGN);
		kill(compat_pid,SIGKILL);
	}
#endif	COMPAT

	fprintf(stderr,"Panic: %s\n",m);
	(void)ls_writelog(0);
	fprintf(stderr,"log dumped\n");
	(void)fflush(stderr);
	if (param.syslog) {
		syslog(LOG_CRIT,"Panic: %s",m);
	}
	_exit(1);

	RET;

END


#if	COMPAT
/*
 * exit_handler --
 *
 * Parameters:
 *
 * sig: signal causing the exit.
 *
 * Results: none
 *
 * Side effects:
 *
 * Prints a message and calls panic().
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT void exit_handler(sig)
	int	sig;
BEGIN("exit_handler")

	fprintf(stderr,"Caught signal %d\n",sig);
	(void)fflush(stderr);
	if (param.syslog) {
		syslog(LOG_CRIT,"Caught signal %d",sig);
	}
	panic("exit_handler");

	RET;

END
#endif	COMPAT


/*
 * ls_read_config_file
 *
 * Parameters:
 *
 * fn: file name for config file or NULL if default.
 *
 * Results:
 *
 * TRUE if successful, FALSE otherwise.
 *
 * Side effects:
 *
 * Initializes all parameters from configuration file.
 * May print error messages.
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT boolean_t ls_read_config_file(fn)
	char	*fn;
BEGIN("ls_read_config_file")
	FILE	*fp;
	char	domain[50];
	char	name[100];
	int	val;
	int	*debug_val = (int *) &debug;
	int	*param_val = (int *) &param;
	int	field;
	extern char	compat_server[];

#if	0
#define	DBG(arg) {	\
	fprintf arg;	\
	fflush(stderr);	\
}
#else
#define	DBG(arg) /* */
#endif

	/*
	 * Open the configuration file.
	 */
	if (fn) {
		if ((fp = fopen(fn,"r")) == NULL) {
			fprintf(stderr,"  Cannot open file %s\n", fn);
			RETURN(FALSE);
		}
	} else {
		if ((fp = fopen(DEFAULT_CONFIG_FILE,"r")) == NULL) {
			fprintf(stderr,"  No default configuration file\n");
			RETURN(TRUE);
		}
	}

	DBG((stderr, "opened config file\n"));
	/*
	 * Read the configuration information.
	 */
	while (!feof(fp)) {
		domain[0] = '\0';
		fscanf(fp,"%49s",domain);

		DBG((stderr,"domain = \"%s\"\n",domain));
		/*
		 * Skip over comments.
		 */
		if ((domain[0] == '#') ||
			(domain[0] == '\0') ||
			(domain[0] == ' ')) {
			while (((getc(fp))!='\n') && (!feof(fp)));
			continue;
		}

		DBG((stderr,"looking for the right domain\n"));
		if (strcmp(domain,"debug") == 0) {
			if (fscanf(fp,"%99s",name) != 1)
				goto error;
			DBG((stderr,"name = \"%s\"\n",name));
			field = stabsearch(name, debug_names, TRUE);
			DBG((stderr,"field = %d\n",field));
			if (field < 0)
				goto error;
			if (fscanf(fp,"%d",&val) != 1)
				goto error;
			DBG((stderr,"&debug=0x%x, &debug_val[field]=0x%x, val=%d\n",&debug,&debug_val[field],val));
			debug_val[field] = val;			
			DBG((stderr, "after assignment, debug_val[field] = %d\n",debug_val[field]));
			fprintf(stdout," New value from configuration file: debug.%s = %d\n", name, val);
			continue;
		}

		if (strcmp(domain,"param") == 0) {
			if (fscanf(fp,"%99s",name) != 1)
				goto error;
			DBG((stderr,"name = \"%s\"\n",name));
			field = stabsearch(name, param_names, TRUE);
			DBG((stderr,"field = %d\n",field));
			if (field < 0)
				goto error;
			if (fscanf(fp,"%d",&val) != 1)
				goto error;
			DBG((stderr,"&param=0x%x, &param_val[field]=0x%x, val=%d\n",&param,&param_val[field],val));
			param_val[field] = val;			
			DBG((stderr, "after assignment, param_val[field] = %d\n",param_val[field]));
			fprintf(stdout," New value from configuration file: param.%s = %d\n", name, val);
			continue;
		}

		if (strcmp(domain,"env") == 0) {
			if (fscanf(fp,"%99s",name) != 1)
				goto error;
			DBG((stderr,"name = \"%s\"\n",name));
			if (strcmp(name,"compat_server") != 0)
			    	goto error;
			if (fscanf(fp,"%99s",compat_server) != 1)
				goto error;
			DBG((stderr,"Got the compat_server: \"%s\"\n",compat_server));
			fprintf(stdout," New value from configuration file: %s = \"%s\"\n", name, compat_server);
			continue;
		}

		fprintf(stderr,"  Unknown domain name in configuration file\n");
		goto error;
	}

	fclose(fp);
	RETURN(TRUE);

error:
	fprintf(stderr,"  Invalid format for configuration file, near %s %s\n",
								domain, name);
	fflush(stderr);
	fclose(fp);
	RETURN(FALSE);

END
