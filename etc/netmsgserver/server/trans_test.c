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
 * $Log:	trans_test.c,v $
 * Revision 1.1  90/02/19  21:57:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.12  89/05/02  11:18:42  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 28-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	crypt_algorithm now obtained from the param record.
 *
 * 13-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added cthread_init() and log initialisation.
 *
 * 28-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * trans_test.c
 *
 *
 */

#ifndef	lint
char trans_test_rcsid[] = "$ Header: trans_test.c,v 1.12 89/05/02 11:18:42 dpj Exp $";
#endif not lint

/*
 * Tests for transport protocols.
 */


#include <mach.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/ioctl.h>

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

static char ipaddr_string[16];
extern char *gets();
extern void tty_wait();
extern int cthread_debug;

static boolean_t	deltat_accept = TRUE;
static boolean_t	datagram_success = TRUE;
static boolean_t	srr_request_success = TRUE;
static boolean_t	srr_response_success = TRUE;



/*
 * handle_deltat_indata
 *	Handle incoming data from the deltat transport protocol.
 *
 * Parameters:
 *	Lots.
 *
 * Results:
 *	DISP_ACCEPT or DISP_REJECT.
 *
 */
PUBLIC handle_deltat_indata(trid, data, from, tr_cleanup, trmod,
			client_id, crypt_level, broadcast)
int		trid;
sbuf_ptr_t	data;
netaddr_t	from;
int		(*tr_cleanup)();
int		trmod;
int		client_id;
int		crypt_level;
boolean_t	broadcast;
BEGIN("handle_deltat_indata")
    int			rc;
    disp_hdr_ptr_t	disp_hdr_ptr;

    ipaddr_to_string(ipaddr_string, from);
    printf("\nhandle_deltat_indata: trid = %d, client_id = %d, transport module = %d.\n",
		trid, client_id, trmod);
    printf("handle_deltat_indata: from = %s, broadcast = %d, crypt_level = %d.\n",
		ipaddr_string, broadcast, crypt_level);
    SBUF_GET_SEG(*data, disp_hdr_ptr, disp_hdr_ptr_t);
    printf("handle_deltat_indata: disp_type = %d, src_format = %d.\n",
		disp_hdr_ptr->disp_type, disp_hdr_ptr->src_format);
    sbuf_printf(stdout, data);
    /*
     * Call cleanup now.
     */
    if ((rc = tr_cleanup(trid)) != TR_SUCCESS) {
	printf("handle_deltat_indata.cleanup fails, rc = %d.\n", rc);
    }
    if (deltat_accept) {
	printf("handle_deltat_indata: returning IPC_SUCCESS (%d).\n", IPC_SUCCESS);
	RETURN(IPC_SUCCESS);
    }
    else {
	printf("handle_deltat_indata: returning IPC_FAILURE (%d).\n", IPC_FAILURE);
	RETURN(IPC_FAILURE);
    }

END



/*
 * handle_datagram_indata_simple
 *	Handle incoming data from the datagram transport protocol.
 *
 * Parameters:
 *	Lots.
 *
 * Results:
 *	DISP_SUCCESS or DISP_FAILURE
 *
 */
PUBLIC handle_datagram_indata_simple(client_id, data, from, broadcast, crypt_level)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("handle_datagram_indata_simple")
    disp_hdr_ptr_t	disp_hdr_ptr;

    ipaddr_to_string(ipaddr_string, from);
    printf("\nhandle_datagram_indata_simple: client_id = %d, from = %s, broadcast = %d, crypt_level = %d.\n",
		client_id, ipaddr_string, broadcast, crypt_level);
    SBUF_GET_SEG(*data, disp_hdr_ptr, disp_hdr_ptr_t);
    printf("handle_datagram_indata_simple: disp_type = %d, src_format = %d.\n",
		disp_hdr_ptr->disp_type, disp_hdr_ptr->src_format);
    sbuf_printf(stdout, data);
    if (datagram_success) {
	printf("handle_datagram_indata_simple: returning DISP_SUCCESS (%d).\n", DISP_SUCCESS);
	RETURN(DISP_SUCCESS);
    }
    else {
	printf("handle_datagram_indata_simple: returning DISP_FAILURE (%d).\n", DISP_FAILURE);
	RETURN(DISP_FAILURE);
    }

END



/*
 * handle_indata_simple
 *	Handle an incoming reply from the srr transport protocol.
 *
 * Parameters:
 *	Lots.
 *
 * Results:
 *	DISP_SUCCESS or DISP_FAILURE
 *
 */
PUBLIC handle_srr_indata_simple(client_id, data, from, broadcast, crypt_level)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("handle_srr_indata_simple")
    disp_hdr_ptr_t	disp_hdr_ptr;
    int			data_size;
    kern_return_t	kr;
    sbuf_ptr_t		sbuf_ptr;

    ipaddr_to_string(ipaddr_string, from);
    printf("\nhandle_srr_indata_simple: client_id = %d, from = %s, broadcast = %d, crypt_level = %d.\n",
		client_id, ipaddr_string, broadcast, crypt_level);
    SBUF_GET_SEG(*data, disp_hdr_ptr, disp_hdr_ptr_t);
    printf("handle_srr_indata: disp_type = %d, src_format = %d.\n",
		disp_hdr_ptr->disp_type, disp_hdr_ptr->src_format);
    sbuf_printf(stdout, data);

    if (srr_response_success) {
	sbuf_ptr = (sbuf_ptr_t)client_id;
	SBUF_GET_SIZE(*sbuf_ptr, data_size);
	SBUF_GET_SEG(*sbuf_ptr, disp_hdr_ptr, disp_hdr_ptr_t);
	printf("handle_srr_indata_simple: deallocating %d bytes at address %d.\n",
			data_size, (int)disp_hdr_ptr);
	if ((kr = vm_deallocate(task_self(), (pointer_t)disp_hdr_ptr, data_size)) != KERN_SUCCESS) {
	    printf("handle_srr_indata_simple.vm_deallocate fails, kr = %1d.\n", kr);
	}
	printf("handle_srr_indata_simple: returning DISP_SUCCESS (%d).\n", DISP_SUCCESS);
	RETURN(DISP_SUCCESS);
    }
    else {
	printf("handle_srr_indata_simple: returning DISP_FAILURE (%d).\n", DISP_FAILURE);
	RETURN(DISP_FAILURE);
    }

END



/*
 * handle_srr_rr_simple
 *	Handle an incoming request from the datagram transport protocol.
 *	Returns a "response" in the input data buffer.
 *
 * Parameters:
 *	Lots.
 *
 * Results:
 *	DISP_SUCCESS or DISP_FAILURE
 *
 */
PUBLIC handle_srr_rr_simple(data, from, broadcast, crypt_level)
sbuf_ptr_t	data;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
BEGIN("handle_srr_rr_simple")
    disp_hdr_ptr_t	disp_hdr_ptr;
    int			data_size;

    ipaddr_to_string(ipaddr_string, from);
    printf("\nhandle_srr_rr_simple: from = %s, broadcast = %d, crypt_level = %d.\n",
		ipaddr_string, broadcast, crypt_level);
    SBUF_GET_SEG(*data, disp_hdr_ptr, disp_hdr_ptr_t);
    SBUF_GET_SIZE(*data,data_size);
    printf("handle_srr_rr_simple: disp_type = %d, src_format = %d.\n",
		disp_hdr_ptr->disp_type, disp_hdr_ptr->src_format);
    sbuf_printf(stdout, data);

    if (srr_request_success) {
	/*
	 * Make the "response" slightly different from the request.
	 */
	if (data_size > sizeof(disp_hdr_t)) data_size --;
	disp_hdr_ptr->src_format = conf_own_format;;
	SBUF_REINIT(*data);
	SBUF_APPEND(*data, disp_hdr_ptr, data_size);
	printf("handle_srr_rr_simple: returning %d bytes of 'response'.\n", data_size);
	printf("handle_srr_rr_simple: returning DISP_SUCCESS (%d).\n", DISP_SUCCESS);
	RETURN(DISP_SUCCESS);
    }
    else {
	printf("handle_srr_rr_simple: returning DISP_FAILURE (%d).\n", DISP_FAILURE);
	RETURN(DISP_FAILURE);
    }

END



/*
 * host_address
 *	Return a host address for a given host name.
 *
 */
PRIVATE long host_address(host_name)
char *host_name;
{
    register struct hostent *hp;
    register netaddr_t host_id;
    if ((host_id = inet_addr(host_name)) != - 1) return host_id;
    else if ((hp = gethostbyname(host_name)) == 0) return 0;
    else return *(long *)(hp->h_addr);
}



/*
 * tr_error_to_string
 *
 * Parameters:
 *	tr_error	: the error to translate
 *
 * Results:
 *	pointer to a string giving a printable representation of the error
 *
 */
PRIVATE char *tr_error_to_string(tr_error)
int	tr_error;
{
    switch (tr_error) {
	case TR_SUCCESS: return("TR_SUCCESS");
	case TR_FAILURE: return("TR_FAILURE");
	case TR_REMOTE_ACCEPT: return("TR_REMOTE_ACCEPT");
	case TR_REMOTE_REJECT: return("TR_REMOTE_REJECT");
	case TR_CRYPT_FAILURE: return("TR_CRYPT_FAILURE");
	case TR_SEND_FAILURE: return("TR_SEND_FAILURE");
	default: return("Unknown Error");
    }

}

/*
 * tt_cleanup
 *	cleanup function for the transport tests
 *
 * Parameters:
 *	client_id	: the identifier assigned to this transport transaction
 *	completion_code	: the result of the transaction
 *	
 *
 */
PUBLIC void tt_cleanup(client_id, completion_code)
int	client_id;
int	completion_code;
BEGIN("tt_cleanup")
    int			num_bytes;
    pointer_t		data_ptr;
    kern_return_t	kr;
    sbuf_ptr_t		sbuf_ptr;

    printf("tt_cleanup: client_id = %d, completion_code = %d (%s).\n", client_id, completion_code,
				tr_error_to_string(completion_code));
    sbuf_ptr = (sbuf_ptr_t)client_id;
    SBUF_GET_SEG(*sbuf_ptr, data_ptr, pointer_t);
    SBUF_GET_SIZE(*sbuf_ptr, num_bytes);
    printf("tt_cleanup: deallocating %d bytes at address %d.\n", num_bytes, (int)data_ptr);
    if ((kr = vm_deallocate(task_self(), data_ptr, num_bytes)) != KERN_SUCCESS) {
	printf("tt_cleanup.vm_deallocate fails, kr = %1d.\n", kr);
    }
    RET;

END



/*
 * main
 *	Accepts requests from the user to send data using a particular transport protocol.
 *
 */

#define USAGE	"Usage: ttest [-c] [-t] [-p #]"

main(argc, argv)
int	argc;
char	**argv;
{
    char		input_line[80], host_name[32];
    int			transport_protocol, num_bytes, trans_id, crypt_level, i;
    sbuf_t		sbuf;
    sbuf_seg_t		sbuf_seg;
    pointer_t		data_ptr;
    netaddr_t		host_id;
    kern_return_t	kr;
    disp_hdr_ptr_t	disp_hdr_ptr;

    cthread_init();

    for (i = 1; i < argc; i++) {
	if (strcmp(argv[i], "-t") == 0) debug.tracing = 1;
	else if (strcmp(argv[i], "-c") == 0) cthread_debug = 1;
	else if ((strcmp(argv[i], "-p") == 0) && ((i + 1) < argc)) {
	    i ++;
	    debug.print_level = atoi(argv[i]);
	}
	else {
	    fprintf(stderr, "%s\n", USAGE);
	    (void)fflush(stderr);
	    _exit(-1);
	}
    }

    fprintf(stdout, "%s: %s %s print_level = %d.\n", argv[0],
			(debug.tracing ? "tracing" : ""),
			(cthread_debug ? "cthread_debug" : ""),
			debug.print_level);
    (void)fflush(stdout);

    /*
     * Initialise various modules.
     */
    if (!(ls_init_1())) panic("ls_init_1 failed.");
    if (!(mem_init())) panic("mem_init failed.");
    if (!(disp_init())) panic("disp_init failed.");
    if (!(network_init())) panic("network_init failed.");
    if (!(timer_init())) panic("timer_init failed.");
    if (!(uid_init())) panic("uid_init failed.");

    /*
     * Initialise the transport protocols about which we know.
     */
    if (!(deltat_init())) panic("deltat_init failed.");
    if (!(datagram_init())) panic("datagram_init failed.");
    if (!(srr_init())) panic("srr_init failed.");

    /*
     * And a couple of higher-level modules.
     */
    if (!(netname_init())) panic("netname_init fails.");
    if (!(km_init())) panic("km_init failed.");
/*
    if (!(ls_init_2())) panic ("ls_init_2 failed.");
*/

    /*
     * Initialise the dispatcher switch.
     */
    dispatcher_switch[0].disp_indata = handle_deltat_indata;
    dispatcher_switch[2].disp_indata_simple = handle_datagram_indata_simple;
    dispatcher_switch[3].disp_rr_simple = handle_srr_rr_simple;
    dispatcher_switch[3].disp_indata_simple = handle_srr_indata_simple;

    while (TRUE) {
	printf("\n");

	printf("Which transport protocol (0 = delta-t, 1 = VMTP, 2 = datagram & 3 = SRR)? \n");
	(void)fflush(stdout);
	tty_wait(input_line);
	(void)sscanf(input_line, "%d", &transport_protocol);
	if ((transport_protocol < TR_DELTAT_ENTRY) || (transport_protocol > TR_SRR_ENTRY)) {
	    fprintf(stderr, "Illegal transport protocol (%d).\n", transport_protocol);
	    continue;
	}

	if (!(transport_switch[transport_protocol].send)) {
	    fprintf(stderr, "Send function not implented for transport protocol %d.\n", transport_protocol);
	    continue;
	}

	printf("How many bytes of data do you want to send? \n");
	tty_wait(input_line);
	(void)sscanf(input_line, "%d", &num_bytes);
	if (num_bytes < 0) {
	    fprintf(stderr, "Illegal number of bytes (%d).\n", num_bytes);
	    continue;
	}
	SBUF_SEG_INIT(sbuf, &sbuf_seg);
	data_ptr = 0;
	num_bytes += sizeof(disp_hdr_t);
	if ((kr = vm_allocate(task_self(), &data_ptr, num_bytes, TRUE)) != KERN_SUCCESS) {
	    fprintf(stderr, "vm_allocate fails, kr = %d.\n", kr);
	}
	disp_hdr_ptr = (disp_hdr_ptr_t)data_ptr;
	disp_hdr_ptr->disp_type = htons(transport_protocol);
	disp_hdr_ptr->src_format = conf_own_format;
	SBUF_APPEND(sbuf, data_ptr, num_bytes);	

	printf("To which host do you want to send the data? \n");
	tty_wait(input_line);
	(void)sscanf(input_line, "%s", host_name);
	if ((host_id = host_address(host_name)) == 0) {
	    fprintf(stderr, "Host '%s' not found.\n", host_name);
	    continue;
	}
	else {
	    ipaddr_to_string(ipaddr_string, host_id);
	    fprintf(stderr, "host_id = %s.\n", ipaddr_string);
	}

	printf("Send data encrypted? [N]\n");
	tty_wait(input_line);
	if ((input_line[0] == '\0') || (input_line[0] == 'n') || (input_line[0] == 'N')) {
	    crypt_level = CRYPT_DONT_ENCRYPT;
	    printf("Sending data unencrypted.\n");
	}
	else {
	    crypt_level = CRYPT_ENCRYPT;
	    printf("Sending data encrypted: current crypt algorithm = %d.\n", param.crypt_algorithm);
	}

	/*
	 * Now try actually making the send.
	 */
	trans_id = transport_switch[transport_protocol].send((int)&sbuf, &sbuf, host_id,
					TRSERV_NORMAL, crypt_level, tt_cleanup);
	if (trans_id == TR_SUCCESS) {
	    fprintf(stderr, "Transport send succeeds.\n");
	}
	else if (trans_id == TR_CRYPT_FAILURE) {
	    km_do_key_exchange(0, (int(*)())0, host_id);
	}
	else {
	    fprintf(stderr, "Transport send fails, error = %d.\n", trans_id);
	}
    }
}



/*
 * tty_wait
 *	Do a non-blocking wait on tty input.
 *
 */
PRIVATE void tty_wait(input_string)
char	*input_string;
{
    int		rc, n, i;

    while (TRUE) {
	rc = ioctl(0, (int)FIONREAD, (char *)(&n));
	if (rc != 0)
	    fprintf(stderr, "tty_wait.ioctl fails, rc = %d.\n", rc);
	if (n == 0) {
	    cthread_yield();
	    continue;
	}
	for (i = 0; i < n; i++) input_string[i] = getchar();
	input_string[n-1] = '\0';
	return;
    }

}
