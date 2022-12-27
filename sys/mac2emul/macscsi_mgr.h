/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2emul/macscsi_mgr.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Macintosh
 * scsi manager defines
 */

/*
 * SCSI Mgr Operation codes
 */
#define scsiReset	0
#define scsiGet		1
#define scsiSelect	2
#define scsiCmd		3
#define scsiComplete	4
#define scsiRead	5
#define scsiWrite	6
#define scsiRBlind	8
#define scsiWBlind	9
#define scsiStat	10

/*
 * Result codes
 */
#define noErr		0
#define scCommErr	2
#define scBadParamsErr	4
#define scPhaseErr	5
#define scCompareErr	6

#define SCSI_NBBLK	512

#define scsiBlkno(x)	\
    (((x)->cmd_msblk<<16)|((x)->cmd_blk<<8)|((x)->cmd_lsblk))

#define scsiNBlk(x)	\
    ((x)->cmd_nblk? (x)->cmd_nblk: 256)

struct scsiRWCmd {
    unsigned char	cmd_op;
    unsigned char	cmd_lun:3,
                        cmd_msblk:5;
    unsigned char	cmd_blk;
    unsigned char	cmd_lsblk;
    unsigned char	cmd_nblk;
};

struct SCSIInstr {
    unsigned short	scOpcode;
    unsigned long	scParam1;
    unsigned long	scParam2;
};

#define scInc		1
#define scNoInc		2
#define scAdd		3
#define scMove		4
#define scLoop		5
#define scNOp		6
#define scStop		7
#define scComp		8
