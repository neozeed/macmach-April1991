/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	main.c,v $
 * Revision 1.1  90/02/19  21:47:43  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.5  89/07/04  17:37:11  mrt
 * 	Changed include <ipcx_types.h> to "ipcx_types.h" as it is a local
 * 	file.
 * 	[89/07/04            mrt]
 * 
 *  4-Jan-87  Mary Thompson (mrt) at Carnegie Mellon
 *	Changed back to ru ipcexecd with the user id and password
 *	then pass the ports to the parent and then exec the target process.
 *	It is no longer possible to exec a setuid program (e.g. ru) if
 *	another task has your kernel or notify port.
 *
 * 30-Dec-87  Mary Thompson (mrt) at Carnegie Mellon
 *	Unrestrictd task_notify so we would notice nm_monitor_port
 *	death messages. Allocated space for PATTERN.
 *
 * 9-Dec-87   Mary Thompson (mrt) at Carnegie Mellon
 *	Added a nm_monitor_port  to indicate when the netmsgserver
 *	dies.
 *
 * 6-May-87   Mary Thompson (mrt) at Carnegie Mellon
 *	Unresticted the service port to work with new kernels.
 *
 * 30-Apr-87  Alessandro Forin (af) at Carnegie-Mellon University
 *  Re-created from Jeff Eppinger`s stuff,  stripped down and Mach-ized
 *
 */
/*
 * Abstract: main.c
 *	Start and login a process
 * Author:
 *	Alessandro Forin
 */
#include <stdio.h>
#include <sys/wait.h>
#include <mach.h>
#include <mach/notify.h>
#include <mig_errors.h>
#include "ipcx_types.h"

#ifndef TRUE
#define TRUE	(1)
#define FALSE	(0)
#endif

#define VERSION " -- Mach Server-Server -- version 1.1"

port_t SERVICE_PORT;
port_t RETURN_PORT;
port_t nm_monitor_port;

int NAME_SERVER_UP;
int REQUESTS;
long LAST_REQUEST_TIME;

static char return_port_name[256];
char *HANDSHAKE = return_port_name;
char *OUTFILE = "/dev/null";
static char ipcexecd_prog_name[256];
char *PROGNAME = ipcexecd_prog_name;
boolean_t isChild = FALSE;

static char *PREFIX = DEFAULT_PREFIX;
static char pattern_space[100];
static char *PATTERN = pattern_space;


extern long time();


static int args(argc, argv)
int argc;
char *argv[];
{
	int             i;
	char           *p;
	extern char    *strcat();

	if (argv[0][0] != '/') {
		fprintf(stderr, "Usage: <full-name> [-p <prefix>] [-o <outfile>]\n");
		exit(9);
	}
	strcpy(PROGNAME, argv[0]);

	for (i = 1; i < argc; i++)
		if (argv[i][0] == '-')
			switch (argv[i++][1]) {
			case 'p':
				PREFIX = argv[i];
				break;
			case 'o':
				OUTFILE = argv[i];
				break;
			case 'h':
				HANDSHAKE = argv[i];
				isChild = TRUE;
				break;
			default:
				break;
			}
	strcpy(PATTERN, PREFIX);
	strcat(PATTERN, "%s");
}

static void init(argv)
char *argv[];
{
	int             gr;
	FILE           *fp;
	int             clock;

	init_assert();
	init_patch(argv[0]);

	clock = time(0);
	fprintf(stderr, "%24.24s %s\n", ctime(&clock), VERSION);
	fprintf(stderr, "\tStarted from %s\n", argv[0]);
	fflush(stderr);

	(void) port_unrestrict(task_self(),task_notify());
	gr = port_allocate(task_self(), &SERVICE_PORT);
	if (gr != KERN_SUCCESS)
		HaltPrintingError(gr, "main.init.port_allocate(SERVICE_PORT)");
	(void) port_unrestrict(task_self(),SERVICE_PORT);

	gr = port_allocate(task_self(), &RETURN_PORT);
	if (gr != KERN_SUCCESS)
		HaltPrintingError(gr, "main.init.port_allocate(RETURN_PORT)");


	sprintf(HANDSHAKE, "%s-%d\0", PREFIX, getpid());

	NAME_SERVER_UP = 0;
	LAST_REQUEST_TIME = time(0);
	REQUESTS = 0;
}

static void check_on_kids()
{
	union wait      status;
	int             options = WNOHANG;
	int             deadpid;

	while ((deadpid = wait3(&status, options, 0)) > 0) {
		fprintf(stderr, "Child %d terminated, status %d\n",
			deadpid, status.w_retcode);
		fflush(stderr);
	}
}

static void DoPortDeath(DeadPort)
port_t DeadPort;
{
	if ((DeadPort == NameServerPort) || 
	    (DeadPort == nm_monitor_port)) {
		fprintf(stderr, "Name Server died!\n");
		fflush(stderr);
		NAME_SERVER_UP = 0;
	} else {
#if DEBUG
		 fprintf(stderr, "port %d is dead.\n", DeadPort);
		 fflush(stderr); 
#endif DEBUG
		
	}
}

static void DoRequest(inp)
msg_header_t *inp;
{
	typedef struct {
		msg_header_t    head;
		msg_type_t      ttype;
		int             retcode;
		int             data[1000];
	}               my_message_t;

	my_message_t    outmsg;
	int             gr;

	LAST_REQUEST_TIME = time(0);
	REQUESTS++;

	if (!ipcx_server(inp, &outmsg)) {
		fprintf(stderr, "Couldn't deal with message.\n");
		PrintMsg(inp);
		fflush(stderr);
	}
	if (outmsg.retcode != MIG_BAD_ID) {
		if ((gr = msg_send(&outmsg, MSG_OPTION_NONE, 0))
		    != SEND_SUCCESS)
			PrintGR(gr, "main.DoRequest.msg_send");
	}
}

static int mainloop()
{
	typedef struct {
		msg_header_t    head;
		msg_type_t      ttype;
		int             deadport;
		int             data[1000];
	}               my_message_t;

	my_message_t    inmsg;
	int             gr;
	int             clock;

	while (TRUE) {
		if ((!NAME_SERVER_UP) &&
		    (netname_look_up(name_server_port, "", "nmmonitor",
			&nm_monitor_port) == KERN_SUCCESS) &&
		    (assert_names( PATTERN, SERVICE_PORT) == KERN_SUCCESS) &&
		    (netname_check_in(name_server_port, HANDSHAKE,
			       task_self(), RETURN_PORT) == KERN_SUCCESS)) {
			NAME_SERVER_UP = 1;
			LAST_REQUEST_TIME = time(0);
#if DEBUG
			fprintf(stderr,"nm_monitor_port is %d\n",nm_monitor_port);
			fflush(stderr);
#endif DEBUG
		}
		patch_cmd_line();

		check_on_kids();

		inmsg.head.msg_size = sizeof(inmsg);
		inmsg.head.msg_local_port = PORT_DEFAULT;

		gr = msg_receive(&inmsg, RCV_TIMEOUT,
				 NAME_SERVER_UP ? 300000 : 30000);

		if (gr == RCV_TIMED_OUT) {	/* Don`t do anything */
		} else if (gr != RCV_SUCCESS)
			PrintGR(gr, "main.mainloop.Receive");
		else if (inmsg.head.msg_id == NOTIFY_PORT_DELETED)
			DoPortDeath(inmsg.deadport);
		else
			DoRequest(&inmsg);
	}
}

main(argc, argv)
int argc;
char *argv[];
{
	args(argc, argv);
	if (isChild) 
	{	child_proc(argc,argv);
		fprintf(stderr, "Exec of target process returned\n");
		exit(13);
	}
	init(argv);
	mainloop();
}

