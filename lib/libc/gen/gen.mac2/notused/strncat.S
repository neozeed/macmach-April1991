#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "@(#)strncat.S	5.2 (Berkeley) 5/12/90"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * At most n characters are moved.
 * Return s1.
 */

/*
char *
strncat(s1, s2, n)
register char *s1, *s2;
register n;
{
	register char *os1;

	os1 = s1;
	while (*s1++)
		;
	--s1;
	while (*s1++ = *s2++)
		if (--n < 0) {
			*--s1 = '\0';
			break;
		}
	return(os1);
}
*/

ENTRY(strncat)
	link a6,#0
	movel d2,sp@-
	movel a6@(8),a0
	movel a6@(12),a1
	movel a6@(16),d1
	movel a0,d2
L2:
	tstb a0@+
	jne L2
	subqw #1,a0
	jra L4
L7:
	subql #1,d1
	jpl L4
	clrb a0@-
	jra L5
L4:
	moveb a1@+,d0
	moveb d0,a0@+
	jne L7
L5:
	movel d2,d0
	movel a6@(-4),d2
	unlk a6
	rts
