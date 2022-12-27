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
 * $Log:	via.h,v $
 * Revision 2.2  90/08/30  11:07:02  bohman
 * 	Created.
 * 	[90/08/29  12:54:43  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/via.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Macintosh II VIA driver
 * defines.
 */

#ifndef	_MAC2DEV_VIA_H_
#define	_MAC2DEV_VIA_H_		1

/*
 * VIA registers.
 */
#define	VIA_REG_OFF	0x200

typedef volatile struct {
    unsigned char regb, regb_x[VIA_REG_OFF-1];	/* 0x0000 reg b */
    unsigned char	xxxx_x[VIA_REG_OFF];	/* 0x0200 not used */
    unsigned char ddrb, ddrb_x[VIA_REG_OFF-1];	/* 0x0400 data dir reg b */
    unsigned char ddra, ddra_x[VIA_REG_OFF-1];	/* 0x0600 data dir reg a */
    unsigned char t1cl, t1cl_x[VIA_REG_OFF-1];	/* 0x0800 timer 1 cnt low */
    unsigned char t1ch, t1ch_x[VIA_REG_OFF-1];	/* 0x0A00 timer 1 cnt high */
    unsigned char t1ll, t1ll_x[VIA_REG_OFF-1];	/* 0x0C00 timer 1 ltch low */
    unsigned char t1lh, t1lh_x[VIA_REG_OFF-1];	/* 0x0E00 timer 1 ltch high */
    unsigned char t2cl, t2cl_x[VIA_REG_OFF-1];	/* 0x1000 timer 2 cnt low */
    unsigned char t2ch, t2ch_x[VIA_REG_OFF-1];	/* 0x1200 timer 2 cnt high */
    unsigned char sr,	sr_x[VIA_REG_OFF-1];	/* 0x1400 shift reg */
    unsigned char acr,	acr_x[VIA_REG_OFF-1];	/* 0x1600 aux ctrl reg */
    unsigned char pcr,	pcr_x[VIA_REG_OFF-1];	/* 0x1800 periph ctrl reg */
    unsigned char ifr,	ifr_x[VIA_REG_OFF-1];	/* 0x1A00 intr flag reg */
    unsigned char ier,	ier_x[VIA_REG_OFF-1];	/* 0x1C00 intr enb reg */
    unsigned char rega,	rega_x[VIA_REG_OFF-1];	/* 0x1E00 reg a */
} *via_reg_t;

/*
 * VIA port bit definitions
 */
/*
 * VIA1 Port A
 */
#define VIA_PORT_vSCCWrReq	0x80
#define VIA_PORT_vRev8		0x40
#define VIA_PORT_vHeadSel	0x20
#define VIA_PORT_vOverlay	0x10
#define VIA_PORT_vSync		0x08
#define VIA_PORT_1A_BIT_2	0x04
#define VIA_PORT_1A_BIT_1	0x02
#define VIA_PORT_1A_BIT_0	0x01

/*
 * VIA1 Port B
 */
#define VIA_PORT_vFDesk2	0x20
#define VIA_PORT_vFDesk1	0x10
#define VIA_PORT_vFDBInt	0x08
#define VIA_PORT_rTCEnb		0x04
#define VIA_PORT_rTCClk		0x02
#define VIA_PORT_rTCData	0x01

/*
 * VIA2 Port A
 */
#define VIA_PORT_v2RAM1		0x80
#define VIA_PORT_v2RAM0		0x40
#define VIA_PORT_v2IRQ6		0x20
#define VIA_PORT_v2IRQ5		0x10
#define VIA_PORT_v2IRQ4		0x08
#define VIA_PORT_v2IRQ3		0x04
#define VIA_PORT_v2IRQ2		0x02
#define VIA_PORT_v2IRQ1		0x01

/*
 * VIA2 Port B
 */
#define VIA_PORT_v2VBL		0x80
#define VIA_PORT_v2SNDEXT	0x40
#define VIA_PORT_v2TM0A		0x20
#define VIA_PORT_v2TM1A		0x10
#define VIA_PORT_vFC3		0x08
#define VIA_PORT_v2PowerOff	0x04
#define VIA_PORT_v2BusLk	0x02
#define VIA_PORT_v2CDis		0x01

/*
 * VIA Peripheral control register
 */
#define VIA_PC_CB2_NEG_INTR	0x20
#define VIA_PC_CB2_POS_INTR	0x60
#define VIA_PC_CA2_POS_INTR	0x06

/*
 * VIA Auxillary control register
 */
#define VIA_AC_T1CONT		0x40
#define VIA_AC_T1PB7		0x80
#define VIA_AC_PBLATCH		0x02

/*
 * VIA Interrupt flag register
 */
#define VIA_IF_IRQ		0x80
#define VIA_IF_TIMER1		0x40
#define VIA_IF_TIMER2		0x20
#define VIA_IF_CB1		0x10
#define VIA_IF_CB2		0x08
#define VIA_IF_SR		0x04
#define VIA_IF_CA1		0x02
#define VIA_IF_CA2		0x01

/*
 * VIA Interrupt enable register
 */
#define VIA_IE_SET		0x80
#define VIA_IE_TIMER1		0x40
#define VIA_IE_TIMER2		0x20
#define VIA_IE_CB1		0x10
#define VIA_IE_CB2		0x08
#define VIA_IE_SR		0x04
#define VIA_IE_CA1		0x02
#define VIA_IE_CA2		0x01
#define VIA_IE_CLEAR		0x00

#define VIA_IE_CLEAR_ALL	(VIA_IE_CLEAR |				\
				 VIA_IE_TIMER1 | VIA_IE_TIMER2 |	\
				 VIA_IE_CB1 | VIA_IE_CB2 | VIA_IE_SR |	\
				 VIA_IE_CA1 | VIA_IE_CA2)

/*
 * VIA driver info.
 */
typedef struct {
    void		(*v_intr_tbl[7])();
    via_reg_t		v_reg_ptr;
    unsigned char	v_softw_ifr;
} via_info_t;

extern	via_info_t	via[];

extern	via_reg_t	*VIA1_ADDR_PTR, *VIA2_ADDR_PTR;
#define VIA1_ADDR	(*VIA1_ADDR_PTR)
#define VIA2_ADDR	(*VIA2_ADDR_PTR)

extern	unsigned char	*VIA1_SOFT_PTR, *VIA2_SOFT_PTR;
#define VIA1_SOFT	(*VIA1_SOFT_PTR)
#define VIA2_SOFT	(*VIA2_SOFT_PTR)

#endif	_MAC2DEV_VIA_H_
