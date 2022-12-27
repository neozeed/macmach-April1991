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
 * $Log:	trint_test.c,v $
 * Revision 1.1  90/02/19  21:57:46  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.5  89/05/02  11:18:58  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 30-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created from rds's trans_test. 
 *
 */
/*
 * trint_test.c 
 *
 *
 */

#ifndef	lint
char trint_test_rcsid[] = "$ Header: trint_test.c,v 1.5 89/05/02 11:18:58 dpj Exp $";

#endif not lint

/*
 * Tests for transport protocols - request-response interface. 
 */


#include <mach.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "crypt.h"
#include "debug.h"
#include "disp_hdr.h"
#include "dispatcher.h"
#include "ipc_rec.h"
#include "keyman.h"
#include "mem.h"
#include "netipc.h"
#include "netmsg.h"
#include "nm_defs.h"
#include "nm_extra.h"
#include "sbuf.h"
#include "transport.h"

#define readln(p) while (((getc(p))!='\n') && (!feof(p)))
#define	FPRINTF(arg)	{					\
	mutex_lock(log_lock);					\
	fprintf arg;						\
	fflush(stdout);						\
	mutex_unlock(log_lock);					\
}


#define TRINT_DEBUG	0

#define	DISPE_TRINT_IPC	1
#define	DISPE_TRINT_RPC 2
#define	DISP_TRINT_IPC	(DISPE_TRINT_IPC	+ DISPATCHER_VERSION)
#define	DISP_TRINT_RPC	(DISPE_TRINT_RPC	+ DISPATCHER_VERSION)

/*
 * Global variables.
 */
#define	PENDING_MAX	20
static int		encryption;
static struct trans {
	int	trmod;
	int	trid;
	char	client_name[100];
} 			pending_list[PENDING_MAX];
static struct mutex	pending_lock;


/*
 * host_address --
 *
 * Return a host address for a given host name. 
 *
 */
PRIVATE long host_address(hn)
	char                           *hn;
{
	struct hostent		*hp;
	static char		chn[100];
	static long		cha;

	if (strcmp(chn,hn) == 0) {
		return (cha);
	} else {
		if ((hp = gethostbyname(hn)) == 0)
			return (0);
		else {
			cha = *(long *) (hp->h_addr);
			strcpy(chn,hn);
			return (cha);
		}
	}
}


/*
 * host_name --
 *
 * Return a host name for a given host address. 
 *
 */
PRIVATE char *host_name(ha)
	long			ha;
{
	struct hostent		*hp;
	static char		chn[100];
	static long		cha;

	if (cha == ha) {
		return (chn);
	} else {
		if ((hp = gethostbyaddr(&ha,sizeof(long),AF_INET)) == 0)
			return (0);
		else {
			cha = ha;
			strcpy(chn,hp->h_name);
			return (chn);
		}
	}
}




/*
 * tr_error_to_string --
 *
 * Parameters: tr_error	: the error to translate 
 *
 * Results: pointer to a string giving a printable representation of the error 
 *
 */
PRIVATE char                   *
                                tr_error_to_string(tr_error)
	int                             tr_error;
{
	switch (tr_error) {
		case TR_SUCCESS:
			return ("TR_SUCCESS");
		case TR_FAILURE:
			return ("TR_FAILURE");
		case TR_REMOTE_ACCEPT:
			return ("TR_REMOTE_ACCEPT");
		case TR_REMOTE_REJECT:
			return ("TR_REMOTE_REJECT");
		case TR_CRYPT_FAILURE:
			return ("TR_CRYPT_FAILURE");
		case TR_SEND_FAILURE:
			return ("TR_SEND_FAILURE");
		case TR_OVERLOAD:
			return ("TR_OVERLOAD");
		default:
			return ("Unknown Error");
	}

}


/*
 * handle_in_request --
 */
int handle_in_request(trmod,trid,data_ptr,from,crypt_level,broadcast)
	int		trmod;
	int		trid;
	sbuf_ptr_t	data_ptr;
	netaddr_t	from;
	int		crypt_level;
	boolean_t	broadcast;
{
	char		client_name[100];
	int		size;
	sbuf_seg_ptr_t	seg_ptr;
	disp_hdr_ptr_t	dhp;
	int		i;
	struct trans	*tp;

	strcpy(client_name,host_name(from));
	SBUF_GET_SIZE((*data_ptr),size);
	seg_ptr = data_ptr->segs;
	while (seg_ptr->s == 0)		/* skip past empty segments */
		seg_ptr++;
	dhp = (disp_hdr_ptr_t) seg_ptr->p;
	FPRINTF((stdout," Received a request from %s, trmod=%d\n",client_name,trmod));
	FPRINTF((stdout,"   trid=0x%x, size=%d, crypt_level=%d, broadcast=%d\n",trid,size,crypt_level,broadcast));
	if (ntohs(dhp->disp_type) == DISP_TRINT_RPC) {
		mutex_lock(&pending_lock);
		for (i = 0; i < PENDING_MAX; i++) {
			if (pending_list[i].trid == 0) {
				break;
			}
		}
		if (i < PENDING_MAX) {
			tp = &pending_list[i];
			tp->trmod = trmod;
			tp->trid = trid;
			strcpy(tp->client_name,client_name);
			mutex_unlock(&pending_lock);
			FPRINTF((stdout,"   Returning DISP_WILL_REPLY, trans=%d\n",i));
			return(DISP_WILL_REPLY);
		} else {
			mutex_unlock(&pending_lock);
			FPRINTF((stdout,"   Was an RPC, but too many pending\n"));
			return(1);
		}
	} else {
		FPRINTF((stdout,"   Returning 1 at once\n"));
		return(1);
	}
}


/*
 * handle_in_reply --
 */
handle_in_reply(clid,code,data_ptr)
	sbuf_ptr_t		clid;
	int			code;
	sbuf_ptr_t		data_ptr;
{
	int		size;
	sbuf_seg_ptr_t	curseg, endseg;

	if (data_ptr != 0) {
		SBUF_GET_SIZE((*data_ptr),size);
	} else {
		size = 0;
	}
	FPRINTF((stdout," Received a reply, clid=0x%x, code=%d, size=%d\n",clid,code,size));

	curseg = clid->segs;
	endseg = clid->end;
	while (curseg < endseg) {
		if (curseg->s != 0) {
			mem_dealloc(curseg->p, curseg->s);
		}
		curseg++;
	}
	SBUF_FREE((*clid));
	mem_dealloc(clid,sizeof(sbuf_t));
}


/*
 * input --
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
		FPRINTF((stdout, "%s", prompt));
	}
	fscanf(stdin, format, var);
	RET;
END


/*
 * printmenu --
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
	mutex_lock(log_lock);
	fprintf(stdout, "\n");
	fprintf(stdout, "This programs exercises the transport modules for the network server.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Available commands:\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "    H,?.......Print this message\n");
	fprintf(stdout, "    M.........Select the transport module to use\n");
	fprintf(stdout, "    E.........Select the encryption level to use\n");
	fprintf(stdout, "    C.........Send a request (Client side)\n");
	fprintf(stdout, "    S.........Send a response (Server side)\n");
	fprintf(stdout, "    L.........List pending requests and parameters\n");
	fprintf(stdout, "    Q.........Quit\n");
	fprintf(stdout, "\n");
	mutex_unlock(log_lock);
	RET;
END

/*
 * setmodule --
 *
 * Select the transport module to use.
 */
setmodule()
{
	int	mod;

	input("Module number: ","%d",&mod);
	tr_default_entry = mod;
}


/*
 * setencryption --
 *
 * Select the encryption level to use.
 */
setencryption()
{
	int	crypt;

	input("Crypt level: ","%d",&crypt);
	encryption = crypt;
}


/*
 * clientsend --
 *
 * Send a request.
 */
clientsend(){
	char		server_name[100];
	long		server_addr;
	int		size;
	char		ipcrpc;
	sbuf_ptr_t	sb;
	disp_hdr_ptr_t	dhp;
	char		*datap;
	int		tr_ret;

	input("Server: ", "%99s",server_name);
	server_addr = host_address(server_name);
	if (server_addr == 0) {
		FPRINTF((stdout,"Unknown host: %s\n",server_name));
		return;
	}
	input("Data size: ", "%d", &size);
	dhp = (disp_hdr_ptr_t)mem_alloc(sizeof(disp_hdr_t));
	input("IPC or RPC (i/r): ","%c",&ipcrpc);
	switch (ipcrpc) {
		case 'i':
		case 'I':
			dhp->disp_type = htons(DISP_TRINT_IPC);
			break;

		case 'r':
		case 'R':
			dhp->disp_type = htons(DISP_TRINT_RPC);
			break;

		default:
			FPRINTF((stdout,"Neither IPC nor RPC specified.\n"));
			mem_dealloc(dhp,sizeof(disp_hdr_t));
			return;
	}
	dhp->src_format = conf_own_format;
	sb = (sbuf_ptr_t)mem_alloc(sizeof(sbuf_t));
	SBUF_INIT((*sb),2);
	SBUF_APPEND((*sb),dhp,sizeof(disp_hdr_t));
	if (size > 0) {
		datap = (char *)mem_alloc(size);
		SBUF_APPEND((*sb),datap,size);
	}
	tr_ret = transport_sendrequest(tr_default_entry,sb,sb,
					server_addr,encryption,handle_in_reply);
	FPRINTF((stdout," sending a request to %s, client_id=0x%x, returns %s\n",
			server_name, sb, tr_error_to_string(tr_ret))); 
}


/*
 * serversend --
 *
 * Send a reply.
 */
serversend()
{
	int		trans_num;
	struct trans	*tp;
	int		size;
	sbuf_ptr_t	sb;
	disp_hdr_ptr_t	dhp;
	char		*datap;
	int		tr_ret;
	sbuf_seg_ptr_t	curseg, endseg;

	input("Transaction number: ", "%d",&trans_num);
	mutex_lock(&pending_lock);
	if ((trans_num < 0) || (trans_num >= PENDING_MAX) ||
			(pending_list[trans_num].trid == 0)) {
		FPRINTF((stdout,"Invalid transaction, use L for list.\n"));
		return;
	}
	tp = &pending_list[trans_num];
	mutex_unlock(&pending_lock);
	input("Data size: ", "%d", &size);
	sb = (sbuf_ptr_t)mem_alloc(sizeof(sbuf_t));
	SBUF_INIT((*sb),2);
	dhp = (disp_hdr_ptr_t)mem_alloc(sizeof(disp_hdr_t));
	SBUF_APPEND((*sb),dhp,sizeof(disp_hdr_t));
	if (size > 0) {
		datap = (char *)mem_alloc(size);
		SBUF_APPEND((*sb),datap,size);
	}
	tr_ret = transport_sendreply(tp->trmod,tp->trid,1,sb,encryption);

	FPRINTF((stdout," sending a reply to %s, trid=0x%x, returns %s\n",
			tp->client_name, tp->trid, tr_error_to_string(tr_ret)));
	mutex_lock(&pending_lock);
	tp->trid = 0;
	mutex_unlock(&pending_lock);
	curseg = sb->segs;
	endseg = sb->end;
	while (curseg < endseg) {
		if (curseg->s != 0) {
			mem_dealloc(curseg->p, curseg->s);
		}
		curseg++;
	}
	SBUF_FREE((*sb));
	mem_dealloc(sb,sizeof(sbuf_t));
}


/*
 * list --
 *
 * List all pending transactions and the working parameters.
 */
list()
{
	int		i;

	mutex_lock(&pending_lock);
	FPRINTF((stdout,"Current transport module: %d\n",tr_default_entry));
	FPRINTF((stdout,"Current crypt level: %d\n",encryption));
	for (i = 0; i < PENDING_MAX; i++) {
		if (pending_list[i].trid != 0) {
			FPRINTF((stdout," %3d:  trmod=%d  trid=0x%x   client=%s\n",
				i,
				pending_list[i].trmod,
				pending_list[i].trid,
				pending_list[i].client_name));
		}
	}
	mutex_unlock(&pending_lock);
}


/*
 * main --
 * Accepts requests from the user to send data using a particular
 * transport protocol. 
 *
 */

#define USAGE	"Usage: ttest [-c] [-t] [-p #]"

extern int		cthread_debug;

main(argc, argv)
	int                             argc;
	char                          **argv;
{
	int				i;
	kern_return_t                   kr;
    boolean_t	success = TRUE;
	int quit;
	char cmd[2];

	cthread_init();

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0)
			debug.tracing = 1;
		else
			if (strcmp(argv[i], "-c") == 0)
				cthread_debug = 1;
			else
				if ((strcmp(argv[i], "-p") == 0) && ((i + 1) < argc)) {
					i++;
					debug.print_level = atoi(argv[i]);
				} else {
					fprintf(stderr, "%s\n", USAGE);
					(void) fflush(stderr);
					_exit(-1);
				}
	}

	fprintf(stdout, "%s: %s %s print_level = %d.\n", argv[0],
		(debug.tracing ? "tracing" : ""),
		(cthread_debug ? "cthread_debug" : ""),
		debug.print_level);
	(void) fflush(stdout);

	/*
	 * Initialise various modules. 
	 */
    if (!(ls_init_1())) {
	panic("ls_init_1 failed.");
	success = FALSE;
    }

    /*
     * Initialise the utility modules.
     */
    if (!(mem_init())) {
	panic("mem_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "mem_init completed");
    if (!(uid_init())) {
	panic("uid_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "uid_init completed");
    if (!(timer_init())) {
	panic("timer_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "timer_init completed");
    if (!(network_init())) {
	ERROR((msg,"Autoconf: network_init failed: no network"));
	param.conf_network = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "network_init completed");
    if (!(disp_init())) {
	panic("disp_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "disp_init completed");
    if (!(transport_init())) {
	panic("transport_init failed.");
	success = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "transport_init completed");

    if (param.conf_network) {
	    /*
	     * Initialise the transport protocols about which we know.
	     */
	    if (!(deltat_init())) {
		panic("deltat_init failed.");
		success = FALSE;
	    }
	    DEBUG_STRING(TRINT_DEBUG, 5, 2, "deltat_init completed");
	    if (!(tcp_init())) {
		panic("tcp_init failed.");
		success = FALSE;
	    }
	    DEBUG_STRING(TRINT_DEBUG, 5, 2, "tcp_init completed");
	
#if	USE_VMTP
#if	RPCMOD
	    if (!(vmtp2_init())) {
#else	RPCMOD
	    if (!(vmtp1_init())) {
#endif	RPCMOD
		ERROR((msg,"Autoconf: vmtp?_init failed."));
	    }
	    DEBUG_STRING(TRINT_DEBUG, 5, 2, "vmtp?_init completed");
#endif	VMTP

	    tr_default_entry = TR_DELTAT_ENTRY;
    } else {
	    tr_default_entry = TR_NOOP_ENTRY;
    }

    /*
     * Initialise the higher level modules.
     */
    if (!(netname_init())) {
	panic("netname_init fails.");
	success = FALSE;
    }
    DEBUG_STRING(TRINT_DEBUG, 5, 2, "netname_init completed");
    if (!(km_init())) {
	panic("km_init failed.");
	success = FALSE;
    }
    if (!(ls_init_2())) {
	panic("ls_init_2 failed.");
	success = FALSE;
    }

#if	NETPORT
    if (netport_init(task_self()) != KERN_SUCCESS) {
	ERROR((msg,"Autoconf: netport_init failed."));
	param.conf_netport = FALSE;
    } else {
	param.conf_netport = TRUE;
    }
#endif	NETPORT

	if (success != TRUE) {
		panic("Initialization failed.");
	}

	/*
	 * Initialise the dispatcher switch. 
	 */
	dispatcher_switch[DISPE_TRINT_RPC].disp_in_request = handle_in_request;
	dispatcher_switch[DISPE_TRINT_IPC].disp_in_request = handle_in_request;


	mutex_init(&pending_lock);
	mutex_lock(&pending_lock);
	encryption = CRYPT_DONT_ENCRYPT;
	for (i = 0; i < PENDING_MAX; i++) {
		pending_list[i].trid = 0;
	}
	mutex_unlock(&pending_lock);

	quit = FALSE;
	do {
		input("> ", "%1s", cmd);
		switch (cmd[0]) {
		case 'H':
		case 'h':
		case '?':
			printmenu();
			break;
		case 'M':
		case 'm':
			setmodule();
			break;
		case 'E':
		case 'e':
			setencryption();
			break;
		case 'C':
		case 'c':
			clientsend();
			break;
		case 'S':
		case 's':
			serversend();
			break;
		case 'L':
		case 'l':
			list();
			break;
		case 'Q':
		case 'q':
			quit = TRUE;
			break;
		default:
			FPRINTF((stdout, "Invalid command. Type ? for list.\n"));
			readln(stdin);
			break;
		}
	} 
	while (!quit);

	fprintf(stdout, "Terminated.\n");
	exit(0);
}
