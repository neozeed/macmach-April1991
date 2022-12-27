/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	cfsio.h,v $
 * Revision 2.2  90/07/05  11:26:55  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.2  90/05/31  17:02:09  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 * 
 */

#ifndef _CFSIO_H_
#define _CFSIO_H_

/* Define ioctl commands for vcioctl, /dev/cfs */

#define CFSRESIZE    _IOW(c, 1, struct cfs_resize )  /* Resize CFS NameCache */
#define CFSSTATS      _IO(c, 2)                      /* Collect stats */
#define CFSPRINT      _IO(c, 3)                      /* Print Cache */
#define CFSTEST       _IO(c, 4)                      /* Print Cache */

struct cfs_resize { int hashsize, heapsize };

#endif !_CFSIO_H_
