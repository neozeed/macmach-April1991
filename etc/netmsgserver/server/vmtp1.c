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
 * $Log:	vmtp1.c,v $
 * Revision 1.1  90/02/19  21:58:02  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.14  89/05/02  11:19:19  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  6-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Bug fix to byte padding in vmtp1_encrypt_segment.
 *	Added encryption/decryption statistics.
 *	Removed some lint.  Surrounded code by #if USE_VMTP.
 *
 *  6-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new version of VMTP code, with initserver() call.
 *
 * 21-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added support for encrypted data.
 *	Added some minimal statistics gathering.
 *
 *  8-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 */
/*
 * vmtp1.c
 *
 *
 */

#ifndef	lint
char vmtp1_rcsid[] = "$ Header: vmtp1.c,v 1.14 89/05/02 11:19:19 dpj Exp $";
#endif not lint

/*
 * VMTP transport module, interface type 1:
 *	all VMTP code in the kernel;
 *	one VMTP server entity per destination machine.
 */



#include	"config.h"

#if	USE_VMTP

#include	<cthreads.h>
#include	<sys/time.h>

#include	"netmsg.h"
#include	"nm_defs.h"

#include	"vmtp1_defs.h"

#include	"crypt.h"
#include	"disp_hdr.h"
#include	"ipc_rec.h"
#include	"ls_defs.h"
#include	"mem.h"
#include	"network.h"
#include	"sbuf.h"
#include	"transport.h"

extern long time();


/*
 * Extra definitions for VMTP header.
 */
#define	vm_ipccode	vm_shortdata[0]


/*
 * Standard well-known timestamp for server entities (sic!)
 */
#define	VMTP_SERVER_TIMESTAMP	0x123456


/*
 * List of available VMTP clients + lock.
 */
PRIVATE vmtp_rec_ptr_t	client_list;
PRIVATE struct mutex	client_lock;
#define	NUM_CLIENTS	4		/* number of preallocated clients */
PRIVATE int		client_mask;	/* mask to select() on clients */
PRIVATE int		max_client;	/* max socket number for select() */
PRIVATE	vmtp_rec_ptr_t	client_vec[32];

/*
 * Server socket
 */
PRIVATE int		server_sock;


extern int	errno;


/*
 * ======================================================================
 * Macros.
 * ======================================================================
 */

/*
 * Free a client record.
 */
#define	FREE_CLIENT(rec_ptr) {		\
	mutex_lock(&client_lock);	\
	rec_ptr->next = client_list;	\
	client_list = rec_ptr;		\
	mutex_unlock(&client_lock);	\
}


/*
 * Allocate and initialize a VMTP record to use for reception.
 */
#define	ALLOC_VMTP_REC(rec_ptr) {					\
	rec_ptr = (vmtp_rec_ptr_t) mem_allocobj(MEM_VMTPREC);		\
	if (rec_ptr == 0) panic("Cannot allocate a VMTP record");	\
	rec_ptr->socket = 0;						\
	rec_ptr->mcb.vm_code = VU_SDA | VU_WRA;				\
	rec_ptr->mcb.vm_segsize = VMTP_SEGSIZE;				\
	SBUF_SEG_INIT(rec_ptr->sbuf,&rec_ptr->sbuf_seg);		\
}


/*
 * Deallocate a VMTP record used for reception.
 */
#define	DEALLOC_VMTP_REC(rec_ptr) {				\
	mem_deallocobj((pointer_t)rec_ptr,MEM_VMTPREC);		\
}


/*
 * vmtp1_allocate_client --
 *
 * Allocate a new VMTP client, and create the associated socket
 * and transport record.
 *
 * Parameters:
 *
 * Results:
 *
 * Pointer to a newly allocated client record.
 *
 * A pointer to the transport record.
 *
 * Side effects:
 *
 * Creates and binds a socket.
 *
 * Design:
 *
 * Note:
 *
 *
 */
PRIVATE vmtp_rec_ptr_t vmtp1_allocate_client()
BEGIN("vmtp1_allocate_client")
	vmtp_rec_ptr_t		rec_ptr;
	int			ret;
	struct vmtpeid		client;

	rec_ptr = (vmtp_rec_ptr_t) mem_allocobj(MEM_VMTPREC);
	if (rec_ptr == 0)
		panic("vmtp1_allocate_client: cannot allocate space for a vmtp_rec");

	rec_ptr->socket = socket(AF_INET, SOCK_TRANSACT, IPPROTO_VMTP);
	if (rec_ptr->socket < 0) {
		ERROR((msg, "vmtp1_allocate_client: socket() returned %d errno=%d", rec_ptr->socket, errno))
		panic("vmtp1_allocate_client: cannot create a socket");
	}
	if (rec_ptr->socket > 31)
/* XXX */	panic("Cannot deal with sockets > 31");

	client.ve_inaddr = my_host_id;
	client.ve_fltm = 0xfffffff & ((int)time((long *)0) + rec_ptr->socket);

	ret = bind(rec_ptr->socket, (struct sockaddr *)&client, sizeof(struct vmtpeid));
	if (ret < 0) {
		ERROR((msg, "vmtp1_allocate_client: bind() returned %d errno=%d", ret, errno))
		panic("vmtp1_allocate_client: cannot bind a socket");
	}

	rec_ptr->mcb.vm_client = client;
	client_vec[rec_ptr->socket] = rec_ptr;
	DEBUG3(debug.vmtp,0,2110,rec_ptr->socket,client.ve_inaddr,
			client.ve_fltm);

	RETURN(rec_ptr);

END


/*
 * vmtp1_encrypt_segment
 *	encrypts a VMTP segment to be sent over the network.
 *
 * Parameters:
 *	rec_ptr		: a pointer to the VMTP client record.
 *	dest		: destination of segment.
 *
 * Results:
 *	TRUE if the encryption was successful, FALSE otherwise.
 *
 * Design:
 *	Looks up the key for the destination host.
 *	Calculates the crypt checksum for the segment.
 *	Encrypts the segment.
 *
 * Note:
 *	the length of the segment data must be padded to a multiple of 8 bytes.
 *
 */
PRIVATE boolean_t vmtp1_encrypt_segment(rec_ptr, dest)
vmtp_rec_ptr_t	rec_ptr;
netaddr_t	dest;
BEGIN("vmtp1_encrypt_segment")
	int		bytes_to_pad;
	char		*pad_ptr;
	key_t		key;
	int		crypt_size;

	/*
 	 * Remember the real data size and calculate the padded data size.
	 */
	rec_ptr->segment.crypt_header.ch_data_size = htons((unsigned short)rec_ptr->mcb.vm_segsize);
	bytes_to_pad = crypt_size = rec_ptr->mcb.vm_segsize + (2 * sizeof(short));
	crypt_size = (crypt_size + 7) & (~(07));
	rec_ptr->mcb.vm_segsize = crypt_size - (2 * sizeof(short));
	bytes_to_pad = crypt_size - bytes_to_pad;

	/*
	 * Pad out the data with zero bytes.
	 */
	pad_ptr = (char *)&(rec_ptr->segment.crypt_header.ch_checksum);
	while (bytes_to_pad--) pad_ptr[crypt_size - bytes_to_pad] = 0;

	/*
	 * Calculate the checksum.
	 */
	rec_ptr->segment.crypt_header.ch_checksum = 0;
	rec_ptr->segment.crypt_header.ch_checksum =
		udp_checksum((unsigned short *)&(rec_ptr->segment.crypt_header.ch_crypt_level),
				(crypt_size + sizeof(long)));

	/*
	 * Look up the encryption key.
	 */
	if (!(km_get_key(dest ,&key))) {
		ERROR((msg, "vmpt1_encrypt_segment.km_get_key fails, host id = %x.", dest));
		RETURN(FALSE);
	}

	/*
	 * Now do the encryption.
	 */
	if (CHECK_ENCRYPT_ALGORITHM(param.crypt_algorithm)) {
		(void)crypt_functions[param.crypt_algorithm].encrypt
			(key, (pointer_t)&(rec_ptr->segment.crypt_header.ch_checksum), crypt_size);
		INCSTAT(vmtp_segs_encrypted);
		RETURN(TRUE);
	}
	else {
		ERROR((msg, "vmtp1_encrypt_segment: illegal encryption algorithm (%d).",
				param.crypt_algorithm));
		RETURN(FALSE);
	}

END


/*
 * vmtp1_decrypt_segment
 *	decrypts a VMTP segment received over the network.
 *
 * Parameters:
 *	rec_ptr		: a pointer to the VMTP client record.
 *
 * Returns:
 *	len_ptr		: set to the true length of the segment data.
 *
 * Results:
 *	TRUE if the decryption was successful, FALSE otherwise.
 *
 * Design:
 *	Looks up the key for the destination host.
 *	Decrypts the segment.
 *	Checks the crypt checksum for the segment.
 *
 */
PRIVATE boolean_t vmtp1_decrypt_segment(rec_ptr, len_ptr)
vmtp_rec_ptr_t	rec_ptr;
int		*len_ptr;
BEGIN("vmtp1_decrypt_segment")
	int		crypt_size;
	key_t		key;

	crypt_size = *len_ptr + (2 * sizeof(short));

	/*
	 * Look up the decryption key.
	 */
	if (!(km_get_dkey(rec_ptr->mcb.vm_server.ve_inaddr ,&key))) {
		ERROR((msg, "vmtp1_decrypt_segment.km_get_dkey fails, host id = %x.",
			rec_ptr->mcb.vm_server.ve_inaddr));
		RETURN(FALSE);
	}

	/*
	 * Now do the decryption.
	 */
	if (CHECK_DECRYPT_ALGORITHM(param.crypt_algorithm)) {
		(void)crypt_functions[param.crypt_algorithm].decrypt
			(key, (pointer_t)&(rec_ptr->segment.crypt_header.ch_checksum), crypt_size);
	}
	else {
		ERROR((msg, "vmtp1_decrypt_segment: illegal decryption algorithm (%d).",
				param.crypt_algorithm));
	RETURN(CRYPT_FAILURE);
	}

	/*
	 * Lastly, check the checksum.
	 * If all is well the checksum algorithm should return zero.
	 */

	if ((udp_checksum((unsigned short *)&(rec_ptr->segment.crypt_header.ch_crypt_level),
			(crypt_size + sizeof(long)))) != 0)
	{
		ERROR((msg, "vmtp1_decrypt_segment.udp_checksum fails, host id = %x.",
			rec_ptr->mcb.vm_server.ve_inaddr));
		RETURN(FALSE);
	}

	*len_ptr = ntohs(rec_ptr->segment.crypt_header.ch_data_size);
	INCSTAT(vmtp_segs_decrypted);
	RETURN(TRUE);

END


/*
 * vmtp1_send --
 *
 * Send a request or a response through the vmtp1 interface.
 *
 * Parameters:
 *
 *	client_id	: an identifier assigned by the client to this transaction
 *	data		: the data to be sent
 *	to		: the destination of the request
 *	trid		: transport-level ID for a previous operation on this transaction
 *	crypt_level	: whether the data should be encrypted
 *	cleanup		: a function to be called when this transaction has finished
 *
 * Results:
 *
 *	TR_SUCCESS or a specific failure code.
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT vmtp1_send(client_id,data,to,trid,crypt_level,cleanup)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	to;
int		trid;
unsigned long	crypt_level;
int		(*cleanup)();
BEGIN("vmtp1_send")
	register vmtp_rec_ptr_t rec_ptr;
	int			ret;

	/*
	 * Get hold of a client record.
	 */
	if (trid) {
	    	rec_ptr = (vmtp_rec_ptr_t)trid;
	} else {
		mutex_lock(&client_lock);
		if ((rec_ptr = client_list)) {
			client_list = rec_ptr->next;
		} else {
			rec_ptr = vmtp1_allocate_client();
			client_mask |= 1 << rec_ptr->socket;
			if (max_client < (rec_ptr->socket + 1))
				max_client = rec_ptr->socket + 1;
		}
		mutex_unlock(&client_lock);
	}

	/*
	 * Put the right data into the mcb.
	 */
	SBUF_FLATTEN(data,rec_ptr->segment.data,rec_ptr->mcb.vm_segsize);
	if (rec_ptr->mcb.vm_segsize > VMTP_MAX_DATA) {
		ERROR((msg, "vmtp1_send: too much VMTP data (%d).", rec_ptr->mcb.vm_segsize));
		RETURN(TR_SEND_FAILURE);
	}
	rec_ptr->mcb.vm_code = VU_SDA;
	rec_ptr->mcb.vm_ipccode = IPC_SUCCESS;

	/*
	 * Remember the client info.
	 */
	rec_ptr->client_id = client_id;
	rec_ptr->callback = cleanup;

	/*
	 * See if we should encrypt the segment.
	 */
	rec_ptr->segment.crypt_header.ch_crypt_level = htonl(crypt_level);
	if (crypt_level != CRYPT_DONT_ENCRYPT) {
		if (!(vmtp1_encrypt_segment(rec_ptr, to))) {
			ERROR((msg, "vmtp1_send: crypt_failure."));
			RETURN(TR_CRYPT_FAILURE);
		}
	}
	rec_ptr->mcb.vm_segsize += sizeof(crypt_header_t);

	DEBUG5(debug.vmtp,0,2100,rec_ptr,rec_ptr->socket,rec_ptr->mcb.vm_segsize,
			rec_ptr->mcb.vm_client.ve_inaddr, rec_ptr->mcb.vm_client.ve_fltm);
	/*
	 * Make sure the destination is right, and send the message away!
	 */
	if (trid) {
		DEBUG0(debug.vmtp,0,2101);
		ret = sendreply(server_sock, &rec_ptr->mcb, &rec_ptr->segment);
		if (ret != 0) {
			ERROR((msg, "vmtp1_send: sendreply() returned %d errno=%d", ret, errno));
		}
		else INCSTAT(vmtp_replies_sent);
		DEALLOC_VMTP_REC(rec_ptr);
/* XXX */	(*cleanup)(client_id, IPC_SUCCESS);
	} else {
		rec_ptr->mcb.vm_server.ve_inaddr = to;
		rec_ptr->mcb.vm_server.ve_fltm = VMTP_SERVER_TIMESTAMP;
		DEBUG2(debug.vmtp,0,2102,rec_ptr->mcb.vm_server.ve_inaddr,
		       	rec_ptr->mcb.vm_server.ve_fltm);
		ret = invoke(rec_ptr->socket, INVOKE_REQ, &rec_ptr->mcb, &rec_ptr->segment, VMTP_SEGSIZE);
		if (ret != 0) {
			ERROR((msg, "vmtp1_send: invoke(REQ) returned %d errno=%d", ret, errno));
		}
		else INCSTAT(vmtp_requests_sent);
	}

	RETURN(((ret == 0) ? TR_SUCCESS : TR_FAILURE));
END


/*
 * vmtp1_cleanup --
 *
 * Clean up the space used by a vmtp record after the message has been processed
 * by the higher levels.
 *
 * Parameters:
 *
 * rec_ptr: pointer to vmtp record.
 *
 * Results:
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 * Can be called synchronously from disp_indata.
 */
PUBLIC vmtp1_cleanup(rec_ptr)
vmtp_rec_ptr_t	rec_ptr;
BEGIN("vmtp1_cleanup")

DEBUG2(debug.vmtp,0,2109,rec_ptr,rec_ptr->socket);
	if (rec_ptr->socket) {
		FREE_CLIENT(rec_ptr);
/* Can never deallocate the record for a server here, because we need it
	for the response or the PNH/PB 
	} else {
		DEALLOC_VMTP_REC(rec_ptr);
*/
	}

RETURN(TR_SUCCESS);

END


/*
 * vmtp1_recvrp --
 *
 * Main loop to receive and process VMTP replies.
 *
 * Parameters:
 *
 * Results:
 *
 * Side effects:
 *
 * Calls the dispatcher module.
 *
 * Design:
 *
 * Note:
 *
 */
PRIVATE vmtp1_recvrp()
BEGIN("vmtp1_recvrp")
vmtp_rec_ptr_t	rec_ptr;
int		nsel;
int		mask;
int		cursock;
int		len;
struct vmtpmcb	*mcb_ptr;
int		ret;
int		crypt_level;

/*
 * Loop forever waiting for messages.
 */
for (;;) {
	mask = client_mask;
	DEBUG1(debug.vmtp,0,2111,mask);
	nsel = select(max_client, (fd_set *)&mask, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
	DEBUG2(debug.vmtp,0,2103,nsel,mask);
	if (nsel < 0) {
		ERROR((msg, "vmtp1_recvrp: select() returned %d errno=%D", nsel, errno));
		continue;
	}
	
	/*
	 * Loop over all ready sockets and process the messages.
	 */
	cursock = 0;
	while (mask) {
		if (mask & 0x1) {
			rec_ptr = client_vec[cursock];
			mcb_ptr = &rec_ptr->mcb;
			mcb_ptr->vm_code = VU_SDA | VU_WRA;
			len = invoke(cursock, INVOKE_RESP, mcb_ptr, &rec_ptr->segment, VMTP_SEGSIZE);
			DEBUG2(debug.vmtp,0,2104,cursock,len);
			DEBUG3(debug.vmtp & 0xf000,0,2112,mcb_ptr->vm_code,
				mcb_ptr->vm_ipccode,mcb_ptr->vm_segsize);
			if (len < 0) {
				ERROR((msg, "vmtp1_recvrp: invoke(RESP) returned %d errno=%d", len, errno));
				continue;
			}
			if (vmtp_ok(mcb_ptr->vm_code)) {
/*			if ((mcb_ptr->vm_code & 0x00ffffff) == VMTP_OK) {*/
				DEBUG0(debug.vmtp,3,2113);
				if (mcb_ptr->vm_ipccode == IPC_SUCCESS) {
					/*
					 * See whether we should decrypt the segment.
					 */
					len -= sizeof(crypt_header_t);
					crypt_level = ntohl(rec_ptr->segment.crypt_header.ch_crypt_level);
					if (crypt_level != CRYPT_DONT_ENCRYPT) {
						if (!(vmtp1_decrypt_segment(rec_ptr, &len))) {
							ERROR((msg, "vmtp1_recvrp.vmtp1_decrypt_segment"));
							(void)rec_ptr->callback(rec_ptr->client_id,
									TR_CRYPT_FAILURE);
							FREE_CLIENT(rec_ptr);
							continue;
						}
					}
							
					SBUF_SEG_INIT(rec_ptr->sbuf,&rec_ptr->sbuf_seg);
					SBUF_APPEND(rec_ptr->sbuf,&rec_ptr->segment.data[0],len);
					INCSTAT(vmtp_replies_rcvd);
					ret = disp_indata((int)rec_ptr,
						    &rec_ptr->sbuf,
						    mcb_ptr->vm_server.ve_inaddr,
						    vmtp1_cleanup,
						    TR_VMTP_ENTRY,
						    rec_ptr->client_id,
						    crypt_level,
						    FALSE);
					DEBUG1(debug.vmtp,0,2105,ret);
					if (ret != IPC_SUCCESS) {
						if ((ret == IPC_PORT_NOT_HERE) ||
						    (ret == IPC_PORT_BLOCKED)) {
							/*
							 * Send a PNH or PB message. XXX
							 */
						} else {
							ERROR((msg, "vmtp1_recvrp: disp_indata returned %d",
								ret));
						}
/* XXX record freed by cleanup in all cases	FREE_CLIENT(rec_ptr); */
					}
				} else {
					(*(int (*)())rec_ptr->callback)(rec_ptr->client_id,
								mcb_ptr->vm_ipccode);
					FREE_CLIENT(rec_ptr);
				}
			} else {
				ERROR((msg, "vmtp1_recvrp: vm_code = 0x%x", mcb_ptr->vm_code));
				(*(int (*)())rec_ptr->callback)(rec_ptr->client_id,
							TR_FAILURE);
				FREE_CLIENT(rec_ptr);
			}
		}
		mask >>=  1;
		cursock++;
	}

}

/* NOTREACHED */

END



/*
 * vmtp1_recvrq --
 *
 * Main loop to receive and process VMTP requests.
 *
 * Parameters:
 *
 * Results:
 *
 * Side effects:
 *
 * Calls the dispatcher module.
 *
 * Design:
 *
 * Note:
 *
 */
PRIVATE vmtp1_recvrq()
BEGIN("vmtp1_recvrq")
vmtp_rec_ptr_t	rec_ptr;
struct vmtpmcb	*mcb_ptr;
int		len;
int		ret;
int		crypt_level;

/*
 * Loop forever waiting for requests.
 */
for (;;) {
	/*
	 * Allocate a VMTP record for the request.
	 */
	ALLOC_VMTP_REC(rec_ptr);
	mcb_ptr = &rec_ptr->mcb;

	/*
	 * Receive a request.
	 */
retry:

	len = recvreq(server_sock, mcb_ptr, &rec_ptr->segment, VMTP_SEGSIZE);
	if (len < 0) {
		ERROR((msg, "vmtp1_recvrq: recvreq() returned %d errno=%d", len, errno));
		goto retry;
	}
#if	0
	if (!vmtp_ok(mcb_ptr->vm_code)) {
		ERROR((msg, "vmtp1_recvrq: got a request with vm_code=0x%x", mcb_ptr->vm_code));
		goto retry;
	}
#endif	0
	DEBUG1(debug.vmtp,0,2106,len);

	/*
	 * See whether we should decrypt the segment.
	 */
	len -= sizeof(crypt_header_t);
	crypt_level = ntohl(rec_ptr->segment.crypt_header.ch_crypt_level);
	if (crypt_level != CRYPT_DONT_ENCRYPT) {
		if (!(vmtp1_decrypt_segment(rec_ptr, &len))) {
			ERROR((msg, "vmtp1_recvrq.vmtp1_decrypt_segment"));
			/*
			 * Send a crypt failure message.
			 */
			mcb_ptr->vm_code = 0;
			mcb_ptr->vm_ipccode = TR_CRYPT_FAILURE;
			ret = sendreply(server_sock, mcb_ptr, (vmtp_segment_t *)0);
			if (ret != 0) {
				ERROR((msg, "vmtp1_recvrq: sendreply() returned %d errno=%d", ret, errno));
			}
			goto retry;
		}
	}

	/*
	 * Process the request and deliver it to the higher module.
	 */
	SBUF_APPEND(rec_ptr->sbuf,&rec_ptr->segment.data[0],len);
	INCSTAT(vmtp_requests_rcvd);
	ret = disp_indata((int)rec_ptr, &rec_ptr->sbuf, mcb_ptr->vm_client.ve_inaddr,
				vmtp1_cleanup, TR_VMTP_ENTRY, 0,
				crypt_level, FALSE);
	DEBUG1(debug.vmtp,0,2107,ret);
	if (ret != IPC_SUCCESS) {
		if ((ret == IPC_PORT_NOT_HERE) || (ret == IPC_PORT_BLOCKED)) {
			/*
			 * Send a PNH or PB message.
			 */
			mcb_ptr->vm_code = 0;
			mcb_ptr->vm_ipccode = ret;
			DEBUG1(debug.vmtp,0,2108,ret);
			ret = sendreply(server_sock, mcb_ptr, (vmtp_segment_t *)0);
			INCSTAT(vmtp_replies_sent);
			if (ret != 0) {
				ERROR((msg, "vmtp1_recvrq: sendreply() returned %d errno=%d", ret, errno));
			}
		} else {
			ERROR((msg, "vmtp1_recvrq: disp_indata() returned %d", ret));
		}
		goto retry;
	}
}

/* NOTREACHED */
END


/*
 * vmtp1_init --
 *
 * Initialize the vmtp1 interface module.
 *
 * Parameters:
 *
 * Results:
 *
 * TRUE if all is OK, FALSE otherwise.
 *
 * Side effects:
 *
 * Starts up two threads to receive requests and replies.
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT boolean_t vmtp1_init ()
BEGIN("vmtp1_init")
	int			i;
	vmtp_rec_ptr_t		rec_ptr;
	int			ret;
	struct vmtpeid		server;
	cthread_t		new_thread;

	/*
	 * Create the list of clients.
	 */
	for (i = 0; i < 32; i++)
		client_vec[i] = NULL;
    	mutex_init(&client_lock);
	client_list = 0;
        client_mask = 0;
	max_client = 0;
    	for (i = 0; i < NUM_CLIENTS; i++) {
		rec_ptr = vmtp1_allocate_client();
		rec_ptr->next = client_list;
		client_list = rec_ptr;
		client_mask |= 1 << rec_ptr->socket;
		if (max_client < (rec_ptr->socket + 1))
			max_client = rec_ptr->socket + 1;
	}

	/*
	 * Allocate the server entity.
	 */
	server_sock = socket(AF_INET, SOCK_TRANSACT, IPPROTO_VMTP);
	if (server_sock < 0) {
		ERROR((msg, "vmtp1_init: socket() returned %d errno=%d", server_sock, errno))
		panic("vmtp1_init: cannot create a socket");
	}
	server.ve_inaddr = my_host_id;
	server.ve_fltm = VMTP_SERVER_TIMESTAMP;

	ret = bind(server_sock, (struct sockaddr *)&server, sizeof(struct vmtpeid));
	if (ret < 0) {
		ERROR((msg, "vmtp1_init: bind() returned %d errno=%d", ret, errno))
		panic("vmtp1_init: cannot bind a socket");
	}
	rec_ptr->mcb.vm_server = server;

	ret = initserver(server_sock, server, VS_MESGPORT);
	if (!vmtp_ok(ret)) {
		ERROR((msg,"vmtp1_init: initserver() failed: %d\n", ret));
		panic("vmtp1_init: cannot initialize the server entity");
	}


	/*
	 * Fire-up service threads.
	 */
	new_thread = cthread_fork((cthread_fn_t)vmtp1_recvrq, (any_t)0);
	cthread_set_name(new_thread, "vmtp1_recvrq");
	cthread_detach(new_thread);

	new_thread = cthread_fork((cthread_fn_t)vmtp1_recvrp, (any_t)0);
	cthread_set_name(new_thread, "vmtp1_recvrp");
	cthread_detach(new_thread);

	/*
	 * Make entry in transport table.
	 */
	transport_switch[TR_VMTP_ENTRY].send = vmtp1_send;

	RETURN(TRUE);

END

#endif	USE_VMTP
