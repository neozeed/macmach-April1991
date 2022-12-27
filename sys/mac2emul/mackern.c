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
 *	File: mac2emul/mackern.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <sys/user.h>

#include <kern/thread.h>
#include <kern/zalloc.h>

#include <mach/vm_param.h>

#include <mac2/pcb.h>
#include <mac2/clock.h>
#include <mac2/act.h>

#include <mac2dev/via.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Slots.h>

#include <mac2slotmgr/slotmgr.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macserver_types.h>
#include <mac2emul/macinst.h>
#include <mac2emul/macvia.h>

int	macdebug = 0;

zone_t	maczone;

#define INTR_CLOCK_LIST		0
#define	INTR_TIMER1_LIST	1
#define INTR_TIMER2_LIST	2
#define INTR_SOUND_LIST		3
#define INTR_KB_LIST		4
#define INTR_INTRSW_LIST	5
#define INTR_SLOT_9_LIST	6
#define INTR_SLOT_A_LIST	7
#define INTR_SLOT_B_LIST	8
#define INTR_SLOT_C_LIST	9
#define INTR_SLOT_D_LIST	10
#define INTR_SLOT_E_LIST	11

#define INTR_LIST_NUMB		12

struct actlist	acttimer1, acttimer2, actsound, actintrsw;
extern struct actlist	actkb;
extern struct actlist	actslot[];

static char	task_name[MAXCOMLEN+1] = "mac emulator";

mac_intr(pcb, list, ap)
struct pcb *pcb;
register int list;
struct act *ap;
{
    register struct mac_internal *mac = pcb->pcb_emul;
    register struct softintr *intr;

    switch (list) {
      case INTR_INTRSW_LIST:
	intr = &mac->softintr[MAC_INTR_INTR];
	if (intr->flags&MAC_IF_ENB) {
	    mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_INTR);
	    pcb->pcb_ast |= TRACE_AST;
	}
	break;

      case INTR_CLOCK_LIST:
	intr = &mac->softintr[MAC_INTR_CLK];
	if (intr->flags&MAC_IF_ENB && mac->via1.IER&VIA_IE_CA1) {
	    mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_CLK);
	    check_mac_interrupts(pcb);
	}
	break;

      case INTR_KB_LIST:
	intr = &mac->softintr[MAC_INTR_INP];
	if (intr->flags&MAC_IF_ENB) {
	    mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_INP);
	    check_mac_interrupts(pcb);
	}
	break;

      case INTR_SOUND_LIST:
	intr = &mac->softintr[MAC_INTR_ASC];
	if (intr->flags&MAC_IF_ENB) {
	    mac->via2.IFR |= VIA_IF_CB1;
	    if (mac->via2.IER&VIA_IE_CB1) {
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_ASC);
		check_mac_interrupts(pcb);
	    }
	}
	break;

      case INTR_TIMER1_LIST:
	intr = &mac->softintr[MAC_INTR_TIMER1];
	if (intr->flags&MAC_IF_ENB) {
	    mac->via1.IFR |= VIA_IF_TIMER1;
	    if (mac->via1.IER&VIA_IE_TIMER1) {
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_TIMER1);
		check_mac_interrupts(pcb);
	    }
	}
	break;
	
      case INTR_TIMER2_LIST:
	intr = &mac->softintr[MAC_INTR_TIMER2];
	if (intr->flags&MAC_IF_ENB) {
	    mac->via1.IFR |= VIA_IF_TIMER2;
	    if (mac->via1.IER&VIA_IE_TIMER2) {
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_TIMER2);
		check_mac_interrupts(pcb);
	    }
	}
	break;

      case INTR_SLOT_9_LIST:
      case INTR_SLOT_A_LIST:
      case INTR_SLOT_B_LIST:
      case INTR_SLOT_C_LIST:
      case INTR_SLOT_D_LIST:
      case INTR_SLOT_E_LIST:
	intr = &mac->softintr[MAC_INTR_SLOT];
	if (intr->flags&MAC_IF_ENB) {
	    mac->via2.portA.dat &= ~(1 << (list - INTR_SLOT_9_LIST));
	    if (mac->via2.IER&VIA_IE_CA1) {
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_SLOT);
		check_mac_interrupts(pcb);
	    }
	}
	break;
    }

    runact(list, ap, pcb, 0);
}

mactimer1_intr()
{
    DOACTLIST(acttimer1);

    VIA1_ADDR->ier = VIA_IE_TIMER1;
}

mactimer2_intr()
{
    DOACTLIST(acttimer2);

    VIA1_ADDR->ier = VIA_IE_TIMER2;
}

macsound_intr()
{
    DOACTLIST(actsound);

    VIA2_ADDR->ier = VIA_IE_CB1;
}

macintrsw_intr()
{
    DOACTLIST(actintrsw);
}

kern_return_t
mac_kern_create(thread)
register thread_t	thread;
{
    register struct pcb	*pcb = thread->pcb;
    register struct mac_internal *mac;

    if (pcb->pcb_emul)
	return (KERN_FAILURE);

    if (maczone == ZONE_NULL) {
	maczone = zinit(sizeof (struct mac_internal),
			sizeof (struct mac_internal),
			sizeof (struct mac_internal),
			FALSE, "mac_internal");

	if (maczone == ZONE_NULL)
	    return (KERN_RESOURCE_SHORTAGE);
    }
/* XXX */
    if (maczone->count >= 1)
	return (KERN_FAILURE);
/* XXX */
    mac = (struct mac_internal *)zalloc(maczone);
    if (mac) {
	struct softintr *si;

/*
 * XXX This initialization should
 * be done with a template.
 */
	bzero(mac, sizeof (*mac));
	queue_init(&mac->ether_proto);
	mac->ether_rcv.ifq_maxlen = 32;
	mac->sr = INIT_R_SR;
	CTRL_INIT(R_SFC);
	CTRL_INIT(R_DFC);
	CTRL_INIT(R_CACR);
	CTRL_INIT(R_USP);
	CTRL_INIT(R_VBR);
	CTRL_INIT(R_CAAR);
	CTRL_INIT(R_MSP);
	CTRL_INIT(R_ISP);

	si = &mac->softintr[MAC_INTR_INTR];
	si->ipl = 7; si->flags = MAC_IF_ASYNC;
	
	si = &mac->softintr[MAC_INTR_ASC];
	si->ipl = 2;

	si = &mac->softintr[MAC_INTR_TIMER1];
	si->ipl = 1;

	si = &mac->softintr[MAC_INTR_TIMER2];
	si->ipl = 1;

	si = &mac->softintr[MAC_INTR_INP];
	si->ipl = 1; si->flags = MAC_IF_ASYNC;

	si = &mac->softintr[MAC_INTR_CLK];
	si->ipl = 1; si->flags = MAC_IF_ASYNC;

	si = &mac->softintr[MAC_INTR_SR];
	si->ipl = 1;

	si = &mac->softintr[MAC_INTR_SLOT];
	si->ipl = 2; si->flags = MAC_IF_ASYNC;

	si = &mac->softintr[MAC_INTR_ETHER];
	si->ipl = 2; si->flags = MAC_IF_ASYNC;

	mac->via1.portA.dat = 0x80;
	mac->via1.portB.dat = 0x88;
	mac->via2.portA.dat = 0x7f;

	mac->intr = makeact(mac_intr, SR_IPL7, INTR_LIST_NUMB);
	if (mac->intr) {
	    register slot;

	    addact(INTR_CLOCK_LIST, mac->intr, &actclock);
	    runact(INTR_CLOCK_LIST, mac->intr, pcb, 0);

	    addact(INTR_KB_LIST, mac->intr, &actkb);
	    runact(INTR_KB_LIST, mac->intr, pcb, 0);

	    addact(INTR_TIMER1_LIST, mac->intr, &acttimer1);
	    runact(INTR_TIMER1_LIST, mac->intr, pcb, 0);

	    addact(INTR_TIMER2_LIST, mac->intr, &acttimer2);
	    runact(INTR_TIMER2_LIST, mac->intr, pcb, 0);

	    addact(INTR_SOUND_LIST, mac->intr, &actsound);
	    runact(INTR_SOUND_LIST, mac->intr, pcb, 0);

	    addact(INTR_INTRSW_LIST, mac->intr, &actintrsw);
	    runact(INTR_INTRSW_LIST, mac->intr, pcb, 0);

	    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++)
		if (slot_to_slotdata_ptr(slot)->SFlags&SLOT_MAPPABLE) {
		    addact(INTR_SLOT_9_LIST+slot_to_index(slot),
			   mac->intr,
			   &actslot[slot_to_index(slot)]);
		    runact(INTR_SLOT_9_LIST+slot_to_index(slot),
			   mac->intr,
			   pcb, 0);
		}

	}
	else {
	    zfree(maczone, mac);
	    return (KERN_FAILURE);
	}
    }
    else
	return (KERN_FAILURE);

    pcb->pcb_emul = mac;

    bcopy(task_name, thread->u_address.utask->uu_comm, sizeof (task_name));

    return (KERN_SUCCESS);
}

boolean_t
mac_kern_emulate(thread,
		 reset_vector,
		 os_vector, os_map,
		 tbox_vector, tbox_map)
thread_t	thread;
unsigned long	reset_vector, os_vector, tbox_vector;
mac_os_map_t	os_map;
mac_tbox_map_t	tbox_map;
{
    register struct pcb			*pcb = thread->pcb;
    register struct mac_internal	*mac = pcb->pcb_emul;

    if ((pcb->pcb_flags&MAC_EMULATION) == 0) {
	mac->reset_vector = reset_vector;

	mac->os_vector = os_vector;
	bcopy(os_map, mac->os_map, sizeof (mac_os_map_t));
	
	mac->tbox_vector = tbox_vector;
	bcopy(tbox_map, mac->tbox_map, sizeof (mac_tbox_map_t));
	
	pcb->pcb_flags |= MAC_EMULATION;

	return (TRUE);
    }

    return (FALSE);
}

kern_return_t
mac_kern_map_compat(thread,
		    address, size,
		    mask, anywhere,
		    memory_object,
		    offset, copy,
		    cur_protection,
		    max_protection,
		    inheritance)
thread_t	thread;
vm_offset_t	*address;
vm_size_t	size;
vm_offset_t	mask;
boolean_t	anywhere;
memory_object_t	memory_object;
vm_offset_t	offset;
boolean_t	copy;
vm_prot_t	cur_protection;
vm_prot_t	max_protection;
vm_inherit_t	inheritance;
{
    kern_return_t	result;
    struct mac_internal	*mac = thread->pcb->pcb_emul;

    if (mac->task_compat_map == VM_MAP_NULL)
	return (KERN_FAILURE);

    result = vm_map(mac->task_compat_map,
		    address, size,
		    mask, anywhere,
		    memory_object,
		    offset, copy,
		    cur_protection,
		    max_protection,
		    inheritance);

    return (result);
}

kern_return_t
mac_kern_input_event(thread, event)
thread_t		thread;
mac_input_event_t	*event;
{
    kern_return_t	result;

    result = mac_get_input(thread, event);

    return (result);
}

mac_kern_destroy(thread)
thread_t	thread;
{
    register struct pcb *pcb = thread->pcb;
    register struct mac_internal *mac = pcb->pcb_emul;
    register s;

    s = splhigh();
    pcb->pcb_flags &= ~MAC_EMULATION;
    splx(s);

    mac_ether_interface_terminate(thread);
    mac_compat_map_cleanup(thread);
    endact(mac->intr);
    macscsi_end(&mac->scsi);
    pcb->pcb_emul = 0;
    zfree(maczone, mac);
}

mac_reset(pcb)
register struct pcb *pcb;
{
    register struct mac_internal *mac = pcb->pcb_emul;

    mac_ether_interface_terminate(pcb->pcb_thread);
    mac->softintr_pend = 0;
    mac->adb_trans_state = 0;
    bzero(&mac->via1, sizeof (struct via_device));
    bzero(&mac->via2, sizeof (struct via_device));

	mac->via1.portA.dat = 0x80;
	mac->via1.portB.dat = 0x88;
	mac->via2.portA.dat = 0x7f;
}

kern_return_t
mac_kern_interrupt_vector(thread, which, address)
thread_t	thread;
int		which;
vm_offset_t	address;
{
    register struct mac_internal *mac = thread->pcb->pcb_emul;

    if (which < 0 || which >= MAC_INTR_NUMB)
	return (KERN_INVALID_ARGUMENT);

    {
	register s = splhigh();

	mac->softintr[which].vector = address;

	splx(s);
    }

    return (KERN_SUCCESS);
}

kern_return_t
mac_kern_interrupt_enable(thread, which, enable)
thread_t	thread;
int		which;
boolean_t	enable;
{
    register struct mac_internal *mac = thread->pcb->pcb_emul;

    if (which < 0 || which >= MAC_INTR_NUMB)
	return (KERN_INVALID_ARGUMENT);

    {
	register s = splhigh();

	if (enable == FALSE) {
	    mac->softintr_pend &= ~MAC_INTR_PEND(which);
	    mac->softintr[which].flags &= ~MAC_IF_ENB;
	}
	else
	    mac->softintr[which].flags |= MAC_IF_ENB;

	splx(s);
    }

    return (KERN_SUCCESS);
}

kern_return_t
mac_kern_interrupt_post(thread, which, post)
thread_t	thread;
int		which;
boolean_t	post;
{
    register struct mac_internal *mac = thread->pcb->pcb_emul;

    if (which < 0 || which >= MAC_INTR_NUMB)
	return (KERN_INVALID_ARGUMENT);

    {
	register s = splhigh();

	if (post == FALSE)
	    mac->softintr_pend &= ~MAC_INTR_PEND(which);
	else if (mac->softintr[which].flags & MAC_IF_ENB)
	    mac->softintr_pend |= MAC_INTR_PEND(which);

	splx(s);
    }

    check_mac_interrupts(thread->pcb);

    return (KERN_SUCCESS);
}
