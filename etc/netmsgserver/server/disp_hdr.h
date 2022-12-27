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
 * $Log:	disp_hdr.h,v $
 * Revision 1.1  90/02/19  21:50:48  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.15  89/05/02  11:08:36  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 18-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added dispatcher version number.
 *
 *  1-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Changed DISP_WILL_REPLY to 0, to have a more consistent
 *	transport interface.
 *
 *  1-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added DISP_IPC_ABORT. Fixed the DISP return codes.
 *
 * 18-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made fields in disp_hdr_t unsigned quantities.
 *
 *  6-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced REQUEST/REPLY entry pairs with single entries.
 *	Added DISP_PORTSEARCH, DISP_PS_AUTH and DISP_STARTUP.
 *
 *  2-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added DISP_PO_RO_HINT, DISP_PO_T_REQUEST, DISP_PO_T_REPLY,
 *	DISP_IPC_UNBLOCK, DISP_PO_DEATH and DISP_PO_RO_XFER..
 *
 * 27-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added DISP_NN_REQUEST and DISP_NN_REPLY for network name service.
 *	Added DISP_IGNORE - meant to be neither DISP_SUCCESS or DISP_FAILURE.
 *
 * 14-Dec-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Removed DISP_PORT_BLOCKED and DISP_PORT_NOT_HERE, which are not
 *	separate messages anymore.  Revamped dispatcher return codes:
 *	only DISP_FAILURE makes sense, otherwise the dispatcher returns
 *	a code from a higher-level module.
 *
 * 26-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added packet types for the checkups module.
 *
 * 25-Nov-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Added DISP_ACCEPT and DISP_REJECT.
 *	Moved src_format values to config.h from here.
 *
 * 15-Nov-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 * disp_hdr.h
 *
 *
 * $ Header: disp_hdr.h,v 1.15 89/05/02 11:08:36 dpj Exp $
 *
 */

/*
 * Definitions for the header common to all network server messages.
 */


#ifndef	_DISP_HDR_
#define	_DISP_HDR_

#include <mach/boolean.h>
#include "config.h"


typedef struct {
    unsigned short	disp_type;	/* type of message (network format) */
    unsigned short	src_format;	/* format for all following data */
} disp_hdr_t, *disp_hdr_ptr_t;


/*
 * Version number for messages.
 */
#define	DISPATCHER_VERSION	(100 * 1)


/*
 * Values for disp_type. DISPE_* represents the index of the
 * entry in dispatcher_switch.
 */
#define DISPE_IPC_MSG		1
#define DISPE_IPC_UNBLOCK	2
#define DISPE_PORTCHECK		3
#define DISPE_NETNAME		4
#define DISPE_PO_RO_HINT	5
#define DISPE_PO_RO_XFER	6
#define DISPE_PO_TOKEN		7
#define DISPE_PO_DEATH		8
#define DISPE_PORTSEARCH	9
#define DISPE_PS_AUTH		10
#define DISPE_STARTUP		11
#define	DISPE_IPC_ABORT		12

#define DISP_TYPE_MAX		(12 + 1)

#define DISP_IPC_MSG		(DISPE_IPC_MSG		+ DISPATCHER_VERSION)
#define DISP_IPC_UNBLOCK	(DISPE_IPC_UNBLOCK	+ DISPATCHER_VERSION)
#define DISP_PORTCHECK		(DISPE_PORTCHECK	+ DISPATCHER_VERSION)
#define DISP_NETNAME		(DISPE_NETNAME		+ DISPATCHER_VERSION)
#define DISP_PO_RO_HINT		(DISPE_PO_RO_HINT	+ DISPATCHER_VERSION)
#define DISP_PO_RO_XFER		(DISPE_PO_RO_XFER	+ DISPATCHER_VERSION)
#define DISP_PO_TOKEN		(DISPE_PO_TOKEN		+ DISPATCHER_VERSION)
#define DISP_PO_DEATH		(DISPE_PO_DEATH		+ DISPATCHER_VERSION)
#define DISP_PORTSEARCH		(DISPE_PORTSEARCH	+ DISPATCHER_VERSION)
#define DISP_PS_AUTH		(DISPE_PS_AUTH		+ DISPATCHER_VERSION)
#define DISP_STARTUP		(DISPE_STARTUP		+ DISPATCHER_VERSION)
#define	DISP_IPC_ABORT		(DISPE_IPC_ABORT	+ DISPATCHER_VERSION)

/*
 * Table for byte-swapping requirements 
 */
extern boolean_t disp_swap_table[DISP_FMT_MAX][DISP_FMT_MAX];

/*
 * Check if byte-swapping is needed 
 */
#define	DISP_CHECK_SWAP(sf) (disp_swap_table[sf][CONF_OWN_FORMAT])

/*
 * Dispatcher return codes. These codes are in the same
 * space as the TR_* and IPC_* codes.
 */
#define	DISP_WILL_REPLY	0
#define DISP_FAILURE	-1
#define DISP_SUCCESS	-2
#define DISP_IGNORE	-3

#endif	_DISP_HDR_
