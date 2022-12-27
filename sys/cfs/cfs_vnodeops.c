/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	cfs_vnodeops.c,v $
 * Revision 2.3  90/07/26  15:50:09  mrt
 * 	    Fixed fix to rename to remove .. from moved directories.
 * 	[90/06/28            dcs]
 * 
 * Revision 1.7  90/06/28  16:24:25  dcs
 * Fixed bug with moving directories, we weren't flushing .. for the moved directory.
 * 
 * Revision 1.6  90/05/31  17:01:47  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 */

#include <mach_cfs.h>
#if	NMACH_CFS

#include <sys/param.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/file.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <sys/viceioctl.h>
#include <vfs/vfs.h>
#include <vfs/vnode.h>
#include <cfs/cfs.h>
#include <cfs/cnode.h>


struct vnode *makecfsnode();
struct cnode *cfsnc_lookup();
struct cnode *cfs_find();
struct fs *igetfs();
struct inode *iget();

int cfs_open();
int cfs_close();
int cfs_rdwr();
int cfs_ioctl();
int cfs_select();
int cfs_getattr();
int cfs_setattr();
int cfs_access();
int cfs_lookup();
int cfs_create();
int cfs_remove();
int cfs_link();
int cfs_rename();
int cfs_mkdir();
int cfs_rmdir();
int cfs_readdir();
int cfs_symlink();
int cfs_readlink();
int cfs_fsync();
int cfs_inactive();
int cfs_bmap();
int cfs_strategy();
int cfs_bread();
int cfs_brelse();
int cfs_lockctl();
int cfs_fid();
int cfs_page_read();
int cfs_page_write();
int cfs_freefid();
int cfs_badop();
int cfs_noop();

struct vnodeops cfs_vnodeops = {
	cfs_open,
	cfs_close,
	cfs_rdwr,
	cfs_ioctl,
	cfs_select,
	cfs_getattr,
	cfs_setattr,
	cfs_access,
	cfs_lookup,
	cfs_create,
	cfs_remove,
	cfs_link,
	cfs_rename,
	cfs_mkdir,
	cfs_rmdir,
	cfs_readdir,
	cfs_symlink,
	cfs_readlink,
	cfs_fsync,
	cfs_inactive,
	cfs_bmap,
	cfs_strategy,
	cfs_bread,
	cfs_brelse,
	cfs_lockctl,
	cfs_fid,
	cfs_page_read,
	cfs_page_write,
	cfs_readdir,		/* read1dir */
	cfs_freefid
};

/* 
 * These flags select various performance enhancements.
 */
int cfs_intercept_rdwr = 1;    /* Set to handle read/write in the kernel */
int cfs_attr_cache  = 1;       /* Set to cache attributes in the kernel */
int cfs_symlink_cache = 1;     /* Set to cache symbolic link information */
int cfs_access_cache = 1;      /* Set to handle some access checks directly */

/*ARGSUSED*/
/* 
 * cfs_open calls Venus to return the device, inode pair of the cache
 * file holding the data. Using iget, cfs_open finds the vnode of the
 * cache file, and then opens it.
 */
int
cfs_open(vpp, flag, cred)
	register struct vnode **vpp;
	int flag;
	struct ucred *cred;
{ 
	struct cfsfidintargs oca;
	struct cfsopenreturnargs out;
	struct cnode *cp;
	struct fs *fs;
	struct inode *ip;
	int error;

	/* Check for open of control file. */
	if (IS_CTL_VP(*vpp)) {
	    if (flag & (FWRITE | FTRUNC | FCREAT | FEXCL))
		return(EACCES);

	    return(0);
	}

	cp = VTOC(*vpp);
	oca.ViceFid.Volume = cp->c_fid.Volume;
	oca.ViceFid.Vnode  = cp->c_fid.Vnode;
	oca.ViceFid.Unique = cp->c_fid.Unique;
	oca.flags = flag;
	oca.cred = *cred;

	error = cfscall((*vpp)->v_vfsp, CFS_OPEN, xdr_fid_int, &oca, 
				  xdr_cfsopenreturn, &out, cred);

	if (!error) {
		error = out.result;
		CFSDEBUG( CFS_OPEN,printf("open: dev %d inode %d result %d\n",
					out.dev,out.inode,error); )
	}

	if (error)
		return(error);

	VN_HOLD(*vpp);                

	/* This is like VFS_VGET(). */
	fs = igetfs(out.dev);
	if (fs == NULL) {
	    printf("cfs_open: igetfs(%d) returns NULL\n", out.dev);
	    return(ENXIO);
	}
	ip = iget(out.dev, fs, out.inode);
	if (ip == NULL) {
	    printf("cfs_open: iget(%d, %x, %d) returns NULL\n", out.dev, fs, out.inode);
	    return(ENOENT);
	}
	iunlock(ip);

	if (cp->c_ovp == NULL) {
	    cp->c_ovp = ITOV(ip);
	}
	else {
	    if (cp->c_ovp != ITOV(ip))
		panic("cfs_open:  cp->c_ovp != ITOV(ip)");	/* For now! -JJK */
	}

	if (flag & FWRITE) {     /* flush the attribute cached if writing the file */
	  cp->c_owrite++;
	  cp->c_flags &= ~C_VATTR;
	}

	cp->c_ocount++;
	error = VOP_OPEN(&cp->c_ovp, flag, cred); 

	/* Should we do something different if the VOP_OPEN above failed? -JJK */
	return(error);
}

/*ARGSUSED*/
/*
 * Close the cache file used for I/O and notify Venus.
 */
int
cfs_close(vp, flag, cred)
	struct vnode *vp;
	int flag;
	struct ucred *cred;
{ 
	struct cfsfidintargs oca;
	struct cnode *cp = VTOC(vp);
	int error;
	int result;

	/* Check for close of control file. */
	if (IS_CTL_VP(vp)) {
	    return(0);
	}

	VOP_CLOSE(cp->c_ovp, flag, cred);	/* Do errors matter here? */
	VN_RELE(cp->c_ovp);
	if (--cp->c_ocount == 0)
	    cp->c_ovp = NULL;

	if (flag & FWRITE)                    /* file was opened for write */
	  --cp->c_owrite;
	    
	oca.ViceFid.Volume = cp->c_fid.Volume;
	oca.ViceFid.Vnode  = cp->c_fid.Vnode;
	oca.ViceFid.Unique = cp->c_fid.Unique;
	oca.flags = flag;
	oca.cred = *cred;

	error = cfscall(vp->v_vfsp, CFS_CLOSE, xdr_fid_int, &oca, 
				   xdr_u_long, &result, cred);

	if (!error) 
		error = result;

	VN_RELE(CTOV(cp));

	CFSDEBUG(CFS_CLOSE, printf("close: result %d\n",error); )
	return(error);
}

int
cfs_rdwr(vp, uiop, rw, ioflag, cred)
	struct vnode *vp;
	struct uio *uiop;
	enum uio_rw rw;
	int ioflag;
	struct ucred *cred;
{ 
	struct cnode *cp = VTOC(vp);
	int error = 0;

	CFSDEBUG(CFS_RDWR, printf("cfs_rdwr(%d, %x, %d, %d, %d)\n", rw, uiop->uio_iov->iov_base, uiop->uio_resid, uiop->uio_offset, uiop->uio_segflg); )

	/* Check for rdwr of control object. */
	if (IS_CTL_VP(vp))
	    return(EINVAL);

	if (cfs_intercept_rdwr) {
	    /* Redirect the request to UFS. */

	    /* If file is not already open do an "internal open" on it. */
	    int opened_internally = 0;
	    if (cp->c_ovp == NULL) {
		opened_internally = 1;

		error = cfs_open(&vp, (rw == UIO_READ ? FREAD : FWRITE), cred);
		if (error) return(error);
	    }

	    /* Have UFS handle the call. */
	    CFSDEBUG(CFS_RDWR, printf("indirect rdwr: fid = (%x.%x.%x), refcnt = %d\n",cp->c_fid.Volume, cp->c_fid.Vnode, cp->c_fid.Unique, vp->v_count); )
	    error = VOP_RDWR(cp->c_ovp, uiop, rw, ioflag, cred);

	    /* Do an "internal close" if necessary. */
	    if (opened_internally)
		(void)cfs_close(vp, (rw == UIO_READ ? FREAD : FWRITE), cred);
	}
	else {
	    /* Read/Write the blocks from/to Venus. */
	    while (uiop->uio_resid > 0) {
		struct cfsrdwrargs in;
		struct cfsrdwrrtnargs out;
		char buf[VC_RDWR_BUFSIZE];
		struct iovec *iovp = uiop->uio_iov;
		unsigned count = iovp->iov_len;

		if (count == 0) {
		    uiop->uio_iov++;
		    uiop->uio_iovcnt--;
		    continue;
		}
		if (count > VC_RDWR_BUFSIZE)
		    count = VC_RDWR_BUFSIZE;

		in.cred = *cred;
		in.ViceFid.Volume = cp->c_fid.Volume;
		in.ViceFid.Vnode  = cp->c_fid.Vnode;
		in.ViceFid.Unique = cp->c_fid.Unique;
		in.rwflag = (int)rw;
		in.count = count;
		in.data = buf;
		if (rw == UIO_WRITE) bcopy(iovp->iov_base, buf, count);
		in.offset = uiop->uio_offset;
		in.ioflag = ioflag;
		out.count = count;
		out.data = buf;

		error = cfscall(CTOV(cp)->v_vfsp, CFS_RDWR, xdr_cfsrdwr, &in, 
				xdr_cfsrdwrrtn, &out, cred);
		if (!error) error = out.result;
		CFSDEBUG(CFS_RDWR, printf("cfs_rdwr(%d, %x, %d, %d, %d) returns (%d, %d)\n", rw, in.data, in.count, in.offset, uiop->uio_segflg, error, out.count); )
		  if (error) break;

		if (rw == UIO_READ) bcopy(buf, iovp->iov_base, out.count);
		iovp->iov_base += out.count;
		iovp->iov_len -= out.count;
		uiop->uio_resid -= out.count;
		uiop->uio_offset += out.count;

		/* Exit the loop if Venus R/W fewer bytes than we specified. */
		/* Maybe we should continue if ANY bytes were R/W? -JJK */
		if (out.count != in.count)
		    break;
	    }
	}

	/* Invalidate cached attributes if writing. */
	if (rw == UIO_WRITE)
	    cp->c_flags &= ~C_VATTR;

	return(error);
}

/*ARGSUSED*/
int
cfs_ioctl(vp, com, data, flag, cred)
	struct vnode *vp;
	int com;
	caddr_t data;
	int flag;
	struct ucred *cred;
{ 
	struct cfsioctlargs ioa;
	struct cfsioctlreturnargs iora;
	int error;
	struct vnode *tvp;
	struct cnode *tcp;
	register struct a {
	    char *path;
	    struct ViceIoctl vidata;
	    int follow;
	} *ap = (struct a *)data;
	char buf[VC_IOCTL_BUFSIZE];
	
	CFSDEBUG(CFS_IOCTL, printf("in cfs_ioctl\n");)

	/* Must be control object to succeed. */
	if (!IS_CTL_VP(vp))
	    return (EOPNOTSUPP);

	/* Look up the pathname. */

	/* Should we use the name cache here? It would get it from lookupname sooner or later anyway, right? */

	error = lookupname(ap->path, UIO_USERSPACE, (ap->follow ? FOLLOW_LINK : NO_FOLLOW),
			   (struct vnode **)0, &tvp);
	if (error)
	    return(error);

	if (vp->v_vfsp != tvp->v_vfsp) {
	    VN_RELE(tvp);
	    return(EINVAL);
	}

	/* Copy in the IN buffer. */
	if (ap->vidata.in_size > VC_IOCTL_BUFSIZE) {
	    VN_RELE(tvp);
	    return(EINVAL);
	}
	error = copyin(ap->vidata.in, buf, ap->vidata.in_size);
	if (error) {
	    VN_RELE(tvp);
	    return(error);
	}

	tcp = VTOC(tvp);
	ioa.ViceFid.Volume = tcp->c_fid.Volume;
	ioa.ViceFid.Vnode  = tcp->c_fid.Vnode;
	ioa.ViceFid.Unique = tcp->c_fid.Unique;
	{
	    /* Command was mutated by increasing its size field to reflect the path and follow args. */
	    /* We need to subtract that out before sending the command to Venus. */

	    int size;

	    ioa.cmd = (com & ~(IOCPARM_MASK << 16));	    /* mask out size field */
	    size = ((com >> 16) & IOCPARM_MASK) - sizeof(char *) - sizeof(int);
	    ioa.cmd |= (size & IOCPARM_MASK) <<	16;	    /* or in corrected size */
	}
	ioa.rwflag = flag;
	ioa.len = ap->vidata.in_size;
	ioa.data = buf;
	ioa.cred = *cred;
	iora.data = buf;

	error = cfscall(vp->v_vfsp, CFS_IOCTL, xdr_cfsioctl, &ioa, 
			xdr_cfsioctlreturn, &iora, cred);

	if (!error) {
	    error = iora.result;

	    CFSDEBUG(CFS_IOCTL, printf("  %d \n", iora.result); )
	}

	/* Copy out the OUT buffer. */
	if (!error) {
	    if (iora.len > ap->vidata.out_size)
		error = EINVAL;
	    else
		error = copyout(iora.data, ap->vidata.out, ap->vidata.out_size);
	}

	VN_RELE(tvp);
	return(error);
}

/*ARGSUSED*/
int
cfs_select(vp, which, cred)
	struct vnode *vp;
	int which;
	struct ucred *cred;
{
	printf("in cfs_select\n");
	return (EOPNOTSUPP);
}

/*
 * To reduce the cost of a user-level venus, we cache attributes in the kernel.
 * Each cnode has storage allocated for an attribute. If c_vattr is valid, return
 * a reference to it. Otherwise, get the attributes from venus and store them in
 * the cnode.
 */
/* There is some question if this method is a security leak. But I think that
 * in order to make this call, the user must have done a lookup and opened the file,
 * and therefore should already have access.
 */
int
cfs_getattr(vp, vap, cred)
	struct vnode *vp;
	struct vattr *vap;
	struct ucred *cred;
{ 
	struct cfsattrreturn ar;
	struct cfsfidcred fca;
	struct cnode *cp = VTOC(vp);
	int error;

	/* Check for getattr of control object. */
	if (IS_CTL_VP(vp))
	    return(ENOENT);

	/* Check to see if the attributes have already been cached */
	if (VALID_VATTR(cp)) { 
		CFSDEBUG(CFS_GETATTR, { printf("attr cache hit: (%x.%x.%x)\n",
					       cp->c_fid.Volume,
					       cp->c_fid.Vnode,
					       cp->c_fid.Unique);
				/*	print_vattr(&cp->c_vattr); */})

		*vap = cp->c_vattr;
		return(0);
	}
	  
	fca.ViceFid.Volume = cp->c_fid.Volume;
	fca.ViceFid.Vnode  = cp->c_fid.Vnode;
	fca.ViceFid.Unique = cp->c_fid.Unique;
	fca.cred = *cred;

	error = cfscall(vp->v_vfsp, CFS_GETATTR, xdr_fidcred, &fca, 
				     xdr_attr_return, &ar, cred);

	if (!error) 
		error = ar.result;

	if (!error) {
		CFSDEBUG(CFS_GETATTR, printf("getattr miss (%x.%x.%x): result %d\n",
					     cp->c_fid.Volume,
					     cp->c_fid.Vnode,
					     cp->c_fid.Unique,
					     ar.result); )
		  
		/* CFSDEBUG(CFS_GETATTR, print_vattr(&ar.attr); ) */

 	        if ((cp->c_owrite == 0) && (cfs_attr_cache)) {  /* Not open for write */
		  cp->c_vattr = ar.attr;       /* store attributes in cnode */
		  cp->c_flags |= C_VATTR;      /* Valid attributes in cnode */
		}

		*vap = ar.attr;
	}
	return(error);
}

int
cfs_setattr(vp, vap, cred)
	register struct vnode *vp;
	register struct vattr *vap;
	struct ucred *cred;
{ 
	struct cnode *cp = VTOC(vp);
	struct cfssetattrargs sa;
	int result;
	int error;

	/* Check for setattr of control object. */
	if (IS_CTL_VP(vp))
	    return(ENOENT);
	
	sa.ViceFid.Volume = cp->c_fid.Volume;
	sa.ViceFid.Vnode  = cp->c_fid.Vnode;
	sa.ViceFid.Unique = cp->c_fid.Unique;
	sa.cred = *cred;
	sa.attr = *vap;

	error = cfscall(vp->v_vfsp, CFS_SETATTR, xdr_cfssetattr, &sa, 
				     xdr_u_long, &result, cred);

	if (!error) 
		error = result;

	if (!error)
	  cp->c_flags &= ~C_VATTR;

	CFSDEBUG(CFS_SETATTR,	printf("setattr %d\n"); )
	return(error);
}

int
cfs_access(vp, mode, cred)
	struct vnode *vp;
	int mode;
	struct ucred *cred;
{ 
	struct cnode *cp = VTOC(vp);
	struct cfsfidintargs oca;
	int result;
	int error;

	/* Check for access of control object.  Only read access is allowed on it. */
	if (IS_CTL_VP(vp)) {
	    return(((mode & VREAD) && !(mode & (VWRITE | VEXEC))) ? 0 : EACCES);
	}

	/*
	 * if the file is a directory, and we are checking exec (eg lookup) 
	 * access, and the file is in the namecache, then the user must have 
	 * lookup access to it.
         */
	if (cfs_access_cache) {
	  if (((vp->v_mode&VFMT) == VDIR) && (mode & VEXEC)) {
	    if (cfsnc_lookup(cp, ".", cred))
	      return(0);                     /* it was in the cache */
	  }
	}

	oca.ViceFid.Volume = cp->c_fid.Volume;
	oca.ViceFid.Vnode  = cp->c_fid.Vnode;
	oca.ViceFid.Unique = cp->c_fid.Unique;
	oca.flags = mode;
	oca.cred = *cred;

	error = cfscall(vp->v_vfsp, CFS_ACCESS, xdr_fid_int, &oca, 
				    xdr_u_long, &result, cred);

	if (!error) 
		error = result;

	return(error);
}

int
cfs_readlink(vp, uiop, cred)
	struct vnode *vp;
	struct uio *uiop;
	struct ucred *cred;
{ 
	int error;
	struct cfsfidcred fca;
	struct cfsreadlinkrtnargs rlr;
	struct cnode *cp = VTOC(vp);
	char buf[CFS_MAXPATHLEN];

	/* Check for readlink of control object. */
	if (IS_CTL_VP(vp))
	    return(ENOENT);

	if ((cfs_symlink_cache) && (VALID_SYMLINK(cp))) { /* symlink was cached */
	  error = uiomove(cp->c_symlink, (int)cp->c_symlen,
			  UIO_READ, uiop);
	  return(error);
	}

	fca.ViceFid.Volume = cp->c_fid.Volume;
	fca.ViceFid.Vnode  = cp->c_fid.Vnode;
	fca.ViceFid.Unique = cp->c_fid.Unique;
	fca.cred = *cred;

	/* We're screwed if Venus passes back > CFS_MAXPATHLEN bytes! -JJK */
	rlr.data = buf;

	error = cfscall(vp->v_vfsp, CFS_READLINK, xdr_fidcred, &fca, 
				      xdr_cfsreadlinkrtn, &rlr, cred);

	if (!error) {
		error = rlr.result;
		if (!error) {
		  if (cfs_symlink_cache) {
		    cp->c_symlink = (char *)kalloc((u_int)rlr.count);
		    cp->c_symlen = rlr.count;
		    bcopy(rlr.data, cp->c_symlink, rlr.count);
		    cp->c_flags |= C_SYMLINK;
		  }
/* XXX - DEBUG */
		    CFSDEBUG(CFS_READLINK,
			     { char buf[CFS_MAXPATHLEN];
			       bcopy(rlr.data, buf, rlr.count);
			       buf[rlr.count] = 0;
			       printf("readlink contents = %s\n", buf); })
/* XXX - DEBUG */
		    error = uiomove(rlr.data, (int)rlr.count,
				    UIO_READ, uiop);
		}
	}

	CFSDEBUG(CFS_READLINK, printf("in readlink result %d\n",error);)

	return(error);
}

/*ARGSUSED*/
int
cfs_fsync(vp, cred)
	struct vnode *vp;
	struct ucred *cred;
{ 
	struct cfsfidcred fca;
	struct cnode *cp = VTOC(vp);
	int result;
	int error;

	/* Check for fsync of control object. */
	if (IS_CTL_VP(vp))
	    return(0);

	fca.ViceFid.Volume = cp->c_fid.Volume;
	fca.ViceFid.Vnode  = cp->c_fid.Vnode;
	fca.ViceFid.Unique = cp->c_fid.Unique;
	fca.cred = *cred;

	error = cfscall(vp->v_vfsp, CFS_FSYNC, xdr_fidcred, &fca, 
				   xdr_u_long, &result, cred);
	if (!error) 
		error = result;

	CFSDEBUG(CFS_FSYNC, printf("in fsync result %d\n",error); )
	return(error);
}

/*ARGSUSED*/
int
cfs_inactive(vp, cred)
struct vnode *vp;
struct ucred *cred;
{ 
	struct cnode *cp = VTOC(vp);
	int error = 0;

	/* We're pretty sure we don't need to send inactive to venus - DCS */
	/* Inactive message should never be sent to venus for control object! -JJK */
/*
	if (!IS_CTL_VP(vp)) {
            struct cfsfidcred fca;
            int result;

	    fca.ViceFid.Volume = cp->c_fid.Volume;
	    fca.ViceFid.Vnode  = cp->c_fid.Vnode;
	    fca.ViceFid.Unique = cp->c_fid.Unique;
	    fca.cred = *cred;

	    error = cfscall(vp->v_vfsp, CFS_INACTIVE, xdr_fidcred, &fca, 
			    xdr_u_long, &result, cred);
	    if (!error) 
		error = result;
	}
*/

	/* Even if the cfscall fails, I think we should still remove the cnode. -DCS */
	/* If an array has been allocated to hold the symbolic link, deallocate it */
	if ((cfs_symlink_cache) && (VALID_SYMLINK(cp))) {
	  if (cp->c_symlink == NULL)
	    panic("cfs_inactive: null symlink pointer in cnode");

	  kfree(cp->c_symlink, cp->c_symlen);
	  cp->c_flags &= ~C_SYMLINK;
	  cp->c_symlen = 0;
	}

	/* Remove it from the table so it can't be found. */
	cfs_unsave(cp);
	if (cp->c_ovp != NULL)
	    panic("cfs_inactive:  cp->ovp != NULL");
	((struct cfs_mntinfo *)vp->v_vfsp->vfs_data)->mi_refct--;
	cfs_free(cp);

	CFSDEBUG(CFS_INACTIVE, printf("in inactive result %d\n",error); )
	return(error);
}

/*
 * Remote file system operations having to do with directory manipulation.
 */

cfs_lookup(dvp, nm, vpp, cred)
	struct vnode *dvp;
	char *nm;
	struct vnode **vpp;
	struct ucred *cred;
{ 
	struct cnode *dcp = VTOC(dvp);
	struct cfsfidnameargs vna;
	struct cfslookupreturnargs lra;
	int error = 0;

	/* Check for lookup of control object. */
	if (IS_CTL_NAME(dvp, nm)) {
	    *vpp = vtomi(dvp)->mi_ctlvp;
	    VN_HOLD(*vpp);
	    return(0);
	}

	/* First try to look the file up in the cfs name cache */
	/* lock the parent vnode? */
	
	*vpp = (struct vnode *) cfsnc_lookup(dvp, nm, cred);
	if (*vpp) {
	  VN_HOLD(*vpp);
	} else {

	  /* The name wasn't cached, so we need to contact Venus */

	  vna.ViceFid.Volume = dcp->c_fid.Volume;
	  vna.ViceFid.Vnode  = dcp->c_fid.Vnode;
	  vna.ViceFid.Unique = dcp->c_fid.Unique;
	  vna.name = nm;
	  vna.cred = *cred;

	  error = cfscall(dvp->v_vfsp, CFS_LOOKUP, xdr_fid_name, &vna, 
				    xdr_cfslookupreturn, &lra, cred);

	  if (!error) 
		error = lra.result;

	  if (error) {
	        CFSDEBUG(CFS_LOOKUP, printf("lookup error %d\n",error);)
		*vpp = (struct vnode *)0;
	  } else {
		CFSDEBUG(CFS_LOOKUP, 
		printf("lookup: vol %x vno %x uni %x type %o result %d\n",
			lra.ViceFid.Volume,
			lra.ViceFid.Vnode,
			lra.ViceFid.Unique,
			lra.vtype,
			lra.result); )

		*vpp = makecfsnode(&lra.ViceFid, dvp->v_vfsp, lra.vtype);

		/* enter the new vnode in the Name Cache only if the top bit isn't set */
		if (!(lra.vtype & CFS_NOCACHE))
		  cfsnc_enter(dvp, nm, cred, *vpp);      
	  }
	}

	CFSDEBUG(CFS_LOOKUP, printf("lookup result %d vpp 0x%x\n",error,*vpp);)

	return(error);
}

/*ARGSUSED*/
cfs_create(dvp, nm, va, exclusive, mode, vpp, cred)
	struct vnode *dvp;
	char *nm;
	struct vattr *va;
	enum vcexcl exclusive;
	int mode;
	struct vnode **vpp;
	struct ucred *cred;
{ 
	struct cfscreateargs ca;
	struct cfsfidattrreturn fidattrreturn;
	struct cnode *dcp = VTOC(dvp);	
	int error;

	/* Check for create of control object. */
	if (IS_CTL_NAME(dvp, nm)) {
	    *vpp = (struct vnode *)0;
	    return(EACCES);
	}

	ca.ViceFid.Volume = dcp->c_fid.Volume;
	ca.ViceFid.Vnode  = dcp->c_fid.Vnode;
	ca.ViceFid.Unique = dcp->c_fid.Unique;
	ca.name = nm;
	ca.excl = exclusive;
	ca.mode = mode;
	ca.attr = *va;
	ca.cred = *cred;

	error = cfscall(dvp->v_vfsp, CFS_CREATE, xdr_cfscreate, &ca, 
				    xdr_fidattrreturn, &fidattrreturn, cred);

	if (!error) 
		error = fidattrreturn.result;

	if (!error) {

	/* If this is a exclusive create, panic if the file already exists. */
	/* Venus should have detected the file and reported EEXIST. */

	        if ((exclusive == EXCL) &&
		    (cfs_find(&fidattrreturn.ViceFid, dvp->v_vfsp) != NULL))
		    panic("cnode existed for newly created file!");

		*vpp = makecfsnode(&fidattrreturn.ViceFid, 
				dvp->v_vfsp, va->va_mode);

		if (cfs_attr_cache) {
		  VTOC(*vpp)->c_vattr = fidattrreturn.attr;  /* update the attr cache */
		  VTOC(*vpp)->c_flags |= C_VATTR;       /* Valid attributes in cnode */
		}

	/* Invalidate the parent's attr cache, the modification time has changed */
		VTOC(dvp)->c_flags &= ~C_VATTR;

		/* enter the new vnode in the Name Cache */
		cfsnc_enter(dvp, nm, cred, *vpp);

		CFSDEBUG(CFS_CREATE, 
			printf("create: (%x.%x.%x), result %d\n",
				fidattrreturn.ViceFid.Volume,
				fidattrreturn.ViceFid.Vnode,
				fidattrreturn.ViceFid.Unique,
				fidattrreturn.result); )
	}
	else {
	        *vpp = (struct vnode *)0;
		CFSDEBUG(CFS_CREATE, printf("create error %d\n",error);)
	}

	return(error);
}

cfs_remove(dvp, nm, cred)
	struct vnode *dvp;
	char *nm;
	struct ucred *cred;
{ 
	int result;
	struct cfsfidnameargs vna;
	struct cnode *cp = VTOC(dvp);
	int error;

	/* Remove the file's entry from the CFS Name Cache */
	/* We're being conservative here, it might be that this person
	 * doesn't really have sufficient access to delete the file
 	 * but we feel zapping the entry won't really hurt anyone -- dcs
	 */
	cfsnc_zapfile(dvp, nm);

	/* Invalidate the parent's attr cache, the modification time has changed */
	VTOC(dvp)->c_flags &= ~C_VATTR;

	/* Check for remove of control object. */
	if (IS_CTL_NAME(dvp, nm))
	    return(ENOENT);

	vna.ViceFid.Volume = cp->c_fid.Volume;
	vna.ViceFid.Vnode  = cp->c_fid.Vnode;
	vna.ViceFid.Unique = cp->c_fid.Unique;

	vna.name = nm;
	vna.cred = *cred;

	error = cfscall(dvp->v_vfsp, CFS_REMOVE, xdr_fid_name, &vna, 
				    xdr_u_long, &result, cred);
	if (!error) 
		error = result;

	CFSDEBUG(CFS_REMOVE,	printf("in remove result %d\n",error); )
	return(error);
}

cfs_link(vp, tdvp, tnm, cred)
	struct vnode *vp;
	struct vnode *tdvp;
	char *tnm;
	struct ucred *cred;
{ 
	struct cfslinkargs la;
	struct cnode *cp = VTOC(vp);
	struct cnode *tdcp = VTOC(tdvp);
	int result;
	int error;

	/* Check for link to/from control object. */
	if (IS_CTL_NAME(tdvp, tnm) || IS_CTL_VP(vp))
	    return(EACCES);

	la.sourceFid.Volume = cp->c_fid.Volume;
	la.sourceFid.Vnode  = cp->c_fid.Vnode;
	la.sourceFid.Unique = cp->c_fid.Unique;

	la.destFid.Volume = tdcp->c_fid.Volume;
	la.destFid.Vnode  = tdcp->c_fid.Vnode;
	la.destFid.Unique = tdcp->c_fid.Unique;

	la.tname = tnm;
	la.cred = *cred;

	error = cfscall(vp->v_vfsp, CFS_LINK, xdr_cfslink, &la, 
				  xdr_u_long, &result, cred);
	if (!error) 
		error = result;

	/* Invalidate the parent's attr cache, the modification time has changed */
	VTOC(tdvp)->c_flags &= ~C_VATTR;
	VTOC(vp)->c_flags &= ~C_VATTR;

	CFSDEBUG(CFS_LINK,	printf("in link result %d\n",error); )

	return(error);
}

cfs_rename(odvp, onm, ndvp, nnm, cred)
	struct vnode *odvp;
	char *onm;
	struct vnode *ndvp;
	char *nnm;
	struct ucred *cred;
{ 
	struct cfsrenameargs ra;
	struct cnode *odcp = VTOC(odvp);
	struct cnode *ndcp = VTOC(ndvp);
	int result;
	int error;

	/* Check for rename involving control object. */
	if (IS_CTL_NAME(odvp, onm) || IS_CTL_NAME(ndvp, nnm))
	    return(EACCES);

	/* Remove the entries for both source and target files */
	cfsnc_zapfile(odvp, onm);
	cfsnc_zapfile(ndvp, nnm);

	/* Problem with moving directories -- need to flush entry for .. */
	if (odvp != ndvp) {
	  struct vnode *ovp = CTOV( cfsnc_lookup(odvp, onm, &ovp, cred));
	  if ((ovp) &&
	      ((ovp->v_mode&VFMT) == VDIR)) /* If it's a directory */
	    cfsnc_zapfile(ovp,"..");
	}
	
	/* Invalidate the parent's attr cache, the modification time has changed */
	VTOC(odvp)->c_flags &= ~C_VATTR;
	VTOC(ndvp)->c_flags &= ~C_VATTR;

	ra.sourceFid.Volume = odcp->c_fid.Volume;
	ra.sourceFid.Vnode  = odcp->c_fid.Vnode;
	ra.sourceFid.Unique = odcp->c_fid.Unique;
	ra.srcname = onm;

	ra.destFid.Volume = ndcp->c_fid.Volume;
	ra.destFid.Vnode  = ndcp->c_fid.Vnode;
	ra.destFid.Unique = ndcp->c_fid.Unique;
	ra.destname = nnm;
	ra.cred = *cred;

	error = cfscall(odvp->v_vfsp, CFS_RENAME, xdr_cfsrename, &ra, 
				    xdr_u_long, &result, cred);

	if (!error) 
		error = result;

	CFSDEBUG(CFS_RENAME, printf("in rename result %d\n",error);)
	return(error);
}

cfs_mkdir(dvp, nm, va, vpp, cred)
	struct vnode *dvp;
	char *nm;
	register struct vattr *va;
	struct vnode **vpp;
	struct ucred *cred;
{ 
	struct cfsmkdirargs ma;
	struct cfsfidattrreturn fidattrreturn;
	struct cnode *dcp = VTOC(dvp);	
	int error;

	/* Check for mkdir of target object. */
	if (IS_CTL_NAME(dvp, nm)) {
	    *vpp = (struct vnode *)0;
	    return(EACCES);
	}

	ma.ViceFid.Volume = dcp->c_fid.Volume;
	ma.ViceFid.Vnode  = dcp->c_fid.Vnode;
	ma.ViceFid.Unique = dcp->c_fid.Unique;
	ma.name = nm;
	ma.attr = *va;
	ma.cred = *cred;

	error = cfscall(dvp->v_vfsp, CFS_MKDIR, xdr_cfsmkdir, &ma, 
				   xdr_fidattrreturn, &fidattrreturn, cred);

	if (!error) 
		error = fidattrreturn.result;

	if (!error) {
	        if (cfs_find(&fidattrreturn.ViceFid, dvp->v_vfsp) != NULL)
		    panic("cnode existed for newly created directory!");

		*vpp = makecfsnode(&fidattrreturn.ViceFid, 
					dvp->v_vfsp, va->va_mode);

		/* enter the new vnode in the Name Cache */
		cfsnc_enter(dvp, nm, cred, *vpp);

		/* as a side effect, enter "." and ".." for the directory */
		cfsnc_enter(*vpp, ".", cred, *vpp);
		cfsnc_enter(*vpp, "..", cred, dvp);

		if (cfs_attr_cache) {
		  VTOC(*vpp)->c_vattr = fidattrreturn.attr;  /* update the attr cache */
		  VTOC(*vpp)->c_flags |= C_VATTR;   /* Valid attributes in cnode */
		}

	/* Invalidate the parent's attr cache, the modification time has changed */
		VTOC(dvp)->c_flags &= ~C_VATTR;

		CFSDEBUG( CFS_MKDIR, printf("mkdir: (%x.%x.%x) result %d\n",
					fidattrreturn.ViceFid.Volume,
					fidattrreturn.ViceFid.Vnode,
					fidattrreturn.ViceFid.Unique,
					fidattrreturn.result); )
	}
	else {
	        *vpp = (struct vnode *)0;
		CFSDEBUG(CFS_MKDIR, printf("mkdir error %d\n",error);)
	}

	return(error);
}

cfs_rmdir(dvp, nm, cred)
	struct vnode *dvp;
	char *nm;
	struct ucred *cred;
{ 
	int result;
	struct cfsfidnameargs vna;
	struct cnode *dcp = VTOC(dvp);
	struct cnode *cp;
	int error;

	/* Check for rmdir of control object. */
	if (IS_CTL_NAME(dvp, nm))
	    return(ENOENT);

	/* We're being conservative here, it might be that this person
	 * doesn't really have sufficient access to delete the file
 	 * but we feel zapping the entry won't really hurt anyone -- dcs
	 */
	/*
	 * As a side effect of the rmdir, remove any entries for children of
         * the directory, especially "." and "..".
	 */
	cp = cfsnc_lookup(dcp, nm, cred);
	if (cp) cfsnc_zapParentfid(&(cp->c_fid));

	/* Remove the file's entry from the CFS Name Cache */
	cfsnc_zapfile(dcp, nm);

	/* Invalidate the parent's attr cache, the modification time has changed */
	dcp->c_flags &= ~C_VATTR;

	vna.ViceFid.Volume = dcp->c_fid.Volume;
	vna.ViceFid.Vnode  = dcp->c_fid.Vnode;
	vna.ViceFid.Unique = dcp->c_fid.Unique;

	vna.name = nm;
	vna.cred = *cred;

	error = cfscall(dvp->v_vfsp, CFS_RMDIR, xdr_fid_name, &vna, 
				   xdr_u_long, &result, cred);
	if (!error) 
		error = result;

	CFSDEBUG(CFS_RMDIR, printf("in rmdir result %d\n",error); )
	return(error);
}

cfs_symlink(tdvp, tnm, tva, lnm, cred)
	struct vnode *tdvp;
	char *tnm;
	struct vattr *tva;
	char *lnm;
	struct ucred *cred;
{ 
	struct cfssymlinkargs sa;
	struct cnode *tdcp = VTOC(tdvp);	
	int result;
	int error;

	/* Check for symlink of control object. */
	if (IS_CTL_NAME(tdvp, tnm))
	    return(EACCES);

	sa.ViceFid.Volume = tdcp->c_fid.Volume;
	sa.ViceFid.Vnode  = tdcp->c_fid.Vnode;
	sa.ViceFid.Unique = tdcp->c_fid.Unique;
	sa.srcname = lnm;
	sa.tname = tnm;
	sa.attr = *tva;
	sa.cred = *cred;

	error = cfscall(tdvp->v_vfsp, CFS_SYMLINK, xdr_cfssymlink, &sa, 
				     xdr_u_long, &result, cred);
	if (!error) 
		error = result;

	/* Invalidate the parent's attr cache, the modification time has changed */
	tdcp->c_flags &= ~C_VATTR;

	CFSDEBUG(CFS_SYMLINK, printf("in symlink result %d\n",error); )

	return(error);
}

/*
 * Read directory entries.
 */
cfs_readdir(vp, uiop, cred)
	struct vnode *vp;
	register struct uio *uiop;
	struct ucred *cred;
{ 
	struct cnode *cp = VTOC(vp);
	int error = 0;

	CFSDEBUG(CFS_READDIR, printf("cfs_readdir(%x, %d, %d, %d)\n", uiop->uio_iov->iov_base, uiop->uio_resid, uiop->uio_offset, uiop->uio_segflg); )

	/* Check for readdir of control object. */
	if (IS_CTL_VP(vp))
	    return(ENOENT);

	if (cfs_intercept_rdwr) {
	    /* Redirect the request to UFS. */

	    /* If directory is not already open do an "internal open" on it. */
	    int opened_internally = 0;
	    if (cp->c_ovp == NULL) {
		opened_internally = 1;

		error = cfs_open(&vp, FREAD, cred);
		if (error) return(error);
	    }

	    /* Have UFS handle the call. */
	    CFSDEBUG(CFS_READDIR, printf("indirect readdir: fid = (%x.%x.%x), refcnt = %d\n",cp->c_fid.Volume, cp->c_fid.Vnode, cp->c_fid.Unique, vp->v_count); )
	    error = VOP_READDIR(cp->c_ovp, uiop, cred);

	    /* Do an "internal close" if necessary. */
	    if (opened_internally)
		(void)cfs_close(vp, FREAD, cred);
	}
	else {
	    /* Read the block from Venus. */
	    struct cfsreaddirargs in;
	    struct cfsreaddirrtnargs out;
	    char buf[VC_READDIR_BUFSIZE];
	    struct iovec *iovp = uiop->uio_iov;
	    unsigned count = iovp->iov_len;

	    /* Make the count a multiple of DIRBLKSIZ (borrowed from ufs_readdir). */	
	    if ((uiop->uio_iovcnt != 1) || (count < DIRBLKSIZ) ||
		(uiop->uio_offset & (DIRBLKSIZ - 1)))
		return (EINVAL);
	    count &= ~(DIRBLKSIZ - 1);
	    uiop->uio_resid -= iovp->iov_len - count;
	    iovp->iov_len = count;
	    if (count > VC_READDIR_BUFSIZE)
		return(EINVAL);

	    in.cred = *cred;
	    in.ViceFid.Volume = cp->c_fid.Volume;
	    in.ViceFid.Vnode  = cp->c_fid.Vnode;
	    in.ViceFid.Unique = cp->c_fid.Unique;
	    in.count = count;
	    in.offset = uiop->uio_offset;
	    out.size = VC_READDIR_BUFSIZE;
	    out.data = buf;

	    error = cfscall(CTOV(cp)->v_vfsp, CFS_READDIR, xdr_cfsreaddir, &in, 
			    xdr_cfsreaddirrtn, &out, cred);
	    if (!error) error = out.result;
	    CFSDEBUG(CFS_READDIR, printf("cfs_readdir(%x, %d, %d, %d) returns (%d, %d)\n", out.data, in.count, in.offset, uiop->uio_segflg, error, out.size); )

	    if (!error) {
		bcopy(buf, iovp->iov_base, out.size);
		iovp->iov_base += out.size;
		iovp->iov_len -= out.size;
		uiop->uio_resid -= out.size;
		uiop->uio_offset += out.size;
	    }
	}

	return(error);
}

/*
 * Convert from file system blocks to device blocks
 */
int
cfs_bmap(vp, bn, vpp, bnp)
	struct vnode *vp;	/* file's vnode */
	daddr_t bn;		/* fs block number */
	struct vnode **vpp;	/* RETURN vp of device */
	daddr_t *bnp;		/* RETURN device block number */
{ 
	*vpp = (struct vnode *)0;
	printf("cfs_bmap called!\n");
	return(EINVAL);
}

struct buf *async_bufhead;
int async_daemon_count;

int
cfs_strategy(bp)
	register struct buf *bp;
{ 
	printf("cfs_strategy called!\n");
	return(EINVAL);
}

/*
 * read a logical block and return it in a buffer
 */
int
cfs_bread(vp, lbn, bpp)
    struct vnode *vp;
    daddr_t lbn;
    struct buf **bpp; 
{
    printf("cfs_bread called!\n");
    return(EINVAL);
}

/*
 * release a block returned by cfs_bread
 */
cfs_brelse(vp, bp)
    struct vnode *vp;
    struct buf *bp; {

    printf("cfs_brelse called!\n");
    return(EINVAL);
}

int
cfs_badop()
{
	panic("cfs_badop");
}

int
cfs_noop()
{
	return (EINVAL);
}

int
cfs_fid(vp, fidpp)
	struct vnode *vp;
	struct fid **fidpp;
{
	struct cfid *cfid;

	cfid = (struct cfid *)kalloc(sizeof(struct cfid));
	bzero((caddr_t)cfid, sizeof(struct cfid));
	cfid->cfid_len = sizeof(struct cfid) - (sizeof(struct fid) - MAXFIDSZ);
	cfid->cfid_fid = VTOC(vp)->c_fid;
	*fidpp = (struct fid *)cfid;
	return (0);
}

int
cfs_freefid(vp, fidp)
	struct vnode *vp;
	struct fid *fidp;
{
	kfree((struct cfid *)fidp, sizeof(struct cfid));
	return (0);
}

/*
 * Record-locking requests are passed to the local Lock-Manager daemon.
 */
int
cfs_lockctl(vp, ld, cmd, cred)
	struct vnode *vp;
	struct flock *ld;
	int cmd;
	struct ucred *cred;
{ 
	printf("cfs_lockctl called!\n");
	return(EINVAL);
}

cfs_page_read(vp, buffer, size, offset, cred)
	struct vnode	*vp;
	caddr_t		buffer;
	int		size;
	vm_offset_t	offset;
	struct ucred *cred;
{ 
	struct cnode *cp = VTOC(vp);
	struct uio uio;
	struct iovec iov;
	int error = 0;

	CFSDEBUG(CFS_RDWR, printf("cfs_page_read(%x, %d, %d), fid = (%x.%x.%x), refcnt = %d\n", buffer, size, offset, VTOC(vp)->c_fid.Volume, VTOC(vp)->c_fid.Vnode, VTOC(vp)->c_fid.Unique, vp->v_count); )

	iov.iov_base = buffer;
	iov.iov_len = size;
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;
	uio.uio_offset = offset;
	uio.uio_segflg = UIO_SYSSPACE;
	uio.uio_resid = size;
	error = cfs_rdwr(vp, &uio, UIO_READ, 0, cred);
	if (error) {
	    printf("error %d on pagein (cfs_rdwr)\n", error);
	    error = EIO;
	}

/*
	if (!error && (cp->states & CWired) == 0)
	    cfs_Wire(cp);
*/

	return(error);
}

cfs_page_write(vp, buffer, size, offset, cred, init)
	struct vnode	*vp;
	caddr_t buffer;
	int size;
	vm_offset_t	offset;
	struct ucred *cred;
	boolean_t init;
{
	struct cnode *cp = VTOC(vp);
	struct uio uio;
	struct iovec iov;
	int error = 0;

	CFSDEBUG(CFS_RDWR, printf("cfs_page_write(%x, %d, %d), fid = (%x.%x.%x), refcnt = %d\n", buffer, size, offset, VTOC(vp)->c_fid.Volume, VTOC(vp)->c_fid.Vnode, VTOC(vp)->c_fid.Unique, vp->v_count); )

	if (init) {
	    panic("cfs_page_write: called from data_initialize");
	}

	iov.iov_base = buffer;
	iov.iov_len = size;
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;
	uio.uio_offset = offset;
	uio.uio_segflg = UIO_SYSSPACE;
	uio.uio_resid = size;
	error = cfs_rdwr(vp, &uio, UIO_WRITE, 0, cred);
	if (error) {
	    printf("error %d on pageout (cfs_rdwr)\n", error);
	    error = EIO;
	}

	return(error);
}

#endif	/* NMACH_CFS */
