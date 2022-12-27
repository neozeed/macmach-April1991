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
 * $Log:	ncr5380.h,v $
 * Revision 2.2  90/08/30  11:06:50  bohman
 * 	Created.
 * 	[90/08/29  12:49:53  bohman]
 * 
 */

/*
 *
 *	ncr5380.h -- interface to ncr SCSI chip and SCSI manager software.
 *
 *	Copyright 1987 Unisoft Corporation.
 */

typedef volatile struct {
    unsigned char	curr_data;	/* R   Current SCSI data */
    unsigned char	fill1[15];
    unsigned char	init_comm;	/* R/W Initiator Command */
    unsigned char	fill2[15];
    unsigned char   mode;		/* R/W Mode		 */
    unsigned char	fill3[15];
    unsigned char 	targ_comm;	/* R/W Target Command	 */
    unsigned char	fill4[15];
    unsigned char	curr_stat;	/* R   Current Status	 */
    unsigned char	fill5[15];
    unsigned char	bus_status;	/* R   Bus and Status bits*/
    unsigned char	fill6[15];
    unsigned char   in_data;	/* R   Input Data	 */
    unsigned char	fill7[15];
    unsigned char	respar_int;	/* R   Reset Parity/Ints */
} *scsi_reg_t;
#define		out_data curr_data	/* W   Output Data 	 */
#define		sel_ena	 curr_stat	/* W   Select Enable	 */
#define		start_xmt bus_status	/* W   Start DMA SCSI xmit */
#define		start_Trcv in_data	/* W   Start Target SCSI rcv */
#define		start_Ircv respar_int	/* W   Start Initiator SCSI rcv */

/*	Initiator Command register bits */

#define	SIC_DB	0x01	/* Data bus */
#define	SIC_ATN 0x02	/* ATN line */
#define	SIC_SEL	0x04	/* SEL line */
#define	SIC_BSY	0x08	/* BSY line */
#define	SIC_ACK	0x10	/* ACK line */
#define	SIC_LA  0x20	/* Lost arbitration */
#define	SIC_AIP 0x40	/* Arbitration in progress */
#define	SIC_RST	0x80	/* Reset line */

/*	Mode register bits */

#define	SMD_ARB	0x01	/* Arbitration */
#define	SMD_DMA	0x02	/* DMA transfer enable */
#define	SMD_BSY	0x04	/* monitor busy */

/*	Current status register bits */

#define SCS_SEL 0x02	/* Select line */
#define	SCS_REQ	0x20	/* REQ line */
#define	SCS_BSY	0x40	/* BSY line */
#define SCS_RST 0x80	/* Reset line */

/*	Bus status register bits */

#define	SBS_BSY	  0x04	/* busy Error */
#define	SBS_PHASE 0x08	/* Phase match */
#define	SBS_IRQ	  0x10	/* Interrupt requested */
#define	SBS_DMA	  0x40	/* DMA request bit */

#define SCSI_ADDR	0x50010000
#define	SDMA_ADDR	sdma_addr	/* SCSI special DMA register */
#define	SDMA_ADDR_R7	0x50006060	/* SCSI special DMA register */
#define	SDMA_ADDR_R8	0x50012000	/* SCSI special DMA register */
#define	SHSK_ADDR	shsk_addr	/* SCSI handshake DMA register */
#define	SHSK_ADDR_R7	0x50006060	/* SCSI handshake DMA register */
#define	SHSK_ADDR_R8	0x50006000	/* SCSI handshake DMA register */
