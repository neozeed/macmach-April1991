/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_interrupts.c,v $
 * Revision 2.2  90/08/30  11:47:52  bohman
 * 	Created.
 * 	[90/08/29  13:04:34  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_interrupts.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Slot Interrupt
 * handling.  See
 * "Inside Macintosh V".
 */

#include <mach/mach_types.h>

#include <machine/ffsl_inline.c>

#include <mac2/act.h>

#include <mac2dev/via.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/ROMDefs.h>

#include <mac2slotmgr/slotmgr.h>

struct actlist	actslot[SLOT_NUM_HIGH-SLOT_NUM_LOW+1];

SIntInstall(sqe, slot)
register SQElemPtr sqe;
register slot;
{
    register vm_offset_t *ptr_link;
    register s = spl2();

    if (!slot_in_range(slot)) {
	splx(s);
	return (smSlotOOBErr);
    }

    ptr_link = &slot_to_slotdata_ptr(slot)->SIntrQue;
    for (;;) {
	if (*ptr_link == 0 || ((SQElemPtr)*ptr_link)->sqPrio < sqe->sqPrio) {
	    sqe->sqLink = *ptr_link;
	    *ptr_link = (vm_offset_t)sqe;
	    break;
	}
	else
	    ptr_link = (vm_offset_t *)&((SQElemPtr)*ptr_link)->sqLink;
    }
    splx(s);

    return (noErr);
}

SIntRemove(sqe, slot)
register SQElemPtr sqe;
register slot;
{
    register vm_offset_t *ptr_link;
    register s = spl2();

    if (!slot_in_range(slot)) {
	splx(s);
	return (smSlotOOBErr);
    }

    ptr_link = &slot_to_slotdata_ptr(slot)->SIntrQue;
    for (;;) {
	if (*ptr_link == 0)
	    break;
	else if (*ptr_link == (vm_offset_t)sqe) {
	    *ptr_link = sqe->sqLink;
	    break;
	}
	else
	    ptr_link = (vm_offset_t *)&((SQElemPtr)*ptr_link)->sqLink;
    }
    splx(s);

    return (noErr);
}

/*
 * Called from VIA2
 * interrupt handler to
 * handle a slot interrupt.
 */
SInterrupt()
{
    register unsigned long reg;
    register bit;
    register via_reg_t vp = VIA2_ADDR;
    register SQElemPtr sqe;

    for (;;) {
	vp->ifr = VIA_IF_CA1;
	reg = ~vp->rega & 0x3f;
	bit = ffsl(reg);
	if (bit < 0)
	    break;

	if (CHECKACTLIST(actslot[bit]))
	    doactlist(&actslot[bit]);

	sqe = (SQElemPtr)slots[bit].SIntrQue;
	for (;;) {
	    if (sqe == 0) {
		printf("Slot %x\n", SLOT_NUM_LOW + bit);
		panic("SInterrupt: not serviced");
	    }

	    if (SCallIntr(sqe->sqAddr, sqe->sqParm))
		break;

	    sqe = (SQElemPtr)sqe->sqLink;
	}
    }
}
