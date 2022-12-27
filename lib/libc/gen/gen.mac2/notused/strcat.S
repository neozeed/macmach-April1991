#if defined(LIBC_SCCS)
	.asciz "@(#)strcat.s MacMach 5.1"
	.even
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

	.text

/* strcat(s1, s2) */
ENTRY(strcat)
	link	a6, #0		/* set up stack frame */
	movel	a6@(8), a0	/* s1 */
	movel	a6@(12), a1	/* s2 */
	movel	a0, d2		/* save s1 to return */
	moveq	#-1, d1		/* count to 65536 */
loop1:	tstb	a0@+		/* find end of s1 */
	dbeq	d1, loop1	/* fast loop */
	bne	loop1		/* loop some more */
	subqw	#1, a0		/* backup over the null */
	moveq	#-1, d1		/* count to 65536 */
loop2:	moveb	a1@+, a0@+	/* get byte from s2, append to s1 */
	dbeq	d1, loop2	/* fast loop */
	bne	loop2		/* loop some more */
	movel	d2, d0		/* return s1 */
	unlk	a6		/* done with stack frame */
	rts			/* all done */
