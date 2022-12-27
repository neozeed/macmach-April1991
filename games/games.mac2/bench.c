/* benchmark program */

#include <stdio.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>

#ifdef mac2
#ifndef MACH
#define MACH            /* Define to enable task_self() benchmark */
#endif
#undef	NOENUM		/* Define if compiler has no enum's */
#undef	NOSTRUCTASSIGN	/* Define if compiler can't assign structures */
#endif

#define TRUE 1
#define FALSE 0
#define	NULL 0

long counter; /* the loop counter */
int seconds;  /* seconds until alarm signal */

#define HANDLE(NAME, MULT) \
  jmp_buf NAME##_exit; \
  int NAME##_alarm() \
  { \
    printf("%d, %.9f secs/, %.2f /sec\n", \
      counter, \
      ((float)seconds) / (counter * MULT), \
      ((float)(counter * MULT)) / seconds); \
    longjmp(NAME##_exit); \
  }

#define COUNT(NAME) \
  if (setjmp(NAME##_exit)) return; \
  signal(SIGALRM, NAME##_alarm); \
  alarm(seconds); \
  for (counter = 0; ; counter++)

/* example of how to use counter, this will show counter overhead */
HANDLE(count, 1)
count_benchmark()
{
  printf("Count: ");
  fflush(stdout);
  COUNT(count) /* stuff to count */;
}

/* fibonacci, float, sieve, quick and savage benchmarks are
 * from the July 1987 issue of Byte Magazine.
 */

HANDLE(fib, 1)

#define FIB_NUMBER 24

unsigned fib(x)
int x;
{
  if (x > 2) return(fib(x - 1) + fib(x - 2));
  else return(1);
}

fib_benchmark()
{
  volatile unsigned value;

  printf("Fibbonacci(%d): ", FIB_NUMBER);
  fflush(stdout);
  COUNT(fib) {
    value = fib(FIB_NUMBER);
#ifdef POUT
    printf("value = %d\n", value);
#endif
  }
}

HANDLE(float, 14) /* 14 floating point operations (*, /) per loop */

#define FLOAT_CONST1 3.141597E0
#define FLOAT_CONST2 1.7839032E4

float_benchmark()
{
  volatile double a, b, c;

  printf("Float (14 opr): ");
  fflush(stdout);
  a = FLOAT_CONST1;
  b = FLOAT_CONST2;
  COUNT(float) {
    c = c * b; c = c * b; c = c * b; c = c * b;
    c = c * b; c = c * b; c = c * b;
    c = c / b; c = c / b; c = c / b; c = c / b;
    c = c / b; c = c / b; c = c / b;
#ifdef POUT
    printf("c = %f\n", c);
#endif
  }
}

HANDLE(integer, 14) /* 14 integer operations (*, /) per loop */

#define INTEGER_CONST1 31415970
#define INTEGER_CONST2 178390324

integer_benchmark()
{
  volatile long a, b, c;

  printf("Integer (14 opr): ");
  fflush(stdout);
  a = INTEGER_CONST1;
  b = INTEGER_CONST2;
  COUNT(float) {
    c = c * b; c = c * b; c = c * b; c = c * b;
    c = c * b; c = c * b; c = c * b;
    c = c / b; c = c / b; c = c / b; c = c / b;
    c = c / b; c = c / b; c = c / b;
#ifdef POUT
    printf("c = %d\n", c);
#endif
  }
}

HANDLE(sieve, 1)

#define SIEVE_SIZE 8190

char flags[SIEVE_SIZE + 1];

sieve_benchmark()
{
  int i, prime, k, count, iter;

  printf("Seive(%d): ", SIEVE_SIZE);
  fflush(stdout);
  COUNT(sieve) {
    count = 0;
    for (i = 0; i <= SIEVE_SIZE; i++) flags[i] = TRUE;
    for (i = 0; i <= SIEVE_SIZE; i++) {
      if (flags[i]) {
        prime = i + i + 3;
        for (k = i + prime; k <= SIEVE_SIZE; k += prime) flags[k] = FALSE;
        count++;
      }
    }
#ifdef POUT
    printf("%d primes\n", count);
#endif
  }
}

HANDLE(quick, 1)

#define QUICK_MAXNUM 1000
#define QUICK_MODULUS ((long) 0x20000)
#define C 13849L
#define A 25173L

long seed = 7L;

long random(size)
long size;
{
  seed = seed * A + C;
  return (seed % size);
}

long buffer[QUICK_MAXNUM] = {0};

quick(lo, hi, base)
int lo, hi;
long base[];
{
  int i,j;
  long pivot,temp;
  if (lo<hi) {
    for (i=lo, j=hi-1, pivot=base[hi]; i<j;) {
      while (i<hi && base[i] <= pivot) ++i;
      while (j>lo && base[j] >= pivot) --j;
      if (i<j) {
        temp = base[i];
        base[i] = base[j];
        base[j] = temp;
      }
    }
    temp = base[i];
    base[i] = base[hi];
    base[hi] = temp;
    quick(lo, i-1, base);
    quick(i+1, hi, base);
  }
}

quick_benchmark()
{
  int i;
  long temp;

  printf("Quick sort (%d items): ", QUICK_MAXNUM);
  fflush(stdout);
  COUNT(quick) {
    for (i = 0; i < QUICK_MAXNUM; ++i) {
      temp = random(QUICK_MODULUS);
      if (temp < 0L) temp = (- temp);
      buffer[i] = temp;
    }
    quick(0, QUICK_MAXNUM - 1, buffer);
  }
}

HANDLE(savage, 7) /* 7 instructions per loop */

savage_benchmark()
{
  volatile double a;

  printf("Savage (7 opr): ");
  fflush(stdout);
  a = 1.0;
  COUNT(savage) {
    a = tan(atan(exp(log(sqrt(a * a))))) + 1.0;
#ifdef POUT
    printf("counter = %d, a = %f\n", counter, a);
#endif
  }
}

HANDLE(dhrystone, 1)

/*
 *
 *	"DHRYSTONE" Benchmark Program
 *
 *	Version:	C/1.1, 12/01/84
 *
 *	Date:		PROGRAM updated 01/06/86, COMMENTS changed 01/31/87
 *
 *	Author:		Reinhold P. Weicker,  CACM Vol 27, No 10, 10/84 pg. 1013
 *			Translated from ADA by Rick Richardson
 *			Every method to preserve ADA-likeness has been used,
 *			at the expense of C-ness.
 *
 *	Compile:	cc -O dry.c -o drynr			: No registers
 *			cc -O -DREG=register dry.c -o dryr	: Registers
 *
 *	Defines:	Defines are provided for old C compiler's
 *			which don't have enums, and can't assign structures.
 *			The time(2) function is library dependant; Most
 *			return the time in seconds, but beware of some, like
 *			Aztec C, which return other units.
 *			The LOOPS define is initially set for 50000 loops.
 *			If you have a machine with large integers and is
 *			very fast, please change this number to 500000 to
 *			get better accuracy.  Please select the way to
 *			measure the execution time using the TIME define.
 *			For single user machines, time(2) is adequate. For
 *			multi-user machines where you cannot get single-user
 *			access, use the times(2) function.  Be careful to
 *			adjust the HZ parameter below for the units which
 *			are returned by your times(2) function.  You can
 *			sometimes find this in <sys/param.h>.  If you have
 *			neither time(2) nor times(2), use a stopwatch in
 *			the dead of the night.
 *			Use a "printf" at the point marked "start timer"
 *			to begin your timings. DO NOT use the UNIX "time(1)"
 *			command, as this will measure the total time to
 *			run this program, which will (erroneously) include
 *			the time to malloc(3) storage and to compute the
 *			time it takes to do nothing.
 *
 *	Run:		drynr; dryr
 *
 *	Results:	If you get any new machine/OS results, please send to:
 *
 *				ihnp4!castor!pcrat!rick
 *
 *			and thanks to all that do.
 *
 *	Note:		I order the list in increasing performance of the
 *			"with registers" benchmark.  If the compiler doesn't
 *			provide register variables, then the benchmark
 *			is the same for both REG and NOREG.
 *
 *	PLEASE:		Send complete information about the machine type,
 *			clock speed, OS and C manufacturer/version.  If
 *			the machine is modified, tell me what was done.
 *			On UNIX, execute uname -a and cc -V to get this info.
 *
 *	80x8x NOTE:	80x8x benchers: please try to do all memory models
 *			for a particular compiler.
 *
 *
 *	The following program contains statements of a high-level programming
 *	language (C) in a distribution considered representative:
 *
 *	assignments			53%
 *	control statements		32%
 *	procedure, function calls	15%
 *
 *	100 statements are dynamically executed.  The program is balanced with
 *	respect to the three aspects:
 *		- statement type
 *		- operand type (for simple data types)
 *		- operand access
 *			operand global, local, parameter, or constant.
 *
 *	The combination of these three aspects is balanced only approximately.
 *
 *	The program does not compute anything meaningfull, but it is
 *	syntactically and semantically correct.
 *
 */

/* for compatibility with goofed up version */
/*#undef GOOF			/* Define if you want the goofed up version */

#ifdef GOOF
char Version[] = "1.0";
#else
char Version[] = "1.1";
#endif

#ifdef NOSTRUCTASSIGN
#define	structassign(d, s) memcpy(&(d), &(s), sizeof(d))
#else
#define	structassign(d, s) d = s
#endif

#ifdef NOENUM
#define	Ident1 1
#define	Ident2 2
#define	Ident3 3
#define	Ident4 4
#define	Ident5 5
typedef int Enumeration;
#else
typedef enum {Ident1, Ident2, Ident3, Ident4, Ident5} Enumeration;
#endif

typedef int OneToThirty;
typedef int OneToFifty;
typedef char CapitalLetter;
typedef char String30[31];
typedef int Array1Dim[51];
typedef int Array2Dim[51][51];

struct	Record
{
  struct Record *PtrComp;
  Enumeration	Discr;
  Enumeration	EnumComp;
  OneToFifty	IntComp;
  String30	StringComp;
};

typedef struct Record 	RecordType;
typedef RecordType *	RecordPtr;
typedef int		boolean;

#ifndef REG
#define	REG
#endif

extern Enumeration	Func1();
extern boolean		Func2();

/*
 * Package 1
 */
int		IntGlob;
boolean		BoolGlob;
char		Char1Glob;
char		Char2Glob;
Array1Dim	Array1Glob;
Array2Dim	Array2Glob;
RecordPtr	PtrGlb;
RecordPtr	PtrGlbNext;

Proc1(PtrParIn)
REG RecordPtr	PtrParIn;
{
#define	NextRecord	(*(PtrParIn->PtrComp))

  structassign(NextRecord, *PtrGlb);
  PtrParIn->IntComp = 5;
  NextRecord.IntComp = PtrParIn->IntComp;
  NextRecord.PtrComp = PtrParIn->PtrComp;
  Proc3(NextRecord.PtrComp);
  if (NextRecord.Discr == Ident1) {
    NextRecord.IntComp = 6;
    Proc6(PtrParIn->EnumComp, &NextRecord.EnumComp);
    NextRecord.PtrComp = PtrGlb->PtrComp;
    Proc7(NextRecord.IntComp, 10, &NextRecord.IntComp);
  }
  else structassign(*PtrParIn, NextRecord);

#undef	NextRecord
}

Proc2(IntParIO)
OneToFifty *IntParIO;
{
  REG OneToFifty		IntLoc;
  REG Enumeration		EnumLoc;

  IntLoc = *IntParIO + 10;
  for(;;) {
    if (Char1Glob == 'A') {
      --IntLoc;
      *IntParIO = IntLoc - IntGlob;
      EnumLoc = Ident1;
    }
    if (EnumLoc == Ident1) break;
  }
}

Proc3(PtrParOut)
RecordPtr *PtrParOut;
{
  if (PtrGlb != NULL) *PtrParOut = PtrGlb->PtrComp;
  else IntGlob = 100;
  Proc7(10, IntGlob, &PtrGlb->IntComp);
}

Proc4()
{
  REG boolean	BoolLoc;

  BoolLoc = Char1Glob == 'A';
  BoolLoc |= BoolGlob;
  Char2Glob = 'B';
}

Proc5()
{
  Char1Glob = 'A';
  BoolGlob = FALSE;
}

extern boolean Func3();

Proc6(EnumParIn, EnumParOut)
REG Enumeration	EnumParIn;
REG Enumeration	*EnumParOut;
{
  *EnumParOut = EnumParIn;
  if (! Func3(EnumParIn) ) *EnumParOut = Ident4;
  switch (EnumParIn)  {
  case Ident1:	*EnumParOut = Ident1; break;
  case Ident2:	if (IntGlob > 100) *EnumParOut = Ident1;
  		else *EnumParOut = Ident4;
  		break;
  case Ident3:	*EnumParOut = Ident2; break;
  case Ident4:	break;
  case Ident5:	*EnumParOut = Ident3;
  }
}

Proc7(IntParI1, IntParI2, IntParOut)
OneToFifty	IntParI1;
OneToFifty	IntParI2;
OneToFifty	*IntParOut;
{
  REG OneToFifty	IntLoc;

  IntLoc = IntParI1 + 2;
  *IntParOut = IntParI2 + IntLoc;
}

Proc8(Array1Par, Array2Par, IntParI1, IntParI2)
Array1Dim	Array1Par;
Array2Dim	Array2Par;
OneToFifty	IntParI1;
OneToFifty	IntParI2;
{
  REG OneToFifty	IntLoc;
  REG OneToFifty	IntIndex;

  IntLoc = IntParI1 + 5;
  Array1Par[IntLoc] = IntParI2;
  Array1Par[IntLoc+1] = Array1Par[IntLoc];
  Array1Par[IntLoc+30] = IntLoc;
  for (IntIndex = IntLoc; IntIndex <= (IntLoc+1); ++IntIndex)
    Array2Par[IntLoc][IntIndex] = IntLoc;
  ++Array2Par[IntLoc][IntLoc-1];
  Array2Par[IntLoc+20][IntLoc] = Array1Par[IntLoc];
  IntGlob = 5;
}

Enumeration Func1(CharPar1, CharPar2)
CapitalLetter	CharPar1;
CapitalLetter	CharPar2;
{
  REG CapitalLetter	CharLoc1;
  REG CapitalLetter	CharLoc2;

  CharLoc1 = CharPar1;
  CharLoc2 = CharLoc1;
  if (CharLoc2 != CharPar2) return (Ident1);
  else return (Ident2);
}

boolean Func2(StrParI1, StrParI2)
String30	StrParI1;
String30	StrParI2;
{
  REG OneToThirty		IntLoc;
  REG CapitalLetter	CharLoc;

  IntLoc = 1;
  while (IntLoc <= 1)
    if (Func1(StrParI1[IntLoc], StrParI2[IntLoc+1]) == Ident1) {
      CharLoc = 'A';
      ++IntLoc;
    }
  if (CharLoc >= 'W' && CharLoc <= 'Z') IntLoc = 7;
  if (CharLoc == 'X') return(TRUE);
  else {
    if (strcmp(StrParI1, StrParI2) > 0) {
      IntLoc += 7;
      return (TRUE);
    }
    else return (FALSE);
  }
}

boolean Func3(EnumParIn)
REG Enumeration	EnumParIn;
{
  REG Enumeration	EnumLoc;

  EnumLoc = EnumParIn;
  if (EnumLoc == Ident3) return (TRUE);
  return (FALSE);
}

#ifdef	NOSTRUCTASSIGN
memcpy(d, s, l)
register char	*d;
register char	*s;
register int	l;
{
  while (l--) *d++ = *s++;
}
#endif

dhrystone_benchmark()
{
  OneToFifty		IntLoc1;
  REG OneToFifty	IntLoc2;
  OneToFifty		IntLoc3;
  REG char		CharLoc;
  REG char		CharIndex;
  Enumeration	 	EnumLoc;
  String30		String1Loc;
  String30		String2Loc;
  extern char		*malloc();

  printf("Dhrystone %s: ", Version);
  fflush(stdout);
  PtrGlbNext = (RecordPtr) malloc(sizeof(RecordType));
  PtrGlb = (RecordPtr) malloc(sizeof(RecordType));
  PtrGlb->PtrComp = PtrGlbNext;
  PtrGlb->Discr = Ident1;
  PtrGlb->EnumComp = Ident3;
  PtrGlb->IntComp = 40;
  strcpy(PtrGlb->StringComp, "DHRYSTONE PROGRAM, SOME STRING");
#ifndef	GOOF
  strcpy(String1Loc, "DHRYSTONE PROGRAM, 1'ST STRING");	/*GOOF*/
#endif
  Array2Glob[8][7] = 10;	/* Was missing in published program */
  COUNT(dhrystone) {
    Proc5();
    Proc4();
    IntLoc1 = 2;
    IntLoc2 = 3;
    strcpy(String2Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    EnumLoc = Ident2;
    BoolGlob = ! Func2(String1Loc, String2Loc);
    while (IntLoc1 < IntLoc2) {
      IntLoc3 = 5 * IntLoc1 - IntLoc2;
      Proc7(IntLoc1, IntLoc2, &IntLoc3);
      ++IntLoc1;
    }
    Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
    Proc1(PtrGlb);
    for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex)
      if (EnumLoc == Func1(CharIndex, 'C'))
        Proc6(Ident1, &EnumLoc);
    IntLoc3 = IntLoc2 * IntLoc1;
    IntLoc2 = IntLoc3 / IntLoc1;
    IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
    Proc2(&IntLoc1);
  }
}

HANDLE(whetstone, 1)

/* Whetstone benchmark in C.  This program is a translation of the
 * original Algol version in "A Synthetic Benchmark" by H.J. Curnow
 * and B.A. Wichman in Computer Journal, Vol  19 #1, February 1976.
 * Used to test compiler optimization and floating point performance.
 * Compile by:		cc -O -s -o whet whet.c
 * or:			cc -O -DPOUT -s -o whet whet.c
 * if output is desired.
 */

double		x1, x2, x3, x4, x, y, z, t, t1, t2;
double 		e1[4];
int		i, j, k, l, n1, n2, n3, n4, n6, n7, n8, n9, n10, n11;

pa(e)
double e[4];
{
  register int j;

  j = 0;
lab:
  e[0] = (  e[0] + e[1] + e[2] - e[3] ) * t;
  e[1] = (  e[0] + e[1] - e[2] + e[3] ) * t;
  e[2] = (  e[0] - e[1] + e[2] + e[3] ) * t;
  e[3] = ( -e[0] + e[1] + e[2] + e[3] ) / t2;
  j += 1;
  if (j < 6) goto lab;
}

p3(x, y, z)
double x, y, *z;
{
  x  = t * (x + y);
  y  = t * (x + y);
  *z = (x + y) /t2;
}

p0()
{
  e1[j] = e1[k];
  e1[k] = e1[l];
  e1[l] = e1[j];
}

#ifdef POUT
pout(n, j, k, x1, x2, x3, x4)
int n, j, k;
double x1, x2, x3, x4;
{
  printf("%6d%6d%6d  %5 e  %5 e  %5 e  %5 e\n",	n, j, k, x1, x2, x3, x4);
}
#endif

whetstone_benchmark()
{
  printf("Whetstone: ");
  fflush(stdout);
  COUNT(whetstone) {
    /* initialize constants */
    t   =   0.499975;
    t1  =   0.50025;
    t2  =   2.0;
    /* set values of module weights */
    n1  =   0; n2  =  12;
    n3  =  14; n4  = 345;
    n6  = 210; n7  =  32;
    n8  = 899; n9  = 616;
    n10 =   0; n11 =  93;
    /* MODULE 1:  simple identifiers */
    x1 =  1.0;
    x2 = x3 = x4 = -1.0;
    for(i = 1; i <= n1; i += 1) {
      x1 = ( x1 + x2 + x3 - x4) * t;
      x2 = ( x1 + x2 - x3 - x4) * t;
      x3 = ( x1 - x2 + x3 + x4) * t;
      x4 = (-x1 + x2 + x3 + x4) * t;
    }
#ifdef POUT
    pout(n1, n1, n1, x1, x2, x3, x4);
#endif
    /* MODULE 2:  array elements */
    e1[0] =  1.0;
    e1[1] = e1[2] = e1[3] = -1.0;
    for (i = 1; i <= n2; i +=1) {
      e1[0] = ( e1[0] + e1[1] + e1[2] - e1[3]) * t;
      e1[1] = ( e1[0] + e1[1] - e1[2] + e1[3]) * t;
      e1[2] = ( e1[0] - e1[1] + e1[2] + e1[3]) * t;
      e1[3] = (-e1[0] + e1[1] + e1[2] + e1[3]) * t;
    }
#ifdef POUT
    pout(n2, n3, n2, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE 3:  array as parameter */
    for (i = 1; i <= n3; i += 1) pa(e1);
#ifdef POUT
    pout(n3, n2, n2, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE 4:  conditional jumps */
    j = 1;
    for (i = 1; i <= n4; i += 1) {
      if (j == 1) j = 2; else j = 3;
      if (j > 2) j = 0; else j = 1;
      if (j < 1) j = 1; else j = 0;
    }
#ifdef POUT
    pout(n4, j, j, x1, x2, x3, x4);
#endif
    /* MODULE 5:  omitted */
    /* MODULE 6:  integer arithmetic */
    j = 1;
    k = 2;
    l = 3;
    for (i = 1; i <= n6; i += 1) {
      j = j * (k - j) * (l -k);
      k = l * k - (l - j) * k;
      l = (l - k) * (k + j);
      e1[l - 2] = j + k + l;		/* C arrays are zero based */
      e1[k - 2] = j * k * l;
    }
#ifdef POUT
    pout(n6, j, k, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE 7:  trig. functions */
    x = y = 0.5;
    for(i = 1; i <= n7; i +=1) {
      x = t * atan(t2*sin(x)*cos(x)/(cos(x+y)+cos(x-y)-1.0));
      y = t * atan(t2*sin(y)*cos(y)/(cos(x+y)+cos(x-y)-1.0));
    }
#ifdef POUT
    pout(n7, j, k, x, x, y, y);
#endif
    /* MODULE 8:  procedure calls */
    x = y = z = 1.0;
    for (i = 1; i <= n8; i +=1) p3(x, y, &z);
#ifdef POUT
    pout(n8, j, k, x, y, z, z);
#endif
    /* MODULE9:  array references */
    j = 1;
    k = 2;
    l = 3;
    e1[0] = 1.0;
    e1[1] = 2.0;
    e1[2] = 3.0;
    for(i = 1; i <= n9; i += 1) p0();
#ifdef POUT
    pout(n9, j, k, e1[0], e1[1], e1[2], e1[3]);
#endif
    /* MODULE10:  integer arithmetic */
    j = 2;
    k = 3;
    for(i = 1; i <= n10; i +=1) {
      j = j + k; k = j + k;
      j = k - j; k = k - j - j;
    }
#ifdef POUT
    pout(n10, j, k, x1, x2, x3, x4);
#endif
    /* MODULE11:  standard functions */
    x = 0.75;
    for(i = 1; i <= n11; i += 1) x = sqrt(exp(log(x) / t1));
#ifdef POUT
    pout(n11, j, k, x, x, x, x);
#endif
  }
}

HANDLE(getpid, 1)

/* count getpid() calls -- measure unix system call overhead */

getpid_benchmark()
{
  printf("Getpid: ");
  fflush(stdout);
  COUNT(getpid) getpid();
}

HANDLE(task_self, 1)

/* count task_self() calls -- measure mach system call overhead */

task_self_benchmark()
{
#ifndef MACH
  printf("task: *** NOT A MACH SYSTEM ***\n");
#else
  printf("Task: ");
  fflush(stdout);
  COUNT(task_self) task_self();
#endif
}

HANDLE(tak, 1)

/* This is primarily a test of subprogram linkage efficiency,
 * using recursion and a touch of super-simple integer arithmetic.
 * Paul Raveling USC-ISI
 */

int tak (x, y, z)
int x, y, z;
{
  if (x <= y) return z;
  return tak(tak(x - 1, y, z), tak(y - 1, z, x), tak(z - 1, x, y));
}

tak_benchmark()
{
  int i, result;

  printf("Tak: ");
  fflush(stdout);
  COUNT(tak) tak(18, 12, 6);
}

HANDLE(pt, 3) /* 3 context switches per loop */

/* Test kernel speed for context switches induced by interprocess
 * communication of short messages.  This particular test uses
 * pipes for the IPC, and is believed to be sufficiently "plain
 * vanilla" to run on any UNIX system. If the machine is suitably idle,
 * the reported system time (billed to only one of the 3 processes)
 * should be very close to real time / 3, and the CPU utilization
 * percentage should be very close to 33%.
 * Paul Raveling USC-ISI
 */

#define PT_SIZE 10

rpipe(pipefd, process, buffer, size)
int pipefd;
int process;
char *buffer;
int size;
{
  int i;

  i = read(pipefd, buffer, size);
  if (i == -1) {
    printf("Error reading pipe in process %d\n\n", process);
    return;
  }
  if (i < size) {
    printf("ir = %d\n", i);
    rpipe(pipefd, process, &buffer[i], size - i);
  }
}

wpipe(pipefd, process, buffer, size)
int pipefd;
int process;
char *buffer;
int size;
{
  int	i;

  i = write(pipefd, buffer, size);
  if (i == -1) {
    printf("Error writing pipe in process %d\n", process);
    return;
  }
  if (i < size) {
    printf("iw = %d\n", i);
    wpipe(pipefd, process, &buffer[i], size - i);
  }
}

pt_benchmark()
{
  int pipe1[2], pipe2[2], pipe3[2];
  int self, child1, child2;
  int i;
  int size;
  char *msg;

  size = PT_SIZE;
  printf("Pt (3 procs, %d bytes): ", size);
  fflush(stdout);
  if (pipe(pipe1)) { perror("pipe 1\n"); exit(1); }
  if (pipe(pipe2)) { perror("pipe 2\n"); exit(1); }
  if (pipe(pipe3)) { perror("pipe 3\n"); exit(1); }
  if (i = fork()) {
    child1 = i;
    if (i = fork()) {
      child2 = i;
      self = 0;
    }
    else self = 2;
  }
  else self = 1;
  if ((msg = (char *)malloc(size)) == NULL) {
    printf("Can't allocate buffer for process %d\n\n", self);
    exit(1);
  }
  switch (self) {
    case 0:
      for (i=0; i<size; ++i) msg[i] = i + '0';
      wpipe(pipe1[1], self, msg, size);
      rpipe(pipe3[0], self, msg, size);
/*    COUNT(pt) { */
      if (setjmp(pt_exit)) {
        kill(child1, 9);
        kill(child2, 9);
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe3[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        close(pipe3[1]);
      	return;
      }
      signal(SIGALRM, pt_alarm);
      alarm(seconds);
      for (counter = 0; ; counter++) {
        wpipe(pipe1[1], self, msg, size);
        rpipe(pipe3[0], self, msg, size);
      }
    case 1:
      while (1) {
        rpipe(pipe1[0], self, msg, size);
        wpipe(pipe2[1], self, msg, size);
      }
    case 2:
      while (1)	{
        rpipe(pipe2[0], self, msg, size);
        wpipe(pipe3[1], self, msg, size);
      }
  }
}

HANDLE(iocall, 7) /* 7 system I/O calls per loop */

/* This benchmark tests speed of Unix system call interface
 * and speed of cpu doing common Unix io system calls.
 * Paul Raveling USC-ISI
 */

iocall_benchmark()
{
  char buf[512];
  int fd, count, i;

  printf("iocall (7 calls): ");
  fflush(stdout);
  fd = creat("/tmp/testfile", 0777);
  close(fd);
  fd = open("/tmp/testfile", 2);
  unlink("/tmp/testfile");
  COUNT(iocall) {
    lseek(fd, 0L, 0);
    count = write(fd, buf, 500);
    lseek(fd, 0L, 0);
    for (i = 0; i <= 3; i++) count = read(fd, buf, 100);
  }
}

main(argc, argv)
int argc;
char **argv;
{
  if ((argc < 2) || (argc > 3)) {
    printf("usage: bench <time> [ <option> ]\n");
    printf("options: fibonacci, float, sieve, quick, savage,\n");
    printf("         whetstone, dhrystone,\n");
    printf("         getpid, task_self, tak, pt, iocall, count\n");
    printf("Each options is run for the specified number of seconds.\n");
    printf("Run all options if none specified.\n");
    printf("See /usr/games/bench.c for source.\n");
    exit(1);
  }
  seconds = atoi(argv[1]);
  if ((argc == 2) || !strcmp(argv[2], "fibonacci")) fib_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "float")) float_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "integer")) integer_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "sieve")) sieve_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "quick")) quick_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "savage")) savage_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "whetstone")) whetstone_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "dhrystone")) dhrystone_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "getpid")) getpid_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "task_self")) task_self_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "tak")) tak_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "pt")) pt_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "iocall")) iocall_benchmark();
  if ((argc == 2) || !strcmp(argv[2], "count")) count_benchmark();
}
