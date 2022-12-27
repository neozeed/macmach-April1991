/* -*-C-*-
********************************************************************************
*
* File:         xldmem.c
* RCS:          $Header: xldmem.c,v 1.6 89/11/25 05:18:06 mayer Exp $
* Description:  xlisp dynamic memory management routines.
* Author:       David Michael Betz; Niels Mayer
* Created:      
* Modified:     Sat Nov 25 05:17:34 1989 (Niels Mayer) mayer@hplnpm
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
static char rcs_identity[] = "@(#)$Header: xldmem.c,v 1.6 89/11/25 05:18:06 mayer Exp $";

#include "xlisp.h"

/* node flags */
#define MARK	1
#define LEFT	2

/* macro to compute the size of a segment */
#define segsize(n) (sizeof(SEGMENT)+((n)-1)*sizeof(struct node))

/* external variables */
extern LVAL obarray,s_gcflag,s_gchook,s_unbound,true;
extern LVAL xlenv,xlfenv,xldenv;
extern char buf[];

/* variables local to xldmem.c and xlimage.c */
SEGMENT *segs,*lastseg,*fixseg,*charseg;
int anodes,nsegs,gccalls;
long nnodes,nfree,total;
LVAL fnodes;

/* external procedures */
extern char *malloc();
extern char *calloc();

/* forward declarations */
FORWARD LVAL newnode();
FORWARD unsigned char *stralloc();
FORWARD SEGMENT *newsegment();
LOCAL findmem();
LOCAL mark();
LOCAL sweep();
LOCAL int addseg();

#ifdef WINTERP

extern LVAL v_savedobjs;
extern void Wxms_Garbage_Collect_XmString(); /* w_XmString.c */
extern void Wpm_Decr_Refcount_Or_Free_Pixmap();	/* w_pixmap.c */
extern void Wcls_Garbage_Collect_WIDGETOBJ(); /* w_classes.c */

/* cv_xtresource - convert a xtresource instance to an XLTYPE_XT_RESOURCE node */
LVAL cv_xtresource(res)
     struct _Resource_Instance *res;
{
  LVAL val;
  val = newnode(XLTYPE_XT_RESOURCE);
  val->n_xtresource = res;
  return (val);
}

/* cv_pixel - convert a Pixel to an XLTYPE_Pixel node */
LVAL cv_pixel(pixel)
     Pixel pixel;
{
  LVAL val;
  val = newnode(XLTYPE_Pixel);
  val->n_pixel = pixel;
  return (val);
}

/* cv_pixmap - allocate and initialize a new XLTYPE_Pixmap node */
LVAL cv_pixmap(pixmap)
     Pixmap pixmap;
{
  LVAL val;
  val = newnode(XLTYPE_Pixmap);
  val->n_pixmap = pixmap;
  return (val);
}

/* cv_ximage -- allocate and initialize a new XLTYPE_XImage node */
LVAL cv_ximage(ximage)
     XImage *ximage;
{
  LVAL val;
  val = newnode(XLTYPE_XImage);
  val->n_ximage = ximage;
  return (val);
}

/* cv_xmstring -- allocate and initialize a new XLTYPE_XmString node */
LVAL cv_xmstring(xmstr)
     XmString xmstr;
{
  LVAL val;
  val = newnode(XLTYPE_XmString);
  val->n_xmstring = xmstr;
  return (val);
}

/* cv_xevent_ptr -- allocate and initialize a new XLTYPE_XEvent node */
LVAL cv_xevent(xevp)
     XEvent *xevp;
{
  LVAL val;
  val = newnode(XLTYPE_XEvent);
  val->n_xevent = xevp;
  return (val);
}

/* cv_window -- allocate and initialize a new XLTYPE_Window node */
LVAL cv_window(win)
     Window win;
{
  LVAL val;
  val = newnode(XLTYPE_Window);
  val->n_window = win;
  return (val);
}

/* cv_xtaccelerators -- allocate and initialize a new XLTYPE_XtAccelerators node */
LVAL cv_xtaccelerators(axl)
     XtAccelerators axl;
{
  LVAL val;
  val = newnode(XLTYPE_XtAccelerators);
  val->n_xtaccelerators = axl;
  return (val);
}

/* cv_xttranslations -- allocate and initialize a new XLTYPE_XtTranslations node */
LVAL cv_xttranslations(txl)
     XtTranslations txl;
{
  LVAL val;
  val = newnode(XLTYPE_XtTranslations);
  val->n_xttranslations = txl;
  return (val);
}

/* new_pixrefobj() -- allocate and initialize a new XLTYPE_PIXMAP_REFOBJ */
LVAL new_pixrefobj()
{
  LVAL val;
  val = newvector(PIXMAP_REFOBJ_SIZE);
  val->n_type = XLTYPE_PIXMAP_REFOBJ;
  return (val);
}

/* new_callbackobj() -- allocate and initialize a new XLTYPE_CALLBACKOBJ */
LVAL new_callbackobj()
{
  LVAL val;
  val = newvector(CALLBACKOBJ_SIZE);
  val->n_type = XLTYPE_CALLBACKOBJ;
  return (val);
}

/* new_timeoutobj() -- allocate and initialize a new XLTYPE_TIMEOUTOBJ */
LVAL new_timeoutobj()
{
  LVAL val;
  val = newvector(TIMEOUTOBJ_SIZE);
  val->n_type = XLTYPE_TIMEOUTOBJ;
  return (val);
}

/* new_evhandlerobj() -- allocate and initialize a new XLTYPE_EVHANDLEROBJ */
LVAL new_evhandlerobj()
{
  LVAL val;
  val = newvector(EVHANDLEROBJ_SIZE);
  val->n_type = XLTYPE_EVHANDLEROBJ;
  return (val);
}

#endif				/* WINTERP */

/* cons - construct a new cons node */
LVAL cons(x,y)
  LVAL x,y;
{
    LVAL nnode;

    /* get a free node */
    if ((nnode = fnodes) == NIL) {
	xlstkcheck(2);
	xlprotect(x);
	xlprotect(y);
	findmem();
	if ((nnode = fnodes) == NIL)
	    xlabort("insufficient node space");
	xlpop();
	xlpop();
    }

    /* unlink the node from the free list */
    fnodes = cdr(nnode);
    --nfree;

    /* initialize the new node */
    nnode->n_type = CONS;
    rplaca(nnode,x);
    rplacd(nnode,y);

    /* return the new node */
    return (nnode);
}

/* cvstring - convert a string to a string node */
LVAL cvstring(str)
  char *str;
{
    LVAL val;
    xlsave1(val);
    val = newnode(STRING);
    val->n_strlen = strlen(str) + 1;
    val->n_string = stralloc(getslength(val));
    strcpy(getstring(val),str);
    xlpop();
    return (val);
}

/* newstring - allocate and initialize a new string */
LVAL newstring(size)
  int size;
{
    LVAL val;
    xlsave1(val);
    val = newnode(STRING);
    val->n_strlen = size;
    val->n_string = stralloc(getslength(val));
    strcpy(getstring(val),"");
    xlpop();
    return (val);
}

/* cvsymbol - convert a string to a symbol */
LVAL cvsymbol(pname)
  char *pname;
{
    LVAL val;
    xlsave1(val);
    val = newvector(SYMSIZE);
    val->n_type = SYMBOL;
    setvalue(val,s_unbound);
    setfunction(val,s_unbound);
    setpname(val,cvstring(pname));
    xlpop();
    return (val);
}

/* cvsubr - convert a function to a subr or fsubr */
LVAL cvsubr(fcn,type,offset)
  LVAL (*fcn)(); int type,offset;
{
    LVAL val;
    val = newnode(type);
    val->n_subr = fcn;
    val->n_offset = offset;
    return (val);
}

/* cvfile - convert a file pointer to a stream */
LVAL cvfile(fp)
  FILE *fp;
{
    LVAL val;
    val = newnode(STREAM);
    setfile(val,fp);
    setsavech(val,'\0');
    return (val);
}

/* cvfixnum - convert an integer to a fixnum node */
LVAL cvfixnum(n)
  FIXTYPE n;
{
    LVAL val;
    if (n >= SFIXMIN && n <= SFIXMAX)
	return (&fixseg->sg_nodes[(int)n-SFIXMIN]);
    val = newnode(FIXNUM);
    val->n_fixnum = n;
    return (val);
}

/* cvflonum - convert a floating point number to a flonum node */
LVAL cvflonum(n)
  FLOTYPE n;
{
    LVAL val;
    val = newnode(FLONUM);
    val->n_flonum = n;
    return (val);
}

/* cvchar - convert an integer to a character node */
LVAL cvchar(n)
  int n;
{
    if (n >= CHARMIN && n <= CHARMAX)
	return (&charseg->sg_nodes[n-CHARMIN]);
    xlerror("character code out of range",cvfixnum((FIXTYPE)n));
}

/* newustream - create a new unnamed stream */
LVAL newustream()
{
    LVAL val;
    val = newnode(USTREAM);
    sethead(val,NIL);
    settail(val,NIL);
    return (val);
}

/* newobject - allocate and initialize a new object */
LVAL newobject(cls,size)
  LVAL cls; int size;
{
    LVAL val;
    val = newvector(size+1);
    val->n_type = OBJECT;
    setelement(val,0,cls);
    return (val);
}

/* newclosure - allocate and initialize a new closure */
LVAL newclosure(name,type,env,fenv)
  LVAL name,type,env,fenv;
{
    LVAL val;
    val = newvector(CLOSIZE);
    val->n_type = CLOSURE;
    setname(val,name);
    settype(val,type);
    setenv(val,env);
    setfenv(val,fenv);
    return (val);
}

/* newstruct - allocate and initialize a new structure node */
LVAL newstruct(type,size)
  LVAL type; int size;
{
    LVAL val;
    val = newvector(size+1);
    val->n_type = STRUCT;
    setelement(val,0,type);
    return (val);
}

/* newvector - allocate and initialize a new vector node */
LVAL newvector(size)
  int size;
{
    LVAL vect;
    int bsize;
    xlsave1(vect);
    vect = newnode(VECTOR);
    vect->n_vsize = 0;
    if (bsize = size * sizeof(LVAL)) {
	if ((vect->n_vdata = (LVAL *)calloc(1,bsize)) == NULL) {
	    findmem();
	    if ((vect->n_vdata = (LVAL *)calloc(1,bsize)) == NULL)
		xlfail("insufficient vector space");
	}
	vect->n_vsize = size;
	total += (long) bsize;
    }
    xlpop();
    return (vect);
}

/* newnode - allocate a new node */
LOCAL LVAL newnode(type)
  int type;
{
    LVAL nnode;

    /* get a free node */
    if ((nnode = fnodes) == NIL) {
	findmem();
	if ((nnode = fnodes) == NIL)
	    xlabort("insufficient node space");
    }

    /* unlink the node from the free list */
    fnodes = cdr(nnode);
    nfree -= 1L;

    /* initialize the new node */
    nnode->n_type = type;
    rplacd(nnode,NIL);

    /* return the new node */
    return (nnode);
}

/* stralloc - allocate memory for a string adding a byte for the terminator */
LOCAL unsigned char *stralloc(size)
  int size;
{
    unsigned char *sptr;

    /* allocate memory for the string copy */
    if ((sptr = (unsigned char *)malloc(size)) == NULL) {
	gc();  
	if ((sptr = (unsigned char *)malloc(size)) == NULL)
	    xlfail("insufficient string space");
    }
    total += (long)size;

    /* return the new string memory */
    return (sptr);
}

/* findmem - find more memory by collecting then expanding */
LOCAL findmem()
{
    gc();
    if (nfree < (long)anodes)
	addseg();
}

/* gc - garbage collect (only called here and in xlimage.c) */
gc()
{
    register LVAL **p,*ap,tmp;
    char buf[STRMAX+1];
    LVAL *newfp,fun;

    /* print the start of the gc message */
    if (s_gcflag && getvalue(s_gcflag)) {
	sprintf(buf,"[ gc: total %ld, ",nnodes);
	stdputstr(buf);
    }

#ifdef WINTERP
    /* mark the callback-obj's, timeout-obj's, etc that are "referenced"
       inside Motif/Xtoolkit implementation. */
    if (v_savedobjs)
        mark(v_savedobjs);
#endif

    /* mark the obarray, the argument list and the current environment */
    if (obarray)
	mark(obarray);
    if (xlenv)
	mark(xlenv);
    if (xlfenv)
	mark(xlfenv);
    if (xldenv)
	mark(xldenv);

    /* mark the evaluation stack */
    for (p = xlstack; p < xlstktop; ++p)
	if (tmp = **p)
	    mark(tmp);

    /* mark the argument stack */
    for (ap = xlargstkbase; ap < xlsp; ++ap)
	if (tmp = *ap)
	    mark(tmp);

    /* sweep memory collecting all unmarked nodes */
    sweep();

    /* count the gc call */
    ++gccalls;

    /* call the *gc-hook* if necessary */
    if (s_gchook && (fun = getvalue(s_gchook))) {
	newfp = xlsp;
	pusharg(cvfixnum((FIXTYPE)(newfp - xlfp)));
	pusharg(fun);
	pusharg(cvfixnum((FIXTYPE)2));
	pusharg(cvfixnum((FIXTYPE)nnodes));
	pusharg(cvfixnum((FIXTYPE)nfree));
	xlfp = newfp;
	xlapply(2);
    }

    /* print the end of the gc message */
    if (s_gcflag && getvalue(s_gcflag)) {
	sprintf(buf,"%ld free ]\n",nfree);
	stdputstr(buf);
    }
}

/* mark - mark all accessible nodes */
LOCAL mark(ptr)
  LVAL ptr;
{
    register LVAL this,prev,tmp;
    int type,i,n;

    /* initialize */
    prev = NIL;
    this = ptr;

    /* mark this list */
    for (;;) {

	/* descend as far as we can */
	while (!(this->n_flags & MARK))

	    /* check cons and unnamed stream nodes */
	    if ((type = ntype(this)) == CONS || type == USTREAM) {
		if (tmp = car(this)) {
		    this->n_flags |= MARK|LEFT;
		    rplaca(this,prev);
		}
		else if (tmp = cdr(this)) {
		    this->n_flags |= MARK;
		    rplacd(this,prev);
		}
		else {				/* both sides nil */
		    this->n_flags |= MARK;
		    break;
		}
		prev = this;			/* step down the branch */
		this = tmp;
	    }

	    /* mark other node types */
	    else {
		this->n_flags |= MARK;
		switch (type) {
#ifdef WINTERP
		case XLTYPE_WIDGETOBJ:
		  /* 
		   * An XLTYPE_WIDGETOBJ is just like a OBJECT node with slot 0
		   * being the class, and the other slots being instance
		   * variables. class WIDGET_CLASS defines a special instance
		   * variable at slot 1 holding the WidgetID. Since that slot
		   * isn't an LVAL, it should not be mark()'d. Any additional
		   * slots means that the WIDGETOBJ was subclassed and new
		   * instance variables were added in the subclass which need to
		   * be marked.
		   */
		  if (tmp = getelement(this, 0))
		    mark(tmp);
		  /* HACK: skip element 1 since it's special, start at 2 */
		  for (i = 2, n = getsize(this) - 2; --n >= 0; ++i)
		    if ((tmp = getelement(this,i)))
		      mark(tmp);
		  break;
		case XLTYPE_TIMEOUTOBJ:
		case XLTYPE_CALLBACKOBJ:
		case XLTYPE_PIXMAP_REFOBJ:
		case XLTYPE_EVHANDLEROBJ:
#endif
		case SYMBOL:
		case OBJECT:
		case VECTOR:
		case CLOSURE:
		case STRUCT:
		    for (i = 0, n = getsize(this); --n >= 0; ++i)
			if (tmp = getelement(this,i))
			    mark(tmp);
		    break;
		}
		break;
	    }

	/* backup to a point where we can continue descending */
	for (;;)

	    /* make sure there is a previous node */
	    if (prev) {
		if (prev->n_flags & LEFT) {	/* came from left side */
		    prev->n_flags &= ~LEFT;
		    tmp = car(prev);
		    rplaca(prev,this);
		    if (this = cdr(prev)) {
			rplacd(prev,tmp);			
			break;
		    }
		}
		else {				/* came from right side */
		    tmp = cdr(prev);
		    rplacd(prev,this);
		}
		this = prev;			/* step back up the branch */
		prev = tmp;
	    }

	    /* no previous node, must be done */
	    else
		return;
    }
}

/* sweep - sweep all unmarked nodes and add them to the free list */
LOCAL sweep()
{
    SEGMENT *seg;
    LVAL p;
    int n;

    /* empty the free list */
    fnodes = NIL;
    nfree = 0L;

    /* add all unmarked nodes */
    for (seg = segs; seg; seg = seg->sg_next) {
	if (seg == fixseg)	 /* don't sweep the fixnum segment */
	    continue;
	else if (seg == charseg) /* don't sweep the character segment */
	    continue;
	p = &seg->sg_nodes[0];
	for (n = seg->sg_size; --n >= 0; ++p)
	    if (!(p->n_flags & MARK)) {
		switch (ntype(p)) {
		case STRING:
			if (getstring(p) != NULL) {
			    total -= (long)getslength(p);
			    free(getstring(p));
			}
			break;
		case STREAM:
			if (getfile(p))
			    osclose(getfile(p));
			break;
#ifdef WINTERP
		case XLTYPE_XmString:
			Wxms_Garbage_Collect_XmString(p);
			break;
		case XLTYPE_Pixmap:
			Wpm_Decr_Refcount_Or_Free_Pixmap(p); /* Tell Motif that the X11 Pixmap is no longer ref'd */
			break;
                case XLTYPE_WIDGETOBJ:
			/* During initialization (:isnew method), we stored
			   a pointer to WIDGETOBJ in the XmNuserData
			   resource. We must clear this since it will be
			   invalid after the WIDGETOBJ is garbage colected */
			Wcls_Garbage_Collect_WIDGETOBJ(p);
				/* fall through to "VECTOR" case */
	        case XLTYPE_TIMEOUTOBJ:
		case XLTYPE_CALLBACKOBJ:
		case XLTYPE_PIXMAP_REFOBJ:
                case XLTYPE_EVHANDLEROBJ:
#endif
		case SYMBOL:
		case OBJECT:
		case VECTOR:
		case CLOSURE:
		case STRUCT:
			if (p->n_vsize) {
			    total -= (long) (p->n_vsize * sizeof(LVAL));
			    free(p->n_vdata);
			}
			break;
		}
		p->n_type = FREE;
		rplaca(p,NIL);
		rplacd(p,fnodes);
		fnodes = p;
		nfree += 1L;
	    }
	    else
		p->n_flags &= ~MARK;
    }
}

/* addseg - add a segment to the available memory */
LOCAL int addseg()
{
    SEGMENT *newseg;
    LVAL p;
    int n;

    /* allocate the new segment */
    if (anodes == 0 || (newseg = newsegment(anodes)) == NULL)
	return (FALSE);

    /* add each new node to the free list */
    p = &newseg->sg_nodes[0];
    for (n = anodes; --n >= 0; ++p) {
	rplacd(p,fnodes);
	fnodes = p;
    }

    /* return successfully */
    return (TRUE);
}

/* newsegment - create a new segment (only called here and in xlimage.c) */
SEGMENT *newsegment(n)
  int n;
{
    SEGMENT *newseg;

    /* allocate the new segment */
    if ((newseg = (SEGMENT *)calloc(1,segsize(n))) == NULL)
	return (NULL);

    /* initialize the new segment */
    newseg->sg_size = n;
    newseg->sg_next = NULL;
    if (segs)
	lastseg->sg_next = newseg;
    else
	segs = newseg;
    lastseg = newseg;

    /* update the statistics */
    total += (long)segsize(n);
    nnodes += (long)n;
    nfree += (long)n;
    ++nsegs;

    /* return the new segment */
    return (newseg);
}
 
/* stats - print memory statistics */
LOCAL stats()
{
    sprintf(buf,"Nodes:       %ld\n",nnodes); stdputstr(buf);
    sprintf(buf,"Free nodes:  %ld\n",nfree);  stdputstr(buf);
    sprintf(buf,"Segments:    %d\n",nsegs);   stdputstr(buf);
    sprintf(buf,"Allocate:    %d\n",anodes);  stdputstr(buf);
    sprintf(buf,"Total:       %ld\n",total);  stdputstr(buf);
    sprintf(buf,"Collections: %d\n",gccalls); stdputstr(buf);
}

/* xgc - xlisp function to force garbage collection */
LVAL xgc()
{
    /* make sure there aren't any arguments */
    xllastarg();

    /* garbage collect */
    gc();

    /* return nil */
    return (NIL);
}

/* xexpand - xlisp function to force memory expansion */
LVAL xexpand()
{
    LVAL num;
    int n,i;

    /* get the new number to allocate */
    if (moreargs()) {
	num = xlgafixnum();
	n = getfixnum(num);
    }
    else
	n = 1;
    xllastarg();

    /* allocate more segments */
    for (i = 0; i < n; i++)
	if (!addseg())
	    break;

    /* return the number of segments added */
    return (cvfixnum((FIXTYPE)i));
}

/* xalloc - xlisp function to set the number of nodes to allocate */
LVAL xalloc()
{
    int n,oldn;
    LVAL num;

    /* get the new number to allocate */
    num = xlgafixnum();
    n = getfixnum(num);

    /* make sure there aren't any more arguments */
    xllastarg();

    /* set the new number of nodes to allocate */
    oldn = anodes;
    anodes = n;

    /* return the old number */
    return (cvfixnum((FIXTYPE)oldn));
}

/* xmem - xlisp function to print memory statistics */
LVAL xmem()
{
    /* allow one argument for compatiblity with common lisp */
    if (moreargs()) xlgetarg();
    xllastarg();

    /* print the statistics */
    stats();

    /* return nil */
    return (NIL);
}

#ifdef SAVERESTORE
/* xsave - save the memory image */
LVAL xsave()
{
    unsigned char *name;

    /* get the file name, verbose flag and print flag */
    name = getstring(xlgetfname());
    xllastarg();

    /* save the memory image */
    return (xlisave(name) ? true : NIL);
}

/* xrestore - restore a saved memory image */
LVAL xrestore()
{
    extern jmp_buf top_level;
    unsigned char *name;

    /* get the file name, verbose flag and print flag */
    name = getstring(xlgetfname());
    xllastarg();

    /* restore the saved memory image */
    if (!xlirestore(name))
	return (NIL);

    /* return directly to the top level */
    stdputstr("[ returning to the top level ]\n");
    longjmp(top_level,1);
}
#endif

/* xlminit - initialize the dynamic memory module */
xlminit()
{
    LVAL p;
    int i;

    /* initialize our internal variables */
    segs = lastseg = NULL;
    nnodes = nfree = total = 0L;
    nsegs = gccalls = 0;
    anodes = NNODES;
    fnodes = NIL;

    /* allocate the fixnum segment */
    if ((fixseg = newsegment(SFIXSIZE)) == NULL)
	xlfatal("insufficient memory");

    /* initialize the fixnum segment */
    p = &fixseg->sg_nodes[0];
    for (i = SFIXMIN; i <= SFIXMAX; ++i) {
	p->n_type = FIXNUM;
	p->n_fixnum = i;
	++p;
    }

    /* allocate the character segment */
    if ((charseg = newsegment(CHARSIZE)) == NULL)
	xlfatal("insufficient memory");

    /* initialize the character segment */
    p = &charseg->sg_nodes[0];
    for (i = CHARMIN; i <= CHARMAX; ++i) {
	p->n_type = CHAR;
	p->n_chcode = i;
	++p;
    }

    /* initialize structures that are marked by the collector */
    obarray = xlenv = xlfenv = xldenv = NIL;
    s_gcflag = s_gchook = NIL;
#ifdef WINTERP
    v_savedobjs = NIL;
#endif

    /* allocate the evaluation stack */
    if ((xlstkbase = (LVAL **)malloc(EDEPTH * sizeof(LVAL *))) == NULL)
	xlfatal("insufficient memory");
    xlstack = xlstktop = xlstkbase + EDEPTH;

    /* allocate the argument stack */
    if ((xlargstkbase = (LVAL *)malloc(ADEPTH * sizeof(LVAL))) == NULL)
	xlfatal("insufficient memory");
    xlargstktop = xlargstkbase + ADEPTH;
    xlfp = xlsp = xlargstkbase;
    *xlsp++ = NIL;
}

