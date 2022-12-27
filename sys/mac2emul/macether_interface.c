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
 *	File: mac2emul/macether_interface.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>
#include <sys/mbuf.h>

#include <mach/mach_types.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macether_types.h>
#include <mac2emul/macether_hash.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <netinet/in_systm.h>
#include <netinet/ip.h>

#include <netinet/in_var.h>

extern struct arpcom	en_softc;	/* XXX */
static struct arpcom	*en = (struct arpcom *)&en_softc;

/*
 * interface routines for the mac ether.
 */

boolean_t
mac_ether_q_send(packet, length)
ether_packet_t	packet;
unsigned int	length;
{
    register struct ifnet	*ifp = &en->ac_if;
    register struct mbuf	*m0 = 0, *m, **n;
    register unsigned int	mlen;

    if ((ifp->if_flags & (IFF_UP|IFF_RUNNING)) != (IFF_UP|IFF_RUNNING))
	return (FALSE);

    if (length > (2 * MCLBYTES))
	return (FALSE);

    while (length > 0) {
	MGET(m, M_DONTWAIT, MT_DATA);
	if (m == 0)
	    goto bad;
#ifdef notdef
	if (length > (5 * MLEN)) {
	    MCLGET(m);
	    if (m->m_len < MCLBYTES) {
		m_free(m);
		goto bad;
	    }
	    mlen = m->m_len = MIN(length, CLBYTES);
	}
	else
#endif
	    mlen = m->m_len = MIN(MLEN, length);

	bcopy(packet, mtod(m, ether_packet_t), mlen);

	if (m0 == 0)
	    m0 = m;
	else
	    *n = m;
	n = &m->m_next;

	packet += mlen; length -= mlen;
    }

    {
	register s = splimp();

	if (IF_QFULL(&ifp->if_snd)) {
	    IF_DROP(&ifp->if_snd);
	    splx(s);
	    goto bad;
	}
	IF_ENQUEUE(&ifp->if_snd, m0);
	enstart(ifp->if_unit);
	splx(s);
    }
    return (TRUE);

  bad:
    m_freem(m0);
    return (FALSE);
}

boolean_t
mac_ether_dq_receive(packet, _length, thread)
ether_packet_t	packet;
unsigned int	*_length;
thread_t	thread;
{
    register unsigned int	length = 0, mlen;
    register struct mbuf 	*m, *n;
    register struct ifqueue 	*ifq = &thread->pcb->pcb_emul->ether_rcv;

    {
	register s = splimp();

	IF_DEQUEUE(ifq, m);
#ifdef notdef
	if (ifq->ifq_len > 0) {
	    register struct mac_internal *mac = thread->pcb->pcb_emul;

	    if (mac->softintr[MAC_INTR_ETHER].flags&MAC_IF_ENB)
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_ETHER);
	}
#endif
	splx(s);
    }

    if (m == 0)
	return (FALSE);

    while (m != 0) {
	mlen = m->m_len;
	bcopy(mtod(m, ether_packet_t), packet, mlen);

	MFREE(m, n);
	m = n;

	packet += mlen; length += mlen;
    }

    *_length = length;

    return (TRUE);
}

static
boolean_t
mac_ether_m_copydata(m, off, len, buf)
register struct mbuf	*m;
register unsigned	off;
register unsigned	len;
register unsigned char	*buf;
{
    register unsigned	count;

    while (off > 0) {
	if (m == 0)
	    return (FALSE);
	if (off < m->m_len)
	    break;
	off -= m->m_len;
	m = m->m_next;
    }
    while (len > 0) {
	if (m == 0)
	    return (FALSE);
	count = MIN(m->m_len - off, len);
	bcopy(mtod(m, caddr_t) + off, buf, count);
	len -= count;
	buf += count;
	off = 0;
	m = m->m_next;
    }

    return (TRUE);
}

boolean_t
mac_ether_inet_arp(m)
struct mbuf	*m;
{
    register struct in_addr	*in;
    struct arpcom		*ac = en;
    struct ether_arp		arp;

    if (mac_ether_m_copydata(m,
			     sizeof (struct ifnet *),
			     sizeof (arp),
			     &arp) == FALSE)
	return (FALSE);

    if (ntohs(arp.arp_hrd) != ARPHRD_ETHER)
	return (FALSE);

    if (ntohs(arp.arp_pro) != ETHERTYPE_IP)
	return (FALSE);

    in = (struct in_addr *)arp.arp_tpa;
    if (in->s_addr == ac->ac_ipaddr.s_addr)
	return (FALSE);

    return (TRUE);
}

boolean_t
mac_ether_inet_ip(m)
struct mbuf	*m;
{
    register struct in_ifaddr	*ia;
    register struct in_addr	*in;
    int				hlen;
    struct ip			ip;

    if (mac_ether_m_copydata(m,
			     sizeof (struct ifnet *),
			     sizeof (struct ip),
			     &ip) == FALSE)
	return (FALSE);

    hlen = ip.ip_hl << 2;
    if (hlen < sizeof (struct ip))
	return (FALSE);

    in = &ip.ip_dst;

    for (ia = in_ifaddr; ia; ia = ia->ia_next) {
#define	satosin(sa)	((struct sockaddr_in *)(sa))
	if (in->s_addr == satosin(&ia->ia_addr)->sin_addr.s_addr)
	    return (FALSE);

	if (ia->ia_ifp->if_flags & IFF_BROADCAST) {
	    if (in->s_addr == satosin(&ia->ia_broadaddr)->sin_addr.s_addr)
		return (FALSE);

	    if (in->s_addr == ia->ia_netbroadcast.s_addr)
		return (FALSE);

	    /*
	     * Check for old-style (host 0) broadcast.
	     */
	    if (in->s_addr == ia->ia_subnet || in->s_addr == ia->ia_net)
		return (FALSE);
	}
    }

    if (in->s_addr == INADDR_BROADCAST || in->s_addr == INADDR_ANY)
	return (FALSE);
    
    return (TRUE);
}

boolean_t
mac_ether_interface_input(eh, m)
register struct ether_header	*eh;
register struct mbuf		*m;
{
    register thread_t		thread;
    register struct ifqueue	*ifq;
    register struct mbuf	*n;
    register unsigned short	proto = eh->ether_type;
    extern thread_t		mac_ether_proto_hash_lookup();

    if (is802_3(proto) && !inuse802_3)
	proto = 0;

    thread = mac_ether_proto_hash_lookup(proto);
    if (thread == THREAD_NULL)
	return (FALSE);
    
    MGET(n, M_DONTWAIT, MT_HEADER);
    if (n == 0)
	return (FALSE);

    ifq = &thread->pcb->pcb_emul->ether_rcv;
    {
	register s = splimp();

	if (IF_QFULL(ifq)) {
	    IF_DROP(ifq);
	    m_free(n);
	    splx(s);
	    return (FALSE);
	}
	else {
	    register struct mac_internal *mac = thread->pcb->pcb_emul;

	    IF_ADJ(m);

	    bcopy(eh,
		  mtod(n, struct ether_header *),
		  sizeof (struct ether_header));

	    n->m_len = sizeof (struct ether_header);
	    n->m_next = m; m = n;

	    IF_ENQUEUE(ifq, m);

	    if (mac->softintr[MAC_INTR_ETHER].flags&MAC_IF_ENB) {
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_ETHER);
		check_mac_interrupts(thread->pcb);
	    }
	}

	splx(s);
    }

    return (TRUE);
}

mac_ether_interface_address(address)
ether_address_t		address;
{
    register struct ifnet	*ifp = &en->ac_if;
    
    if ((ifp->if_flags & (IFF_UP|IFF_RUNNING)) != (IFF_UP|IFF_RUNNING))
	return (FALSE);

    bcopy(en->ac_enaddr, address, sizeof (en->ac_enaddr));

    return (TRUE);
}

mac_ether_interface_terminate(thread)
thread_t	thread;
{
    register struct ifqueue	*ifq = &thread->pcb->pcb_emul->ether_rcv;
    register struct mbuf	*m;
    register short		*p = thread->pcb->pcb_emul->ether_multi_class;
    register s;

    mac_ether_proto_hash_terminate(thread);

    s = splimp();
    for (;;) {
	IF_DEQUEUE(ifq, m);
	if (m == 0)
	    break;

	m_freem(m);
    }
    splx(s);

    for (s = 0; s < 64; p++, s++)
	if (*p > 0) {
	    en_multicast_class_decr(0, s, *p);
	    *p = 0;
	}
}

boolean_t
mac_ether_interface_add_multi(address, thread)
ether_address_t		address;
thread_t		thread;
{
    register struct ifnet	*ifp = &en->ac_if;
    register short		*p = thread->pcb->pcb_emul->ether_multi_class;
    register 			class;
    
    if ((ifp->if_flags & (IFF_UP|IFF_RUNNING)) != (IFF_UP|IFF_RUNNING))
	return (FALSE);

    class = ether_multicast_class(address);
    if ((p[class] + 1) < 0 ||
	en_multicast_class_incr(0, class) != TRUE)
	return (FALSE);

    p[class]++;

    return (TRUE);
}

boolean_t
mac_ether_interface_delete_multi(address, thread)
ether_address_t		address;
thread_t		thread;
{
    register struct ifnet	*ifp = &en->ac_if;
    register short		*p = thread->pcb->pcb_emul->ether_multi_class;
    register			class;
    
    if ((ifp->if_flags & (IFF_UP|IFF_RUNNING)) != (IFF_UP|IFF_RUNNING))
	return (FALSE);

    class = ether_multicast_class(address);
    if ((p[class] - 1) < 0 ||
	en_multicast_class_decr(0, class, 1) != TRUE)
	return (FALSE);

    p[class]--;

    return (TRUE);
}

static inline
unsigned long
ether_crc_enter_bit(reg, data)
register unsigned long	reg;
register unsigned long	data;
{
    register			bit;
    register unsigned long	tmp;

    /*
     * First the input bit is xored with
     * the ms bit in the shift register.
     */
    asm("bfextu	%1{#0:#1},%0" : "=d" (tmp) : "d" (reg));

    data ^= tmp;

    for (bit = 31; bit >= 0; bit--)
	switch (bit) {
	    /*
	     * Special case: low order bit
	     * equals input bit.
	     */
	  case 0:
	    asm("bfins	%1,%0{%2:#1}" :
		"=d" (reg) :
		"d" (data), "d" (31 - bit), "0" (reg));
	    break;

	    /*
	     * These bits equal the input bit
	     * xor the next lower bit.
	     */
	  case 1: case 2: case 4: case 5:
	  case 7: case 8: case 10: case 11:
	  case 12: case 16: case 22: case 23: case 26:
	    asm("bfextu	%1{%2:#1},%0" :
		"=d" (tmp) :
		"d" (reg), "d" (31 - bit + 1));
	    tmp ^= data;
	    asm("bfins	%1,%0{%2:#1}" :
		"=d" (reg) :
		"d" (tmp), "d" (31 - bit), "0" (reg));
	    break;

	    /*
	     * The rest equal the next lower
	     * bit.
	     */
	  default:
	    asm("bfextu	%1{%2:#1},%0" :
		"=d" (tmp) :
		"d" (reg), "d" (31 - bit + 1));
	    asm("bfins	%1,%0{%2:#1}" :
		"=d" (reg) :
		"d" (tmp), "d" (31 - bit), "0" (reg));
	    break;
	}

    return (reg);
}

unsigned long
ether_multicast_class(address)
ether_address_t		address;
{
    register unsigned long	reg, data;
    register			byte, bit;
    register unsigned char	*p = address;

    reg = 0xffffffff;
    for (byte = 0; byte < sizeof (ether_address_t); p++, byte++)
	for (bit = 0; bit < 8; bit++) {
	    asm("bfextu	%1{%2:#1},%0" :
		"=d" (data) :
		"m" (*p), "d" (7 - bit));
	    reg = ether_crc_enter_bit(reg, data);
	}

    asm("bfextu	%1{#0:#6},%0" : "=d" (data) : "d" (reg));

    return (data);
}
