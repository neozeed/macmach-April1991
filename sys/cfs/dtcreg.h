/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	dtcreg.h,v $
 * Revision 2.2  90/07/05  11:27:38  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.1  90/05/31  17:04:40  dcs
 * Initial revision
 * 
 * 
 */

#ifndef _DTCREG_H_
#define _DTCREG_H_

/* location of major registers on Data Translation 2806 board */

#define	Timer0	(caddr_t) 0xF0000370
#define Timer1	(caddr_t) 0xF0000770
#define Timer2	(caddr_t) 0xF0000B70
#define ModeReg (caddr_t) 0xF0000F70
#define FPLA	(caddr_t) 0xF0001770

/*
 * Support for the IOCTLs for DTC .
 */
struct dtcclk {
	long dtc_low, dtc_high;
	};

#define DTCSCLK	_IO(d,1)		/* set device bounds */
#define DTCBCLK	_IOR(d,3, struct dtcclk) /* return bound settings */
#define DTCRCLK	_IOR(d,2, struct dtcclk) /* return bound settings */
#define DTCRLS  _IOR(d,4, struct timeval)/* return base time value */ 
#define DTCCTL  _IO(d,5)

/* debugging values */

#define DTCDEBUG 0x1			/* turn on debugging in dtc.c */

#endif _DTCREG_H_
