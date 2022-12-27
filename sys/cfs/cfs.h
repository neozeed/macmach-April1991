/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	cfs.h,v $
 * Revision 2.2  90/07/05  11:26:23  mrt
 * 	Changed message size back to 4k. Also changed VC_MAXDATASIZE to reflect
 * 	the xdr implementation of treating shorts (2 bytes) as longs (4 bytes).
 * 	[90/05/23            dcs]
 * 
 * 	Added constants to support READDIR, IOCTL, and RDWR messages 
 * 	in an effort to compensate for the VFS exec bogusity.
 * 	[90/05/23            dcs]
 * 
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.6  90/05/31  17:01:05  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 */
#ifndef _CFS_HEADER_
#define _CFS_HEADER_

#include <sys/types.h>
#include <sys/user.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
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
#endif	KERNEL

/*
 * Cfs constants
 */
#define CFS_MAXNAMLEN 255
#define CFS_MAXPATHLEN 1024
#define CFS_MAXARRAYSIZE 8192

#define CFS_PROGRAM	((u_long)0x20202020)
#define CFS_VERSION	((u_long)1)	
	
/*
#define CFS_MOUNT	((u_long) 1)
#define CFS_UNMOUNT	((u_long) 2)
*/
#define CFS_ROOT	((u_long) 2)
#define CFS_SYNC	((u_long) 3)
#define CFS_OPEN	((u_long) 4)
#define CFS_CLOSE	((u_long) 5)
#define CFS_IOCTL	((u_long) 6)
#define CFS_GETATTR	((u_long) 7)
#define CFS_SETATTR	((u_long) 8)
#define CFS_ACCESS	((u_long) 9)
#define CFS_LOOKUP	((u_long) 10)
#define CFS_CREATE	((u_long) 11)
#define CFS_REMOVE	((u_long) 12)
#define CFS_LINK	((u_long) 13)
#define CFS_RENAME	((u_long) 14)
#define CFS_MKDIR	((u_long) 15)
#define CFS_RMDIR	((u_long) 16)
#define CFS_READDIR	((u_long) 17)
#define CFS_SYMLINK	((u_long) 18)
#define CFS_READLINK	((u_long) 19)
#define CFS_FSYNC	((u_long) 20)
#define CFS_INACTIVE	((u_long) 21)
#define	CFS_VGET	((u_long) 22)
#define	CFS_SIGNAL	((u_long) 23)
#define	CFS_INVALIDATE	((u_long) 24)
#define CFS_FLUSH       ((u_long) 25)
#define CFS_PURGEUSER   ((u_long) 26)
#define CFS_ZAPFILE     ((u_long) 27)
#define CFS_ZAPDIR      ((u_long) 28)
#define CFS_ZAPVNODE    ((u_long) 29)
#define CFS_PURGEFID    ((u_long) 30)
#define	CFS_RDWR	((u_long) 31)

/*
 * XDR routine and data structure definitions
 */
#ifndef	C_ARGS
#ifdef	__STDC__
#define	C_ARGS(arglist)	arglist
#else	__STDC__
#define	C_ARGS(arglist)	()
#endif	__STDC__
#endif	C_ARGS

extern bool_t xdr_cred C_ARGS((XDR *, void *));
extern bool_t xdr_fid C_ARGS((XDR *, void *));
extern bool_t xdr_fidcred C_ARGS((XDR *, void *));
extern bool_t xdr_fidreturn C_ARGS((XDR *, void *));
extern bool_t xdr_fidattrreturn C_ARGS((XDR *, void *));
extern bool_t xdr_attr_return C_ARGS((XDR *, void *));
extern bool_t xdr_cfssetattr C_ARGS((XDR *, void *));
extern bool_t xdr_cfscreate C_ARGS((XDR *, void *));
extern bool_t xdr_fid_int C_ARGS((XDR *, void *));
extern bool_t xdr_cfsopenreturn C_ARGS((XDR *, void *));
extern bool_t xdr_fid_name C_ARGS((XDR *, void *));
extern bool_t xdr_cfslookupreturn C_ARGS((XDR *, void *));
extern bool_t xdr_cfslink C_ARGS((XDR *, void *));
extern bool_t xdr_cfsrename C_ARGS((XDR *, void *));
extern bool_t xdr_cfsmkdir C_ARGS((XDR *, void *));
extern bool_t xdr_cfssymlink C_ARGS((XDR *, void *));
extern bool_t xdr_cfsioctl C_ARGS((XDR *, void *));
extern bool_t xdr_cfsioctlreturn C_ARGS((XDR *, void *));
extern bool_t xdr_cfsreaddir C_ARGS((XDR *, void *));
extern bool_t xdr_cfsreaddirrtn C_ARGS((XDR *, void *));
extern bool_t xdr_cfsreadlinkrtn C_ARGS((XDR *, void *));
extern bool_t xdr_cfsrdwr C_ARGS((XDR *, void *));
extern bool_t xdr_cfsrdwrrtn C_ARGS((XDR *, void *));

struct cfsfidcred {
	struct  ucred cred;
	ViceFid ViceFid;
};

struct cfsfidreturn {
	u_long	result;
	ViceFid ViceFid;
};

struct cfsfidattrreturn {
	u_long	result;
	ViceFid ViceFid;
	struct vattr attr;
};

struct cfsattrreturn {
	u_long result;
	struct vattr attr;
};

struct cfssetattrargs {
	struct  ucred cred;
	struct vattr attr;
	ViceFid ViceFid;
};

struct cfscreateargs {
	struct  ucred cred;
	ViceFid ViceFid;
	char	*name;
	struct vattr attr;
	int excl;
	int mode;
};

struct cfsfidintargs {
	struct  ucred cred;
	ViceFid	ViceFid;
	int	flags;
};

struct cfsopenreturnargs {
	u_long	result;
	dev_t	dev;
	ino_t	inode;
};

struct cfsfidnameargs {
	struct  ucred cred;
	ViceFid	ViceFid;
	char	*name;
};

struct cfslookupreturnargs {
	u_long	result;
	ViceFid ViceFid;
	int	vtype;
};

/* 
 * Occasionally, don't cache the fid returned by CFS_LOOKUP. For instance, if
 * the fid is inconsistent. This case is handled by setting the top bit of the
 * return result parameter.
 */
#define CFS_NOCACHE          0x80000000

struct cfslinkargs {
	struct  ucred cred;
	ViceFid sourceFid;
	ViceFid destFid;
	char	*tname;
};

struct cfsrenameargs {
	struct  ucred cred;
	ViceFid	sourceFid;
	char	*srcname;
	ViceFid destFid;
	char	*destname;
};

struct cfsmkdirargs {
	struct  ucred cred;
	ViceFid	ViceFid;
	char	*name;
	struct vattr attr;
};

struct cfssymlinkargs {
	struct  ucred cred;
	ViceFid	ViceFid;
	char	*srcname;
	struct vattr attr;
	char	*tname;
};

struct cfsioctlargs {
	struct	ucred cred;
	ViceFid ViceFid;
	int	cmd;
	caddr_t	data;
	int	len;
	int	rwflag;
};

struct cfsioctlreturnargs {
	u_long	result;
	int	len;
	caddr_t	data;
};

struct cfsreaddirargs {
	struct	ucred cred;
	ViceFid	ViceFid;
	int	count;
	int	offset;
};

struct cfsreaddirrtnargs {
	u_long	result;
	int	size;
	caddr_t	data;
};

struct cfsreadlinkrtnargs {
	u_long	result;
	int	count;
	caddr_t	data;
};

struct cfsrdwrargs {
	struct	ucred cred;
	ViceFid	ViceFid;
	int	rwflag;
	int	count;
	caddr_t	data;
	int	offset;
	int	ioflag;
};

struct cfsrdwrrtnargs {
	u_long	result;
	int	rwflag;
	int	count;
	caddr_t	data;
};


/*
 * This overlays the fid structure (see vfs.h)
 */
struct cfid {
	u_short	cfid_len;
	ViceFid	cfid_fid;
};

/*
 * Kernel <--> Venus communications.
 */
#define	VC_IOCTL_BUFSIZE    4096
#define	VC_READDIR_BUFSIZE  4096
#define	VC_RDWR_BUFSIZE     4096
#define	VC_PREFIXSIZE	    2 * sizeof(u_long)
#define	VC_MAXDATASIZE	    VC_READDIR_BUFSIZE + 2 * sizeof(struct cfsrdwrargs)
#define	VC_MAXMSGSIZE	    VC_PREFIXSIZE + VC_MAXDATASIZE

#ifdef	KERNEL
struct vmsg {
	struct vmsg	*vm_chain[2];
	int		vm_flags; 
	char		vm_wchan;
	u_long		vm_seq;
	u_long		vm_opcode;
	xdrproc_t	vm_xdrargs;
	caddr_t		vm_argsp;
	xdrproc_t	vm_xdrres;
	caddr_t		vm_resp;
};

#define	VM_READ	    1
#define	VM_WRITE    2
#define	VM_INTR	    4

#include <sys/proc.h>
struct vcomm {
	u_long		vc_seq;
	struct proc	*vc_selproc;
	struct vmsg	*vc_requests[2];
	struct vmsg	*vc_replys[2];
	caddr_t		vc_buf;
};

#define	VC_OPEN(vcp)	    ((vcp)->vc_buf != (caddr_t)0)
#endif	KERNEL


#ifdef	KERNEL
#include <mach_cfs.h>

/*
 * CFS structure to hold mount/file system information
 */
struct cfs_mntinfo {
	struct vnode 	*mi_rootvp;
	int		mi_refct;
	struct vnode	*mi_ctlvp;
	struct vcomm	mi_vcomm;
};

struct cfs_mntinfo cfs_mnttbl[NMACH_CFS]; /* indexed by minor device number */

/*
 * vfs pointer to mount info
 */
#define vftomi(vfsp)    ((struct cfs_mntinfo *)((vfsp)->vfs_data))

/*
 * vnode pointer to mount info
 */
#define vtomi(vp)       ((struct cfs_mntinfo *)(((vp)->v_vfsp)->vfs_data))

#define	CFS_MOUNTED(vfsp)   (vftomi((vfsp)) != (struct cfs_mntinfo *)0)


/*
 * Used for identifying usage of "Control" object
 */
#define	CFS_CONTROL		".CONTROL"
#define	CTL_VOL			-1
#define	CTL_VNO			-1
#define	CTL_UNI			-1
#define	IS_ROOT_VP(vp)		((vp) == vtomi((vp))->mi_rootvp)
#define	IS_CTL_VP(vp)		((vp) == vtomi((vp))->mi_ctlvp)
#define	IS_CTL_NAME(dvp, name)	(IS_ROOT_VP((dvp)) && strcmp(name, CFS_CONTROL) == 0)
#define	IS_CTL_FID(fidp)	((fidp)->cfid_fid.Volume == CTL_VOL &&\
				 (fidp)->cfid_fid.Vnode == CTL_VNO &&\
				 (fidp)->cfid_fid.Unique == CTL_UNI)
#define	ISDIR(fid)		((fid).Vnode & 0x1)

/*
 * Used to select debugging statements throughout the cfs code.
 */
extern int cfsdebug;
#define CFSDBGMSK(N)    (1 << N)
#define CFSDEBUG(N, STMT)       { if (cfsdebug & CFSDBGMSK(N)) { STMT } }
#endif	KERNEL

#endif !_CFS_HEADER_
