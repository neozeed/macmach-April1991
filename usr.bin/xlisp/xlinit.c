/* -*-C-*-
********************************************************************************
*
* File:         xlinit.c
* RCS:          $Header: xlinit.c,v 1.3 89/11/25 05:31:53 mayer Exp $
* Description:  xlisp initialization module
* Author:       David Michael Betz
* Created:      
* Modified:     Sat Nov 25 05:31:30 1989 (Niels Mayer) mayer@hplnpm
* Language:     C
* Package:      N/A
* Status:       X11r4 contrib tape release
*
* WINTERP 1.0 Copyright 1989 Hewlett-Packard Company (by Niels Mayer).
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Hewlett-Packard and David Betz not be
* used in advertising or publicity pertaining to distribution of the software
* without specific, written prior permission.  Hewlett-Packard and David Betz
* make no representations about the suitability of this software for any
* purpose. It is provided "as is" without express or implied warranty.
*
* HEWLETT-PACKARD AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
* IN NO EVENT SHALL HEWLETT-PACKARD NOR DAVID BETZ BE LIABLE FOR ANY SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* See ./winterp/COPYRIGHT for information on contacting the authors.
* 
* Please send modifications, improvements and bugfixes to mayer@hplabs.hp.com
* Post XLISP-specific questions/information to the newsgroup comp.lang.lisp.x
*
********************************************************************************
*/
static char rcs_identity[] = "@(#)$Header: xlinit.c,v 1.3 89/11/25 05:31:53 mayer Exp $";

#include "xlisp.h"

/* external variables */
extern LVAL true,s_dot,s_unbound;
extern LVAL s_quote,s_function,s_bquote,s_comma,s_comat;
extern LVAL s_lambda,s_macro;
extern LVAL s_stdin,s_stdout,s_stderr,s_debugio,s_traceout;
extern LVAL s_evalhook,s_applyhook,s_tracelist;
extern LVAL s_tracenable,s_tlimit,s_breakenable;
extern LVAL s_setf,s_car,s_cdr,s_nth,s_aref,s_get,s_eql;
extern LVAL s_svalue,s_sfunction,s_splist;
extern LVAL s_rtable,k_wspace,k_const,k_nmacro,k_tmacro;
extern LVAL k_sescape,k_mescape;
extern LVAL s_ifmt,s_ffmt,s_printcase;
extern LVAL s_1plus,s_2plus,s_3plus,s_1star,s_2star,s_3star,s_minus;
extern LVAL k_test,k_tnot;
extern LVAL k_direction,k_input,k_output;
extern LVAL k_start,k_end,k_1start,k_1end,k_2start,k_2end;
extern LVAL k_verbose,k_print,k_count,k_key,k_upcase,k_downcase;
extern LVAL lk_optional,lk_rest,lk_key,lk_aux,lk_allow_other_keys;
extern LVAL a_subr,a_fsubr,a_cons,a_symbol;
extern LVAL a_fixnum,a_flonum,a_string,a_stream,a_object;
extern LVAL a_vector,a_closure,a_char,a_ustream;
extern LVAL s_gcflag,s_gchook;
extern FUNDEF funtab[];

#ifdef WINTERP
extern LVAL a_XtAccelerators, a_XtTranslations, a_XtCallbackList, a_XEvent,
  a_Window, a_Pixel, a_Pixmap, a_XImage, a_XmString, a_XmFontList,
  a_caddr_t, a_XT_RESOURCE, a_CALLBACKOBJ, a_TIMEOUTOBJ, a_PIXMAP_REFOBJ,
  a_WIDGETOBJ, a_EVHANDLEROBJ;
#endif

/* forward declarations */
LOCAL initwks();

/* xlinit - xlisp initialization routine */
xlinit()
{
    /* initialize xlisp (must be in this order) */
    xlminit();	/* initialize xldmem.c */
    xldinit();	/* initialize xldbug.c */

    /* finish initializing */
#ifdef SAVERESTORE
    if (!xlirestore("xlisp.wks"))
#endif
	initwks();
}

/* initwks - build an initial workspace */
LOCAL initwks()
{
    FUNDEF *p;
    int i;
    
    xlsinit();	/* initialize xlsym.c */
    xlsymbols();/* enter all symbols used by the interpreter */
    xlrinit();	/* initialize xlread.c */
    xloinit();	/* initialize xlobj.c */

    /* setup defaults */
    setvalue(s_evalhook,NIL);		/* no evalhook function */
    setvalue(s_applyhook,NIL);		/* no applyhook function */
    setvalue(s_tracelist,NIL);		/* no functions being traced */
    setvalue(s_tracenable,NIL);		/* traceback disabled */
    setvalue(s_tlimit,NIL); 		/* trace limit infinite */
    setvalue(s_breakenable,NIL);	/* don't enter break loop on errors */
    setvalue(s_gcflag,NIL);		/* don't show gc information */
    setvalue(s_gchook,NIL);		/* no gc hook active */
    setvalue(s_ifmt,cvstring(IFMT));	/* integer print format */
    setvalue(s_ffmt,cvstring("%g"));	/* float print format */
    setvalue(s_printcase,k_upcase);	/* upper case output of symbols */

    /* install the built-in functions and special forms */
    for (i = 0, p = funtab; p->fd_subr != NULL; ++i, ++p)
	if (p->fd_name)
	    xlsubr(p->fd_name,p->fd_type,p->fd_subr,i);

    /* add some synonyms */
    setfunction(xlenter("NOT"),getfunction(xlenter("NULL")));
    setfunction(xlenter("FIRST"),getfunction(xlenter("CAR")));
    setfunction(xlenter("SECOND"),getfunction(xlenter("CADR")));
    setfunction(xlenter("THIRD"),getfunction(xlenter("CADDR")));
    setfunction(xlenter("FOURTH"),getfunction(xlenter("CADDDR")));
    setfunction(xlenter("REST"),getfunction(xlenter("CDR")));
}

/* xlsymbols - enter all of the symbols used by the interpreter */
xlsymbols()
{
    LVAL sym;

    /* enter the unbound variable indicator (must be first) */
    s_unbound = xlenter("*UNBOUND*");
    setvalue(s_unbound,s_unbound);

    /* enter the 't' symbol */
    true = xlenter("T");
    setvalue(true,true);

    /* enter some important symbols */
    s_dot	= xlenter(".");
    s_quote	= xlenter("QUOTE");
    s_function	= xlenter("FUNCTION");
    s_bquote	= xlenter("BACKQUOTE");
    s_comma	= xlenter("COMMA");
    s_comat	= xlenter("COMMA-AT");
    s_lambda	= xlenter("LAMBDA");
    s_macro	= xlenter("MACRO");
    s_eql	= xlenter("EQL");
    s_ifmt	= xlenter("*INTEGER-FORMAT*");
    s_ffmt	= xlenter("*FLOAT-FORMAT*");

    /* symbols set by the read-eval-print loop */
    s_1plus	= xlenter("+");
    s_2plus	= xlenter("++");
    s_3plus	= xlenter("+++");
    s_1star	= xlenter("*");
    s_2star	= xlenter("**");
    s_3star	= xlenter("***");
    s_minus	= xlenter("-");

    /* enter setf place specifiers */
    s_setf	= xlenter("*SETF*");
    s_car	= xlenter("CAR");
    s_cdr	= xlenter("CDR");
    s_nth	= xlenter("NTH");
    s_aref	= xlenter("AREF");
    s_get	= xlenter("GET");
    s_svalue	= xlenter("SYMBOL-VALUE");
    s_sfunction	= xlenter("SYMBOL-FUNCTION");
    s_splist	= xlenter("SYMBOL-PLIST");

    /* enter the readtable variable and keywords */
    s_rtable	= xlenter("*READTABLE*");
    k_wspace	= xlenter(":WHITE-SPACE");
    k_const	= xlenter(":CONSTITUENT");
    k_nmacro	= xlenter(":NMACRO");
    k_tmacro	= xlenter(":TMACRO");
    k_sescape	= xlenter(":SESCAPE");
    k_mescape	= xlenter(":MESCAPE");

    /* enter parameter list keywords */
    k_test	= xlenter(":TEST");
    k_tnot	= xlenter(":TEST-NOT");

    /* "open" keywords */
    k_direction = xlenter(":DIRECTION");
    k_input     = xlenter(":INPUT");
    k_output    = xlenter(":OUTPUT");

    /* enter *print-case* symbol and keywords */
    s_printcase = xlenter("*PRINT-CASE*");
    k_upcase	= xlenter(":UPCASE");
    k_downcase  = xlenter(":DOWNCASE");

    /* other keywords */
    k_start	= xlenter(":START");
    k_end	= xlenter(":END");
    k_1start	= xlenter(":START1");
    k_1end	= xlenter(":END1");
    k_2start	= xlenter(":START2");
    k_2end	= xlenter(":END2");
    k_verbose	= xlenter(":VERBOSE");
    k_print	= xlenter(":PRINT");
    k_count	= xlenter(":COUNT");
    k_key	= xlenter(":KEY");

    /* enter lambda list keywords */
    lk_optional	= xlenter("&OPTIONAL");
    lk_rest	= xlenter("&REST");
    lk_key	= xlenter("&KEY");
    lk_aux	= xlenter("&AUX");
    lk_allow_other_keys = xlenter("&ALLOW-OTHER-KEYS");

    /* enter *standard-input*, *standard-output* and *error-output* */
    s_stdin = xlenter("*STANDARD-INPUT*");
    setvalue(s_stdin,cvfile(stdin));
    s_stdout = xlenter("*STANDARD-OUTPUT*");
    setvalue(s_stdout,cvfile(stdout));
    s_stderr = xlenter("*ERROR-OUTPUT*");
    setvalue(s_stderr,cvfile(stderr));

    /* enter *debug-io* and *trace-output* */
    s_debugio = xlenter("*DEBUG-IO*");
    setvalue(s_debugio,getvalue(s_stderr));
    s_traceout = xlenter("*TRACE-OUTPUT*");
    setvalue(s_traceout,getvalue(s_stderr));

    /* enter the eval and apply hook variables */
    s_evalhook = xlenter("*EVALHOOK*");
    s_applyhook = xlenter("*APPLYHOOK*");

    /* enter the symbol pointing to the list of functions being traced */
    s_tracelist = xlenter("*TRACELIST*");

    /* enter the error traceback and the error break enable flags */
    s_tracenable = xlenter("*TRACENABLE*");
    s_tlimit = xlenter("*TRACELIMIT*");
    s_breakenable = xlenter("*BREAKENABLE*");

    /* enter a symbol to control printing of garbage collection messages */
    s_gcflag = xlenter("*GC-FLAG*");
    s_gchook = xlenter("*GC-HOOK*");

    /* enter a copyright notice into the oblist */
#ifdef WINTERP
    sym = xlenter("**Copyright-1989-by-David-Betz-and-Niels-Mayer**");
#else
    sym = xlenter("**Copyright-1989-by-David-Betz**");
#endif
    setvalue(sym,true);

    /* enter type names */
    a_subr	= xlenter("SUBR");
    a_fsubr	= xlenter("FSUBR");
    a_cons	= xlenter("CONS");
    a_symbol	= xlenter("SYMBOL");
    a_fixnum	= xlenter("FIXNUM");
    a_flonum	= xlenter("FLONUM");
    a_string	= xlenter("STRING");
    a_object	= xlenter("OBJECT");
    a_stream	= xlenter("FILE-STREAM");
    a_vector	= xlenter("ARRAY");
    a_closure	= xlenter("CLOSURE");
    a_char      = xlenter("CHARACTER");
    a_ustream	= xlenter("UNNAMED-STREAM");

#ifdef WINTERP
    a_XtAccelerators = xlenter("XT_ACCELERATORS");
    a_XtTranslations = xlenter("XT_TRANSLATIONS");
    a_XtCallbackList = xlenter("XT_CALLBACKLIST");
    a_XEvent = xlenter("XEVENT");
    a_Window = xlenter("WINDOW");
    a_Pixel = xlenter("PIXEL");
    a_Pixmap = xlenter("PIXMAP");
    a_XImage = xlenter("XIMAGE");
    a_XmString = xlenter("XM_STRING");
    a_XmFontList = xlenter("XM_FONT_LIST");
    a_caddr_t = xlenter("CADDR_T");
    a_XT_RESOURCE = xlenter("XT_RESOURCE");
    a_CALLBACKOBJ = xlenter("CALLBACK_OBJ");
    a_TIMEOUTOBJ = xlenter("TIMEOUT_OBJ");
    a_PIXMAP_REFOBJ = xlenter("PIXMAP_REFOBJ");
    a_WIDGETOBJ = xlenter("WIDGET_OBJ");
    a_EVHANDLEROBJ = xlenter("EVHANDLER_OBJ");
#endif

    /* add the object-oriented programming symbols and os specific stuff */
    obsymbols();	/* object-oriented programming symbols */
    ossymbols();	/* os specific symbols */
}

