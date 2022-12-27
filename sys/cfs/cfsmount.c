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


static char *rcsid = "$Header: cfsmount.c,v 2.2 90/07/05 11:27:00 mrt Exp $";
#endif _BLURB_

/*
 * HISTORY
 * $Log:	cfsmount.c,v $
 * Revision 2.2  90/07/05  11:27:00  mrt
 * 	Created
 * 	[90/07/05  10:14:37  mrt]
 * 
 * Revision 1.4  90/06/07  13:16:58  dcs
 * Changed procedure headings to oldstyle for Pmax port.
 * 
 * Revision 1.3  90/03/19  17:42:48  dcs
 * Eliminated MACH conditionals.
 * 
 * Revision 1.2  90/03/19  16:35:05  dcs
 * Changed Revision Log.
 * 
 */

#include <sys/syscall.h>
#include <sys/errno.h>
#include <libc.h>
#include <stdio.h>

/* Needed for MOUNT_CFS definition. */
#include <vfs/vfs.h>

main(argc, argv)
     int argc;
     char **argv;
{
    if (argc != 3) {
	printf("Usage: %s special directory\n", argv[0]);
	exit(-1);
    }

    if (syscall(SYS_vfsmount, MOUNT_CFS, argv[2], (caddr_t)0, argv[1]) < 0) {
	perror("vfsmount");
	exit(-1);
    }
}
