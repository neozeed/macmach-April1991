/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	osi.h,v $
 * Revision 2.7  89/08/02  08:01:53  jsb
 * 	Define osi_Zalloc, etc. macros for MACH and non-MACH cases.
 * 	[89/07/31  18:03:03  jsb]
 * 
 * Revision 2.6  89/06/03  15:31:34  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  19:08:12  jsb]
 * 
 * Revision 2.5  89/04/22  15:16:59  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#ifndef _AFS_OSI_
#define _AFS_OSI_

#ifndef AFS_ENV
/* include afs param file if we need it */
#include <afs/param.h>
#endif AFS_ENV

#include <sys/types.h>
#include <sys/param.h>
#ifdef	AFS_MACH_ENV
#include <mach_nbc.h>
#include <kern/zalloc.h>
#endif

#ifdef AFS_MACH_ENV
typedef zone_t osi_zone_t;
#define osi_Zalloc(zone)	zalloc(zone)
#define osi_Zfree(zone, var)	zfree(zone, var)
#define osi_Zinit(size, name)	zinit((size), 10000 * (size), 0, FALSE, (name))
#define osi_Zcollectable(zone)	zcollectable(zone)
#define osi_Zsize(zone)		((zone)->elem_size)
#else
typedef int osi_zone_t;
#define	osi_Zalloc(zone)	AFS_KALLOC(zone)
#define osi_ZFree(zone, var)	AFS_KFREE(var, zone)
#define osi_Zinit(size, name)	((name) = (size))
#define osi_Zcollectable(zone)	(zone)
#define osi_Zsize(zone)		(zone)
#endif

/* this is just a dummy type decl, we're really using struct sockets here */
struct osi_socket {
    int junk;
};

struct osi_stat {
    long size;	    /* file size in bytes */
    long blksize;   /* optimal transfer size in bytes */
    long mtime;	    /* modification date */
    long atime;	    /* access time */
};

struct osi_file {
    struct vnode *vnode;
    long offset;
    int	(*proc)();	/* proc, which, if not null, is called on writes */
    char *rock;		/* rock passed to proc */
};

struct osi_dev {
    long dev;
};

struct osi_WaitHandle {
    caddr_t proc;	/* process waiting */
};

#define	osi_SetFileProc(x,p)	((x)->proc=(p))
#define	osi_SetFileRock(x,r)	((x)->rock=(r))
#define	osi_GetFileProc(x)	((x)->proc)
#define	osi_GetFileRock(x)	((x)->rock)
#define	osi_Seek(x,p)	((x)->offset = (p))
#define	osi_Sleep(x)	sleep((caddr_t) x,PZERO-2)
#define	osi_Wakeup(x)	wakeup((caddr_t) x)
#define	osi_NullHandle(x)	((x)->proc == (caddr_t) 0)
#define	osi_PACKETSIZE	4096		/* buffer size */
#define	osi_NPACKETS	20		/* number of cluster packets to allocate */
extern struct osi_socket *osi_NewSocket();
extern struct osi_file *osi_UFSOpen();
extern struct ucred osi_cred;
extern char *osi_Alloc();
extern char *osi_AllocSendSpace();

#ifdef	AFS_GFS_ENV
extern struct gnode_ops *afs_ops;
#define	vType(vc)	    IFTOVT((vc)->v.g_mode)
#define	vSetType(vc,type)   (vc)->v.g_mode = VTTOIF((type))
#define	vSetVfsp(vc,vfsp)   (vc)->v.g_mp = (struct mount *)(vfsp)->vfs_data
#define	iunlock(gp)	    gfs_unlock((gp))
#define	AFS_MAXNAMLEN	    255
#else	AFS_GFS_ENV
/* GFS-based defines/macros for vfs system sun & ibm032 */
extern struct vnodeops *afs_ops;
#ifdef	AFS_MACH_ENV
#define	vType(vc)	    ((vc)->v.v_mode&VFMT)
#define	vSetType(vc,type)   (vc)->v.v_mode = ((vc)->v.v_mode&~VFMT) | ((type)&VFMT)
#else	AFS_MACH_ENV
#define	vType(vc)	    (vc)->v.v_type
#define	vSetType(vc,type)   (vc)->v.v_type = (type)
#endif	AFS_MACH_ENV
#define	vSetVfsp(vc,vfsp)   (vc)->v.v_vfsp = (vfsp)
#define	gop_lookupname(fnamep,segflg,followlink,dirvpp,compvpp) \
lookupname((fnamep),(segflg),(followlink),(dirvpp),(compvpp))
#if	defined(AFS_MACH_ENV) && MACH_NBC
/* Go directly to ufs layer to avoid deadlock */
#define	gop_rdwr(rw,gp,base,len,offset,segflg,unit,aresid) \
ufs_rdwri((rw),(VTOI((gp))),(base),(len),(offset),(segflg),(unit),(aresid))
#else	defined(AFS_MACH_ENV) && MACH_NBC
#ifndef	AFS_AIX_ENV	    /* AIX also has it's own gop_rdwr() */
#define	gop_rdwr(rw,gp,base,len,offset,segflg,unit,aresid) \
vn_rdwr((rw),(gp),(base),(len),(offset),(segflg),(unit),(aresid))
#endif
#endif	defined(AFS_MACH_ENV) && MACH_NBC
#endif	AFS_GFS_ENV

#define	DEBUG	1	/* Temporary default to enable debugging/logging */


#if	(defined(AFS_AIX_ENV) || defined(AFS_AUX_ENV))
/* Aix/Aux doesn't have bufpages per se but since the allocated buffer space is based in v.v_buf (equivalent to nbuf) we use this instead... */
#define	bufpages    v.v_buf
#else
#ifndef	AFS_VFS40
extern int  bufpages;
#endif
#endif
#ifndef	AFS_VFS40
#define	afs_bufferpages	    bufpages
#endif
#define	afs_suser	    suser

/* The following afs_hz definition SHOULD go into afs/param.h. */
#ifdef	AFS_AIX_ENV
#define	afs_hz	    HZ
#else
#ifdef	AFS_AUX_ENV
#define	afs_hz	    v.v_hz
#else
/* the rest... */
#define	afs_hz	    hz
#endif
#endif	AFS_AIX_ENV

#endif _AFS_OSI_
