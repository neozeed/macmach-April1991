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
 * $Log:	mach_exception.h,v $
 * Revision 1.1  90/02/18  17:48:08  bww
 * 	Mach Release 2.5
 * 	[90/02/18  17:47:49  bww]
 * 
 * Revision 1.4  89/10/25  00:02:41  mrt
 * 	Changed the include exclusion variable to _USR_MACH_EXCEPTION so
 * 	that it would not conflict with include/mach/exception.h.
 * 	[89/10/09            mrt]
 * 
 * Revision 1.3  89/05/05  18:45:18  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 25-Apr-88  Karl Hauth (hauth) at Carnegie-Mellon University
 *	Created.
 *
 */ 

#ifndef	_USR_MACH_EXCEPTION_H_
#define	_USR_MACH_EXCEPTION_H_	1

#include <mach/kern_return.h>

char		*mach_exception_string(
/*
 *	Returns a string appropriate to the error argument given
 */
#if	c_plusplus
	int	exception
#endif	/* c_plusplus */
				);


void		mach_exception(
/*
 *	Prints an appropriate message on the standard error stream
 */
#if	c_plusplus
	char	*str,
	int	exception
#endif	/* c_plusplus */
				);


char		*mach_sun3_exception_string(
/*
 *	Returns a string appropriate to the error argument given
 */
#if	c_plusplus
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);


void		mach_sun3_exception(
/*
 *	Prints an appropriate message on the standard error stream
 */
#if	c_plusplus
	char	*str,
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);

char		*mach_mac2_exception_string(
/*
 *	Returns a string appropriate to the error argument given
 */
#if	c_plusplus
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);


void		mach_mac2_exception(
/*
 *	Prints an appropriate message on the standard error stream
 */
#if	c_plusplus
	char	*str,
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);

char		*mach_romp_exception_string(
/*
 *	Returns a string appropriate to the error argument given
 */
#if	c_plusplus
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);

void		mach_romp_exception(
/*
 *	Prints an appropriate message on the standard error stream
 */
#if	c_plusplus
	char	*str,
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);

char		*mach_vax_exception_string(
/*
 *	Returns a string appropriate to the error argument given
 */
#if	c_plusplus
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);

void		mach_vax_exception(
/*
 *	Prints an appropriate message on the standard error stream
 */
#if	c_plusplus
	char	*str,
	int	exception,
	int	code,
	int	subcode
#endif	/* c_plusplus */
				);

#endif	/* _USR_MACH_EXCEPTION_H_ */
