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


static char *rcsid = "$Header: cfsnc_resize.c,v 2.2 90/07/05 11:27:11 mrt Exp $";
#endif _BLURB_

/*
 * HISTORY
 * $Log:	cfsnc_resize.c,v $
 * Revision 2.2  90/07/05  11:27:11  mrt
 * 	Created for Coda file system
 * 	[90/07/05  10:15:39  mrt]
 * 
 * Revision 1.2  90/03/19  16:35:26  dcs
 * Changed Revision Log.
 * 
 */

/*
 * This file will send an ioctl to the cfs device driver (vcioctl in cfs_subr.c)
 * which will cause the cfs name cache to change it's size.
 */

#include <sys/ioctl.h>
#include <cfs/cfsio.h>

main(argc, argv)
     int argc;
     char *argv[];
{
  struct cfs_resize data;

  int fd, err;

  if (argc < 3) {
    printf("Usage: cfsnc_resize <newheapsize> <newhashsize>\n");
    exit(-1);
  }

  data.heapsize = atoi(argv[1]);
  data.hashsize = atoi(argv[2]);

  if ((data.heapsize % 2) || (data.hashsize %2)) {
    printf("The sizes must be multiples of 2\n");
    exit(-1);
  }

  fd = open("/dev/cfs1",0,0);
  if (fd < 0) {
    perror("Open /dev/cfs1");
    exit(-1);
  }

  err = ioctl(fd, CFSRESIZE, &data);
  if (err < 0) {
    perror("Ioctl /dev/cfs1");
    exit(-1);
  }

  close(fd);
}




