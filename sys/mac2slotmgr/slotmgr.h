/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr.h,v $
 * Revision 2.2  90/08/30  11:47:17  bohman
 * 	Created.
 * 	[90/08/29  13:01:09  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * slot manager defines.
 */

#include <kern/queue.h>

struct slotdata {
    vm_offset_t		SIntrQue;
    int			SFlags;
#define SLOT_MAPPABLE	0x01
    SInfoRecord		SInfoRec;
    SPRAMRecord		SPRAMRec;
    queue_head_t	SRsrcLst;
};

typedef struct SRsrcLstEnt {
    queue_chain_t	SRsrcLst;
    vm_offset_t		SRsrcPtr;
    unsigned long	SRsrcId;
    struct SRsrcType {
	unsigned short	Category;
	unsigned short	cType;
	unsigned short	DrvrSW;
	unsigned short	DrvrHW;
    } SRsrcType;
#define SRsrcType_Category	SRsrcType.Category
#define SRsrcType_cType		SRsrcType.cType
#define SRsrcType_DrvrSW	SRsrcType.DrvrSW
#define SRsrcType_DrvrHW	SRsrcType.DrvrHW
    unsigned long	SRsrcHWDevID;
    unsigned long	SRsrcExtDev;
    unsigned long	SRsrcIOReserved;
#define SRsrcIOReserved_Ignore	0x0001
} SRsrcLstEnt, *SRsrcLstEntPtr;

/*
 * Slot number range
 */
#define	SLOT_NUM_LOW	0x9
#define SLOT_NUM_HIGH	0xe

/*
 * Perform range check on
 * a slot number
 */
#define slot_in_range(slot)	\
    ((slot) >= SLOT_NUM_LOW && (slot) <= SLOT_NUM_HIGH)

/*
 * Convert a slot number into a
 * table index
 */
#define slot_to_index(slot)	((slot) - SLOT_NUM_LOW)

/*
 * Convert a slot number into a slot
 * base address (slot space)
 */
#define slot_to_ptr(slot)	\
    ((vm_offset_t)(0xf0000000 | ((slot) << 24)))

/*
 * Convert a (pointer to a slot) to (an offset
 * within the slot)
 */
#define slot_ptr_to_offset(p)	\
    ((vm_offset_t)(p) & 0x00ffffff)

/*
 * Convert (an offset within a slot) to
 * (a pointer to the slot)
 */
#define slot_offset_to_ptr(slot, offset)	\
    (slot_to_ptr(slot) | ((vm_offset_t)(offset) & 0x00ffffff)

/*
 * Convert a slot number into a slot
 * base address (super slot space)
 */
#define slot_to_super_ptr(slot)	\
    ((vm_offset_t)((slot) << 28))

#define slot_check_status(slot, action)					\
{									\
    register status;							\
\
    status = slot_to_slotdata_ptr(slot)->SInfoRec.siInitStatusA;	\
    if (status)								\
	action;								\
}

/*
 * Access a byte in rom
 * at vm_offset p
 */
#define byte(p)		(*(unsigned char *)(p))

/*
 * The id/offset data
 * type
 */
struct rom_idoffset {
    unsigned long	id:8,
		    offset:24;
};

/*
 * SExec block header
 */
typedef struct SExecHeader {
    unsigned long	rev:8,
			CPU:8,
			reserved:16;
    unsigned long	offset;
} SExecHeader, *SExecHdrPtr;
    
/*
 * Convert a 24 bit
 * signed offset into
 * a signed 32 bit quantity
 */
#define extend_offset(o)	\
    (((o) & 0x00800000) ? (0xff000000 | (o)) : (o))

/*
 * Decrement a rom byte
 * pointer to point to the
 * previous byte
 */
#define decr_byte_ptr(p, bl)		\
{					\
    (p)--;				\
    for (;;) {				\
	if ((1 << ((p) & 0x3)) & (bl))	\
	    break;			\
	else				\
	    (p)--;			\
    }					\
}

/*
 * Increment a rom byte
 * pointer to point to the
 * next byte
 */
#define incr_byte_ptr(p, bl)		\
{					\
    (p)++;				\
    for (;;) {				\
	if ((1 << ((p) & 0x3)) & (bl))	\
	    break;			\
	else				\
	    (p)++;			\
    }					\
}

/*
 * Convert a slot number into a
 * slotdata ptr
 */
#define slot_to_slotdata_ptr(slot)	(&slots[slot_to_index(slot)])

extern struct slotdata slots[];
