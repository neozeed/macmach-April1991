/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_subr.c,v $
 * Revision 2.2  90/08/30  11:48:03  bohman
 * 	Created.
 * 	[90/08/29  13:05:20  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_subr.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Slot Manager
 * internal subroutines.
 */

#include <mach/mach_types.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/ROMDefs.h>

#include <mac2slotmgr/slotmgr.h>

vm_offset_t slot_get_data();

/*
 * Retrieve a ROM header, given a
 * pointer to it and the card bytelane.
 */
vm_offset_t
slot_get_fheader(p, b, bl)
register vm_offset_t p, b;
register unsigned char bl;
{
    return (slot_get_data(p, -sizeof (FHeaderRec), b, bl));
}

/*
 * Return the card bytelane and a pointer
 * to the ROM bytelane, given a pointer
 * to a slot.
 */
vm_offset_t
slot_get_bytelane(p, var_bytelane)
register vm_offset_t p;
unsigned char *var_bytelane;
{
    register unsigned char bl;
    register i;

    /*
     * Start at end of ROM
     */
    p |= 0x00ffffff;
    for (i = 4; i > 0; i--, p--) {
	/*
	 * Fetch byte at pointer
	 * and check for valid
	 * bytelane format
	 */
	bl = byte(p);
	if ((bl & 0x0f) != (~(bl >> 4) & 0x0f))
	    continue;

	/*
	 * Only want positive
	 * bytelane nibble; also
	 * check for valid bytelane
	 * address
	 */
	bl &= 0x0f;
	if (((1 << (p & 0x3)) & bl) == 0)
	    continue;

	break;
    }

    /*
     * Failed to find valid
     * bytelane
     */
    if (i == 0)
	return ((vm_offset_t)0);

    /*
     * Return bytelane
     * value
     */
    *var_bytelane = bl;

    /*
     * Return address of
     * bytelane
     */
    return (p);
}

/*
 * Retrieve a data structure
 * from a card and return
 * a pointer to the start
 * of the structure.
 */
vm_offset_t
slot_get_data(p, n, b, bl)
register vm_offset_t p, b;
register n;
register unsigned char bl;
{
    if (n < 0) {
	/*
	 * n negative means
	 * that p points to
	 * the end of data
	 * in rom: copy backwards
	 */
	b -= (n + 1);
	for (;;) {
	    byte(b--) = byte(p);
	    if (++n >= 0)
		break;
	    decr_byte_ptr(p, bl);
	}
    }
    else if (n > 0) {
	register vm_offset_t pt = p;

	/*
	 * n positive means
	 * that p points to
	 * the beginning of data
	 * in rom: copy forwards
	 */
	for (;;) {
	    byte(b++) = byte(p);
	    if (--n <= 0)
		break;
	    incr_byte_ptr(p, bl);
	}
	/*
	 * Restore pointer to
	 * beginning of rom
	 * data
	 */
	p = pt;
    }

    /*
     * Return rom pointer
     */
    return (p);
}

/*
 * Calculate a new card pointer given
 * an old pointer and offset.
 */
vm_offset_t
slot_pointer_offset(p, offset, bl)
register vm_offset_t p;
register long offset;
register unsigned char bl;
{
    register long remainder;
    register lanes = 0, i;

    /*
     * Check to see if the
     * pointer is valid
     * for the bytelane
     */
    if (((1 << (p & 0x3)) & bl) == 0)
	return ((vm_offset_t)0);

    /*
     * Convert the signed 24 bit
     * offset to a signed 32 bit
     * value
     */
    offset = extend_offset(offset);

    /*
     * Calculate the number
     * of bytelanes in use
     */
    for (i = 0; i < 4; i++)
	lanes += ((bl >> i) & 0x1);

    /*
     * Calculate actual byte
     * offset and remainder
     */
    remainder = offset % lanes;
    offset = (offset / lanes) * 4;

    p += offset;

    /*
     * Positive remainder
     * for positive offset
     */
    while (remainder > 0) {
	incr_byte_ptr(p, bl);
	remainder--;
    }

    /*
     * Negative remainder
     * for negative offset
     */
    while (remainder < 0) {
	decr_byte_ptr(p, bl);
	remainder++;
    }

    return (p);
}

/*
 * Calculate slot ROM CRC.
 */
static inline
unsigned long
crc_add(crc, byte)
unsigned long crc;
unsigned char byte;
{
    asm("roll #1,%0" : "=d" (crc) : "0" (crc));

    return (crc + (unsigned long)byte);
}

unsigned long
slot_calc_crc(p, length, bl)
register vm_offset_t p;
register long length;
register unsigned char bl;
{
    register vm_offset_t ptr_crc, ptr_past_crc;
    register unsigned long crc = 0;

    {
	register FHeaderRecPtr fh = 0;

	/*
	 * Calculate pointer to beginning
	 * of CRC field in rom
	 */
	ptr_crc = slot_pointer_offset(p, -((int)&fh->fhByteLanes -
					  (int)&fh->fhCRC),
				      bl);

	/*
	 * Calculate pointer past CRC
	 * field in rom
	 */
	ptr_past_crc = slot_pointer_offset(ptr_crc, sizeof (long), bl);
    }

    /*
     * Calculate pointer to beginning
     * of rom
     */
    p = slot_pointer_offset(p, -length + 1, bl);

    /*
     * Perform CRC calculation for
     * specified length.  Treat
     * CRC field in rom as 0.
     *
     * crc_add() performs the 32
     * bit rotate left and add
     * function
     */
    while (length-- > 0) {
	if (p < ptr_crc || p >= ptr_past_crc)
	    crc = crc_add(crc, byte(p));
	else
	    crc = crc_add(crc, 0);

	incr_byte_ptr(p, bl);
    }

    return (crc);
}
