#include "server_defs.h"

/*
 * 68010/68020 instruction disassembler
 */

#define ISYM 0

int	dot, dotinc;
static int	space;
static char *aregs[] = { "a0","a1","a2","a3","a4","a5","a6","sp" };

#ifdef	talkative
static char pcrel[] = "[PC-relative]";
static char normallong[] = ":L" ;
#else	!talkative
static char pcrel[] = "";
static char normallong[] = "" ;
#endif	!talkative

char *bname[16] = { "ra", "sr", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *cname[16] = { "t", "f", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *dbname[16] = { "t", "ra", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *trapname[16] = { "t", "f", "hi", "ls", "cc", "cs", "ne",
		    "eq", "vc", "vs", "pl", "mi", "ge", "lt", "gt", "le" };

char *shro[4] = { "as", "ls", "rox", "ro" };

char *bit[4] = { "btst", "bchg", "bclr", "bset" };

char *oprsize[4] = {"b", "w", "l", "?"};

char *cassize[4] = {"?", "b", "w", "l"};

char *trapsize[8] = {"?", "?", "w", "l", "", "?", "?", "?"};

static char *fregs[] = { "fp0","fp1","fp2","fp3","fp4","fp5","fp6","fp7" };
static char *fcregs[] = { "fpc","fps","fpi" };

char *fccname[32] = {
			"f", "eq", "ogt", "oge", "olt", "ole", "ogl", "or",
			"un", "ueq", "ugt", "uge", "ult", "ule", "ne", "t",
			"sf", "seq", "gt", "ge", "lt", "le", "gl", "leq",
			"nleq", "ngl", "nle", "nlt", "nge", "ngt", "sne","st"
		    } ;

char fpformat[8] = {'l', 's', 'x', 'p', 'w', 'd', 'b', '?' } ;

char *fop[64] = {
	"move",		"int",		"sinh",		"intrz",
	"sqrt",		"?",		"lognp1",	"?",
	"etoxm1", 	"tanh",		"atan",		"?",	
	"asin",		"atanh",	"sin",		"tan",
	"etox",		"twotox",	"tentox",	"?",
	"logn",		"log10",	"log2",		"?",
	"abs",		"cosh",		"neg",		"?",
	"acos",		"cos",		"getexp",	"getman",
	"div",		"mod",		"add",		"mul",
	"sgldiv",	"rem",		"scale",	"sglmul",
	"sub",		"?",		"?",		"?",
	"?",		"?",		"?",		"?",
	"sincos",	"sincos",	"sincos",	"sincos",
	"sincos",	"sincos",	"sincos",	"sincos",
	"cmp",		"?",		"tst",		"?",
	"?",		"?",		"?",		"?"
                } ;

int omove(),obranch(),oimmed(),oprint(),oneop(),soneop(),oreg(),ochk();
int olink(),omovem(),oquick(),omoveq(),otrap(),oscc(),opmode(),shroi();
int extend(),biti(),odbcc(), ostop(), omovs(), omovc(), bkpt();
int cmpchk2(),longmul(),longdiv(),otrapcc(),ortm(),ocallm(),opack();
int cas(), cas2(), bfinst(), bfins(), bfext(), o_uimmed(), omovespecial();
int omovep();
int cp_general(), cp_bccw(), cp_bccl(), cp_tcc(), cp_tccw(), cp_tccl(), cp_scc();
int cp_save(), cp_restore(), cp_dbcc(), cp_unknown();

struct opdesc {			
	unsigned short mask, match;
	int (*opfun)();
	char *farg;
} opdecode[] =
{					/* order is important below */
  0xF000, 0x1000, omove, "b",		/* move instructions */
  0xF000, 0x2000, omove, "l",
  0xF000, 0x3000, omove, "w",
  0xF000, 0x6000, obranch, 0,		/* branches   */
  0xFFF0, 0x06C0, ortm, "rtm",		/* 68020 */
  0xFFC0, 0x06C0, ocallm, "callm",	/* 68020 */
  0xF138, 0x0108, omovep, "movep",
  0xF100, 0x0100, biti, 0,
  0xFF00, 0x0800, biti, 0,
  0xF9FF, 0x08FC, cas2, "cas2",		/* 68020 */
  0xF9C0, 0x08C0, cas, "cas",		/* 68020 */
  0xF9C0, 0x00C0, cmpchk2, 0,		/* 68020 */
  0xFF00, 0x0000, o_uimmed, "or",	/* op class 0  */
  0xFF00, 0x0200, o_uimmed, "and",
  0xFF00, 0x0400, oimmed, "sub",
  0xFF00, 0x0600, oimmed, "add",
  0xFF00, 0x0A00, o_uimmed, "eor",
  0xFF00, 0x0C00, oimmed, "cmp",
  0xFF00, 0x0E00, omovs,  "movs",
  0xFFC0, 0x40C0, omovespecial, 0,	/* move from sr */
  0xFF00, 0x4000, soneop, "negx",
  0xFFC0, 0x42C0, omovespecial, 0,	/* move from cc */
  0xFF00, 0x4200, soneop, "clr",
  0xFFC0, 0x44C0, omovespecial, 0,	/* move to cc */
  0xFF00, 0x4400, soneop, "neg",
  0xFFC0, 0x46C0, omovespecial, 0,	/* move to sr */
  0xFF00, 0x4600, soneop, "not",
  0xFFF8, 0x4808, olink, "\tlinkl\ta%D,#%+Z",	/* 68020 */
  0xFFC0, 0x4800, oneop, "nbcd	",
  0xFFF8, 0x4848, bkpt, "bkpt",		/* 68020 */
  0xFFF8, 0x4840, oreg, "\tswap\td%D",
  0xFFC0, 0x4840, oneop, "pea	",
  0xFFF8, 0x4880, oreg, "\textw\td%D",
  0xFFF8, 0x48C0, oreg, "\textl\td%D",
  0xFFF8, 0x49C0, oreg, "\textbl\td%D",/* 68020 */
  0xFB80, 0x4880, omovem, 0,
  0xFFFF, 0x4AFC, oprint, "illegal",
  0xFFC0, 0x4AC0, oneop, "tas	",
  0xFF00, 0x4A00, soneop, "tst",
  0xFFC0, 0x4C00, longmul, 0,		/* 68020 */
  0xFFC0, 0x4C40, longdiv, 0,		/* 68020 */
  0xFFF0, 0x4E40, otrap, 0,
  0xFFF8, 0x4E50, olink, "\tlinkw\ta%D,#%+z",
  0xFFFE, 0x4E7A, omovc, "\tmovc\t",
  0xFFF8, 0x4E58, oreg, "\tunlk\ta%D",
  0xFFF8, 0x4E60, oreg, "\tmove\ta%D,usp",
  0xFFF8, 0x4E68, oreg, "\tmove\tusp,a%D",
  0xFFFF, 0x4E70, oprint, "reset",
  0xFFFF, 0x4E71, oprint, "nop",
  0xFFFF, 0x4E72, ostop, "\tstop\t#%z",
  0xFFFF, 0x4E73, oprint, "rte",
  0xFFFF, 0x4E74, ostop, "\trtd\t#%+z",
  0xFFFF, 0x4E75, oprint, "rts",
  0xFFFF, 0x4E76, oprint, "trapv",
  0xFFFF, 0x4E77, oprint, "rtr",
  0xFFC0, 0x4E80, oneop, "jsr	",
  0xFFC0, 0x4EC0, oneop, "jmp	",
  0xF1C0, 0x4100, ochk, "chkl",
  0xF1C0, 0x4180, ochk, "chkw",
  0xF1C0, 0x41C0, ochk, "lea",
  0xF0F8, 0x50C8, odbcc, "\tdb%s\td%D,",
  0xF0F8, 0x50F8, otrapcc, 0,		/* 68020 */
  0xF0C0, 0x50C0, oscc,  0,
  0xF100, 0x5000, oquick, "addq",
  0xF100, 0x5100, oquick, "subq",
  0xF000, 0x7000, omoveq, 0,
  0xF1C0, 0x80C0, ochk, "divu",
  0xF1F0, 0x8140, opack, "pack",	/* 68020 */
  0xF1F0, 0x8180, opack, "unpk",	/* 68020 */
  0xF1C0, 0x81C0, ochk, "divs",
  0xF1F0, 0x8100, extend, "sbcd",
  0xF000, 0x8000, opmode, "or",
  0xF1C0, 0x91C0, opmode, "sub",
  0xF130, 0x9100, extend, "subx",
  0xF000, 0x9000, opmode, "sub",
  0xF1C0, 0xB1C0, opmode, "cmp",
  0xF138, 0xB108, extend, "cmpm",
  0xF100, 0xB000, opmode, "cmp",
  0xF100, 0xB100, opmode, "eor",
  0xF1C0, 0xC0C0, ochk, "mulu",
  0xF1C0, 0xC1C0, ochk, "muls",
  0xF1F8, 0xC188, extend, "exg",
  0xF1F8, 0xC148, extend, "exg",
  0xF1F8, 0xC140, extend, "exg",
  0xF1F0, 0xC100, extend, "abcd",
  0xF000, 0xC000, opmode, "and",
  0xF1C0, 0xD1C0, opmode, "add",
  0xF130, 0xD100, extend, "addx",
  0xF000, 0xD000, opmode, "add",
  0xFFC0, 0xE8C0, bfinst, "bftst",	/*68020 */
  0xFFC0, 0xE9C0, bfext, "bfextu",	/*68020 */
  0xFFC0, 0xEAC0, bfinst, "bfchg",	/*68020 */
  0xFFC0, 0xEBC0, bfext, "bfexts",	/*68020 */
  0xFFC0, 0xECC0, bfinst, "bfclr",	/*68020 */
  0xFFC0, 0xEDC0, bfext, "bfffo",	/*68020 */
  0xFFC0, 0xEEC0, bfinst, "bfset",	/*68020 */
  0xFFC0, 0xEFC0, bfins, "bfins",	/*68020 */
  0xF100, 0xE000, shroi, "r",
  0xF100, 0xE100, shroi, "l",
  0xf000+0700, 0xF000+0000, cp_general, 0,	/*68020 */
  0xf000+0700, 0xF000+0200, cp_bccw, 0,		/*68020 */
  0xf000+0700, 0xF000+0300, cp_bccl, 0,		/*68020 */
  0xf000+0700, 0xF000+0400, cp_save, 0,		/*68020 */
  0xf000+0700, 0xF000+0500, cp_restore, 0,	/*68020 */
  0xf000+0777, 0xF000+0172, cp_tccw, 0,		/*68020 */
  0xf000+0777, 0xF000+0173, cp_tccl, 0,		/*68020 */
  0xf000+0777, 0xF000+0174, cp_tcc, 0,		/*68020 */
  0xf000+0770, 0xF000+0110, cp_dbcc, 0,		/*68020 */
  0xf000+0700, 0xF000+0100, cp_scc, 0,		/*68020 */
  0xf000,      0xf000,      cp_unknown, 0,
  0, 0, 0, 0
};

#define immedfmt(sz)   (sz==4 ? "#%+Z" : "#%+z")
#define u_immedfmt(sz) (sz==4 ? "#%Z" : "#%z")

examine_ins(addr)
unsigned long addr;
{
  dot = addr;
  dotinc = 0;
  iprintf("%Z:", addr);
  printins(0, 0, instfetch(2));
  flushbuf();
  return (dotinc);
}

printins(f, idsp, inst)
unsigned short inst;
{
	register struct opdesc *p;
	register int (*fun)();

	space = idsp; dotinc = 2;
	for (p = opdecode; p->mask; p++)
		if ((inst & p->mask) == p->match) break;
	if (p->mask != 0) (*p->opfun)(inst, p->farg);
	else unknown_op(inst) ; 
}

unsigned long
instfetch(size)
int size;
{
	long insword;

	insword = chkget(inkdot(dotinc), space);
	if (size==4) {
	    dotinc += 4;
	} else {
	    insword >>= 16;
	    dotinc += 2;
	}
	return(insword);
}

putindex(index)
	unsigned index;
{
	static short scale[] = {1,2,4,8};

	if (index & 0x8000) {
		iprintf("%s:%c",aregs[(index >> 12) & 0x7],
			(index & 0x800) ? 'l' : 'w');
	} else {
		iprintf("d%D:%c",(index >> 12) & 0x7,
			(index & 0x800) ? 'l' : 'w');
	}
	index = scale[(index >> 9) & 0x3];
	if (index > 1)
		iprintf(":%d",index);
}

printmemind(str,index)
char * str;
long index;
{	unsigned int bdsize, ixtime, odsize;
	long disp;
	int parpr, w;

	bdsize = (index >> 4) & 0x3;
	ixtime = ((index >> 5) & 0x2) | ((index >> 2) & 0x1);
	odsize = index & 0x3;
	parpr = 0;
	if ((index & 0x80) == 0) {
		iprintf("%s",str);
	}
	if (bdsize == 2) {
		disp = instfetch(2);
		iprintf("@(%+z:w",disp);
		parpr = 1;
	} else if (bdsize == 3) {
		disp = instfetch(4);
		iprintf("@(%+Z:l",disp);
		parpr = 1;
	}
	if (parpr == 0) {
		iprintf("@");
	}
	if (ixtime == 0) {
		if (parpr) {
			iprintf(",");
		} else {
			iprintf("(");
			parpr = 1;
		}
		putindex(index);
	}
	if (parpr) {
		iprintf(")");
	}
	if (index & 0x7) {
		iprintf("@");
	}
	parpr = 0;
	if (odsize == 2) {
		disp = instfetch(2);
		iprintf("(%+z:w",disp);
		parpr = 1;
	} else if (odsize == 3) {
		disp = instfetch(4);
		iprintf("(%+Z:l",disp);
		parpr = 1;
	}
	if (ixtime == 1) {
		if (parpr) {
			iprintf(",");
		} else {
			iprintf("(");
			parpr = 1;
		}
		putindex(index);
	}
	if (parpr) {
		iprintf(")");
	}
	if (ixtime == 3) {
		iprintf("???");
	}
}

printea(mode,reg,size)
long mode, reg;
int size;
{

    long index,disp, pcdisp, fs ;

     switch ((int)(mode)) {
      case 0:	iprintf("d%D",reg); break;	
      case 1:	iprintf(aregs[reg]); break;
      case 2:	iprintf("%s@",aregs[reg]); break;
      case 3:	iprintf("%s@+",aregs[reg]); break;
      case 4:	iprintf("%s@-",aregs[reg]); break;
      case 5:	iprintf("%s@(%+z)",aregs[reg],instfetch(2)); break;
      case 6:	index = instfetch(2);
		if (index & 0x100) {
		    printmemind(aregs[reg],index);
		} else {
		    disp = (char)(index&0377);
		    iprintf("%s@(",aregs[reg]);
		    if (disp) {
			iprintf("%+z,",disp);
		    }
		    putindex(index);
		    iprintf(")");
		}
		break;
      case 7:	switch ((int)(reg)) {
	  case 0:	index = instfetch(2);
			psymoff(index, ISYM, ":w");
			break;
	  case 1:	index = instfetch(4);
			psymoff(index, ISYM, normallong) ;/* normal mode :L */
			break;
	  case 2:	
			disp = instfetch(2);
			pcdisp = disp + inkdot(dotinc-2) ;
			fs = findsym( pcdisp, ISYM ) ;
			if (fs <= 9)
				psymoff(pcdisp, ISYM, pcrel); /* normal mode */
			else
				iprintf("pc@(%+z)",disp) ;     /* explicit pc@ */
			break;
	 case 3:	index = instfetch(2);
			if (index & 0x100) {
			    printmemind("pc",index);
			} else {
			    disp = (char)(index&0377);
			    iprintf("pc@(");
			    if (disp) {
				iprintf("%+z,",disp);
			    }
			    putindex(index);
			    iprintf(")");
			}
			break;
	 case 4:	index = instfetch(size==4?4:2);
			if (size==1) index &= 0xff;
			iprintf(immedfmt(size), index);
			break;
	 default:	iprintf("???");
			break;
	}
	break;

      default:	iprintf("???");
    }
}

printEA(ea,size)
long ea;
int size;
{
	printea((ea>>3)&07,ea&07,size);
}

mapsize(inst)
register long inst;
{
	inst >>= 6;
	inst &= 03;
	return((inst==0) ? 1 : (inst==1) ? 2 : (inst==2) ? 4 : -1);
}

char suffix(size)
register int size;
{
	return((size==1) ? 'b' : (size==2) ? 'w' : (size==4) ? 'l' : '?');
}

omove(inst, s)
long inst;
char *s;
{
	int size;

	iprintf("\tmov%c\t",*s);
	size = ((*s == 'b') ? 1 : (*s == 'w') ? 2 : 4);
	printea((inst>>3)&07,inst&07,size);
	printc(',');
	printea((inst>>6)&07,(inst>>9)&07,size);
}

obranch(inst,dummy)
long inst;
{
	long disp = inst & 0377;
	char *s; 

	s = "s ";
	if (disp == 0xFF) {
		s = "l";
		disp = instfetch(4);
	} else if (disp == 0) {
		s = " ";
		disp = instfetch(2);
	} else if (disp > 127) {
		disp |= ~0377;
	}
	iprintf("\tb%s%s\t", bname[(int)((inst>>8)&017)], s );
	psymoff(disp+inkdot(2), ISYM, pcrel);
}

odbcc(inst,form)
long inst;
char *form;
{
	long disp;
	iprintf(form,dbname[(int)((inst>>8)&017)], inst&07);
	disp = instfetch(2);
	psymoff(disp+inkdot(2), ISYM, pcrel);
}

oscc(inst,dummy)	/* called from trapcc */
long inst;
{
	iprintf("\ts%s\t",cname[(int)((inst>>8)&017)]);
	printea((inst>>3)&07,inst&07,1);
}

biti(inst, dummy)
long inst;
{
	iprintf("\t%s\t", bit[(int)((inst>>6)&03)]);
	if (inst&0x0100)
	    iprintf("d%D,", inst>>9);
	else {
	    iprintf(u_immedfmt(2), instfetch(2));
	    printc(',');
	}
	printEA(inst, 2);
}

opmode(inst,opcode)
long inst;
{
	register int opmode = (int)((inst>>6) & 07);
	register int reg = (int)((inst>>9) & 07);
	int size;

	size = (opmode==0 || opmode==4) ?
		1 : (opmode==1 || opmode==3 || opmode==5) ? 2 : 4;
	iprintf("\t%s%c\t", opcode, suffix(size));
	if (opmode>=4 && opmode<=6) {
		iprintf("d%d,",reg);
		printea((inst>>3)&07,inst&07, size);
	} else {
		printea((inst>>3)&07,inst&07, size);
		iprintf(",%c%d",(opmode<=2)?'d':'a',reg);
	}
}

shroi(inst,ds)
long inst;
char *ds;	/* direction string */
{
	int rx, ry;
	char *opcode;
	if ((inst & 0xC0) == 0xC0) {
		/*
		 * shift ops w/memory operands
		 * -- 1 bit shift, shortword only
		 */
		opcode = shro[(int)((inst>>9)&03)];
		iprintf("\t%s%s%c\t", opcode, ds, 'w');
		printEA(inst, 2);
	} else {
		opcode = shro[(int)((inst>>3)&03)];
		iprintf("\t%s%s%c\t", opcode, ds, suffix(mapsize(inst)));
		rx = (int)((inst>>9)&07); ry = (int)(inst&07);
		if ((inst>>5)&01)
			iprintf("d%d,d%d", rx, ry);
		else {
			iprintf(u_immedfmt(2), (rx ? rx : 8));
			iprintf(",d%d", ry);
		}
	}
}		

o_uimmed(inst,opcode) 
long inst;
register char *opcode;
{
	register int size = mapsize(inst);
	long _const;

	if (size > 0) {
		_const = instfetch(size==4?4:2);
		iprintf("\t%s%c\t", opcode, suffix(size));
		iprintf(u_immedfmt(size), _const); printc(',');
		switch (inst & 0xff) {
		case 0x3c:
			iprintf("cc");
			break;
		case 0x7c:
			iprintf("sr");
			break;
		default:
			printEA(inst,size);
			break;
		}
	} else 
		unknown_op(inst) ;
}

oimmed(inst,opcode) 
long inst;
register char *opcode;
{
	register int size = mapsize(inst);
	long _const;

	if (size > 0) {
		_const = instfetch(size==4?4:2);
		iprintf("\t%s%c\t", opcode, suffix(size));
		iprintf(immedfmt(size), _const); printc(',');
		switch(inst & 0xff) {
		case 0x3c:
			iprintf("cc");
			break;
		case 0x7c:
			iprintf("sr");
			break;
		default:
			printEA(inst,size);
			break;
		}
	} else 
		unknown_op(inst) ;
}


omovep(inst, opcode)
long	inst;
char	*opcode;
{
	unsigned dreg, mode, size;
	dreg = (inst >> 9) & 7;
	mode = (inst >> 6) & 7;
	size = ((mode&1)? 4 : 2);
	inst |= 0x20;	/* fake areg@(disp16) mode */
	iprintf("\t%s%c\t", opcode, size==2? 'w' : 'l');
	switch(mode){
	case 0x4:
	case 0x5:
		/* from mem to register */
		printEA(inst, size);
		iprintf(",d%d", dreg);
		break;
	case 0x6:
	case 0x7:
		/* from register to mem */
		iprintf("d%d,", dreg);
		printEA(inst, size);
		break;
	default:
		iprintf("????");
	}
}


extend(inst, opcode)
long	inst;
char	*opcode;
{
	register int size = mapsize(inst);
	int ry = (inst&07), rx = ((inst>>9)&07);
	char c;

	c = ((inst & 0x1000) ? suffix(size) : ' ');
	iprintf("\t%s%c\t", opcode, c);
	if (*opcode == 'e') {
		if (inst & 0x0080)
		    iprintf("d%D,a%D", rx, ry);
		else if (inst & 0x0008)
		    iprintf("a%D,a%D", rx, ry);
		else
		    iprintf("d%D,d%D", rx, ry);
	} else if ((inst & 0xF000) == 0xB000)
		iprintf("a%D@+,a%D@+", ry, rx);
	else if (inst & 0x8)
		iprintf("a%D@-,a%D@-", ry, rx);
	else
		iprintf("d%D,d%D", ry, rx);
}

olink(inst,opcode)
long inst;
char *opcode;
{
	int len = ((inst&~07) == 0x4e50) ? 2 : 4;
	iprintf(opcode, inst&07, instfetch(len));
}

otrap(inst,dummy)
long inst;
{
	iprintf("\ttrap\t");
	iprintf(immedfmt(2), inst&017);
}

oreg(inst,opcode)
long inst;
char *opcode;
{
	iprintf(opcode, inst&0x7);
}

oneop(inst,opcode)
long inst;
register char *opcode;
{
	iprintf("\t%s",opcode);
	printEA(inst, 2);
}

omovespecial(inst,opcode)
long inst;
register char *opcode;
{
	iprintf("\tmovw\t");
	switch((inst>>9)&3) {
	case 0:
		iprintf("sr,");
		printEA(inst,2);
		break;
	case 1:
		iprintf("cc,");
		printEA(inst,2);
		break;
	case 2:
		printEA(inst,2);
		iprintf(",cc");
		break;
	case 3:
		printEA(inst,2);
		iprintf(",sr");
		break;
	}
}

pregmask(mask)
register int mask;
{
	register int i;
	register int flag = 0;

	if (mask) 
		{
		for (i=0; i<16; i++) 
			{
			if (mask&1) 
				{
				if (flag) printc('/'); else flag++;
				iprintf("%c%d",(i<8)?'d':'a',i&07);
				}
			mask >>= 1;
			}
		}
	else
		iprintf("#0") ;
}

omovem(inst,dummy)
long inst;
{
	register int i, list = 0, mask = 0100000;
	register int reglist = (int)(instfetch(2));

	if ((inst & 070) == 040){	/* predecrement */
		for(i = 15; i > 0; i -= 2) {
			list |= ((mask & reglist) >> i);
			mask >>= 1;
		}
		for(i = 1; i < 16; i += 2) {
			list |= ((mask & reglist) << i);
			mask >>= 1;
		}
		reglist = list;
	}
	iprintf("\tmovem%c\t",(inst&0100)?'l':'w');
	if (inst&02000) {
		printEA(inst, 2);
		printc(',');
		pregmask(reglist);
	} else {
		pregmask(reglist);
		printc(',');
		printEA(inst, 2);
	}
}

ochk(inst,opcode)
long inst;
register char *opcode;
{
	int size;

	switch ((inst>>6)&0x7) {
	case 6:
		size = 2;
		break;
	case 4:
		size = 4;
		break;
	}
	iprintf("\t%s\t",opcode);
	printEA(inst, size);
	iprintf(",%c%D",(*opcode=='l')?'a':'d',(inst>>9)&07);
}

soneop(inst,opcode)
long inst;
register char *opcode;
{
	register int size = mapsize(inst);

	if (size > 0) {
		iprintf("\t%s%c\t",opcode,suffix(size));
		printEA(inst, size);
	} else
	    unknown_op(inst) ;
}

oquick(inst,opcode)
long inst;
register char *opcode;
{
	register int size = mapsize(inst);
	register int data = (int)((inst>>9) & 07);

	if (data == 0) data = 8;
	if (size > 0) {
		iprintf("\t%s%c\t", opcode, suffix(size));
		iprintf(u_immedfmt(2), data); printc(',');
		printEA(inst, size);
	} else
		unknown_op(inst) ;
}

omoveq(inst,dummy)
long inst;
{
	int data = (int)(inst & 0377);
	iprintf("\tmoveq\t#%z,d%D", (char)data, (inst>>9)&07 );
}

oprint(inst,opcode)
long inst;
register char *opcode;
{
	iprintf("\t%s",opcode);
}

ostop(inst,opcode)
long inst;
register char *opcode;
{
	iprintf(opcode, instfetch(2));
}

omovs( inst, opcode)
long inst;
char *opcode;
{
	register int size = mapsize(inst);
	unsigned register controlword = instfetch(2)>>11;
	iprintf("\t%s%c\t", opcode, suffix(size));
	if (controlword & 1){
		controlword >>= 1;
		iprintf( (controlword&0x8)?"a%D,":"d%D,", controlword&7 );
		printEA( inst&0xff, size);
	}else{
		controlword >>= 1;
		printEA( inst&0xff, size);
		iprintf( (controlword&0x8)?",a%D":",d%D", controlword&7 );
	}
}

omovc( inst, opcode )
long inst;
char *opcode;
{
	unsigned register controlword = instfetch(2);
	char *creg;
	switch( controlword & 0xfff ){
	case 0:     creg = "sfc"; break;
	case 1:     creg = "dfc"; break;
	case 2:     creg = "cacr"; break;
	case 0x800: creg = "usp"; break;
	case 0x801: creg = "vbr"; break;
	case 0x802: creg = "caar"; break;
	case 0x803: creg = "msp"; break;
	case 0x804: creg = "isp"; break;
	default:    creg = "???";
	}
	controlword >>= 12;
	if( inst & 1){
		iprintf((controlword&0x8)?"%sa%D,%s":"%sd%D,%s",
			opcode, controlword&7, creg );
	} else {
		iprintf((controlword&0x8)?"%s%s,a%D":"%s%s,d%D",
			opcode, creg, controlword&7 );
	}
}

cmpchk2(inst,dummy)
long inst;
{	long word2;
	int size;

	word2 = instfetch(2);
	if (word2 & 0x800) {
		iprintf("\tchk2%s\t",oprsize[(inst >> 9) & 0x3]);
	} else {
		iprintf("\tcmp2%s\t",oprsize[(inst >> 9) & 0x3]);
	}
	size = (inst >> 9) & 0x3;
	size = (size == 0) ? 1: 2 * size;
	printea((inst >> 3) & 0x7, inst & 0x7, size);
	if ((unsigned) word2 & 0x8000) {
		iprintf(",%s",aregs[(word2 >> 12) & 0x7]);
	} else {
		iprintf(",d%D",(word2 >> 12) & 0x7);
	}
}

longmul(inst,dummy)
long inst;
{	long word2;
	word2 = instfetch(2);
	if (word2 & 0x800) {
		iprintf("\tmulsl\t");
	} else {
		iprintf("\tmulul\t");
	}
	printea((inst >> 3) & 0x7, inst & 0x7,4);
	iprintf(",");
	if (word2 & 0x400) {
		iprintf("d%D:",word2 & 0x7);
	}
	iprintf("d%D",(word2 >> 12) & 0x7);
}

longdiv(inst,dummy)
long inst;
{	long word2;
	int size;
	int drem,dquot;
	int dividend64;

	word2 = instfetch(2);
	dquot = (word2>>12)& 0x7;
	drem = word2 & 0x7;
	dividend64 = word2 & 0x400;
	if (word2 & 0x800) {
		iprintf("\tdivs");
	} else {
		iprintf("\tdivu");
	}
	if ( drem != dquot && !dividend64 ) {
		/* backasswards, but agrees with the manual */
		iprintf("l");
	}
	iprintf("l\t");
	printea((inst >> 3) & 0x7, inst & 0x7,4);
	iprintf(",");
	if (drem != dquot) {
		iprintf("d%D:", drem);
	}
	iprintf("d%D", dquot);
}

bkpt(inst,s)
long inst;
char *s;
{
	iprintf("\t%s\t#%z",s,inst & 0x7);
}

otrapcc(inst,dummy)
long inst;
{	long word2;
	int size;

	/*
	 * The "modes" for trapcc, not to be confused with addressing
	 * modes, occupy the same bits as the reg field of an effective
	 * address.  There is a conflict between trapcc's opcode and
	 * effective address mode 0x7 (absolute) which is resolved by
	 * noting that with ea mode 7, the only legal values in the
	 * reg field are {0,1}. For trapcc, the only legal values of
	 * this field are {2,3,4}. wakarimasu ka?
	 */
	switch(inst&0x7) {
	case 0:
	case 1:
		oscc(inst,dummy);
		return;
	case 2:
		size = 2;
		break;
	case 3:
		size = 4;
		break;
	default:
		size = 0;
		break;
	}
	iprintf("\ttrap%s%s\t",trapname[(int) ((inst >> 8) & 0xf)],
		trapsize[inst &0x7]);
	if (size != 0) {
		word2 = instfetch(size);
		iprintf(u_immedfmt(size),word2);
	}
}

ortm(inst,s)
long inst;
char *s;
{	iprintf("\t%s\t",s);
	if (inst & 0x8) {
		iprintf("%s",aregs[inst & 0x7]);
	} else {
		iprintf("d%D",inst & 0x7);
	}
}

ocallm(inst,s)
long inst;
char * s;
{	long word2;

	word2 = instfetch(2);
	iprintf("\t%s\t#%z,",s,word2 & 0xff);
	printea((inst >> 3) & 0x7,inst & 0x7,4);
}

opack(inst,s)
long inst;
char * s;
{	long word2;

	iprintf("\t%s\t",s);
	if (inst & 0x8) {
		iprintf("%s@-,%s@-,#",aregs[inst & 0x7],
			aregs[(inst >> 9) & 0x7]);
	} else {
		iprintf("d%D,d%D,#",inst & 0x7,(inst >> 9) & 0x7);
	}
	word2 = instfetch(2);
	iprintf("%z",word2);
}

cas(inst,s)
long inst;
char *s;
{	long word2;
	word2 = instfetch(2);
	iprintf("\t%s%s\td%D,d%D,",s,cassize[(inst >> 9) & 0x3],word2 & 0x7,
		(word2 >> 6) & 0x7);
	printea((inst >> 3) & 0x7,inst & 0x7,4);
}

cas2(inst,s)
long inst;
char *s;
{	long word2, word3;
	word2 = instfetch(2);
	word3 = instfetch(2);
	iprintf("\t%s%s\td%D:d%D,d%D:d%D,",s,cassize[(inst >> 9) & 0x3],
		word2 & 0x7,word3 & 0x7,
		(word2 >> 6) & 0x7,(word3 >> 6) & 0x7);
	if ((unsigned) word2 & 0x8000) {
		iprintf("(%s):",aregs[(word2 >> 12) & 0x7]);
	} else {
		iprintf("(d%D):",(word2 >> 12) & 0x7);
	}
	if ((unsigned) word3 & 0x8000) {
		iprintf("(%s)",aregs[(word3 >> 12) & 0x7]);
	} else {
		iprintf("(d%D)",(word3 >> 12) & 0x7);
	}
}

bfoperand(w)
register w;
{
	if (w & 0x800) {
		iprintf("{d%D:",(w >> 6) & 0x7);
	} else {
		iprintf("{#%D:",(w >> 6) & 0x1F);
	}
	if (w & 0x20) {
		iprintf("d%D}",w & 0x7);
	} else {
		/* a field width of 0 is interpreted as 32 */
		w &= 0x1f;
		if (w == 0)
			w = 32;
		iprintf("#%D}",w);
	}
}

bfinst(inst,s)
long inst;
char *s;
{	long word2;
	word2 = instfetch(2);
	iprintf("\t%s\t",s);
	printea((inst >> 3) & 0x7,inst & 0x7,4);
	bfoperand(word2);
}

bfins(inst,s)
long inst;
char *s;
{	long word2;
	word2 = instfetch(2);
	iprintf("\t%s\t",s);
	iprintf("d%D,",(word2 >> 12) & 0x7);
	printea((inst >> 3) & 0x7,inst & 0x7,4);
	bfoperand(word2);
}

bfext(inst,s)
long inst;
char *s;
{	long word2;
	word2 = instfetch(2);
	iprintf("\t%s\t",s);
	printea((inst >> 3) & 0x7,inst & 0x7,4);
	bfoperand(word2);
	iprintf(",d%D",(word2 >> 12) & 0x7);
}

printcpid(inst)
int inst ;
{
short cpid ;

cpid = (inst & 0x0e00 ) >> 9 ;
if (cpid != 1) iprintf("@%d\t",cpid) ; else iprintf("\t") ;
}

cp_oneop(inst,opcode)
int inst ;
register char *opcode;
{
	iprintf("\t%s",opcode);
	printcpid(inst) ;
	printEA(inst, 2);
}

cp_bccw(inst,dummy)
{	
int word2 ;

if ((inst & 040) != 0 ) { cp_unknown (inst) ; return ; }

word2 = instfetch(2) ;
if (((inst & 077) == 0) && (word2 == 0))
  	{
	iprintf("\tfnop");
	printcpid(inst) ;
	}
else
	{
	iprintf("\tfb%s",fccname[inst & 077]) ;
	printcpid(inst) ;
	psymoff(word2+inkdot(2), ISYM, pcrel);
	}
}

cp_bccl(inst,dummy)
{	
long word2 ;

if ((inst & 040) != 0 ) { cp_unknown (inst) ; return ; }

word2 = instfetch(4) ;
	iprintf("\tfb%sl",fccname[inst & 077]) ;
	printcpid(inst) ;
	psymoff(word2+inkdot(2), ISYM, pcrel);
}

cp_tcc(inst,dummy)
{	
int word2 ;

word2 = instfetch(2) ;
if ((word2 & 0xffe0) != 0) { cp_unknown2 ( inst, word2 ) ; return ; }
iprintf("\tftrap%s",fccname[word2 & 077]) ;
printcpid(inst) ;
}

cp_tccw(inst,dummy)
{	
int word2 ;
int word3 ;

word2 = instfetch(2) ;
if ((word2 & 0xffe0) != 0) { cp_unknown2 ( inst, word2 ) ; return ; }
word3 = instfetch(2) ;
iprintf("\tftrap%sw",fccname[word2 & 077]) ;
printcpid(inst) ;
iprintf(immedfmt(2),word3) ;
}

cp_tccl(inst,dummy)
{	
int word2 ;
long word3 ;

word2 = instfetch(2) ;
if ((word2 & 0xffe0) != 0) { cp_unknown2 ( inst, word2 ) ; return ; }
word3 = instfetch(4) ;
iprintf("\tftrap%sl",fccname[word2 & 077]) ;
printcpid(inst) ;
iprintf(immedfmt(4),word3) ;

}

cp_scc(inst,dummy)
{	
int word2 ;

word2 = instfetch(2) ;
if ((word2 & 0xffe0) != 0) { cp_unknown2 ( inst, word2 ) ; return ; }
iprintf("\tfs%s",fccname[word2 & 077]) ;
printcpid(inst) ;
printEA(inst&077,0);
}

cp_dbcc(inst,dummy)
{	
int word2 ;
int word3 ;

word2 = instfetch(2) ;
if ((word2 & 0xffe0) != 0) { cp_unknown2 ( inst, word2 ) ; return ; }
word3 = instfetch(2) ;
iprintf("\tfdb%s",fccname[word2 & 077]) ;
printcpid(inst) ;
iprintf("d%d,",inst&7) ;
psymoff(word3+inkdot(4), ISYM, pcrel);
}

cp_save(inst,dummy)
{	
cp_oneop(inst,"fsave") ;
}

cp_restore(inst,dummy)
{	
cp_oneop(inst,"frestore") ;
}

fp_unknown ( inst,word2 ) /* unrecognized floating point command */
{
iprintf("\t.word\t%z,%z\t| invalid floating point command ",inst,word2) ;
}

iprintformat ( inst, word2 )
{	/* prints format character and cpid field */
printc(fpformat[(word2 >> 10) & 7]) ;
printcpid(inst) ;
}

iprintfea( inst, word2 )
{	/* Handles effective address for floating point general instructions. */
short ea = inst & 077 ;
short format = (word2 >> 10) & 7 ;
short intequiv ;
union           {               /* floating point equivalence */
                unsigned long words[3] ;
                float f ;
                double d ;
                /* extended x ; bcdrecord p ; */
                }
                float_equivalence ;

if (ea != 074) printEA(ea,0) ; /* Ordinary ea handling works. */
else switch (format)		/* Special handling for immediates. */
{
case 0: { printEA(ea,4) ; break ; }	/* long */
case 4: { printEA(ea,2) ; break ; }	/* word */
case 6: { printEA(ea,1) ; break ; }	/* byte */
case 1: 
	{ /* single */
	float_equivalence.words[0] = instfetch(4) ;
	intequiv = float_equivalence.f ;
	if (intequiv == float_equivalence.f)
		iprintf("#%d",intequiv) ;
	else
		iprintf("#0f%f",(double) float_equivalence.f) ;
	break ;
	}
case 5: 
	{ /* double */
	float_equivalence.words[0] = instfetch(4) ;
	float_equivalence.words[1] = instfetch(4) ;
	intequiv = float_equivalence.d ;
	if (intequiv == float_equivalence.d)
		iprintf("#%d",intequiv) ;
	else
		iprintf("#0f%f", float_equivalence.d) ;
	break ;
	}
default:
	{ /* extended, packed, or unknown */
	float_equivalence.words[0] = instfetch(4) ;
	float_equivalence.words[1] = instfetch(4) ;
	float_equivalence.words[2] = instfetch(4) ;
	iprintf("#0x%X%X%X", float_equivalence.words[0], 
		float_equivalence.words[1], float_equivalence.words[2]) ;
	break ;
	}
}
}

cp_general(inst,dummy)
{	
int word2 ;

word2 = instfetch(2) ;
switch((word2 >> 13) & 7) 
{
case 0: { fp_0(inst,word2) ; break ; }
case 1: { fp_1(inst,word2) ; break ; }
case 2: { fp_2(inst,word2) ; break ; }
case 3: { fp_3(inst,word2) ; break ; }
case 4: { fp_4(inst,word2) ; break ; }
case 5: { fp_5(inst,word2) ; break ; }
case 6: { fp_6(inst,word2) ; break ; }
case 7: { fp_7(inst,word2) ; break ; }
}
}

fp_0 ( inst,word2 ) 	/* general floating point command with opclas = 0 */
/*
	fopx	fpm,fpn
	fsincosx fpm,fpn:fpq
	ftstx	ea
*/
{
short format = (word2 >> 10) & 7 ;
short op = word2 & 0177 ;
short ea = inst & 077 ;

if ((ea != 0) || (op >= 0100) || (fop[op][0] == '?')) 
	{
	fp_unknown( inst, word2 ) ;
	return ;
	}
iprintf("\tf%sx",fop[op]) ;
printcpid(inst) ;
iprintf("%s",fregs[format]) ;
if (op == 0x3a) return ; 	/* ftst is done */
iprintf(",") ;
if ((0x30 <= op) && (op <= 0x37)) iprintf("%s:",fregs[word2 & 7]) ; /* sincos */
iprintf("%s",fregs[(word2 >> 7) & 7]) ; /* destination */
}

fp_1 ( inst,word2 ) 	/* general floating point command with opclas = 1 */
{
fp_unknown( inst, word2 ) ;
}

fp_2 ( inst,word2 ) 	/* general floating point command with opclas = 2 */
/*
	fopt	ea,fpn
	fsincost ea,fpn:fpq
	ftstt	ea
	fmovecr	#addr,fpn
*/
{
short format = (word2 >> 10) & 7 ;
short op = word2 & 0177 ;
short ea = inst & 077 ;

if (format == 7)
	{
	if (ea != 0)
		{
		fp_unknown( inst, word2 ) ;
		return ;
		}
	else
		{ /* fmovecr */
		iprintf("\tfmovecr") ; printcpid(inst) ;
		iprintf("#%z,%s", op, fregs[(word2 >> 7) & 7]) ;
		return ;
		}
	}
if ((op >= 0100) || (fop[op][0] == '?')) 
	{
	fp_unknown( inst, word2 ) ;
	return ;
	}
iprintf("\tf%s",fop[op]) ;
iprintformat( inst, word2 ) ;
iprintfea(inst, word2) ;
if (op == 0x3a) return ; 	/* ftst is done */
iprintf(",") ;
if ((0x30 <= op) && (op <= 0x37)) iprintf("%s:",fregs[word2 & 7]) ; /* sincos */
iprintf("%s",fregs[(word2 >> 7) & 7]) ; /* destination */
}

fp_3 ( inst,word2 ) 	/* general floating point command with opclas = 3 */
/*
	fmovet	fpm,ea
	fmovep	fpm,ea{#k}	static bcd
	fmovep	fpm,ea{dn}	dynamic bcd
*/
{
short format = (word2 >> 10) & 7 ;

if ((word2 & 0177) != 0) switch (format) 
	{ /* check for illegal extension fields */
	case 3: break ;		/* static movep uses extension for k */
	case 7: if ((word2 & 017) == 0) break ; 
				/* dynamic movep uses extension for dn */
	default: fp_unknown( inst, word2 ) ; return ;
	}

iprintf("\tfmove") ;
if (format == 7) printc('p') ; else printc(fpformat[format]) ;
printcpid(inst) ;
iprintf("%s,",fregs[(word2 >> 7) & 7]) ;
printEA(inst&077,0) ;
switch (format)
{
case 3:	{ iprintf("{#%+z}",word2 & 0177) ; break ; }	/* static movep */
case 7:	{ iprintf("{d%d}",(word2 >> 4) & 7) ; break ; }	/* dynamic movep */
}
}

fp_4 ( inst,word2 ) 	/* general floating point command with opclas = 4 */
/*
	fmovel	ea,fcreg
	fmovem	ea,fcreglist
*/
{
short mask ;

if (word2 & 01777) { fp_unknown(inst, word2) ; return ; }
iprintf("\tfmove") ;
mask = (word2 & 0x1c00) >> 10 ;
if ((mask==1) || (mask==2) || (mask==4)) printc('l') ; else printc('m') ;
	/* Distinguish one from multiple registers. */
printcpid(inst) ;
	{ /* static reg list */
	printEA(inst&077,4) ;
	printc(',') ;
	iprintfcreglist((word2 & 0x1c00) >> 10) ;
	}
}

fp_5 ( inst,word2 ) 	/* general floating point command with opclas = 5 */
/*
	fmovel	fcreg,ea
	fmovem	fcreglist,ea
*/
{
short mask ;

if (word2 & 01777) { fp_unknown(inst, word2) ; return ; }
iprintf("\tfmove") ;
mask = (word2 & 0x1c00) >> 10 ;
if ((mask==1) || (mask==2) || (mask==4)) printc('l') ; else printc('m') ;
	/* Distinguish one from multiple registers. */
printcpid(inst) ;
	{ /* static reg list */
	iprintfcreglist((word2 & 0x1c00) >> 10) ;
	printc(',') ;
	printEA(inst&077,0) ;
	}
}

fp_6 ( inst,word2 ) 	/* general floating point command with opclas = 6 */
/*
	fmovem	ea,reglist
	fmovem	ea,dn
*/
{
if 	(((word2 & 0x700) != 0) || 
	(((word2 & 0x800) == 0x800) && ((word2 & 0x8f) != 0)))
	{ 
	fp_unknown( inst, word2) ; 
	return ; 
	}

iprintf("\tfmovem") ; printcpid(inst) ;
if (word2 & 0x800)
	{ /* dynamic reg list */
	printEA(inst&077,0);
	iprintf(",d%d",(word2 >>4) & 7) ;
	}
else
	{ /* static reg list */
	printEA(inst&077,0) ;
	printc(',') ;
	iprintfreglist(word2 & 0xff, word2 & 0x1000) ;
	}
}

fp_7 ( inst,word2 ) 	/* general floating point command with opclas = 7 */
/*
	fmovem	reglist,ea
	fmovem	dn,ea
*/
{
if 	(((word2 & 0x700) != 0) || 
	(((word2 & 0x800) == 0x800) && ((word2 & 0x8f) != 0)))
	{ 
	fp_unknown( inst, word2) ; 
	return ; 
	}

iprintf("\tfmovem") ; printcpid(inst) ;
if (word2 & 0x800)
	{ /* dynamic reg list */
	iprintf("d%d,",(word2 >>4) & 7) ;
	printEA(inst&077,0);
	}
else
	{ /* static reg list */
	iprintfreglist(word2 & 0xff, word2 & 0x1000) ;
	printc(',') ;
	printEA(inst&077,0) ;
	}
}

iprintfreglist( mask, reversed )
int mask, reversed ;
{
short i ;

if (reversed == 0)
	{ /* bit 12 is off: mask order is fp7..fp0 */
	for ( i = 0 ; mask != 0 ; i++ ) 
		{
		if (mask & 1) 
			{
			iprintf("%s",fregs[i]) ;
			mask = mask >> 1 ;
			if (mask != 0) printc('/') ;
			}
		else	mask = mask >> 1 ;
		}
	}
else
	{ /* bit 12 is on: mask order is fp0..fp7 */
	for ( i = 0 ; mask != 0 ; i++ ) 
		{
		if (mask & 1) 
			{
			iprintf("%s",fregs[7-i]) ;
			mask = mask >> 1 ;
			if (mask != 0) printc('/') ;
			}
		else 	mask = mask >> 1 ;
		}
	}
}

iprintfcreglist( mask )
int mask;
{
short i ;

	for ( i = 0 ; mask != 0 ; i++ ) 
		{
		if (mask & 1) 
			{
			iprintf("%s",fcregs[2-i]) ;
			mask = mask >> 1 ;
			if (mask != 0) printc('/') ;
			}
		else	mask = mask >> 1 ;
		}
}

cp_unknown(inst)
	int inst ;
{
iprintf("\t.word\t%z\t| invalid coprocessor instruction",inst) ;
}

cp_unknown2(inst, word2)
	int inst, word2 ;
{
iprintf("\t.word\t%z,%z\t| invalid coprocessor instruction",inst,word2) ;
}

unknown_op(inst)
	int inst ;
{
iprintf("\t.word\t%z\t| invalid instruction",inst) ;
}


/* --------------------------------------------------------------------- */


int	infile = 0;
int	outfile = 1;
int	maxpos;
int	maxoff;
int	radix = 16;

#define	MAXLIN	255
char	printbuf[MAXLIN];
char	*printptr = printbuf;
char	*digitptr;

printc(c)
	char c;
{
	char d, *q;
	int posn, tabs, p;

	if ((*printptr = c) == '\n') {
		tabs = 0;
		posn = 0;
		q = printbuf;
		for (p = 0; p < printptr - printbuf; p++) {
			d = printbuf[p];
			if ((p&7) == 0 && posn) {
				tabs++;
				posn = 0;
			}
			if (d == ' ')
				posn++;
			else {
				while (tabs > 0) {
					*q++= '\t';
					tabs--;
				}
				while (posn > 0) {
					*q++ = ' ';
					posn--;
				}
				*q++ = d;
			}
		}
		*q++ = '\n';
		(void) write(outfile, printbuf, q - printbuf);
		printptr = printbuf;
		return;
	}
	if (c == '\t') {
		*printptr++ = ' ';
		while ((printptr - printbuf) & 7)
			*printptr++ = ' ';
	} else if (c)
		printptr++;
	if (printptr >= &printbuf[sizeof (printbuf)-9]) {
		(void) write(outfile, printbuf, printptr - printbuf);
		printptr = printbuf;
	}
}

charpos()
{

	return (printptr - printbuf);
}

flushbuf()
{

	if (printptr != printbuf)
		printc('\n');
}

/* VARARGS */
iprintf(fmt)
	char *fmt;
{
	char *s;
	int *vptr, width, prec;
	char c, adj;
	int n, val, _signed;
	double d;
	char digits[1024 /* temporary kluge for sprintf bug */];

	vptr = (int *)&fmt + 1;
	while (c = *fmt++) {
		if (c != '%') {
			printc(c);
			continue;
		}
		if (*fmt == '-') {
			adj = 'l';
			fmt++;
		} else
			adj='r';
		width = convert(&fmt);
		if (*fmt=='.') {
			fmt++;
			prec = convert(&fmt);
		} else
			prec = -1;
		if (*fmt=='+') {
			fmt++;
			_signed = 1;
		} else {
			_signed = 0;
		}
		digitptr = digits;
		val = *vptr++;
		s = 0;
		switch (c = *fmt++) {

		case 'o':
			printoct((unsigned short)val, 0); break;
		case 'q':
			printoct((short)val, -1); break;
		case 'x':
			printhex((unsigned short)val); break;
		case 'Y':
			printdate(val); break;
		case 'r':
			/*
			 * "%+r" is printed in the current radix
			 * with a minus sign if the value is negative
			 */
			if (_signed) {
				printnum((short)val, '+', radix);
			} else {
				printnum((unsigned short)val, c, radix);
			}
			break;
		case 'R':
			printnum(val, (_signed? '+': c), radix); break;
		case 'd':
			printnum((short)val, c, 10); break;
		case 'u':
			printnum((unsigned short)val, c, 10); break;
		case 'D':
		case 'U':
			printnum(val, c, 10); break;
		case 'O':
			printoct(val, 0); break;
		case 'Q':
			printoct(val, -1); break;
		case 'X':
			printhex(val); break;
		case 'c':
			printc(val); break;
		case 's':
			s = (char *)val; break;
		case 'z':
			{ /* form for disassembled 16 bit immediate constants. */
			if ((-9 <= val) && (val <= 9))
				{ /* Let's not use 0x for unambiguous numbers. */
				printnum( val, 'd', 10) ;
				}
			else
				{ /* 0xhex for big numbers. */
				if (_signed && (val < 0)) printc('-') ;
				printc('0') ; printc('x') ;
				if (_signed && (val < 0))
					printhex( -val) ;
				else 	printhex(  val) ;
				}
			break ;
			}
		case 'Z':
			{ /* form for disassembled 32 bit immediate constants. */
			if ((-9 <= val) && (val <= 9))
				{ /* Let's not use 0x for unambiguous numbers. */
				printnum( val, 'D', 10) ;
				}
			else
				{ /* 0xhex for big numbers. */
				if (_signed && (val < 0)) printc('-') ;
				printc('0') ; printc('x') ;
				if (_signed && (val < 0)) 
					printhex(-val) ;
				else 	printhex(val) ;
				}
			break ;
			}
		case 'f':
		case 'F':
			s = digits;
			d = *(double *)(vptr-1);
			vptr += sizeof (double) / sizeof (int);
			prec = -1;
#ifdef vax
			(void) sprintf(s, "%.16e", d);
#else
			doubletos( d, s) ;
#endif
			break;
		case 'm':
			vptr--; break;
		case 'M':
			width = val; break;
		case 'T':
		case 't':
			if (c == 'T')
				width = val;
			else
				vptr--;
			if (width)
				width -= charpos() % width;
			break;
		default:
			printc(c);
			vptr--;
		}
		if (s == 0) {
			*digitptr = 0;
			s = digits;
		}
		n = strlen(s);
		if (prec < n && prec >= 0)
			n = prec;
		width -= n;
		if (adj == 'r')
			while (width-- > 0)
				printc(' ');
		while (n--)
			printc(*s++);
		while (width-- > 0)
			printc(' ');
		digitptr = digits;
	}
}

printdate(tvec)
	int tvec;
{
	register int i;
	char *ctime();
	register char *timeptr = ctime((time_t *)&tvec);

	for (i = 20; i < 24; i++)
		*digitptr++ = timeptr[i];
	for (i = 3; i<19; i++)
		*digitptr++ = timeptr[i];
}

prints(s)
	char *s;
{

	iprintf("%s", s);
}

newline()
{

	printc('\n');
}

convert(cp)
	register char **cp;
{
	register char c;
	int n;

	n = 0;
	while ((c = *(*cp)++) >= '0' && c <= '9')
		n = n * 10 + c - '0';
	(*cp)--;
	return (n);
}

printnum(n, fmat, base)
	int n;
	char fmat;
	int base;
{
	register int k;
	register unsigned un;
	char digs[15];
	register char *dptr = digs;

	/*
	 * if signs are wanted, put 'em out
	 */
	switch(fmat){
	case 'r':
	case 'R':
		if (base != 10) break;
	case '+':
	case 'd':
	case 'D':
	case 'q':
	case 'Q':
		if (n < 0) {
			n = -n;
			*digitptr++ = '-';
		}
		break;
	}
	/*
	 * put out radix
	 */
	switch(base) {
	default:
		break;
	case 010:
		*digitptr++ = '0';
		break;
	case 0x10:
		*digitptr++ = '0';
		*digitptr++ = 'x';
		break;
	}
	un = n;
	while (un) {
		*dptr++ = un % base;
		un /= base;
	}
	if (dptr == digs)
		*dptr++ = 0;
	while (dptr != digs) {
		k = *--dptr;
		*digitptr++ = k + (k <= 9 ? '0' : 'a'-10);
	}
}

printoct(o, s)
	int o, s;
{
	int i, po = o;
	char digs[12];

	if (s) {
		if (po < 0) {
			po = -po;
			*digitptr++ = '-';
		} else
			if (s > 0)
				*digitptr++ = '+';
	}
	for (i = 0; i<=11; i++) {
		digs[i] = po & 7;
		po >>= 3;
	}
	digs[10] &= 03; digs[11]=0;
	for (i = 11; i >= 0; i--)
		if (digs[i])
			break;
	for (i++; i>=0; i--)
		*digitptr++ = digs[i] + '0';
}

printhex( x )
	register int x;
{
	int i;
	char digs[8];
	static char hexe[]="0123456789abcdef";

	for (i = 0; i<=7; i++) {
		digs[i] = x & 0xf;
		x >>= 4;
	}
	for (i = 7; i > 0; i--)
		if (digs[i])
			break;
	for (i; i>=0; i--)
		*digitptr++ = hexe[digs[i]];
}

oclose()
{

	if (outfile != 1) {
		flushbuf();
		(void) close(outfile);
		outfile = 1;
	}
}

endline()
{

	if (maxpos <= charpos())
		iprintf("\n");
}

doubletos(d,s)

double d ;
char *s ;

/* Prints double precision number d. */

{

	{ /* no 68881 */
	union { double d ; 
		struct  { unsigned s:1, e:11, f1:20 ; long f2; } i ; } equivalence ;

	equivalence.d = d ;
        if (equivalence.i.e == 0x7ff)
        	{
		if ((equivalence.i.f1 == 0) && (equivalence.i.f2 == 0))
			strcpy(s , (equivalence.i.s == 0) ? "Inf" : "-Inf") ;
		else
			strcpy(s , (equivalence.i.s == 0) ? "NaN" : "-NaN") ;
		}
        else
                 (void) sprintf(s, "%.16e", d);
	} /* no 68881 */
}

findsym()
{
  return (0x7fffffff);
}

psymoff(v, t, s)
int v, t;
char *s;
{
  iprintf("%Z", v);
  iprintf(s);
}

inkdot(incr)
int incr;
{
  return (dot+incr);
}

unsigned long
chkget(addr, space)
vm_address_t	addr;
{
    unsigned long	x;

    read_task(addr, sizeof (long), &x);
    return (x);
}
