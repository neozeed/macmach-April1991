/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	param.h,v $
 * Revision 2.11  90/07/03  16:31:22  mrt
 * 	Merged mt Xinu 2.6MSD Changes.
 * 	[90/05/17  17:45:10  mrt]
 * 
 * Revision 2.10  89/08/02  09:04:38  jsb
 * 	Require MACH_NFS for AFS_GATEWAY.
 * 	[89/07/31  18:01:07  jsb]
 * 
 * Revision 2.9  89/06/03  15:31:45  jsb
 * 	Changed to define new macros to reflect splitting of gateway support
 * 	from Mach support. Removed MACH conditionals and unused definitions.
 * 	Moved SYS_NAME definitions to vfs/vfs_sysnames.h (to allow for @sys
 * 	expansion for all filesystems). Picked up new VATTR_NULL and
 * 	AFS{BIG,LITTLE}_ENDIAN definitions.
 * 	[89/06/03  14:40:30  jsb]
 * 
 * Revision 2.8  89/05/30  10:31:02  rvb
 * 	Since it is used primarily to get the right executables,
 * 	need to distnguish between Pmax and Mseries Mips machines.
 * 	[89/04/20            af]
 * 
 * Revision 2.7  89/04/22  15:17:10  gm0w
 * 	Added missing #endif KERNEL.
 * 	[89/04/19            gm0w]
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
#ifndef	_AFS_PARAM_H_
#define	_AFS_PARAM_H_

#include <vfs/vfs_sysnames.h>
#ifdef KERNEL
#include <mach_nfs.h>
#endif

#define	AFS_MACH_ENV	1
#define	AFS_MACH_FIX	1
#define	AFS_PAGINCRED	1
#define	AFS_ENV		1

#if	MACH_NFS
#define	AFS_GATEWAY	1
#endif

#define	AFS_GREEDY43_ENV	1

#if	BYTE_MSF
#define	AFSBIG_ENDIAN		1
#else
#define	AFSLITTLE_ENDIAN	1
#endif

/* Extra kernel definitions (from kdefs file) */
#ifdef KERNEL
#define	AFS_VFS34		1	/* What is VFS34??? */
#define	AFS_SHORTGID		1	/* are group id's short? */
#define	afsio_iov	uio_iov
#define	afsio_iovcnt	uio_iovcnt
#define	afsio_offset	uio_offset
#define	afsio_seg	uio_segflg
#define	afsio_resid	uio_resid
#define	AFS_UIOSYS	UIO_SYSSPACE
#define	AFS_UIOUSER	UIO_USERSPACE
#define	AFS_CLBYTES	CLBYTES
#define	osi_GetTime(x)	microtime(x)
#define	AFS_KALLOC(x)	kalloc(x)
#define	AFS_KFREE(x,y)	kfree(x,y)
#define	EREMOTE		ENFSREMOTE
#define	VATTR_NULL	vattr_null
#endif	KERNEL

#endif	/* _AFS_PARAM_H_ */
