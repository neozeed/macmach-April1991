/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rx_vab.h,v $
 * Revision 2.3  89/06/03  15:41:01  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:26:01  jsb]
 * 
 * Revision 2.2  89/04/22  15:29:49  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

/*
****************************************************************************
*        Copyright IBM Corporation 1988, 1989 - All Rights Reserved        *
*                                                                          *
* Permission to use, copy, modify, and distribute this software and its    *
* documentation for any purpose and without fee is hereby granted,         *
* provided that the above copyright notice appear in all copies and        *
* that both that copyright notice and this permission notice appear in     *
* supporting documentation, and that the name of IBM not be used in        *
* advertising or publicity pertaining to distribution of the software      *
* without specific, written prior permission.                              *
*                                                                          *
* IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL IBM *
* BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY      *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER  *
* IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING   *
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.    *
****************************************************************************
*/

#ifndef RX_SC_VAB_HEADER
#define RX_SC_VAB_HEADER

#ifdef	KERNEL
#include <rx/rx.h>
#else	KERNEL
#include "rx.h"
#endif	KERNEL

/* The vice authenticated bcrypt security object. */

extern struct rx_securityClass *rxvab_NewServerSecurityObject();
extern struct rx_securityClass *rxvab_NewClientSecurityObject();

struct rxvab_EncryptionKey {
    char data[8];
};

struct rxvab_Ticket {
    char MagicString[16]; /* Value should be equal to the magic string of the day. If
			     so, encrypted token is valid. */
    long AuthHandle;	    /* Handle, might come in useful for revocation in future */
    long Noise1;
    long ViceId;	    /* Whom is this token for? */
    long BeginTimestamp;    /* Seconds since Jan 1 1970 before which token is invalid */
    long Noise2;
    long EndTimestamp;	    /* Seconds since Jan 1 1970 after which token is invalid */
    long Noise3;
    struct rxvab_EncryptionKey HandShakeKey;   /* For use by server's GetKeys() routine */
    long Noise4;
};

/* private data in client-side security object */
struct rxvab_cprivate {
    int	type;				/* always 1, means client */
    struct rxvab_EncryptionKey key;	/* the session key */
    struct rxvab_Ticket	ticket;		/* the ticket for the server */
};

/* private data in server-side security object */
struct rxvab_sprivate {
    int	type;				/* always 2, means server */
    struct rxvab_EncryptionKey key;	/* the server key */
};

/* private data in server-side connection */
struct rxvab_conn {
    long challengeID;	    /* number we'll use for the challenge */
    long expirationTime;		/* when the ticket expires */
    struct rxvab_EncryptionKey key;	/* session key */
    long viceID;	    /* the vice id of the user */
    char tried;		    /* did we ever try to authenticate this connection */
    char worked;	    /* if tried, did we succeed */
};

#endif	RX_SC_VAB_HEADER
