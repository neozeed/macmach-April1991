/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)memcmp.c	5.4 (Berkeley) 6/27/88";
#endif /* LIBC_SCCS and not lint */

memcmp(s1, s2, n)
	register char *s1, *s2;
	register n;
{
#ifdef mac2
	return bcmp(s1, s2, n);
#else
	while (--n >= 0)
		if (*s1++ != *s2++)
			return (*--s1 - *--s2);
	return (0);
#endif
}
