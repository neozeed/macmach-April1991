#ifndef lint
static char *RCSid = "$Header: rules.c,v 5.0 89/03/01 01:41:08 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	rules.c,v $
 * Revision 5.0  89/03/01  01:41:08  bww
 * 	Version 5.
 * 	[89/03/01  01:33:21  bww]
 * 
 */

static	char *sccsid = "@(#)rules.c	3.1";

/*
 * DEFAULT RULES FOR UNIX
 *
 * These are the internal rules that "make" trucks around with it at
 * all times. One could completely delete this entire list and just
 * conventionally define a global "include" makefile which had these
 * rules in it. That would make the rules dynamically changeable
 * without recompiling make. This file may be modified to local
 * needs.
 */

#include "defs.h"

char *builtin[] = {
#ifdef pwb
	".SUFFIXES : .L .out .o .c .f .e .r .y .yr .ye .l .s .z .x .t .h .cl",
#else
	".SUFFIXES : .out .o .s .S .c .F .f .e .r .y .yr .ye .l .p .sh .csh .h",
#endif

	/*
	 * PRESET VARIABLES
	 */
	"MAKE=make",
	"AR=ar",
	"ARFLAGS=",
	"RANLIB=ranlib",
	"LD=ld",
	"LDFLAGS=",
	"LINT=lint",
	"LINTFLAGS=",
	"CO=co",
	"COFLAGS=-q",
	"CP=cp",
	"CPFLAGS=",
	"MV=mv",
	"MVFLAGS=",
	"RM=rm",
	"RMFLAGS=-f",
	"YACC=yacc",
	"YACCR=yacc -r",
	"YACCE=yacc -e",
	"YFLAGS=",
	"LEX=lex",
	"LFLAGS=",
	"CC=cc",
	"CFLAGS=",
	"AS=as",
	"ASFLAGS=",
	"PC=pc",
	"PFLAGS=",
	"RC=f77",
	"RFLAGS=",
	"EC=efl",
	"EFLAGS=",
	"FC=f77",
	"FFLAGS=",
	"LOADLIBES=",
#ifdef pwb
	"SCOMP=scomp",
	"SCFLAGS=",
	"CMDICT=cmdict",
	"CMFLAGS=",
#endif
#ifdef pdp11
	"CPU=pdp11",
	"MACHINE=PDP11",
	"machine=pdp11",
	"CPUTYPE=PDP11",
	"cputype=pdp11",
#endif
#ifdef vax
	"CPU=vax",
	"MACHINE=VAX",
	"machine=vax",
	"CPUTYPE=VAX",
	"cputype=vax",
#endif
#ifdef sun
#ifdef sparc
	"CPU=sparc",
	"MACHINE=SUN4",
	"machine=sun4",
	"CPUTYPE=SPARC",
	"cputype=sparc",
#else
#ifdef mc68020
	"CPU=mc68020",
#ifdef CMUCS
	"MACHINE=SUN",  /* SUN3 */
	"machine=sun",  /* sun3 */
#else
	"MACHINE=SUN3",
	"machine=sun3",
#endif
	"CPUTYPE=MC68020",
	"cputype=mc68020",
#else
	"CPU=mc68000",
	"MACHINE=SUN2",
	"machine=sun2",
	"CPUTYPE=MC68000",
	"cputype=mc68000",
#endif
#endif
#endif
#ifdef mac2
	"CPU=mc68000",
	"MACHINE=mac2",
	"machine=mac2",
	"CPUTYPE=mc68020",
	"cputype=mc68020",
#endif
#ifdef ibm032
	"CPU=ibm032",
	"MACHINE=IBMRT",
	"machine=ibmrt",
	"CPUTYPE=IBM032",
	"cputype=ibm032",
#endif
#ifdef ibm370
	"CPU=ibm370",
	"MACHINE=IBM",
	"machine=ibm",
	"CPUTYPE=IBM370",
	"cputype=ibm370",
#endif
#ifdef ns32000
#ifdef balance
	"CPU=ns32032",
	"MACHINE=BALANCE",
	"machine=balance",
	"CPUTYPE=NS32032",
	"cputype=ns32032",
#endif
#ifdef MULTIMAX
	"CPU=ns32032",
	"MACHINE=MMAX",
	"machine=mmax",
	"CPUTYPE=NS32032",
	"cputype=ns32032",
#endif
#endif

	/*
	 * SINGLE SUFFIX RULES
	 */
	".s :",
	"\t$(AS) $(ASFLAGS) -o $@ $<",

	".S :",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) $(ASFLAGS) $< $(LOADLIBES) -o $@",

	".c :",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) $< $(LOADLIBES) -o $@",

	".F .f :",
	"\t$(FC) $(LDFLAGS) $(FFLAGS) $< $(LOADLIBES) -o $@",

	".e :",
	"\t$(EC) $(LDFLAGS) $(EFLAGS) $< $(LOADLIBES) -o $@",

	".r :",
	"\t$(RC) $(LDFLAGS) $(RFLAGS) $< $(LOADLIBES) -o $@",

	".p :",
	"\t$(PC) $(LDFLAGS) $(PFLAGS) $< $(LOADLIBES) -o $@",

	".y :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) y.tab.c $(LOADLIBES) -ly -o $@",
	"\t$(RM) $(RMFLAGS) y.tab.c",

	".l :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) lex.yy.c $(LOADLIBES) -ll -o $@",
	"\t$(RM) $(RMFLAGS) lex.yy.c",

	".sh :",
	"\t$(CP) $(CPFLAGS) $< $@",
	"\tchmod +x $@",

	".csh :",
	"\t$(CP) $(CPFLAGS) $< $@",
	"\tchmod +x $@",

	".CO :",
	"\t$(CO) $(COFLAGS) $< $@",

	".CLEANUP :",
	"\t$(RM) $(RMFLAGS) $?",

	/*
	 * DOUBLE SUFFIX RULES
	 */
	".s.o :",
	"\t$(AS) -o $@ $<",

	".S.o :",
	"\t$(CC) $(CFLAGS) $(ASFLAGS) -c $<",

	".c.o :",
	"\t$(CC) $(CFLAGS) -c $<",

	".F.o .f.o :",
	"\t$(FC) $(FFLAGS) -c $<",

	".e.o :",
	"\t$(EC) $(EFLAGS) -c $<",

	".r.o :",
	"\t$(RC) $(RFLAGS) -c $<",

	".y.o :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c y.tab.c",
	"\t$(RM) $(RMFLAGS) y.tab.c",
	"\t$(MV) $(MVFLAGS) y.tab.o $@",

	".yr.o:",
	"\t$(YACCR) $(YFLAGS) $<",
	"\t$(RC) $(RFLAGS) -c y.tab.r",
	"\t$(RM) $(RMFLAGS) y.tab.r",
	"\t$(MV) $(MVFLAGS) y.tab.o $@",

	".ye.o :",
	"\t$(YACCE) $(YFLAGS) $<",
	"\t$(EC) $(EFLAGS) -c y.tab.e",
	"\t$(RM) $(RMFLAGS) y.tab.e",
	"\t$(MV) $(MVFLAGS) y.tab.o $@",

	".l.o :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c lex.yy.c",
	"\t$(RM) $(RMFLAGS) lex.yy.c",
	"\t$(MV) $(MVFLAGS) lex.yy.o $@",

	".p.o :",
	"\t$(PC) $(PFLAGS) -c $<",

#ifdef pwb
	".cl.o :",
	"\tclass -c $<",
#endif

	".y.c :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(MV) $(MVFLAGS) y.tab.c $@",

	".yr.r:",
	"\t$(YACCR) $(YFLAGS) $<",
	"\t$(MV) $(MVFLAGS) y.tab.r $@",

	".ye.e :",
	"\t$(YACCE) $(YFLAGS) $<",
	"\t$(MV) $(MVFLAGS) y.tab.e $@",

	".l.c :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(MV) $(MVFLAGS) lex.yy.c $@",

#ifdef pwb
	".o.L .c.L .t.L:",
	"\t$(SCOMP) $(SCFLAGS) $<",

	".t.o:",
	"\t$(SCOMP) $(SCFLAGS) -c $<",

	".t.c:",
	"\t$(SCOMP) $(SCFLAGS) -t $<",

	".h.z .t.z:",
	"\t$(CMDICT) $(CMFLAGS) $<",

	".h.x .t.x:",
	"\t$(CMDICT) $(CMFLAGS) -c $<",
#endif

	".o.out .s.out .S.out .c.out :",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) $< $(LOADLIBES) -o $@",

	".F.out .f.out :",
	"\t$(FC) $(LDFLAGS) $(FFLAGS) $< $(LOADLIBES) -o $@",

	".e.out :",
	"\t$(EC) $(LDFLAGS) $(EFLAGS) $< $(LOADLIBES) -o $@",

	".r.out :",
	"\t$(RC) $(LDFLAGS) $(RFLAGS) $< $(LOADLIBES) -o $@",

	".y.out :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) y.tab.c $(LOADLIBES) -ly -o $@",
	"\t$(RM) $(RMFLAGS) y.tab.c",

	".l.out :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(LDFLAGS) $(CFLAGS) lex.yy.c $(LOADLIBES) -ll -o $@",
	"\t$(RM) $(RMFLAGS) lex.yy.c",

	".p.out :",
	"\t$(PC) $(LDFLAGS) $(PFLAGS) $< $(LOADLIBES) -o $@",

	0
};
