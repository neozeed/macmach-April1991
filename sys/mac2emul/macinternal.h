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
 *	File: mac2emul/macinternal.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2EMUL_MACINTERNAL_H_
#define _MAC2EMUL_MACINTERNAL_H_

#include <sys/buf.h>
#include <sys/socket.h>
#include <net/if.h>

#include <vm/vm_map.h>

#include <mac2emul/macserver_types.h>

/*
 * macintosh emulation
 * internal data structures
 */
struct mac_internal {
    struct act *	intr;		/* thread's interrupt activity */
    unsigned long	softintr_pend;	/* bit mask of pending interrupts */
#define MAC_INTR_PEND(n)	(1 << (31 - (n)))
    struct softintr {
	unsigned long	vector;		/* address of interrupt vector */
	unsigned long	ipl;		/* priority level of interrupt */
	int		flags;
#define MAC_IF_ENB	0x00000002	/* interrupt is enabled */
#define MAC_IF_ASYNC	0x00000004
    } softintr[MAC_INTR_NUMB];		/* software interrupt vector table */
    struct via_device {			/* VIA emulation */
	struct via_port {
	    unsigned char	dir;	/* data direction register */
	    unsigned char	reg;	/* data output latch */
	    unsigned char	dat;	/* data input value */
	} portA, portB;
	unsigned char	ACR;		/* auxiliary control register */
	unsigned char	PCR;		/* peripheral control register */
	unsigned char	IFR;		/* interrupt flag register */
	unsigned char	IER;		/* interrupt enable register */
	unsigned char	T1CH;
	unsigned char	T1CL;
	unsigned char	T2CH;
	unsigned char	T2CL;
    } via1, via2;
    unsigned long	adb_trans_state;/* ADB garbage */
#define ADB_TRANS_CMD	0x0
#define ADB_TRANS_EVEN	0x1
#define ADB_TRANS_ODD	0x2
#define ADB_TRANS_IDLE	0x3
    unsigned short	sr;		/* software status register */
    unsigned long	ctrl[8];	/* software cpu registers */
    struct pmmu_regs {
	unsigned long	TC;
	unsigned long	CRP[2];
    } pmmu;
    struct scsi_data {			/* scsi manager */
	struct buf	sc_buf;
	int		sc_flags;
#define SCSI_F_READ	1
#define SCSI_F_WRITE	2
    } scsi;
    struct vm_map	*task_compat_map;
    unsigned long	reset_vector;
    mac_os_map_t	os_map;
    unsigned long	os_vector;
    mac_tbox_map_t	tbox_map;
    unsigned long	tbox_vector;
    short		ether_multi_class[64];
    queue_head_t	ether_proto;
    struct ifqueue	ether_rcv;
};

/*
 * Initial values
 * for machine registers
 */
#define CTRL_INIT(n)	mac->ctrl[n] = INIT_##n

#define INIT_R_SR   0x0000
#define INIT_R_SFC  0x0
#define INIT_R_DFC  0x0
#define INIT_R_CACR 0x0
#define INIT_R_USP  0x0
#define INIT_R_VBR  0x0
#define INIT_R_CAAR 0x0
#define INIT_R_MSP  0x0
#define INIT_R_ISP  0x0

extern int	macdebug;

#endif _MAC2EMUL_MACINTERNAL_H_
