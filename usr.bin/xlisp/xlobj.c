/* -*-C-*-
********************************************************************************
*
* File:         xlobj.c
* RCS:          $Header: xlobj.c,v 1.3 89/11/25 05:41:26 mayer Exp $
* Description:  xlisp object functions
* Author:       David Michael Betz
* Created:      
* Modified:     Sat Nov 25 05:41:13 1989 (Niels Mayer) mayer@hplnpm
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
static char rcs_identity[] = "@(#)$Header: xlobj.c,v 1.3 89/11/25 05:41:26 mayer Exp $";

#include "xlisp.h"

/* external variables */
extern LVAL xlenv,xlfenv,xlvalue;
extern LVAL s_stdout,s_lambda;

/* local variables */
static LVAL s_self,k_new,k_isnew;
static LVAL class,object;

/* instance variable numbers for the class 'Class' */
#define MESSAGES	0	/* list of messages */
#define IVARS		1	/* list of instance variable names */
#define CVARS		2	/* list of class variable names */
#define CVALS		3	/* list of class variable values */
#define SUPERCLASS	4	/* pointer to the superclass */
#define IVARCNT		5	/* number of class instance variables */
#define IVARTOTAL	6	/* total number of instance variables */

/* number of instance variables for the class 'Class' */
#define CLASSSIZE	7

/* forward declarations */
FORWARD LVAL entermsg();
FORWARD LVAL sendmsg();
FORWARD LVAL evmethod();
LOCAL int getivcnt();
LOCAL int listlength();

/* xsend - send a message to an object */
LVAL xsend()
{
    LVAL obj;
    obj = xlgaobject();
    return (sendmsg(obj,getclass(obj),xlgasymbol()));
}

/* xsendsuper - send a message to the superclass of an object */
LVAL xsendsuper()
{
    LVAL env,p;
    for (env = xlenv; env; env = cdr(env))
	if ((p = car(env)) && objectp(car(p)))
	    return (sendmsg(car(p),
			    getivar(cdr(p),SUPERCLASS),
			    xlgasymbol()));
    xlfail("not in a method");
}

/* xlclass - define a class */
LVAL xlclass(name,vcnt)
  char *name; int vcnt;
{
    LVAL sym,cls;

    /* create the class */
    sym = xlenter(name);
    cls = newobject(class,CLASSSIZE);
    setvalue(sym,cls);

    /* set the instance variable counts */
    setivar(cls,IVARCNT,cvfixnum((FIXTYPE)vcnt));
    setivar(cls,IVARTOTAL,cvfixnum((FIXTYPE)vcnt));

    /* set the superclass to 'Object' */
    setivar(cls,SUPERCLASS,object);

    /* return the new class */
    return (cls);
}

#ifdef WINTERP
/* xlclass_p -- check if object is a class object as created by xlclass() */
int xlclass_p(o_class)
     LVAL o_class;		/* assume type==OBJECT */
{
  return (getclass(o_class) == class);
}
#endif

/* xladdivar - enter an instance variable */
xladdivar(cls,var)
  LVAL cls; char *var;
{
    setivar(cls,IVARS,cons(xlenter(var),getivar(cls,IVARS)));
}

/* xladdmsg - add a message to a class */
xladdmsg(cls,msg,offset)
  LVAL cls; char *msg; int offset;
{
    extern FUNDEF funtab[];
    LVAL mptr;

    /* enter the message selector */
    mptr = entermsg(cls,xlenter(msg));

    /* store the method for this message */
    rplacd(mptr,cvsubr(funtab[offset].fd_subr,funtab[offset].fd_type,offset));
}

/* xlobgetvalue - get the value of an instance variable */
int xlobgetvalue(pair,sym,pval)
  LVAL pair,sym,*pval;
{
    LVAL cls,names;
    int ivtotal,n;

    /* find the instance or class variable */
    for (cls = cdr(pair); objectp(cls); cls = getivar(cls,SUPERCLASS)) {

	/* check the instance variables */
	names = getivar(cls,IVARS);
	ivtotal = getivcnt(cls,IVARTOTAL);
	for (n = ivtotal - getivcnt(cls,IVARCNT); n < ivtotal; ++n) {
	    if (car(names) == sym) {
		*pval = getivar(car(pair),n);
		return (TRUE);
	    }
	    names = cdr(names);
	}

	/* check the class variables */
	names = getivar(cls,CVARS);
	for (n = 0; consp(names); ++n) {
	    if (car(names) == sym) {
		*pval = getelement(getivar(cls,CVALS),n);
		return (TRUE);
	    }
	    names = cdr(names);
	}
    }

    /* variable not found */
    return (FALSE);
}

/* xlobsetvalue - set the value of an instance variable */
int xlobsetvalue(pair,sym,val)
  LVAL pair,sym,val;
{
    LVAL cls,names;
    int ivtotal,n;

    /* find the instance or class variable */
    for (cls = cdr(pair); objectp(cls); cls = getivar(cls,SUPERCLASS)) {

	/* check the instance variables */
	names = getivar(cls,IVARS);
	ivtotal = getivcnt(cls,IVARTOTAL);
	for (n = ivtotal - getivcnt(cls,IVARCNT); n < ivtotal; ++n) {
	    if (car(names) == sym) {
		setivar(car(pair),n,val);
		return (TRUE);
	    }
	    names = cdr(names);
	}

	/* check the class variables */
	names = getivar(cls,CVARS);
	for (n = 0; consp(names); ++n) {
	    if (car(names) == sym) {
		setelement(getivar(cls,CVALS),n,val);
		return (TRUE);
	    }
	    names = cdr(names);
	}
    }

    /* variable not found */
    return (FALSE);
}

/* obisnew - default 'isnew' method */
LVAL obisnew()
{
    LVAL self;
    self = xlgaobject();
    xllastarg();
    return (self);
}

/* obclass - get the class of an object */
LVAL obclass()
{
    LVAL self;
    self = xlgaobject();
    xllastarg();
    return (getclass(self));
}

/* obshow - show the instance variables of an object */
LVAL obshow()
{
    LVAL self,fptr,cls,names;
    int ivtotal,n;

    /* get self and the file pointer */
    self = xlgaobject();
    fptr = (moreargs() ? xlgetfile() : getvalue(s_stdout));
    xllastarg();

    /* get the object's class */
    cls = getclass(self);

    /* print the object and class */
    xlputstr(fptr,"Object is ");
    xlprint(fptr,self,TRUE);
    xlputstr(fptr,", Class is ");
    xlprint(fptr,cls,TRUE);
    xlterpri(fptr);

    /* print the object's instance variables */
    for (; cls; cls = getivar(cls,SUPERCLASS)) {
	names = getivar(cls,IVARS);
	ivtotal = getivcnt(cls,IVARTOTAL);
	for (n = ivtotal - getivcnt(cls,IVARCNT); n < ivtotal; ++n) {
	    xlputstr(fptr,"  ");
	    xlprint(fptr,car(names),TRUE);
	    xlputstr(fptr," = ");
	    xlprint(fptr,getivar(self,n),TRUE);
	    xlterpri(fptr);
	    names = cdr(names);
	}
    }

    /* return the object */
    return (self);
}

/* clnew - create a new object instance */
LVAL clnew()
{
    LVAL self;
    self = xlgaobject();
    return (newobject(self,getivcnt(self,IVARTOTAL)));
}

/* clisnew - initialize a new class */
LVAL clisnew()
{
    LVAL self,ivars,cvars,super;
    int n;

    /* get self, the ivars, cvars and superclass */
    self = xlgaobject();
    ivars = xlgalist();
    cvars = (moreargs() ? xlgalist() : NIL);
    super = (moreargs() ? xlgaobject() : object);
    xllastarg();

    /* store the instance and class variable lists and the superclass */
    setivar(self,IVARS,ivars);
    setivar(self,CVARS,cvars);
    setivar(self,CVALS,(cvars ? newvector(listlength(cvars)) : NIL));
    setivar(self,SUPERCLASS,super);

    /* compute the instance variable count */
    n = listlength(ivars);
    setivar(self,IVARCNT,cvfixnum((FIXTYPE)n));
    n += getivcnt(super,IVARTOTAL);
    setivar(self,IVARTOTAL,cvfixnum((FIXTYPE)n));

    /* return the new class object */
    return (self);
}

/* clanswer - define a method for answering a message */
LVAL clanswer()
{
    LVAL self,msg,fargs,code,mptr;

    /* message symbol, formal argument list and code */
    self = xlgaobject();
    msg = xlgasymbol();
    fargs = xlgalist();
    code = xlgalist();
    xllastarg();

    /* make a new message list entry */
    mptr = entermsg(self,msg);

    /* setup the message node */
    xlprot1(fargs);
    fargs = cons(s_self,fargs); /* add 'self' as the first argument */
    rplacd(mptr,xlclose(msg,s_lambda,fargs,code,xlenv,xlfenv));	/* changed by NPM -- pass in lexical and functional environment */
    xlpop();

    /* return the object */
    return (self);
}

/* entermsg - add a message to a class */
LOCAL LVAL entermsg(cls,msg)
  LVAL cls,msg;
{
    LVAL lptr,mptr;

    /* lookup the message */
    for (lptr = getivar(cls,MESSAGES); lptr; lptr = cdr(lptr))
	if (car(mptr = car(lptr)) == msg)
	    return (mptr);

    /* allocate a new message entry if one wasn't found */
    xlsave1(mptr);
    mptr = consa(msg);
    setivar(cls,MESSAGES,cons(mptr,getivar(cls,MESSAGES)));
    xlpop();

    /* return the symbol node */
    return (mptr);
}

/* sendmsg - send a message to an object */
LOCAL LVAL sendmsg(obj,cls,sym)
  LVAL obj,cls,sym;
{
    LVAL msg,msgcls,method,val,p;

    /* look for the message in the class or superclasses */
    for (msgcls = cls; msgcls; ) {

	/* lookup the message in this class */
	for (p = getivar(msgcls,MESSAGES); p; p = cdr(p))
	    if ((msg = car(p)) && car(msg) == sym)
		goto send_message;

	/* look in class's superclass */
	msgcls = getivar(msgcls,SUPERCLASS);
    }

    /* message not found */
    xlerror("no method for this message",sym);

send_message:

    /* insert the value for 'self' (overwrites message selector) */
    *--xlargv = obj;
    ++xlargc;
    
    /* invoke the method */
    if ((method = cdr(msg)) == NULL)
	xlerror("bad method",method);
    switch (ntype(method)) {
    case SUBR:
	val = (*getsubr(method))();
	break;
    case CLOSURE:
	if (gettype(method) != s_lambda)
	    xlerror("bad method",method);
	val = evmethod(obj,msgcls,method);
	break;
    default:
	xlerror("bad method",method);
    }

    /* after creating an object, send it the ":isnew" message */
    if (car(msg) == k_new && val) {
	xlprot1(val);
	sendmsg(val,getclass(val),k_isnew);
	xlpop();
    }
    
    /* return the result value */
    return (val);
}

/* evmethod - evaluate a method */
LOCAL LVAL evmethod(obj,msgcls,method)
  LVAL obj,msgcls,method;
{
    LVAL oldenv,oldfenv,cptr,name,val;
    CONTEXT cntxt;

    /* protect some pointers */
    xlstkcheck(3);
    xlsave(oldenv);
    xlsave(oldfenv);
    xlsave(cptr);

    /* create an 'object' stack entry and a new environment frame */
    oldenv = xlenv;
    oldfenv = xlfenv;
    xlenv = cons(cons(obj,msgcls),getenv(method));
    xlenv = xlframe(xlenv);
    xlfenv = getfenv(method);

    /* bind the formal parameters */
    xlabind(method,xlargc,xlargv);

    /* setup the implicit block */
    if (name = getname(method))
	xlbegin(&cntxt,CF_RETURN,name);

    /* execute the block */
    if (name && setjmp(cntxt.c_jmpbuf))
	val = xlvalue;
    else
	for (cptr = getbody(method); consp(cptr); cptr = cdr(cptr))
	    val = xleval(car(cptr));

    /* finish the block context */
    if (name)
	xlend(&cntxt);

    /* restore the environment */
    xlenv = oldenv;
    xlfenv = oldfenv;

    /* restore the stack */
    xlpopn(3);

    /* return the result value */
    return (val);
}

/* getivcnt - get the number of instance variables for a class */
LOCAL int getivcnt(cls,ivar)
  LVAL cls; int ivar;
{
    LVAL cnt;
    if ((cnt = getivar(cls,ivar)) == NIL || !fixp(cnt))
	xlfail("bad value for instance variable count");
    return ((int)getfixnum(cnt));
}

/* listlength - find the length of a list */
LOCAL int listlength(list)
  LVAL list;
{
    int len;
    for (len = 0; consp(list); len++)
	list = cdr(list);
    return (len);
}

/* obsymbols - initialize symbols */
obsymbols()
{
    /* enter the object related symbols */
    s_self  = xlenter("SELF");
    k_new   = xlenter(":NEW");
    k_isnew = xlenter(":ISNEW");

    /* get the Object and Class symbol values */
    object = getvalue(xlenter("OBJECT"));
    class  = getvalue(xlenter("CLASS"));
}

/* xloinit - object function initialization routine */
xloinit()
{
    /* create the 'Class' object */
    class = xlclass("CLASS",CLASSSIZE);
    setelement(class,0,class);

    /* create the 'Object' object */
    object = xlclass("OBJECT",0);

    /* finish initializing 'class' */
    setivar(class,SUPERCLASS,object);
    xladdivar(class,"IVARTOTAL");	/* ivar number 6 */
    xladdivar(class,"IVARCNT");		/* ivar number 5 */
    xladdivar(class,"SUPERCLASS");	/* ivar number 4 */
    xladdivar(class,"CVALS");		/* ivar number 3 */
    xladdivar(class,"CVARS");		/* ivar number 2 */
    xladdivar(class,"IVARS");		/* ivar number 1 */
    xladdivar(class,"MESSAGES");	/* ivar number 0 */
    xladdmsg(class,":NEW",FT_CLNEW);
    xladdmsg(class,":ISNEW",FT_CLISNEW);
    xladdmsg(class,":ANSWER",FT_CLANSWER);

    /* finish initializing 'object' */
    setivar(object,SUPERCLASS,NIL);
    xladdmsg(object,":ISNEW",FT_OBISNEW);
    xladdmsg(object,":CLASS",FT_OBCLASS);
    xladdmsg(object,":SHOW",FT_OBSHOW);
}
