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
 * $Log:	mem.c,v $
 * Revision 2.2  90/08/30  11:02:02  bohman
 * 	Created.
 * 	[90/08/29  11:40:52  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/mem.c
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)mem.c	7.1 (Berkeley) 6/5/86
 */

/*
 * Memory special file
 */
#include <ramdisk.h>

#include <sys/param.h>
#include <sys/errno.h>
#include <sys/buf.h>
#include <sys/uio.h>
#include <sys/mman.h>

#include <mach/vm_param.h>

#include <mac2/pmap_kmap.h>

mmread(dev, uio)
    dev_t dev;
    struct uio *uio;
{
    return (mmrw(dev, uio, UIO_READ));
}

mmwrite(dev, uio)
    dev_t dev;
    struct uio *uio;
{
    return (mmrw(dev, uio, UIO_WRITE));
}

mmrw(dev, uio, rw)
    dev_t dev;
    struct uio *uio;
    enum uio_rw rw;
{
    register struct iovec *iov;
    register c;
    int error = 0;

    switch (minor(dev)) {

/* minor device 0 is physical memory */
	case 0:
	    if ((uio->uio_offset+uio->uio_resid) > mem_size)
		goto fault;

	    error = uiomove(uio->uio_offset, uio->uio_resid, rw, uio);
	    break;

/* minor device 1 is kernel memory */
        case 1:
	    if (!kernacc(uio->uio_offset, uio->uio_resid,
			 rw == UIO_READ ? B_READ : B_WRITE))
		goto fault;

	    error = uiomove(uio->uio_offset, uio->uio_resid, rw, uio);
	    break;

/* minor device 2 is EOF/RATHOLE */
	case 2:
	    if (rw == UIO_READ)
		return (0);

	    while (uio->uio_resid > 0) {
		iov = uio->uio_iov;
		if (iov->iov_len == 0) {
		    uio->uio_iov++;
		    uio->uio_iovcnt--;
		    if (uio->uio_iovcnt < 0)
			panic("mmrw");
		    continue;
		}
		c = iov->iov_len;
		iov->iov_base += c;
		iov->iov_len -= c;
		uio->uio_offset += c;
		uio->uio_resid -= c;
	    }
	    break;

/* minor device 3 is for physical device mapping */
	case 3:
	    error = EFAULT;
	    break;
    }
    return (error);

fault:
    return (EFAULT);
}

/*
 * This driver has a minor device 3
 * which allows certain physical address
 * ranges to be mapped into a user address
 * space.  The mmregion structure defines
 * what physical address ranges can be mapped
 * and with what protection.
 *
 * NB: this table is sorted by the offset field.
 */
struct mmregion {
    vm_offset_t	    offset;
    vm_size_t	    size;
    int		    prot;
} mmregion[] = {
    { PMAP_ROM_MAP_ADDR, PMAP_ROM_MAP_SIZE, PROT_EXEC|PROT_READ },
    								/* MacOS ROM */
    { 0x50004000, (8*1024),	    PROT_READ|PROT_WRITE },	/* SCC */
    { 0x50014000, (8*1024),	    PROT_READ|PROT_WRITE },	/* ASC */
    { 0x50016000, (8*1024),	    PROT_READ|PROT_WRITE },	/* IWM */
    { 0x50f04000, (8*1024),	    PROT_READ|PROT_WRITE },	/* SCC */
    { 0x50f14000, (8*1024),	    PROT_READ|PROT_WRITE },	/* ASC */
    { 0x50f16000, (8*1024),	    PROT_READ|PROT_WRITE },	/* IWM */
    { PMAP_SLOT_MAP_ADDR, PMAP_SLOT_MAP_SIZE, PROT_READ|PROT_WRITE },
    								/* NuBus */
};

#define	mmregion_end	&mmregion[sizeof (mmregion)/sizeof (struct mmregion)]

mmmmap(dev, offset, prot)
dev_t			dev;
register vm_offset_t	offset;
register		prot;
{
    register struct mmregion *p;

    if (minor(dev) != 3)
	return (-1);

    for (p = mmregion; p < mmregion_end; p++) {
	if (offset < p->offset)
	    return (-1);

	if (offset < (p->offset+p->size))
	    break;
    }
    if (p >= mmregion_end)
	return (-1);

    if ((prot&p->prot) != prot)
	return (-1);

    return (mac2_btop(offset));
}

#if	RAMDISK
/*
 * simple ramdisk stuff - AWW 11/88
 */

extern	caddr_t	ramd_addr;
extern	int ramd_size;

/* block ramdisk stragegy */
ramdstrategy(bp) register struct buf *bp;
{
	register int bn, i;
	bn = bp->b_blkno;
	i = ramd_size - bn*DEV_BSIZE;
	if(i <= bp->b_bcount) {
		bp->b_resid = bp->b_bcount;	
		if(i < bp->b_bcount) {
			bp->b_error = EFAULT;
			bp->b_flags |= B_ERROR;
		}
		goto done;
	}
	if(bp->b_flags & B_READ)
		bcopy(ramd_addr+DEV_BSIZE*bn, bp->b_un.b_addr, bp->b_bcount);
	else
		bcopy(bp->b_un.b_addr, ramd_addr+DEV_BSIZE*bn, bp->b_bcount);
	bp->b_resid = 0;
done:
	iodone(bp);
}

/* raw ramdisk stuff */
ramdread(dev, uio)
    dev_t dev;
    struct uio *uio;
{
    int i;
    i = ramd_size - uio->uio_offset;
    if(i < 0)
	return(EFAULT);
    return(uiomove(ramd_addr+uio->uio_offset, i, UIO_READ, uio));
}

ramdwrite(dev, uio)
    dev_t dev;
    struct uio *uio;
{
    int i;
    i = ramd_size - uio->uio_offset;
    if(i < 0)
	return(EFAULT);
    return(uiomove(ramd_addr+uio->uio_offset, i, UIO_WRITE, uio));
}

/* move the ramdisk image to high memory and adjust parameters accordingly */
ramd_init()
{
	register caddr_t pi, po;
	register int i;

	if(ramd_size <= 0 || ramd_size >= mem_size/4 || ramd_addr+ramd_size > (caddr_t)last_addr) {
		ramd_size = 0;
		ramd_addr = 0;
		return;
	}
	if(ramd_addr) {
		/* there is an initial memory image to copy */
		for(pi = ramd_addr + ramd_size, po = (caddr_t)last_addr, i = ramd_size; --i >= 0; )
			*--po = *--pi; /* copy backwards since we are moving to higher mem */
		ramd_addr = po;
	} else
		ramd_addr = (int)last_addr - ramd_size;	/* empty ramdisk space */
	/* patch up the system memory parameters */
	last_addr -= ramd_size;
	mem_size -= ramd_size;
}
#endif
