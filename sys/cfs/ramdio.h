/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	ramdio.h,v $
 * Revision 2.2  90/07/05  11:27:50  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.2  90/05/31  17:05:17  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 * 
 */

#ifndef _RAMDIO_H_
#define _RAMDIO_H_

/* Define ioctl commands and structure for ramdisk */

#define RAMDSBND     _IOW(d,1, struct ramdbound)   /* set device bounds */
#define RAMDGBND     _IOR(d,2, struct ramdbound)   /* return bound settings */
#define RAMDFREEZE   _IO(d, 3)		   	   /* block further writes */
#define RAMDTHAW     _IO(d, 4)		   	   /* reallow further writes */

struct ramdbound { 
	char    *addr;
	int     size;
};

#endif _RAMDIO_H_
