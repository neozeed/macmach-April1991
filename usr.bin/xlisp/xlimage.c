/* -*-C-*-
********************************************************************************
*
* File:         xlimage.c
* RCS:          $Header: xlimage.c,v 1.5 89/11/25 05:30:58 mayer Exp $
* Description:  xlisp memory image save/restore functions
* Author:       David Michael Betz
* Created:      
* Modified:     Sat Nov 25 05:30:50 1989 (Niels Mayer) mayer@hplnpm
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
static char rcs_identity[] = "@(#)$Header: xlimage.c,v 1.5 89/11/25 05:30:58 mayer Exp $";

#include "xlisp.h"

#ifdef SAVERESTORE

/* external variables */
extern LVAL obarray,xlenv,xlfenv,xldenv,s_gchook,s_gcflag;
extern long nnodes,nfree,total;
extern int anodes,nsegs,gccalls;
extern struct segment *segs,*lastseg,*fixseg,*charseg;
extern CONTEXT *xlcontext;
extern LVAL fnodes;

/* local variables */
static OFFTYPE off,foff,doff;
static FILE *fp;

/* external procedures */
extern SEGMENT *newsegment();
extern FILE *osbopen();
extern char *malloc();

/* forward declarations */
OFFTYPE readptr();
OFFTYPE cvoptr();
LVAL cviptr();
LOCAL freeimage();
LOCAL setoffset();
LOCAL writenode();
LOCAL writeptr();
LOCAL readnode();

/* xlisave - save the memory image */
int xlisave(fname)
  char *fname;
{
    char fullname[STRMAX+1];
    unsigned char *cp;
    SEGMENT *seg;
    int n,i,max;
    LVAL p;

    /* default the extension */
    if (needsextension(fname)) {
	strcpy(fullname,fname);
	strcat(fullname,".wks");
	fname = fullname;
    }

    /* open the output file */
    if ((fp = osbopen(fname,"w")) == NULL)
	return (FALSE);

    /* first call the garbage collector to clean up memory */
    gc();

    /* write out the pointer to the *obarray* symbol */
    writeptr(cvoptr(obarray));

    /* setup the initial file offsets */
    off = foff = (OFFTYPE)2;

    /* write out all nodes that are still in use */
    for (seg = segs; seg != NULL; seg = seg->sg_next) {
	p = &seg->sg_nodes[0];
	for (n = seg->sg_size; --n >= 0; ++p, off += 2)
	    switch (ntype(p)) {
	    case FREE:
		break;
	    case CONS:
	    case USTREAM:
		setoffset();
		osbputc(p->n_type,fp);
		writeptr(cvoptr(car(p)));
		writeptr(cvoptr(cdr(p)));
		foff += 2;
		break;
	    default:
		setoffset();
		writenode(p);
		break;
	    }
    }

    /* write the terminator */
    osbputc(FREE,fp);
    writeptr((OFFTYPE)0);

    /* write out data portion of vector-like nodes */
    for (seg = segs; seg != NULL; seg = seg->sg_next) {
	p = &seg->sg_nodes[0];
	for (n = seg->sg_size; --n >= 0; ++p)
	    switch (ntype(p)) {
	    case SYMBOL:
	    case OBJECT:
	    case VECTOR:
	    case CLOSURE:
	    case STRUCT:
#ifdef WINTERP
	    case XLTYPE_TIMEOUTOBJ:
	    case XLTYPE_CALLBACKOBJ:
	    case XLTYPE_EVHANDLEROBJ:
	    case XLTYPE_PIXMAP_REFOBJ:
	    case XLTYPE_WIDGETOBJ:
#endif
		max = getsize(p);
		for (i = 0; i < max; ++i)
		    writeptr(cvoptr(getelement(p,i)));
		break;
	    case STRING:
		max = getslength(p);
		for (cp = getstring(p); --max >= 0; )
		    osbputc(*cp++,fp);
		break;
	    }
    }

    /* close the output file */
    osclose(fp);

    /* return successfully */
    return (TRUE);
}

/* xlirestore - restore a saved memory image */
int xlirestore(fname)
  char *fname;
{
    extern FUNDEF funtab[];
    char fullname[STRMAX+1];
    unsigned char *cp;
    int n,i,max,type;
    SEGMENT *seg;
    LVAL p;

    /* default the extension */
    if (needsextension(fname)) {
	strcpy(fullname,fname);
	strcat(fullname,".wks");
	fname = fullname;
    }

    /* open the file */
    if ((fp = osbopen(fname,"r")) == NULL)
	return (FALSE);

    /* free the old memory image */
    freeimage();

    /* initialize */
    off = (OFFTYPE)2;
    total = nnodes = nfree = 0L;
    fnodes = NIL;
    segs = lastseg = NULL;
    nsegs = gccalls = 0;
    xlenv = xlfenv = xldenv = s_gchook = s_gcflag = NIL;
    xlstack = xlstkbase + EDEPTH;
    xlcontext = NULL;

    /* create the fixnum segment */
    if ((fixseg = newsegment(SFIXSIZE)) == NULL)
	xlfatal("insufficient memory - fixnum segment");

    /* create the character segment */
    if ((charseg = newsegment(CHARSIZE)) == NULL)
	xlfatal("insufficient memory - character segment");

    /* read the pointer to the *obarray* symbol */
    obarray = cviptr(readptr());

    /* read each node */
    while ((type = osbgetc(fp)) >= 0)
	switch (type) {
	case FREE:
	    if ((off = readptr()) == (OFFTYPE)0)
		goto done;
	    break;
	case CONS:
	case USTREAM:
	    p = cviptr(off);
	    p->n_type = type;
	    p->n_flags = 0;
	    rplaca(p,cviptr(readptr()));
	    rplacd(p,cviptr(readptr()));
	    off += 2;
	    break;
	default:
	    readnode(type,cviptr(off));
	    off += 2;
	    break;
	}
done:

    /* read the data portion of vector-like nodes */
    for (seg = segs; seg != NULL; seg = seg->sg_next) {
	p = &seg->sg_nodes[0];
	for (n = seg->sg_size; --n >= 0; ++p)
	    switch (ntype(p)) {
	    case SYMBOL:
	    case OBJECT:
	    case VECTOR:
	    case CLOSURE:
	    case STRUCT:
#ifdef WINTERP
	    case XLTYPE_TIMEOUTOBJ:
	    case XLTYPE_CALLBACKOBJ:
            case XLTYPE_PIXMAP_REFOBJ:
	    case XLTYPE_WIDGETOBJ:
	    case XLTYPE_EVHANDLEROBJ:
#endif
		max = getsize(p);
		if ((p->n_vdata = (LVAL *)malloc(max * sizeof(LVAL))) == NULL)
		    xlfatal("insufficient memory - vector");
		total += (long)(max * sizeof(LVAL));
		for (i = 0; i < max; ++i)
		    setelement(p,i,cviptr(readptr()));
		break;
	    case STRING:
		max = getslength(p);
		if ((p->n_string = (unsigned char *)malloc(max)) == NULL)
		    xlfatal("insufficient memory - string");
		total += (long)max;
		for (cp = getstring(p); --max >= 0; )
		    *cp++ = osbgetc(fp);
		break;
	    case STREAM:
		setfile(p,NULL);
		break;
	    case SUBR:
	    case FSUBR:
		p->n_subr = funtab[getoffset(p)].fd_subr;
		break;
	    }
    }

    /* close the input file */
    osclose(fp);

    /* collect to initialize the free space */
    gc();

    /* lookup all of the symbols the interpreter uses */
    xlsymbols();

    /* return successfully */
    return (TRUE);
}

/* freeimage - free the current memory image */
LOCAL freeimage()
{
    SEGMENT *seg,*next;
    FILE *fp;
    LVAL p;
    int n;

    /* free the data portion of vector-like nodes */
    for (seg = segs; seg != NULL; seg = next) {
	p = &seg->sg_nodes[0];
	for (n = seg->sg_size; --n >= 0; ++p)
	    switch (ntype(p)) {
	    case SYMBOL:
	    case OBJECT:
	    case VECTOR:
	    case CLOSURE:
	    case STRUCT:
#ifdef WINTERP
	    case XLTYPE_TIMEOUTOBJ:
	    case XLTYPE_CALLBACKOBJ:
            case XLTYPE_PIXMAP_REFOBJ:
	    case XLTYPE_WIDGETOBJ:
	    case XLTYPE_EVHANDLEROBJ:
#endif
		if (p->n_vsize)
		    free(p->n_vdata);
		break;
	    case STRING:
		if (getslength(p))
		    free(getstring(p));
		break;
	    case STREAM:
	if ((fp = getfile(p)) && (fp != stdin && fp != stdout && fp != stderr))
		    osclose(getfile(p));
		break;
	    }
	next = seg->sg_next;
	free(seg);
    }
}

/* setoffset - output a positioning command if nodes have been skipped */
LOCAL setoffset()
{
    if (off != foff) {
	osbputc(FREE,fp);
	writeptr(off);
	foff = off;
    }
}

/* writenode - write a node to a file */
LOCAL writenode(node)
  LVAL node;
{
    char *p = (char *)&node->n_info;
    int n = sizeof(union ninfo);
    osbputc(node->n_type,fp);
    while (--n >= 0)
	osbputc(*p++,fp);
    foff += 2;
}

/* writeptr - write a pointer to a file */
LOCAL writeptr(off)
  OFFTYPE off;
{
    char *p = (char *)&off;
    int n = sizeof(OFFTYPE);
    while (--n >= 0)
	osbputc(*p++,fp);
}

/* readnode - read a node */
LOCAL readnode(type,node)
  int type; LVAL node;
{
    char *p = (char *)&node->n_info;
    int n = sizeof(union ninfo);
    node->n_type = type;
    node->n_flags = 0;
    while (--n >= 0)
	*p++ = osbgetc(fp);
}

/* readptr - read a pointer */
LOCAL OFFTYPE readptr()
{
    OFFTYPE off;
    char *p = (char *)&off;
    int n = sizeof(OFFTYPE);
    while (--n >= 0)
	*p++ = osbgetc(fp);
    return (off);
}

/* cviptr - convert a pointer on input */
LOCAL LVAL cviptr(o)
  OFFTYPE o;
{
    OFFTYPE off = (OFFTYPE)2;
    SEGMENT *seg;

    /* check for nil */
    if (o == (OFFTYPE)0)
	return ((LVAL)o);

    /* compute a pointer for this offset */
    for (seg = segs; seg != NULL; seg = seg->sg_next) {
	if (o >= off && o < off + (OFFTYPE)(seg->sg_size << 1))
	    return (seg->sg_nodes + ((int)(o - off) >> 1));
	off += (OFFTYPE)(seg->sg_size << 1);
    }

    /* create new segments if necessary */
    for (;;) {

	/* create the next segment */
	if ((seg = newsegment(anodes)) == NULL)
	    xlfatal("insufficient memory - segment");

	/* check to see if the offset is in this segment */
	if (o >= off && o < off + (OFFTYPE)(seg->sg_size << 1))
	    return (seg->sg_nodes + ((int)(o - off) >> 1));
	off += (OFFTYPE)(seg->sg_size << 1);
    }
}

/* cvoptr - convert a pointer on output */
LOCAL OFFTYPE cvoptr(p)
  LVAL p;
{
    OFFTYPE off = (OFFTYPE)2;
    SEGMENT *seg;

    /* check for nil and small fixnums */
    if (p == NIL)
	return ((OFFTYPE)p);

    /* compute an offset for this pointer */
    for (seg = segs; seg != NULL; seg = seg->sg_next) {
	if (CVPTR(p) >= CVPTR(&seg->sg_nodes[0]) &&
	    CVPTR(p) <  CVPTR(&seg->sg_nodes[0] + seg->sg_size))
	    return (off + (OFFTYPE)((p - seg->sg_nodes) << 1));
	off += (OFFTYPE)(seg->sg_size << 1);
    }

    /* pointer not within any segment */
    xlerror("bad pointer found during image save",p);
}

#endif

