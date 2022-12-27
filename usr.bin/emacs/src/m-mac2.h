/* m- file for Apple Macintosh running Mach (for either 68020 or 68030 systems).

   Copyright (C) 1985, 1986 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.  Refer to the GNU Emacs General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute
GNU Emacs, but only under the conditions described in the
GNU Emacs General Public License.   A copy of this license is
supposed to have been given to you along with GNU Emacs so you
can know your rights and responsibilities.  It should be in a
file named COPYING.  Among other things, the copyright notice
and this notice must be preserved on all copies.  */


/* The following three symbols give information on
 the size of various data types.  */
#define SHORTBITS 16		/* Number of bits in a short */
#define INTBITS 32		/* Number of bits in an int */
#define LONGBITS 32		/* Number of bits in a long */

/* 68000 has lowest-numbered byte as most significant */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN
#endif

/* Define how to take a char and sign-extend into an int.
   On machines where char is signed, this is a no-op.  */
#define SIGN_EXTEND_CHAR(c) (c)

/* Say this machine is a 68000 */
#ifndef m68000
#define m68000
#endif

/* Use type int rather than a union, to represent Lisp_Object */
#define NO_UNION_TYPE

/* XINT must explicitly sign-extend */
#define EXPLICIT_SIGN_EXTEND

/* Data type of load average. */
#define LOAD_AVE_TYPE double

/* Convert that into an integer that is 100 for a load average of 1.0  */
#define LOAD_AVE_CVT(x) ((int)x)

/* Must use the system's termcap.  It does special things.  */
#define LIBS_TERMCAP -ltermcap

/* Mask for address bits within a memory segment */
#define SEGMENT_MASK (SEGSIZ - 1)

/* Say that the text segment of a.out includes the header;
   the header actually occupies the first few bytes of the text segment
   and is counted in hdr.a_text.  */
#define A_TEXT_OFFSET(HDR) sizeof (HDR)
