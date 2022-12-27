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
 *	File: mac2emul/macostrap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <kern/thread.h>

#include <machine/psl.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macinst.h>
#include <mac2emul/macostrap.h>
#include <mac2emul/macuser.h>

int	atrapdebug = 0;

/*
 * Handle a kernel emulated
 * MacOS traps. This code
 * is on its way out.
 */
mac_osinst(frame, regs)
register normal_exception_frame_t *frame;
register struct regs *regs;
{
    register (*vector)(), autopop = 0;
    unsigned short inst;
    register struct {
	unsigned short :4,
      		   code:12;
    } *i = &inst;

    FETCH(frame->f_pc, &inst, unsigned short);

    if (atrapdebug)
	printf("[%04x]", inst);

    if ((i->code&(OSTRAP_TBOX_TYPE|OSTRAP_TBOX_APOP)) ==
	(OSTRAP_TBOX_TYPE|OSTRAP_TBOX_APOP)) {
	i->code &= ~OSTRAP_TBOX_APOP;
	autopop++;
    }

    vector = mac_osinst_vector[i->code];

    if (vector) {
	register struct status_reg *sr;

	if (autopop) {
	    FETCH(regs->r_sp, &frame->f_pc, unsigned long);
	    regs->r_sp += sizeof (long);
	}
	else
	    frame->f_pc += sizeof (inst);
	(*vector)(frame, regs);

	sr = (struct status_reg *)&frame->f_sr;
	sr->sr_cc &= SR_X;
	if ((short)regs->r_r0 == 0)
	    sr->sr_z = 1;
	else
	if ((short)regs->r_r0 < 0)
	    sr->sr_n = 1;

	return (1);
    }

    panic("mac_osinst");
}

struct SysParam {
    unsigned char valid;
    unsigned char aTalkA;
    unsigned char aTalkB;
    unsigned char config;
    unsigned short portA;
    unsigned short portB;
    unsigned long alarm;
    unsigned short font;
    unsigned short kbdPrint;
    unsigned short volClik;
    unsigned short misc;
};

#include <sys/kernel.h>

#include <mac2/clock.h>

InitUtil(frame, regs)
register struct regs *regs;
{
    unsigned long t;

    STORE(0x1f8, 0x1f8, struct SysParam);

    t = time.tv_sec + T0_DELTA - (tz.tz_minuteswest*60);
    STORE(0x20c, &t, unsigned long);

    regs->r_r0 = 0; /* noErr */
}

ReadDateTime(frame, regs)
register struct regs *regs;
{
    unsigned long t;

    t = time.tv_sec + T0_DELTA - (tz.tz_minuteswest*60);

    STORE(0x20c, &t, unsigned long);
    STORE(regs->r_areg[0], &t, unsigned long);

    regs->r_r0 = 0; /* noErr */
}

SetDateTime(frame, regs)
register struct regs *regs;
{
    regs->r_r0 = 0; /* noErr */
}

WriteParam(frame, regs)
register struct regs *regs;
{
    FETCH(0x1f8, 0x1f8, struct SysParam);

    rtc_writeparam(0x0, 0x1f8, sizeof (struct SysParam));

    regs->r_r0 = 0; /* noErr */
}

ReadXPram(frame, regs)
register struct regs *regs;
{
    union {
	unsigned long	arg;
	struct {
	    unsigned short	len;
	    unsigned short	addr;
	} y;
    } x;
    unsigned long scratch;

    x.arg = regs->r_r0;
    if (x.y.len <= sizeof (scratch)) {
	rtc_readXpram(x.y.addr, &scratch, x.y.len);
	STORE_N(regs->r_areg[0], &scratch, x.y.len);
    }
    else {
	scratch = kalloc(x.y.len);

	rtc_readXpram(x.y.addr, scratch, x.y.len);
	STORE_N(regs->r_areg[0], scratch, x.y.len);

	kfree(scratch, x.y.len);
    }	

    regs->r_r0 = 0; /* noErr */
}

WriteXPram(frame, regs)
register struct regs *regs;
{
    union {
	unsigned long	arg;
	struct {
	    unsigned short	len;
	    unsigned short	addr;
	} y;
    } x;
    unsigned long scratch;

    x.arg = regs->r_r0;
    if (x.y.len <= sizeof (scratch)) {
	FETCH_N(regs->r_areg[0], &scratch, x.y.len);
	rtc_writeXpram(x.y.addr, &scratch, x.y.len);
    }
    else {
	scratch = kalloc(x.y.len);

	FETCH_N(regs->r_areg[0], scratch, x.y.len);
	rtc_writeXpram(x.y.addr, scratch, x.y.len);

	kfree(scratch, x.y.len);
    }	

    regs->r_r0 = 0; /* noErr */
}
