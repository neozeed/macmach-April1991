/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rxkad.h,v $
 * Revision 2.3  89/06/03  15:32:08  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  19:08:35  jsb]
 * 
 * Revision 2.2  89/04/22  15:17:19  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
/*
 * rxkad.h:
 * This file is automatically generated; please do not edit it.
 */
/* Including rxkad.p.h at beginning of rxkad.h file. */


/*
 * COPYRIGHT (C) IBM CORPORATION 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#ifndef RX_SC_KAD_HEADER
#define RX_SC_KAD_HEADER

#ifndef MAXKTCTICKETLEN
#ifdef KERNEL
#include "auth.h"
#else KERNEL
#include <afs/auth.h>
#endif KERNEL
#endif MAXKTCTICKETLEN

#include "fcrypt.h"

/* The Kerberos Authenticated DES security object. */

#ifndef NEVERDATE
#define NEVERDATE 0xffffffff
#endif

extern struct rx_securityClass *rxkad_NewServerSecurityObject();
extern struct rx_securityClass *rxkad_NewClientSecurityObject();

typedef char rxkad_type;
#define rxkad_client 1
#define rxkad_server 2

typedef char rxkad_level;
#define rxkad_clear 0			/* send packets in the clear */
#define rxkad_auth 1			/* send encrypted sequence numbers */
#define rxkad_crypt 2			/* encrypt packet data */

#define rxkad_GetLevel(conn) (((struct rxkad_sconn *)((conn)->securityData))->level)

/* private data in client-side security object */
struct rxkad_cprivate {
    rxkad_type	   type;		/* always client */
    rxkad_level	   level;		/* minimum security level of client */
    long	   kvno;		/* key version of ticket */
    long	   ticketLen;		/* length of ticket */
    fc_KeySchedule keysched;		/* the session key */
    fc_InitializationVector ivec;	/* initialization vector for block chaining */
    char	   ticket[MAXKTCTICKETLEN]; /* the ticket for the server */
};

/* Per connection client-side info */
struct rxkad_cconn {
    int dummy;
};

/* private data in server-side security object */
struct rxkad_sprivate {
    rxkad_type  type;			/* always server */
    rxkad_level level;			/* minimum security level of server */
    char       *get_key_rock;		/* rock for get_key function */
    int	      (*get_key)();		/* function of kvno and server key ptr */
    int	      (*user_ok)();		/* server function to call with new client name */
};

/* private data in server-side connection */
struct rxkad_sconn {
    rxkad_level	     level;		/* security level of connection */
    char	     tried;		/* did we ever try to authenticate this connection */
    char	     authenticated;	/* connection is good */
    unsigned long    expirationTime;	/* when the ticket expires */
    long	     challengeID;	/* unique challenge */
    fc_KeySchedule   keysched;		/* session key */
    fc_InitializationVector ivec;	/* initialization vector for block chaining */
    struct rxkad_serverinfo *rock;	/* info about client if saved */
};

struct rxkad_serverinfo {
    int  type;
    struct ktc_principal client;
};

/* A challenge packet looks like:
   4 bytes of challenge number which will be recv_sequence
   4 bytes of minimum security level
 */

#define rxkad_challengeSize (2*sizeof(long))

/* A challenge response packet looks like: 
   8 bytes of (challenge number + 1) and encryption level encrypted w/ session key
   4 byte key version number of ticket
   4 bytes of ticket length
   n bytes of ticket
 */

#define rxkad_responseSize (2*sizeof(long) + sizeof(long) /*kvno*/ + sizeof(long) /*ticketLen*/)

#if 0
(process-errors "RXKAD" (quote (
 (inconsistency  1  "Security module structure inconsistent")
 (packetshort	 2  "Packet too short for security challenge")
 (levelfail	 3  "Security level negotiation failed")
 (ticketlen	 4  "Ticket length too long or too short")
 (outofsequence	 5  "packet had bad sequence number")
 (noauth	 9  "caller not authorized")
 (badkey	 17 "illegal key: bad parity or weak")
 (badticket	 25 "security object was passed a bad ticket")
 (unknownkey	 26 "ticket contained unknown key version number")
)))
#endif

/* begin error table */
#if 0
#ifdef KERNEL
/* should really modify kernel build procedure to use packages.h */
#define	RXKADMINERROR	((8<<16)+1000)
#else KERNEL
#include <afs/packages.h>
#endif KERNEL
#define RXKADINCONSISTENCY (RXKADMINERROR+1)
#define RXKADPACKETSHORT (RXKADMINERROR+2)
#define RXKADLEVELFAIL (RXKADMINERROR+3)
#define RXKADTICKETLEN (RXKADMINERROR+4)
#define RXKADOUTOFSEQUENCE (RXKADMINERROR+5)
#define RXKADNOAUTH (RXKADMINERROR+9)
#define RXKADBADKEY (RXKADMINERROR+17)
#define RXKADBADTICKET (RXKADMINERROR+25)
#define RXKADUNKNOWNKEY (RXKADMINERROR+26)
#define RXKADMAXERROR (RXKADMINERROR+26)

#ifdef ERRORMESSAGES
static char *RXKADErrorMessages[RXKADMAXERROR-RXKADMINERROR+1] = {
0,
"Security module structure inconsistent",
"Packet too short for security challenge",
"Security level negotiation failed",
"Ticket length too long or too short",
"packet had bad sequence number",
0,
0,
0,
"caller not authorized",
0,
0,
0,
0,
0,
0,
0,
"illegal key: bad parity or weak",
0,
0,
0,
0,
0,
0,
0,
"security object was passed a bad ticket",
"ticket contained unknown key version number"};
#endif
#endif
/* end error table */

#endif	RX_SC_KAD_HEADER

/* End of prolog file rxkad.p.h. */

#define RXKADINCONSISTENCY                       (19270400L)
#define RXKADPACKETSHORT                         (19270401L)
#define RXKADLEVELFAIL                           (19270402L)
#define RXKADTICKETLEN                           (19270403L)
#define RXKADOUTOFSEQUENCE                       (19270404L)
#define RXKADNOAUTH                              (19270405L)
#define RXKADBADKEY                              (19270406L)
#define RXKADBADTICKET                           (19270407L)
#define RXKADUNKNOWNKEY                          (19270408L)
#define RXKADEXPIRED                             (19270409L)
#define RXKADSEALEDINCON                         (19270410L)
#define RXKADDATALEN                             (19270411L)
extern void initialize_rxk_error_table ();
#define ERROR_TABLE_BASE_rxk (19270400L)

/* for compatibility with older versions... */
#define init_rxk_err_tbl initialize_rxk_error_table
#define rxk_err_base ERROR_TABLE_BASE_rxk