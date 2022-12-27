/*	setjmp.h	4.1	83/05/03	*/
/*  6-Jan-89  Zon Williamson (zon) at Carnegie Mellon University
 *	Added mc68000 version for mac2.
 *	See /usr/src/lib/libc/gen/gen.mac2/setjmp.s for more info.
 */

#ifdef mc68000
typedef int jmp_buf[14]; /* ps, sigmask, d2-d7, a2-a7 */
#else
typedef int jmp_buf[10];
#endif
