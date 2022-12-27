/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rxgen_consts.h,v $
 * Revision 2.3  89/06/03  15:35:27  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:11:39  jsb]
 * 
 * Revision 2.2  89/04/22  15:19:04  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
#ifndef	_RXGEN_CONSTS_
#define	_RXGEN_CONSTS_

/* These are some rxgen-based (really arbitrary) error codes... */
#define	RXGEN_SUCCESS	    0
#define	RXGEN_CC_MARSHAL    -450
#define	RXGEN_CC_UNMARSHAL  -451
#define	RXGEN_SS_MARSHAL    -452
#define	RXGEN_SS_UNMARSHAL  -453
#define	RXGEN_DECODE	    -454
#define	RXGEN_OPCODE	    -455
#define	RXGEN_SS_XDRFREE    -456
#define	RXGEN_CC_XDRFREE    -457

/* These are some rxgen defines */
#define	RXGEN_MAKECALL_TIMEOUT	0
#define	RXGEN_WAIT		1
#define	RXGEN_DONTWAIT		0

#ifdef KERNEL
/* kernel's xdr.h doesn't normally define these things, but we need them */
extern bool_t	xdr_char();
extern bool_t	xdr_u_char();
#endif KERNEL
#endif	_RXGEN_CONSTS_
