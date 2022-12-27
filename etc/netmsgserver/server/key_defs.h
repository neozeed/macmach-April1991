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
 * $Log:	key_defs.h,v $
 * Revision 1.1  90/02/19  21:52:00  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.7  89/05/02  11:10:31  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  5-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added macros to convert keys between network and host order.
 *
 * 12-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added KEY_IS_NULL.
 *
 *  2-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added KEY_EQUAL.
 *
 *  5-Nov-86  Robert Sansom (rds) at Carnegie-Mellon University
 *	Started.
 *
 */
/*
 * key_defs.h
 *
 *
 * $ Header: key_defs.h,v 1.7 89/05/02 11:10:31 dpj Exp $
 *
 */

/*
 * Definitions of encryption keys etc..
 */


#ifndef	_KEY_DEFS_
#define	_KEY_DEFS_

/*
 * An encrytion key.
 */
typedef union {
    unsigned char	key_bytes[16];
    unsigned long	key_longs[4];
} key_t, *key_ptr_t;

#define KEY_EQUAL(key1, key2) 					\
    ((key1.key_longs[0] == key2.key_longs[0])			\
	&& (key1.key_longs[1] == key2.key_longs[1])		\
	&& (key1.key_longs[2] == key2.key_longs[2])		\
	&& (key1.key_longs[3] == key2.key_longs[3]))

#define KEY_IS_NULL(key)					\
    (((key).key_longs[0] == 0) && ((key).key_longs[1] == 0)	\
	&& ((key).key_longs[2] == 0) && ((key).key_longs[3] == 0))


/*
 * Macros to convert keys between network and host byte order.
 */
#define NTOH_KEY(key) {							\
    (key).key_longs[0] = ntohl((key).key_longs[0]);			\
    (key).key_longs[1] = ntohl((key).key_longs[1]);			\
    (key).key_longs[2] = ntohl((key).key_longs[2]);			\
    (key).key_longs[3] = ntohl((key).key_longs[3]);			\
}

#define HTON_KEY(key) {							\
    (key).key_longs[0] = htonl((key).key_longs[0]);			\
    (key).key_longs[1] = htonl((key).key_longs[1]);			\
    (key).key_longs[2] = htonl((key).key_longs[2]);			\
    (key).key_longs[3] = htonl((key).key_longs[3]);			\
}

/*
 * Structure used to transmit or store a token or a key.
 */
typedef union {
    key_t	si_key;
    key_t	si_token;
} secure_info_t, *secure_info_ptr_t;

/*
 * Security Level of ports and messages.
 */
#define PORT_NOT_SECURE		0
#define MESSAGE_NOT_SECURE	0

#endif	_KEY_DEFS_
