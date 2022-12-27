/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	cfs_subr.c,v $
 * Revision 2.3  90/08/10  10:23:20  mrt
 * 	Removed include of vm/vm_page.h as it no longer exists.
 * 	[90/08/10            mrt]
 * 
 * Revision 2.2  90/07/05  11:26:35  mrt
 * 	Initialize name cache on first call to vcopen.
 * 	[90/05/23            dcs]
 * 
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.5  90/05/31  17:01:35  dcs
 * Prepare for merge with facilities kernel.
 * 
 * Revision 1.2  90/03/19  15:56:25  dcs
 * Initialize name cache on first call to vcopen.
 * 
 * Revision 1.1  90/03/15  10:43:26  jjk
 * Initial revision
 * 
 */ 

/* @(#)cfs_subr.c	1.5 87/09/14 3.2/4.3CFSSRC */

#include <mach_cfs.h>
#if	NMACH_CFS

#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <vfs/vfs.h>
#include <vfs/vnode.h>
#include <kern/mfs.h>
#include <cfs/cfs.h>
#include <cfs/cnode.h>
#include <cfs/cfsio.h>

struct cnode *cfs_alloc();
struct cnode *cfs_find();

/*
 * Statistics
 */
struct {
	int	ncalls;			/* client requests */
	int	nbadcalls;		/* rpc failures */
	int	reqs[32];		/* count of each request */
} cfs_clstat;

/*
 *   There are 4 cases in which entries should be invalidated:
 *
 *	1. Specific Fid, Specific Cred	-- User lost rights to that file
 *	2. Specific Fid, All Creds	-- Callback on that file
 *	3. All Fids, Specific Cred	-- User has lost tokens
 *	4. All Fids, All Creds		-- Flush the CFS Name Cache
 *
 * A uid == -1 indicates that all creds should be considered, and a 
 * fid.volume == 0 indicates that all fids should be flushed.
 *
 * Is the first case ever used? jjk will look into this. -- dcs
 *
 * This is kept around for backwards compatability. See vcwrite for current
 * invalidation code -- dcs.
 */

int
cfs_invalidate(fid, cred)
	ViceFid *fid;
	struct ucred *cred;
{
	CFSDEBUG(CFS_INVALIDATE, printf("invalidate: fid = (%x.%x.%x), uid = %d\n",
					fid->Volume, fid->Vnode, fid->Unique, cred->cr_uid); )

	if (fid->Volume == 0) {
	    if (cred->cr_uid == -1) {
	      cfsnc_flush();		/* Case 4 */
	    } else {
		cfsnc_purge_user(cred);	/* Case 3 */
	    }
	} else {
	    if (cred->cr_uid == (uid_t) -1) {
	       	cfsnc_zapParentfid(fid);	/* Case 2 */
	    } else {
		cfsnc_zapvnode(fid,cred);	/* Case 1 */
	    }
	}
	return(0);
}


int
cfscall(vfsp, which, xdrargs, argsp, xdrres, resp, cred)
	struct vfs *vfsp;
	int	 which;
	xdrproc_t xdrargs;
	caddr_t	argsp;
	xdrproc_t xdrres;
	caddr_t	resp;
	struct ucred *cred;
{
	struct vcomm *vcp = &vftomi(vfsp)->mi_vcomm;
	struct vmsg vm;
	int error = 0;
	label_t qsave;

	cfs_clstat.ncalls++;
	cfs_clstat.reqs[which]++;

	if (!VC_OPEN(vcp))
	    return(ENODEV);

	/* Format the request message. */
	bzero((caddr_t)&vm, sizeof(struct vmsg));
	vm.vm_seq = ++vcp->vc_seq;
	vm.vm_opcode = which;
	vm.vm_xdrargs = xdrargs;
	vm.vm_argsp = argsp;
	vm.vm_xdrres = xdrres;
	vm.vm_resp = resp;

	/* Append msg to request queue and poke Venus. */
	insque(&vm, vcp->vc_requests[1]);
	if (vcp->vc_selproc)
	    selwakeup(vcp->vc_selproc, 0);

	/* We can be interrupted while we wait for Venus to process our request. */
	/* If the interrupt occurs before Venus has read the request, we dequeue and return. */
	/* If it occurs after the read but before the reply, we dequeue, send a signal message, and return. */
	/* If it occurs after the reply we ignore it. */
	/* In no case do we want to restart the syscall. */
	qsave = u.u_qsave;
	if (setjmp(&u.u_qsave) == 0) {
	    sleep(&vm.vm_wchan, PSLEP - 1);

	    /* We were woken up either by vcwrite() or vcclose(). */
	    remque(&vm);

	    /* If it was vcclose() and we haven't received the reply yet, then return ENODEV. */
	    if (!VC_OPEN(vcp) && !(vm.vm_flags & VM_WRITE)) {
		error = ENODEV;
		goto exit;
	    }

	    /* Otherwise just return. */
	}
	else {
	    remque(&vm);
	    if (cfsdebug)
		printf("interrupted cfscall: op = %d, seq = %d, flags = %x\n",
		       vm.vm_opcode, vm.vm_seq, vm.vm_flags);

	    /* Request was not yet read by Venus.  Pretend it was never issued. */
	    if (!(vm.vm_flags & VM_READ)) {
		error = EINTR;
		goto exit;
	    }

	    /* Request was read, but reply not yet received. */
	    if (!(vm.vm_flags & VM_WRITE)) {
		/* Send Venus a signal (if it's still around). */
		if (VC_OPEN(vcp)) {
		    struct vmsg *svmp = (struct vmsg *)kalloc((u_int)sizeof(struct vmsg));
		    bzero(svmp, sizeof(struct vmsg));
		    svmp->vm_seq = vm.vm_seq;
		    svmp->vm_opcode = CFS_SIGNAL;
		    svmp->vm_xdrargs = xdr_void;
		    svmp->vm_argsp = 0;
		    svmp->vm_xdrres = xdr_void;
		    svmp->vm_resp = 0;

		    if (cfsdebug)
			printf("cfscall: enqueing signal msg (%d, %d)\n",
			       svmp->vm_opcode, svmp->vm_seq);
		    insque(svmp, vcp->vc_requests[0]);	/* insert at head of queue! */
		    if (vcp->vc_selproc)
			selwakeup(vcp->vc_selproc, 0);
		}

		error = EINTR;
		goto exit;
	    }

	    /* Reply was received; ignore interrupt. */
	}

exit:
	u.u_qsave = qsave;
	return(error);
}


/* ************************************************** */

/* These routines are the device entry points for Venus. */

extern int cfsnc_initialized;    /* Set if cache has been initialized */

int
vcopen(dev)
	dev_t dev;
{
	register struct vcomm *vcp;

	if (minor(dev) >= NMACH_CFS || minor(dev) < 0)
	    return(ENXIO);

	if (!cfsnc_initialized)
	  cfsnc_init();

	vcp = &cfs_mnttbl[minor(dev)].mi_vcomm;
	if (VC_OPEN(vcp))
	    return(EBUSY);

	vcp->vc_selproc = 0;
	vcp->vc_requests[0] = (struct vmsg *)vcp->vc_requests;
	vcp->vc_requests[1] = (struct vmsg *)vcp->vc_requests;
	vcp->vc_replys[0] = (struct vmsg *)vcp->vc_replys;
	vcp->vc_replys[1] = (struct vmsg *)vcp->vc_replys;
	vcp->vc_buf = kalloc((u_int)VC_MAXMSGSIZE);

	return(0);
}


int
vcclose(dev)
	dev_t dev;
{
	register struct vcomm *	vcp = &cfs_mnttbl[minor(dev)].mi_vcomm;
	register struct vmsg *vmp;

	if (!VC_OPEN(vcp))
	    panic("vcclose: not open");

	kfree(vcp->vc_buf, (u_int)VC_MAXMSGSIZE);
	vcp->vc_buf = 0;

	/* Wakeup clients so they can return. */
	for (vmp = vcp->vc_requests[0]; vmp != (struct vmsg *)vcp->vc_requests; vmp = vmp->vm_chain[0]) {
	    /* Free signal request messages, and don't wakeup cause no one is waiting. */
	    if (vmp->vm_opcode == CFS_SIGNAL) {
		kfree((caddr_t)vmp, (u_int)sizeof(struct vmsg));
		continue;
	    }

	    wakeup(&vmp->vm_wchan);
	}
	for (vmp = vcp->vc_replys[0]; vmp != (struct vmsg *)vcp->vc_replys; vmp = vmp->vm_chain[0])
	    wakeup(&vmp->vm_wchan);
}


int
vcread(dev, uiop)
	dev_t dev;
	struct uio *uiop;
{
	register struct vcomm *	vcp = &cfs_mnttbl[minor(dev)].mi_vcomm;
	register struct vmsg *vmp;
	int error = 0;
	XDR inxdr;

	/* Get message at head of request queue. */
	if ((vmp = vcp->vc_requests[0]) == (struct vmsg *)vcp->vc_requests)
	    return(0);

	/* Gather the prefix + args into vc_buf, and then move the whole thing into user space. */
	xdrmem_create(&inxdr, vcp->vc_buf, VC_MAXMSGSIZE, XDR_ENCODE);
	if (!xdr_u_long(&inxdr, &vmp->vm_seq) ||
	    !xdr_u_long(&inxdr, &vmp->vm_opcode) ||
	    !(*vmp->vm_xdrargs)(&inxdr, vmp->vm_argsp))
	    panic("vcread: encode message");
	error = uiomove(vcp->vc_buf, XDR_GETPOS(&inxdr), UIO_READ, uiop);
	if (error) {
	    printf("vcread: error (%d) on uiomove\n", error);
	    error = EINVAL;
	}

	remque(vmp);

	/* If request was a signal, free up the message and don't enqueue it in the reply queue. */
	if (vmp->vm_opcode == CFS_SIGNAL) {
	    if (cfsdebug)
		printf("vcread: signal msg (%d, %d)\n",
		       vmp->vm_opcode, vmp->vm_seq);

	    kfree((caddr_t)vmp, (u_int)sizeof(struct vmsg));
	    return(error);
	}

	vmp->vm_flags |= VM_READ;
	insque(vmp, vcp->vc_replys[1]);

	return(error);
}

#define ODD(vnode)        ((vnode) & 0x1)

int
vcwrite(dev, uiop)
	dev_t dev;
	struct uio *uiop;
{
	register struct vcomm *	vcp = &cfs_mnttbl[minor(dev)].mi_vcomm;
	register struct vmsg *vmp;
	u_long seq;
	u_long opcode;
	int error = 0;
	XDR outxdr;

	/* Move the message from user space into vc_buf. */
	error = uiomove(vcp->vc_buf, VC_MAXMSGSIZE, UIO_WRITE, uiop);
	if (error) {
	    printf("vcwrite: error (%d) on uiomove\n", error);
	    return(EINVAL);
	}

	/* XDR message prefix into {seq, opcode}. */
	xdrmem_create(&outxdr, vcp->vc_buf, VC_MAXMSGSIZE, XDR_DECODE);
	if (!xdr_u_long(&outxdr, &seq) ||
	    !xdr_u_long(&outxdr, &opcode))
	    panic("vcwrite: decode prefix");

/*
 * There are 5 cases where invalidations occur. The semantics of each is listed here.
 * CFS_FLUSH     -- flush all entries from the name cache and the cnode cache.
 * CFS_PURGEUSER -- flush all entries from the name cache for a specific user
 *                  This call is a result of token expiration.
 *
 * The next two are the result of callbacks on a file or directory.
 * CFS_ZAPDIR    -- flush the attributes for the dir from its cnode.
 *                  Zap all children of this directory from the namecache.
 * CFS_ZAPFILE   -- flush the attributes for a file.
 *
 * The fifth is a result of Venus detecting an inconsistent file.
 * CFS_PURGEFID  -- flush the attribute for the file
 *                  If it is a dir (odd vnode), purge its children from the namecache
 *                  remove the file from the namecache.
 */
	/* Handle invalidate requests. */
	switch (opcode) {
	  case CFS_INVALIDATE : {                /* For backwards compatability */
	    struct cfsfidcred iv_args;

	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_INVALIDATE]++;

	    if (!xdr_fidcred(&outxdr, &iv_args))
		panic("vcwrite: decode data (invalidate)");

	    cfs_invalidate(&iv_args.ViceFid, &iv_args.cred);
	    return(0);
	  }

	  case CFS_FLUSH : {
	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_FLUSH]++;

	    cfsnc_flush();
	    CFSDEBUG(CFS_FLUSH,cfs_testflush();)    /* print any remaining cnodes */
	    return(0);
	  }

	  case CFS_PURGEUSER : {
	    struct ucred cred;

	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_PURGEUSER]++;
	    if (!xdr_cred(&outxdr, &cred))
	      panic("vcwrite: decode data (purgeuser)");

	    cfsnc_purge_user(&cred);
	    return(0);
	  }

	  case CFS_ZAPFILE : {
	    ViceFid CodaFid;
	    struct cnode *cp;
	    int error = 0;

	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_ZAPFILE]++;
	    if (!xdr_fid(&outxdr, &CodaFid))
	      panic("vcwrite: decode data (zap)");

	    cp = cfs_find(&CodaFid);
	    if (cp != NULL) {
	      VN_HOLD(CTOV(cp));

	      cp->c_flags &= ~C_VATTR;
	      if ((CTOV(cp)->v_flag & VTEXT) && !(inode_uncache_try(VTOI(CTOV(cp)))))
		  error = ETXTBSY;

	      CFSDEBUG(CFS_ZAPFILE, printf("zapfile: fid = (%x.%x.%x), refcnt = %d, error = %d\n",cp->c_fid.Volume, cp->c_fid.Vnode, cp->c_fid.Unique, CTOV(cp)->v_count - 1, error);)

	      VN_RELE(CTOV(cp));
	    }

	    return(error);
	  }

	  case CFS_ZAPDIR : {
	    ViceFid CodaFid;
	    struct cnode *cp;

	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_ZAPDIR]++;
	    if (!xdr_fid(&outxdr, &CodaFid))
	      panic("vcwrite: decode data (zapparent)");

	    cp = cfs_find(&CodaFid);
	    if (cp != NULL) {
	      VN_HOLD(CTOV(cp));

	      cp->c_flags &= ~C_VATTR;
	      cfsnc_zapParentfid(&CodaFid);     

	      CFSDEBUG(CFS_ZAPDIR, printf("zapdir: fid = (%x.%x.%x), refcnt = %d\n",cp->c_fid.Volume, cp->c_fid.Vnode, cp->c_fid.Unique, CTOV(cp)->v_count - 1);)

	      VN_RELE(CTOV(cp));
	    }

	    return(0);
	  }

	  case CFS_ZAPVNODE : {
	    struct cfsfidcred zv_args;
	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_ZAPVNODE]++;
	    if (!xdr_fidcred(&outxdr, &zv_args))
		panic("vcwrite: decode data (zapvnode)");

	    cfsnc_zapvnode(&zv_args.ViceFid, &zv_args.cred);
	    return(0);
	  }	

	  case CFS_PURGEFID : {
	    ViceFid CodaFid;
	    struct cnode *cp;
	    int error = 0;

	    cfs_clstat.ncalls++;
	    cfs_clstat.reqs[CFS_PURGEFID]++;

	    if (!xdr_fid(&outxdr, &CodaFid))
	      panic("vcwrite: decode data (purgefid)");

	    cp = cfs_find(&CodaFid);
	    if (cp != NULL) {
	      VN_HOLD(CTOV(cp));

	      if (ODD(CodaFid.Vnode)) {   /* Vnode is a directory */
		cfsnc_zapParentfid(&CodaFid);     
	      }
	    
	      cp->c_flags &= ~C_VATTR;
	      cfsnc_zapfid(&CodaFid);
	      if (!(ODD(CodaFid.Vnode)) && (CTOV(cp)->v_flag & VTEXT) && !(inode_uncache_try(VTOI(CTOV(cp)))))
		  error = ETXTBSY;

	      CFSDEBUG(CFS_PURGEFID, printf("purgefid: fid = (%x.%x.%x), refcnt = %d, error = %d\n",cp->c_fid.Volume, cp->c_fid.Vnode, cp->c_fid.Unique, CTOV(cp)->v_count - 1, error);)

	      VN_RELE(CTOV(cp));
	    }
	    return(error);
	  }
	}

	/* Look for the message on the (waiting for) reply queue. */
	for (vmp = vcp->vc_replys[0]; vmp != (struct vmsg *)vcp->vc_replys; vmp = vmp->vm_chain[0])
	    if (vmp->vm_seq == seq) break;
	if (vmp == (struct vmsg *)vcp->vc_replys) {
	    if (cfsdebug)
		printf("vcwrite: msg (%d, %d) not found\n",
		      opcode, seq);

	    return(ESRCH);
	}

	/* XDR data into resp. */
	if (!(*vmp->vm_xdrres)(&outxdr, vmp->vm_resp))
	    panic("vcwrite: decode data");

	vmp->vm_flags |= VM_WRITE;
	wakeup(&vmp->vm_wchan);

	return(0);
}

extern int cfsnc_use;

int
vcioctl(dev,cmd,addr,flag)
     dev_t dev;
     caddr_t addr;
{
  struct cfs_resize *data = (struct cfs_resize *)addr;

  switch(cmd) {
    case CFSRESIZE :
      return(cfsnc_resize(data->hashsize, data->heapsize));

    case CFSSTATS :
      if (cfsnc_use) {
        cfsnc_gather_stats();
        return(0);
      } else {
	return(ENODEV);
      }

    case CFSPRINT :
      if (cfsnc_use) {
	      print_cfsnc();
	      return(0);
      } else {
	return(ENODEV);
      }

    default :
      return(EINVAL);
    }
}

int
vcselect(dev, flag)
	dev_t dev;
	int flag;
{
	register struct vcomm *	vcp = &cfs_mnttbl[minor(dev)].mi_vcomm;

	if (flag != FREAD)
	    return(0);

	if (vcp->vc_requests[0] != (struct vmsg *)vcp->vc_requests)
	    return(1);

	vcp->vc_selproc = (struct proc *)current_thread();
	return(0);
}


/* ************************************************** */

struct cnode *cfs_freelist = NULL;
int cfs_reuse = 0;
int cfs_new = 0;
int cfs_active = 0;

/*
 * Return a vnode for the given fid.
 * If no cnode exists for this fid create one and put it
 * in a table hashed by fid.Volume and fid.Vnode.  If the cnode for
 * this fid is already in the table return it (ref count is
 * incremented by cfs_find.  The cnode will be flushed from the
 * table when cfs_inactive calls cfs_unsave.
 */
struct vnode *
makecfsnode(fid, vfsp, type)
	ViceFid *fid;
	struct vfs *vfsp;
	short	type;
{
	struct cnode *cp;

	if ((cp = cfs_find(fid)) == NULL) {
	    struct vnode *vp;
	    struct vm_info *vitmp;

	    cp = cfs_alloc();
	    cp->c_fid = *fid;

	    vp = CTOV(cp);
	    vitmp = vp->v_vm_info;
	    VN_INIT(vp, vfsp, type, 0);
	    vp->v_vm_info = vitmp;
	    vp->v_vm_info->pager = MEMORY_OBJECT_NULL;
	    vp->v_type = ITYPE_CFS;

	    cfs_save(cp);
	    ((struct cfs_mntinfo *)(vfsp->vfs_data))->mi_refct++;
	} else {
	    VN_HOLD(CTOV(cp));
	}

        return(CTOV(cp));
}

/*
 * Cnode lookup stuff.
 * These routines maintain a table of cnodes hashed by fid so
 * that the cnode for a fid can be found if it already exists.
 * NOTE: CFS_CACHESIZE must be a power of 2 for cfshash to work!
 */

#define CFS_CACHESIZE 512
struct cnode *cfs_cache[CFS_CACHESIZE];

#define cfshash(fid) \
    (((fid)->Volume + (fid)->Vnode) & (CFS_CACHESIZE-1))

/*
 * As a debugging measure, print out any cnodes that lived through a name cache flush.
 */
cfs_testflush()
{
        int hash;
	struct cnode *cp;

	for (hash = 0; hash < CFS_CACHESIZE; hash++) {
	  for (cp = cfs_cache[hash];
	       cp != NULL;
	       cp = CNODE_NEXT(cp)) {  
	    printf("Live cnode fid %8x-%8x-%8x\n",
		   (cp->c_fid).Volume,(cp->c_fid).Vnode,(cp->c_fid).Unique);
	  }
	}
}

/*
 * Put a cnode in the hash table
 */
cfs_save(cp)
	struct cnode *cp;
{
	CTOV(cp)->v_next = CTOV(cfs_cache[cfshash(&cp->c_fid)]);
	cfs_cache[cfshash(&cp->c_fid)] = cp;
}

/*
 * Remove a cnode from the hash table
 */
cfs_unsave(cp)
	struct cnode *cp;
{
	struct cnode *ptr;
	struct cnode *ptrprev = NULL;
	 
	ptr = cfs_cache[cfshash(&cp->c_fid)]; 
	while (ptr != NULL) { 
		if (ptr == cp) { 
			if (ptrprev == NULL) {
				cfs_cache[cfshash(&cp->c_fid)] = CNODE_NEXT(ptr);
			} else {
				CTOV(ptrprev)->v_next = CTOV(CNODE_NEXT(ptr));
			}
			CTOV(cp)->v_next = (struct vnode *)NULL;
			
			return; 
		}	
		ptrprev = ptr;
		ptr = CNODE_NEXT(ptr);
	}	
}

/*
 * Allocate a cnode.
 */
struct cnode *
cfs_alloc()
{
    struct cnode *cp;
    struct vm_info *vitmp;

    if (cfs_freelist) {
	cp = cfs_freelist;
	cfs_freelist = CNODE_NEXT(cp);
	cfs_reuse++;
    }
    else {
	cp = (struct cnode *)kalloc((u_int)sizeof(struct cnode));
	CTOV(cp)->v_vm_info = VM_INFO_NULL;
	vm_info_init(CTOV(cp));
	cfs_new++;
    }
    vitmp = CTOV(cp)->v_vm_info;
    CN_INIT(cp);
    CTOV(cp)->v_vm_info = vitmp;

    return(cp);
}

/*
 * Deallocate a cnode.
 */
cfs_free(cp)
	register struct cnode *cp;
{
	CTOV(cp)->v_next = CTOV(cfs_freelist);
	cfs_freelist = cp;
}

/*
 * Lookup a cnode by fid
 */
struct cnode *
cfs_find(fid)
ViceFid *fid;
{
    struct cnode *cp;

    cp = cfs_cache[cfshash(fid)];
    while (cp) {
	if ((cp->c_fid.Vnode == fid->Vnode) &&
	    (cp->c_fid.Volume == fid->Volume) &&
	    (cp->c_fid.Unique == fid->Unique)) {
	    cfs_active++;
	    return(cp); 
	}		    
	cp = CNODE_NEXT(cp);
    }
    return(NULL);
}

/*
 * Utilities used by both client and server
 * Standard levels:
 * 0) no debugging
 * 1) hard failures
 * 2) soft failures
 * 3) current test software
 * 4) main procedure entry points
 * 5) main procedure exit points
 * 6) utility procedure entry points
 * 7) utility procedure exit points
 * 8) obscure procedure entry points
 * 9) obscure procedure exit points
 * 10) random stuff
 * 11) all <= 1
 * 12) all <= 2
 * 13) all <= 3
 * ...
 */

/*VARARGS2*/
dprint(var, level, str, a1, a2, a3, a4, a5, a6, a7, a8, a9)
	int var;
	int level;
	char *str;
	int a1, a2, a3, a4, a5, a6, a7, a8, a9;
{
	if (var == level || (var > 10 && (var - 10) >= level))
		printf(str, a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

print_vattr( attr )
	struct vattr *attr;
{
	printf("getattr: mode %d uid %d gid %d fsid %d rdev %d\n",
		attr->va_mode, attr->va_uid,
		attr->va_gid, attr->va_fsid, attr->va_rdev);

	printf("	nodeid %d nlink %d size %d blocksize %d blocks %d\n",
		attr->va_nodeid, attr->va_nlink, attr->va_size,
		attr->va_blocksize,attr->va_blocks);

	printf("	atime sec %d usec %d",
		attr->va_atime.tv_sec, attr->va_atime.tv_usec);
	printf("	mtime sec %d usec %d",
		attr->va_mtime.tv_sec, attr->va_mtime.tv_usec);
	printf("	ctime sec %d usec %d\n",
		attr->va_ctime.tv_sec, attr->va_ctime.tv_usec);
}

#endif	/* NMACH_CFS */
