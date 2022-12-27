#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "@(#)strncpy.S	5.2 (Berkeley) 5/12/90"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

/*
 * Copy s2 to s1, truncating or null-padding to always copy n bytes
 * return s1
 */

/*
char *
strncpy(s1, s2, n)
register char *s1, *s2;
{
	register i;
	register char *os1;

	os1 = s1;
	for (i = 0; i < n; i++)
		if ((*s1++ = *s2++) == '\0') {
			while (++i < n)
				*s1++ = '\0';
			return(os1);
		}
	return(os1);
}
*/

ENTRY(strncpy)
	link a6,#0
	moveml #0x3000,sp@-
	movel a6@(8),a0
	movel a6@(12),a1
	movel a6@(16),d2
	movel a0,d3
	clrl d1
	cmpl d1,d2
	jle L11
L9:
	moveb a1@+,d0
	moveb d0,a0@+
	jne L4
	jra L12
L8:
	clrb a0@+
L12:
	addql #1,d1
	cmpl d1,d2
	jgt L8
	jra L11
L4:
	addql #1,d1
	cmpl d1,d2
	jgt L9
L11:
	movel d3,d0
	moveml a6@(-8),#0xc
	unlk a6
	rts
