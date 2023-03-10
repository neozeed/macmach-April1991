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
 *
 * All recipients should regard themselves as participants in an ongoing
 * research project and hence should feel obligated to report their
 * experiences (good or bad) with these elementary function codes, using
 * the sendbug(8) program, to the authors.
 */

#ifdef mac2
#include "mac2/log.c"
#else

#ifndef lint
static char sccsid[] = "@(#)log.c	5.3 (Berkeley) 6/30/88";
#endif /* not lint */

/* LOG(X)
 * RETURN THE LOGARITHM OF x 
 * DOUBLE PRECISION (VAX D FORMAT 56 bits or IEEE DOUBLE 53 BITS)
 * CODED IN C BY K.C. NG, 1/19/85;
 * REVISED BY K.C. NG on 2/7/85, 3/7/85, 3/24/85, 4/16/85.
 *
 * Required system supported functions:
 *	scalb(x,n)
 *	copysign(x,y)
 *	logb(x)	
 *	finite(x)
 *
 * Required kernel function:
 *	log__L(z) 
 *
 * Method :
 *	1. Argument Reduction: find k and f such that 
 *			x = 2^k * (1+f), 
 *	   where  sqrt(2)/2 < 1+f < sqrt(2) .
 *
 *	2. Let s = f/(2+f) ; based on log(1+f) = log(1+s) - log(1-s)
 *		 = 2s + 2/3 s**3 + 2/5 s**5 + .....,
 *	   log(1+f) is computed by
 *
 *	     		log(1+f) = 2s + s*log__L(s*s)
 *	   where
 *		log__L(z) = z*(L1 + z*(L2 + z*(... (L6 + z*L7)...)))
 *
 *	   See log__L() for the values of the coefficients.
 *
 *	3. Finally,  log(x) = k*ln2 + log(1+f).  (Here n*ln2 will be stored
 *	   in two floating point number: n*ln2hi + n*ln2lo, n*ln2hi is exact
 *	   since the last 20 bits of ln2hi is 0.)
 *
 * Special cases:
 *	log(x) is NaN with signal if x < 0 (including -INF) ; 
 *	log(+INF) is +INF; log(0) is -INF with signal;
 *	log(NaN) is that NaN with no signal.
 *
 * Accuracy:
 *	log(x) returns the exact log(x) nearly rounded. In a test run with
 *	1,536,000 random arguments on a VAX, the maximum observed error was
 *	.826 ulps (units in the last place).
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following constants.
 * The decimal values may be used, provided that the compiler will convert
 * from decimal to binary accurately enough to produce the hexadecimal values
 * shown.
 */

#if defined(vax)||defined(tahoe)	/* VAX D format */
#include <errno.h>
#ifdef vax
#define _0x(A,B)	0x/**/A/**/B
#else	/* vax */
#define _0x(A,B)	0x/**/B/**/A
#endif	/* vax */
/* static double */
/* ln2hi  =  6.9314718055829871446E-1    , Hex  2^  0   *  .B17217F7D00000 */
/* ln2lo  =  1.6465949582897081279E-12   , Hex  2^-39   *  .E7BCD5E4F1D9CC */
/* sqrt2  =  1.4142135623730950622E0     ; Hex  2^  1   *  .B504F333F9DE65 */
static long     ln2hix[] = { _0x(7217,4031), _0x(0000,f7d0)};
static long     ln2lox[] = { _0x(bcd5,2ce7), _0x(d9cc,e4f1)};
static long     sqrt2x[] = { _0x(04f3,40b5), _0x(de65,33f9)};
#define    ln2hi    (*(double*)ln2hix)
#define    ln2lo    (*(double*)ln2lox)
#define    sqrt2    (*(double*)sqrt2x)
#else	/* defined(vax)||defined(tahoe)	*/
static double
ln2hi  =  6.9314718036912381649E-1    , /*Hex  2^ -1   *  1.62E42FEE00000 */
ln2lo  =  1.9082149292705877000E-10   , /*Hex  2^-33   *  1.A39EF35793C76 */
sqrt2  =  1.4142135623730951455E0     ; /*Hex  2^  0   *  1.6A09E667F3BCD */
#endif	/* defined(vax)||defined(tahoe)	*/

double log(x)
double x;
{
	static double zero=0.0, negone= -1.0, half=1.0/2.0;
	double logb(),scalb(),copysign(),log__L(),s,z,t;
	int k,n,finite();

#if !defined(vax)&&!defined(tahoe)
	if(x!=x) return(x);	/* x is NaN */
#endif	/* !defined(vax)&&!defined(tahoe) */
	if(finite(x)) {
	   if( x > zero ) {

	   /* argument reduction */
	      k=logb(x);   x=scalb(x,-k);
	      if(k == -1022) /* subnormal no. */
		   {n=logb(x); x=scalb(x,-n); k+=n;} 
	      if(x >= sqrt2 ) {k += 1; x *= half;}
	      x += negone ;

	   /* compute log(1+x)  */
              s=x/(2+x); t=x*x*half;
	      z=k*ln2lo+s*(t+log__L(s*s));
	      x += (z - t) ;

	      return(k*ln2hi+x);
	   }
	/* end of if (x > zero) */

	   else {
#if defined(vax)||defined(tahoe)
		extern double infnan();
		if ( x == zero )
		    return (infnan(-ERANGE));	/* -INF */
		else
		    return (infnan(EDOM));	/* NaN */
#else	/* defined(vax)||defined(tahoe) */
		/* zero argument, return -INF with signal */
		if ( x == zero )
		    return( negone/zero );

		/* negative argument, return NaN with signal */
		else 
		    return ( zero / zero );
#endif	/* defined(vax)||defined(tahoe) */
	    }
	}
    /* end of if (finite(x)) */
    /* NOTREACHED if defined(vax)||defined(tahoe) */

    /* log(-INF) is NaN with signal */
	else if (x<0) 
	    return(zero/zero);      

    /* log(+INF) is +INF */
	else return(x);      

}

#endif
