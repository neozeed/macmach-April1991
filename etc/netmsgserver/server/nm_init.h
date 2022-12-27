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
 * $Log:	nm_init.h,v $
 * Revision 1.1  90/02/19  21:54:14  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/05/02  11:14:27  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 10-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nm_init.h
 *
 *
 * $ Header: nm_init.h,v 1.4 89/05/02 11:14:27 dpj Exp $
 *
 */

/*
 * Network Server Initialisation.
 */


#ifndef	_NM_INIT_
#define	_NM_INIT_

#include <mach/boolean.h>

extern boolean_t nm_init();

#endif	_NM_INIT_
