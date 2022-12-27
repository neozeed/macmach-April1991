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
 * $Log:	mscsi.c,v $
 * Revision 2.2  90/08/30  11:06:38  bohman
 * 	Created.
 * 	[90/08/29  12:49:04  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/mscsi.c
 *	Author: Art Wetzel (CMU macmach)
 */

#define	POLL_WAIT	50000	/* max wait parameter for scsi out poll */
#define	SCSI_INT_WAIT	100000	/* retries waiting for scsi_int */
#define	GET_TRIES	1000	/* get_scsi_bus retries */
#include <sys/param.h>
#include <sys/errno.h>
#include <sys/kernel.h>

#include <mac2/clock.h>
#include <mac2/act.h>

#include <mac2dev/via.h>
#include <mac2dev/mscsi.h>
#include <mac2dev/ncr5380.h>

struct act  *scsi_act, *scsi_timer_act;
#define	SCSI_TIMER_CLOCK_LIST	0

extern volatile label_t *nofault;

int	scsi_bus_wanted;
int	scsi_get_counter;

get_scsi_bus() {
	register int i;
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;

	ncr->mode = 0;
	ncr->curr_data = MAC_SCSI_ID;
	ncr->mode = SMD_ARB;
	/* the 5380 will wait for bus free before beginning arbitration */
	for(i = 0; !(ncr->init_comm & SIC_AIP); i++) {
		if(i > GET_TRIES) {
failed:
printf("get failed i %d\n", i);
			ncr->mode = 0;
			return(SI_BSY);
		}
	}
i = -1; /* for failure print */
	delay(2);
	if(ncr->init_comm & SIC_LA)
		goto failed;
i = -2; /* for failure print */
	if(ncr->curr_data > MAC_SCSI_ID)
		goto failed;
i = -3; /* for failure print */
	if(ncr->init_comm & SIC_LA)
		goto failed;
	ncr->init_comm = SIC_SEL | SIC_BSY; /* | SIC_ATN */
	ncr->mode = 0;
	scsi_get_counter++;
	return(0);	/* got it! */
}

wait_for_scsi_bus() {
	int s;
	s = spl2();
#ifdef MM_BUGFIX
	if (get_scsi_bus()) {
	    scsi_reset();
	    splx(s);
	    return (1);
	}
#else
	scsi_bus_wanted++;
	while(scsi_bus_wanted > 1 || get_scsi_bus()) {
		/* failed to get it */
/*
if no cmd in progress set up fast timer or interrupt to detect free by other master
*/
printf("sleep on scsi_bus_wanted %d\n", scsi_bus_wanted);
		sleep(&scsi_bus_wanted, PRIBIO);
printf("scsi_bus_wanted awoke\n");
		spl2();
		if(get_scsi_bus() == 0)
			break;
printf("get failed after sleep\n");
	}
	/* got it */
	scsi_bus_wanted--;
#endif
	splx(s);
	return(0);
}

select_scsi_id(id)
int	id;
{
	register int i;
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;

	ncr->targ_comm = 0;
	ncr->out_data = (1 << id) | MAC_SCSI_ID;
	ncr->sel_ena = 0;
	ncr->init_comm = SIC_SEL | SIC_DB;
	delay(2);
	for(i = 0; !(ncr->curr_stat & SCS_BSY); ++i) {
		if(i >= 25000) {	/* 250ms selection timeout */
			ncr->init_comm &= ~SIC_DB;
			delay(200);	/* 200us selection abort time */
			if(ncr->curr_stat & SCS_BSY)
				break;
			ncr->init_comm = 0;
			ncr->sel_ena = MAC_SCSI_ID;
#ifdef	DEBUG
printf("select busy\n");
#endif
			return(SI_BSY);
		}
		delay(10);		/* wait 10usec per test */
	}
	delay(2);
	ncr->init_comm = 0;
	ncr->sel_ena = MAC_SCSI_ID;
	return(0);	/* SI_DONE */
}

mscsiin_poll(buf, len)
register caddr_t buf;
register int len;
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	register int i;
	register volatile unsigned char *dp = (unsigned char *)SHSK_ADDR_R8;
	register volatile unsigned char *stp = &ncr->bus_status;
	register unsigned char st, m;

	ncr->init_comm = 0;
	ncr->mode |= SMD_DMA;
	ncr->start_Ircv = 0;
	m = SBS_PHASE | SBS_DMA;
	i = len;
	do {
		while((st = (*stp & m)) != m)
			if(!(st & SBS_PHASE))
				goto quit;
		*buf++ = *dp;
	} while(--i);
quit:
if(i) printf("in_poll %d %d\n", len, i);
	return(len - i);
}

mscsiout_poll(buf, len)
register caddr_t buf; register len;
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	register int i;
	register char st, m;

	/*
	 * wait a while for initial phase match
	 * there is no SCSI spec on how long this can take but we
	 * wait for a max of approximately 50 ms
	 */
	for(i = 0; (!(ncr->bus_status & SBS_PHASE)) && i < POLL_WAIT; i++)
		delay(10);	/* 10usec wait each time */
	if(i >= POLL_WAIT) {	/* no match */
		printf("no poll phase\n");
		return(0);
	}
	m = SCS_BSY | SCS_REQ;
	i = len;
	do {
		while((st = (ncr->curr_stat & m)) != m) {
			if(!(st & SCS_BSY) || !(ncr->bus_status & SBS_PHASE)) {
				ncr->init_comm &= ~SIC_DB;
				goto quit;
			}
		}
		ncr->init_comm = SIC_DB;
		ncr->out_data = *buf++;
		ncr->init_comm |= SIC_ACK;
		while(((st = ncr->curr_stat) & SCS_REQ))
			;
		ncr->init_comm = 0;
	} while(--i);
quit:
if(i) printf("out poll %d %d\n", len, i);
	return(len - i);
}

clr_scsi_int()
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	register int newphase;
	newphase = ncr->respar_int; /* garbage read to clear it */
	ncr->mode = 0;
	VIA_CLRSIRQ();
	ncr->sel_ena = MAC_SCSI_ID;
	newphase = (ncr->curr_stat >> 2) & 0x7;
	ncr->targ_comm = newphase;
	return(newphase);
}

struct	scsi_cmd_blk	*scp = &scsi_cmd_blk;

scsi_reset() {
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	int s;
	s = spl2();
	VIA_SIRQ(1);
	printf("SCSI RESET\n");
	ncr->out_data = 0;
	ncr->mode = 0;
	ncr->targ_comm = 0;
	ncr->init_comm = SIC_RST;
	delay(100);	/* must be > 25us */
	ncr->init_comm = 0;
	delay(1000000);	/* give devices plenty of time (1 sec) to reset */
	clr_scsi_int();
	/* if scp->cmd_buf_in_use end it with an error */
if(scp->cmd_buf_in_use) printf("RESET with scp->cmd_buf_in_use\n");
	if(scp->cmd_buf_in_use)
		scsi_op_done(1);
	splx(s);
}

scsi_xfer_fun(newphase) {
	switch(newphase) {
	case	DATA_IN_P:
		if(scp->cmd_blind)
			scsi_blind_read(scp->cmd_xfer_addr, scp->cmd_xfer_len, DEV_BSIZE);
		else
			scsi_xfer(newphase, scp->cmd_xfer_addr, scp->cmd_xfer_len, DEV_BSIZE);
if(scp->cmd_transferred != scp->cmd_xfer_len) printf("IN %d %d\n", scp->cmd_transferred, scp->cmd_xfer_len);
		break;
	case	DATA_OUT_P:
		if(scp->cmd_blind)
			scsi_blind_write(scp->cmd_xfer_addr, scp->cmd_xfer_len, DEV_BSIZE);
		else
			scsi_xfer(newphase, scp->cmd_xfer_addr, scp->cmd_xfer_len, DEV_BSIZE);
if(scp->cmd_transferred != scp->cmd_xfer_len) printf("OUT %d %d\n", scp->cmd_transferred, scp->cmd_xfer_len);
		break;
	default:
		printf("bad newphase %d\n", newphase);
	}
	spl0();	/* xfer routines raise pri to ensure proper status before interrupt */
}

mscsiirq() {
	int newphase;
	newphase = clr_scsi_int();
	switch(newphase) {
	case	DATA_IN_P:
	case	DATA_OUT_P:
		softact(scsi_act, newphase);
		break;
	case	STATUS_P:
		scsi_stat();
		break;
	case	MSG_IN_P:
		scsi_msg_in();
		break;
	case	MSG_OUT_P:
	case	CMD_P:
	case	UN4_P:
	case	UN5_P:
	default:
		printf("unknown phase %d\n", newphase);
	}
}

/*
 * Blind (no byte by byte handshake) programmed SCSI data transfer.
 * This is the fastest data transfer method on a MAC II since there
 * is, unfortunately, no DMA.  This is sometimes called "pseudodma".
 * These routines assume the transfer length, len, and the interval between
 * handshakes, resync, are multiples of 16 bytes.  This is not a problem
 * for its intended use transfering disk blocks.
 */
scsi_blind_read(buf, len, resync)
register int *buf;		/* location in memory for transfer buffer */
int	len;			/* length of transfer in bytes */
int	resync;			/* bytes to transfer between handshakes */
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	register volatile int *port = (int *)SHSK_ADDR_R8;
	register int current;
	register int len2;
	label_t *saved_jb = nofault;
	label_t	jb;

	if (setjmp(&jb)) {
		nofault = saved_jb;
printf("scsi_blind_read fault\n");
		return(-1);	/* actual count clobbered so return failure */
	}
	nofault = &jb;
	ncr->mode |= SMD_DMA;
	ncr->init_comm = 0;
	ncr->start_Ircv = 0;
	for(current = 0; current < len; current += resync) {
	    while((ncr->bus_status & (SBS_PHASE | SBS_DMA)) != (SBS_PHASE | SBS_DMA)) {
		if(!(ncr->bus_status & SBS_PHASE)) {
printf("scsi_blind_read: phase changed to curr 0x%x bus 0x%x with %d bytes left\n",
			ncr->curr_stat, ncr->bus_status, len - current);
		    nofault = saved_jb;
		    return(current);	/* bytes actually transferred */
		}
	    }
	    if(current == len - resync)	/* no interrupt till we get back from here */
		spl2();
	    /*
	     * The actual read loops are partly unwound to get the
	     * maximum possible programmed transfer speed.  Note: the 68020/30
	     * is actually doing one byte at a time transfers using dynamic
	     * bus sizing to interface with the 8 bit pseudo dma port.
	     */
	    for(len2 = resync>>4; --len2 >= 0; ) {
		 *buf++ = *port;
		 *buf++ = *port;
		 *buf++ = *port;
		 *buf++ = *port;
	    }
	    scp->cmd_transferred += resync;
	}
	nofault = saved_jb;
	return(len);		/* the entire read was OK */
}

int scsi_blind_write_inject_fault = 0;

scsi_blind_write(buf, len, resync)
register int *buf;		/* location in memory for transfer buffer */
int	len;			/* length of transfer in bytes */
int	resync;			/* bytes to transfer between handshakes */
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	register volatile int *port = (int *)SHSK_ADDR_R8;
	register int current;
	register int len2;
	register char *bp;
	label_t *saved_jb = nofault;
	label_t	jb;

	if (setjmp(&jb)) {
		nofault = saved_jb;
printf("scsi_blind_write fault\n");
		return(-1);	/* actual count clobbered so return failure */
	}
	if (scsi_blind_write_inject_fault > 0) {
	    scsi_blind_write_inject_fault--;
	    longjmp(&jb);
	}
	nofault = &jb;
	ncr->mode |= SMD_DMA;
	ncr->init_comm = SIC_DB;
	ncr->start_xmt = 0;
	for(current = 0; current < len; current += resync) {
	    bp = (char *)buf;
	    while((ncr->bus_status & (SBS_PHASE | SBS_DMA)) != (SBS_PHASE | SBS_DMA)) {
		if(!(ncr->bus_status & SBS_PHASE)) {
printf("scsi_blind_write: phase changed to curr 0x%x bus 0x%x with %d bytes left\n",
			ncr->curr_stat, ncr->bus_status, len - current);
		    nofault = saved_jb;
		    return(current);	/* bytes actually transferred */
		}
	    }
	    if(current == len - resync)	/* no interrupt till we get back from here */
		spl2();
	    /*
	     * Unfortunatenly on some drives the above test does not really
	     * mean that the drive is ready for a full sector and the second
	     * byte will fail if the its buffer is nearly full.  This kludge
	     * puts out one byte and then retests to avoid the problem. !!!
	     */
	    *((char *)port) = *bp++;
	    while((ncr->bus_status & (SBS_PHASE | SBS_DMA)) != (SBS_PHASE | SBS_DMA)) {
		if(!(ncr->bus_status & SBS_PHASE)) {
printf("scsi_blind_write: phase changed to curr 0x%x bus 0x%x with %d bytes left\n",
			ncr->curr_stat, ncr->bus_status, len - current);
		    nofault = saved_jb;
		    return(current);	/* bytes actually transferred */
		}
    	    }
	    /* remaining 15 of the 1st 16 bytes */
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    *((char *)port) = *bp++;
	    buf = (int *)bp;
	    /*
	     * The actual transfer loops are partly unwound (16 bytes) to get the
	     * maximum possible programmed transfer speed.  Note: the 68020/30
	     * is actually doing one byte at a time transfers using dynamic
	     * bus sizing to interface with the 8 bit pseudo dma port.
	     */
	    for(len2 = resync>>4; --len2 > 0; ) {
		*port = *buf++;
		*port = *buf++;
		*port = *buf++;
		*port = *buf++;
	    }
	    scp->cmd_transferred += resync;
	}
	while((ncr->bus_status & (SBS_PHASE | SBS_DMA)) == SBS_PHASE)
	    ;
	*((char *)port) = 0;	/* extra byte needed to start the write? */
	nofault = saved_jb;
	return(len);		/* the entire transfer was OK */
}

scsi_xfer(phase, buf, len)
register char *buf;		/* location in memory for transfer buffer */
int	len;			/* length of transfer in bytes */
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	register volatile char *port = (char *)SHSK_ADDR_R8;
	register int current;
	register int len2;
	label_t *saved_jb = nofault;
	label_t	jb;

	if (setjmp(&jb)) {
		nofault = saved_jb;
printf("scsi_xfer fault\n");
		return(-1);	/* actual count clobbered so return failure */
	}
	nofault = &jb;
	ncr->mode |= SMD_DMA;
	if(phase == DATA_OUT_P) {
		ncr->init_comm = SIC_DB;
		ncr->start_xmt = 0;
	} else {
		ncr->init_comm = 0;
		ncr->start_Ircv = 0;
	}
/* XXX adjust this loop for better speed */
	for(current = 0; current < len; current++) {
	    while((ncr->bus_status & (SBS_PHASE | SBS_DMA)) != (SBS_PHASE | SBS_DMA)) {
		if(!(ncr->bus_status & SBS_PHASE)) {
printf("scsi_xfer: phase changed to 0x%x with %d bytes left\n",
			ncr->curr_stat, len - current);
		    nofault = saved_jb;
		    return(current);	/* bytes actually transferred */
		}
	    }
	    if(current == len - 1)	/* no interrupt till we get back from here */
		spl2();
	    if(phase == DATA_IN_P)
	    	*buf++ = *port;
	    else
		*port = *buf++;
	    scp->cmd_transferred++;
	}
	if(phase == DATA_OUT_P) {
	    while((ncr->bus_status & (SBS_PHASE | SBS_DMA)) == SBS_PHASE) {
		;
	    }
	    *((char *)port) = 0;/* extra byte needed to start the write? */
	}
	nofault = saved_jb;
	return(len);		/* the entire transfer was OK */
}

scsi_cmd() {
	int i;
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;

	ncr->targ_comm = CMD_P;
	ncr->start_xmt = 0;
	/* set cmd len according to SCSI command group */
	switch(*scp->cmd_buf >> 5) {
	case	0:
		i = 6;
		break;
	case	2:
		i = 10;
		break;
	case	5:
		i = 12;
		break;
	default:	/* unknown group */
printf("unknown cmd group 0x%x\n", *scp->cmd_buf);
		return(-1);
	}
/* XXX how should this initial phase really be done??? */
	/* mscsiout_poll waits for an initial phase match */
	if(mscsiout_poll(scp->cmd_buf, i) != i) {
printf("bad out poll\n");
		scsi_reset();
		return(1);
	}
	ncr->mode |= SMD_DMA;
	/* ncr->init_comm = SIC_ATN; /* only for dis/reconn */
	if(scp->cmd_comp_fcn == NULL) {
		int s;
		s = spl2();
		while(!scp->cmd_done)
			sleep(&scp->cmd_done, PRIBIO);
		splx(s);
		s = scp->cmd_statb;	/* XXX change to number transferred */
		return(s);
	}
	return(0);
}

scsi_stat() {
	if(mscsiin_poll(&scp->cmd_statb, 1) != 1)
		scp->cmd_statb = 0xFF;
}

scsi_msg_in()
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	if(mscsiin_poll(&scp->cmd_msgb, 1) != 1)
		scp->cmd_msgb = 0xFF;
/* XXX check this */
	ncr->init_comm = SIC_ACK;
	delay(2);
	ncr->init_comm = 0;
	scsi_op_done(0);
}

scsi_op_done(e) {
	scp->cmd_done = 1;
	if(scp->cmd_comp_fcn) {
		int i = scp->cmd_transferred;
#ifdef	DEBUG
if(e) printf("scsi_op_done with e %d\n", e);
if(scp->cmd_statb) printf("statb 0x%x\n", scp->cmd_statb);
#endif
if(scp->cmd_msgb) printf("msgb 0x%x\n", scp->cmd_msgb);
/* XXX seek position seems to get screwed up when i != scp->cmd_req */
#ifdef	DEBUG
if(i != scp->cmd_req) printf("got %d req %d\n", i, scp->cmd_req);
#endif
		if(e || scp->cmd_transferred == 0)
			i = -1;
		scp->cmd_buf_in_use = 0;
		wakeup(&scp->cmd_buf_in_use);	/* XXX eliminate sometime */
		(scp->cmd_comp_fcn)(i);
	} else {
		if(e)
			scp->cmd_statb = 0xFF;
		wakeup(&scp->cmd_done);
	}
}

scsi_rw(read1_or_write0, ctlr, drv, block, addr, nblks, comp_func)
caddr_t addr;
int (*comp_func)();
{
	register int i;
	char cmd_buf[CMDLEN];
	for(i = 0; i < CMDLEN; i++)
		cmd_buf[i] = 0;
	*((int *)cmd_buf) = block | (drv << 21); /* cheat */
	cmd_buf[0] = read1_or_write0 ? SC_READ : SC_WRITE;
	/* place # of 512 byte blocks in cmd_buf (0-256max with 0 => 256) */
	cmd_buf[4] = nblks;
	return(scsi_op(ctlr, cmd_buf, addr, nblks<<9, 1, comp_func));
}

char	test_unit_cmd[CMDLEN];	/* all 0's */
test_unit_rdy(ctlr)
{
	return(scsi_op(ctlr, test_unit_cmd, 0, 0, 0, 0));
}

char	inquire_cmd[CMDLEN];
inquire(ctlr, addr, len)
caddr_t addr;
{
	inquire_cmd[0] = SC_INQUIRY;
	inquire_cmd[4] = len;
	return(scsi_op(ctlr, inquire_cmd, addr, len, 0, 0));
}

scsi_op(ctlr, cmdp, buf, xlen, blind, comp_func)
caddr_t cmdp;
caddr_t buf;
int (*comp_func)();
{
	register int i, s;

	s = spl2();
	while(scp->cmd_buf_in_use) {
/*
printf("sleep on scp->cmd_buf_in_use\n");
for(i = 0; i < CMDLEN; i++) printf(" 0x%x", scp->cmd_buf[i]); printf("\n");
*/
		sleep(&scp->cmd_buf_in_use, PRIBIO);
		spl2();
/*
printf("will get scp->cmd_buf\n");
*/
	}
	scp->cmd_buf_in_use = 1;
	scp->cmd_scsiid = ctlr;
	scp->cmd_blind = blind;
	scp->cmd_req = xlen;
	scp->cmd_done = 0;
	scp->cmd_transferred = 0;
	scp->cmd_statb = 0xFF;	/* assume failure */
	splx(s);
	for(i = 0; i < CMDLEN; i++)
		scp->cmd_buf[i] = cmdp[i];
	scp->cmd_xfer_addr = buf;
	scp->cmd_comp_fcn = comp_func;
	scp->cmd_xfer_len = xlen;
	if(wait_for_scsi_bus())
		goto error;
	if(select_scsi_id(ctlr))
		goto error;
	if(scsi_cmd()) {
error:
		scsi_op_done(1);
		scp->cmd_buf_in_use = 0;
		return(1);
	}
	if(!comp_func) {
if(scp->cmd_buf_in_use == 0) printf("scp->cmd_buf_in_use already 0\n");
		scp->cmd_buf_in_use = 0;
		wakeup(&scp->cmd_buf_in_use);
	}
	return(0);
}

scsi_timer(last_scsi_get_counter)
{
static int second = 0;
	if(scp->cmd_buf_in_use && (last_scsi_get_counter == scsi_get_counter)) {
		/* punt since something seems to be hung */
printf("scsi_timer wanted %d %d %d\n", scsi_bus_wanted, last_scsi_get_counter, scsi_get_counter);
		scsi_reset();
		if(scp->cmd_buf_in_use) {
printf("timer wake scp->cmd_buf\n");
			scsi_op_done(1);
		}
else printf("timer no scp->cmd_buf\n");
#ifdef MM_BUGFIX
#else
		wakeup(&scsi_bus_wanted);
#endif
	}
	runact(SCSI_TIMER_CLOCK_LIST, scsi_timer_act, scsi_get_counter, 5*hz);
}

/*
 * Only called once for initialization
 */
scsiinit()
{
	register scsi_reg_t ncr = (scsi_reg_t)SCSI_ADDR;
	int i;

	calibrate_delay();
	scsi_act = (struct act *)makesoftact(scsi_xfer_fun, SR_IPL0);
	scsi_timer_act = makeact(scsi_timer, SR_IPL2, 1);
	addact(SCSI_TIMER_CLOCK_LIST, scsi_timer_act, &actclock);
	ncr->mode = 0;
	ncr->init_comm = 0;
	ncr->sel_ena = 0;
	VIA_SIRQ(1);
	if(ncr->curr_stat & SCS_BSY)
		scsi_reset();
	runact(SCSI_TIMER_CLOCK_LIST, scsi_timer_act, scsi_get_counter, 5*hz);
	return(1);
}

#define	INITIAL_MPY	256
int delay_mpy = INITIAL_MPY;	/* calibration multiplier for delay */
int delay_overhead;		/* delay call overhead in microseconds */
delay(n)
register int n;
{
	register int delay_cnt;
	n -= delay_overhead;
	/* XXX - the multiply could overflow on huge delays */
	delay_cnt = (delay_mpy * n) >> 8;
	/* spin loop for about (n - delay_overhead) microseconds */
	while(--delay_cnt >= 0)
		;
}

#define	DELAY_TRIAL	2048
int	via_overhead;	/* global for access by timing tests */
calibrate_delay() {
	int t, tt, s;
	s = spl7();
	do {
		t = via_microsec();
		t -= via_microsec();
	} while(t < 0);
	do {
		via_overhead = via_microsec();
		via_overhead -= via_microsec();
	} while(via_overhead < 0);
	if(t < via_overhead)
		via_overhead = t;
	/* via_overhead is now the back to back via_microsec() overhead */
	do {
		t = via_microsec();
		delay(0);
		t -= via_microsec();
		t -= via_overhead;
	} while(t < 0);
	do {
		tt = via_microsec();
		delay(0);
		tt -= via_microsec();
		tt -= via_overhead;
	} while(tt < 0);
	if(t < tt)
		tt = t;
	delay_overhead = tt;
	do {
		t = via_microsec();
		delay(DELAY_TRIAL);	/* we want to get DELAY_TRIAL microseconds */
		t -= via_microsec();
		t -= via_overhead;
		t -= delay_overhead;
	} while(t < 0);
	do {
		tt = via_microsec();
		delay(DELAY_TRIAL);	/* we want to get DELAY_TRIAL microseconds */
		tt -= via_microsec();
		tt -= via_overhead;
		tt -= delay_overhead;
	} while(tt < 0);
	if(t < tt)
		tt = t;
	/* adjust delay_mpy according to the measured delay in tt */
	delay_mpy = delay_mpy * (DELAY_TRIAL - delay_overhead) / tt;
	splx(s);
}

/* return microseconds remaining in wraparound via timer */
via_microsec() {
	register via_reg_t vp = VIA1_ADDR;
	register int h, l;
	do {
		h = vp->t2ch;
		l = vp->t2cl;
	} while(vp->t2ch != h);
	h <<= 8;
	h += l;
	h += h >> 2;	/* convert to approximate 1.25 micro seconds per tick */
	return(h);
}
