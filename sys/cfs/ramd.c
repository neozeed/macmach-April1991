/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	ramd.c,v $
 * Revision 2.2  90/07/05  11:27:46  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.3  90/05/31  17:04:49  dcs
 * Prepare for merge with facilities kernel.
 * 
 * 
 * 
 */

/*
 * The purpose of this driver is to provide a fast memory-based `disk'.
 * The interface to this disk should match the specs of regular magnetic
 * media as closely as possible. The memory addresses to be used for the
 * disk are specified in an Ioctl(2) call. They can either refer to a
 * range of memory in the I/O address space corresponding to a board on
 * the I/O bus, or the driver can have the kernel allocate wired-down pages.
 */

#include <ramd.h>
#if	NRAMD > 0

#include <mach/boolean.h>
#include <sys/param.h>
#include <sys/vm.h>
#include <sys/buf.h>
#include <sys/errno.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <ca/io.h>
#include <ca/debug.h>
#include <ca/scr.h>
#include <caio/ioccvar.h>
#include <vm/vm_kern.h>
#include <cfs/ramdio.h>

/* flags for turning device on and off */
short initialized = FALSE;
short frozen	  = FALSE;

/* buffer for input/output */
char *start_addr;
int bufsize;

/*
 * Some of the data structures here are never really used by the ramdisk,
 * but I left them around for semantic reasons.
 */

/* per-drive buffers */
struct buf rramdbuf[NRAMD];		/* buffers for raw I/O */

struct iocc_device *ramddinfo[NRAMD];
struct iocc_ctlr *ramdminfo[NRAMD];

int ramdprobe(), ramdslave(), ramdattach(), ramddgo(), ramdint();
int ramdstrategy(), ramdminphys();
extern int nodev();

caddr_t ramdstd[] = {
	(caddr_t)0xf4160000	/* csr as listed in ../conf/MASTER.ca */
};

struct iocc_driver ramddriver = {
	ramdprobe, 0, ramdattach, ramddgo, ramdstd, "ramd", 
	ramddinfo, "ramdc", ramdminfo, ramdint
};

/* debugging flag */
int ramddebug;

/* ramdprobe, slave, attach, ddgo, select, int, open, close, and size are not
 * really needed. I keep them around mostly for semantic reasons.
 */
ramdprobe(reg)
	struct ramddevice *reg;
{
	return(PROBE_NOINT);		/* Indicate no interrupt occured */
}

ramdslave(iod, reg)
	struct iocc_device *iod;
	struct ramddevice *reg;
{
	return(1);
}

/*
 * Initialize the device to hold zeros 
 */

ramdattach(iod)
	struct iocc_device *iod;
{
	DEBUGF(ramddebug, printf("In ramdattach\n"););
	return(1);
}

ramddgo(ic,length,addr,bp)
	struct iocc_ctlr *ic;
	int length, addr;
	struct buf *bp;
{
	DEBUGF(ramddebug, printf("In ramddgo\n"););
}

ramdint(ctlr)
	int ctlr;
{
	DEBUGF(ramddebug, printf("In ramdint\n"););
	return(0);
}

ramdopen(dev, flag)
	dev_t dev;
{
	DEBUGF(ramddebug, printf("In function ramdopen\n"););

	/* On the off chance that this works, set the ccr value in the current
	 * thread to allow reading and writing on the I/O memory bus 
	 */
	set_ccr(0xff);
	return (0);
}

ramdclose(dev)
	dev_t dev;
{
	DEBUGF(ramddebug, printf("In function ramdclose\n"););

	/* Reset the bits set on open in the channel control register (ccr) */
	set_ccr(0x80000000 | (int) CCR_DEFAULT); /* this is a problem, what if someone
						    else had set another bit at
						    some point? This would clear it out.
						  */
	return (0);
}

ramdread(dev, uio)
	dev_t dev;
	struct uio *uio;
{
        int unit = minor(dev);

	DEBUGF(ramddebug, printf("In function ramdread, unit=%d\n",unit););
	DEBUGF(ramddebug, printf("read: offset = %d, count=%d\n",
				  uio->uio_offset,uio->uio_resid););
	if ((unit < 0) || (unit >= NRAMD)) {
		printf("invalid device minor number\n");
		return(ENXIO);
	}

	if (!initialized)
		return(EIO);

	return( physio(ramdstrategy, &rramdbuf[unit], dev, B_READ,
			 ramdminphys, uio));
}

/* i'm not freezing this routine because it doesn't directly affect the
 * device. It will read info from internal buffers as long as it can,
 * then it will use the strategy routine to update the device. I will
 * stop that update from happening during a freeze in the strategy routine.
 */

ramdwrite(dev, uio)
	dev_t dev;
	struct uio *uio;
{
        int unit = minor(dev);

	DEBUGF(ramddebug, printf("In function ramdwrite, unit=%d\n",unit););
	DEBUGF(ramddebug, printf("write: offset=%d, count=%d\n",
				  uio->uio_offset,uio->uio_resid););

	if ((unit < 0) || (unit >= NRAMD)) {
		printf("invalid device minor number\n");
		return(ENXIO);
	}

	if (!initialized)
		return(EIO);

	return( physio(ramdstrategy, &rramdbuf[unit], dev, B_WRITE,
			 ramdminphys, uio));
}

ramdminphys(bp)
	register struct buf *bp;
{
	minphys(bp);	/* enforces kernel-wide b_bcount restriction */
}

ramdioctl(dev, cmd, addr, flag)
	dev_t dev;
	caddr_t addr;
{
	DEBUGF(ramddebug, printf("In function ramdioctl "););

	int unit = minor(dev);
	struct ramdbound *data = (struct ramdbound *)addr;

	if ((unit < 0) || (unit >= NRAMD)) {
		printf("invalid device minor number\n");
		return(ENXIO);
	}

	switch(cmd) {
		case RAMDSBND :
			DEBUGF(ramddebug, printf("SBND %x %d\n",
						  data->addr, data->size););
			if (data->size > 0)
				return(set_ramd_bounds(data->addr, 
							      data->size));
			else	
				return(EINVAL);

		case RAMDGBND :
			DEBUGF(ramddebug, printf("GBND\n"););
			if (!initialized)
				return(EIO);
			
			data->addr = start_addr;
			data->size = bufsize;
			break;

		case RAMDFREEZE : 
			DEBUGF(ramddebug, printf("FREEZE\n"););
			if (!initialized)
				return(EIO);

			frozen = TRUE;
			break;

		case RAMDTHAW : 
			DEBUGF(ramddebug, printf("THAW\n"););
			if (!initialized)
				return(EIO);

			if (frozen) {	/* wake up sleeping processes */
				frozen = FALSE;
				wakeup(&frozen);
			}
			break;

		default : 
			return(EINVAL);
	}

	return(0);
}

ramdselect (dev, rw)
	dev_t dev;
	int rw;
{
	DEBUGF(ramddebug, printf("In function ramdselect\n"););
	return(0);
}

/* 
 * The basic plan here is to read/write blocks to disk. the operation will
 * be specified in bp->b_flags and the block number in bp->b_blkno. The
 * block should be copied into the buffer (address?). Upon completion, the
 * routine iodone will set the flags to indicate the I/O is complete. Upon
 * error, flags should set B_ERROR and bp->b_error should hold the error code.
 */

ramdstrategy(bp)
	register struct	buf *bp;
{
	int unit, offset;
	char *addr;
	unit = minor(bp->b_dev);
	offset = dbtob(bp->b_blkno);
	int i;

	if (bp->b_flags & B_READ) { 
		DEBUGF(ramddebug,
			printf("In function ramdstrategy reading\n"););
	}
	else {
		DEBUGF(ramddebug,
			printf("In function ramdstrategy writing\n"););
	}

	if ((unit < 0) || (unit >= NRAMD)) {
		bp->b_error = ENXIO;
		bp->b_flags |= B_ERROR;
		iodone(bp);	
		return;
	}

	if (!initialized) {
		bp->b_error = EIO;
		bp->b_flags |= B_ERROR;
		iodone(bp);	
		return;
	}

	if (!(bp->b_flags & B_READ))
		while (frozen)
			sleep(&frozen, PZERO+1);

	if ((offset < 0) || (offset > bufsize)) {
		bp->b_error = EINVAL;
		bp->b_flags |= B_ERROR;
		iodone(bp);	
		return;
	}
	
	bp->b_resid = bp->b_bcount;
	addr = start_addr + offset;
	if (offset + bp->b_bcount >= bufsize)
		bp->b_bcount = bufsize - offset;

	DEBUGF(ramddebug, printf("unit %d,offset %d, count %d\n",
				  unit,offset,bp->b_bcount););
	DEBUGF(ramddebug, printf("blockno %d resid %d\n",
				  bp->b_blkno,bp->b_resid););

	if (bp->b_flags & B_READ)
		for (i=0; i<bp->b_bcount; i++)
		  bp->b_un.b_addr[i] = addr[i];
		/*  bcopy(addr, bp->b_un.b_addr, bp->b_bcount); */
	else
		for (i=0; i<bp->b_bcount; i++)
		  addr[i] = bp->b_un.b_addr[i];
		/* bcopy(bp->b_un.b_addr, addr, bp->b_bcount); */
	bp->b_resid -= bp->b_bcount;
	iodone(bp);
	return(0);
}

#define LOW_IO_MEM (char *) 0xf4000000
#define HI_IO_MEM (char *) 0xf5000000
#define IN_IO_MEM(a,s) ((a >= LOW_IO_MEM) && (a+s < HI_IO_MEM))
#define NUM_TIMES 100

set_ramd_bounds(start, size)
	char *start;
	int size;
{
/*
 * kmem_alloc allocates PAGE_SIZE blocks at a time, so size must be a 
 * multiple of PAGE_SIZE. kmem_alloc returns an address to the zero-filled
 *  pages, or if it doesn't work, it returns 0.
 */
	if (start == (char *) -1) {
		size = (size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;
		/* get wired down memory from mach */
		start = (caddr_t)kmem_alloc(kernel_map, size);
		if (start == 0) 
			return(EINVAL);
	}

	else if (IN_IO_MEM(start,size)) {
		/* need to validate the memory.. test every so often to
			see if it's been initialized. */

	  /* Assume every process to use ramd will initialize the device to what they
	   * want it to look like, so don't bzero, as data might be lost
	   *	bzero((caddr_t)start,size); 
	   */
/*
 * for now just assume it's valid too many problems to worry about
 * if it's not valid, a parity error should be generated, or some sort
 * of error. so establish an interrupt handler -- problem is that
 * for now, ramdisk is a pseudo device and as such cannot have interrupts...
 *	int step, valid = TRUE; 
 *	char *addr, contents = 'a'; 
 *
 *		step = size / NUM_TIMES;
 *		for (addr = start; (addr < start+size) && valid; addr+=0x400) {
 *			valid = (*addr == contents);
 *		}
 *		if (!valid)
 *			return(EINVAL);
 */	}
	else
		return(EINVAL);

/* Start and Size should now define a valid address range */
	start_addr = start;
	bufsize = size;
	initialized = TRUE;

	return(0);
}

ramdsize(qp)
	struct	mouseq	*qp;
{
	DEBUGF(ramddebug, printf("In function ramdsize\n"););
	return(0);
}

#endif	/* NRAMD */
