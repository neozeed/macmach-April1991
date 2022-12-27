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
 * $Log:	cons.c,v $
 * Revision 2.2  90/08/30  11:04:04  bohman
 * 	Created.
 * 	[90/08/29  12:40:32  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/cons.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/uio.h>

#include <mac2/act.h>

#include <mac2dev/font.h>
#include <mac2dev/key.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Devices.h>
#include <mac2slotmgr/Video.h>

#include <mac2dev/video.h>

/*
 * MAC2 console driver
 */

struct tty  cons;

#define	cons_vp  &video[0]

struct cons_data {
    vm_offset_t		cons_addr;	/* address of screen bitmap */
    long	 	cons_mem_x;	/* screen x width in pixels */
    long 		cons_mem_y;	/* screen y height in pixels */
    long 		cons_scr_x;	/* memory x width in pixels */
    long 		cons_scr_y;	/* memory y height in pixels */
    int			cons_xpos;	/* cursor x position */
    int			cons_ypos;	/* cursor y position */
    struct font		cons_font;	/* font structure */
    struct act		*cons_act;
    int			cons_flags;
#define	CONS_SCREEN_EXISTS	0x00000001
} cons_data;

int cnstart(), cninput(), cnoutput(), ttrstrt();

extern u_char	font_524[], font_521[];;

extern struct tty kbtty[]; /* XXX */

/*ARGSUSED*/
cnopen(dev, flag)
dev_t dev;
{
    register struct tty *tp;
    int error, first= 0;

    tp = &cons;
    tp->t_addr = 0;
    tp->t_oproc = cnstart;
    if ((tp->t_state&TS_ISOPEN) == 0) {
	first = 1;
	ttychars(tp);
	tp->t_flags = EVENP|ODDP|ECHO|XTABS|CRMOD;
	tp->t_ispeed = tp->t_ospeed = B9600;
	tp->t_state = TS_ISOPEN|TS_CARR_ON;
    }

    /*
     * allocate console output
     * activity
     */
    {
	register struct cons_data *cd = &cons_data;

	if (cd->cons_act == 0) {
	    cd->cons_act = (struct act *)makesoftact(cnoutput, SR_IPL0);
	    if (cd->cons_act == 0)
		panic("cnopen: can't allocate output activity");
	}
    }

    if (tp->t_state&TS_XCLUDE && u.u_uid != 0)
	return (EBUSY);
    error = ((*linesw[tp->t_line].l_open)(dev, tp));
    if (error)
    	return (error);

    key_init(); mouse_init();

    if (first && (kbtty[0].t_state&TS_ISOPEN) == 0) /* XXX */
	while (key_open(0, cninput, KEY_ASCII) == 0)
	    ;

    return (0);
}

/*ARGSUSED*/
cnclose(dev, flag)
dev_t dev;
{
    if ((kbtty[0].t_state&TS_ISOPEN) == 0) /* XXX */
        key_close(0);
    (*linesw[cons.t_line].l_close)(&cons);
    ttyclose(&cons);
    return (0);
}

/*ARGSUSED*/
cnread(dev, uio)
dev_t dev;
struct uio *uio;
{
    return ((*linesw[cons.t_line].l_read)(&cons, uio));
}

/*ARGSUSED*/
cnwrite(dev, uio)
    dev_t dev;
    struct uio *uio;
{
    return ((*linesw[cons.t_line].l_write)(&cons, uio));
}

/*ARGSUSED*/
cnioctl(dev, cmd, addr, flag)
dev_t dev;
caddr_t addr;
{
    register struct tty *tp = &cons;
    register error;
    
    error = (*linesw[tp->t_line].l_ioctl)(tp, cmd, addr);
    if (error >= 0)
	return (error);
    error = ttioctl(tp, cmd, addr, flag);
    if (error < 0)
	error = ENOTTY;
    return (error);
}

cnselect(dev, rw)
dev_t dev;
int rw;
{
    return (ttselect(dev, rw));
}

cninput(id, cmd, c, next)
{
    register struct tty *tp;

    tp = &cons;
    if (tp->t_state & TS_ISOPEN)
	if (cmd == KC_CHAR)
	    (*linesw[tp->t_line].l_rint)(c, tp);
}

cnstart(tp)
register struct tty *tp;
{
    if (tp->t_state & (TS_TIMEOUT|TS_BUSY|TS_TTSTOP))
	return;
    if (tp->t_outq.c_cc <= TTLOWAT(tp)) {
	if (tp->t_state&TS_ASLEEP) {
	    tp->t_state &= ~TS_ASLEEP;
	    wakeup((caddr_t)&tp->t_outq);
	}
	if (tp->t_wsel) {
	    selwakeup(tp->t_wsel, tp->t_state & TS_WCOLL);
	    tp->t_wsel = 0;
	    tp->t_state &= ~TS_WCOLL;
	}
    }
    if (tp->t_outq.c_cc == 0)
	return;

    tp->t_state |= TS_BUSY;
    softact(cons_data.cons_act, tp);
}

/*
 * Called at console output activity
 * at IPL0 to do actual output.
 */
cnoutput(tp)
register struct tty *tp;
{
    register c, s;

    for (;;) {
	s = spl7();
	if ((tp->t_state&TS_TTSTOP) || (tp->t_outq.c_cc == 0))
	    break;
	c = getc(&tp->t_outq);
	if (c <= 0177 || tp->t_flags&(RAW|LITOUT))
	    mac2_putc(c);
	else {
	    timeout(ttrstrt, (caddr_t)tp, (c&0177));
	    tp->t_state |= TS_TIMEOUT;
	    break;
	}
	splx(s);
    }

    tp->t_state &= ~TS_BUSY;
    if (tp->t_outq.c_cc <= TTLOWAT(tp)) {
	if (tp->t_state&TS_ASLEEP) {
	    tp->t_state &= ~TS_ASLEEP;
	    wakeup((caddr_t)&tp->t_outq);
	}
	if (tp->t_wsel) {
	    selwakeup(tp->t_wsel, tp->t_state & TS_WCOLL);
	    tp->t_wsel = 0;
	    tp->t_state &= ~TS_WCOLL;
	}
    }
    splx(s);
}

/*
 * Used by kernel printf() to
 * output characters.
 */
cnputc(c)
register c;
{
    register s = spl7();

    if (c == '\n')
	mac2_putc('\r');

    mac2_putc(c);
    splx(s);
}

/*
 * mac2_putc(c)
 *
 * displays the character on the screen.
 */
mac2_putc(c)
register c;
{
    register struct cons_data *cd = &cons_data;
    register struct font *fp = &cd->cons_font;
    extern panicstr;

    if ((cd->cons_flags&CONS_SCREEN_EXISTS) == 0)
	return;

    font_invert(fp, cd->cons_xpos, cd->cons_ypos);
    c &= 0177;
    switch (c) {
	case '\b':
	    if (cd->cons_xpos)
		cd->cons_xpos--;
	    font_char(fp, cd->cons_xpos, cd->cons_ypos, ' ');	/* XXX */
	    break;

	case '\r':
	    cd->cons_xpos = 0;
	    break;

	case '\n':
	    cd->cons_ypos++;
	    if (cd->cons_ypos > (fp->font_maxy-1)) {
		cd->cons_ypos = fp->font_maxy-1;
		font_scrollup(fp, 0, fp->font_maxy-1, 1);
	    }
	    break;

	/* provide some control with a simple termcap entry like
		ma|MacMach console:\
			:am:do=^J:le=^H:bs:co#89:li#28:\
			:nd=^L:up=^K:cl=^Z:sr=^R:dl=^X:
		for the 480*640 pixel screens
		little Mac's (like SE30) have co#71:li#19
		and the large 2 page screen is co#159:li#52
	*/
	case 013:	/* up=^K */
	    cd->cons_ypos--;
	    if (cd->cons_ypos < 0)
		cd->cons_ypos = 0;
	    break;

	case 014:	/* nd=^L */
	    cd->cons_xpos++;
	    if (cd->cons_xpos >= fp->font_maxx)
		cd->cons_xpos--;
	    break;

	case 032:	/* cl=^Z */
	    video_setup(cons_vp, FALSE);	/* XXX */
	    cons_clear();
	    cd->cons_xpos = 0;
	    cd->cons_ypos = 0;
	    font_invert(&cd->cons_font, cd->cons_xpos, cd->cons_ypos);
	    break;

	case 022:	/* sr=^R */
	    cd->cons_ypos--;
	    if (cd->cons_ypos < 0) {
		cd->cons_ypos = 0;
		font_scrolldown(fp, 0, fp->font_maxy-1, 1);
	    }
	    break;

	case 030:	/* dl=^X */
	    font_scrollup(fp, cd->cons_ypos, fp->font_maxy-1, 1);
	    break;

	default:
	    if (font_char(fp, cd->cons_xpos, cd->cons_ypos, c)) {
		cd->cons_xpos++;
		if (cd->cons_xpos >= fp->font_maxx) {
		    cd->cons_xpos = 0;
		    cd->cons_ypos++;
		    if (cd->cons_ypos > (fp->font_maxy-1)) {
			cd->cons_ypos = fp->font_maxy-1;
			font_scrollup(fp, 0, fp->font_maxy-1, 1);
		    }
		}
	    }
	    break;
    }
    font_invert(fp, cd->cons_xpos, cd->cons_ypos);
}

/*
 * cons_printf_init()
 *
 * Initialize console screen device so
 * that kernel printfs work.
 */
cons_printf_init()
{
    VPBlock v;

    if (video_cons_find(&v) == TRUE)
	cons_init(&v);
}

cons_init(v)
VPBlock	*v;
{
    register struct cons_data *cd = &cons_data;
    register struct video *vp = cons_vp;

    cd->cons_flags |= CONS_SCREEN_EXISTS;
    cd->cons_mem_x = 8*v->vpRowBytes;
    cd->cons_mem_y = v->vpBounds.bottom - v->vpBounds.top;
    cd->cons_scr_x = v->vpBounds.right - v->vpBounds.left;
    cd->cons_scr_y = v->vpBounds.bottom - v->vpBounds.top;
    cd->cons_addr = vp->video_devbase + v->vpBaseOffset;
    cons_clear();
    cons_font_set(LEFT_SHAVE, RIGHT_SHAVE, TOP_SHAVE, BOTTOM_SHAVE);
}


/*
 * Reset the console screen
 * after it has been used for
 * something else.  Called from
 * video_close();
 */
cons_reset()
{
    register struct cons_data *cd = &cons_data;
    register s;

    if (cd->cons_flags&CONS_SCREEN_EXISTS) {
	s = spl7();
	cons_clear();
	font_invert(&cd->cons_font, cd->cons_xpos, cd->cons_ypos);
	splx(s);
    }
}

/*
 * Clear the console screen
 */
cons_clear()
{
    register struct cons_data *cd = &cons_data;
    register int i, lines, width, inc;
    register char *cp;
    
    cp = (char *)cd->cons_addr;
    lines = cd->cons_scr_y - 1;
    width = (cd->cons_scr_x+(8*sizeof(long))-1)/(8*sizeof(long)) - 1;
    inc = (cd->cons_mem_x/(8*sizeof(long)) - width - 1)*sizeof(long);
    do {
	i = width;
	do {
	    *(long *)cp = 0;
	    cp += sizeof(long);
	} while(i--);
	cp += inc;
    } while (lines--);
}

cons_force()
{
    extern struct tty	*xcons_tp;

    xcons_tp = (struct tty *)0;
}

/*
 * Pick an appropriate font for
 * the console, and initialize
 * data structures.
 */
cons_font_set(left, right, top, bottom)
unsigned left, right, top, bottom;
{
    register struct fontrec *f;
    register struct cons_data *cd = &cons_data;
    register struct font *fp = &cd->cons_font;

    f = (struct fontrec *)font_524; /* Monaco 12 (slightly modified) */
    if ((cd->cons_scr_x - left - right) / f->fRectMax < 80)
	f = (struct fontrec *)font_521; /* Monaco 9 (slightly modified) */

    fp->font_pnt = f;
    fp->font_screen = cd;
    fp->font_inverse = 1;
    fp->font_height = f->chHeight;
    fp->font_theight = f->chHeight+f->leading;
    fp->font_width = f->fRectMax;
    fp->font_leading = f->leading;
    fp->font_rowwords = ((unsigned int)f->rowWords)<<4;
    fp->font_bitimage = (unsigned char *)&f->bitImage[0];
    fp->font_loctable = &f->bitImage[f->rowWords*fp->font_height];
    fp->font_owtable = &f->bitImage[f->rowWords*fp->font_height+
				    f->lastChar + 2 - f->firstChar];
    fp->font_linewidth = (fp->font_height+fp->font_leading) * cd->cons_mem_x;
    fp->font_maxx = (cd->cons_scr_x - left - right) / fp->font_width;
    fp->font_maxy = (cd->cons_scr_y / (fp->font_height+fp->font_leading)) - 
	top - bottom;
    fp->font_offset = (top * fp->font_linewidth) +
	(cd->cons_scr_x - (fp->font_width * fp->font_maxx))/2;
}
