/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	pioctl.c,v $
 * Revision 2.2  90/07/05  11:27:42  mrt
 * 	Created for the Coda file system.
 * 	[90/07/05            dcs]
 * 
 */
#ifndef _BLURB_
#define _BLURB_
/*
                           CODA FILE SYSTEM
                      School of Computer Science
                      Carnegie Mellon University
                          Copyright 1987-89

Present and past contributors to Coda include M. Satyanarayanan, James
Kistler, Puneet Kumar, Maria Okasaki, Ellen Siegel, Walter  Smith  and
David  Steere.  Parts of Coda are derived from the August 1986 version
of the Andrew file system, which is owned  by  IBM,  and  may  not  be
distributed without explicit permission.
*/


static char *rcsid = "$Header: pioctl.c,v 2.2 90/07/05 11:27:42 mrt Exp $";
#endif _BLURB_

#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/viceioctl.h>
#include <sysent.h>

#define	CTL_FILE    "/coda/.CONTROL"


int pioctl(const char *path, unsigned long com, struct ViceIoctl *vidata, int follow) {
    /* Pack <path, vidata, follow> into a structure since ioctl takes only one data arg. */
    struct {
	const char *path;
	struct ViceIoctl vidata;
	int follow;
    } data;
    data.path = path;
    data.vidata = *vidata;
    data.follow = follow;

    /* Must change the size field of the command to match that of the new structure. */
    unsigned long cmd = (com & ~(IOCPARM_MASK << 16));	    /* mask out size field */
    int	size = ((com >> 16) & IOCPARM_MASK) + sizeof(char *) + sizeof(int);
    cmd	|= (size & IOCPARM_MASK) << 16;			    /* or in corrected size */

    int fd = open(CTL_FILE, O_RDONLY, 0);
    if (fd < 0) return(fd);

    int code = ioctl(fd, cmd, &data);

    /* Ignore close code. */
    (void)close(fd);

    /* Return result of ioctl. */
    return(code);
}
