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
 * $Log:	if_en.c,v $
 * Revision 2.2  90/08/30  11:07:24  bohman
 * 	Created.
 * 	[90/08/29  12:30:35  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_en.c
 *	Author: Gregg Lebovitz
 */

/*
 * Macintosh II ethernet driver
 *
 * This driver works with both
 * the 3COM nubus board and
 * the Kinetics EtherPort for
 * the SE/30.
 */

#include <macos_emul.h>

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <net/if.h>
#include <net/route.h>
#include <net/netisr.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <netinet/if_ether.h>
#endif INET

#ifdef NS
#include <netns/ns.h>
#include <netns/ns_if.h>
#endif NS

#include <dli.h>
#if    DLI
#include <net/dli_var.h>
#endif DLI

#include <mac2if/if_enregs.h>
#include <mac2if/if_en.h>

#include <mac2/clock.h>
#include <mac2/act.h>

#define	NEN		6
#define EN_RSTTIMEOUT	10000
#define EN_RSTCOUNT	5

int	enattach(), enintr(), enrintr(), enxintr();
int    	eninit(), enoutput(), enioctl(), entimeout();

struct en_softc {
	struct arpcom	en_ac;		/* Ethernet common part */
#define	en_if		en_ac.ac_if	/* network-visible interface */
#define	en_enaddr	en_ac.ac_enaddr
					/* hardware ethernet address */
	int		en_flags;	/* flag bits */
#define ENF_OACTIVE	0x1
#define ENF_XMTPENDING	0x2
#define ENF_RSTPENDING	0x4
#define ENF_RUNNING	0x8
#define ENF_SETUP	0x10
#define ENF_SWAPDATA	0x20
	en_reg_ptrs_t	en_reg[1];	/* NIC registers */
	char		*en_ram;	/* ethernet on board ram */
	char		*en_rom;	/* ethernet on board rom */
	int		en_size;
	int		en_rstcount;	/* number of resets allowed until we give up */
	struct act	*en_toact;	/* timeout activity */
	struct en_stats en_stats;
	short		en_multi_class[64];	/* multicast ref counts */
#if    DLI
	struct dli_var  en_dlv;
#endif DLI
} en_softc[NEN];

struct en_params {
    int	enp_id;
    int enp_ram;
    int enp_reg;
    int enp_size;
    int enp_flags;
} en_param[] = {
    /*  id       ram      reg     size */
    { 	0x8,   0xd0000, 0xe0000, 0x4000, ENF_SWAPDATA },
    {   0x12A, 0x00000, 0x80000, 0x2000, 0 },
    {   0,     0xd0000, 0xe0000, 0x4000, ENF_SWAPDATA }, /* default */
};

#define	EN_TOACT_CLOCK_LIST	0

int	en_nunits;

extern void wcopy();

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>

#include <mac2slotmgr/slotmgr.h>

en_get_ether_address(slot, id, eaddr)
register slot;
unsigned char eaddr[];
{
    SpBlock slot_params;

    slot_params.spSlot = slot;
    slot_params.spID = id;
    slot_params.spExtDev = 0;
    if (SRsrcInfo(&slot_params) != noErr)
	return (FALSE);

    slot_params.spID = 128; /* XXX */
    if (SFindStruct(&slot_params) != noErr)
	return (FALSE);

    slot_params.spSize = 6;
    slot_params.spResult = (vm_offset_t)eaddr;
    if (SReadStruct(&slot_params) != noErr)
	return (FALSE);

    return (TRUE);
}

/*
 * this routine is for configuring the network IF
 * and is called from the autoconfig system.
 *
 */

enattach(slot, srsc)
register int slot;
int srsc;
{
    if (en_nunits < NEN) {
	register int unit = en_nunits++;
	register struct en_softc *is = &en_softc[unit];
	register struct ifnet *ifp = &is->en_if;
	register caddr_t addr = (caddr_t)slot_to_ptr(slot);
	register en_reg_t reg;
	register int id;
	struct en_params *p;
	extern void enintr_catch();
	SQElemPtr sqe;
	int i;

	id = slot_to_slotdata_ptr(slot)->SPRAMRec.boardID;

	for (p = en_param; p->enp_id ; p++)
	    if (p->enp_id == id)
		break;

	if (p->enp_id == 0)
	    printf ("id %x unknown ethernet board at slot %x\n",id, slot);

	reg = is->en_reg;

	for (i = 0 ; i < 16 ; i++)
	    reg->en_register[i] = 
		(u_char *)(addr + p->enp_reg + 
			   4 * ((p->enp_flags & ENF_SWAPDATA) 
				? 15 - i 
				: i));

	reg->en_csr = CR_STP | CR_RD2;
	reg->en_rbcr(0) = 0;  
	reg->en_rbcr(1) = 0;

	/* wait a maximum time for either the RST bit to set */
	for (i = 0; (i < EN_RSTTIMEOUT) && !(reg->en_isr & ISR_RST); i++)
	    continue;

	if ((reg->en_isr & ISR_RST) == 0) {
	    printf ("en%d: interface did not reset\n", unit);
#ifdef notwanted
	    return(0);
#endif
	}

	/* clear all interrupts */
	i = reg->en_isr;
	reg->en_isr = i;

	/*
	 * initialize the softc
	 */
	if (!en_get_ether_address(slot, srsc, is->en_enaddr))
	    return (0);

	is->en_ram = (addr + p->enp_ram);
	is->en_size = p->enp_size;
	is->en_flags = p->enp_flags;
	is->en_toact = makeact(entimeout, SR_IPL2, 1);
	addact(EN_TOACT_CLOCK_LIST, is->en_toact, &actclock);

	/*
	 * set up the interrupt vector
	 */
	sqe = (SQElemPtr)(NewPtr(sizeof (*sqe)));
	sqe->sqPrio = 1;
	sqe->sqAddr = enintr_catch;
	sqe->sqParm = unit;
	SIntInstall(sqe, slot);

	/*
	 * initialize the if
	 */

	ifp->if_unit = unit;
	ifp->if_name = "en";
	ifp->if_mtu = ETHERMTU;
	ifp->if_flags = IFF_BROADCAST;
	ifp->if_init = eninit;
	ifp->if_ioctl = enioctl;
	ifp->if_output = enoutput;
	if_attach(ifp);
	printf("ether sRsrc: slot %x id %d assigned to unit %d\n",
	       slot, srsc, unit);
	return (1);
    }

    else
	return (0);
}


/*
 * this routine verifys that the NIC chip has indeed responded
 * to a stop signal by setting the RST bit in the control register.
 * (see eninit routine)
 *
 */
en_wait_for_reset (unit)
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    register int i;

    for (i = 0; (i < EN_RSTTIMEOUT) && !(reg->en_isr & ISR_RST); i++)
	continue;

    if ((reg->en_csr &  CR_STP) &&
	(reg->en_isr & ISR_RST)) {	
	is->en_rstcount = 0;
	return 1;
    }

    else if (++is->en_rstcount < EN_RSTCOUNT) {
	    is->en_flags &= ~ENF_RUNNING;
	    printf("en%d: Network Reset Failed - Retrying\n", unit);
	    runact(EN_TOACT_CLOCK_LIST, is->en_toact, unit, hz * 5);
    }

    else {
	    printf("en%d: Cannot Reset Ethernet - Network is Down\n", unit);
	    is->en_rstcount = 0;
	    is->en_if.if_flags &= ~IFF_RUNNING;
    }
    return 0;
}

/*
 * This routine finishes an initialize operation. It is called either
 * directly from eninit or after a timeout from entimeout. 
 * (see eninit routine).
 *
 */
ensetup(unit)
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    struct ifnet *ifp = &is->en_if;
    int pstop = is->en_size>>EN_PGSHIFT;
    int i;
    int s = splimp();
    
    reg->en_tcr =  TCR_LOOP_EXT;

    if (is->en_flags & ENF_SETUP) {
	reg->en_rcr =  (RCR_AM|RCR_AB);
	reg->en_dcr =  (is->en_flags & ENF_SWAPDATA)
	    ? DCR_FIFO_4 | DCR_LS | DCR_WTS
		: DCR_FIFO_4 | DCR_LS;
    
	reg->en_pstart = EN_PSTART;
	reg->en_pstop  = pstop;
	reg->en_bnry   = pstop - 1;
	reg->en_isr    = ISR_ALL;

	reg->en_csr = CR_PS1 | CR_STP | CR_RD2; /* change register page number to one */

	for (i = 0; i < 6; i++)
	    reg->en_par(i) = is->en_enaddr[i]; /* set the ethernet address */

	reg->en_curr = EN_PSTART;

	for (i = 0; i < 8; i++)
	    reg->en_mar(i) = 0x00;	/* initialize the multicast address */
	
	reg->en_csr = CR_PS0 | CR_STP | CR_RD2; /* change register page back to zero */

	reg->en_imr = 0x1f;		     /* enable all interrupts */
	is->en_flags  &= ENF_SWAPDATA;	     /* clear flags except swap flag */
    }
    else
	is->en_flags &= ~ENF_RSTPENDING;

    reg->en_csr = CR_STA | CR_RD2;	     /* and start the device */
    reg->en_tcr = TCR_LOOP_NORM;
    
    is->en_rstcount = 0;
    is->en_flags  |= ENF_RUNNING;
    ifp->if_flags |= IFF_RUNNING;

    if (ifp->if_snd.ifq_head)
	enstart(unit);
    splx(s);
}

static entimeout(unit)
int unit;
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    
    printf ("en%d: timeout\n", unit);

    if (is->en_flags & ENF_RSTPENDING) {

	if ((reg->en_csr &  CR_STP) &&
	    (reg->en_isr & ISR_RST))
	    ensetup(unit);
	else 
	    enreset(unit);
    }

    else if (is->en_flags & ENF_XMTPENDING) {
	is->en_flags &= ~(ENF_XMTPENDING|ENF_OACTIVE);
	enreset(unit);
    }

}

/*
 * this routine restarts the NIC chip after a buffer ring overflow.
 * the overflow restart procedure is as follows:
 *
 *	1 - set the STP bit in the command register
 *	2 - wait for the RST. If we don't see the RST
 *	    after a few microseconds then schedule a timeout.
 *	    (see the eninit routine for more info on timeouts).
 *
 *	3 - start the NIC chip running.
 *
 */

enoverflow (unit)
    int unit;
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    int s = splimp();
    
    /*
     * Reset the NIC as specified in board documentation in the case
     * of an overflow warning interrupt. 
     */
    printf ("en%d: overflow\n", unit);

    if (is->en_flags & ENF_RSTPENDING)
	return(0);

    is->en_flags |= ENF_RSTPENDING;

    reg->en_csr = CR_STP | CR_RD2;
    reg->en_rbcr(0) = 0;    /* must be cleared for reset to occur*/
    reg->en_rbcr(1) = 0;

    enrintr(unit, CR_STP);

    if (en_wait_for_reset (unit)) {
	ensetup(unit);
    }
    splx(s);
}

/*
 * this is the NIC chip initalize routine.
 * the initialize procedure goes as follows:
 *
 *    1 - set the STP bit in the command register.
 *    2 - clear the remote byte count registers (the
 *	  NIC manual says the STP command will not complete
 *	  until these registers are cleared.
 *    3 - wait for the RST bit is the interrupt status register
 *
 * 	  if the NIC chip is in the middle of an transmit, the chip may
 * 	  take a long time (milliseconds) to respond. In this case
 *	  we schedule a timeout that will finish the initalize.
 *
 *	  If the RST bit is set then we call ensetup which will
 *	  set the proper parameters for the NIC chip.
 *	  Otherwise schedule the timeout and return. The timeout routine
 *	  will check for the RST bit and then call ensetup.
 *
 */ 
enreset(unit)
int	unit;
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    
    is->en_flags |= (ENF_SETUP|ENF_RSTPENDING);

    reg->en_tcr =  TCR_LOOP_EXT; /* take the chip offline */
    reg->en_csr = CR_STP | CR_RD2;
    reg->en_rbcr(0) = 0;    /* must be cleared for reset to occur*/
    reg->en_rbcr(1) = 0;

    reg->en_tpsr = 0;
    reg->en_tbcr(0) = 0;
    reg->en_tbcr(1) = 0;


    if (en_wait_for_reset(unit))
	ensetup (unit);
}

/*
 * This routine is called from outside this driver
 * to initialize the chip.
 *
 */ 
eninit(unit)
int	unit;
{ 
    register struct en_softc *is = &en_softc[unit];
   
    /* not yet, if address still unknown */
    if (is->en_if.if_addrlist == (struct ifaddr *)0)
	return;

    if (is->en_flags & (ENF_RSTPENDING|ENF_XMTPENDING)) {
	cancelact(is->en_toact);
	is->en_rstcount = 0;
	is->en_flags &= ~(ENF_RSTPENDING|ENF_XMTPENDING);
    }

    enreset(unit);
#if    DLI
    dli_init(&is->en_dlv,
	     &is->en_if,
	     is->en_enaddr,
	     etherbroadcastaddr,
	     sizeof(etherbroadcastaddr),
	     sizeof(struct ether_header));
#endif DLI
}

en_multicast_class_incr(unit, class)
int		unit;
register	class;
{
    register struct en_softc	*is = &en_softc[unit];
    register en_reg_t		reg = is->en_reg;
    register unsigned char	entry;
    register 			bit, byte;

    if ((is->en_multi_class[class] + 1) < 0)
	return (FALSE);

    byte = class / 8;
    bit = class % 8;

    if (++is->en_multi_class[class] == 1) {
	register s = splimp();

	reg->en_csr = CR_PS1 | CR_STA | CR_RD2;

	entry = reg->en_mar(byte);

	asm("bfset	%0{%1:#1}" : "=d" (entry) :
	    "d" (31 - bit), "0" (entry));

	reg->en_mar(byte) = entry;

	reg->en_csr = CR_PS0 | CR_STA | CR_RD2;

	splx(s);
    }

    return (TRUE);
}

en_multicast_class_decr(unit, class, n)
int			unit;
register		class;
register short		n;
{
    register struct en_softc	*is = &en_softc[unit];
    register en_reg_t		reg = is->en_reg;
    register unsigned char	entry;
    register 			bit, byte;

    if ((is->en_multi_class[class] - n) < 0)
	n = is->en_multi_class[class];

    byte = class / 8;
    bit = class % 8;

    if ((is->en_multi_class[class] -= n) == 0) {
	register s = splimp();

	reg->en_csr = CR_PS1 | CR_STA | CR_RD2;

	entry = reg->en_mar(byte);

	asm("bfclr	%0{%1:#1}" : "=d" (entry) :
	    "d" (31 - bit), "0" (entry));

	reg->en_mar(byte) = entry;

	reg->en_csr = CR_PS0 | CR_STA | CR_RD2;

	splx(s);
    }

    return (TRUE);
}

static unsigned char en_ring_head (reg, flags)
register en_reg_t reg;
register int flags;
{
    unsigned char page;
    
    reg->en_csr = CR_PS1 | CR_RD2 | flags;
    page = reg->en_curr;
    reg->en_csr = CR_PS0 | CR_RD2 | flags;
    return (page);
}

u_char *encopy (is, cp, pp, totlen)
register struct en_softc *is;
register u_char *cp;
u_char *pp;
int totlen;
{
    register u_char *ram = (u_char *)is->en_ram;
    register int ringlen = MAX(ram + is->en_size - cp, 0);
    register int len = MIN(totlen, ringlen);

    wcopy ((u_short *)cp, (u_short *)pp, len);
    cp += len;
    if ((len = totlen - len) > 0) {
	cp = ram + EN_PGTOP(EN_PSTART);
	wcopy ((u_short *)cp, (u_short *)(pp + ringlen), len);
	cp += len;
    }
    return cp;
}

/* this routine is modeled after the vax if_ubaget routine */
struct mbuf *enget(is, buf, totlen, off0)
register struct en_softc *is;
register u_char *buf;
int totlen, off0;
{
    u_char *ram = (u_char *)is->en_ram;
    struct mbuf *top, **mp;
    register struct mbuf *m;
    register int off = off0, len;
    register u_char *cp = buf + sizeof(struct en_rheader);
    struct ifnet *ifp = &is->en_if;

    top = 0;
    mp = &top;
    totlen -= sizeof(struct ether_header);
    while (totlen > 0) {
	MGET(m, M_DONTWAIT, MT_DATA);
	if (m == 0) {
	    m_freem(top);
	    top = 0;
	    goto out;
	}
	if (off) {
	    len = totlen - off;
	    cp = buf + sizeof(struct en_rheader) + off;
	    if (cp >= ram + is->en_size)
		cp -= is->en_size - EN_PGTOP(EN_PSTART);
	} else
	    len = totlen;

	if (len >= CLBYTES) {
	    if (ifp) {
		len = 0;
		goto nopage;
	    }
	    MCLGET(m);
	    if (m->m_len != CLBYTES)
		goto nopage;
	    m->m_len = MIN(len, CLBYTES);
	    goto copy;
	}
nopage:
	m->m_off = MMINOFF;
	if (ifp) {
	    m->m_len = MIN(MLEN - sizeof(ifp), len);
	    m->m_off += sizeof(ifp);
	}
	else
	    m->m_len = MIN(MLEN, len);
copy:
	cp = encopy (is, cp, mtod(m, u_char *), m->m_len);
nocopy:
	*mp = m;
	mp = &m->m_next;
	if (off) {
	    off += m->m_len;
	    if (off == totlen) {
		cp = buf + sizeof(struct en_rheader);
		off = 0;
		totlen = off0;
	    }
	}
	else
	    totlen -= m->m_len;

	if (ifp) {
	    /*
	     * Prepend interface pointer to first mbuf.
	     */
	    m->m_len += sizeof(ifp);
	    m->m_off -= sizeof(ifp);
	    *(mtod(m, struct ifnet **)) = ifp;
	    ifp = (struct ifnet *)0;
	}
    }
out:
    return (top);
}

enstart(unit)
int unit;
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    register struct mbuf *mp;
    register u_char *bp = (u_char *)is->en_ram;
    register int len = 0;
    struct mbuf *m;
    
    if (is->en_flags & ENF_OACTIVE)
	return;
    
    if (!(is->en_flags & ENF_RUNNING))
	return;

    IF_DEQUEUE(&is->en_if.if_snd, m);
    if (m == 0) {
	return;
    }

    for (mp = m; mp; mp = mp->m_next) {
	register unsigned mlen = mp->m_len;
	unsigned short temp;
	unsigned short *tp;
	u_char *mcp;
	
	if (mlen == 0)
	    continue;
	len += mlen;

	mcp = mtod(mp, u_char *);

	if(((unsigned)bp & 0x1) != 0) {
	    bp--;
	    tp = (unsigned short *)bp;
	    temp = *tp;
	    *tp++ = ((temp & 0xff00) | (*mcp++ & 0xff)) & 0xffff;
	    mlen--;
	    bp = (u_char *)tp;
	}
	wcopy((unsigned short *) mcp, (unsigned short *) bp, (int) mlen);
	bp += mlen;
    }

    if (len & 01) {
	len++;
    }

    m_freem(m);

    if (len - sizeof(struct ether_header) < ETHERMIN) 
	len = ETHERMIN + sizeof(struct ether_header);

    reg->en_tpsr = EN_XSTART;
    reg->en_tbcr(0) = (len & 0xff);
    reg->en_tbcr(1) = (len >> 8) & 0x7;

    reg->en_csr = CR_TXP | CR_STA | CR_RD2;
    is->en_flags |= ENF_XMTPENDING | ENF_OACTIVE;
    runact(EN_TOACT_CLOCK_LIST, is->en_toact, unit, hz * 3);
}

static enxintr(unit)
int unit;
{
    register struct en_softc *is = &en_softc[unit];
    int s;

    if (!(is->en_flags & ENF_OACTIVE))
	return;

    if(is->en_flags & ENF_XMTPENDING) {
	cancelact(is->en_toact);
	is->en_flags &= ~ENF_XMTPENDING;
    }

    is->en_flags &= ~ENF_OACTIVE;
    s = splimp();
    if (is->en_if.if_snd.ifq_head)
	enstart(unit);
    splx(s);
}

int	entypedebug = 0;

enrintr(unit, flags)
int unit;
int flags;
{
    register struct en_softc *is = &en_softc[unit];
    register volatile u_char *bnry = &is->en_reg->en_bnry;
    struct en_rheader		enheader;
    register struct en_rheader	*enh;
    register unsigned char	*enp;
    register short len;
    u_char tail;
    int off;
    int resid;
    register struct mbuf *m;
    struct ifqueue *inq;
    struct mbuf *temp = 0;
    int pstop = is->en_size>>EN_PGSHIFT;
    int s;

    tail = *bnry + 1;
    if (tail > (pstop - 1))
	tail = EN_PSTART;

    while (tail != en_ring_head(is->en_reg, flags)) {

	is->en_if.if_ipackets++;
	enp = (EN_PGTOP(tail) + is->en_ram);
	wcopy(enp, &enheader, sizeof (enheader));
	enh = &enheader;
	len = (u_short) ((enh->enr_count1 << 8) | enh->enr_count0);
	if (len == 0)
	    goto setup;
    
#define	endataaddr(enp, off, type)	((type)((caddr_t)((enp)+1)+(off)))

	if ((enh->enr_type >= ETHERTYPE_TRAIL) &&
	    (enh->enr_type < ETHERTYPE_TRAIL + ETHERTYPE_NTRAILER)) {
	    off = (enh->enr_type - ETHERTYPE_TRAIL) * 512;
	    if (off >= ETHERMTU)
		goto setup;
	    enh->enr_type = ntohs(*endataaddr(enp, off, u_short *));
	    resid = ntohs(*(endataaddr(enp, off + 2, u_short *)));
	    if (off + resid > len)
		goto setup;
	    len = off + resid;
	} else
	    off = 0;
	if (len == 0)
	    goto setup;

	/*
	 * Pull packet off interface.  Off is nonzero if packet
	 * has trailing header; enget will then force this header
	 * information to be at the front, but we still have to drop
	 * the type and length which are at the front of any trailer data.
	 */
	m = enget(is, enp, len, off, &is->en_if);
	if (m == 0)
	    goto setup;
	if (off) {
		struct ifnet *ifp;

		ifp = *(mtod(m, struct ifnet **));
		m->m_off += 2 * sizeof (u_short);
		m->m_len -= 2 * sizeof (u_short);
		*(mtod(m, struct ifnet **)) = ifp;
	}

	switch (enh->enr_type) {

#ifdef INET
	  case ETHERTYPE_IP:
#if	MACOS_EMUL
	    if (mac_ether_inet_ip(m) == TRUE)
		goto mac_ether_input;
#endif
	    schednetisr(NETISR_IP);
	    inq = &ipintrq;
	    break;

	  case ETHERTYPE_ARP:
#if	MACOS_EMUL
	    if (mac_ether_inet_arp(m) == TRUE)
		goto mac_ether_input;
#endif
	    arpinput(&is->en_ac, m);
	    goto setup;
#endif
#ifdef NS
	case ETHERTYPE_NS:
	    schednetisr(NETISR_NS);
	    inq = &nsintrq;
	    break;
#endif

	  default:
#if	MACOS_EMUL
mac_ether_input:
	    if (mac_ether_interface_input(&enh->enr_eh, m) == FALSE) {
		if (entypedebug)
		    printf("[et %x]", enh->enr_type);
		m_freem(m);
	    }
#else
#if    DLI
	    dli_input(m, htons(enh->enr_type), &enh->enr_src[0], 
		      &is->en_dlv, &enh->enr_eh);
#else  DLI
	    m_freem(m);
#endif DLI
#endif
	    goto setup;
	}

	s = splimp();
	if (IF_QFULL(inq)) {
	    IF_DROP(inq);
	    m_freem(m);
	} else
	    IF_ENQUEUE(inq, m);
	splx(s);

setup:
	tail = enh->enr_next;
        *bnry = ((tail == EN_PSTART) ? pstop - 1 : tail - 1);
    }
}


enintr(unit)
int unit;
{
    register struct en_softc *is = &en_softc[unit];
    register en_reg_t reg = is->en_reg;
    struct ifnet *ifp = &is->en_if;
    register int isr, rsr = 0;

    if (unit >= NEN) {
	printf("enintr:  interrupt from slot %d\n", unit);
	panic("enintr");
	/*NOTREACHED*/
    }

    /* 
     * Read interrupt status register 
     */
    if(reg->en_isr == 0) {
	printf("en%d spurious interrupt\n", unit);
	return;
    }

    /* xmit interrupts */
    if (isr = (reg->en_isr & (ISR_PTX | ISR_TXE))) {
	reg->en_isr = isr;
	if (isr & ISR_PTX)
	    ifp->if_opackets++;
	else if (reg->en_isr & ISR_TXE) {
	    ifp->if_oerrors++;
	    rsr = reg->en_rsr;
	    if (rsr & TSR_COL)
		is->en_stats.scol_err++;
	    if (rsr & TSR_ABT)
		is->en_stats.sabt_err++;
	    if (rsr & TSR_CRS)
		is->en_stats.scrs_err++;
	    if (rsr & TSR_FU)
		is->en_stats.sfu_err++;
	    if (rsr & TSR_CDH)
		is->en_stats.scdh_err++;
	    if (rsr & TSR_OWC)
		is->en_stats.sowc_err++;
	}
	enxintr(unit);
    }

    /* receive interrupts */
    if (isr = (reg->en_isr & (ISR_PRX | ISR_OVW | ISR_RXE))) {
	reg->en_isr = isr;

	if (isr & ISR_RXE) {
	    ifp->if_ierrors++;
	    rsr = reg->en_rsr;

	    if (rsr & RSR_CRC) {
		if (rsr & RSR_FAE)
		    is->en_stats.rfae_err++;
		else
		    is->en_stats.rcrc_err++;
	    }

	    if (rsr & RSR_FO)
		is->en_stats.rfo_err++;
	    
	    if (rsr & RSR_MPA) {
		is->en_stats.rmiss_err++;
	    }
	}

	if (isr & ISR_OVW)
	    enoverflow(unit);
	else
	    enrintr(unit, 0);
    }
}

enoutput(ifp, m0, dst)
    struct ifnet *ifp;
    struct mbuf *m0;
    struct sockaddr *dst;
{
    int type, s, error;
    u_char edst[6];
    struct in_addr idst;
    register struct en_softc *is = &en_softc[ifp->if_unit];
    register struct mbuf *m = m0;
    register struct ether_header *eh;
    register int off;
    int usetrailers;
    
    if ((ifp->if_flags & (IFF_UP|IFF_RUNNING)) != (IFF_UP|IFF_RUNNING)) {
	error = ENETDOWN;
	goto bad;
    }

    switch (dst->sa_family) {

#ifdef INET
    case AF_INET:
	idst = ((struct sockaddr_in *) dst)->sin_addr;
	if (!arpresolve(&is->en_ac, m, &idst, edst, &usetrailers))
	    return (0);	/* if not yet resolved */
	off = ntohs((u_short)mtod(m, struct ip *)->ip_len) - m->m_len;
	if (usetrailers && off > 0 && (off & 0x1ff) == 0 &&
	    m->m_off >= MMINOFF + 2 * sizeof (u_short)) {
		type = ETHERTYPE_TRAIL + (off>>9);
		m->m_off -= 2 * sizeof (u_short);
		m->m_len += 2 * sizeof (u_short);
		*mtod(m, u_short *) = htons((u_short)ETHERTYPE_IP);
		*(mtod(m, u_short *) + 1) = htons((u_short)m->m_len);
		goto gottrailertype;
	}
	type = ETHERTYPE_IP;
	off = 0;
	goto gottype;
#endif
#ifdef NS
	case AF_NS:
	type = ETHERTYPE_NS;
	bcopy((caddr_t)&(((struct sockaddr_ns *)dst)->sns_addr.x_host),
	      (caddr_t)edst, sizeof (edst));
	off = 0;
	goto gottype;
#endif
#if	DLI
	case AF_DLI:
	if (m->m_len < sizeof(struct ether_header)) {
	    error = EMSGSIZE;
	    goto bad;
	}
	eh = mtod(m, struct ether_header *);
	bcopy(dst->sa_data, (caddr_t)eh->ether_dhost, sizeof (eh->ether_dhost));
	goto gotheader;
#endif	DLI

    case AF_UNSPEC:
	eh = (struct ether_header *)dst->sa_data;
	bcopy(eh->ether_dhost, edst, sizeof (edst));
	type = eh->ether_type;
	goto gottype;

    default:
	printf("en%d: can't handle af%d\n", ifp->if_unit, dst->sa_family);
	error = EAFNOSUPPORT;
	goto bad;
    }

gottrailertype:
	/*
	 * Packet to be sent as trailer: move first packet
	 * (control information) to end of chain.
	 */
	while (m->m_next)
		m = m->m_next;
	m->m_next = m0;
	m = m0->m_next;
	m0->m_next = 0;
	m0 = m;

gottype:
    /*
     * Add local net header.  If no space in first mbuf, allocate
     * another. 
     */
    if (m->m_off > MMAXOFF ||
	MMINOFF + sizeof(struct ether_header) > m->m_off) {
	m = m_get(M_DONTWAIT, MT_HEADER);
	if (m == 0) {
	    error = ENOBUFS;
	    goto bad;
	}
	m->m_next = m0;
	m->m_off = MMINOFF;
	m->m_len = sizeof(struct ether_header);
    } else {
	m->m_off -= sizeof(struct ether_header);
	m->m_len += sizeof(struct ether_header);
    }
    eh = mtod(m, struct ether_header *);
    eh->ether_type = htons((u_short) type);
    bcopy(edst, eh->ether_dhost, sizeof (eh->ether_dhost));

gotheader:
    /*
     * Fill in the source address.
     */
    bcopy(is->en_enaddr, eh->ether_shost, sizeof (eh->ether_shost));

    /*
     * Queue message on interface, and start output if interface
     * not yet active.
     */
    s = splimp();
    if (IF_QFULL(&ifp->if_snd)) {
	IF_DROP(&ifp->if_snd);
	splx(s);
	m_freem(m);
	return (ENOBUFS);
    }
    IF_ENQUEUE(&ifp->if_snd, m);
    if (!(is->en_flags & ENF_OACTIVE))
	enstart(ifp->if_unit);
    splx(s);
    return (0);
    
bad:
    m_freem(m0);
    return (error);
}

enioctl(ifp, cmd, data)
    register struct ifnet *ifp;
    int	cmd;
    caddr_t data;
{
    register struct ifaddr *ifa = (struct ifaddr *)data;
    struct en_softc *is = &en_softc[ifp->if_unit];
    int s = splimp(), error = 0;
    
    switch (cmd) {
	
    case SIOCSIFADDR:
	ifp->if_flags |= IFF_UP;
	switch (ifa->ifa_addr.sa_family) {
#ifdef INET
	case AF_INET:
	    /* 
	     * Initalize the interface.  This includes setting the
	     * ethernet address for the interface.
	     */
	    eninit(ifp->if_unit);
	    ((struct arpcom *)ifp)->ac_ipaddr = IA_SIN(ifa)->sin_addr;
	    arpwhohas((struct arpcom *)ifp, &IA_SIN(ifa)->sin_addr);
	    break;
#endif	    
#ifdef NS
	  case AF_NS:
	    {
		register struct ns_addr *ina = &(IA_SNS(ifa)->sns_addr);
		
		if (ns_nullhost(*ina)) {
		    ina->x_host = * (union ns_host *)(is->en_addr);
		} else {
		    bcopy ((caddr_t)ina->x_host.c_host, is->en_addr, 
			   sizeof is->en_addr);
		    eninit(ifp->if_unit);
		    return (0);
		}
		break;
	    }
#endif
	  default:
	    error = EINVAL;
	    break;
	}
	break;

    case SIOCSIFFLAGS:
	if ((ifp->if_flags & IFF_UP) == 0 &&
	    ifp->if_flags & IFF_RUNNING) {
	    ifp->if_flags &= ~IFF_RUNNING;
	} else if (ifp->if_flags & IFF_UP &&
		   (ifp->if_flags & IFF_RUNNING) == 0)
	    eninit(ifp->if_unit);
	break;
    default:
	error = EINVAL;
	break;
    }
    splx(s);
    return (error);
}
