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
 * $Log:	mscsi.h,v $
 * Revision 2.2  90/08/30  11:06:47  bohman
 * 	Created.
 * 	[90/08/29  12:49:30  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/mscsi.h
 *	Author: Art Wetzel (CMU macmach)
 */

#define	MAC_SCSI_ID (1<<7)		/* Mac SCSI id in 1s representation */

/* the SCSI phases */
#define	DATA_OUT_P	0
#define	DATA_IN_P	1
#define	CMD_P		2
#define	STATUS_P	3
#define	UN4_P		4
#define	UN5_P		5
#define	MSG_OUT_P	6
#define	MSG_IN_P	7

#define	SI_BSY		10

#define	SC_TSTUNITRDY	0x00
#define	SC_REZERO	0x01
#define	SC_REWIND	0x01
#define	SC_REQSNS	0x03
#define	SC_FORMAT	0x04
#define	SC_READBLKLIM	0x05
#define	SC_REASIGNBLKS	0x07
#define	SC_READ		0x08
#define	SC_WRITE	0x0A
#define	SC_SEEK		0x0B
#define	SC_TRACKSEL	0x0B
#define	SC_FILEMARK	0x10
#define	SC_SPACE	0x11
#define	SC_INQUIRY	0x12
#define	SC_MODESEL	0x15
#define	SC_RESERVE	0x16
#define	SC_RELEASE	0x17
#define	SC_COPY		0x18
#define	SC_ERASE	0x19
#define	SC_MODESNS	0x1A
#define	SC_STARTSTOP	0x1B
#define	SC_LOADUNLOAD	0x1B
#define	SC_RCVDIAG	0x1C
#define	SC_SENDDIAG	0x1D
#define	SC_REMOVAL	0x1E
#define	SC_PARAM	0x24
#define	SC_READCAP	0x25
#define	SC_READ1	0x28
#define	SC_SEND		0x2A
#define	SC_GETSTATUS	0x34

/* scsi command structure used in mscsi.c */
#define	CMDLEN	10
struct	scsi_cmd_blk {
	unsigned char cmd_buf[CMDLEN];
	char	*cmd_xfer_addr;
	int	cmd_xfer_len;
	char	cmd_statb;
	char	cmd_msgb;
	int	(*cmd_comp_fcn)();
	int	cmd_done;
	int	cmd_transferred;
	int	cmd_req;
	int	cmd_scsiid;
	int	cmd_blind;
	int	cmd_buf_in_use;
}	scsi_cmd_blk;

/*
 * Macros for SCSI DRQ and SCSI IRQ interrupts.
 */
#define	VIA_SDRQ(on)	(VIA2_ADDR->ier = \
	(on ? VIA_IE_SET : VIA_IE_CLEAR) | VIA_IE_CA2)
#define	VIA_CLRSDRQ()	(VIA2_ADDR->ifr = VIA_IE_CA2)
#define VIA_SIRQ(on)	(VIA2_ADDR->ier = \
	(on ? VIA_IE_SET : VIA_IE_CLEAR) | VIA_IE_CB2)
#define	VIA_CLRSIRQ()	(VIA2_ADDR->ifr = VIA_IE_CB2)
