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
 * $Log:	mach_error.h,v $
 * Revision 1.1  90/02/18  17:48:05  bww
 * 	Mach Release 2.5
 * 	[90/02/18  17:47:49  bww]
 * 
 * Revision 1.3  89/05/05  18:44:57  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 11-May-88  Mary Thompson (mrt) at Carnegie Mellon
 *	Added definition for mach_error_type
 *
 * 29-Mar-88  Mary Thompson (mrt) at Carnegie Mellon
 *	Corrected args to mach_error and added c_plusplus conditionals
 *
 * 30-Oct-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created.
 *
 */ 

#ifndef	_LIB_MACH_ERROR_H_
#define	_LIB_MACH_ERROR_H_	1

#include <mach/error.h>

char		*mach_error_string(
/*
 *	Returns a string appropriate to the error argument given
 */
#if	c_plusplus
	mach_error_t error_value
#endif	/* c_plusplus */
				);

void		mach_error(
/*
 *	Prints an appropriate message on the standard error stream
 */
#if	c_plusplus
	char 		*str,
	mach_error_t	error_value
#endif	/* c_plusplus */
				);

char		*mach_error_type(
/*
 *	Returns a string with the error system, subsystem and code
*/
#if	c_plusplus
	mach_error_t	error_value
#endif  /* c_plusplus */
				);

#endif	/* _LIB_MACH_ERROR_H_ */
