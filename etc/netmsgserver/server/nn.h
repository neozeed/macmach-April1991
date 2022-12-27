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
 * $Log:	nn.h,v $
 * Revision 1.1  90/02/19  21:54:22  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/05/02  11:14:39  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 23-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nn.h
 *
 *
 * $ Header: nn.h,v 1.4 89/05/02 11:14:39 dpj Exp $
 *
 */

/*
 * External definitions for the network name service module.
 */


#ifndef	_NN_
#define	_NN_

#include <mach/boolean.h>

extern boolean_t netname_init();
/*
*/

extern void nn_remove_entries();
/*
port_t	port_id;
*/

#endif	_NN_
