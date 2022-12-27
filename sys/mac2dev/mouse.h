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
 * $Log:	mouse.h,v $
 * Revision 2.2  90/08/30  11:06:35  bohman
 * 	Created.
 * 	[90/08/29  12:48:18  bohman]
 * 
 */

/*
 *
 *
 *
 *	UniPlus Source Code. This program is proprietary
 *	with Unisoft Corporation and is not to be reproduced
 *	or used in any manner except as authorized in
 *	writing by Unisoft.
 *
 */

extern char mouse_button[];
extern short mouse_x[];
extern short mouse_y[];

#define MOUSE_OPEN	0
#define MOUSE_CHANGE	1

#define MOUSE_OP_DELAY	0
#define MOUSE_OP_MODE	1
#define MOUSE_OP_INTR	2
#define MOUSE_OP_OPEN	3
#define MOUSE_OP_X	4
#define MOUSE_OP_Y	5
#define MOUSE_OP_BUTTON	6

/*
 *	mouse interface structure
 */

struct mouse_data {
	int	(*mouse_open)();
	int	(*mouse_close)();
	long	(*mouse_op)();
};
