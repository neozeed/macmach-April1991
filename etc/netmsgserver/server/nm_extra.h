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
 * $Log:	nm_extra.h,v $
 * Revision 1.1  90/02/19  21:54:05  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.7  89/05/02  11:14:17  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Make netmsg_receive a macro.
 *
 * 15-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed external definition of panic.
 *
 * 15-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nm_extra.h
 *
 *
 * $ Header: nm_extra.h,v 1.7 89/05/02 11:14:17 dpj Exp $
 *
 */

/*
 * External definitions of functions provided by nm_extra.c.
 */


#ifndef	_NM_EXTRA_
#define	_NM_EXTRA_

#include "config.h"
#include "debug.h"

#if	LOCK_THREADS
#define netmsg_receive(msg_ptr) netmsg_receive_locked(msg_ptr)
#else	LOCK_THREADS
#define netmsg_receive(msg_ptr) msg_receive(msg_ptr, MSG_OPTION_NONE, 0)
#endif	LOCK_THREADS

extern void ipaddr_to_string();
/*
char		*output_string;
netaddr_t	input_address;
*/

#endif	_NM_EXTRA_
