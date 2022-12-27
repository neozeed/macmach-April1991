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
 * $Log:	portsearch.h,v $
 * Revision 1.1  90/02/19  21:55:54  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.6  89/05/02  11:16:40  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 18-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * portsearch.h
 *
 *
 * $ Header: portsearch.h,v 1.6 89/05/02 11:16:40 dpj Exp $
 *
 */

/*
 * External definitions for the port search module
 */


#ifndef	_PORTSEARCH_
#define	_PORTSEARCH_

#include <mach/boolean.h>


extern void ps_do_port_search();
/*
port_rec_ptr_t		port_rec_ptr;
boolean_t		new_information;
network_port_ptr_t	new_nport_ptr;
int			(*retry)();
*/

extern boolean_t ps_init();
/*
*/

#endif	_PORTSEARCH_
