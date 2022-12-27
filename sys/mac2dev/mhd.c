/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mhd.c,v $
 * Revision 2.2  90/08/30  11:06:29  bohman
 * 	Created.
 * 	[90/08/29  12:47:25  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/mhd.c
 *	Author: Art Wetzel (CMU macmach)
 */

#define	APPLE_80SC_REVB_BUG
#define	OLDWAY

#include <sys/param.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/disklabel.h>
#ifdef	SUNLABEL
#include <sun/dklabel.h>
#endif	SUNLABEL

/* Apple driver descriptor map info - see Inside Mac V-577 */
struct	dm {
	short	sbSig;
	short	sbBlockSize;
	long	sbBlkCount;
};

/* Apple partition map info - see Inside Mac V-579 */
struct	pm {
	short	pmSig;
	short	pmSigPad;
	long	pmMapBlkCnt;
	long	pmPyPartStart;
	long	pmPartBlkCnt;
	char	pmPartName[32];
	char	pmPartType[32];
};

/* old style Apple device partition map - see Inside Mac IV-292 */
struct	pd {
	short	pdSig;
	long	pdStart;
	long	pdSize;
	long	pdFSID;
};

char	old_mach_string[] = "MACH";
#define	OLD_MACH	(*(int *)old_mach_string)
#define	MACH_PARTITION_TYPE	"Mach_UNIX_BSD4.3"
#define	MAX_A_PART	16	/* max # Apple partitions to examine */

#define	NPART		8		/* allow 8 UNIX partions per drive */
#define	ctlr(dev) (((dev) >> 5) & 0x7)	/* controller # - one of 8 SCSI IDs */
#define	drv(dev)  (((dev) >> 3) & 0x3)	/* drive # - max 4 per ctlr */
#define	udrv(dev) (((dev) >> 3) & 0x1F)	/* unique drive and controller # */
#define	prt(dev)  ((dev) & (NPART-1))	/* partition # - 8 per drive */

#define	NCTLR		7	/* not 8 since the Mac host takes SCSI ID 7 */
#define	CTLR_IDLE	0	/* nothing happening */
#define	CTLR_BUSY	1	/* get, select, cmd, 1st intr */
/* char	ctlr_state[NCTLR];	/* record each controller's current state */
/* char	ctlr_dactive[NCTLR];	/* currently active drive on this controller */
struct	buf raw_buf[NCTLR];	/* raw buffers allowing one active per ctlr */

#define	INQ_SIZE	32	/* size for inquire requests */
#define	DISK_TYPE	0	/* inquire type for disks */
char	ctlr_type[NCTLR] =	/* device type code returned by inquire */
	{ -1, -1, -1, -1, -1, -1, -1};

#define	NDRV	(4*NCTLR)	/* max of 4 drives per controller */
#define	DRV_NOTREADY	0	/* not initialized - initialize on first use */
#define	DRV_IDLE	1	/* initialized and waiting to go */
/* #define	DRV_BUSY	2	/* drive is doing an operation */
char	drv_state[NDRV];	/* record each drive's current state */
int	drv_size[NDRV];		/* size of entire drive in blocks */
int	mprt_offset[NDRV];	/* physical offset of the Mach/UNIX partition */
int	prt_offset[NDRV][NPART];/* start offset of partition in blocks */
int	prt_size[NDRV][NPART];	/* size of partition in blocks */
				/* NOTE: drv_size, prt_offset, and prt_size are
				   initialized on 1st access if DRV_NOTREADY */
struct	buf drqueue[NDRV];	/* the sorted request queue for each drive */

#define	HUGE	0x1FFFFFFF;	/* huge default size of uninitialized disk in blocks */
#define	PARTC	2		/* index of partition C in tables */

/* return # of blocks on an Apple format disk */
apple_disk_size(a) char *a; {
	struct dm *dm = (struct dm *)a;
	if(dm->sbSig == 0x4552)		/* see Inside MAC V-577 */
		return(dm->sbBlkCount);
	return(-1);			/* no Apple magic # */
}

/*
 * the name amhdopen is used to distinguish between
 * mapped and unmapped major devices from the strategy routine
 */
mhdopen(dev, flag)	/* normal disk with mapped block addressing */
dev_t dev;
{
	int i;
	i = amhdopen(dev, flag);
	/* if inquire fails or its not a disk device return ENXIO */
	if(i || ctlr_type[ctlr(dev)] != DISK_TYPE)
		return(ENXIO);
	return(0);
}

amhdopen(dev, flag)	/* absolute unmapped generic devices */
dev_t dev;
{
	char inq_buf[INQ_SIZE];

	/* test unit ready fails on some devices but inquire seems ok */
/*
	if(test_unit_rdy(ctlr(dev)))
		return(ENXIO);
*/
	if(inquire(ctlr(dev), inq_buf, INQ_SIZE))
		return(ENXIO);
	ctlr_type[ctlr(dev)] = inq_buf[0];
	return(0);
}

mhdclose(dev)
dev_t dev;
{
	bflush(dev);
	return(0);
}

mhdioctl(dev, cmd, addr, flag)
dev_t dev; int cmd; caddr_t addr; int flag;
{
	int s, blkno, ud;
	char blk_buf[DEV_BSIZE];
	switch(cmd) {
	case DIOCWDINFO:
		if(!suser())
			return(EPERM);
		if(!bsd_label(dev, addr))	/* install label in system */
			return(EINVAL);
		ud = udrv(dev);
		dinit(dev);	/* make sure mprt_offset is good for disk_part_label */
		/* calculate physical block # of LABELSECTOR on dev */
		blkno = LABELSECTOR + mprt_offset[ud];
		/* we must first read this possible boot block */
		if(scsi_rw(B_READ, ctlr(dev), drv(dev), blkno, blk_buf, 1, 0)) {
			drv_state[ud] = DRV_NOTREADY;
			return(ENXIO);
		}
		/* then overlay the label info */
		bcopy(addr, blk_buf + LABELOFFSET, sizeof(struct disklabel));
		/* and write it all back out */
		if(scsi_rw(B_WRITE, ctlr(dev), drv(dev), blkno, blk_buf, 1, 0)) {
			drv_state[ud] = DRV_NOTREADY;
			return(ENXIO);
		}
		return(0);
	case DIOCGDINFO:
		ud = udrv(dev);
		if(/* drv_state[ud] == DRV_NOTREADY && */ (s = dinit(dev)))
			return(s);
		/* calculate physical block # of LABELSECTOR on dev */
		blkno = LABELSECTOR + mprt_offset[ud];
		if(scsi_rw(B_READ, ctlr(dev), drv(dev), blkno, blk_buf, 1, 0)) {
			drv_state[ud] = DRV_NOTREADY;
			return(ENXIO);
		}
		/* if its not a label then err */
		if(!bsd_label(dev, blk_buf + LABELOFFSET))
			return(ENXIO);
		/* copy the label info back for the user */
		bcopy(blk_buf + LABELOFFSET, addr, sizeof(struct disklabel));
		return(0);
	default:
		;
		/* fall into error */
	}
	printf("Unknown mdioctl 0x%x\n", cmd);
	return(ENOTTY);
}

amhdioctl(dev, cmd, addr, flag)
dev_t dev; int cmd; caddr_t addr; int flag;
{
	printf("amhdiotcl dev 0x%x cmd 0x%x addr 0x%x -> 0x%x flag 0x%x\n",
		dev, cmd, addr, *(long *)addr, flag);
error:
	return(ENOTTY);
}

#define	b_cylin	b_resid

dinit(dev)
dev_t dev;
{
	register int ud, finalstate, i, n, s;
	char blk_buf[DEV_BSIZE];
	struct pm *pm = (struct pm *)blk_buf;
	struct pd *pd = (struct pd *)blk_buf;

	ud = udrv(dev);
	finalstate = DRV_NOTREADY;

	/* clear out old partition info */
	for(i = 0; i < NPART; i++) {
		prt_offset[ud][i] = 0;
		prt_size[ud][i] = 0;
	}
	mprt_offset[ud] = 0;

	/* fail if not a disk */
	if(ctlr_type[ctlr(dev)] != DISK_TYPE)
		goto quit;
	/* read possible Apple driver descriptor map from block 0 */
	if(scsi_rw(B_READ, ctlr(dev), drv(dev), 0, blk_buf, 1, 0))
		goto quit;

#ifdef	APPLE_80SC_REVB_BUG
	/*
	 * Apple 80SC Rev B drives have a firmware bug which causes
	 * multisector reads to fail after a reset.  The only known
	 * workaround is to do a write which some how clears the problem!
	 */
	if(apple_disk_size(blk_buf) > 0	/* only if apple disk */
		&& scsi_rw(B_WRITE, ctlr(dev), drv(dev), 0, blk_buf, 1, 0)) {
printf("**** rewrite of block 0 on ctlr %d drv %d Failed ****\n", ctlr(dev), drv(dev));
			goto quit;
	}
#endif

	/*
	 * If Apple disk size info is present use it. Otherwise set the
	 * drive size to a huge number of blocks.
	 */
	if((i = apple_disk_size(blk_buf)) > 0) {
		drv_size[ud] = i;
		/* read the partition maps looking for the Mach partition */
		for(i = 1; i < MAX_A_PART; i++) {
			if(scsi_rw(B_READ, ctlr(dev), drv(dev), i, blk_buf, 1, 0))
				goto quit;
			/* check old style Apple partitioning - see Inside MAC IV-292 */
			if(i == 1 && pd->pdSig == 0x5453) {
				for(i = 1; i < MAX_A_PART; i++, pd++) {
					if(pd->pdSig == 0x5453 && pd->pdFSID == OLD_MACH) {
						n = pd->pdStart;
						mprt_offset[ud] = n;
						/* point /dev/Xc here to the entire partition */
						prt_offset[ud][PARTC] = n;
						prt_size[ud][PARTC] = pd->pdSize;
						break;
					}
				}
				break;
			}
			/* quit if not a partition map - see Inside MAC V-579 */
			if(pm->pmSig != 0x504D)
				goto quit;
			if(strcmp(MACH_PARTITION_TYPE, pm->pmPartType) == 0) {
				n = pm->pmPyPartStart;
				mprt_offset[ud] = n;
				/* point /dev/Xc here to the entire partition */
				prt_offset[ud][PARTC] = n;
				prt_size[ud][PARTC] = pm->pmPartBlkCnt;
				break;
			}
#ifdef	OLDWAY
			if(strcmp("Apple_UNIX_SVR2", pm->pmPartType) == 0) {
				n = pm->pmPyPartStart;
				if(strcmp("MACHRoot", pm->pmPartName) == 0) {
					/* set up offset and size for sdXa */
					prt_offset[ud][0] = n;
					prt_size[ud][0] = pm->pmPartBlkCnt;
printf("*** WARNING *** OBSOLETE: MACHRoot %d at %d\n", prt_size[ud][0], n);
				} else if(strcmp("MACHUser", pm->pmPartName) == 0) {
					/* set up offset and size for sdXg */
					prt_offset[ud][6] = n;
					prt_size[ud][6] = pm->pmPartBlkCnt;
printf("*** WARNING *** OBSOLETE: MACHUser %d at %d\n", prt_size[ud][6], n);
					finalstate = DRV_IDLE;
					goto quit;
				}
			}
#endif	OLDWAY
		}
		if(i >= MAX_A_PART) { /* we didn't find what we were looking for above */
			printf("No %s partition on ctlr %d drv %d\n",
				MACH_PARTITION_TYPE, ctlr(dev), drv(dev));
			goto quit;
		}
	} else
		drv_size[ud] = HUGE;
	finalstate = DRV_IDLE;
	/* try to read disk label information from /dev/Xc block 0 */
	if(scsi_rw(B_READ, ctlr(dev), drv(dev), prt_offset[ud][PARTC], blk_buf, 1, 0))
		goto quit;
	/* if it really is a disk label, use it */
	if(!bsd_label(dev, blk_buf + LABELOFFSET)) {
#ifdef	SUNLABEL
		if(!sun_label(dev, blk_buf))
#endif	SUNLABEL
		{
			printf("No existing disk label on ctlr %d drv %d\n",
					ctlr(dev), drv(dev));
			goto quit;
		}
	}
quit:
	drv_state[ud] = finalstate;
	if(finalstate == DRV_NOTREADY) {
		printf("Initialization error on ctlr %d drv %d\n",
			ctlr(dev), drv(dev));
		return(ENXIO);
	}
	return(0);
}

/*
 * Authenticate a disk label and install it in the system tables
 */
bsd_label(dev, a)
char *a;
{
	struct disklabel *dlbp = (struct disklabel *)a;
	int i, n, ud;

	ud = udrv(dev);
	if(dlbp->d_magic == DISKMAGIC) {
		for(i = 0; i < NPART; i++) {
			prt_size[ud][i] = dlbp->d_partitions[i].p_size;
			/*
			 * Add the partition offset from the label to the
			 * Mach/UNIX offset so that prt_offset accounts
			 * for both the Apple partition offset for the Mach
			 * section of the disk and the label partion offset.
			 */
			prt_offset[ud][i] = mprt_offset[ud] +
						dlbp->d_partitions[i].p_offset;
		}
		return(1);
	}
	return(0);
}

is_bsd_label_block(p) char *p; {
	struct disklabel *lp = (struct disklabel *)(p + LABELOFFSET);
	if(lp->d_magic == DISKMAGIC && lp->d_magic2 == DISKMAGIC)
		return(1);
	return(0);
}

#ifdef	SUNLABEL
/*
 * Authenticate a disk label and install it in the system tables
 */
sun_label(dev, a)
char *a;
{
	struct dk_label *dklp = (struct dk_label *)a;
	int i, n, ud;

	ud = udrv(dev);
	if(dklp->dkl_magic == DKL_MAGIC) {
		n = dklp->dkl_nhead * dklp->dkl_nsect;
		for(i = 0; i < NPART; i++) {
			prt_size[ud][i] = dklp->dkl_map[i].dkl_nblk;
			/*
			 * Add the partition offset from the label to the
			 * Mach/UNIX offset so that prt_offset accounts
			 * for both the Apple partition offset for the Mach
			 * section of the disk and the label partion offset.
			 */
			prt_offset[ud][i] = mprt_offset[ud] + 
						dklp->dkl_map[i].dkl_cylno*n;
		}
		return(1);
	}
	return(0);
}
#endif	SUNLABEL

struct buf *bp_in_progress;

mhdstrategy(bp) struct buf *bp; {
	register int ud, p, nblks, max, s;
	register struct buf *dp;
/*
printf("mhdstrat %c blk %d cnt %d - %d\n", bp->b_flags & B_READ ? 'R' : 'W', bp->b_blkno, bp->b_bcount, bp->b_bcount/DEV_BSIZE);
*/
	ud = udrv(bp->b_dev);
	if(bp->b_blkno < 0 || (bp->b_bcount&(DEV_BSIZE-1))) {
		bp->b_error = EINVAL;
error:
printf("strat error ctlr %d blk %d count %d\n", ctlr(bp->b_dev), bp->b_blkno, bp->b_bcount);
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	if(bp == &raw_buf[ctlr(bp->b_dev)] &&
	    cdevsw[major(bp->b_dev)].d_open == amhdopen) {
		/* its a raw unmapped access */
		bp->b_cylin = bp->b_blkno;
		max = HUGE;
	} else if(bp != &raw_buf[ctlr(bp->b_dev)] &&
	    bdevsw[major(bp->b_dev)].d_open == amhdopen) {
		/* its a buffered unmapped access */
		bp->b_cylin = bp->b_blkno;
		max = HUGE;
	} else {
		/* its a mapped access */
		p = prt(bp->b_dev);
		if(drv_state[ud] == DRV_NOTREADY && dinit(bp->b_dev)) {
			bp->b_error = ENXIO;
			goto error;
		}
		/* do the mapping and store the mapped block in b_cylin */
		bp->b_cylin = bp->b_blkno +
			prt_offset[ud][p];
		max = prt_size[ud][p];
		/*
		 * the following assumes that LABELSECTOR is 0 so
		 * we can get away with checking for a match on only
		 * the first block of the transfer !!!
		 */
		if(bp->b_cylin == mprt_offset[ud] + LABELSECTOR
		    && !(bp->b_flags & B_READ)) {
printf("ignored label sector write\n");
			/*
			 * silently ignore writes to LABELSECTOR by
			 * modifying the request to skip over it
			 */
			if((bp->b_bcount -= DEV_BSIZE) <= 0) {
				bp->b_resid = 0;
				iodone(bp);
				return;
			}
			bp->b_un.b_addr += DEV_BSIZE;
			bp->b_blkno++;
			bp->b_cylin++;
		}
	}
	nblks = bp->b_bcount >> DEV_BSHIFT;
	if(bp->b_blkno + nblks > max) {
		/* request overlaps end so */
		nblks = max - bp->b_blkno;	/* how much will fit? */
		if(nblks <= 0) {
			if(nblks < 0) {	/* entirely past the end */
				bp->b_error = EINVAL;
				goto error;
			}
			/* we are right at end so return EOF */
			bp->b_resid = bp->b_bcount;
			iodone(bp);
			return;
		}
		/* leave nblks with the number of blocks that will work */
	}
	bp->b_bcount |= nblks; /* hack: tag block count onto lsb's of b_bcount */
	/*
	 * we sort in the hope that SCUZZY drives keep
	 * blocks in linear order even though SCSI can hide
	 * the actual disk location and with it goes the
	 * possibility of good rotational optimizations
	 */
	ud = 0; /* XXX temp while no disconnect/reconnect - everything on queue 0 */
	dp = &drqueue[ud];
	s = spl2();
	disksort(dp, bp);
/*	if(ctlr_state[ctlr(bp->b_dev)] == CTLR_IDLE) */
	if(bp_in_progress == NULL)	/* XXX tmp way to see if nothing running */
		mstart();
	splx(s);
}

int	in_mstart;

cmd_complete(r)
{
	int s;
	register struct buf *bp = bp_in_progress;
	/* errors causing SCSI reset may have already cleared bp_in_progress */
	if(bp == NULL)
		return;
	if(r >= 0)
		bp->b_resid = bp->b_bcount - r;
	else {
		bp->b_resid = bp->b_bcount;
		bp->b_error = EIO;
		bp->b_flags |= B_ERROR;
	}
	/* XXX return error on 0 len transfer writes with non 0 bcount??? */
	iodone(bp);
	s = spl2();
	bp_in_progress = NULL;
	if(!in_mstart)
		mstart();
	splx(s);
}

mstart()
{
	register struct buf *bp;
	register struct buf *dp = &drqueue[0];
	int nblks;

again:
	if((bp = dp->b_actf) == NULL)
		return;
/*
printf("mstart bp 0x%x dev 0x%x blk %d inprog 0x%x actf 0x%x\n",
bp, bp->b_dev, bp->b_cylin, bp_in_progress, bp->av_forw);
*/
	bp_in_progress = bp;
	dp->b_actf = bp->av_forw;	/* XXX - now or at iodone time ??? */
#define	BCNT_MASK	(DEV_BSIZE-1)
	nblks = bp->b_bcount & BCNT_MASK;	/* # of 512 blocks requested */
	bp->b_bcount &= ~BCNT_MASK;	/* reset b_bcount value */
	in_mstart = 1;
	if(scsi_rw(bp->b_flags&B_READ, ctlr(bp->b_dev), drv(bp->b_dev),
	    bp->b_cylin, bp->b_un.b_addr, nblks, cmd_complete)) {
#ifdef	DEBUG
printf("mstart error bp 0x%x in_progress 0x%x\n", bp, bp_in_progress);
#endif
		/* iodone was already called from cmd_complete */
		drv_state[udrv(bp->b_dev)] = DRV_NOTREADY;	/* force new dinit() */
		in_mstart = 0;
		goto again;
	}
	in_mstart = 0;
}

/* NOTE: minphys must be limited to 32K for writes to some SCSI drives */
mhdread(dev, uio)
dev_t	dev;
struct	uio *uio;
{
    return(physio(mhdstrategy, &raw_buf[ctlr(dev)], dev, B_READ, minphys, uio));
}

mhdwrite(dev, uio)
dev_t	dev;
struct	uio *uio;
{
    return(physio(mhdstrategy, &raw_buf[ctlr(dev)], dev, B_WRITE, minphys, uio));
}
