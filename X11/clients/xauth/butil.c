/*
 * This file is used by System V based systems.
 */

void bcopy (b1, b2, length)
register unsigned char *b1, *b2;
register length;
{
    if (b1 < b2) {
	b2 += length;
	b1 += length;
	while (length--) {
	    *--b2 = *--b1;
	}
    }
    else {
	while (length--) {
	    *b2++ = *b1++;
	}
    }
}

int bcmp (b1, b2, length)
register unsigned char *b1, *b2;
register length;
{
    while (length--) {
	if (*b1++ != *b2++) return 1;
    }
    return 0;
}

void bzero (b, length)
register unsigned char *b;
register length;
{
    while (length--) {
	*b++ = '\0';
    }
}

