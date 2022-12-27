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


static char *rcsid = "$Header: cfsnc_print.c,v 2.2 90/07/05 11:27:08 mrt Exp $";
#endif _BLURB_

/*
 * HISTORY
 * $Log:	cfsnc_print.c,v $
 * Revision 2.2  90/07/05  11:27:08  mrt
 * 	Created for Coda file system
 * 	[90/07/05  10:15:18  mrt]
 * 
 * Revision 1.2  90/03/19  16:35:16  dcs
 * Changed Revision Log.
 * 
 */

/*
 * This file sends an ioctl to the cfs device driver to ask it to print
 * the cache table (by the hash table) to /dev/console.
 */

#include <sys/ioctl.h>
#include <cfs/cfsio.h>

main()
{
  int fd, err;
	
  fd = open("/dev/cfs1",0,0);
  if (fd < 0) {
    perror("Open /dev/cfs1");
    exit(-1);
  }

  err = ioctl(fd, CFSPRINT, 0);
  if (err < 0) {
    perror("Ioctl /dev/cfs1");
    exit(-1);
  }

  close(fd);
}
