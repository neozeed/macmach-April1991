/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	autoconf.h,v $
 * Revision 2.2  90/08/30  11:00:41  bohman
 * 	Created.
 * 	[90/08/29  10:59:23  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/autoconf.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MAC2_AUTOCONF_H_
#define _MAC2_AUTOCONF_H_

/*
 * Configuration structure for
 * fixed devices.
 */
struct devconf {
    char		*dc_name;	/* ascii device name */
    int			dc_unit;	/* device unit number */
    vm_offset_t		dc_csr;		/* device csr */
    int			dc_vec;		/* device intr vector number */
    int			(*dc_init)();	/* device init routine */
    void		(*dc_intr)();	/* device intr handler routine */
};

extern struct devconf	    devconf[];

#endif	_MAC2_AUTOCONF_H_
