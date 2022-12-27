/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	afs.h,v $
 * Revision 2.9  89/08/24  19:35:19  jsb
 * 	Defined CWired vcache state flag for afs_Wire/afs_IsWired functions.
 * 	[89/08/24  16:39:13  jsb]
 * 
 * Revision 2.8  89/08/09  09:47:36  jsb
 * 	Added extra fields to brequest (background daemon request) structure
 * 	to support afs resource pausing and interruption thereof.
 * 	[89/08/09  09:29:42  jsb]
 * 
 * Revision 2.7  89/08/02  07:57:35  jsb
 * 	Change NOTOKTIMEOUT such that authentication data structures for users
 * 	with no tokens are deallocated as soon as afs_GCUserData is called
 * 	(once every 10 minutes or after every few PSetTokens). There used to
 * 	be a two hour waiting period, which allowed too much garbage to
 * 	pile up, often resulting in kalloc panics.
 * 	[89/08/01  14:52:19  jsb]
 * 
 * 	Declare afs zones.
 * 	[89/07/31  17:56:07  jsb]
 * 
 * Revision 2.6  89/06/03  15:26:01  jsb
 * 	Merged with newer ITC sources. Changes include a renaming of
 * 	struct lock to struct afs_lock; thus we no longer need to do
 * 	our redefinition.  Also added hostaddr fields to structures
 * 	containing uid fields, for new gateway support, and declared
 * 	exported gateway routines.
 * 	[89/05/27  11:37:11  jsb]
 * 
 * Revision 2.5  89/04/22  15:12:29  gm0w
 * 	Updated to RX version.  Define lock -> afs_lock macro to avoid
 * 	conflict with <sys/lock.h>.
 * 	[89/04/14            gm0w]
 * 
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083

 * AFS system call opcodes
 */

#define	AFSOP_START_R		  0	/* no aux parms */
#define	AFSOP_START_CALLBACK	  1	/* no aux parms */
#define	AFSOP_START_RFTP	  2	/* no aux parms */
#define	AFSOP_START_AFS		  3	/* no aux parms */
#define	AFSOP_START_BKG		  4	/* no aux parms */
#define	AFSOP_ADDCELL		  5	/* parm 2 = cell str */
#define	AFSOP_CACHEINIT		  6	/* parms 2-4 -> cache sizes */
#define	AFSOP_CACHEINFO		  7	/* the cacheinfo file */
#define	AFSOP_VOLUMEINFO	  8	/* the volumeinfo file */
#define	AFSOP_CACHEFILE		  9	/* a random cache file (V*) */
#define	AFSOP_CACHEINODE	 10	/* random cache file by inode */
#define	AFSOP_AFSLOG		 11	/* output log file */
#define	AFSOP_ROOTVOLUME	 12	/* non-standard root volume name */
#define	AFSOP_STARTLOG		 14	/* temporary: Start afs logging */
#define	AFSOP_ENDLOG		 15	/* temporary: End afs logging */
#define	AFSOP_GO		100	/* whether settime is being done */
/* not for initialization: debugging assist */
#define	AFSOP_CHECKLOCKS	200	/* dump lock state */
#define	AFSOP_SHUTDOWN		201	/* Totally shutdown afs (deallocate all) */

/* The following aren't used by afs_initState but by afs_termState! */
#define	AFSOP_STOP_CALLBACK	210	/* Stop CALLBACK process */
#define	AFSOP_STOP_R		211	/* Stop R process */
#define	AFSOP_STOP_RFTP		212	/* Stop RFTP process */
#define	AFSOP_STOP_AFS		213	/* Stop AFS process */
#define	AFSOP_STOP_BKG		214	/* Stop BKG process */

/* The following are various levels of afs debugging */
#define	AFSDEB_GENERAL		1	/* Standard debugging */
#define	AFSDEB_NETWORK		2	/* low level afs networking */
#define	AFSDEB_RX		4	/* RX debugging */
#define	AFSDEB_VNLAYER		8	/* interface layer to AFS (aixops, gfsops, etc) */

/* generic undefined vice id */
#define	UNDEFVID	    (-1)

/* The basic defines for the Andrew file system
    better keep things powers of two so "& (foo-1)" hack works for masking bits */
#define	VCBS		16	    /* max callbacks per server to queue */
#define	MAXHOSTS	8	    /* max hosts per single volume */
#define	NBRS		10	    /* max number of queued daemon requests */
#define	NUSERS		16	    /* hash table size for unixuser table */
#define	NSERVERS	16	    /* hash table size for server table */
#define	NVOLS		64	    /* hash table size for volume table */
#define	NFENTRIES	256	    /* hash table size for disk volume table */
#define	VCSIZE		128	    /* stat cache hash table size */
#define	DCSIZE		64	    /* disk cache hash table size */
#define	PIGGYSIZE	1350	    /* max piggyback size */
#define	MAXVOLS		50	    /* max vols we can store */
#define	MAXMULTI	3	    /* max conns per single user/fs pair */
#define	TOKENTIMEOUT	(26*3600)   /* time after which to timeout conns with tokens */
#ifdef	AFS_MACH_ENV
#define	NOTOKTIMEOUT	0	    /* timeout tokenless conns immediately! */
#else
#define	NOTOKTIMEOUT	(2*3600)    /* time after which to timeout conns sans tokens */
#endif

/* below here used only for kernel procedures */
#ifdef KERNEL
/* background request structure */
#define	BPARMS		4

#define	BOP_NOOP	0	    /* leave 0 unused */
#define	BOP_FETCH	1	    /* parm1 is chunk to get */
#define	BOP_STORE	2	    /* parm1 is chunk to store */
#define	BOP_PATH	3	    /* parm1 is path, parm2 is chunk to fetch */

/* protocol is: refCount is incremented by user to take block out of free pool.
    Next, BSTARTED is set when daemon finds request.  This prevents
    other daemons from picking up the same request.  Finally, when
    request is done, refCount is zeroed.  BDONE and BWAIT are used by
    dudes waiting for operation to proceed to a certain point before returning.
*/
#define	BSTARTED	1	    /* request picked up by a daemon */
#define	BUVALID		2	    /* code is valid (store) */
#define	BUWAIT		4	    /* someone is waiting for BUVALID */
#ifdef	AFS_MACH_ENV
#define BUQUIT		8	    /* user interrupted, daemon should quit */
#endif
struct brequest {
    struct vcache *vnode;	    /* vnode to use, with vrefcount bumped */
    struct ucred *cred;		    /* credentials to use for operation */
    long parm[BPARMS];		    /* random parameters */
    long code;			    /* return code */
#ifdef	AFS_MACH_ENV
    struct tty *tty;		    /* user's u_ttyp, for uprintf */
    char *comm;			    /* user's u_comm (current command) */
    char *self;			    /* daemon's u_procp */
    char *sleep;		    /* address slept on when r.pausing */
#endif
    short refCount;		    /* use counter for this structure */
    char opcode;		    /* what to do (store, fetch, etc) */
    char flags;			    /* free, etc */
};

struct SecretToken {
    char data[56];
};

struct ClearToken {
	long AuthHandle;
	char HandShakeKey[8];
	long ViceId;
	long BeginTimestamp;
	long EndTimestamp;
};

struct VenusFid {
    long Cell;			    /* better sun packing if at end of structure */
    struct ViceFid Fid;
};

struct vrequest {
    long uid;			/* user id making the request */
#ifdef	AFS_GATEWAY
    unsigned long hostaddr;	/* internet address of his host */
#endif	AFS_GATEWAY
    long busyCount;		/* how many busies we've seen so far */
    char initd;			/* if non-zero, non-uid fields meaningful */
    char accessError;		/* flags for overriding error return code */
    char volumeError;		/* encountered a missing volume */
    char networkError;		/* encountered network problems */
};

#define	LOCALCELL	    1	/* local cell's number is always 1 */

/* cell flags */
#define	CPrimary	    1	    /* on if this is the primary cell */
#define	CNoSUID		    2	    /* 1 if no suid progs can run from this cell */
#define	COldVLDB	    4	    /* known to use old vldb */
#define	CNewVLDB	    8	    /* known to use new vldb */

struct cell {
    struct cell *next;
    long cell;				    /* unique id assigned by venus */
    char *cellName;			    /* char string name of cell */
    struct server *cellHosts[MAXHOSTS];	    /* volume *location* hosts for this cell */
    short states;			    /* state flags */
};

/* the unixuser flag bit definitions */
#define	UHasTokens	1	    /* are the st and ct fields valid (ever set)? */
#define	UTokensBad	2	    /* are tokens bad? */

struct unixuser {
    struct unixuser *next;	    /* next hash pointer */
    long uid;			    /* search based on uid and cell */
    long cell;
#ifdef	AFS_GATEWAY
    unsigned long hostaddr;	    /* ... and host internet address */
#endif	AFS_GATEWAY
    long vid;			    /* corresponding vice id in specified cell */
    short refCount;		    /* reference count for allocation */
    char states;		    /* flag info */
    char primary;		    /* true iff primary identity */
    long tokenTime;		    /* last time tokens were set, used for timing out conn data */
    long stLen;			    /* ticket length (if kerberos, includes kvno at head) */
    char *stp;			    /* pointer to ticket itself */
    struct ClearToken ct;
};

struct conn {
    /* Per-connection block. */
    struct conn	*next;		    /* Next dude same server. */
/*    struct afs_lock lock;		    /* lock */
    struct unixuser *user;	    /* user validated with respect to. */
    struct rx_connection *id;	    /* RPC connid. */
    struct server *server;	    /* server associated with this conn */
    short refCount;		    /* reference count for allocation */
    unsigned short port;	    /* port associated with this connection */
    char tag;			    /* which conn for this this is */
    char forceConnectFS;	    /* Should we try again with these tokens? */
};

/*
  * Queues implemented with both pointers and short offsets into a disk file.
  */
struct afs_q {
    struct afs_q *next;
    struct afs_q *prev;
};

struct afs_sq {
    short next;
    short prev;
};

#define SQNULL -1

/* Fid comparison routines */
#define	FidCmp(a,b) ((a)->Fid.Unique != (b)->Fid.Unique \
    || (a)->Fid.Vnode != (b)->Fid.Vnode \
    || (a)->Fid.Volume != (b)->Fid.Volume \
    || (a)->Cell != (b)->Cell)

/*
  * Operations on circular queues implemented with pointers.  Note: these queue objects are
  * always located at the beginning of the structures they are linking.
  */
#define	QInit(q)    ((q)->prev = (q)->next = (q))
#define	QAdd(q,e)   ((e)->next = (q)->next, (e)->prev = (q), \
			(q)->next->prev = (e), (q)->next = (e))
#define	QRemove(e)  ((e)->next->prev = (e)->prev, (e)->prev->next = (e)->next)
#define	QNext(e)    ((e)->next)
#define QPrev(e)    ((e)->prev)
/* do lots of address arithmetic to go from lruq to the base of the vcache structure.
      Don't move struct vnode, since we think of a struct vcache as a specialization
      of a struct vnode */
#define	QTOV(e)	    ((struct vcache *)(((char *) (e)) - (((char *)(&(((struct vcache *)(e))->lruq))) - ((char *)(e)))))

/*
  * Operations on circular dcache queues implemented with short file offsets.  Note: these queue
  * objects are never located at the beginning of the structures they are linking, so we always
  * need to provide the queue's name in the dcache record.
  */
#define	sQInit(q)		((q).next = (q).prev = SQNULL)
#define	sQAdd(q, qname, eidx)	(Fill this in)
#define	sQRemove(q, qname, eidx)	(Fill this in)
#define	sQNext(qname, eidx, nextidx)	(Fill this in)
#define	sQPrev(qname, eidx, previdx)	(Fill this in)

struct server {
    struct server *next;
    struct cell	*cell;		    /* cell in which this host resides */
    struct conn	*conns;		    /* all user connections to this server */
    struct ViceFid vcbs[VCBS];
    long host;			    /* in network byte order, except subsys */
    long portal;		    /* in network byte order */
    long subsys;		    /* in host order */
    long creatingVol;		    /* vol whose reference created this struct */
    unsigned short random;	    /* server priority, used for randomizing requests */
    char isDown;		    /* result of decision if server is down. */
    char vcbCount;		    /* count of vcbs */
};

/* state bits for volume */
#define VRO			1		/* volume is readonly */
#define VRecheck		2		/* recheck volume info with server */
#define	VBackup			4		/* is this a backup volume? */

struct volume {
    /* One structure per volume, describing where the volume is located
	  and where its mount points are. */
    struct volume *next;	/* Next volume in hash list. */
    long cell;			/* the cell in which the volume resides */
    struct afs_lock lock;		/* the lock for this structure */
    long volume;		/* This volume's ID number. */
    char *name;			/* This volume's name, or 0 if unknown */
    struct server *serverHost[MAXHOSTS];    /* servers serving this volume */
    struct VenusFid dotdot;	/* dir to access as .. */
    struct VenusFid mtpoint;	/* The mount point for this volume. */
    long roVol;
    long backVol;
    long rwVol;			/* For r/o vols, original read/write volume. */
    long accessTime;		/* last time we used it */
    long vtix;			/* volume table index */
    short refCount;		/* reference count for allocation */
    char states;		/* snuck here for alignment reasons */
};

/* format of an entry in volume info file */
struct fvolume {
    long cell;			/* cell for this entry */
    long volume;		/* volume */
    long next;			/* has index */
    struct VenusFid dotdot;	/* .. value */
    struct VenusFid mtpoint;	/* mt point's fid */
};

/* vcache state bits */
#define	CStatd	    1		/* has this file ever been stat'd? */
#define	CWired	    2		/* cache file wired down */
#define	CRO	    4		/* is it on a read-only volume */
#define	CMValid	    8		/* is the mount point info valid? */
#define	CCore	    0x10	/* storing a core file, needed since we don't get an open */
#define	CSafeStore  0x40	/* close must wait for store to finish (should be in fd) */

#define	CPSIZE	    2
#define	vrefCount   v.v_count

#define	AFS_MAXDV   0x7fffffff	    /* largest dataversion number */
#define	AFS_NOTRUNC 0x7fffffff	    /* largest dataversion number */
struct vcache {
    struct vnode v;			/* Has reference count in v.v_count */
    struct afs_q lruq;			/* lru q next and prev */
    struct vcache *hnext;		/* Hash next */
    struct VenusFid fid;
    struct mstat {
        long Length;
        long DataVersion;
        long Date;
        long Owner;
	long Group;
        short Mode;
        unsigned char LinkCount;
	/* vnode type is in v.v_type */
    } m;
    struct afs_lock lock;			/* The lock on the vcache contents. */
    long parentVnode;			/* Parent dir, if a file. */
    long parentUnique;
    struct VenusFid *mvid;		/* Either parent dir (if root) or root (if mt pt) */
    char *linkData;			/* Link data if a symlink. */
    long flushDV;			/* data version we've last flushed */
    long truncPos;			/* truncate file to this position at next store */
    long randomUid[CPSIZE];		/* random users' ids and access rights. */
#ifdef	AFS_GATEWAY
    long randomHostaddr[CPSIZE];	/* host addr for each random user */
#endif	AFS_GATEWAY
    long callback;			/* The callback host, if any */
    long cbExpires;			/* time the callback expires */
    short opens;			/* The numbers of opens, read or write, on this file. */
    short execsOrWriters;		/* The number of execs (if < 0) or writers (if > 0) of
					 this file. */
    short flockCount;			/* count of flock readers, or -1 if writer */
    unsigned char chunkShift;		/* log chunksize in bytes */
    char randomAccess[CPSIZE];		/* watch for more rights in prs_fs.h */
    char anyAccess;			/* System:AnyUser's access to this. */
    char mvstat;			/* 0->normal, 1->mt pt, 2->root. */
    char states;			/* state bits */
};

/*----------------------------------------------------------------------
 * AFS Data cache definitions
 *
 * Each entry describes a Unix file on the local disk that is
 * is serving as a cached copy of all or part of a Vice file.
 * Entries live in circular queues for each hash table slot
 *
 * Which queue is this thing in?  Good question.
 * A struct dcache entry is in the freeDSlot queue when not associated with a cache slot (file).
 * Otherwise, it is in the DLRU queue.  The freeDSlot queue uses the lruq.next field as
 * its "next" pointer.
 *
 * Cache entries in the DLRU queue are either associated with vice files, in which case
 * they are hashed by hvNextp and hcNextp pointers, or they are in the freeDCList
 * and are not associated with any vice file.  This last list uses the hvNextp pointer for
 * its "next" pointer.
 *
 * Note that the DLRU and freeDSlot queues are *memory* queues, while the hvNextp and
 * hcNextp hash lists and the freeDCList all go through the disk elements.  Think of the
 * memory queues as lower-level queues caching a number of the disk entries.
 *----------------------------------------------------------------------*/

#define	NULLIDX	    (-1)	/* null index definition */
/* struct dcache states bits */
#define	DDataMod    1		/* Data modified */
#define	DHeadChunk  4		/* entry is head chunk; in hvNextp list */
#define	DWriting    8		/* file being written (used for cache validation) */

/* dcache flags */
#define	DFNextStarted	1	/* next chunk has been prefetched already */
#define	DFEntryMod	2	/* has entry itself been modified? */
#define	DFFetching	4	/* file is currently being fetched */
#define	DFWaiting	8	/* someone waiting for file */
#define	DFFetchReq	0x10	/* someone is waiting for DFFetching to go on */


/* flags in afs_indexFlags array */
#define	IFEverUsed	1		/* index entry has >= 1 byte of data */
#define	IFFree		2		/* index entry in freeDCList */
#define	IFDataMod	4		/* file needs to be written out */
#define	IFFlag		8		/* utility flag */

struct afs_ioctl {
	caddr_t in, out;	/* Data to be transferred in, or out */
	short in_size;		/* Size of input buffer <= 2K */
	short out_size;		/* Maximum size of output buffer, <= 2K */
};

/* CacheItems file has a header of type struct afs_fheader (keep aligned properly) */
struct afs_fheader {
    long magic;
    long firstCSize;
    long otherCSize;
    long spare;
};
#define	AFS_FHMAGIC	    0x7635fab8

/* kept on disk and in dcache entries */
struct fcache {
    short hvNextp;		/* Next in vnode hash table, or freeDCList */
    short hcNextp;		/* Next index in [fid, chunk] hash table */
    short chunkNextp;		/* File queue of all chunks for a single vnode */
    struct VenusFid fid;	/* Fid for this file */
    long modTime;		/* last time this entry was modified */
    long versionNo;		/* Associated data version number */
    long chunk;			/* Relative chunk number */
    long inode;			/* Unix inode for this chunk */
    long chunkBytes;		/* Num bytes in this chunk */
    char states;		/* Has this chunk been modified? */
};

/* kept in memory */
struct dcache {
    struct afs_q lruq;		/* Free queue for in-memory images */
    short refCount;		/* Associated reference count. */
    short index;		/* The index in the CacheInfo file*/
    short flags;		/* more flags bits */
    long validPos;		/* number of valid bytes during fetch */
    struct fcache f;		/* disk image */
};

/* macros to compute useful numbers from offsets.  AFS_CHUNK gives the chunk
    number for a given offset; AFS_CHUNKOFFSET gives the offset into the chunk
    and AFS_CHUNKBASE gives the byte offset of the base of the chunk.
      AFS_CHUNKSIZE gives the size of the chunk containing an offset.
      AFS_CHUNKTOBASE converts a chunk # to a base position.
      Chunks are 0 based and go up by exactly 1, covering the file.
      The other fields are internal and shouldn't be used */
/* basic parameters */
#ifdef AFS_NOCHUNKING

#define	AFS_OTHERCSIZE	0x10000
#define	AFS_LOGCHUNK	16
#define	AFS_FIRSTCSIZE	0x40000000

#else AFS_NOCHUNKING

#define	AFS_OTHERCSIZE	0x10000	    /* 65536, must change AFS_LOGCHUNK, too */
#define	AFS_LOGCHUNK	16	    /* log of AFS_OTHERCSIZE */
#define	AFS_FIRSTCSIZE	0x10000	    /* 65536 */

#endif AFS_NOCHUNKING

/* macros */
#define	AFS_CHUNKOFFSET(x)  ((x) < AFS_FIRSTCSIZE? (x) : \
(((x)-AFS_FIRSTCSIZE) & (AFS_OTHERCSIZE-1)))
#define	AFS_CHUNK(x)    ((x) < AFS_FIRSTCSIZE? 0 : \
(((x)-AFS_FIRSTCSIZE)>>AFS_LOGCHUNK)+1)
#define	AFS_CHUNKBASE(x)    ((x) < AFS_FIRSTCSIZE? 0 : \
(((x)-AFS_FIRSTCSIZE) & ~(AFS_OTHERCSIZE-1))+AFS_FIRSTCSIZE)
#define	AFS_CHUNKSIZE(x)    ((x) < AFS_FIRSTCSIZE? AFS_FIRSTCSIZE : AFS_OTHERCSIZE)
#define	AFS_CHUNKTOBASE(x)  ((x) == 0? 0 : AFS_FIRSTCSIZE + (((x)-1)<<AFS_LOGCHUNK))
#define	AFS_CHUNKTOSIZE(x)  ((x) == 0? AFS_FIRSTCSIZE : AFS_OTHERCSIZE)

#define	AFS_ZEROS   64	    /* zero buffer */

#define DCHASHSIZE 1024
#define DVHASHSIZE 1024
/* The PFlush algorithm makes use of the fact that Fid.Unique is not used in
  below hash algorithms.  Change it if need be so that flushing algorithm
  doesn't move things from one hash chain to another
*/
#define	DCHash(v, c)	((((v)->Fid.Vnode + (v)->Fid.Volume + (c))) & (DCHASHSIZE-1))
	/*Vnode, Chunk -> Hash table index*/
#define	DVHash(v)	((((v)->Fid.Vnode + (v)->Fid.Volume )) & (DVHASHSIZE-1))
	/*Vnode -> Other hash table index*/
/* don't hash on the cell, our callback-breaking code sometimes fails to compute
    the cell correctly, and only scans one hash bucket */
#define	VCHash(fid)	(((fid)->Fid.Volume + (fid)->Fid.Vnode) & (VCSIZE-1))

extern short afs_dvhashTable[DVHASHSIZE];	/*Data cache hash table (on vnodes)*/
extern short afs_dchashTable[DCHASHSIZE];	/*Ditto, on [fid, chunk]*/
extern struct dcache **afs_indexTable;		/*Pointers to in-memory dcache entries*/
extern long afs_cacheFiles;			/*Size of afs_indexTable*/
extern long afs_cacheBlocks;			/*1K blocks in cache*/
extern long afs_cacheStats;			/*Stat entries in cache*/
extern struct vcache *afs_vhashTable[VCSIZE];	/*Stat cache hash table*/
extern long afs_initState;			/*Initialization state*/
extern long afs_termState;			/* Termination state */
extern struct VenusFid afs_rootFid;		/*Root for whole file system*/
extern long afs_oddCBs;				/* Count of odd callbacks*/
extern long afs_evenCBs;			/* Count of even callbacks*/
extern long afs_oddZaps;			/* Count of odd fid deletes */
extern long afs_evenZaps;			/* Count of even fid deletes */
extern struct brequest afs_brs[NBRS];		/* request structures */
extern long afs_debug;

#define	UHash(auid)	((auid) & (NUSERS-1))
#define	VHash(avol)	((avol)&(NVOLS-1))
#define	SHash(aserv)	((aserv) & (NSERVERS-1))
#define	FVHash(acell,avol)  (((avol)+(acell)) & (NFENTRIES-1))

extern long dc_sync();
    /* Args:
         *	    vnode *vp		vnode whose pages must be flushed
         *
         */

extern long dc_init();
    /*
        * Args:
        *	None.
        */

extern long dc_xfer();
    /*
         * Args:
         *	vnode *vp		Ptr to vnode involved.
         *	struct uio *uio		transfer info cntg positions and lengths of buffer and file
         *	enum uio_rw reading	UIO_READ and UIO_WRITE
         */

extern osi_zone_t afs_buf_zone;
extern osi_zone_t afs_fid_zone;
extern osi_zone_t afs_cell_zone;
extern osi_zone_t afs_conn_zone;
extern osi_zone_t afs_vcache_zone;
extern osi_zone_t afs_packet_zone;
extern osi_zone_t afs_server_zone;
extern osi_zone_t afs_osifile_zone;
extern osi_zone_t afs_unixuser_zone;
extern osi_zone_t afs_VenusFid_zone;

extern struct cell	    *afs_GetCell();
extern struct cell	    *afs_GetCellByName();
extern struct unixuser	    *afs_GetUser();
extern struct volume	    *afs_GetVolume();
extern struct volume	    *afs_GetVolumeByName();
extern struct conn	    *afs_Conn();
extern struct conn	    *afs_ConnByHost();
extern struct conn	    *afs_ConnByMHosts();
extern struct server	    *afs_GetServer();
extern struct cell	    *afs_NewCell();
extern struct dcache	    *afs_GetDCache();
extern struct dcache	    *afs_FindDCache();
extern struct dcache	    *afs_NewDCache();
extern struct dcache	    *afs_GetDSlot();
extern struct vcache	    *afs_GetVCache();
extern struct brequest	    *afs_BQueue();
#ifdef	AFS_GATEWAY
extern unsigned long	    afs_gateway_get_auid();
extern char		    *afs_gateway_get_sysname();
#endif	AFS_GATEWAY
#endif KERNEL

#define	AFS_FSPORT	    htons(7000)
#define	AFS_VLPORT	    htons(7003)
