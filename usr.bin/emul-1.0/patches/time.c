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
 *	File: emul/patches/time.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach.h>

#include <mac2emul/macdefs.h>

#include "mac.h"

#include "emul.h"

/*
 * Emulate the Time Manager
 *
 * Please note: This isn't really finished
 * yet, but there is enough support
 * so that the sound manager works well.
 * MacTCP 1.0.1 also works, however it
 * really slows the machine down due to
 * the 10 ms task it installs.  I suspect
 * that the VIA register accesses from
 * this module is what is causing the
 * problem.  In any case, this will
 * be done differently in the new kernel.
 */

extern port_t	kernel_request_port;

extern	TIM1vec[], TIM2vec[];

typedef struct tm {
    struct tm		*tmLink;
    unsigned short	tmType;
    int			(*tmAddr)();
    int			tmCount;
} TMTask, *TMTaskPtr;

struct {
    unsigned short	qFlags;
    TMTaskPtr		qHead, qTail;
} TMQ = { 0, 0, 0 }, TmpQ = { 0, 0, 0 };

#define TIMECONST	((783360/1000)/2)

#define VIA(r)	*((unsigned char *)(0x50f00000 + (r)))
#define VIAW(r)	*((unsigned short *)(0x50f00000 + (r)))
#define T2CL	(0x1000)
#define T2CH	(0x1200)
#define IFR	(0x1a00)

#define TimerMax	0xffff

union {
    unsigned short	count;
    struct {
	unsigned char	hicount;
	unsigned char	locount;
    } reg;
} Timer2val = { TimerMax }, Tmpval = { TimerMax };
#define high_count	reg.hicount
#define low_count	reg.locount

boolean_t	TimeInited = FALSE;

TimeReset()
{
    TimeInited = FALSE;

    TMQ.qTail = TMQ.qHead = 0;
    Timer2val.count = TimerMax;
}

TimeIntr()
{
    register TMTaskPtr		tp, *ppl;
    register unsigned short	count;
    register			s = splhigh();
    
    TMQ.qFlags = 1;

    count = Timer2val.count;
    Timer2val.count = TimerMax;

    ppl = &TMQ.qHead;
    tp = *ppl;

    while (tp != 0) {
	tp->tmCount -= count;
	if (tp->tmCount <= 0) {
	    *ppl = tp->tmLink;
	    if (TMQ.qTail == tp)
		TMQ.qTail = (TMTaskPtr)ppl;

	    asm volatile ("movl %0,a1; jsr %1@"
			  : : "r" (tp), "a" (tp->tmAddr) :
			  "a0", "a1", "a2", "a3", "d0", "d1", "d2", "d3" );
	}
	else {
	    if (tp->tmCount < Timer2val.count)
		Timer2val.count = tp->tmCount;

	    ppl = &tp->tmLink;
	}

	tp = *ppl;
    }

    if (TmpQ.qHead != 0) {
	
	if (TMQ.qHead != 0) {
	    TMQ.qTail->tmLink = TmpQ.qHead;
	    TMQ.qTail = TmpQ.qTail;
	}
	else {
	    TMQ.qHead = TmpQ.qHead;
	    TMQ.qTail = TmpQ.qTail;
	}
	TmpQ.qHead = TmpQ.qTail = 0;
	
	if (Tmpval.count < Timer2val.count)
	    Timer2val.count = Tmpval.count;
	
	Tmpval.count = TimerMax;
    }
    
    if (TMQ.qHead != 0)
	VIAW(T2CH) = Timer2val.count;
    else {
	Timer2val.count = TimerMax;
	VIA(IFR) = 0x20;
    }
    
    TMQ.qFlags = 0;
	
    splx(s);
}

InsTime(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    if (TimeInited == FALSE) {
	(void) mac_interrupt_vector(kernel_request_port,
				    MAC_INTR_TIMER1,
				    (vm_address_t) TIM1vec);
	
	(void) mac_interrupt_vector(kernel_request_port,
				    MAC_INTR_TIMER2,
				    (vm_address_t) TIM2vec);

	TimeInited = TRUE;
    }	

    regs.d_0 = 0;
}

PrimeTime(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    register TMTaskPtr	tp = (TMTaskPtr)regs.a_0;
    register		count = (int)regs.d_0;
    register		s = splhigh();
			     
    if (TMQ.qFlags == 0)
	Timer2val.count = VIAW(T2CL);

    if (count < 0)
	tp->tmCount = -count;	/* close */
    else
	tp->tmCount = (count * TIMECONST);

    tp->tmLink = 0;

    if (TMQ.qFlags == 0) {
	if (TMQ.qHead == 0)
	    TMQ.qTail = TMQ.qHead = tp;
	else {
	    TMQ.qTail->tmLink = tp;
	    TMQ.qTail = tp;
	}

	if (tp->tmCount < Timer2val.count)
	    Timer2val.count = tp->tmCount;
    }
    else {
	if (TmpQ.qHead == 0)
	    TmpQ.qTail = TmpQ.qHead = tp;
	else {
	    TmpQ.qTail->tmLink = tp;
	    TmpQ.qTail = tp;
	}

	if (tp->tmCount < Tmpval.count)
	    Tmpval.count = tp->tmCount;
    }

    if (TMQ.qFlags == 0)
	VIAW(T2CH) = Timer2val.count;

    splx(s);

    regs.d_0 = 0;
}

RmvTime(trapword, regs)
unsigned long	trapword;
os_reg_t	regs;
{
    register TMTaskPtr	qp, *ppl, tp = (TMTaskPtr)regs.a_0;
    register		s = splhigh();

    ppl = &TMQ.qHead;
    qp = *ppl;

    while (qp != 0) {
	if (qp == tp) {
	    *ppl = tp->tmLink;
	    if (TMQ.qTail == tp)
		TMQ.qTail = (TMTaskPtr)ppl;

	    break;
	}
	else
	    ppl = &qp->tmLink;

	qp = *ppl;
    }

    splx(s);

    regs.d_0 = 0;
}
