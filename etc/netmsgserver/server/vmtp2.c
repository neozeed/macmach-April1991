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
 * $Log:	vmtp2.c,v $
 * Revision 1.1  90/02/19  21:58:10  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.8  89/05/02  11:19:33  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 25-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Renamed dummy variable to avoid a compiler warning.
 *
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 15-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified to avoid creating too many VMTP clients, and return
 *	TR_OVERLOAD instead.
 *
 * 14-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Revised for new transport interface. Modified the crypt_header
 *	handling to make it a trailer instead.
 *
 *  3-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created from vmtp1.c.
 */
/*
 * vmtp2.c
 *
 *
 */

#ifndef	lint
char vmtp2_rcsid[] = "$ Header: vmtp2.c,v 1.8 89/05/02 11:19:33 dpj Exp $";
#endif not lint

/*
 * VMTP transport module, interface type 2:
 *	all VMTP code in the kernel;
 *	one VMTP server entity per destination machine.
 *	request-response interface to the client modules.
 */


#include	<cthreads.h>

#include	"netmsg.h"
#include	"nm_defs.h"

#if	USE_VMTP
#include	"vmtp2_defs.h"

#include	"crypt.h"
#include	"disp_hdr.h"
#include	"ipc_rec.h"
#include	"ls_defs.h"
#include	"mem.h"
#include	"network.h"
#include	"sbuf.h"
#include	"transport.h"

#if	RPCMOD
#else	RPCMOD
 ! You lose !
#endif	RPCMOD


/*
 * Extra definitions for VMTP header.
 */
#define	vm_ipccode			vm_shortdata[0]
#define	vm_crypt_level			vm_shortdata[1]


/*
 * Standard well-known timestamp for server entities (sic!)
 */
#define	VMTP_SERVER_TIMESTAMP	0x123456


/*
 * List of available VMTP clients + lock.
 */
PRIVATE vmtp_rec_ptr_t	client_list;
PRIVATE struct mutex	client_lock;
#define	INIT_CLIENTS	4		/* number of preallocated clients */
#define	MAX_CLIENTS	10		/* maximum number of clients */
PRIVATE	int		num_clients;	/* current number of clients */
PRIVATE int		client_mask;	/* mask to select() on clients */
PRIVATE int		client_maxsock;	/* max socket number for select() */
PRIVATE	vmtp_rec_ptr_t	client_vec[32];

/*
 * Server socket
 */
PRIVATE int		server_sock;


extern int	errno;


/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_VMTPREC;


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
	MEM_ALLOCOBJ(rec_ptr,vmtp_rec_ptr_t,MEM_VMTPREC);		\
	if (rec_ptr == 0) panic("Cannot allocate a VMTP record");	\
	rec_ptr->socket = 0;						\
	rec_ptr->mcb.vm_code = VU_SDA | VU_WRA;				\
	rec_ptr->mcb.vm_segsize = VMTP_SEGSIZE;				\
	SBUF_SEG_INIT(rec_ptr->sbuf,&rec_ptr->sbuf_seg);		\
}


/*
 * Deallocate a VMTP record used for reception.
 */
#define	DEALLOC_VMTP_REC(rec_ptr) {			\
	MEM_DEALLOCOBJ(rec_ptr,MEM_VMTPREC);		\
}


/*
 * vmtp2_allocate_client --
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
PRIVATE vmtp_rec_ptr_t vmtp2_allocate_client()
BEGIN("vmtp2_allocate_client")
	vmtp_rec_ptr_t		rec_ptr;
	int			ret;
	struct vmtpeid		client;

	if (num_clients >= MAX_CLIENTS) {
		/*
		 * Just refuse to allocate more clients, and let the
		 * caller deal with the problem.
		 */
		RETURN(NULL);
	} else {
		num_clients++;
	}

	MEM_ALLOCOBJ(rec_ptr,vmtp_rec_ptr_t,MEM_VMTPREC);
	if (rec_ptr == 0)
		panic("vmtp2_allocate_client: cannot allocate space for a vmtp_rec");

	rec_ptr->socket = socket(AF_INET, SOCK_TRANSACT, IPPROTO_VMTP);
	if (rec_ptr->socket < 0) {
		ERROR((msg, "vmtp2_allocate_client: socket() returned %d errno=%d", rec_ptr->socket, errno))
		panic("vmtp2_allocate_client: cannot create a socket");
	}
	if (rec_ptr->socket > 31)
/* XXX */	panic("Cannot deal with sockets > 31");

	client.ve_inaddr = my_host_id;
	client.ve_fltm = 0xfffffff & (time(0) + rec_ptr->socket);

	ret = bind(rec_ptr->socket, &client, sizeof(struct vmtpeid));
	if (ret < 0) {
		ERROR((msg, "vmtp2_allocate_client: bind() returned %d errno=%d", ret, errno))
		panic("vmtp2_allocate_client: cannot bind a socket");
	}

	rec_ptr->mcb.vm_client = client;
	client_vec[rec_ptr->socket] = rec_ptr;
	DEBUG3(debug.vmtp,0,2210,rec_ptr->socket,client.ve_inaddr,
			client.ve_fltm);

	RETURN(rec_ptr);

END


/*
 * vmtp2_encrypt_segment
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
PRIVATE boolean_t vmtp2_encrypt_segment(rec_ptr, dest)
vmtp_rec_ptr_t	rec_ptr;
netaddr_t	dest;
BEGIN("vmtp2_encrypt_segment")
	unsigned long			bytes_to_pad;
	char				*pad_ptr;
	key_t				key;
	unsigned long			crypt_size;
	unsigned long			data_size;
	vmtp_crypt_trailer_ptr_t	ct_ptr;

	/*
 	 * Remember the real data size and calculate the padded data size.
	 */
	data_size = rec_ptr->mcb.vm_segsize;
	crypt_size = (data_size + sizeof(vmtp_crypt_trailer_t) + 7) & (~0x7);
	bytes_to_pad = crypt_size - data_size;
	rec_ptr->mcb.vm_segsize = crypt_size;

	/*
	 * Pad out the data with zero bytes.
	 */
	pad_ptr = (char *)&(rec_ptr->segment.data[0]);
	while (bytes_to_pad--) pad_ptr[crypt_size - bytes_to_pad] = 0;

	/*
	 * Prepare the vmtp_crypt_trailer.
	 */
	ct_ptr = (vmtp_crypt_trailer_ptr_t)
			&(pad_ptr[crypt_size - sizeof(vmtp_crypt_trailer_t)]);
	ct_ptr->ct_data_size = htons(data_size & 0xffff);

	/*
	 * Calculate the checksum.
	 */
	ct_ptr->ct_checksum = 0;
	ct_ptr->ct_checksum =
		udp_checksum((unsigned short *)&(rec_ptr->segment.data[0]),
								crypt_size);

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
			(key, (pointer_t)&(rec_ptr->segment.data[0]),crypt_size);
		RETURN(TRUE);
	}
	else {
		ERROR((msg, "vmtp2_encrypt_segment: illegal encryption algorithm (%d).",
				param.crypt_algorithm));
		RETURN(FALSE);
	}

END


/*
 * vmtp2_decrypt_segment
 *	decrypts a VMTP segment received over the network.
 *
 * Parameters:
 *	rec_ptr		: a pointer to the VMTP client record.
 *	len_ptr		: length of the VMTP segment.
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
PRIVATE boolean_t vmtp2_decrypt_segment(rec_ptr, len_ptr)
vmtp_rec_ptr_t	rec_ptr;
int		*len_ptr;
BEGIN("vmtp2_decrypt_segment")
	unsigned long			crypt_size;
	key_t				key;
	vmtp_crypt_trailer_ptr_t	ct_ptr;
	char				*data_ptr;

	crypt_size = *len_ptr;
	data_ptr = (char *)&(rec_ptr->segment.data[0]);
	ct_ptr = (vmtp_crypt_trailer_ptr_t)
			&(data_ptr[crypt_size - sizeof(vmtp_crypt_trailer_t)]);

	/*
	 * Look up the decryption key.
	 */
	if (!(km_get_dkey(rec_ptr->mcb.vm_server.ve_inaddr ,&key))) {
		ERROR((msg, "vmtp2_decrypt_segment.km_get_dkey fails, host id = %x.",
			rec_ptr->mcb.vm_server.ve_inaddr));
		RETURN(FALSE);
	}

	/*
	 * Now do the decryption.
	 */
	if (CHECK_DECRYPT_ALGORITHM(param.crypt_algorithm)) {
		(void)crypt_functions[param.crypt_algorithm].decrypt
			(key, (pointer_t)&(rec_ptr->segment.data[0]), crypt_size);
	}
	else {
		ERROR((msg, "vmtp2_decrypt_segment: illegal decryption algorithm (%d).",
				param.crypt_algorithm));
	RETURN(CRYPT_FAILURE);
	}

	/*
	 * Lastly, check the checksum.
	 * If all is well the checksum algorithm should return zero.
	 */

	if ((udp_checksum((unsigned short *)&(rec_ptr->segment.data[0]),
							crypt_size)) != 0)
	{
		ERROR((msg, "vmtp2_decrypt_segment.udp_checksum fails, host id = %x.",
			rec_ptr->mcb.vm_server.ve_inaddr));
		RETURN(FALSE);
	}

	*len_ptr = ntohs(ct_ptr->ct_data_size);
	RETURN(TRUE);

END


/*
 * vmtp2_sendrequest --
 *
 * Send a request through the vmtp2 interface.
 *
 * Parameters:
 *
 *	client_id	: an identifier assigned by the client to this transaction
 *	data		: the data to be sent
 *	to		: the destination of the request
 *	crypt_level	: whether the data should be encrypted
 *	reply_proc	: a function to be called to handle the response
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
EXPORT int vmtp2_sendrequest(client_id,data,to,crypt_level,reply_proc)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	to;
int		crypt_level;
int		(*reply_proc)();
BEGIN("vmtp2_sendrequest")
	register vmtp_rec_ptr_t rec_ptr;
	int			ret;

	/*
	 * Get hold of a client record.
	 */
	mutex_lock(&client_lock);
	if ((rec_ptr = client_list)) {
		client_list = rec_ptr->next;
	} else {
		rec_ptr = vmtp2_allocate_client();
		if (rec_ptr == NULL) {
			/*
			 * Tell the user to retry later.
			 */
			mutex_unlock(&client_lock);
			RETURN(TR_OVERLOAD);
		}
		client_mask |= 1 << rec_ptr->socket;
		if (client_maxsock < (rec_ptr->socket + 1))
			client_maxsock = rec_ptr->socket + 1;
	}
	mutex_unlock(&client_lock);

	/*
	 * Put the right data into the mcb.
	 */
	if (data) {
		SBUF_FLATTEN(data,rec_ptr->segment.data,rec_ptr->mcb.vm_segsize);
		if (rec_ptr->mcb.vm_segsize > VMTP_MAX_DATA) {
			ERROR((msg, "vmtp2_sendrequest: too much VMTP data (%d).",
							rec_ptr->mcb.vm_segsize));
			FREE_CLIENT(rec_ptr);
			RETURN(TR_SEND_FAILURE);
		}
		rec_ptr->mcb.vm_code = VU_SDA;
	} else {
		rec_ptr->mcb.vm_segsize = 0;
		rec_ptr->mcb.vm_code = 0;
	}

	rec_ptr->mcb.vm_ipccode = IPC_REQUEST;
	rec_ptr->mcb.vm_crypt_level = crypt_level;

	/*
	 * Remember the client info.
	 */
	rec_ptr->client_id = client_id;
	rec_ptr->reply_proc = reply_proc;

	/*
	 * See if we should encrypt the segment.
	 */
	if (crypt_level != CRYPT_DONT_ENCRYPT) {
		if (!(vmtp2_encrypt_segment(rec_ptr, to))) {
			ERROR((msg, "vmtp2_send: crypt_failure."));
			FREE_CLIENT(rec_ptr);
			RETURN(TR_CRYPT_FAILURE);
		}
		/*
		 * The vmtp_crypt_trailer is in the segment.
		 * The encryption routine must adjust segsize.
		 */
		rec_ptr->mcb.vm_code |= VU_SDA;
	}

	/*
	 * Make sure the destination is right, and send the message away!
	 */
	rec_ptr->mcb.vm_server.ve_inaddr = to;
	rec_ptr->mcb.vm_server.ve_fltm = VMTP_SERVER_TIMESTAMP;
	DEBUG5(debug.vmtp,0,2200,rec_ptr,rec_ptr->socket,rec_ptr->mcb.vm_segsize,
			rec_ptr->mcb.vm_client.ve_inaddr, rec_ptr->mcb.vm_client.ve_fltm);
	DEBUG1(debug.vmtp,0,2201,rec_ptr->mcb.vm_ipccode);
	ret = invoke(rec_ptr->socket, INVOKE_REQ, &rec_ptr->mcb, &rec_ptr->segment, VMTP_SEGSIZE);
	if (ret != 0) {
		ERROR((msg, "vmtp2_sendrequest: invoke(REQ) returned %d errno=%d",
									ret, errno));
		FREE_CLIENT(rec_ptr);
		RETURN(TR_FAILURE);
	} else {
		INCSTAT(vmtp_requests_sent);
		RETURN(TR_SUCCESS);
	}

END


/*
 * vmtp2_sendreply --
 *
 * Send a response through the vmtp2 interface.
 *
 * Parameters:
 *
 *	trid		: transport-level ID for a previous operation on this transaction
 *	code		: a return code to be passed to the client.
 *	data		: the data to be sent
 *	crypt_level	: whether the data should be encrypted
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
EXPORT int vmtp2_sendreply(trid,code,data,crypt_level)
int		trid;
int		code;
sbuf_ptr_t	data;
int		crypt_level;
BEGIN("vmtp2_sendreply")
	register vmtp_rec_ptr_t rec_ptr;
	int			ret;

	/*
	 * Get hold of the transport record.
	 */
    	rec_ptr = (vmtp_rec_ptr_t)trid;

	/*
	 * Put the right data into the mcb.
	 */
	if (data) {
		SBUF_FLATTEN(data,rec_ptr->segment.data,rec_ptr->mcb.vm_segsize);
		if (rec_ptr->mcb.vm_segsize > VMTP_MAX_DATA) {
			ERROR((msg, "vmtp2_sendreply: too much VMTP data (%d).",
							rec_ptr->mcb.vm_segsize));
			RETURN(TR_SEND_FAILURE);
		}
		rec_ptr->mcb.vm_code = VU_SDA;
	} else {
		rec_ptr->mcb.vm_segsize = 0;
		rec_ptr->mcb.vm_code = 0;
	}

	rec_ptr->mcb.vm_ipccode = code;
	rec_ptr->mcb.vm_crypt_level = crypt_level;

	/*
	 * See if we should encrypt the segment.
	 */
	if (crypt_level != CRYPT_DONT_ENCRYPT) {
		if (!(vmtp2_encrypt_segment(rec_ptr, 
					rec_ptr->mcb.vm_client.ve_inaddr))) {
			ERROR((msg, "vmtp2_send: crypt_failure."));
			RETURN(TR_CRYPT_FAILURE);
		}
		/*
		 * The vmtp_crypt_trailer is in the segment.
		 * The encryption routine must adjust segsize.
		 */
		rec_ptr->mcb.vm_code |= VU_SDA;
	}

	DEBUG5(debug.vmtp,0,2202,rec_ptr,rec_ptr->socket,rec_ptr->mcb.vm_segsize,
		rec_ptr->mcb.vm_client.ve_inaddr, rec_ptr->mcb.vm_client.ve_fltm);
	DEBUG1(debug.vmtp,0,2205,rec_ptr->mcb.vm_ipccode);

	/*
	 * Send the reply away!
	 */
	ret = sendreply(server_sock, &rec_ptr->mcb, &rec_ptr->segment);
	if (ret != 0) {
		ERROR((msg, "vmtp2_send: sendreply() returned %d errno=%d", ret, errno));
	} else {
		INCSTAT(vmtp_replies_sent);
	}
	DEALLOC_VMTP_REC(rec_ptr);

	RETURN(((ret == 0) ? TR_SUCCESS : TR_FAILURE));
END


/*
 * vmtp2_recvrp --
 *
 * Main loop to receive and process VMTP replies.
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
PRIVATE vmtp2_recvrp()
BEGIN("vmtp2_recvrp")
vmtp_rec_ptr_t	rec_ptr;
int		nsel;
int		mask;
int		cursock;
int		len;
struct vmtpmcb	*mcb_ptr;
int		crypt_level;
int		reply_code;
sbuf_ptr_t	sbuf_ptr;

/*
 * Loop forever waiting for messages.
 */
for (;;) {
	mask = client_mask;
	DEBUG1(debug.vmtp,0,2211,mask);
	nsel = select(client_maxsock, &mask, 0, 0, 0);
	DEBUG2(debug.vmtp,0,2203,nsel,mask);
	if (nsel < 0) {
		ERROR((msg, "vmtp2_recvrp: select() returned %d errno=%D", nsel, errno));
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
			DEBUG2(debug.vmtp,0,2204,cursock,len);
			DEBUG3(debug.vmtp & 0xf000,0,2212,mcb_ptr->vm_code,
				mcb_ptr->vm_ipccode,mcb_ptr->vm_segsize);
			if (len < 0) {
				ERROR((msg, "vmtp2_recvrp: invoke(RESP) returned %d errno=%d", len, errno));
				continue;
			}
			if (vmtp_ok(mcb_ptr->vm_code)) {
				INCSTAT(vmtp_replies_rcvd);
				DEBUG0(debug.vmtp,3,2213);
				reply_code = mcb_ptr->vm_ipccode;
				if (len > sizeof(vmtp_crypt_trailer_t)) {
					sbuf_ptr = &rec_ptr->sbuf;
					/*
					 * See whether we should decrypt the segment.
					 */
					crypt_level = mcb_ptr->vm_crypt_level;
					if (crypt_level != CRYPT_DONT_ENCRYPT) {
						if (!(vmtp2_decrypt_segment(rec_ptr, &len))) {
							ERROR((msg, "vmtp2_recvrp.vmtp2_decrypt_segment"));
							reply_code = TR_CRYPT_FAILURE;
							sbuf_ptr = NULL;
						}
					}
							
					SBUF_SEG_INIT(rec_ptr->sbuf,&rec_ptr->sbuf_seg);
					SBUF_APPEND(rec_ptr->sbuf,&rec_ptr->segment.data[0],len);
				} else {
					sbuf_ptr = NULL;
				}
			} else {
				ERROR((msg, "vmtp2_recvrp: vm_code = 0x%x", mcb_ptr->vm_code));
				reply_code = TR_FAILURE;
				sbuf_ptr = NULL;
			}
			(void) rec_ptr->reply_proc(rec_ptr->client_id,
								reply_code,sbuf_ptr);
			FREE_CLIENT(rec_ptr);
		}
		mask >>=  1;
		cursock++;
	}

	LOGCHECK;
}

/* NOTREACHED */

END



/*
 * vmtp2_recvrq --
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
PRIVATE void vmtp2_recvrq()
BEGIN("vmtp2_recvrq")
vmtp_rec_ptr_t	rec_ptr;
struct vmtpmcb	*mcb_ptr;
int		len;
int		disp_ret, tr_ret;
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
	DEBUG3(debug.vmtp,0,2206,len,mcb_ptr->vm_code,mcb_ptr->vm_ipccode);
	if (len <= 0) {
		if (len < 0) {
			ERROR((msg, "vmtp2_recvrq: recvreq() returned %d errno=%d",
								len, errno));
		}
		goto retry;
	}
#if	0
	if (!vmtp_ok(mcb_ptr->vm_code)) {
		ERROR((msg, "vmtp2_recvrq: got a request with vm_code=0x%x",
								mcb_ptr->vm_code));
		goto retry;
	}
#endif	0

	/*
	 * See whether we should decrypt the segment.
	 */
	crypt_level = mcb_ptr->vm_crypt_level;
	if (crypt_level != CRYPT_DONT_ENCRYPT) {
		if (!(vmtp2_decrypt_segment(rec_ptr, &len))) {
			ERROR((msg, "vmtp2_recvrq.vmtp2_decrypt_segment"));
			/*
			 * Send a crypt failure message.
			 */	
			tr_ret = vmtp2_sendreply((int)rec_ptr,TR_CRYPT_FAILURE,
						(sbuf_ptr_t)0,CRYPT_DONT_ENCRYPT);
			if (tr_ret != TR_SUCCESS) {
				ERROR((msg, 
		"vmtp2_recvrq: vmtp2_sendreply() returned %d errno=%d", 
								tr_ret, errno));
			}
			continue;
		}
	}

	/*
	 * Process the request and deliver it to the higher module.
	 */
	SBUF_APPEND(rec_ptr->sbuf,&rec_ptr->segment.data[0],len);
	INCSTAT(vmtp_requests_rcvd);
	disp_ret = disp_in_request(TR_VMTP2_ENTRY,(int)rec_ptr,
					&rec_ptr->sbuf, mcb_ptr->vm_client.ve_inaddr,
					crypt_level, FALSE);
	DEBUG1(debug.vmtp,0,2207,disp_ret);
	if (disp_ret != DISP_WILL_REPLY) {
		/*
		 * Send a PNH or PB message.
		 */
		DEBUG1(debug.vmtp,0,2208,disp_ret);
		tr_ret = vmtp2_sendreply((int)rec_ptr,disp_ret,(sbuf_ptr_t)0,
								CRYPT_DONT_ENCRYPT);
		if (tr_ret != TR_SUCCESS) {
			ERROR((msg, 
				"vmtp2_recvrq: vmtp2_sendreply() returned %d errno=%d", 
								tr_ret, errno));
		}
	}

	LOGCHECK;
}

/* NOTREACHED */
END


/*
 * vmtp2_init --
 *
 * Initialize the vmtp2 interface module.
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
EXPORT boolean_t vmtp2_init()
BEGIN("vmtp2_init")
	int			i;
	vmtp_rec_ptr_t		rec_ptr;
	int			ret;
	struct vmtpeid		server;
	cthread_t		new_thread;

	/*
	 * Initialize the memory management facilities.
	 */
	mem_initobj(&MEM_VMTPREC,"VMTP record",sizeof(vmtp_rec_t),TRUE,7,1);

	/*
	 * Allocate the server entity.
	 */
	server_sock = socket(AF_INET, SOCK_TRANSACT, IPPROTO_VMTP);
	if (server_sock < 0) {
		ERROR((msg, "vmtp2_init: socket() returned %d errno=%d", server_sock, errno))
		goto abort;
	}
	server.ve_inaddr = my_host_id;
	server.ve_fltm = VMTP_SERVER_TIMESTAMP;

	ret = bind(server_sock, &server, sizeof(struct vmtpeid));
	if (ret < 0) {
		ERROR((msg, "vmtp2_init: bind() returned %d errno=%d", ret, errno))
		goto abort;
	}
/*	rec_ptr->mcb.vm_server = server; */

	ret = initserver(server_sock, server, VS_MESGPORT);
	if (!vmtp_ok(ret)) {
		ERROR((msg,"vmtp2_init: initserver() failed: %d\n", ret));
		goto abort;
	}


	/*
	 * Create the list of clients.
	 */
	for (i = 0; i < 32; i++)
		client_vec[i] = NULL;
    	mutex_init(&client_lock);
	client_list = 0;
        client_mask = 0;
	client_maxsock = 0;
	num_clients = 0;
    	for (i = 0; i < INIT_CLIENTS; i++) {
		rec_ptr = vmtp2_allocate_client();
		rec_ptr->next = client_list;
		client_list = rec_ptr;
		client_mask |= 1 << rec_ptr->socket;
		if (client_maxsock < (rec_ptr->socket + 1))
			client_maxsock = rec_ptr->socket + 1;
	}

	/*
	 * Fire-up service threads.
	 */
	new_thread = cthread_fork(vmtp2_recvrq, 0);
	cthread_set_name(new_thread, "vmtp2_recvrq");
	cthread_detach(new_thread);

	new_thread = cthread_fork(vmtp2_recvrp, 0);
	cthread_set_name(new_thread, "vmtp2_recvrp");
	cthread_detach(new_thread);

	/*
	 * Make entry in transport table.
	 */
	transport_switch[TR_VMTP2_ENTRY].sendrequest = vmtp2_sendrequest;
	transport_switch[TR_VMTP2_ENTRY].sendreply = vmtp2_sendreply;

	RETURN(TRUE);

abort:
	RETURN(FALSE);

END

#else	USE_VMTP
	/*
	 * Just a dummy to keep the loader happy.
	 */
int	vmtp2_dummy;
#endif	USE_VMTP
