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
 * $Log:	nm_defs.h,v $
 * Revision 1.1  90/02/19  21:53:53  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.12  89/05/02  11:14:01  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.11  89/04/24  20:41:01  dpj
 * 	Added declaration for conf_own_format, moved here from config.h.
 * 	[89/04/19  23:26:48  dpj]
 * 
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:55:35  dpj]
 * 
 * Revision 1.10  88/10/08  22:28:34  dpj
 * 	Replace sys/mach_ipc_netport.h with mach/netport.h. Sigh.
 * 
 * 24-Aug-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Replace sys/mach_ipc_netport.h with mach/netport.h. Sigh.
 *
 * 24-May-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Replace mach_ipc_vmtp.h with mach_ipc_netport.h.
 *
 *  4-Sep-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed for new kernel include files which declare a lot
 *	of network server stuff internally, because of the NETPORT
 *	option.
 *
 *  5-Nov-86  Robert Sansom (rds) at Carnegie-Mellon University
 *	Started.
 *
 */
/*
 * nm_defs.h
 *
 *
 * $ Header: nm_defs.h,v 1.12 89/05/02 11:14:01 dpj Exp $
 *
 */

/*
 * Random definitions for the network service that everyone needs!
 */


#ifndef	_NM_DEFS_
#define	_NM_DEFS_

/*
 * netaddr_t is declared with the kernel files,
 * in sys/mach_ipc_netport.h.
 */
#if	NeXT
#include	<sys/mach_ipc_netport.h>
#else	NeXT
#include	<mach/netport.h>
#endif	NeXT

#ifdef	notdef
typedef unsigned long	netaddr_t;
#endif	notdef

typedef union {
    struct {
	unsigned char ia_net_owner;
	unsigned char ia_net_node_type;
	unsigned char ia_host_high;
	unsigned char ia_host_low;
    } ia_bytes;
    netaddr_t ia_netaddr;
} ip_addr_t;

extern short 			conf_own_format;

#endif	_NM_DEFS_
