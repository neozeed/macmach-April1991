/* Print mips instructions for GDB, the GNU debugger.
   Copyright (C) 1989 Free Software Foundation, Inc.
   Contributed by Nobuyuki Hikichi(hikichi@sra.co.jp)

This file is part of GDB.

GDB is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GDB is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GDB; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

/*
 * HISTORY
 * $Log:	mips-pinsn.c,v $
 * Revision 3.1  90/04/11  13:33:27  af
 * 	PC-relative long jumps use all of the higher 4 bits.
 * 	[Noted by jmiller@macadamia.cs.bandeis.edu]
 * 
 */
#include <stdio.h>

#include "defs.h"
#include "param.h"
#include "symtab.h"
#include "mips-opcode.h"

/* Mips instructions are never longer than this many bytes.  */
#define MAXLEN 4

/* Number of elements in the opcode table.  */
#define NOPCODES (sizeof mips_opcodes / sizeof mips_opcodes[0])

#ifdef BITS_BIG_ENDIAN
#define MKLONG(p)  \
  ((((((p[0] << 8) + p[1]) << 8) + p[2]) << 8) + p[3])
#else
#define MKLONG(p)  *(unsigned long*)p
#endif

#if 1
extern char *reg_names[];
#else /* for test */
/* Initializer for an array of names of registers.
   There should be NUM_REGS strings in this initializer.  */

#define sREGISTER_NAMES \
 {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7",	\
  "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",	\
  "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",	\
  "$24", "$25", "$26", "$27", "$28", "$sp", "$30", "$ra",	\
  "hi", "lo",  "pc", \
  "$f0", "$f1", "$f2", "$f3", "$f4", "$f5", "$f6", "$f7",	\
  "$f8", "$f9", "$f10", "$f11", "$f12", "$f13", "$f14", "$f15",	\
  "fcsr"};

static char *sreg_names[] = sREGISTER_NAMES;
#define reg_names sreg_names
#endif /* 0 */


/* subroutine */
static unsigned char *
print_insn_arg (d, l, stream, pc)
     char *d;
     register unsigned long int *l;
     FILE *stream;
     CORE_ADDR pc;
{
  extern int use_hex_unasm_p;

  switch (*d)
    {
    case ',':
    case '(':
    case ')':
      fputc (*d, stream);
      break;

    case 's':
      fprintf (stream, "%s", reg_names[((struct op_i_fmt *) l)->rs]);
      break;

    case 't':
      fprintf (stream, "%s", reg_names[((struct op_i_fmt *) l)->rt]);
      break;

    case 'I': /* same as i, but in hex */
      if (use_hex_unasm_p)
        {
	  fprintf (stream, "0x%x", ((struct op_i_fmt *) l)->immediate);
	  break;
	}
      /* fall through */
    case 'i':
      fprintf (stream, "%d", ((struct op_i_fmt *) l)->immediate);
      break;

    case 'J': /* same as j, but in signed hex */
      if (use_hex_unasm_p)
        {
	  register int val = ((struct op_b_fmt *) l)->delta;
	  if (val < 0)
	    {
	      val = -val;
	      fputc ('-', stream);
	    }
	  fprintf (stream, "0x%x", val);
	  break;
      }
      /* fall through */

    case 'j': /* same as i, but sign-extended */
      fprintf (stream, "%d", ((struct op_b_fmt *) l)->delta);
      break;

    case 'a':
      print_address ((pc & 0xF0000000) | (((struct op_j_fmt *)l)->target << 2),
		     stream);
      break;

    case 'b':
      print_address ((((struct op_b_fmt *) l)->delta << 2) + pc + 4, stream);
      break;

    case 'd':
      fprintf (stream, "%s", reg_names[((struct op_r_fmt *) l)->rd]);
      break;

    case 'h':
      fprintf (stream, "0x%x", ((struct op_r_fmt *) l)->shamt);
      break;

    case 'S':
      fprintf (stream, "f%d", ((struct fop_r_fmt *) l)->fs);
      break;

    case 'T':
      fprintf (stream, "f%d", ((struct fop_r_fmt *) l)->ft);
      break;

    case 'D':
      fprintf (stream, "f%d", ((struct fop_r_fmt *) l)->fd);
      break;

    case 'B':
      fprintf (stream, "%d", (*l) >> 16);
      break;

    case 'C':
      {
	char *r;
	switch (((struct op_r_fmt *) l)->rd)
	  {
	  case  0: r = "irr"; break;
	  case 30: r = "eir"; break;
	  case 31: r = "csr"; break;
	  default: r = "???"; break;
	  }
	fprintf (stream, "fpc_%s", r);
	break;
      }

    default:
      fprintf (stream, "# internal error, undefined modifier(%c)", *d);
      break;
    }
}

/* Print the mips instruction at address MEMADDR in debugged memory,
   on STREAM.  Returns length of the instruction, in bytes, which
   is always 4 for normal instructions.  For instructions that have
   branch-delay slots we print the delayed instruction as well
   and return, therefore 8. */

int
print_insn (memaddr, stream)
     CORE_ADDR memaddr;
     FILE *stream;
{
  unsigned char buffer[MAXLEN];
  register int i;
  register char *d;
  unsigned long int l;

  read_memory (memaddr, buffer, MAXLEN);

  for (i = 0; i < NOPCODES; i++)
    {
      register unsigned int opcode = mips_opcodes[i].opcode;
      register unsigned int match = mips_opcodes[i].match;
      if ((*(unsigned int*)buffer & match) == opcode)
	break;
    }

  l = MKLONG (buffer);
  /* Handle undefined instructions.  */
  if (i == NOPCODES)
    {
      fprintf (stream, "0x%x",l);
      return 4;
    }

  fprintf (stream, "%s", mips_opcodes[i].name);

  if (!(d = mips_opcodes[i].args))
    return 4;

  fputc (' ', stream);

  while (*d)
    print_insn_arg (d++, &l, stream, memaddr);

  if (mips_opcodes[i].bdelay)
    {
      fprintf (stream, "\n\t\t");
      return 4 + print_insn( memaddr + 4, stream);
    }
  return 4;
}
