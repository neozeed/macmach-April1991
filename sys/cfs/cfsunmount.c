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


static char *rcsid = "$Header: cfsunmount.c,v 2.2 90/07/05 11:27:19 mrt Exp $";
#endif _BLURB_

/*
 * HISTORY
 * $Log:	cfsunmount.c,v $
 * Revision 2.2  90/07/05  11:27:19  mrt
 * 	Created for Coda file system.
 * 	[90/07/05  10:17:20  mrt]
 * 
 * Revision 1.3  90/06/07  13:17:20  dcs
 * Changed procedure headings to oldstyle for Pmax port.
 * 
 * Revision 1.2  90/03/19  16:37:44  dcs
 * Changed Revision Log.
 * 
 */
#include <sys/syscall.h>
#include <sys/errno.h>
#include <libc.h>
#include <stdio.h>


main(argc, argv)
     int argc;
     char **argv;
{
    if (argc != 2) {
	printf("Usage: %s directory\n", argv[0]);
	exit(-1);
    }

    if(syscall(SYS_unmount, argv[1]) < 0) {
	perror("unmount");
	exit(-1);
    }
}
