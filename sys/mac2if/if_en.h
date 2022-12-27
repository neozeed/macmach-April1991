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
 * $Log:	if_en.h,v $
 * Revision 2.2  90/08/30  11:07:28  bohman
 * 	Created.
 * 	[90/08/29  12:34:32  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_en.h
 *	Author: Gregg Lebovitz
 */

struct en_rheader {
	u_char enr_status;
	u_char enr_next;
	u_char enr_count0;
	u_char enr_count1;
	struct ether_header enr_eh;
#define	enr_dest	enr_eh.ether_dhost
#define	enr_src		enr_eh.ether_shost
#define	enr_type	enr_eh.ether_type
};

struct en_stats {
    			/* Receive errors: */
    int	rcrc_err;	/*    crc errors */
    int	rfae_err;	/*    framing errors */
    int	rfo_err;	/*    fifo overrun errors */
    int	rmiss_err;	/*    missed packets */
    			/* Send errors: */
    int	scol_err;	/*    collisions */
    int	sabt_err;	/*    aborts */
    int	scrs_err;	/*    lost send packets */
    int	sfu_err;	/*    fifo underruns */
    int	scdh_err;	/*    heartbeat missing */
    int	sowc_err;	/*    out of window collisions */
};
