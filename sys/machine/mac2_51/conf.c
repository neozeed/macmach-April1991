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
 * $Log:	conf.c,v $
 * Revision 2.2  90/09/04  17:33:57  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/conf.c
 */

#include <cputypes.h>

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)conf.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/conf.h>

int	nulldev();
int	nodev();
int	ttselect(), seltrue();

extern	mhdopen(), mhdclose(), mhdstrategy();
extern	mhdread(), mhdwrite(), mhdioctl();
extern	amhdopen(), amhdioctl();

#include <ramdisk.h>
#if	RAMDISK
extern	ramdstrategy(), ramdread(), ramdwrite();
#define	ramdopen	nulldev
#define ramdclose	nulldev
#define ramdselect	seltrue
#else
#define ramdopen	nodev
#define ramdclose	nodev
#define	ramdstrategy	nodev
#define ramdread	nodev
#define ramdwrite	nodev
#define ramdselect	nodev
#endif

struct bdevsw	bdevsw[] =
{
	{ mhdopen,	mhdclose,	mhdstrategy,	0,		/*0*/
	  0,		0, },
	{ nodev,	nodev,		nodev,		0,		/*1*/
	  0,		0, },
	{ nodev,	nodev,		nodev,		0,		/*2*/
	  0,		0, },
	{ nodev,	nodev,		nodev,		0,		/*3*/
	  0,		0, },
	{ nodev,	nodev,		nodev,		0,		/*4*/
	  0,		0, },
	{ nodev,	nodev,		nodev,		0,		/*5*/
	  0,		0, },
	{ nodev,	nodev,		nodev,		0,		/*6*/
	  0,		0, },
	{ amhdopen,	mhdclose,	mhdstrategy,	0,		/*7*/
	  0,		0, },
	{ ramdopen,	ramdclose,	ramdstrategy,	0,		/*8*/
	  0,		0, },
};
int	nblkdev = sizeof (bdevsw) / sizeof (bdevsw[0]);

extern	cnopen(), cnclose(), cnread(), cnwrite(), cnioctl(), cnselect();
extern	struct tty  cons;

extern	syopen(), syclose(), syread(), sywrite(), syioctl(), syselect();

extern	mmread(), mmwrite(), mmmmap();
#define	mmselect    seltrue

extern int kbopen(), kbclose(), kbread(), kbioctl(), kbselect();
extern struct tty kbtty[];

extern int video_open(), video_close(), video_ioctl(), video_mmap();

#include <pty.h>
#if NPTY > 0
extern int ptsopen(), ptsclose(), ptsread(), ptswrite(), ptsstop();
extern int ptcopen(), ptcclose(), ptcread(), ptcwrite(), ptcselect();
extern int ptyioctl();
extern struct tty pt_tty[];
extern int	npty;
#define	ptsntty	(&npty)
#else
#define	ptsopen		nodev
#define	ptsclose	nodev
#define	ptsread		nodev
#define	ptswrite	nodev
#define	ptcopen		nodev
#define	ptcclose	nodev
#define	ptcread		nodev
#define	ptcwrite	nodev
#define	ptyioctl	nodev
#define	pt_tty		0
#define	ptcselect	nodev
#define	ptsstop		nulldev
#define	ptsntty		0
#endif

struct cdevsw	cdevsw[] =
{
    {
	cnopen,		cnclose,	cnread,		cnwrite,	/*0*/
	cnioctl,	nulldev,	nulldev,	&cons,
	cnselect,	nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*1*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	syopen,		nulldev,	syread,		sywrite,	/*2*/
	syioctl,	nulldev,	nulldev,	0,
	syselect,	nodev,
    },
    {
	nulldev,	nulldev,	mmread,		mmwrite,	/*3*/
	nodev,		nulldev,	nulldev,	0,
	mmselect,	mmmmap,
    },
    {
	kbopen,		kbclose,	kbread,		nodev,		/*4*/
	kbioctl,	nodev,		nodev,		kbtty,
	kbselect,	nodev,
    },
    {
	video_open,	video_close,	nodev,		nodev,		/*5*/
	video_ioctl,	nodev,		nodev,		0,
	nodev,		video_mmap,
    },
    {
	mhdopen,	mhdclose,	mhdread,	mhdwrite,	/*6*/
	mhdioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,
    },
    {
	amhdopen,	mhdclose,	mhdread,	mhdwrite,	/*7*/
	amhdioctl,	nodev,		nulldev,	0,
	seltrue,	nodev,
    },
    {
	ramdopen,	ramdclose,	ramdread,	ramdwrite,	/*8*/
	nodev,		nodev,		nodev,		0,
	ramdselect,	nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*9*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*10*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*11*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*12*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*13*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*14*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*15*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*16*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*17*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*18*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	nodev,		nodev,		nodev,		nodev,		/*19*/
	nodev,		nodev,		nodev,		0,
	nodev,		nodev,
    },
    {
	ptsopen,	ptsclose,	ptsread,	ptswrite,	/*20*/
	ptyioctl,	ptsstop,	nodev,		pt_tty,
	ttselect,	0,
    },
    {
	ptcopen,	ptcclose,	ptcread,	ptcwrite,	/*21*/
	ptyioctl,	nulldev,	nodev,		pt_tty,
	ptcselect,	0,
    }
};
int	nchrdev = sizeof (cdevsw) / sizeof (cdevsw[0]);

int	*nttysw[] =
{
	0,
};
int	ntty = sizeof(nttysw) / sizeof(nttysw[0]);

dev_t	sydev = makedev(2, 0);	/* device number for indirect tty */

int	mem_no = 3; 	/* major device number of memory special file */

/*
 * Swapdev is a fake device implemented
 * in sw.c used only internally to get to swstrategy.
 * It cannot be provided to the users, because the
 * swstrategy routine munches the b_dev and b_blkno entries
 * before calling the appropriate driver.  This would horribly
 * confuse, e.g. the hashing routines. Instead, /dev/drum is
 * provided as a character (raw) device.
 */
dev_t	swapdev = makedev(4, 0);
