/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	config.h,v $
 * Revision 1.1  90/02/19  21:48:00  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/08/24  17:45:59  mrt
 * 	Cleanup
 * 
 */
/*
 *	File:	config.h
 *	Author:	Joseph S. Barrera III
 *
 *	Copyright (C) 1989, Joseph S. Barrera III
 *
 *	Configuration parameters for netmemoryserver.
 *
 */
/*
 *  DEBUG: controls printing of debugging messages.
 *
 *  USE_SYSLOG: if on, debugging and errors are syslogged instead
 *	of being printed on terminal. USE_SYSLOG will be overridden
 *	(set to 0) if DEBUG is on.
 *
 *  MAX_NUM_KERNELS: maximum number of kernels that an object can
 *	be used by at one time. Currently must not be greater than
 *	32 (due to use of bitmasks as sets).
 *
 *  TIMEOUT: how often (in seconds) the routine which checks for
 *	errant kernels (crashed machines) gets called.
 *
 *  CACHE_OBJECTS: whether the kernels should be told to cache the
 *	objects that this server provides. In the short term, this
 *	should be on, to prevent the object port death problem.
 *
 *  MIN_PAGE_SIZE: smallest page size which can provided by itself.
 *	Kernels with page sizes smaller than MIN_PAGE_SIZE will
 *	be provided with clusters of pages which together make up
 *	a minpage (a page MIN_PAGE_SIZE big). MIN_PAGE_SIZE should
 *	be set such that most machines which will use this server
 *	have a page size that is not smaller than MIN_PAGE_SIZE.
 *
 *  MAX_PAGE_SIZE: no kernels with page sizes larger than this
 *	will be able to use the server. All memory objects managed
 *	by this server will be rounded up to a multiple of
 *	MAX_PAGE_SIZE.
 *
 *  MIN_PAGE_SHIFT: log base 2 of MIN_PAGE_SIZE.
 *
 *  MAX_PAGE_SHIFT: log base 2 of MAX_PAGE_SIZE.
 */

#define	DEBUG		0
#define	USE_SYSLOG	0
#define	MAX_NUM_KERNELS	32
#define	TIMEOUT		15
#define	CACHE_OBJECTS	1

#define	MIN_PAGE_SIZE	4096
#define	MIN_PAGE_SHIFT	12
#define	MAX_PAGE_SIZE	8192
#define	MAX_PAGE_SHIFT	13
