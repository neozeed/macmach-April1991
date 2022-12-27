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
 * $Log:	clock.c,v $
 * Revision 2.2.1.1  90/09/07  00:55:48  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:17:26  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/clock.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <cputypes.h>
#include <macos_emul.h>

#include <sys/param.h>
#include <sys/time.h>
#include <sys/kernel.h>

#include <mach/mac2/frame.h>

#include <mac2/clock.h>
#include <mac2/act.h>

#include <mac2dev/via.h>

/*
 * Machine-dependent clock routines.
 */

/*
 * VIA Free running counter constant
 * for 60 Hz signal.
 *
 * 783360: The E clock is 783.36 kHz.
 *     60: Want a 60 Hz interrupt.
 *      2: Count every cycle (not every EDGE)
 */
#define	TIMECONST ((783360/60)/2)

struct timeval onesec_time;

/*
 * The 60 Hz clock activity list
 */
struct actlist	actclock;

/*
 * Start the real-time clock.
 */
startrtclock()
{
    VIA2_ADDR->acr = VIA_AC_T1CONT | VIA_AC_T1PB7;
    VIA2_ADDR->t1cl = TIMECONST;
    VIA2_ADDR->t1ch = TIMECONST >> 8;
    VIA1_ADDR->ier = VIA_IE_SET | VIA_IE_CA1;
}

/*
 * Initialze the 'time' variable from the
 * clock chip.  Also initialize the onesec
 * time variable and enable the 1 second
 * interrupt from the RTC.
 */
inittodr()
{
    time_t t, nt, rtc_gettime();

    rtc_init();
    for (t = 0;;) {
	nt = rtc_gettime();
	if (t == nt)
	    break;
	t = nt;
    }

    /*
     * The Macintosh Operating System
     * stores the time as local time,
     * so it has to be converted to GMT here.
     * Unfortunately, there is no (easy) way
     * of knowing whether the local time is
     * currently DST.  This means that if the
     * MacOS system time is set to DST the GMT value
     * calculated here will be 1 hour off.
     */
    time.tv_sec = t - (T0_DELTA - (tz.tz_minuteswest * 60));
    time.tv_usec = 0;
    onesec_time = time;
    VIA1_ADDR->ier = VIA_IE_SET | VIA_IE_CA2;
}

/*
 * Called to set the time of day clock
 * when the system time is adjusted.  MacMach
 * does not ever set the RTC time, the user
 * is expected to set it from MacOS to standard time
 * (i.e. NOT daylight savings time) for the current time zone.
 */
resettodr()
{
    /*
     * Reset the 1 second counter
     * to the new time.
     */
    onesec_time = time;
}

/*
 * Handle 60Hz clock ticks.
 */
clock_intr(frame)
register generic_exception_frame_t *frame;
{

    /*
     * Run activities in the
     * clock list.
     */
    if (CHECKACTLIST(actclock))
	doactlist(&actclock);

    /*
     * Pass clock interrupt to BSD
     * code.
     */
    hardclock(frame->f_normal.f_pc, frame->f_normal.f_sr);
    VIA1_ADDR->ifr = VIA_IF_CA1;
}

/*
 * Handles the 1Hz tick from the RTC chip.
 * The system clock is (incorrectly) at
 * the lowest priority.  This makes it
 * easy to lose system clock ticks. The
 * 1Hz tick is used to catch missed 60Hz
 * ticks.  The 1Hz tick is assumed to be
 * accurate.
 */
onesec_intr(frame)
register generic_exception_frame_t *frame;
{
    struct timeval delta;

    /*
     * Increment counter.
     */
    onesec_time.tv_sec += 1;

    /*
     * Calculate delta.
     */
    delta.tv_sec = onesec_time.tv_sec - time.tv_sec;
    delta.tv_usec = onesec_time.tv_usec - time.tv_usec;

    /*
     * Normalize delta.
     */
    if (delta.tv_sec > 0 && delta.tv_usec < 0) {
	delta.tv_sec -= 1;
	delta.tv_usec += 1000000;
    }
    else if (delta.tv_sec < 0 && delta.tv_usec > 0) {
	delta.tv_sec += 1;
	delta.tv_usec -= 1000000;
    }
      
    /*
     * Check time and correct if necessary
     */
    if (delta.tv_sec > 0)
	time = onesec_time;

    VIA1_ADDR->ifr = VIA_IF_CA2;
}


/*
 * RTC interface routines.
 */

#define RTC_CLOCK_LOW(vp)	(vp)->regb &= ~VIA_PORT_rTCClk
#define RTC_CLOCK_HIGH(vp)	(vp)->regb |= VIA_PORT_rTCClk

#define RTC_ENABLE(vp)		(vp)->regb &= ~VIA_PORT_rTCEnb
#define RTC_DISABLE(vp)		(vp)->regb |= VIA_PORT_rTCEnb

#define RTC_DATA_OUTPUT(vp)	(vp)->ddrb |= VIA_PORT_rTCData
#define RTC_DATA_INPUT(vp)	(vp)->ddrb &= ~VIA_PORT_rTCData

rtc_init()
{
    register via_reg_t vp = VIA1_ADDR;
    
    vp->acr &= ~VIA_AC_PBLATCH;
    vp->regb |= (VIA_PORT_rTCClk|VIA_PORT_rTCEnb);
    vp->ddrb &= ~VIA_PORT_rTCData;
    vp->ddrb |= (VIA_PORT_rTCClk|VIA_PORT_rTCEnb);
    rtc_wprot(1);
}

unsigned char
rtc_rcv()
{
    register via_reg_t vp = VIA1_ADDR;
    register unsigned char val;
    register i;

    val = 0;
    for (i = 0; i < 8; i++) {
	RTC_CLOCK_LOW(vp);
	val = (val << 1) | (vp->regb & VIA_PORT_rTCData);
	RTC_CLOCK_HIGH(vp);
    }
    return(val);
}

rtc_send(val)
register unsigned char val;
{
    register via_reg_t vp = VIA1_ADDR;
    register i;

    RTC_DATA_OUTPUT(vp);
    for (i = 0; i < 8; i++) {
	RTC_CLOCK_LOW(vp);
	vp->regb = (vp->regb & ~VIA_PORT_rTCData) |
	    ((val >> 7) & VIA_PORT_rTCData);
	val <<= 1;
	RTC_CLOCK_HIGH(vp);
    }
    RTC_DATA_INPUT(vp);
}

rtc_wprot(x)
int x;
{
    register via_reg_t vp = VIA1_ADDR;

    RTC_ENABLE(vp);
    rtc_send(0x35);		/* write protect */
    rtc_send(x? 0xd5: 0x55);
    RTC_DISABLE(vp);
}

time_t
rtc_gettime()
{
    register via_reg_t vp = VIA1_ADDR;
    register int i;
    time_t res;
    register unsigned char *cp;

    cp = &((unsigned char *)&res)[3];
    for (i = 0; i < 4; i++) {
	RTC_ENABLE(vp);
	rtc_send(0x81 | ((i << 2) & 0x0c));
	*cp-- = rtc_rcv();
	RTC_DISABLE(vp);
    }
    rtc_wprot(1);

    return (res);
}

rtc_writeparam(addr, data, len)
unsigned char addr;
unsigned char *data;
unsigned len;
{
    register via_reg_t vp = VIA1_ADDR;

    rtc_wprot(0);
    while (len-- > 0)
	if (addr < 0x10) {
	    RTC_ENABLE(vp);
	    rtc_send(0x41 | (addr++ << 2));
	    rtc_send(*data++);
	    RTC_DISABLE(vp);
	}
	else if (addr < 0x14) {
	    RTC_ENABLE(vp);
	    rtc_send(0x21 | (addr++ << 2));
	    rtc_send(*data++);
	    RTC_DISABLE(vp);
	}
    rtc_wprot(1);
}

rtc_readXpram(maddr, data, len)
unsigned maddr;
unsigned char *data;
unsigned len;
{
    register via_reg_t vp = VIA1_ADDR;
    unsigned char sect, addr;

    sect = (maddr >> 5) & 7;
    addr = maddr & 0x1f;
    while (len-- > 0) {
	RTC_ENABLE(vp);
	rtc_send(0xb8 | sect);
	rtc_send(addr++ << 2);
	*data++ = rtc_rcv();
	RTC_DISABLE(vp);
    }
    rtc_wprot(1);
}

rtc_writeXpram(maddr, data, len)
unsigned maddr;
unsigned char *data;
unsigned len;
{
    register via_reg_t vp = VIA1_ADDR;
    unsigned char sect, addr;

    sect = (maddr >> 5) & 7;
    addr = maddr & 0x1f;
    rtc_wprot(0);
    while (len-- > 0) {
	RTC_ENABLE(vp);
	rtc_send(0x38 | sect);
	rtc_send(addr++ << 2);
	rtc_send(*data++);
	RTC_DISABLE(vp);
    }
    rtc_wprot(1);
}
