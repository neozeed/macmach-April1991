/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	procs.c,v $
 * Revision 1.1  90/02/19  21:47:56  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.5  89/07/04  17:37:33  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 14-Jan-88  Alessandro Forin (af) at Carnegie-Mellon University
 *	Changed to use standard Berkeley su rather than CMU ru.  Made
 *	sure childs are detached from the control tty to make this work.
 *	It will only work for accounts that have a shell as login program.
 *
 *   4-Jan-88  Mary Thompson at Carnegie Mellon
 *	Added Child_proc which is called when ipcexecd ru's itself.
 *	This process returns the ports to the parent and execs the
 *	target process.
 *
 *  30-Dec-87  Mary Thompson at Carnegie Mellon
 *	Changed to do a /bin/ru of ./child which will
 *	then send the ports to ipcexecd and execv the real
 *	program. 
 *
 *  8-May-87  Mary Thompson at Carnegie Mellon
 *	Changed startserver to return task_notify()
 *	instead of task_data(). Added CheckPassword to
 *	check that password and user id are valid.
 *
 *  1-May-87  Alessandro Forin at Carnegie Mellon
 *	Machized Jeff Eppinger's version
 *
 */
/*
 *  Abstract:
 *	This module contains the procedure startserver
 *	which is called by the client via the ipcxServer
 *	and the utility routines: 
 *		Send_Ports, Recv_Ports, DecryptPassword
 *		SkipBlanks, FindBlank, CoundCmdLineArgs,
 *		ParseCmdLine, PrintArgv, CheckFile, FixIO,
 *		and Parse_Exec.
 * Author:
 *	Alessandro Forin
 */
#include <stdio.h>
#include <pwd.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <mach.h>
#include <mach/message.h>
#include "ipcx_types.h"

#ifndef TRUE
#define TRUE	(1)
#define FALSE	(0)
#endif

extern port_t RETURN_PORT;
extern char *HANDSHAKE;
extern char *PROGNAME;
extern char *OUTFILE;

extern char *index();
extern char *strncat();
extern char *strcmp();
extern char *crypt();
extern struct passwd *getpwnam();
extern char **environ;

typedef struct {
	    msg_header_t      head;
	    msg_type_t kportTType;
	    port_t     kport;
	    msg_type_t dportTType;
	    port_t     dport;
	} return_msg_t;

static int Send_Ports(theKPort, theDPort)
port_t theKPort, theDPort;
{
	return_msg_t    m;
	port_t          returnport;
	int             gr;

#if DEBUG
	  fprintf(stderr, "Child:   sending ports to parent!\n");
	  fflush(stderr); 
#endif DEBUG


	if ((gr = netname_look_up(NameServerPort,
				  "", HANDSHAKE, &returnport))
	    != KERN_SUCCESS) {
		fprintf(stderr,
		 "procs.Send_Ports.netname_look_up(returnport): %d\n.", gr);
		fflush(stderr);
		return (gr);
	}
#if DEBUG
	  fprintf(stderr, "Child: sending my ports to returnport %d.\n",
	  returnport); fflush(stderr); 
#endif DEBUG

	m.head.msg_size = sizeof(return_msg_t);
	m.head.msg_type = MSG_TYPE_NORMAL;
	m.head.msg_local_port = PORT_NULL;
	m.head.msg_remote_port = returnport;
	m.head.msg_simple = FALSE;
	m.head.msg_id = 66;

	m.kportTType.msg_type_inline = TRUE;
	m.kportTType.msg_type_deallocate = FALSE;
	m.kportTType.msg_type_longform = FALSE;
	m.kportTType.msg_type_name = MSG_TYPE_PORT;
	m.kportTType.msg_type_size = 32;
	m.kportTType.msg_type_number = 1;
	m.kport = theKPort;

	m.dportTType = m.kportTType;
	m.dport = theDPort;

	gr = msg_send(&m, MSG_OPTION_NONE, 0);
	port_deallocate(task_self(), returnport);
	return (gr);
}


static int Recv_Ports(pKPort, pDPort)
port_t *pKPort, *pDPort;
{
	return_msg_t    m;
	int             gr;

#if DEBUG
	  fprintf(stderr, "parent: receiving child`s ports\n");
	  fflush(stderr); 
#endif DEBUG

	m.head.msg_size = sizeof(return_msg_t);
	m.head.msg_local_port = RETURN_PORT;
	gr = msg_receive(&m, MSG_OPTION_NONE, 30000);
	if (gr != KERN_SUCCESS) {
		fprintf(stderr, "Could not receive child`s ports: %d\n", gr);
		fflush(stderr);
		return (gr);
	}
	*pKPort = m.kport;
	*pDPort = m.dport;

#if DEBUG
	  fprintf(stderr, "parent: kport=%d dport=%d\n", *pKPort, *pDPort);
	  fflush(stderr); 
#endif DEBUG

	return (KERN_SUCCESS);
}


static void DecryptPassword(password)
char *password;
{
	int             i;

	for (i = 0; password[i]; i++)
		password[i] = (password[i] - 1) ^ (((i + 1) * 11) % 128);
}

static char *SkipBlanks(pos)
char *pos;
{
	char            ch;

	ch = *pos;
	while ((ch != '\0') && (ch == ' ')) {
		pos++;
		ch = *pos;
	}

	return (pos);
}

static char *FindBlank(pos)
char *pos;
{
	char            ch;

	ch = *pos;
	while ((ch != '\0') && (ch != ' ')) {
		pos++;
		ch = *pos;
	}
	return (pos);
}

static int CountCmdLineArgs(cmdline)
char *cmdline;
{
	char           *start_pos;
	int             count;

	start_pos = SkipBlanks(cmdline);
	count = 0;
	while (*start_pos != '\0') {
		count++;
		start_pos = FindBlank(start_pos);
		start_pos = SkipBlanks(start_pos);
	}
	return (count);
}

static void ParseCmdLine(cmdline, argv)
char *cmdline, *argv[];
{
	char           *start_pos, *end_pos;
	int             count;

	start_pos = SkipBlanks(cmdline);
	count = 0;
	while (*start_pos != '\0') {
		end_pos = FindBlank(start_pos);

		argv[count] = (char *) malloc(end_pos - start_pos + 1);
		*argv[count] = '\0';
		strncat(argv[count], start_pos, end_pos - start_pos);
		count++;

		start_pos = SkipBlanks(end_pos);
	}
}

static void PrintArgv(argv)
char **argv;
{
	int             argc;

	for (argc = 0; argv[argc]; argc++) {
		fprintf(stderr, "argv[%d] = `%s`\n", argc, argv[argc]);
		fflush(stderr);
	}
}

static int CheckFile(cmdline)
char *cmdline;
{
	char           *start_pos, *end_pos, filename[255];

	start_pos = SkipBlanks(cmdline);
	end_pos = FindBlank(start_pos);

	filename[0] = '\0';
	strncat(filename, start_pos, end_pos - start_pos);

	return (access(filename, 1) == 0);
}

static int CheckPassword(uid,password)
    char	*uid, *password;
{
    struct passwd *pw;

    pw = getpwnam(uid);
    if (pw == NULL)
	return(1);

    /* if there is a password and we want it checked, do so */
    if (*(pw->pw_passwd) && password != NULL) {
	if (strcmp(pw->pw_passwd, crypt(password, pw->pw_passwd)) != 0)
	    return(2);
	return(0);
    }
}

static void FixIO()
{
	/*
	 * Can`t do this:
	 * if (freopen(INFILE, "r", stdin) != stdin) exit(101);
	 * otherwise the password won`t be sent to ru.
	 */
	if (freopen(OUTFILE, "a", stdout) != stdout)
		exit(102);
	if (freopen(OUTFILE, "a", stderr) != stderr)
		exit(103);
}

static void Parse_Exec(cmdline)
char *cmdline;
{
	int             argc;
	char          **argv;

	argc = CountCmdLineArgs(cmdline) + 1;
	argv = (char **) malloc(sizeof(int) * (argc + 1));
	ParseCmdLine(cmdline, argv);
	argv[argc] = 0;

#if DEBUG
	  PrintArgv(argv); 
#endif DEBUG

	execve(argv[0], argv, environ);
	fprintf(stderr,
	      "procs.Parse_Exec.execv:  Failure:  cmdline=`%s`\n", cmdline);
	fflush(stderr);
	exit(11);
}


void child_proc (argc,argv)
   int argc;
   char *argv[];
{
    kern_return_t	gr;
    int		i;

#if DEBUG
	printf("child: ru succeeded\n");
#endif DEBUG

	i = 1;
	while (argv[i][0] == '-') i += 2;

#if DEBUG
	PrintArgv(&argv[i]);
#endif DEBUG
	if ((gr = Send_Ports(task_self(), task_notify())) != KERN_SUCCESS)
	{	fprintf(stderr,"Child: Send_Ports error return is %d\n",gr);
		fflush(stderr);
	}
	FixIO();
	execve(argv[i], &argv[i], environ);
	fprintf(stderr,
	      "Child:execv:  Failure:  progname=`%s`\n", argv[1]);
	fflush(stderr);
	exit(14);
}


int startserver(servport, userid, password, commandline,
			  serverkport, serverdport)
port_t servport, *serverkport, *serverdport;
a_string userid, password, commandline;
{
	char           *sucmdline;
	int             gr;
	int             pid, fildes[2], pwdlen, clock;

	*serverkport = PORT_NULL;
	*serverdport = PORT_NULL;

	clock = time(0);
	fprintf(stderr, "%24.24s  StartServer:  userid=%s  commandline=%s\n",
		ctime(&clock), userid, commandline);
	fflush(stderr);


	if (!CheckFile(commandline))
		return (IPCX_FILENOTFOUND);

	DecryptPassword(password);
	if (CheckPassword(userid, password) != 0)
		return (IPCX_USERWRONG);

	if (pipe(fildes) < 0) {
		fprintf(stderr, "procs.startserver.pipe:  Failure.\n");
		fflush(stderr);
		return (KERN_FAILURE);
	}
	switch (pid = fork()) {
	case -1:		/* fork failed */
		close(fildes[0]);
		close(fildes[1]);

		fprintf(stderr, "procs.startserver.fork:  Failure.\n");
		fflush(stderr);
		return (IPCX_FORKFAILED);
	case 0:		/* child process */
		pid = getpid();
		setpgrp(pid, pid);
		fprintf(stderr, "Child: mypid=%d\n", pid);
		fflush(stderr);

		/* make sure we do not have a controlling tty */
		{
			int             f = open("/dev/tty", O_RDONLY);
			if (f >= 0) {
				ioctl(f, TIOCNOTTY, 0);
				close(f);
			}
		}

		close(fildes[1]);
		dup2(fildes[0], 0);
		close(fildes[0]);

		/*
		 * Ok, here is how it works.  We exec the su program in the
		 * child, and the parent "types in" the password through the
		 * pipe to su. SU is told to start fast (-f) and it will
		 * start a shell.  To the shell we ask to execute a single
		 * command, which must be a single string and we build that
		 * in sucmdline. It will look like "exec ipcexecd -o output
		 * -h port <cmd>", where <cmd> is the user request. After
		 * that, the new ipcexecd will recognize the "-h" switch, and
		 * send us back the ports, and finally exec the user`s
		 * command line.  whew!
		 */

		sucmdline = (char *) malloc(strlen(commandline) + 256);
		sprintf(sucmdline, "exec %s -o %s -h %s %s",
			PROGNAME, OUTFILE, HANDSHAKE, commandline);
		execle("/bin/su",
		       "/bin/su", "-f", userid, "-c", sucmdline, 0,
		       environ);

		exit(-1);
	default:		/* parent process */
		fprintf(stderr, "Started as process %d\n", pid);
		fflush(stderr);

		close(fildes[0]);
		pwdlen = strlen(password);

		if ((write(fildes[1], password, pwdlen) != pwdlen) ||
		    (write(fildes[1], "\n", 1) != 1)) {
			fprintf(stderr, "procs.startserver.write: Failure.\n");
			fflush(stderr);
			close(fildes[1]);
			return (KERN_FAILURE);
		}
		close(fildes[1]);

		if ((gr = Recv_Ports(serverkport, serverdport)) != KERN_SUCCESS) {
			PrintGR(gr, "Parent: procs.startserver.Recv_Ports");
			return (gr);
		}
		return (KERN_SUCCESS);
	}
}
