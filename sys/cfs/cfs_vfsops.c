/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	cfs_vfsops.c,v $
 * Revision 2.2  90/07/05  11:26:40  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.3  90/05/31  17:01:42  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 */ 

#include <mach_cfs.h>
#if	NMACH_CFS

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/systm.h>
#include <sys/types.h>
#include <vfs/vfs.h>
#include <vfs/vnode.h>
#include <cfs/cfs.h>
#include <cfs/cnode.h>

int cfsdebug = 0;
struct vnode *makecfsnode();


/*
 * cfs vfs operations.
 */
int cfs_mount();
int cfs_unmount();
int cfs_root();
int cfs_statfs();
int cfs_sync();
int cfs_vget();
extern int cfs_badop();
extern int cfsnc_initialized;     /* Set if cache has been initialized */

extern struct cdevsw cdevsw[];    /* For sanity check in cfs_mount */
extern int vcopen();

struct vfsops cfs_vfsops = {
	cfs_mount,
	cfs_unmount,
	cfs_root,
	cfs_statfs,
	cfs_sync,
	cfs_vget
};

print_cred(cred)
	struct ucred *cred;
{
	int i;

	printf("ref %d uid %d ruid %d gid %d rgid %d pag %d\n",
		cred->id_ref,cred->id_uid,cred->id_ruid,
		cred->id_gid,cred->id_rgid,cred->id_pag);

	for (i=0; i < 16; i++)
		printf("%d groups %d ",i,cred->id_groups[i]);
	printf("\n");
}

/*
 * cfs mount vfsop
 * Set up mount info record and attach it to vfs struct.
 */
/*ARGSUSED*/
cfs_mount(vfsp, path, data)
	struct vfs *vfsp;
	char *path;
	caddr_t data;
{
	struct vnode *dvp;
	dev_t dev;
	struct cfs_mntinfo *mi;
	struct vnode *rootvp;
	ViceFid rootfid;
	struct vnode *ctlvp;
	ViceFid ctlfid;
	int error;
	
	if (CFS_MOUNTED(vfsp))
		return(EBUSY);

	if (!cfsnc_initialized)
	  cfsnc_init();

	/* Validate mount device.  Similar to getmdev(). */
	error = lookupname(data, UIO_USERSPACE, FOLLOW_LINK,
			   (struct vnode **)0, &dvp);
	if (error)
		return (error);
	if ((dvp->v_mode & VFMT) != VCHR) {
		VN_RELE(dvp);
		return(ENXIO);
	}
	dev = dvp->v_rdev;
	VN_RELE(dvp);
	if (major(dev) >= nchrdev || major(dev) < 0)
		return(ENXIO);

	/*
 	 * See if the device table matches our expectations.
	 */
	if (cdevsw[major(dev)].d_open != vcopen)
		return(ENXIO);

	if (minor(dev) >= NMACH_CFS || minor(dev) < 0)
		return(ENXIO);

	/*
	 * Initialize the mount record and link it to the vfs struct
	 */
	mi = &cfs_mnttbl[minor(dev)];
	mi->mi_rootvp = 0;
	mi->mi_refct = 0;
	mi->mi_ctlvp = 0;
	/* No initialization (here) of mi_vcomm! */
	vfsp->vfs_data = (caddr_t)mi;
	vfsp->vfs_fsid.val[0] = 0;
	vfsp->vfs_fsid.val[1] = MOUNT_CFS;

	/*
	 * Make a root vnode to placate the Vnode interface, but don't
	 * actually make the CFS_ROOT call to venus until the first call
	 * to cfs_root in case a server is down while venus is starting.
	 */
	rootfid.Volume = 0;
	rootfid.Vnode = 0;
	rootfid.Unique = 0;
	rootvp = makecfsnode(&rootfid, vfsp, VDIR);
	rootvp->v_flag |= VROOT;
	mi->mi_rootvp = rootvp;

	/* set filesystem block size */
	vfsp->vfs_bsize	= 8192;	    /* XXX -JJK */

	/* make the control vnode and link it to the mount record */
	ctlfid.Volume = CTL_VOL;
	ctlfid.Vnode = CTL_VNO;
	ctlfid.Unique = CTL_UNI;
	ctlvp = makecfsnode(&ctlfid, vfsp, VCHR);
	mi->mi_ctlvp = ctlvp;

	return(error);
}

cfs_unmount(vfsp)
	struct vfs *vfsp;
{
        struct cfs_mntinfo *mi;

	if (!CFS_MOUNTED(vfsp))
	    return(EINVAL);

	cfsnc_flush();			    /* flush all entries to
					       bring the ref count down */
	mi = vftomi(vfsp);
	if (mi->mi_refct != 2 ||
	    mi->mi_rootvp->v_count != 1 ||
	    mi->mi_ctlvp->v_count != 1)
	    return (EBUSY);

	VN_RELE(mi->mi_rootvp);
	VN_RELE(mi->mi_ctlvp);
	/* No de-initialization (here) of mi_vcomm! */
	vfsp->vfs_data = 0;

	return(0);
}

/*
 * find root of cfs
 */
int
cfs_root(vfsp, vpp)
	struct vfs *vfsp;
	struct vnode **vpp;
{
        struct cfs_mntinfo *mi = vftomi(vfsp);
	struct vnode *rvp = mi->mi_rootvp;
        struct cnode *rcp = VTOC(rvp);

	/*
	 * If root vnode's ViceFid is (0,0,0) it isn't yet valid.
	 * Get the rootfid from Venus and plug it into the vnode.
	 */

	if (rcp->c_fid.Volume == 0 || 
	    rcp->c_fid.Vnode  == 0 ||
	    rcp->c_fid.Unique == 0) {
	    struct ucred cred;
	    struct cfsfidreturn fra;
	    int error;

	    cred = *u.u_cred;

	    error = cfscall(vfsp, CFS_ROOT, xdr_cred, &cred, 
			    xdr_fidreturn, &fra, u.u_cred);
	    if (!error)
		error = fra.result;

	    /*
	     * Save the new rootfid in the cnode, and rehash the cnode into the
	     * cnode hash with the new fid key.
	     */

	    if (!error) {
	        cfs_unsave(rcp);
		rcp->c_fid = fra.ViceFid;
		cfs_save(rcp);
	      }

	    /* Gross hack here! */
	    /*
 	     * If Venus fails to respond to the CFS_ROOT call, cfscall returns
	     * ENODEV. Return the uninitialized root vnode to allow vfs
	     * operations such as unmount to continue. Without this hack,
             * there is no way to do an unmount if Venus dies before a 
	     * successful CFS_ROOT call is done. All vnode operations 
	     * will fail.
	     */
	    if (error && error != ENODEV) {
		CFSDEBUG( CFS_ROOT, printf("error %d in CFS_ROOT\n", error); )

		return(error);
	    }
	}

	*vpp = rvp;
	VN_HOLD(*vpp);
	return(0);
}

/*
 * Get file system statistics.
 */
int
cfs_statfs(vfsp, sbp)
register struct vfs *vfsp;
struct statfs *sbp;
{
    sbp->f_type = 0;
    sbp->f_bsize = 8192;	/* XXX -JJK */
    sbp->f_blocks = -1;
    sbp->f_bfree = -1;
    sbp->f_bavail = -1;
    sbp->f_files = -1;
    sbp->f_ffree = -1;
    bcopy((caddr_t)&vfsp->vfs_fsid, (caddr_t)&sbp->f_fsid, sizeof (fsid_t));

    return(0);
}

/*
 * Flush any pending I/O.
 */
int
cfs_sync(vfsp)
	struct vfs *vfsp;
{
    return(0);
}


int
cfs_vget(vfsp, vpp, fidp)
	struct vfs *vfsp;
	struct vnode **vpp;
	struct fid *fidp;
{
	struct cfid *cfid = (struct cfid *)fidp;
	struct cnode *cp = 0;
	struct cfsfidcred fca;
	struct cfslookupreturnargs lra;
	int error;

	/* Check for vget of control object. */
	if (IS_CTL_FID(cfid)) {
	    *vpp = vftomi(vfsp)->mi_ctlvp;
	    VN_HOLD(*vpp);
	    return(0);
	}

	fca.ViceFid = cfid->cfid_fid;
	fca.cred = *u.u_cred;

	error = cfscall(vfsp, CFS_VGET, xdr_fidcred, &fca, 
			xdr_cfslookupreturn, &lra, u.u_cred);

	if (!error) 
	    error = lra.result;

	if (error) {
	    CFSDEBUG(CFS_VGET, printf("vget error %d\n",error);)
	      *vpp = (struct vnode *)0;
	}
	else {
	    CFSDEBUG(CFS_VGET, 
		     printf("vget: vol %u vno %d uni %d type %d result %d\n",
			    lra.ViceFid.Volume,
			    lra.ViceFid.Vnode,
			    lra.ViceFid.Unique,
			    lra.vtype,
			    lra.result); )

	      *vpp = makecfsnode(&lra.ViceFid, vfsp, lra.vtype);
	}
	return(error);
}

#endif	/* NMACH_CFS */
