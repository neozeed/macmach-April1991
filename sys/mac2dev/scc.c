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
 * $Log:	scc.c,v $
 * Revision 2.2  90/08/30  11:06:53  bohman
 * 	Created.
 * 	[90/08/29  12:52:34  bohman]
 * 
 */

#include <sys/types.h>

#include <mac2dev/scc.h>

#include <mac2/autoconf.h>

scc_dev_t	sccaddr;

/*
 * Dummy SCC serial driver
 * resets chip on every interrupt. (for now)
 */
sccinit(dp)
register struct devconf *dp;
{
    register scc_reg_t sc;

    sccaddr = (scc_dev_t)dp->dc_csr;

#ifdef notdef
    sc = scc_reg(sccaddr, b);
    sc->csr = 0x9;	/* register 9 */
    sc->csr = W9NV|W9BRESET;

    sc = scc_reg(sccaddr, a);
    sc->csr = 0x9;	/* register 9 */
    sc->csr = W9NV|W9ARESET;
#endif
    return (1);
}

int	sccdebug = 1;

sccintr()
{
    register scc_reg_t	sca, scb;

    sca = scc_reg(sccaddr, a);
    scb = scc_reg(sccaddr, b);

    if (sccdebug)
	printf("scc stat A %x B %x\n", sca->csr, scb->csr);

    scb->csr = 0x9;	/* register 9 */
    scb->csr = W9NV|W9BRESET;

    sca->csr = 0x9;	/* register 9 */
    sca->csr = W9NV|W9ARESET;
}
