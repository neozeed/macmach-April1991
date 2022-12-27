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
 *	File: emul/server/input.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"

static int	input_fd = -1;

open_input()
{
    if (input_fd < 0)
	input_fd = open("/dev/kb0", 2);

    if (input_fd < 0)
	unix_error_exit("open keyboard");
}

close_input()
{
    if (input_fd >= 0)
	close(input_fd);

    input_fd = -1;
}
