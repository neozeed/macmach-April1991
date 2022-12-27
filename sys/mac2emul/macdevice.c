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
 *	File: mac2emul/macdevice.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Device emulation
 * routines
 */

#include <sys/types.h>

#include <vm/vm_map.h>

#include <mac2/pcb.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macdevice.h>

int (*device_emul_switch[])() = {
    device_via1,	/* 50f00000 */
    device_via2,	/* 50f02000 */
    device_scc,		/* 50f04000 */
    device_scsi_h,	/* 50f06000 */
    device_none,	/* 50f08000 */
    device_none,	/* 50f0a000 */
    device_none,	/* 50f0c000 */
    device_none,	/* 50f0e000 */
    device_scsi,	/* 50f10000 */
    device_scsi_d,	/* 50f12000 */
    device_sound,	/* 50f14000 */
    device_iwm,		/* 50f16000 */
    device_none,	/* 50f18000 */
    device_none,	/* 50f1a000 */
    device_none,	/* 50f1c000 */
    device_none,	/* 50f1e000 */
    device_null,	/* 50f20000 */
    device_none,	/* 50f22000 */
    device_null,	/* 50f24000 */
    device_none,	/* 50f26000 */
    device_none,	/* 50f28000 */
    device_none,	/* 50f2a000 */
    device_none,	/* 50f2c000 */
    device_none,	/* 50f2e000 */
    device_none,	/* 50f30000 */
    device_none,	/* 50f32000 */
    device_none,	/* 50f34000 */
    device_none,	/* 50f36000 */
    device_none,	/* 50f38000 */
    device_none,	/* 50f3a000 */
    device_none,	/* 50f3c000 */
    device_none		/* 50f3e000 */
};

valid_device(addr, data, size, rw)
register unsigned long addr;
register unsigned long *data;
register size, rw;
{
    if ((current_thread_pcb()->pcb_flags&MAC_24BIT) == 0) {
	if ((addr&DEV_VALID_MASK) == DEV_VALID_ADDR_1 ||
	    (addr&DEV_VALID_MASK) == DEV_VALID_ADDR_2) {
	    /* FALL THROUGH */
	}
	else
	    return (0);
    }
    else {
	addr &= 0x00ffffff;
	if ((addr&DEV_VALID_MASK) != DEV_VALID_ADDR_3)
	    return (0);
    }
    return ((*device_emul_switch[(addr&DEV_TYPE_MASK)>>DEV_TYPE_SHIFT])
	    (addr&DEV_OFFSET_MASK, data, size, rw));
}

device_none()
{
    return (0);
}

device_ones(offset, data, size, rw)
unsigned long	*data;
{
    if (rw == BUSERR_READ)
	*data = 0xffffffff;

    return (1);
}

device_null(offset, data, size, rw)
unsigned long	*data;
{
    if (rw == BUSERR_READ)
	*data = 0;

    return (1);
}
