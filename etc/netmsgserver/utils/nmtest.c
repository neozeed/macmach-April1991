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
 * $Log:	nmtest.c,v $
 * Revision 1.2  90/02/25  17:24:54  bww
 * 	Removed & before array.
 * 	[90/02/25  17:23:11  bww]
 * 
 * Revision 1.1  90/02/19  21:58:40  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/05/02  11:20:33  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 19-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 * nmtest.c
 *
 *
 */
#ifndef	lint
static char     rcsid[] = "$ Header: nmtest.c,v 1.4 89/05/02 11:20:33 dpj Exp $";
#endif not lint
/*
 * Program to test various operations of the network server.
 */



#include	"debug.h"
#include	"trace.h"
#include	"nm_defs.h"

#include	<stdio.h>
#include	<mach.h>
#include	<cthreads.h>
#include	<servers/netname.h>
#include	<mach/message.h>
#include	<mach/msg_type.h>
#include	<mach/notify.h>

#define readln(p) while (((getc(p))!='\n') && (!feof(p)))
#define NETNAME_NAME	"NEW_NETWORK_NAME_SERVER"

#define	MAXPORTS	40

#define	RIGHT_SEND	0x1
#define	RIGHT_RECEIVE	0x2
#define	RIGHT_OWNER	0x4


/*
 * Global variables
 */
static port_t		network_server_port;		/* port to new network server */
static struct ports {
	port_t	port;
	char	host[100];
	char	name[100];
	long	rights;
} ports[MAXPORTS];
int	last_port;
struct	my_msg {
	msg_header_t	header;
	msg_type_long_t	desc;
	long		data;
} send_buff;
long	current_seqnum;
char	rcv_msg[100];

#define	CHECK_PORT(index,stmt) {						\
	if ((index < 0) || (index > last_port) || (ports[index].rights == 0)) {	\
		fprintf(stdout,"Invalid port number: %d\n", index);		\
		stmt;								\
	}									\
}

/*
 * Tracing values.
 */
int	trace_recursion_level = 0;
#undef	tracing_on
int	tracing_on = 0;

/*
 * input
 *
 * Obtain user input, allowing multiple inputs on the same line.
 *
 * Parameters:
 *
 * prompt: string to use as a prompt.
 * format: specification for data required.
 * OUT var: address where to put the data read.
 *
 * Results:
 *
 * Side effects:
 *
 * May print a prompt on stdout, and start a read on stdin.
 *
 * Design:
 *
 * Note:
 *
 */
void input(prompt, format, var)
char *prompt;
char *format;
char *var;
BEGIN("input")
	char c;
	int fcnt;

	while ((stdin->_cnt > 0) &&
	    ((((c = *(stdin)->_ptr&0377) == ' ') || (c == '\n'))))
		getc(stdin);
	fcnt = stdin->_cnt;

	if (fcnt == 0) {
		fprintf(stdout, "%s", prompt);
		fflush(stdout);
	}
	fscanf(stdin, format, var);

	RET;
END


/*
 * nmconnnect
 *
 * Connect to the network name server.
 */
void nmconnect()
BEGIN("nmconnect")
	kern_return_t	kr;

	kr = netname_look_up(name_server_port, "", NETNAME_NAME, &network_server_port);
	if (kr == NETNAME_SUCCESS) {
		fprintf(stdout,
			"Look up of new network server succeeds, port = %x.\n",
			network_server_port);
	} else {
		fprintf(stdout,
	"Look up of new network server fails, using old network server, port = %x.\n",
			name_server_port);
		network_server_port = name_server_port;
	}
	RET;
END


/*
 * printmenu
 *
 * Parameters: none
 *
 * Results:
 *
 * Side effects:
 *
 * Print a command menu on stdout.
 *
 * Design:
 *
 * Note:
 *
 */
void printmenu()
BEGIN("printmenu")
	fprintf(stdout, "\n");
	fprintf(stdout, "This programs allows to perform tests on a network server.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Available commands:\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "    H,?.......Print this message\n");
	fprintf(stdout, "    C.........(Re)connect to network server\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Ports:\n");
	fprintf(stdout, "       PA.........Allocate\n");
	fprintf(stdout, "       PC,PI......Check in <port> <name>\n");
	fprintf(stdout, "       PO.........Check out <port>\n");
	fprintf(stdout, "       PL.........Look up <host> <name>\n");
	fprintf(stdout, "       PD.........Deallocate <port>\n");
	fprintf(stdout, "       PS.........Print status of all known ports\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Messages:\n");
	fprintf(stdout, "       MI.........Send IPC <remote-port> <local-port>\n");
	fprintf(stdout, "       MR.........Send RPC <remote-port> <local-port>\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Rights:\n");
	fprintf(stdout, "       RS.........Transfer send rights <port> <destination>\n");
	fprintf(stdout, "       RR.........Transfer receive rights <port> <destination>\n");
	fprintf(stdout, "       RO.........Transfer ownership rights <port> <destination>\n");
	fprintf(stdout, "       RA.........Transfer all rights <port> <destination>\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "    Q.........Quit\n");
	fprintf(stdout, "\n");
	RET;
END


/*
 * printport
 *
 * Print all information on a port.
 */
void printport(str,index)
	char	*str;
	int	index;
BEGIN("printport")
	CHECK_PORT(index,return);
	fprintf(stdout,"%20s %2d: 0x%8x ",str,index,ports[index].port);
	if (ports[index].rights & RIGHT_SEND)
		fprintf(stdout,"SEND ");
	else
		fprintf(stdout,"     ");
	if (ports[index].rights & RIGHT_RECEIVE)
		fprintf(stdout,"RECEIVE ");
	else
		fprintf(stdout,"        ");
	if (ports[index].rights & RIGHT_OWNER)
		fprintf(stdout,"OWNER ");
	else
		fprintf(stdout,"      ");
	fprintf(stdout," \"%s\" \"%s\"\n",ports[index].host,ports[index].name);
	RET;
END


/*
 * findport
 *
 * Make sure there is a record for a given port.
 */
int findport(port)
	port_t	port;
BEGIN("findport")
	int	port_index;
	int	i;

	port_index = -1;
	for (i = 0; i < MAXPORTS; i++) {
		if (ports[i].port == port) {
			port_index = i;
			break;
		}
	}
	if (port_index == -1) {
		if (last_port >= (MAXPORTS - 1)) {
			port_index = 0;
			fprintf(stdout,"** Sorry, you have exceeded the maximum **\n");
			fprintf(stdout,"** number of ports for this program.    **\n");
			fflush(stdout);
		} else {
			last_port++;
			port_index = last_port;
			ports[port_index].port = port;
			ports[port_index].host[0] = '\0';
			ports[port_index].name[0] = '\0';
			ports[port_index].rights = 0;
		}
	}

	RETURN(port_index);
END


/*
 * genericports
 *
 * Execute any port operation.
 */
void genericports()
BEGIN("genericports")
	char 		subcmd[2];
	kern_return_t	kr;
	int		port_index;
	char		host[100];
	char		name[100];
	port_t		new_port;

	input("Enter Port sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
		case 'A':
		case 'a':
			kr = port_allocate(task_self(),&new_port);
			if (kr == KERN_SUCCESS) {
				port_index = findport(new_port);
				strcpy(ports[port_index].host,"local");
				ports[port_index].rights = 
					RIGHT_SEND | RIGHT_RECEIVE | RIGHT_OWNER;
				(void)port_enable(task_self(),ports[port_index].port);
				printport("Allocated port",port_index);
			} else {
				fprintf(stdout,"port_allocate returns %d\n",kr);
			}
			break;
		case 'C':
		case 'c':
		case 'I':
		case 'i':
			input("Port: ", "%d", (char *)&port_index);
			CHECK_PORT(port_index,break);
			input("Name: ", "%99s", name);
			kr = netname_check_in(network_server_port, name,
						PORT_NULL, ports[port_index].port);
			if (kr == NETNAME_SUCCESS) {
				fprintf(stdout,"Check in succeeds.\n");
				strcpy(ports[port_index].host,"local");
				strcpy(ports[port_index].name,name);
			} else {
				fprintf(stdout,"netname_check_in returns %d\n",kr);
			}
			break;
		case 'O':
		case 'o':
			input("Port: ", "%d", (char *)&port_index);
			CHECK_PORT(port_index,break);
			kr = netname_check_out(network_server_port, 
						ports[port_index].name,PORT_NULL);
			if (kr == NETNAME_SUCCESS) {
				fprintf(stdout,"Check out succeeds.\n");
				ports[port_index].name[0] = '\0';
			} else {
				fprintf(stdout,"netname_check_out returns %d\n",kr);
			}
			break;
		case 'L':
		case 'l':
			input("Host: ", "%99s", host);
			input("Name: ", "%99s", name);
			kr = netname_look_up(network_server_port, host, name, &new_port);
			if (kr == NETNAME_SUCCESS) {
				fprintf(stdout,"Look up succeeds:\n");
				port_index = findport(new_port);
				strcpy(ports[port_index].host,host);
				strcpy(ports[port_index].name,name);
				ports[port_index].rights |= RIGHT_SEND;
				printport("Found port",port_index);
			} else {
				fprintf(stdout,"netname_look_up returns %d\n",kr);
			}
			break;
		case 'D':
		case 'd':
			input("Port: ", "%d", (char *)&port_index);
			CHECK_PORT(port_index,break);
			kr = port_deallocate(task_self(),ports[port_index].port);
			if (kr == KERN_SUCCESS) {
				fprintf(stdout,"Port 0x%x deallocated.\n",
							ports[port_index].port);
				ports[port_index].rights = 0;
			} else {
				fprintf(stdout,"port_deallocate returns %d\n",kr);
			}
			break;
		case 'S':
		case 's':
			for (port_index = 0; port_index < MAXPORTS; port_index++) {
				if (ports[port_index].rights != 0)
					printport("Port",port_index);
			}
			break;
		default:
			fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
			readln(stdin);
			break;
	}
	fflush(stdout);
	RET;
END


/*
 * genericmsg
 *
 * Execute any message operation.
 */
void genericmsg()
BEGIN("genericmsg")
	char		subcmd[2];
	int		msg_type;
	int		remote_port;
	int		local_port;
	kern_return_t	kr;

	input("Enter Message sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
		case 'I':
		case 'i':
			msg_type = MSG_TYPE_NORMAL;
			break;
		case 'R':
		case 'r':
			msg_type = MSG_TYPE_RPC;
			break;
		default:
			fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
			readln(stdin);
			RET;
	}

	input("Remote Port: ", "%d", (char *)&remote_port);
	input("Local Port: ", "%d", (char *)&local_port);
	CHECK_PORT(remote_port,return);
	CHECK_PORT(local_port,return);

	send_buff.header.msg_simple = TRUE;
	send_buff.header.msg_size = sizeof(struct my_msg);
	send_buff.header.msg_type = msg_type;
	send_buff.header.msg_local_port = ports[local_port].port;
	send_buff.header.msg_remote_port = ports[remote_port].port;
	send_buff.header.msg_id = current_seqnum;

	send_buff.desc.msg_type_long_name = MSG_TYPE_INTEGER_32;
	send_buff.desc.msg_type_long_size = 32;
	send_buff.desc.msg_type_long_number = 1;
	send_buff.desc.msg_type_header.msg_type_inline = TRUE;
	send_buff.desc.msg_type_header.msg_type_longform = TRUE;
	send_buff.desc.msg_type_header.msg_type_deallocate = FALSE;
	send_buff.data = current_seqnum;

	kr = msg_send((msg_header_t *)&send_buff, MSG_OPTION_NONE, 0);
	if (kr == SEND_SUCCESS) {
		fprintf(stdout,"Message away,seqnum = %d\n",current_seqnum);
	} else {
		fprintf(stdout,"msg_send returned %d\n",kr);
	}
	current_seqnum++;

	fflush(stdout);
	RET;
END


/*
 * genericrights
 *
 * Execute any rights operation.
 */
void genericrights()
BEGIN("genericrights")
	char		subcmd[2];
	int		rights;
	int		rightmask;
	int		destination_port;
	int		object_port;
	kern_return_t	kr;

	input("Enter Rights sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
		case 'S':
		case 's':
			rights = MSG_TYPE_PORT;
			rightmask = 0;
			break;
		case 'R':
		case 'r':
			rights = MSG_TYPE_PORT_RECEIVE;
			rightmask = RIGHT_RECEIVE;
			break;
		case 'O':
		case 'o':
			rights = MSG_TYPE_PORT_OWNERSHIP;
			rightmask = RIGHT_OWNER;
			break;
		case 'A':
		case 'a':
			rights = MSG_TYPE_PORT_ALL;
			rightmask = RIGHT_RECEIVE | RIGHT_OWNER;
			break;
		default:
			fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
			readln(stdin);
			RET;
	}

	input("Object Port: ", "%d", (char *)&object_port);
	input("Destination Port: ", "%d", (char *)&destination_port);
	CHECK_PORT(object_port,return);
	CHECK_PORT(destination_port,return);

	send_buff.header.msg_simple = FALSE;
	send_buff.header.msg_size = sizeof(struct my_msg);
	send_buff.header.msg_type = MSG_TYPE_NORMAL;
	send_buff.header.msg_local_port = PORT_NULL;
	send_buff.header.msg_remote_port = ports[destination_port].port;

	send_buff.desc.msg_type_long_name = rights;
	send_buff.desc.msg_type_long_size = 32;
	send_buff.desc.msg_type_long_number = 1;
	send_buff.desc.msg_type_header.msg_type_inline = TRUE;
	send_buff.desc.msg_type_header.msg_type_longform = TRUE;
	send_buff.desc.msg_type_header.msg_type_deallocate = FALSE;
	send_buff.data = (long)ports[object_port].port;

	kr = msg_send((msg_header_t *)&send_buff, MSG_OPTION_NONE, 0);
	if (kr == SEND_SUCCESS) {
		ports[object_port].rights &= ~rightmask;
	} else {
		fprintf(stdout,"msg_send returned %d\n",kr);
	}

	fflush(stdout);
	RET;
END


/*
 * main
 *
 * Main loop.
 *
 * Parameters:
 *
 * Results:
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
main(argc, argv)
int argc;
char **argv;
BEGIN("main")
	int quit;
	char cmd[2];

	(void)initialize();

	quit = FALSE;
	do {
		input("> ", "%1s", cmd);
		switch (cmd[0]) {
		case 'H':
		case 'h':
		case '?':
			printmenu();
			break;
		case 'C':
		case 'c':
			nmconnect();
			break;
		case 'P':
		case 'p':
			genericports();
			break;
		case 'M':
		case 'm':
			genericmsg();
			break;
		case 'R':
		case 'r':
			genericrights();
			break;
		case 'Q':
		case 'q':
			quit = TRUE;
			break;
		default:
			fprintf(stdout, "Invalid command. Type ? for list.\n");
			readln(stdin);
			break;
		}
	} 
	while (!quit);

	fprintf(stdout, "Terminated.\n");
	exit(0);
END


/*
 * receive_main
 *
 * Main loop for the msg receive thread.
 */
void receive_main()
BEGIN("receive_main")
	msg_return_t	msg_ret;
	int		index;
	struct my_msg	*rcv_buff;
	notification_t	*notify_buff;

	for (;;) {
		rcv_buff = (struct my_msg *)rcv_msg;
		rcv_buff->header.msg_local_port = PORT_ENABLED;
		rcv_buff->header.msg_size = sizeof(struct my_msg);
		rcv_buff->desc.msg_type_long_name = 0;
		rcv_buff->data = 0;

		msg_ret = msg_receive(rcv_buff,MSG_OPTION_NONE,0);
		if (msg_ret != RCV_SUCCESS) {
			fprintf(stdout,"msg_receive returned %d\n",msg_ret);
			fflush(stdout);
			continue;
		}

		if (rcv_buff->header.msg_local_port == task_notify()) {
			fprintf(stdout,"\nReceived a notification:\n");
			notify_buff = (notification_t *)rcv_buff;
			index = findport((port_t)notify_buff->notify_port);
			switch(notify_buff->notify_header.msg_id) {
				case NOTIFY_RECEIVE_RIGHTS:
					ports[index].rights |= 
							RIGHT_RECEIVE | RIGHT_SEND;
					(void)port_enable(task_self(),ports[index].port);
					printport(" Receive rights",index);
					break;
				case NOTIFY_OWNERSHIP_RIGHTS:
					ports[index].rights |= RIGHT_OWNER | RIGHT_SEND;
					printport(" Ownership rights",index);
					break;
				case NOTIFY_PORT_DELETED:
					printport(" Port dead",index);
					ports[index].rights = 0;
					break;
				default:
					fprintf(stdout,"Unknown notification: %d\n",
						notify_buff->notify_header.msg_id);
					break;
			}
		} else {
			fprintf(stdout,"\nReceived a message:\n");
			index = findport(rcv_buff->header.msg_local_port);
			ports[index].rights |= RIGHT_RECEIVE;
			printport("Local port",index);
			index = findport(rcv_buff->header.msg_remote_port);
			ports[index].rights |= RIGHT_SEND;
			printport("Remote port",index);
			switch(rcv_buff->desc.msg_type_long_name) {
				case MSG_TYPE_INTEGER_32:
					if (rcv_buff->header.msg_type & MSG_TYPE_RPC) {
						fprintf(stdout,"   RPC  ");
					} else {
						fprintf(stdout,"        ");
					}
					fprintf(stdout,"  Data = %d\n",
									rcv_buff->data);
					break;
				case MSG_TYPE_PORT:
					index = findport((port_t)rcv_buff->data);
					ports[index].rights |= RIGHT_SEND;
					printport(" Send rights",index);
					break;
				case MSG_TYPE_PORT_RECEIVE:
					index = findport((port_t)rcv_buff->data);
					ports[index].rights |= 
							RIGHT_RECEIVE | RIGHT_SEND;
					(void)port_enable(task_self(),ports[index].port);
					printport(" Receive rights",index);
					break;
				case MSG_TYPE_PORT_OWNERSHIP:
					index = findport((port_t)rcv_buff->data);
					ports[index].rights |= RIGHT_OWNER | RIGHT_SEND;
					printport(" Ownership rights",index);
					break;
				case MSG_TYPE_PORT_ALL:
					index = findport((port_t)rcv_buff->data);
					ports[index].rights |= 
						RIGHT_RECEIVE | RIGHT_OWNER |RIGHT_SEND;
					(void)port_enable(task_self(),ports[index].port);
					printport(" All rights",index);
					break;
				default:
					fprintf(stdout,"Unknown message data: %d\n",
						rcv_buff->desc.msg_type_long_name);
					break;
			}
		}
		fflush(stdout);
	}
	/* NOTREACHED */
END


/*
 * initialize
 *
 * Initialize all the test functions.
 */
int initialize()
BEGIN("initialize")
	int		i;
	cthread_t	receive_thread;

	/*
	 * Connect to name server.
	 */
	init_netname(PORT_NULL);
	nmconnect();

	current_seqnum = 1;

	/*
	 * Prepare port list.
	 */
	for (i = 0; i < MAXPORTS; i++)
		ports[i].rights = 0;
	last_port = 0;
	ports[0].port = PORT_NULL;
	ports[0].host[0] = '\0';
	strcpy(ports[0].name,"NULLPORT");
	ports[0].rights = RIGHT_SEND;

	/*
	 * Set up the msg receive thread.
	 */
	cthread_init();
	receive_thread = cthread_fork(receive_main, 0);
	cthread_set_name(receive_thread, "receive_main");
    	cthread_detach(receive_thread);

	(void)port_enable(task_self(),task_notify());

	RETURN(1);
END


