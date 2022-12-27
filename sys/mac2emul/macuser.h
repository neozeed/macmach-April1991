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
 *	File: mac2emul/macuser.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2emul/macuser_inline.c>

/*
 * Fetch the value in user memory
 * addressed by p having the
 * type t and store it in
 * the variable addressed by px,
 * incrementing p in the process.
 */
#define FETCH_INCR(p, px, t)	\
    fetch_user_n_byte(((t *)(p))++, (px), sizeof (t))

/*
 * Fetch the value in user memory
 * addressed by p having the
 * type t and store it in the variable
 * addressed by px.
 */
#define FETCH(p, px, t)	\
    fetch_user_n_byte((p), (px), sizeof (t))

/*
 * Fetch the value in user memory
 * addressed by p of size n and store
 * it in the variable addressed by px.
 */
#define FETCH_N(p, px, n) \
    fetch_user_n_byte((p), (px), (n));

/*
 * Store the value in the variable
 * addressed by px having the type t
 * in user memory addressed by p.
 */
#define STORE(p, px, t)	\
    store_user_n_byte((p), (px), sizeof (t))

/*
 * Store the value in the variable
 * addressed by px of size n in
 * user memory addressed by p.
 */
#define STORE_N(p, px, n) \
    store_user_n_byte((p), (px), (n))
