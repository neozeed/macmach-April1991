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
 *	File: emul/patches/input.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * mac keyboard and
 * mouse handling
 */

#include <mach.h>

#include <mac2emul/macdefs.h>

#include "mac.h"

#define NewCrsrX ((unsigned short *)0x828)
#define NewCrsrY ((unsigned short *)0x82a)
#define EnableMouse() *((unsigned char *)0x8ce) = 0xff
#define Ticks ((unsigned long *)0x16a)
#define Button ((unsigned char *)0x172)
#define ButtonTick ((unsigned long *)0x16e)
#define ButtonTick1 ((unsigned long *)0x156)
#define ModFlags ((unsigned short *)0x17a)

extern port_t	kernel_request_port;
unsigned char newkey;
unsigned short modflg = 0;
#define KEY_MOD_COMND 0x8000
#define KEY_MOD_SHFT  0x0001
#define KEY_MOD_CAPLK 0x0002
#define KEY_MOD_OPTN  0x0004
#define KEY_MOD_CNTRL 0x0008

input_events()
{
    register		m, retmask = 0;
#define INPUT_EV_MSBUT 0x00000001
#define INPUT_EV_MSMOV 0x00000002
#define INPUT_EV_KEY   0x00000004
    mac_input_event_t	event;

    if (mac_input_event(kernel_request_port, &event) == KERN_FAILURE)
	return (0);

    if (event.e_code == 0x7e) {
	/*
	 * Button change
	 */
	m = 0x80 & event.m_X;
	if (*Button != m) {
	    *Button = m;
	    m = *Ticks - *ButtonTick;
	    *ButtonTick += m;
	    *ButtonTick1 += m;
	    retmask |= INPUT_EV_MSBUT;
	}

	/*
	 * X delta
	 */
	m = event.m_X & 0x7f;
	if (m&0x40)
	    m |= 0x80;
	if (m)
	    retmask |= INPUT_EV_MSMOV;
	*NewCrsrX += (char)m;

	/*
	 * Y delta
	 */
	m = event.m_Y & 0x7f;
	if (m&0x40)
	    m |= 0x80;
	if (m)
	    retmask |= INPUT_EV_MSMOV;
	*NewCrsrY += (char)m;
    }
    else
	switch (event.e_code) {
	  case 0x37:
	    modflg |= KEY_MOD_COMND;
	    *ModFlags = modflg;
	    break;
	    
	  case 0xb7:
	    modflg &= ~KEY_MOD_COMND;
	    *ModFlags = modflg;
	    break;
	    
	  case 0x38:
	    modflg |= KEY_MOD_SHFT;
	    *ModFlags = modflg;
	    break;
	    
	  case 0xb8:
	    modflg &= ~KEY_MOD_SHFT;
	    *ModFlags = modflg;
	    break;
	    
	  case 0x39:
	    modflg |= KEY_MOD_CAPLK;
	    *ModFlags = modflg;
	    break;
	    
	  case 0xb9:
	    modflg &= ~KEY_MOD_CAPLK;
	    *ModFlags = modflg;
	    break;
	    
	  case 0x3a:
	    modflg |= KEY_MOD_OPTN;
	    *ModFlags = modflg;
	    break;
	    
	  case 0xba:
	    modflg &= ~KEY_MOD_OPTN;
	    *ModFlags = modflg;
	    break;
	    
	  case 0x36:
	    modflg |= KEY_MOD_CNTRL;
	    *ModFlags = modflg;
	    break;
	    
	  case 0xb6:
	    modflg &= ~KEY_MOD_CNTRL;
	    *ModFlags = modflg;
	    break;
	    
	  default:
	    newkey = event.e_code;
	    retmask |= INPUT_EV_KEY;
	    break;
	}

    if (retmask&INPUT_EV_MSMOV)
	EnableMouse();
    return (retmask);
}
