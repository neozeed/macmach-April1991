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
 * $Log:	if_enregs.h,v $
 * Revision 2.2  90/08/30  11:09:05  bohman
 * 	Created.
 * 	[90/08/29  12:35:34  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_enregs.h
 *	Author: Gregg Lebovitz
 */

#define	EN_RAMADDR(x)	((caddr_t)(x)+0x000d0000)
#define	EN_REGADDR(x)	((en_reg_t)((caddr_t)(x)+0x000e0000))
#define	EN_ROMADDR(x)	((caddr_t)(x)+0x000f0000)
#define	EN_RAMSIZE	0x4000
#define	EN_ROMSIZE	0x4000

#define	EN_XSTART	0
#define	EN_PSTART	6
#define	EN_PSTOP	32

#define	EN_PGSIZE	256
#define EN_PGSHIFT	8
#define EN_PGTOP(pg)	((pg)<<EN_PGSHIFT)	

typedef struct {
    volatile u_char *en_register[16];
} en_reg_ptrs_t, *en_reg_t;

#define NIC_REGISTER(offset)   en_register[offset][0]

/* Page 0 write registers */

#define	en_csr		NIC_REGISTER(0)
#define	en_pstart	NIC_REGISTER(1)
#define	en_pstop	NIC_REGISTER(2)
#define	en_bnry 	NIC_REGISTER(3)
#define	en_tpsr 	NIC_REGISTER(4)
#define	en_tbcr(n)	NIC_REGISTER(5  + n)  /* registers  5 -  6 */
#define	en_isr		NIC_REGISTER(7)
#define	en_rsar(n)	NIC_REGISTER(8  + n)  /* registers  8 -  9 */
#define	en_rbcr(n)	NIC_REGISTER(10 + n) /* registers 10 - 11 */
#define	en_rcr		NIC_REGISTER(12)
#define	en_tcr		NIC_REGISTER(13)
#define	en_dcr		NIC_REGISTER(14)
#define	en_imr		NIC_REGISTER(15)

/* Page 0 read registers */

     /* en_cr     0 */
#define	en_clda(n)	NIC_REGISTER(1  + n)  /* registers  1 -  2 */
     /* en_bnry */			   /* register   3 */
#define	en_tsr		NIC_REGISTER(4)
#define	en_ncr		NIC_REGISTER(5)
#define	en_fifo 	NIC_REGISTER(6)
     /* en_isr */			   /* register   7 */
#define	en_crda(n)	NIC_REGISTER(8  + n)  /* registers  8 -  9 */
     /* reserved */			   /* registers 10 - 11 */
#define	en_rsr		NIC_REGISTER(12)
#define	en_cntr(n)	NIC_REGISTER(13 + n) /* registers 13 - 15 */

/* Page 1 read and write registers */

     /* en_cr     0 */
#define	en_par(n)	NIC_REGISTER(1  + n) /* registers  1 -  6 */
#define en_mar(n)	NIC_REGISTER(8  + n) /* registers  8 - 15 */
#define	en_curr 	NIC_REGISTER(7)


/*
 * Control Register (READ/WRITE)
 */

#define	CR_STP		0x01
#define	CR_STA		0x02
#define	CR_TXP		0x04
#define CR_RD0		0x08
#define CR_RD1		0x10
#define CR_RD2		0x20
#define	CR_PS0		0x00
#define	CR_PS1		0x40
#define	CR_PS2		0x80

#define	CR_DMA_READ	(CR_RD0)
#define	CR_DMA_WRITE	(CR_RD1)
#define	CR_DMA_SEND	(CR_RD0 | CR_RD1)
#define	CR_DMA_ABORT	(CR_RD2)


/*
 * Interrupt Status Register (READ/WRITE)
 */

#define	ISR_PRX		0x01
#define	ISR_PTX		0x02
#define	ISR_RXE		0x04
#define	ISR_TXE		0x08
#define	ISR_OVW		0x10
#define	ISR_CNT		0x20
#define	ISR_RDC		0x40
#define	ISR_RST		0x80
#define	ISR_ALL		0x7F

/*
 * Interrupt Mask Register (WRITEONLY)
 */

#define	IMR_NONE	0x00
#define	IMR_PRXE	0x01
#define	IMR_PTXE	0x02
#define	IMR_RXEE	0x04
#define	IMR_TXEE	0x08
#define	IMR_OVWE	0x10
#define	IMR_CNTE	0x20
#define	IMR_RDCE	0x40

/*
 * Data Control Register (WRITEONLY)
 */

#define	DCR_WTS		0x01
#define	DCR_BOS		0x02
#define	DCR_LAS		0x04
#define	DCR_LS		0x08
#define	DCR_AR		0x10
#define	DCR_FT0		0x20
#define	DCR_FT1		0x40

#define DCR_FIFO_1	0x00
#define DCR_FIFO_2      (DCR_FT0)
#define DCR_FIFO_4	(DCR_FT1)
#define DCR_FIFO_6	(DCR_FT0 | DCR_FT1)

/*
 * Transmit Control Register (WRITEONLY)
 */

#define	TCR_CRC		0x00
#define TCR_NOCRC	0x01
#define TCR_LB0		0x02
#define TCR_LB1		0x04
#define	TCR_ATD		0x08
#define	TCR_OFST	0x10

#define	TCR_LOOP_NORM	 0x00
#define	TCR_LOOP_INT	 (TCR_LB0)
#define	TCR_LOOP_STARLAN (TCR_LB1)
#define	TCR_LOOP_EXT	 (TCR_LB0 | TCR_LB1)


/* 
 * The transmit status register (TSR) bit definitions.  This is a read 
 * only register.
 */

#define	TSR_PTX		0x01
#define	TSR_DFR		0x02
#define	TSR_COL		0x04
#define	TSR_ABT		0x08
#define	TSR_CRS		0x10
#define	TSR_FU		0x20
#define	TSR_CDH		0x40
#define	TSR_OWC		0x80


/* 
 * Receive Control Register (WRITEONLY)
 */

#define	RCR_SEP		0x01
#define	RCR_AR		0x02
#define	RCR_AB		0x04
#define	RCR_AM		0x08
#define	RCR_PRO		0x10
#define	RCR_MON		0x20

/*
 * Receive Status Register (READONLY)
 */

#define	RSR_PRX		0x01
#define	RSR_CRC		0x02
#define	RSR_FAE		0x04
#define	RSR_FO		0x08
#define	RSR_MPA		0x10
#define	RSR_PHY		0x20
#define	RSR_DIS		0x40
#define	RSR_DFR		0x80
