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
 * $Log:	datagram.h,v $
 * Revision 1.1  90/02/19  21:50:08  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.6  89/05/02  11:07:39  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  4-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added datagram_max_data_size.  Removed datagram_abort.
 *
 *  3-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * datagram.h
 *
 *
 * $ Header: datagram.h,v 1.6 89/05/02 11:07:39 dpj Exp $
 *
 */

/*
 * Definitions for the datagram transport protocol.
 */


#ifndef	_DATAGRAM_
#define	_DATAGRAM_

#include <mach/boolean.h>
#include "transport.h"

/*
 * Datagram specific failure codes.
 */
#define DATAGRAM_ERROR_BASE		(-(TR_DATAGRAM_ENTRY * 16))
#define DATAGRAM_TOO_LARGE		(1 + DATAGRAM_ERROR_BASE)
#define DATAGRAM_SEND_FAILURE		(2 + DATAGRAM_ERROR_BASE)

/*
 * The maximum amount of data that can be placed in a datagram.
 */
extern int datagram_max_data_size;

/*
 * Exported functions.
 */
extern boolean_t datagram_init();
extern int datagram_send();

#endif	_DATAGRAM_
