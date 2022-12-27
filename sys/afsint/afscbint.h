/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	afscbint.h,v $
 * Revision 2.3  89/06/03  15:34:21  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:10:31  jsb]
 * 
 * Revision 2.2  89/04/22  15:18:28  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
/* Machine generated file -- Do NOT edit */

#ifndef	_RXGEN_AFSCBINT_
#define	_RXGEN_AFSCBINT_

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


struct AFSFid {
	u_long Volume;
	u_long Vnode;
	u_long Unique;
};
typedef struct AFSFid AFSFid;
bool_t xdr_AFSFid();


struct AFSCallBack {
	u_long CallBackVersion;
	u_long ExpirationTime;
	u_long CallBackType;
};
typedef struct AFSCallBack AFSCallBack;
bool_t xdr_AFSCallBack();

#define EXCLUSIVE 1
#define SHARED 2
#define DROPPED 3
#define AFSNAMEMAX 256
#define AFSPATHMAX 1024
#define AFSOPAQUEMAX 1024

typedef struct AFSOpaque {
	u_int AFSOpaque_len;
	char *AFSOpaque_val;
} AFSOpaque;
bool_t xdr_AFSOpaque();

#define AFSCBMAX 50

typedef struct AFSCBFids {
	u_int AFSCBFids_len;
	AFSFid *AFSCBFids_val;
} AFSCBFids;
bool_t xdr_AFSCBFids();


typedef struct AFSCBs {
	u_int AFSCBs_len;
	AFSCallBack *AFSCBs_val;
} AFSCBs;
bool_t xdr_AFSCBs();


/* Opcode-related useful stats for package: RXAFSCB_ */
#define RXAFSCB_LOWEST_OPCODE   204
#define RXAFSCB_HIGHEST_OPCODE	206
#define RXAFSCB_NUMBER_OPCODES	3

#endif	_RXGEN_AFSCBINT_
