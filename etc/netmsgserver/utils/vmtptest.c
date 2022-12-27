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
 * $Log:	vmtptest.c,v $
 * Revision 1.1  90/02/19  21:59:02  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/05/02  11:21:09  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 */

#define KERNEL_FEATURES	1

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netinet/vmtp_so.h>
#include	<netinet/vmtp.h>

#include	<stdio.h>
#include	<cthreads.h>
#include	<netdb.h>

#define	VMTP_SERVER_TIMESTAMP	0x123456

char		my_host_name[100];
long		my_host_id;
int		sock[2];
int		max_client;

sendreq() {
	int			to;
	int			i;
	int			ret;
	struct vmtpmcb		mcb;

	fprintf(stdout,"Enter server IP address: 0x");
	fflush(stdout);
	fscanf(stdin,"%x",&to);

	for (i = 0; i < 2; i++) {
		mcb.vm_code = 0;
		mcb.vm_segsize = 0;
		mcb.vm_server.ve_inaddr = to;
		mcb.vm_server.ve_fltm = VMTP_SERVER_TIMESTAMP;
		mcb.vm_shortdata[0] = i + 2;
		ret = invoke(sock[i], INVOKE_REQ, &mcb, 0, 0);
		fprintf(stdout,"invoke (%d,REQ) returned %d\n",i,ret);
		fflush(stdout);
		if (ret != 0) {
			exit(1);
		}
		sleep(10);
	}
}


getrep()
{
	int		nsel;
	int		mask;
	int		cursock;
	int		len;
	struct vmtpmcb	mcb;

	/*
	 * Loop forever waiting for messages.
	 */
	for (;;) {
		mask = (1 << sock[0]) | (1 << sock[1]);
		nsel = select(max_client, &mask, 0, 0, 0);
		fprintf(stdout,"select returned %d, mask=0x%x\n",nsel,mask);
		fflush(stdout);

		/*
		 * Loop over all ready sockets and process the messages.
		 */
		cursock = 0;
		while (mask) {
			if (mask & 0x1) {
				mcb.vm_code = 0;
				len = invoke(cursock, INVOKE_RESP, &mcb, 0, 0);
				fprintf(stdout,"invoke(%d,RESP) returned %d\n", 
									cursock,len);
				fprintf(stdout,"  vm_code=0x%x, data=%d\n",
						mcb.vm_code,mcb.vm_shortdata[0]);
				fflush(stdout);
			}
			mask >>=  1;
			cursock++;
		}
	}

	/* NOTREACHED */
}


client() 
{
	int			i;
	int			ret;
	struct vmtpeid		client;
	cthread_t		new_thread;
	int			ex;

	for (i = 0; i < 2; i++) {
		sock[i] = socket(AF_INET, SOCK_TRANSACT, IPPROTO_VMTP);
		if (sock[i] < 0) {
			fprintf(stdout,"socket returned %d\n",sock[i]);
			fflush(stdout);
			exit(1);
		}

		client.ve_inaddr = my_host_id;
		client.ve_fltm = 0xfffffff & (time(0) + sock[i]);

		ret = bind(sock[i], &client, sizeof(struct vmtpeid));
		fprintf(stdout,"bind (%d,0x%x) returned %d\n",
							sock[i],client.ve_fltm,ret);
		fflush(stdout);
		if (sock[i] < 0) {
			exit(1);
		}
	}
	max_client = sock[1] + 1;

	new_thread = cthread_fork(getrep, 0);
	cthread_set_name(new_thread, "getrep");
	cthread_detach(new_thread);

	sendreq();

	fprintf(stdout,"Hit Ctl-C to exit\n");
	fflush(stdout);
	fscanf(stdin,"%d",&ex);
	exit(0);
}


server()
{
	int			ret;
	struct vmtpeid		server;
	int			server_sock;
	struct vmtpmcb		mcb;
	int			len;
	struct vmtpeid		clients[10];
	int			last_client;
	int			i;

	/*
	 * Allocate the server entity.
	 */
	server_sock = socket(AF_INET, SOCK_TRANSACT, IPPROTO_VMTP);
	fprintf(stdout,"socket() returned %d\n", server_sock);
	fflush(stdout);
	if (server_sock < 0) {
		exit(1);
	}
	server.ve_inaddr = my_host_id;
	server.ve_fltm = VMTP_SERVER_TIMESTAMP;

	ret = bind(server_sock, &server, sizeof(struct vmtpeid));
	fprintf(stdout,"bind() returned %d\n", ret);
	fflush(stdout);
	if (ret < 0) {
		exit(1);
	}

	ret = initserver(server_sock, server, VS_MESGPORT);
	if (!vmtp_ok(ret)) {
		fprintf(stdout,"initserver() failed: %d\n", ret);
		exit(1);
	}

	last_client = 0;

	for (;;) {
		mcb.vm_server = server;
		mcb.vm_code = 0;
		mcb.vm_shortdata[0] = 1;
		len = recvreq(server_sock, &mcb, 0, 0);
		fprintf(stdout,"recvreq returned %d\n", len);
		fprintf(stdout,"  client=0x%x.0x%x\n",mcb.vm_client.ve_inaddr,
							mcb.vm_client.ve_fltm);
		fprintf(stdout,"  vm_code=0x%x, data=%d\n",mcb.vm_code,
							mcb.vm_shortdata[0]);
		fflush(stdout);
		clients[last_client] = mcb.vm_client;
		last_client++;

		if (mcb.vm_shortdata[0] == 3) {
/*			sleep(20); */
			for (i = 0; i < last_client; i++) {
				mcb.vm_code = 0;
				mcb.vm_shortdata[0] += 10;
				mcb.vm_client = clients[i];
/*				if (i == 1) continue; */
				fprintf(stdout,"Calling sendreply(%d)\n",
								mcb.vm_shortdata[0]);
				ret = sendreply(server_sock, &mcb, 0 , 0);
				fprintf(stdout,"sendreply 0x%x,0x%x returned %d\n",
					mcb.vm_client.ve_inaddr,mcb.vm_client.ve_fltm,
										ret);
				fflush(stdout);
			}
		}
	}

	/* NOTREACHED */
}


main(argc,argv)
	int			argc;
	char			*argv[];
{
	struct hostent		*hp;

	if ((gethostname(my_host_name, 100)) != 0) {
		fprintf(stdout,"gethostname fails\n");
		exit(1);
	}
	hp = gethostbyname(my_host_name);
	if (hp == 0) {
		fprintf(stdout,"gethostbyname fails\n");
		exit(1);
	} else {
		my_host_id = *(netaddr_t *)(hp->h_addr);
	}
	fprintf(stdout,"my_host_id=0x%x\n",my_host_id);
	fflush(stdout);

	if (argv[1][1] == 'd') 
		server();
	else
	    	client();
}
