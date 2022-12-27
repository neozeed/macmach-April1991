/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	printmsg.c,v $
 * Revision 1.1  90/02/19  21:47:53  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/07/04  17:37:27  mrt
 * 	Cleanup for Mach 2.5
 * 
 */

#include <stdio.h>
#include <mach.h>
#include <mach/message.h>

void PrintMsg(inp)
msg_header_t *inp;
{
	struct MyMessage {
		msg_header_t    head;
		int             data[3000];
	}              *p;

	int             i, datasize;

	p = (struct MyMessage *) inp;

	fprintf(stderr, "     msg_simple=%08x\n", p->head.msg_simple);
	fprintf(stderr, "       msg_size=%08x\n", p->head.msg_size);
	fprintf(stderr, "       msg_type=%08x\n", p->head.msg_type);
	fprintf(stderr, " msg_local_port=%08x\n", p->head.msg_local_port);
	fprintf(stderr, "msg_remote_port=%08x\n", p->head.msg_remote_port);
	fprintf(stderr, "         msg_id=%08x\n", p->head.msg_id);

	datasize = (p->head.msg_size - sizeof(p->head) + 3) / 4;
	for (i = 0; i < datasize; i += 4)
		fprintf(stderr, "data[%03x]   %08x   %08x   %08x   %08x\n", i,
		p->data[i], p->data[i + 1], p->data[i + 2], p->data[i + 3]);

	fflush(stderr);
}
