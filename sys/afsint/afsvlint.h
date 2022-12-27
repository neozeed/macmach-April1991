/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	afsvlint.h,v $
 * Revision 2.3  89/06/03  15:34:54  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:11:06  jsb]
 * 
 * Revision 2.2  89/04/22  15:18:45  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
/* Machine generated file -- Do NOT edit */

#ifndef	_RXGEN_AFSVLINT_
#define	_RXGEN_AFSVLINT_

#ifdef	KERNEL
/* The following 'ifndefs' are not a good solution to the vendor's omission of surrounding all system includes with 'ifndef's since it requires that this file is included after the system includes...*/
#include <sys/types.h>
#ifndef	SOCK_DGRAM  /* XXXXX */
#include <sys/socket.h>
#endif
#ifndef	DTYPE_SOCKET  /* XXXXX */
#include <sys/file.h>
#endif
#ifndef	S_IFMT  /* XXXXX */
#include <sys/stat.h>
#endif
#ifndef	IPPROTO_UDP /* XXXXX */
#include <netinet/in.h>
#endif
#ifndef	DST_USA  /* XXXXX */
#include <sys/time.h>
#endif
#include <rpc/types.h>
#ifndef	XDR_GETLONG /* XXXXX */
#include <rpc/xdr.h>
#endif
#include <afsint/rxgen_consts.h>
#include <afs/osi.h>
#include <afsint/rx.h>
#else	KERNEL
#include <sys/types.h>
#include <rx/xdr.h>
#include <rx/rx.h>
#include <afs/rxgen_consts.h>
#endif	KERNEL

#define MAXNAMELEN 65
#define MAXNSERVERS 8
#define MAXTYPES 3
#define	VLF_RWEXISTS	    0x1000  /* flags for whole vldb entry */
#define	VLF_ROEXISTS	    0x2000
#define	VLF_BACKEXISTS	    0x4000
#define	VLSF_NEWREPSITE	    0x01    /* flags for indiv. server entry */
#define	VLSF_ROVOL	    0x02
#define	VLSF_RWVOL	    0x04
#define	VLSF_BACKVOL	    0x08

struct vldbentry {
	char name[MAXNAMELEN];
	long volumeType;
	long nServers;
	long serverNumber[MAXNSERVERS];
	long serverPartition[MAXNSERVERS];
	long serverFlags[MAXNSERVERS];
	u_long volumeId[MAXTYPES];
	long cloneId;
	long flags;
};
typedef struct vldbentry vldbentry;
bool_t xdr_vldbentry();


/* Opcode-related useful stats for package: VL_ */
#define VL_LOWEST_OPCODE   503
#define VL_HIGHEST_OPCODE	504
#define VL_NUMBER_OPCODES	2

#endif	_RXGEN_AFSVLINT_
