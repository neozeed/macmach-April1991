#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)atof.c MacMach 5.1";
#endif LIBC_SCCS and not lint

/*
 *	C library - ascii to floating (atof) and string to double (strtod)
 *
 *	This version compiles both atof and strtod depending on the value
 *	of STRTOD, which is set in the file and may be overridden on the
 *	"cc" command line.  The only difference is the storage of a pointer
 *	to the character which terminated the conversion.
 */
#ifndef STRTOD
#define STRTOD	0
#endif
#include <ctype.h>

extern double ldexp();

/*
 *	The following defines used to set up the length of the array
 *	of powers of 10/8 (1.25).  We use _DEXPLEN of 11, the number of bits
 *	in a double exponent.  Since the number of bits necessary to express
 *	any number as a power of 2 will always be greater than the number
 *	of bits to express the same number as a power of 10, the array
 *	will always be large enough no matter what the binary 
 *	representation.  We will waste a few sizeof(double) entries
 *	at the end of the array, but this should not be significant.
 *	As an example, using IEEE representation, with DEXPLEN=11, the
 *	actual minimum value of POW1_25LEN = 9.  Using 11 instead doesn't
 *	hurt.   Fred Fish, UniSoft Systems Inc., 17-Jan-86.
 *
 *		#if u3b || u3b5
 *		#	define POW1_25LEN	9
 *		#else
 *		#	define POW1_25LEN	6
 *		#endif
 */

#define _DEXPLEN 11
#define POW1_25LEN (_DEXPLEN)	/* See comment above */

static double pow1_25[POW1_25LEN] = { 0.0 };

#define BITSPERBYTE	8
#define BITS(type)	(BITSPERBYTE * (int)sizeof(type))
#define HIBITL	(1L << BITS(long) - 1)
#define MAXLONG	(~HIBITL)
#define _IEEE		0
#define DMAXEXP	((1 << _DEXPLEN - 1) - 1 + _IEEE)

#if STRTOD
#define STORE_PTR	(*ptr = p)
#define GOT_DIGIT	(got_digit++)
#define RET_ZERO(val)	if (!got_digit) return (0.0)

double
strtod(p, ptr)
register char *p;
char **ptr;
#else
#define STORE_PTR
#define GOT_DIGIT
#define RET_ZERO(val)	if (!val) return (0.0)

double
atof(p)
register char *p;
#endif
{
	register int c;
	int exp = 0, neg_val = 0;
	double fl_val;
	double dbltmp;	/* M68881 precision/conversion quirks require storing */
			/* intermediate results */

#if STRTOD
	int got_digit = 0;
	char *dummy;
	if (ptr == (char **)0)
		ptr = &dummy; /* harmless dumping place */
	STORE_PTR;
#endif
	while (isspace(c = *p)) /* eat leading white space */
		p++;
	switch (c) { /* process sign */
	case '-':
		neg_val++;
	case '+': /* fall-through */
		p++;
	}
	{	/* accumulate value */
		register long high = 0, low = 0, scale = 1;
		register int decpt = 0, nzeroes = 0;

		while (isdigit(c = *p++) || c == '.' && !decpt++) {
			if (c == '.')
				continue;
			GOT_DIGIT;
			if (decpt) { /* handle trailing zeroes specially */
				if (c == '0') { /* ignore zero for now */
					nzeroes++;
					continue;
				}
				while (nzeroes > 0) { /* put zeroes back in */
					exp--;
					if (high < MAXLONG/10) {
						high *= 10;
					} else if (scale < MAXLONG/10) {
						scale *= 10;
						low *= 10;
					} else
						exp++;
					nzeroes--;
				}
				exp--; /* decr exponent if decimal pt. seen */
			}
			if (high < MAXLONG/10) {
				high *= 10;
				high += c - '0';
			} else if (scale < MAXLONG/10) {
				scale *= 10;
				low *= 10;
				low += c - '0';
			} else
				exp++;
		}
		RET_ZERO(high);
		fl_val = (double)high;
		if (scale > 1)
			fl_val = (double)scale * fl_val + (double)low;
	}
	STORE_PTR; /* in case there is no legitimate exponent */
	if (c == 'E' || c == 'e') { /* accumulate exponent */
		register int e_exp = 0, neg_exp = 0;

		switch (*p) { /* process sign */
		case '-':
			neg_exp++;
		case '+': /* fall-through */
		case ' ': /* many FORTRAN environments generate this! */
			p++;
		}
		if (isdigit(c = *p)) { /* found a legitimate exponent */
			do {
				/* limit outrageously large exponents */
				if (e_exp < DMAXEXP)
					e_exp = 10 * e_exp + c - '0';
			} while (isdigit(c = *++p));
			if (neg_exp)
				exp -= e_exp;
			else
				exp += e_exp;
			STORE_PTR;
		}
	}
#if STRTOD
	if (!fl_val) /* atof will already have returned, but strtod had */
		return (fl_val); /* to find the end of the exponent first */
#endif
	/*
	 * The following computation is done in two stages,
	 * first accumulating powers of (10/8), then jamming powers of 8,
	 * to avoid underflow in situations like the following (for
	 * the DEC representation): 1.2345678901234567890e-37,
	 * where exp would be about (-37 + -18) = -55, and the
	 * value 10^(-55) can't be represented, but 1.25^(-55) can
	 * be represented, and then 8^(-55) jammed via ldexp().
	 */
	if (exp != 0) { /* apply exponent */
		register double *powptr = pow1_25, fl_exp = fl_val;

		if (*powptr == 0.0) { /* need to initialize table */
			*powptr = 1.25;
			for (; powptr < &pow1_25[POW1_25LEN - 1]; powptr++)
				powptr[1] = *powptr * *powptr;
			powptr = pow1_25;
		}
		if ((c = exp) < 0) {
			c = -c;
			fl_exp = 1.0;
		}
		if (c > (dbltmp = DMAXEXP/2)) /* outrageously large exponents */
			c = DMAXEXP/2; /* will be handled by ldexp */
		for ( ; ; powptr++) {
			/* binary representation of ints assumed; otherwise
			 * replace (& 01) by (% 2) and (>>= 1) by (/= 2) */
			if (c & 01)
				fl_exp *= *powptr;
			if ((c >>= 1) == 0)
				break;
		}
		fl_val = ldexp(exp < 0 ? fl_val/fl_exp : fl_exp, 3 * exp);
	}
	return (neg_val ? -fl_val : fl_val); /* apply sign */
}
