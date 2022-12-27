/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	netmemory_defs.h,v $
 * Revision 1.1  90/02/19  21:48:21  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.3  89/08/24  17:46:18  mrt
 * 	Cleanup
 * 
 * Revision 1.2  89/04/29  13:22:14  mrt
 * 	Clean up for Mach 2.5. Inlcuded servers/errorlib.h to define
 * 	errorcode base.
 * 	[89/04/29            mrt]
 * 
 */
/*
 *	File:	netmemory_defs.h
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	C definitions for netmemory mig interface.
 *
 */

#ifndef	NETMEMORY_DEFS
#define	NETMEMORY_DEFS		1

#include <mach/error.h>
#include <servers/errorlib.h>


#define	NETMEMORY_SUCCESS		ERR_SUCCESS
#define NETMEMORY_INVALID_ARGUMENT	(SERV_NETMEMORY_MOD | 0x1)
#define	NETMEMORY_RESOURCE_SHORTAGE	(SERV_NETMEMORY_MOD | 0x2)

typedef mach_error_t netmemory_return_t;

typedef struct fault_info {
    int read_faults;
    int write_faults;
    int protection_faults;
} fault_info_t;

#endif	/* NETMEMORY_DEFS */
