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
 *	File: emul/patches/rtc.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach.h>

#include <sys/time.h>
#include <sys/file.h>

#include "emul.h"

/*
 * RTC Time and
 * Parameter RAM Traps
 */

/*
 * The delta (in seconds) between MacOS T(0) and U**X T(0)
 */
#define T0_DELTA (((365*(1970-1904))+((1970-1904)/4)+1)*24*60*60)

ReadDateTime(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    register unsigned long	t;
    struct timeval		time;
    struct timezone		tz;

    (void) gettimeofday(&time, &tz);

    t = time.tv_sec + T0_DELTA - (tz.tz_minuteswest*60);

    *(unsigned long *)0x20c = t;
    *(unsigned long *)(regs.a_0) = t;

    regs.d_0 = 0;
}

SetDateTime(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    regs.d_0 = 0;
}

#ifdef notdef
/*
 * These are currently (3/22/91) still
 * emulated in the kernel.  I was just
 * playing around one afternoon...
 */

typedef struct {
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
} SysParamType;

SysParamType	DefParam
    = { 0xa8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x6300, 0x388, 0x4c };

#define SysParam	((SysParamType *)0x1f8)
#define GetParam	((SysParamType *)0x1e4)

int	pram = -1;

InitUtil(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    register boolean_t		failed = FALSE;
    struct timeval		time;
    struct timezone		tz;

    if (pram < 0)
	pram = open("/tmp/mac_pram", O_RDWR | O_CREAT, 0600);

    if (pram < 0)
	failed = TRUE;
    else {
	(void) lseek(pram, 0, L_SET);

	if (read(pram,
		 GetParam,
		 sizeof (SysParamType)) != sizeof (SysParamType))
	    failed = TRUE;
	else if (GetParam->valid != DefParam.valid)
	    failed = TRUE;
    }

    if (failed) {
	bcopy(&DefParam, GetParam, sizeof (SysParamType));
	regs.d_0 = -88;	/* prInitErr */

	(void) lseek(pram, 0, L_SET);

	(void) write(pram, GetParam, sizeof (SysParamType));
    }

    bcopy(GetParam, SysParam, sizeof (SysParamType));

    (void) gettimeofday(&time, &tz);

    *(unsigned long *)0x20c = time.tv_sec + T0_DELTA - (tz.tz_minuteswest*60);

    if (!failed)
	regs.d_0 = 0;
}

WriteParam(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    if (pram < 0)
	pram = open("/tmp/mac_pram", O_RDWR | O_CREAT, 0600);

    if (pram < 0) {
	regs.d_0 = -87;	/* prWrErr */
	return;
    }

    (void) lseek(pram, 0, L_SET);

    if (write(pram, SysParam, sizeof (SysParamType)) < 0) {
	regs.d_0 = -87;	/* prWrErr */
	return;
    }

    bcopy(SysParam, GetParam, sizeof (SysParamType));

    regs.d_0 = 0;
}

ReadXPRam(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    union {
	unsigned long	arg;
	struct {
	    unsigned short	len;
	    unsigned short	addr;
	} y;
    } x;

    if (pram < 0)
	pram = open("/tmp/mac_pram", O_RDWR | O_CREAT, 0600);

    if (pram < 0) {
	regs.d_0 = -87;	/* prWrErr */
	return;
    }

    x.arg = regs.d_0;

    printf("ReadXPRam: addr %x len %d\n", x.y.addr, x.y.len);

    if (x.y.addr == 0x76 && x.y.len == 2)
	*(unsigned short *)regs.a_0 = 1;
    else {
	(void) lseek(pram, x.y.addr, L_SET);

	if (read(pram, regs.a_0, x.y.len) != x.y.len)
	    bzero(regs.a_0, x.y.len);
    }

    regs.d_0 = 0;
}

WriteXPRam(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    union {
	unsigned long	arg;
	struct {
	    unsigned short	len;
	    unsigned short	addr;
	} y;
    } x;

    if (pram < 0)
	pram = open("/tmp/mac_pram", O_RDWR | O_CREAT, 0600);

    if (pram < 0) {
	regs.d_0 = -87;	/* prWrErr */
	return;
    }

    x.arg = regs.d_0;

    printf("WriteXPRam: addr %x len %d\n", x.y.addr, x.y.len);

    (void) lseek(pram, x.y.addr, L_SET);

    if (write(pram, regs.a_0, x.y.len) != x.y.len) {
	regs.d_0 = -87;	/* prWrErr */
	return;
    }

    regs.d_0 = 0;
}
#endif
