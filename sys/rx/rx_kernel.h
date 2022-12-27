/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rx_kernel.h,v $
 * Revision 2.3  89/06/03  15:40:05  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:25:16  jsb]
 * 
 * Revision 2.2  89/04/22  15:29:19  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

/*
****************************************************************************
*        Copyright IBM Corporation 1988, 1989 - All Rights Reserved        *
*                                                                          *
* Permission to use, copy, modify, and distribute this software and its    *
* documentation for any purpose and without fee is hereby granted,         *
* provided that the above copyright notice appear in all copies and        *
* that both that copyright notice and this permission notice appear in     *
* supporting documentation, and that the name of IBM not be used in        *
* advertising or publicity pertaining to distribution of the software      *
* without specific, written prior permission.                              *
*                                                                          *
* IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL IBM *
* BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY      *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER  *
* IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING   *
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.    *
****************************************************************************
*/

/* Definitions specific to the in-kernel implementation of Rx, for in-kernel clients */

#ifndef __RX_KERNEL_INCL_
#define	__RX_KERNEL_INCL_   1
extern int (*rxk_GetPacketProc)(); /* set to packet allocation procedure */
extern int (*rxk_PacketArrivalProc)();
#define	SPLVAR	    register int splvar
#define	NETPRI	    splvar=splnet()
#define	USERPRI	    splx(splvar)
static int MyPacketProc();
static int MyArrivalProc();
extern rxi_StartListener();
#define	rxi_ReScheduleEvents	0 /* Not needed by kernel */

/* This is a no-op, because the kernel server procs are pre-allocated */
#define rxi_StartServerProcs(x)

/* Socket stuff */
typedef struct socket *osi_socket;
#define	OSI_NULLSOCKET	((osi_socket) 0)

extern rx_ReScheduleEvents();
extern osi_socket rxi_GetUDPSocket();

#define	osi_Msg printf)(
extern int printf();
#define	osi_YieldIfPossible()
#define	osi_WakeupAndYieldIfPossible(x)	    osi_Wakeup(x)
#endif __RX_KERNEL_INCL_
