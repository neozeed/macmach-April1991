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
 *	File: mac2emul/macserver_types.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2EMUL_MACSERVER_TYPES_H_
#define _MAC2EMUL_MACSERVER_TYPES_H_

typedef union {
    unsigned char	e_code;
    struct {
	unsigned char	e_code;
	unsigned char	mu_X;
	unsigned char	mu_Y;
    } m_un;
} mac_input_event_t;
#define m_X	m_un.mu_X
#define m_Y	m_un.mu_Y

typedef unsigned char	mac_os_map_t[256 / 8];
typedef unsigned char	mac_tbox_map_t[1024 / 8];

#endif
