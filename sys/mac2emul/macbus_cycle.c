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
 *	File: mac2emul/macbus_cycle.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <mac2/pcb.h>

#include <mac2dev/via.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macvia.h>
#include <mac2emul/macuser.h>

int	bus_cycle_count;
/*
 * Handle bus cycle errors.  Only
 * gets called for data cycle faults.
 */
macdevice_cycle(frame, regs)
register normal_exception_frame_t *frame;
register struct regs *regs;
{
    if (frame->f_fmt == STKFMT_SHORT_BUSERR) {
	register short_buserr_exception_frame_t *b =
	    (short_buserr_exception_frame_t *)frame;

	if (valid_device(b->f_fault, &b->f_dob, b->f_size, b->f_rw)) {
	    b->f_dfault = 0;
	    bus_cycle_count++;
	    return (1);
	}
	else
	    return (0);
    }
    else {
	register long_buserr_exception_frame_t *b =
	    (long_buserr_exception_frame_t *)frame;

	if (b->f_rw) {
	    if (valid_device(b->f_fault, &b->f_dib, b->f_size, b->f_rw)) {
		b->f_dfault = 0;
		bus_cycle_count++;
		return (1);
	    }
	    else
		return (0);
	}
	else {
	    if (valid_device(b->f_fault, &b->f_dob, b->f_size, b->f_rw)) {
		b->f_dfault = 0;
		bus_cycle_count++;
		return (1);
	    }
	    else
		return (0);
	}
    }
}

macram_cycle(frame, regs)
register normal_exception_frame_t *frame;
register struct regs *regs;
{
    if (frame->f_fmt == STKFMT_SHORT_BUSERR) {
	register short_buserr_exception_frame_t *b =
	    (short_buserr_exception_frame_t *)frame;

	if (b->f_dfault &&
	    wrapped_ram(b->f_fault, &b->f_dob, b->f_size, b->f_rw)) {
	    b->f_dfault = 0;
	    bus_cycle_count++;
	    return (1);
	}
	else
	    return (0);
    }
    else {
	register long_buserr_exception_frame_t *b =
	    (long_buserr_exception_frame_t *)frame;

	if (b->f_rw) {
	    if (b->f_dfault &&
		wrapped_ram(b->f_fault, &b->f_dib, b->f_size, b->f_rw)) {
		b->f_dfault = 0;
		bus_cycle_count++;
		return (1);
	    }
	    else
		return (0);
	}
	else {
	    if (b->f_dfault &&
		wrapped_ram(b->f_fault, &b->f_dob, b->f_size, b->f_rw)) {
		b->f_dfault = 0;
		bus_cycle_count++;
		return (1);
	    }
	    else
		return (0);
	}
    }
}


wrapped_ram(addr, data, size, rw)
register unsigned long addr;
register unsigned long *data;
register size, rw;
{
    register struct pcb	*pcb = current_thread_pcb();

    if (pcb->pcb_flags&MAC_24BIT)
	addr &= 0x00ffffff;

    if (addr > 0x08000000)
	return (0);

#ifdef notdef
    {
	static char	*F_size[] = { "LONG", "BYTE", "WORD", "????" };
	static char	*F_rw[] = { "WRITE", "READ" };
	static char	*F_bits[] = { "256K", "1M", "4M", "16M" };

	printf("%s wrapped_ram %s %s at %08x\n",
	       F_bits[(pcb->pcb_emul->via2.portA.reg &
		       (VIA_PORT_v2RAM1|VIA_PORT_v2RAM0)) >> 6],
	       F_size[size & 3],
	       F_rw[rw & 1],
	       addr);
    }
#endif
    switch (pcb->pcb_emul->via2.portA.reg&(VIA_PORT_v2RAM1|VIA_PORT_v2RAM0)) {
      case VIA_PORT_v2RAM1|VIA_PORT_v2RAM0:
	/* 16 Mbyte simms */
	if (addr >= 0x08000000) {
	    addr &= 0x07ffffff;
	    access_ram(addr, data, size, rw);
	}
	else if (addr >= 0x05000000) {
	    addr &= 0x05ffffff;
	    access_ram(addr, data, size, rw);
	} else if (addr >= 0x04400000) {
	    addr &= 0x044fffff;
	    access_ram(addr, data, size, rw);
	}
	else if (addr >= 0x04100000) {
	    addr &= 0x041fffff;
	    access_ram(addr, data, size, rw);
	}
	break;

      case VIA_PORT_v2RAM1:
	/* 4 Mbyte simms */
	if (addr >= 0x02000000) {
	    addr &= 0x01ffffff;
	    access_ram(addr, data, size, rw);
	}
	else if (addr >= 0x01400000) {
	    addr &= 0x014fffff;
	    access_ram(addr, data, size, rw);
	}
	else if (addr >= 0x01100000) {
	    addr &= 0x011fffff;
	    access_ram(addr, data, size, rw);
	}
	break;

      case VIA_PORT_v2RAM0:
	/* 1 Mbyte simms */
	if (addr >= 0x00800000) {
	    addr &= 0x007fffff;
	    access_ram(addr, data, size, rw);
	}
	else if (addr >= 0x00500000) {
	    addr &= 0x004fffff;
	    access_ram(addr, data, size, rw);
	}
	break;

      case 0:
	/* 256 Kbyte simms */
	if (addr >= 0x00200000) {
	    addr &= 0x001fffff;
	    access_ram(addr, data, size, rw);
	}
	break;
    }
    return (1);
}

access_ram(addr, data, size, rw)
register unsigned long addr;
register unsigned long *data;
register size, rw;
{
    unsigned long l;
    unsigned short s;
    unsigned char c;

    if (rw == BUSERR_READ)
	switch (size) {
	  case BUSERR_SIZE_BYTE:
	    FETCH(addr, &c, unsigned char);
	    *data = c;
	    break;
	    
	  case BUSERR_SIZE_WORD:
	    FETCH(addr, &s, unsigned short);
	    *data = s;
	    break;
	    
	  case BUSERR_SIZE_LONG:
	    FETCH(addr, &l, unsigned long);
	    *data = l;
	    break;
	}
    else
	switch (size) {
	  case BUSERR_SIZE_BYTE:
	    c = *data;
	    STORE(addr, &c, unsigned char);
	    break;
	    
	  case BUSERR_SIZE_WORD:
	    s = *data;
	    STORE(addr, &s, unsigned short);
	    break;
	    
	  case BUSERR_SIZE_LONG:
	    l = *data;
	    STORE(addr, &l, unsigned long);
	    break;
	}
}
