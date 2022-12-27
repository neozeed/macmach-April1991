/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	cnode.h,v $
 * Revision 2.2  90/07/05  11:27:24  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.4  90/05/31  17:02:16  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 * 
 */

#ifndef	_CNODE_H_
#define	_CNODE_H_

#include <sys/types.h>
#ifdef	KERNEL
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

struct cnode {
    struct vnode    c_vnode;
    u_short	    c_flags;	/* flags (see below) */
    ViceFid	    c_fid;	/* file handle */
#ifdef	KERNEL
    struct vnode    *c_ovp;	/* open vnode pointer */
    u_short	    c_ocount;	/* count of openers */
    u_short         c_owrite;   /* count of open for write */
    struct vattr    c_vattr;    /* attributes */
    char            *c_symlink; /* pointer to symbolic link */
    u_short         c_symlen;   /* length of symbolic link */
#else	KERNEL
    dev_t	    c_device;	/* associated vnode device */
    ino_t	    c_inode;	/* associated vnode inode */
#endif	KERNEL
};

/* flags */
#define C_VATTR       0x1         /* Validity of vattr in the cnode */
#define C_SYMLINK     0x2         /* Validity of symlink pointer in the cnode */

/*
 * Macro operations on the cnode
 */

/* Check validity of the cnode fields */
#define VALID_VATTR(cp)          ((cp->c_flags) & C_VATTR)
#define VALID_SYMLINK(cp)        ((cp->c_flags) & C_SYMLINK)

/* Convert between vnode and cnode */
#define	VTOC(vp)	((struct cnode *)(vp))
#define	CTOV(cp)	((struct vnode *)(cp))

#ifdef	KERNEL
#define CN_INIT(CP) {\
    bzero((CP), sizeof(struct cnode));\
}
#else	KERNEL
#define CN_INIT(CP, FID, DEV, INO) {\
    bzero((CP), sizeof(struct cnode));\
    VN_INIT(CTOV((CP)), 0, 0, 0);\
    (CP)->c_fid = FID;\
    (CP)->c_device = DEV;\
    (CP)->c_inode = INO;\
}
#endif	KERNEL

#define	CNODE_NEXT(cp)	(VTOC(CTOV(cp)->v_next))

#endif	not _CNODE_H_

