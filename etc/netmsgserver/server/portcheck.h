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
 * $Log:	portcheck.h,v $
 * Revision 1.1  90/02/19  21:55:26  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.5  89/05/02  11:16:01  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  6-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed pc_checkup_interval - now obtained from param record.
 *
 * 19-Nov-86  Sanjay Agrawal (agrawal) and Healfdene Goguen (hhg) at Carnegie Mellon University
 *	Started.
 */
/*
 * portcheck.h
 *
 *
 * $ Header: portcheck.h,v 1.5 89/05/02 11:16:01 dpj Exp $
 */

/*
 * Definitions exported by the port checkups module.
 */


#ifndef _PORTCHECK_
#define _PORTCHECK_

#include <mach/boolean.h>

extern boolean_t pc_init();

#endif _PORTCHECKUPS_
