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
 * $Log:	kb.c,v $
 * Revision 2.2  90/08/30  11:06:19  bohman
 * 	Created.
 * 	[90/08/29  12:46:06  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/kb.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <sys/file.h>

#include <mac2/act.h>

#include <mac2dev/key.h>
#include <mac2dev/mouse.h>

/*
 * Macintosh keyboard
 * driver.
 */

#define NKB 1

struct tty kbtty[NKB];

int kbinput(), kbmsinput();
extern nulldev();

struct actlist	actkb;

extern struct tty cons; /* XXX */
extern cninput(); /* XXX */

kbopen(dev, flag)
dev_t dev;
{
    register struct tty *tp;
    register error, unit;
    
    if (minor(dev) > (NKB-1))
	return (ENXIO);
    
    unit = minor(dev);
    tp = &kbtty[unit];
    
    if (tp->t_state&TS_ISOPEN)
	return (EBUSY);

    tp->t_pgrp = u.u_procp->p_pgrp;
    tp->t_addr = 0;
    tp->t_oproc = nulldev;
    tp->t_flags = RAW;
    tp->t_ispeed = tp->t_ospeed = B9600;
    tp->t_state = TS_ISOPEN|TS_CARR_ON|TS_NBIO;
    
    error = ((*linesw[tp->t_line].l_open)(dev, tp));
    if (error)
	return (error);
    
    key_init();
    mouse_init();
    
    while (key_open(unit, kbinput, KEY_ARAW) == 0)
	;
    
    while (mouse_open(unit, kbmsinput, 1) == 0)
	;
    
    return (0);
}

kbclose(dev, flag)
dev_t dev;
{
    register struct tty *tp;
    register unit;
    
    unit = minor(dev);
    tp = &kbtty[unit];
    
    if (unit == 0 && (cons.t_state&TS_ISOPEN)) {
	while (key_open(unit, cninput, KEY_ASCII) == 0)
	    ;
    } else
	key_close(unit);
    
    mouse_close(unit);
    
    (*linesw[tp->t_line].l_close)(tp);
    ttyclose(tp);
    return (0);
}

kbread(dev, uio)
dev_t dev;
struct uio *uio;
{
    register struct tty *tp = &kbtty[minor(dev)];
    
    return ((*linesw[tp->t_line].l_read)(tp, uio));
}

kbioctl(dev, cmd, addr, flag)
dev_t dev;
caddr_t addr;
{
    register struct tty *tp = &kbtty[minor(dev)];
    register error;
    
    error = (*linesw[tp->t_line].l_ioctl)(tp, cmd, addr);
    if (error >= 0)
	return (error);
    error = ttioctl(tp, cmd, addr, flag);
    if (error < 0)
	error = ENOTTY;
    return (error);
}

kbselect(dev, rw)
dev_t dev;
int rw;
{
    if (rw == FWRITE)
	return (FALSE);
    
    return (ttselect(dev, rw));
}

kbinput(id, cmd, c)
{
    register struct tty *tp = &kbtty[id];
    
    if (tp->t_state & TS_ISOPEN) {
	if (tp->t_rawq.c_cc > TTYHOG)
	    return;

	putc(c, &tp->t_rawq);
	ttwakeup(tp);
	    
	if (CHECKACTLIST(actkb))
	    doactlist(&actkb);
    }
}

kbmsinput(id, cmd, v)
{
    register struct tty *tp = &kbtty[id];

    if (cmd != MOUSE_CHANGE)
	return;

    if (tp->t_state & TS_ISOPEN) {
	if ((tp->t_rawq.c_cc + 3) > TTYHOG)
	    return;

	putc(MOUSE_ESCAPE, &tp->t_rawq);
	putc(((v>>8)&0xff), &tp->t_rawq);
	putc((v&0xff), &tp->t_rawq);
	ttwakeup(tp);
	    
	if (CHECKACTLIST(actkb))
	    doactlist(&actkb);
    }
}
