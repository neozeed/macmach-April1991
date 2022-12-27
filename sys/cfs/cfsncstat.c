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


static char *rcsid = "$Header: cfsncstat.c,v 2.2 90/07/05 11:27:15 mrt Exp $";
#endif _BLURB_

/*
 * HISTORY
 * $Log:	cfsncstat.c,v $
 * Revision 2.2  90/07/05  11:27:15  mrt
 * 	Created for Coda file system.
 * 	[90/07/05  10:16:20  mrt]
 * 
 * Revision 1.2  90/03/19  16:35:50  dcs
 * Changed Revision Log.
 * 
 */

/* 
 * This program will report on the current cfsnamecache statistics.
 * It obtains the stats by reading /dev/kmem.
 */

#include <nlist.h>
#include <sys/ioctl.h>
#include <cfs/cfsio.h>

struct cfsnc_statistics {
	unsigned	hits;
	unsigned	misses;
	unsigned	enters;
	unsigned	dbl_enters;
	unsigned	long_name_enters;
	unsigned	long_name_lookups;
	unsigned	long_remove;
	unsigned	lru_rm;
	unsigned	zapPfids;
	unsigned	zapFids;
	unsigned	zapFile;
	unsigned	zapUsers;
	unsigned	Flushes;
	unsigned        Sum_bucket_len;
	unsigned        Sum2_bucket_len;
	unsigned        Max_bucket_len;
	unsigned        Num_zero_len;
	unsigned        Search_len;
} cfsnc_stat;

print_cfsnc_stats()
{
	printf("\nSTATISTICS\n");
	printf("cfsnc_hits : %d\n", cfsnc_stat.hits);
	printf("cfsnc_misses : %d\n", cfsnc_stat.misses);
	printf("cfsnc_enters : %d\n", cfsnc_stat.enters);
	printf("cfsnc_dbl_enters : %d\n", cfsnc_stat.dbl_enters);
	printf("cfsnc_long_name_enters : %d\n", cfsnc_stat.long_name_enters);
	printf("cfsnc_long_name_lookups : %d\n", cfsnc_stat.long_name_lookups);
	printf("cfsnc_long_remove : %d\n", cfsnc_stat.long_remove);
	printf("cfsnc_lru_rm : %d\n", cfsnc_stat.lru_rm);
	printf("cfsnc_zapPfids : %d\n", cfsnc_stat.zapPfids);
	printf("cfsnc_zapFids : %d\n", cfsnc_stat.zapFids);
	printf("cfsnc_zapFile : %d\n", cfsnc_stat.zapFile);
	printf("cfsnc_zapUsers : %d\n", cfsnc_stat.zapUsers);
	printf("cfsnc_Flushes : %d\n", cfsnc_stat.Flushes);
        printf("cfsnc_SumLen : %d\n", cfsnc_stat.Sum_bucket_len);
        printf("cfsnc_Sum2Len : %d\n", cfsnc_stat.Sum2_bucket_len);
        printf("cfsnc_# 0 len : %d\n", cfsnc_stat.Num_zero_len);
        printf("cfsnc_MaxLen : %d\n", cfsnc_stat.Max_bucket_len);
        printf("cfsnc_SearchLen : %d\n", cfsnc_stat.Search_len);
}

main()
{
  struct nlist RawStats[4];
  int kmem;
  struct cfsnc_statistics *cfsnc_statptr = &cfsnc_stat;
  int fd, err;
  int cachesize, hashsize;

  fd = open("/dev/cfs1",0,0);
  if (fd < 0) {
    perror("Open /dev/cfs1");
    exit(-1);
  }

  err = ioctl(fd, CFSSTATS);
  if (err < 0) {
    perror("Ioctl /dev/cfs1");
  }

  close(fd);

  RawStats[0].n_name = "_cfsnc_stat";
  RawStats[1].n_name = "_cfsnc_size";
  RawStats[2].n_name = "_cfsnc_hashsize";
  RawStats[3].n_name = 0;

  if (nlist("/vmunix",RawStats) == -1) {
    printf("-1 returned from nlist\n");
    exit(-1);
  }

  if (RawStats[0].n_type == 0) {
    printf("Could not find the symbol in the namelist in VMUNIX\n");
    exit(-1);
  }

  kmem = open("/dev/kmem",0,0);
  if (kmem <= 0) {
    perror("open /dev/kmem");
    exit(-1);
  }

  
  if ((lseek(kmem, (long)RawStats[1].n_value, 0)) &&
      (read(kmem, (char *)&cachesize, sizeof(int)) > 0)) {
    printf("Cache size : %d ",cachesize);
  } else {
    printf("Nothing Read!\n");
  }

  if ((lseek(kmem, (long)RawStats[2].n_value, 0)) &&
      (read(kmem, (char *)&hashsize, sizeof(int)) > 0)) {
    printf("Hash size : %d\n", hashsize);
  } else {
    printf("Nothing Read!\n");
  }

  lseek(kmem, (long)RawStats[0].n_value, 0);
  read(kmem, (char *)cfsnc_statptr, sizeof(struct cfsnc_statistics));
  close(kmem);

  print_cfsnc_stats();
}

  
