/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rfs_subr.c,v $
 * Revision 2.12  90/07/03  16:47:00  mrt
 * 	Changed unlocking in failure case in rfsCache, since the
 * 	previous code could cause a kernel panic.
 * 	[90/05/14            mrt]
 * 
 * Revision 2.11  89/08/08  22:01:03  jsb
 * 	Oops. Removed Bob's fix since Glenn already stuck in a modified
 * 	version of same.
 * 
 * Revision 2.10  89/08/08  21:50:06  jsb
 * 	When an inode is created for exec'ing a file don't forget
 * 	to INITIALIZE_INODE() it.
 * 	[89/07/31            rvb]
 * 
 * Revision 2.9  89/08/07  14:56:27  gm0w
 * 	Added code to initialize inode i_header.ih_mode to IFREG in
 * 	rfsInodeAllocate().
 * 	[89/08/07            gm0w]
 * 
 * Revision 2.8  89/04/22  15:27:18  gm0w
 * 	Changed to use new inode macros for manipulation of inodes.
 * 	[89/04/14            gm0w]
 * 
 * Revision 2.7  89/03/15  15:14:52  gm0w
 * 	changed include sys/rfs.h to rfs/rfs/h
 * 
 * Revision 2.6  89/03/09  19:30:11  rpd
 * 	More cleanup.
 * 
 * Revision 2.5  89/02/25  14:45:15  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.4  89/02/09  04:32:35  mwyoung
 * Code cleanup cataclysm.
 * 
 * Revision 2.3  89/01/18  00:40:17  jsb
 * 	Vnode support.
 * 	[89/01/16            jsb]
 * 
 * Revision 2.2  88/08/24  01:26:10  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 	
 * 	Removed RFS conditional entirely... this file isn't built unless
 * 	CMUCS_RFS is specified.
 * 	[88/08/23            mwyoung]
 * 
 * 30-Aug-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Converted "bool" uses to "boolean_t".  Added definition
 *	of generic NULL to pacify compilation until properly fixed.
 *
 * 13-Dec-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Created for 4.2BSD.
 * 
 */

/*	rfs_subr.c	CMU	85/12/13	*/

/*
 *  Remote file system - utility procedures modules
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/buf.h>
#include <sys/mbuf.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/inode.h>
#include <sys/map.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <rfs/rfs.h>
#include <sys/fs.h>
#include <sys/mount.h>

#define NULL	0


/*
 *  rfsAllocate - allocate structure from dynamic memory
 *
 *  rfdp = dynamic allocation descriptor
 *
 *  Allocate a new structure of the size specified in the supplied descriptor.
 *  If there are no free structures recorded in the descriptor, allocate
 *  another mbuf to provide more.  Space for each structure is allocated
 *  contiguously from the initial offset to the data area for as many
 *  structures which will completely fit within the data area of the mbuf.  The
 *  length field of the mbuf is used to remember the number of structures carved
 *  out of the mbuf in this fashion which are currently free.
 *
 *  Return: a pointer to the newly allocated structure (which can be cast as
 *  appropriate) or NULL if no more mbufs could be allocated.
 *
 *  TODO:  assign new mbuf types.
 */

struct rfsFreeOverlay *
rfsAllocate(rfdp)
    register struct rfsFreeDescriptor *rfdp;
{
    register struct rfsFreeOverlay *rfop;
    register struct mbuf *m;

    rfs_printf4(RFSD_U_TRACEIN, "<Allo rfd=[%X,%d,%d]\n", rfdp->rfd_head, rfdp->rfd_size, rfdp->rfd_max);
    while (rfdp->rfd_head == NULL)
    {
	m = m_get(M_WAIT, MT_PCB);
	if (m == 0)
	    return(NULL);
	for (m->m_len = 0; m->m_len < rfdp->rfd_max;m->m_len++)
	{
	    rfop = mtod(m, struct rfsFreeOverlay *); 
	    rfop->rfo_next = rfdp->rfd_head;
	    rfdp->rfd_head = rfop;
	    m->m_off += rfdp->rfd_size;
	    rfs_printf5(RFSD_U_TRACEOUT, "*Allo rfop=%X,rfd=[%X,%d,%d]\n",
		       rfop, rfdp->rfd_head, rfdp->rfd_size, rfdp->rfd_max);
	}
    }
    rfop = rfdp->rfd_head;
    rfdp->rfd_head = rfop->rfo_next;
    m = dtom(rfop);
    if (m->m_len < 1 || m->m_len > rfdp->rfd_max)
	panic("rfsAllocate");
    m->m_len--;
    rfs_printf7(RFSD_U_TRACEOUT, ">Allo %X,m=[%X,%d],rfd=[%X,%d,%d]\n", 
	       rfop, m, m->m_len,
	       rfdp->rfd_head, rfdp->rfd_size, rfdp->rfd_max);
    return(rfop);
}



/*
 *  rfsFree - free structure to dynamic memory
 *
 *  rfdp = dynamic allocation descriptor
 *  rfop = structure to free
 *
 *  Link the freed structure onto the indicated descriptor.  If this releases
 *  the last structure which was carved out of a particular mbuf, scan the
 *  decsriptor and remove all structures in this mbuf from the list so that the
 *  mbuf may be returned to the free mbuf pool.  As a consistency check, we
 *  must find exactly as many free structures from this mbuf in the descriptor
 *  list as were recorded in the m_len field of the mbuf header.
 */

void
rfsFree(rfdp, rfop)
    register struct rfsFreeDescriptor *rfdp;
    register struct rfsFreeOverlay *rfop;
{
    register struct mbuf *m;

    m = dtom(rfop);
    rfs_printf7(RFSD_U_TRACEIN, "<Free rfd=[%X,%d,%d],rfo=%X,m=[%X,%d]\n",
	       rfdp->rfd_head, rfdp->rfd_size, rfdp->rfd_max, rfop,
	       m, m->m_len);
    rfop->rfo_next = rfdp->rfd_head;
    rfdp->rfd_head = rfop;
    if (m->m_len < 0 || m->m_len >= rfdp->rfd_max)
	panic("rfsFree m_len");
    if (++(m->m_len) == rfdp->rfd_max)
    {
        register struct rfsFreeOverlay **rfopp = &rfdp->rfd_head;

	for (rfop=rfdp->rfd_head; rfop; rfop = rfop->rfo_next)
	{
	    if (dtom(rfop) == m)
	    {
		*rfopp = rfop->rfo_next; 
		m->m_len--;
		rfs_printf5(RFSD_U_TRACEOUT, "*Free rfop=%X,rfd=[%X,%d,%d]\n",
			   rfop, rfdp->rfd_head, rfdp->rfd_size, rfdp->rfd_max);
	    }
	    else
		rfopp = &rfop->rfo_next;
	}
	if (m->m_len != 0)
	    panic("rfsFree m_free");
	m_free(m);
    }
    rfs_printf4(RFSD_U_TRACEOUT, ">Free rfd=[%X,%d,%d]\n",
	       rfdp->rfd_head, rfdp->rfd_size, rfdp->rfd_max);
}



/*
 *  Remote inode cache manipulation operations and data structures
 */

struct rfsMountEntry rfsMountTable[NMOUNT] = {0};
struct rfsCacheEntry *rfsCacheTable[NRFSCACHE] = {0};


/*
 *  Use at most 1/3 of the free fragments in a file system for the remote
 *  file system cache.
 */
int rfsCacheFactor = 3;

/*
 *  Cache hit statistics
 */
int rfsCacheHit = 0;		/* cache hits validated */
int rfsCacheInvalid = 0;	/* cache hits invalidated */
int rfsCacheFetch = 0;		/* cache fetches performed */
int rfsCacheBusy = 0;		/* cache fetch remote file busy errors */



/*
 *  rfsInCache - test for a remote inode in the local execution cache
 *
 *  addr  = IP address holding remote file
 *  st    = stat buffer for remote file
 *  index = result hash array index for remote inode
 *
 *  Check for a remote inode in the local execution cache with matching IP
 *  address, i-number and device number.
 *
 *  Return: the appropriate cache entry with its local inode locked, or NULL if
 *  the remote inode cannot be located in the cache.  Always set the result
 *  index value in any case so that it can be used by the caller to insert a
 *  new entry if the cache search fails.
 */

struct rfsCacheEntry *
rfsInCache(addrp, stp, indexp)
    struct in_addr *addrp;
    struct stat *stp;
    int *indexp;
{
    register struct rfsCacheEntry *rcep;

    for (rcep = rfsCacheTable[*indexp=rfsCacheHash(addrp, stp->st_ino, stp->st_dev)];
	 rcep != NULL;
	 rcep = rcep->rce_next)
    {
	if
        (
          rcep->rce_addr.s_addr == addrp->s_addr
          &&
	  rcep->rce_ino == stp->st_ino && rcep->rce_dev == stp->st_dev
	)
	{
	    LOCK_INODE(rcep->rce_ip);
	    break;
	}
    }
    return(rcep);
}




/*
 *  rfsValidCache - check for obsolete cache entry
 *
 *  rcep = cache entry
 *  stp  = stat structure of remote inode
 *
 *  Return:  the local inode if the cache entry is still valid (the size and
 *  modification times haven't changed on the remote file), otherwise NULL.
 */

struct inode *
rfsValidCache(rcep, stp)
    register struct rfsCacheEntry *rcep;
    register struct stat *stp;
{
    if 
    (
      rcep->rce_mtime == stp->st_mtime
      &&
      rcep->rce_size == stp->st_size
    )
    {
	rfsCacheHit++;
	rcep->rce_used = time.tv_sec;
	return(rcep->rce_ip);
    }
    rfsCacheInvalid++;
    return(NULL);
}



/*
 *  rfsCache - attach to a remote inode in the local execution cache
 *
 *  rcep   = cache entry (for existing file) or NULL 
 *  rfep   = remote file entry
 *  stp    = stat buffer for remote file
 *  length = number of bytes to cache locally
 *
 *  If the remote inode is not found in the cache or has changed since we last
 *  updated it, retrieve a new version and update the cache appropriately.
 *
 *  Return: a locked inode containing a cached copy of the remote file
 *  or NULL on error with u.u_errno set appropriately.
 *
 *  TODO: deal with possible network hang situations with inode locked
 */

struct inode *
rfsCache(rcep, rfep, stp, length)
    register struct rfsCacheEntry *rcep;
    register struct rfsFileEntry *rfep;
    struct stat *stp;
    off_t length;
{
    register struct rfsConnectionBlock *rcbp = rfep->rfe_rcbp;
    register struct inode *ip = NULL;
    register struct buf *bp = NULL;
    struct stat st;
    struct uio uio;
    struct iovec iov;

    /*
     *  If we don't currently have a cache entry (because the check in our
     *  caller failed to locate a match for this remote inode), we must create
     *  and allocate one now.  Up to this point and including either or both of
     *  the cache and inode allocation calls below, multiple processes may be
     *  trying to execute the same remote file through this point.  Thus, after
     *  the allocations complete, we must check the cache again to see if some
     *  other process has perhaps already created the entry we were in the
     *  middle of attempting.  If so, we must release the allocated inode and
     *  cache entries and use this one instead.  If not, we can safely insert
     *  it into the cache and thereafter the locked inode will prevent any
     *  other processes from connecting to it until it has been completely
     *  updated.  The cache is always swept before we allocate a new inode to
     *  insure that the new allocation happens on the "best" file system and
     *  that any old allocations are flushed from file systems which have
     *  subsequently become scarce on space.
     */
    if (rcep == NULL)
    {
	struct rfsCacheEntry *trcep;
	int midx = -1;
	int index;

	rfsCacheSweep(FALSE);
	if ((rcep = rfsCacheAllocate()) == NULL)
	    return(NULL);
	ip = rfsInodeAllocate(stp, length, &midx);
	if (ip == NULL)
	{
	    rfsCacheFree(rcep);
	    return(NULL);
	}
        if ((trcep=rfsInCache(&rcbp->rcb_rl.rl_addr, stp, &index)) == NULL)
	{
	    rcep->rce_ip = ip;
	    rcep->rce_addr  = rcbp->rcb_rl.rl_addr;
	    rcep->rce_ino   = stp->st_ino;
	    rcep->rce_dev   = stp->st_dev;
	    rcep->rce_mt    = midx;
	    rcep->rce_next  = rfsCacheTable[index];
	    rfsCacheTable[index] = rcep;
	}
	else
	{
#if MACH_VFS
	/* FINISH_WITH_INODE insists on the INODE being unlocked */
	    ip->i_flag &= ~ILOCKED;
	    if ((ip)->i_flag & IWANT) {
		(ip)->i_flag &= ~IWANT;
		wakeup((caddr_t)(ip));
	    }

#endif /* MACH_VFS */
	    FINISH_WITH_INODE(ip);
	    rfsCacheFree(rcep);
	    rcep = trcep;
	    ip = rfsValidCache(rcep, stp);
	    if (ip)
		return(ip);
	}
    }
    ip = rcep->rce_ip;
    /*
     *  When we get here, either we have just created a completely new cache
     *  entry or picked up an obsolete existing cache entry for the remote
     *  inode which needs to be updated.  In the latter case, we must release
     *  the disk space currently assigned to the local inode before updating
     *  its new contents (in case the size has contracted).  In either case,
     *  the local inode is now locked (either as a result of the
     *  rfsInodeAlloc() call above or the rfsInCache() call above or in our
     *  caller.  We also zero the size recorded in the cache entry in case
     *  something goes wrong during the retrieval below so that the incomplete
     *  cache entry will not be used without attempting to refresh it.
     */
    rfsMountTable[rcep->rce_mt].rme_frags -= btodb(ip->i_size+DEV_BSIZE-1);
    itrunc(ip, (u_long)0);
    rcep->rce_size  = 0;

    /*
     *  Copy the file from the remote inode onto the local copy in MAXBSIZE
     *  chunks.  At the end of the transfer, the remote file must not have
     *  changed during this operation or we could be in trouble.  For now,
     *  turn this into an ETXTBSY error although this is not strictly accurate
     *  (since we may not even have a text segment in the file).
     */
    bp = geteblk(MAXBSIZE);
    if (bp == NULL)
	goto out;
    rfsCacheFetch++;
    uio.uio_offset = 0;
    uio.uio_iov    = &iov;
    uio.uio_segflg = 1;
    while(length > 0)
    {
	uio.uio_iovcnt = 1;
	iov.iov_base   = bp->b_un.b_addr;
	iov.iov_len    = MAXBSIZE;
	uio.uio_resid  = MAXBSIZE;
	u.u_error = rfsRdWr(rfep, &uio, RFST_READ);
	if (u.u_error)
	    goto out;
	uio.uio_iovcnt = 1;
	iov.iov_base   = bp->b_un.b_addr;
	iov.iov_len    = MAXBSIZE-uio.uio_resid;
	if (iov.iov_len == 0)
	    break;
	uio.uio_resid  = iov.iov_len;
	uio.uio_offset -= uio.uio_resid;
	length -= iov.iov_len;
	u.u_error = rwip(ip, &uio, UIO_WRITE);
	if (u.u_error)
	    goto out;
    }
    if (length != 0)
    {
	u.u_error = EIO;
	goto out;
    }
    rfs_stat1(rcbp, (struct nameidata *)NULL, 0, &st, RFST_NFSTAT|RFST_KERNEL, 0);
    if (u.u_error)
	goto out;
    if (st.st_mtime != stp->st_mtime || st.st_size != stp->st_size)
    {
	u.u_error = ETXTBSY;
	rfsCacheBusy++;
	goto out;
    }
    brelse(bp);

    /*
     *  Now that the remote file has been successfully copied to the local
     *  inode, show the entry accessed, update the cache entry fields, and
     *  adjust the total cache space in use on the target file system to
     *  reflect the newly created inode.
     */
    rcep->rce_used  = time.tv_sec;
    rcep->rce_mtime = stp->st_mtime;
    rcep->rce_size  = stp->st_size;
    rfsMountTable[rcep->rce_mt].rme_frags += btodb(ip->i_size+DEV_BSIZE-1);
    return(ip);

out:
    if (bp)
	brelse(bp);
    if (ip)
    {
	itrunc(ip, (u_long)0);
	UNLOCK_INODE(ip);
    }
    return(NULL);
}



/*
 *  rfsCacheSweep - scan the remote inode cache for purgable entries.
 *
 *  all = purge entire cache flag
 *
 *  Iterate through the mounted file system table, looking for any file systems
 *  with more fragments in use in the cache than should be retained there
 *  according to the caching factor.  For each such file system, purge the
 *  oldest entry located on it from the cache until the cache space use falls
 *  below this threshhold or no more entries can be purged from the cache.
 *  If the purge all flag was specified, purge all entries from the cache without
 *  regard to the the current space usage.
 */

void
rfsCacheSweep(all)
    boolean_t all;
{
    register int i;

    for (i=0; i<NMOUNT; i++)
    {
	register struct mount *mp = &mount[i];
	register struct rfsMountEntry *rmep = &rfsMountTable[i];
	register struct fs *fsp;

        if (mp->m_bufp == NULL || mp->m_dev == NODEV)
	    continue;
	fsp = mp->m_bufp->b_un.b_fs;
	rfs_printf6(RFSD_LOG, "*CchS Dev=%X,used=%d(*%d=%d),left=%d\n", mp->m_dev,
		    rmep->rme_frags, rfsCacheFactor, rfsCacheFactor*rmep->rme_frags,
		    freespace(fsp, fsp->fs_minfree));  
	while
        (
	  (all || (rmep->rme_frags*rfsCacheFactor) > freespace(fsp, fsp->fs_minfree))
	  &&
	  rfsCachePurge(i)
	)
	    ;
    }
}



/*
 *  rfsCachePurge - purge oldest entry from remote inode cache
 *
 *  midx = mount table index of file system to purge
 *
 *  Scan the remote inode cache looking for the least recently accessed entry
 *  which is stored on the indicated local file system, is not locked and has a
 *  reference count of only 1 (i.e. will free space when released).  If such an
 *  entry is located, remove its entry from the cache, adjust the cache space
 *  in use on this file system and release its inode, and deallocate the entry
 *  to completely purge it from the cache.
 *
 *  Return: true if an entry is purged, otherwise false.
 */

boolean_t
rfsCachePurge(midx)
int midx;
{
    register struct rfsCacheEntry *oldrcep = NULL;
    register struct rfsCacheEntry **oldrcepp = NULL;
    struct fs *fsp = (&mount[midx])->m_bufp->b_un.b_fs;
    time_t oldused;
    register int i;

#ifdef	lint
    oldused = 0;
#endif	lint
    rfs_printf2(RFSD_U_TRACEIN, "<Purg midx=%d\n", midx);
    for (i=0; i<NRFSCACHE; i++)
    {
	register struct rfsCacheEntry **rcepp = &rfsCacheTable[i];
	register struct rfsCacheEntry *rcep;

	for (rcep = rfsCacheTable[i]; rcep; rcep = rcep->rce_next)
	{
	    register struct inode *ip = rcep->rce_ip;

	    if (ip->i_fs != fsp)
		continue;
	    if ((ip->i_flag&ILOCKED) || ip->i_count != 1)
	    {
		rfs_printf3(RFSD_LOG, "*Purg locked=%o,count=%d\n",
				      ip->i_flag&ILOCKED, ip->i_count);
		continue;
	    }
	    if (rcep->rce_used < oldused || oldrcepp == NULL)
	    {
		oldrcep  = rcep;
		oldrcepp = rcepp;
		oldused  = rcep->rce_used;
	    }
	    rcepp = &rcep->rce_next;
	}
    }
    if (oldrcepp)
    {
	*oldrcepp = oldrcep->rce_next;
	LOCK_INODE(oldrcep->rce_ip);
	rfsMountTable[midx].rme_frags -= btodb(oldrcep->rce_ip->i_size+DEV_BSIZE-1);
	rfs_printf4(RFSD_LOG, "*Purg rce=%X,ip=%X,frags=%d\n",
		              oldrcep, oldrcep->rce_ip, rfsMountTable[midx].rme_frags);
	FINISH_WITH_INODE(oldrcep->rce_ip);
	rfsCacheFree(oldrcep);
	rfs_printf1(RFSD_U_TRACEOUT, ">Purg T\n");
	return(TRUE);
    }
    else
    {
	rfs_printf1(RFSD_U_TRACEOUT, ">Purg F\n");
	return(FALSE);
    }
}



/*
 *  rfsInodeAllocate - allocate an inode for the local cache
 *
 *  stp    = stat information for remote inode
 *  length = length to be cached
 *  midxp  = result mount table entry of inode file system
 *
 *  Search the mounted file system table for a file system with sufficient free
 *  space to hold a file of the indicated length.  The total number of
 *  fragments used for the cache on any file system must be no greater than
 *  1/rfsCacheFactor times the total number of free fragments there.  If such a file
 *  system is located, allocate an inode on it with the mode and owners
 *  specified in the status structure.  Prefer a file system with an identical
 *  device number to any other as long as it has at least the minimum free
 *  space available.  If no file system has sufficient free space as described
 *  above, use the file system with the most absolute free space.
 *
 *  Return: the allocated inode and the mount table entry associated with its
 *  file system, or NULL if none are available.
 */
struct inode *
rfsInodeAllocate(stp, length, midxp)
    struct stat *stp;
    off_t length;
    int *midxp;
{
    register struct inode *ip = NULL;
    register struct mount *mp;
    register struct fs *fsp;
    register int midx;
    struct inode *pip;
    int frags = btodb(length+DEV_BSIZE-1);
    int mostf;
    int mostidx = -1;

#ifdef	lint
    mostf = 0;
#endif	lint
    rfs_printf3(RFSD_U_TRACEIN, "<IAlc dev=%X, length=%d\n", stp->st_dev, length);
    for (midx=0; midx < NMOUNT; midx++)
    {
	register struct rfsMountEntry *rmep;
	int ffrags;

	mp = &mount[midx];
	if (mp->m_bufp == NULL || NO_MOUNT_DEVICE(mp))
	    continue;
	fsp = MOUNT_TO_FS(mp);
	if (FS_READONLY(fsp))
	    continue;
	rmep = &rfsMountTable[midx];
	FS_FREE_SPACE(fsp, ffrags);
	if (ffrags > mostf || mostidx < 0) 
	{
	    mostidx = midx;
	    mostf   = ffrags;
        }
	rfs_printf3(RFSD_LOG, "*IAlc dev=%x, frags=%d\n", mp->m_dev, ffrags);
	/*
	 *  Check for enough free space, including any space already consumed
	 *  by other cache entries on this file system.
	 */
	if ((ffrags+rmep->rme_frags)-(frags+rmep->rme_frags)*rfsCacheFactor < 0)
	    continue;
	if (mp->m_dev == stp->st_dev)
	{
	    mostidx = midx;
	    break;
	}
    }
    rfs_printf3(RFSD_LOG, "*IAlc dev=%x, frags=%d\n", mount[mostidx].m_dev, frags);

    /*
     *  Allocate the inode on the determined target local device.
     */
    GET_ROOT_INODE(mostidx, pip);
    if (pip == NULL)
	goto out;
    ip = ialloc(pip, (ino_t)0, 0);
    FINISH_WITH_INODE(pip);
    if (ip != NULL)
    {
	MODIFY_INODE(ip);
	ip->i_mode  = IFREG+(stp->st_mode&0777);
	ip->i_header.ih_mode = IFREG;
	ip->i_uid   = stp->st_uid;
	ip->i_gid   = stp->st_gid;
	ip->i_nlink = 0;
	*midxp = mostidx;
    }
out:
    rfs_printf3(RFSD_U_TRACEOUT, ">IAlc ip=%X(%d)\n", ip, u.u_error);
    return(ip);
}



#ifdef	RFSDEBUG
/*
 *  rfsWaitTTY - wait for terminal output queue to drain
 *
 *  tp = control structure for target terminal (may be NULL)
 *
 *  If the terminal output queue size is currently at or above the high water
 *  mark, delay (by busy looping) until it drains to at least the low water
 *  mark.
 *
 *  N.B.  This routine is used only when printing debug information and cannot
 *  be invoked from interrupt context or at high IPL since the terminal output
 *  queue drains at interrupt level.
 */

void
rfsWaitTTY(tp)
register struct tty *tp;
{
    if (tp && tp->t_outq.c_cc >= TTHIWAT(tp))
	while (tp->t_outq.c_cc > TTLOWAT(tp));
}
#endif	RFSDEBUG
