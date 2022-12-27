/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	auxinode.h,v $
 * Revision 2.6  89/06/03  15:30:31  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  19:07:21  jsb]
 * 
 * Revision 2.5  89/04/22  15:16:16  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

/*
 * Auxilliary include file for vice changes to inodes.
 */

#define VICEMAGIC       0x84fa1cb6

/* These are totally ignored by Aix/Aux since no available fields are possible on a 64 byte inode.h structure that most sys-V systems have */
#if	(!defined(AFS_AUX_ENV) && !defined(AFS_AIX))

#ifdef i_gen

#ifndef	AFS_GFS_ENV
#define i_vicemagic	i_ic.ic_gen
#define i_vicep1        i_ic.ic_spare[0]
#define i_vicep2        i_ic.ic_spare[1]
#define i_vicep3        i_ic.ic_spare[2]
#define i_vicep4        i_ic.ic_spare[3]

#define di_vicemagic	di_ic.ic_gen
#define di_vicep1	di_ic.ic_spare[0]
#define di_vicep2	di_ic.ic_spare[1]
#define di_vicep3	di_ic.ic_spare[2]
#define di_vicep4	di_ic.ic_spare[3]

#else AFS_GFS_ENV

#define	di_vicemagic	di_ic.dg_gennum
#define	di_vicep1	di_ic.dg_spare[0]
#define	di_vicep2	di_ic.dg_spare[1]
#define	di_vicep3	di_ic.dg_spare[2]
#define	di_vicep4 	di_ic.dg_spare[3]

#define	i_vicemagic	di_vicemagic
#define	i_vicep1	di_vicep1
#define	i_vicep2	di_vicep2
#define	i_vicep3	di_vicep3
#define	i_vicep4	di_vicep4
#endif	AFS_GFS_ENV

#else i_gen

#ifndef	AFS_GFS_ENV
#define i_vicemagic	i_ic.ic_spare[0]
#define i_vicep1        i_ic.ic_spare[1]
#define i_vicep2        i_ic.ic_spare[2]
#define i_vicep3        i_ic.ic_spare[3]
#define i_vicep4        i_ic.ic_spare[4]

#define di_vicemagic	di_ic.ic_spare[0]
#define di_vicep1	di_ic.ic_spare[1]
#define di_vicep2	di_ic.ic_spare[2]
#define di_vicep3	di_ic.ic_spare[3]
#define di_vicep4	di_ic.ic_spare[4]
#else AFS_GFS_ENV
/* ultrix 2.2 comes in here */
#define	di_vicemagic	di_ic.dg_gennum
#define	di_vicep1	di_ic.dg_spare[0]
#define	di_vicep2	di_ic.dg_spare[1]
#define	di_vicep3	di_ic.dg_spare[2]
#define	di_vicep4 	di_ic.dg_spare[3]

#define	i_vicemagic	di_vicemagic
#define	i_vicep1	di_vicep1
#define	i_vicep2	di_vicep2
#define	i_vicep3	di_vicep3
#define	i_vicep4	di_vicep4
#endif	AFS_GFS_ENV
#endif i_gen

#endif /*(!defined(AFS_AUX_ENV) && !defined(AFS_AIX))*/

#ifdef	AFS_AIX_ENV
/* This is a temporary hack for handling the vol stuff in aix */
#define	AUXVOLFILE  ".VOLFILE"
#define	BACKAUXDIR  "/usr/afs/backup"

struct afs_auxheader {
    long magic;	    
    long spare1;
    long spare2;
};

/* Disk image of the auxinode sructute */
struct dauxinode {
    long	aux_magic;
    long	aux_param1;
    long	aux_param2;
    long	aux_param3;
    long	aux_param4;
    long	aux_modTime;
    long	aux_spare;
};

#ifdef	KERNEL
/* This must be included after osi.h and afs.h because it references structures in those */
struct afspart {
    struct afspart  *next;
    long	    fileInode;
    struct osi_dev  cdev;
    long	    maxIndex;
    long	    modTime;
};

/* Virtual memory image of the structure */
struct vauxinode {
    struct vauxinode *aux_next;
    struct afspart  *aux_afspart;
    struct afs_q    aux_lruq;
    dev_t	    aux_dev;
    ino_t	    aux_ino;
    short	    aux_refCount;
    struct dauxinode aux_dimage;
};
#endif	KERNEL
#endif	AFS_AIX_ENV

/* KAZAR-RA */
#define	IFILLING		0x1000	/* file currently being filled from network */
#define	IFILLWAIT		0x2000	/* someone is waiting for this data */
#define	IFILLERR		0x4000	/* an error occurred during the fetch --> set EIO */
