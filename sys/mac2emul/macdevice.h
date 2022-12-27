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
 *	File: mac2emul/macdevice.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Device emulation
 * defines
 */

#define DEV_VALID_MASK  0xFFF00000
#define DEV_TYPE_MASK   0x0003E000
#define DEV_TYPE_SHIFT  13
#define DEV_OFFSET_MASK 0x00001FFF

#define DEV_VALID_ADDR_1 0x50F00000
#define DEV_VALID_ADDR_2 0x50000000
#define DEV_VALID_ADDR_3 0x00F00000

int	device_none();
int	device_ones(), device_null();
int	device_via1();
int	device_scc();
int	device_via2();
#define device_scsi_h	device_none
#define device_scsi	device_null
#define device_scsi_d	device_none
#define device_sound	device_none
#define device_iwm	device_none
