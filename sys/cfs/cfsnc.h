/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	cfsnc.h,v $
 * Revision 2.2  90/07/05  11:27:04  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.4  90/05/31  17:02:12  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 */
#ifndef _CFSNC_HEADER_
#define _CFSNC_HEADER_

#include <sys/types.h>
#include <sys/user.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <cfs/cfs.h>
#include <cfs/cnode.h>

#ifdef	KERNEL
#include <vfs/vfs.h>
#include <vfs/vnode.h>

#ifndef	VICEFID_DEFINED
#define	VICEFID_DEFINED	1
typedef u_long VolumeId;
typedef u_long VnodeId;
typedef u_long Unique;
typedef struct ViceFid {
    VolumeId Volume;
    VnodeId Vnode;
    Unique Unique;
} ViceFid;
#endif	not VICEFID_DEFINED

#else	KERNEL
#include <vfs/vfs.h>
#include <vfs/vnode.h>
#endif KERNEL

/*
 * Cfs constants
 */
#define CFSNC_NAMELEN	15		/* longest name stored in cache */
#define CFSNC_CACHESIZE 256		/* Default cache size */
#define CFSNC_HASHSIZE	64		/* Must be multiple of 2 */

/*
 * Hash function for the primary hash.
 */

/* 
 * First try -- (first + last letters + length + (int)cp) mod size
 * 2nd try -- same, except dir fid.vnode instead of cp
 */

#define CFSNC_HASH(name, namelen, cp) \
	((name[0] + name[namelen-1] + namelen + (int)(cp)) & (cfsnc_hashsize-1))

#define CFS_NAMEMATCH(cp, name, namelen, dcp) \
	((namelen == cp->namelen) && (dcp == cp->dcp) && \
		 (bcmp(cp->name,name,namelen) == 0))

/*
 * Functions to modify the hash and lru chains.
 * insque and remque assume that the pointers are the first thing
 * in the list node, thus the trickery for lru.
 */

#define CFSNC_HSHINS(elem, pred)	insque(elem,pred)
#define CFSNC_HSHREM(elem)		remque(elem)
#define CFSNC_HSHNUL(elem)		(elem)->hash_next = \
					(elem)->hash_prev = (elem)

#define CFSNC_LRUINS(elem, pred)	insque(LRU_PART(elem), LRU_PART(pred))
#define CFSNC_LRUREM(elem)		remque(LRU_PART(elem));
#define CFSNC_LRUGET(lruhead)		LRU_TOP((lruhead).lru_prev)

#define CFSNC_VALID(cncp)	(cncp->dcp != (struct cnode *)0)
 
#define LRU_PART(cncp)			(struct cfscache *) \
				((char *)cncp + (2*sizeof(struct cfscache *)))
#define LRU_TOP(cncp)				(struct cfscache *) \
			((char *)cncp - (2*sizeof(struct cfscache *)))
#define DATA_PART(cncp)				(struct cfscache *) \
			((char *)cncp + (4*sizeof(struct cfscache *)))
#define DATA_SIZE	(sizeof(struct cfscache)-(4*sizeof(struct cfscache *)))

/*
 * Structure for an element in the CFS Name Cache.
 * NOTE: I use the position of arguments and their size in the
 * implementation of the functions CFSNC_LRUINS, CFSNC_LRUREM, and
 * DATA_PART.
 */

struct cfscache {	
	struct cfscache	*hash_next,*hash_prev;	/* Hash list */
	struct cfscache	*lru_next, *lru_prev;	/* LRU list */
	struct cnode	*cp;			/* vnode of the file */
	struct cnode	*dcp;			/* parent's cnode */
	struct ucred	*cred;			/* user credentials */
	char		name[CFSNC_NAMELEN];	/* segment name */
	int		namelen;		/* length of name */
};

struct	cfslru {		/* Start of LRU chain */
	char *dummy1, *dummy2;			/* place holders */
	struct cfscache *lru_next, *lru_prev;   /* position of pointers is important */
};


struct cfshash {		/* Start of Hash chain */
	struct cfscache *hash_next, *hash_prev; /* NOTE: chain pointers must be first */
        int length;                             /* used for tuning purposes */
};

/*
 * Structure to contain statistics on the cache usage
 */

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
};

/* 
 * Symbols to aid in debugging the namecache code. Assumes the existence
 * of the variable cfsnc_debug, which is defined in cfs_namecache.c
 */
extern int cfsnc_debug;
#define CFSNC_DEBUG(N, STMT)     { if (cfsnc_debug & (1 <<N)) { STMT } }

#define CFSNC_FIND		((u_long) 1)
#define CFSNC_REMOVE		((u_long) 2)
#define CFSNC_INIT		((u_long) 3)
#define CFSNC_ENTER		((u_long) 4)
#define CFSNC_LOOKUP		((u_long) 5)
#define CFSNC_ZAPPFID		((u_long) 6)
#define CFSNC_ZAPFID		((u_long) 7)
#define CFSNC_ZAPVNODE		((u_long) 8)
#define CFSNC_ZAPFILE		((u_long) 9)
#define CFSNC_PURGEUSER		((u_long) 10)
#define CFSNC_FLUSH		((u_long) 11)
#define CFSNC_PRINTCFSNC	((u_long) 12)
#define CFSNC_PRINTSTATS	((u_long) 13)

#endif _CFSNC_HEADER_
