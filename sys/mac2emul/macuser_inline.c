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
 *	File: mac2emul/macuser_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

static inline
fetch_user_n_byte(p, px, n)
register n;
{
    switch (n) {
      case 1:
	return (fetch_user_1_byte(p, px));

      case 2:
	return (fetch_user_2_byte(p, px));

      case 4:
	return (fetch_user_4_byte(p, px));

      default:
	return (copyin(p, px, n));
    }
}

static inline
store_user_n_byte(p, px, n)
register n;
{
    switch (n) {
      case 1:
	return (store_user_1_byte(p, px));

      case 2:
	return (store_user_2_byte(p, px));

      case 4:
	return (store_user_4_byte(p, px));

      default:
	return (copyout(px, p, n));
    }
}
