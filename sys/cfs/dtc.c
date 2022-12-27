/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	dtc.c,v $
 * Revision 2.2  90/07/05  11:27:28  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.1  90/05/31  17:02:22  dcs
 * Initial revision
 * 
 * 
 */

#include <dtc.h>
#if	NDTC > 0

/*
 * Device driver for the Data Translation DT2806 (clock board).
 *
 * The CODA project was not satisfied with the granularity of the clock used
 * by Gettimeofday(2). This driver was developed to provice a better clock.
 * The board is configured to increment a 48 bit counter every 1/2
 * microsecond. Because of the size of the counter, the count should never
 * reach the end, and so we really don't have to worry about interrupts.
 * When the clock is initialized, the driver saves the current time. The routine 
 * dtcmicrotime calculates the current time by adding the number of
 * 1/2 usecond ticks that have occured since the clock was started to the
 * starting time (base time). the dtcpresent flag indicates that the board was
 * present at boot time and so this driver should be used. If the autoconf routine
 * at boottime doesn't see the board, the kernel will use the old method for
 * telling time.
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/kernel.h>
#include <mach/boolean.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/buf.h>
#include <ca/debug.h>
#include <ca/io.h>
#include <caio/ioccvar.h>
#include <cfs/dtcreg.h>

/* Debugging flag */
int dtcdebug;

/* Kernel-wide flag to indicate the dtc device is present */
int dtcpresent = FALSE;
int dtcexists = FALSE;

/* variable to hold the clock data for the kernel */
struct timeval dtc_basetime;

struct iocc_device *dtcdinfo[NDTC];

int dtcprobe(), dtcattach(), dtcint(), dtc_clock_start();

struct iocc_driver dtcdriver = { 
	/* probe slave attach dgo addr dname dinfo mname minfo intr csr */
	dtcprobe, 0, dtcattach, 0, 0, "dtc", dtcdinfo, 0, 0, dtcint, 0x1400
};

/* Internal structure to hold the counter/timer values */
struct dtc_counters {
	long timer2, timer1, timer0
};

/*
 * dtcprobe currently doesn't even try to generate an interrupt. what needs
 * to be done is to reset c/t 0 to 1 c/t 1 to 1 and c/t 2 to 1. i hope that 
 * will have the device interrupt in 1/2 microseconds. the other option is
 * to just use PROBE_NOINT, since it works fine so far.
 */
/* ARGSUSED */
dtcprobe(addr)
	register caddr_t addr;
{
	DEBUGF( dtcdebug & DTCDEBUG, printf("In dtcprobe\n"));
	dtc_clock_start();
	return(PROBE_NOINT);
}


dtcattach(iod)
	register struct iocc_device *iod;
{
	DEBUGF( dtcdebug & DTCDEBUG, printf("in dtcattach\n"));
	dtcpresent = TRUE;
	dtcexists = TRUE;
}


/*ARGSUSED*/
dtcopen(dev, flag)
	dev_t dev;
{
	DEBUGF( dtcdebug & DTCDEBUG, printf("in dtcopen\n"));
}

	

/*ARGSUSED*/
dtcclose(dev)
	dev_t dev;
{
	DEBUGF( dtcdebug & DTCDEBUG, printf("in dtcclose\n"));
}


dtcread(dev, uio)
	register dev_t dev;
	register struct uio *uio;
{
	DEBUGF( dtcdebug & DTCDEBUG, printf("in dtcread\n"));
	return(0);
}


/*
 * Initialize the DT 2806 timer. Also reset the time of day stored in 
 * dtc_basetime.
 */
dtc_clock_start()
{
	char *fpla = FPLA, *modereg = ModeReg;
	char *timer0 = Timer0, *timer1 = Timer1, *timer2 = Timer2;

	*fpla = 0x00;
	*modereg = 0xB0;
	*timer2 = 0x0;
	*timer2 = 0x0;
	*modereg = 0x74;
	*timer1 = 0x0;
	*timer1 = 0x0;
	*modereg = 0x34;
	*timer0 = 0x0;
	*timer0 = 0x0;
	*fpla = 0xF8;
	*fpla = 0xC0;
	*fpla = 0xF;

	/* get the time of day */
	dtc_basetime.tv_usec = time.tv_usec;
	dtc_basetime.tv_sec = time.tv_sec;

	DEBUGF( dtcdebug & DTCDEBUG, 
		printf("in dtc_clock_start sec %d usec %d\n",
			dtc_basetime.tv_sec,dtc_basetime.tv_usec));
}

/* Get the time from the counters. Each counter can be read 8 bits at a time,
 * with the low 8 getting read first. I use the masks to insure any stray 
 * bits do not interfere. The counters start at 0, go to FFFF, then decrement
 * back to 0. So a 0 register could either mean nothing or 0x10000 events have
 * passed. I treat it like 0x10000, because the next counter in line should
 * have caught the 17th bit.
 *
 * Latch the counter value in a storage register to prevent overflow between
 * the read of the low 8 bits and the read of the high 8 bits.  (Latch occurs
 * because of the writes to the mode register.)  Also, require two consecutive
 * reads to have the same values for the high and middle counters (c/t 2 and
 * c/t 1 respectively) and require the second read to have a greater value than
 * the first read on the low counter (c/t 0).  This requirement ensures that no
 * overflows occurred between the registers while reading.  (Necessary because
 * overflow will occur every 32 usecs between c/t 0 and c/t 1.)
 */

#define DTCLOWMASK(n)	(n & 0xFF)
#define DTCMASK(n)	(n & 0xFFFF)

#define MAX_TRIES 10

dtc_read_clock(data)
	struct dtcclk *data;
{
        int index, correct_read;
	long tempa, tempb;
	char *modereg = ModeReg;
	struct dtc_counters value1, value2;

	index = 0;
	correct_read = FALSE;
	/* Loop no more than MAX_TRIES times to obtain a valid read */
	/* Note that after MAX_TRIES times the read value may be incorrect
	 * but this is better than an infinite loop... */
	while ( (!correct_read) && (index < MAX_TRIES) ) {
	  /* Read once */
	  *modereg = 0x80;
	  tempa =  DTCLOWMASK(*Timer2);
	  tempb =  DTCLOWMASK(*Timer2);
	  value1.timer2 = DTCMASK(0x10000 - ((tempb << 8) | tempa));
	  *modereg = 0x40;
	  tempa =  DTCLOWMASK(*Timer1);
	  tempb =  DTCLOWMASK(*Timer1);
	  value1.timer1 = DTCMASK(0x10000 - ((tempb << 8) | tempa));
	  *modereg = 0x00;
	  tempa =  DTCLOWMASK(*Timer0);
	  tempb =  DTCLOWMASK(*Timer0);
	  value1.timer0 = DTCMASK(0x10000 - ((tempb << 8) | tempa));

	  /* Read again */
	  *modereg = 0x80;
	  tempa =  DTCLOWMASK(*Timer2);
	  tempb =  DTCLOWMASK(*Timer2);
	  value2.timer2 = DTCMASK(0x10000 - ((tempb << 8) | tempa));
	  *modereg = 0x40;
	  tempa =  DTCLOWMASK(*Timer1);
	  tempb =  DTCLOWMASK(*Timer1);
	  value2.timer1 = DTCMASK(0x10000 - ((tempb << 8) | tempa));
	  *modereg = 0x00;
	  tempa =  DTCLOWMASK(*Timer0);
	  tempb =  DTCLOWMASK(*Timer0);
	  value2.timer0 = DTCMASK(0x10000 - ((tempb << 8) | tempa));

	  /* Check for valid read */
	  if ( (value1.timer2 == value2.timer2) &&
	       (value1.timer1 == value2.timer1) &&
	       (value1.timer0 <  value2.timer0) )
	    correct_read = TRUE;
	  index++;
	}

	DEBUGF( dtcdebug & DTCDEBUG, 
		printf("In dtc_read_clock: timer2 %x timer1 %x timer0 %x\n",
			value1.timer2,value1.timer1,value1.timer0));

 	/* Make sure that we have a valid read */
 	if (index < MAX_TRIES) {
  	  data->dtc_low = (value1.timer1 << 16) | value1.timer0;
	  data->dtc_high = value1.timer2;
	}
	else {
	  data->dtc_low = 0;
	  data->dtc_high = 0;
	}
}

/*ARGSUSED*/
dtcioctl(dev, cmd, addr, flag)
	dev_t dev;
	struct dtcclk *addr;
{
	int unit = minor(dev);

	struct timeval *basetime;

	if ((unit < 0) || (unit >= NDTC)) {
		printf("invalid device minor number\n");
		return(ENXIO);
	}

	switch(cmd) {
		case DTCSCLK :
			DEBUGF(dtcdebug & DTCDEBUG, 
				printf("in dtcioctl DTCSCLK\n"));
			dtc_clock_start();
			break;

		/* DTCCTL returns control of the DT2806 board to the kernel */
 	        case DTCCTL :
			/* Check to make sure the board was present */
  			if (dtcexists)
		          dtcpresent = TRUE;
		        break;

		/* DTCRLS releases control of the DT2806 board to the caller */
		case DTCRLS :
			DEBUGF(dtcdebug&DTCDEBUG, 
				printf("in dtcioctl DTCRLS\n"));
			basetime = (struct timeval *)addr;
		        if (dtcpresent == TRUE) {
			  dtcpresent = FALSE;
			  basetime->tv_usec = dtc_basetime.tv_usec;
			  basetime->tv_sec = dtc_basetime.tv_sec;
			}
			else {
			  basetime->tv_usec = 0;
			  basetime->tv_sec = 0;
			  return(-1);
			}
		        break;

                /* DTCRCLK provides a valid read for the counter/timers */
		case DTCBCLK :  /* = DTCRCLK -- so we don't break things */
		case DTCRCLK :
			DEBUGF(dtcdebug&DTCDEBUG, 
				printf("in dtcioctl DTCRCLK\n"));
			dtc_read_clock(addr);
			break;

		default :
			return(EINVAL);
	}

	return(0);
}

/*
 * Setting the dtc parameters (baud rate, data length, etc...)
 */
dtcparam(unit)
	register int unit;
{
	DEBUGF( dtcdebug & DTCDEBUG, printf("in dtcparam\n"));
}

/* 
 * ctlr specifies the ctlr that handled the interrupt. I don't know why
 * we'd ever get anything but the iocc. Also, the IRQ for the board
 * is 4. (hopefully) I don't think i have to reset anything...
 * famous last words.
 */
dtcint(ctlr,irq)
	register int ctlr, irq;
{
	char *modereg = ModeReg;

	DEBUGF(dtcdebug&DTCDEBUG,printf("in dtcint ctlr %d irq %d\n",ctlr,irq));
	if (irq != 4)
		return(1);		/* ignore it */

	*modereg = 0x70;		/* reset interrupt */
	return(0);
}


/*
 * The CODA project wasn't convinced that the current gettimeofday returned
 * a very accurate system time (its granularity was basically 16 milliseconds
 * or a clock tick), so we installed a DT2806 clock board, which has the
 * resolution of 1/2 microsecond. If the driver (dtc) is present, use it,
 * otherwise use the old version.
 */

/*
 * These lines are needed if this routine is ever moved back to machdep.c.
 *  #if NDTC > 0
 * #include <cfs/dtcreg.h>
 */
unsigned long	dtcsec, dtcusec;

dtcmicrotime(tvp)
	register struct timeval *tvp;
{
	extern struct timeval dtc_basetime;
	extern int dtcdebug;
	struct dtcclk clock_offset;

	dtc_read_clock(&clock_offset);

	DEBUGF(dtcdebug & DTCDEBUG,
		 printf("low %x high %x\n",clock_offset.dtc_low,
			 clock_offset.dtc_high));
/*
 * dtcdiv is a custom written assembler routine (in cfs/dtcdiv.s) which
 * takes a 48 bit count of 1/2 usec and returns the equal number of seconds
 * and microseconds.
 */
	dtcusec = clock_offset.dtc_low;
	dtcsec = clock_offset.dtc_high;
	dtcdiv();

	DEBUGF( dtcdebug & DTCDEBUG, printf("usec %d sec %d\n",dtcusec,dtcsec));

	tvp->tv_usec = dtc_basetime.tv_usec + dtcusec;

	if (tvp->tv_usec >= 1000000) {
		dtcsec++;
		tvp->tv_usec -= 1000000;
	}
	tvp->tv_sec = dtc_basetime.tv_sec + dtcsec;
}

#endif	/* NDTC */
