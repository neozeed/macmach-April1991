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
 *	File: mac2emul/macscc.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/types.h>

#include <vm/vm_map.h>

#include <mac2/pcb.h>

#include <mac2dev/scc.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macdevice.h>

#define	SCC_CHAN_OFF	0x02
#define SCC_CHAN_B	0
#define SCC_CHAN_A	SCC_CHAN_OFF

#define SCC_REG_OFF	0x04
#define SCC_REG_CSR	0
#define SCC_REG_DATA	SCC_REG_OFF

static unsigned char	reg_data[2];

device_scc(offset, data, size, rw)
unsigned long	offset, *data;
int		size, rw;
{
    register scc_reg_t		sc;
    register unsigned char	*reg;

    if ((macdebug&0x10) == 0)
	return (0);

    if ((offset&SCC_CHAN_OFF) == SCC_CHAN_A) {
	sc = scc_reg(sccaddr, a);
	reg = &reg_data[0];
    }
    else {
	sc = scc_reg(sccaddr, b);
	reg = &reg_data[1];
    }

    if (rw == BUSERR_READ) {
	switch (offset&SCC_REG_OFF) {
	  case SCC_REG_DATA:
	    *data = sc->data;
	    break;

	  case SCC_REG_CSR:
	    switch (*reg) {
	      case 0:
		*data = sc->csr;
		break;

	      default:
		sc->csr = *reg;
		*data = sc->csr;
		*reg = 0;
		break;
	    }
	    break;
	}
	if (macdebug&0x08)
	    printf("scc r o %x d %x\n", offset, *data&0xff);
    }
    else {
	register unsigned char	dat;

	if (macdebug&0x08)
	    printf("scc w o %x d %x\n", offset, *data&0xff);

	switch (offset&SCC_REG_OFF) {
	  case SCC_REG_DATA:
	    sc->data = *data;
	    break;

	  case SCC_REG_CSR:
	    switch (*reg) {
	      case 0:
		dat = *data;
		if (dat < W0REXT)
		    *reg = dat;
		else
		    sc->csr = dat;
		break;

	      case 9:
		dat = *data;
		dat &= ~W9MIE;
		if ((dat&W9HRESET) == W9HRESET)
		    dat &= ~W9HRESET;
		sc->csr = *reg;
		sc->csr = dat;
		*reg = 0;
		break;

	      case 4:
		dat = *data;
		if (dat == 0x20) {
		    sc->csr = *reg;
		    sc->csr = dat;
		}
		*reg = 0;
		break;
		
	      default:
		sc->csr = *reg;
		sc->csr = *data;
		*reg = 0;
		break;
	    }
	    break;
	}
    }

    return (1);
}
