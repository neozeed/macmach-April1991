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
 * $Log:	uid.h,v $
 * Revision 1.1  90/02/19  21:57:54  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.5  89/05/02  11:19:10  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 */
/*
 * uid.h
 *
 * $ Header: uid.h,v 1.5 89/05/02 11:19:10 dpj Exp $
 *
 */
/*
 * Public definitions for generator of locally unique identifiers.
 */

#ifndef	_UID_
#define	_UID_

#include <mach/boolean.h>

extern boolean_t uid_init();
/*
*/

extern long uid_get_new_uid();
/*
*/

#endif	_UID_
