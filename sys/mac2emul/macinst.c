/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2emul/macinst.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <kern/thread.h>

#include <mach/mac2/exception.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macinst.h>
#include <mac2emul/macuser.h>

/*
 * Privileged instruction emulation.
 */

#define FATAL(xxx)							\
{									\
    frame->f_pc = i_pc;							\
    printf("%08x %s\n", i_pc, (xxx));					\
    return (0);								\
}

#ifdef DEBUG_INST_ACTION
#define DEBUG_INST_HALT(xxx)	\
{                               \
    int s_pc = frame->f_pc;     \
    printf("0x%x:", i_pc);      \
    { xxx; }                    \
    inst_cmd(regs);             \
    frame->f_pc = s_pc;         \
}
#else DEBUG_INST_ACTION
#define DEBUG_INST_HALT(xxx)
#endif DEBUG_INST_ACTION

int	instdebug = 0;

#define INST_ACTION_0_EXTEN(nnn)					\
inst_##nnn##(inst, frame, regs)      				\
unsigned short inst;                     				\
register normal_exception_frame_t *frame;				\
register struct regs *regs;       				\
{                                        				\
    register struct status_reg *sr = (struct status_reg *)&frame->f_sr;	\
    register i_pc = frame->f_pc - sizeof (inst);			\
    {

#define INST_ACTION_1_EXTEN(nnn)   					\
inst_##nnn##(inst, frame, regs)					\
unsigned short inst;                     				\
register normal_exception_frame_t *frame;				\
register struct regs *regs;					\
{									\
    register struct status_reg *sr = (struct status_reg *)&frame->f_sr;	\
    register i_pc = frame->f_pc - sizeof (inst);			\
    unsigned short exten;						\
    FETCH_INST(exten);							\
    {

#define INST_EMUL(xxx) \
    { xxx; }

#define INST_CC(xxx) \
    { xxx; }

#define INST_END() } return (1); }

#define DEFINE_CELL(name) \
    struct cell name;     \
    name.c_longword = 0

#define DEFINE_MULT_CELL(name, nelt)	\
    struct cell name[nelt];		\
    {					\
	register _n;			\
					\
	for (_n = 0; _n < nelt; _n++)	\
	    name[_n].c_longword = 0;	\
    }

#define r_SR	(current_thread_pcb()->pcb_emul->sr)

#define r_CTRL(x)	(current_thread_pcb()->pcb_emul->ctrl[x])

#define UPDATE_r_SR() \
    ((struct status_reg *)&r_SR)->sr_cc = frame->f_sr;

#define r_TC	(current_thread_pcb()->pcb_emul->pmmu.TC)
#define r_CRP	(current_thread_pcb()->pcb_emul->pmmu.CRP)

/*
 * Return the next
 * word from the instruction
 * stream.
 */
#define FETCH_INST(x)	FETCH_INCR(frame->f_pc, &(x), unsigned short)

/*
 * The current
 * value of the PC
 */
#define PC_VALUE	(frame->f_pc)

/*
 * Instruction emulation
 * routines.
 */

INST_ACTION_1_EXTEN(andi_sr)
     DEBUG_INST_HALT(printf("\tandi #0x%x, SR", exten));
     INST_EMUL(UPDATE_r_SR();
	       r_SR &= exten;
	       if (r_SR&SR_TRACE)
	           sr->sr_t1 = 1;
	       else
	           sr->sr_t1 = 0;
	       INST_CC(sr->sr_cc = r_SR));
INST_END()

INST_ACTION_1_EXTEN(eori_sr)
     DEBUG_INST_HALT(printf("\teori #0x%x, SR", exten));
     INST_EMUL(UPDATE_r_SR();
	       r_SR ^= (exten&~SR_SUPR);
	       if (r_SR&SR_TRACE)
	           sr->sr_t1 = 1;
	       else
	           sr->sr_t1 = 0;
	       INST_CC(sr->sr_cc = r_SR));
INST_END()

INST_ACTION_1_EXTEN(ori_sr)
     DEBUG_INST_HALT(printf("\tori #0x%x, SR", exten));
     INST_EMUL(UPDATE_r_SR();
	       r_SR |= (exten&~SR_SUPR);
	       if (r_SR&SR_TRACE)
	           sr->sr_t1 = 1;
	       else
	           sr->sr_t1 = 0;
	       INST_CC(sr->sr_cc = r_SR));
INST_END()

INST_ACTION_0_EXTEN(move_f_sr)
     struct {
	 unsigned short :10,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     DEBUG_INST_HALT(printf("\tmove SR, ");
		     print_addr_mode(i->mode, i->reg, OPSIZE_WORD, &PC_VALUE));
     INST_EMUL(DEFINE_CELL(data);
	       UPDATE_r_SR();
	       data.c_loword = r_SR;
	       store_operand(&data,
			     i->mode, i->reg,
			     OPSIZE_WORD,
			     &PC_VALUE, regs));
INST_END()

INST_ACTION_0_EXTEN(move_t_sr)
     struct {
	 unsigned short :10,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     DEBUG_INST_HALT(printf("\tmove ");
		     print_addr_mode(i->mode, i->reg, OPSIZE_WORD, &PC_VALUE);
		     printf(", SR"));
     INST_EMUL(DEFINE_CELL(data);
	       fetch_operand(&data,
			     i->mode, i->reg,
			     OPSIZE_WORD,
			     &PC_VALUE, regs);
	       r_SR = (data.c_loword&~SR_SUPR);
	       if (r_SR&SR_TRACE)
	           sr->sr_t1 = 1;
	       else
	           sr->sr_t1 = 0;
	       INST_CC(sr->sr_cc = r_SR));
INST_END()

INST_ACTION_0_EXTEN(move_usp)
     struct {
	 unsigned short :12,
                     dir:1,
                     reg:3;
     } *i = (typeof (i))&inst;
     DEBUG_INST_HALT(printf(i->dir?"\tmove USP, A%x":"\tmove A%x, USP",
			    i->reg));
     INST_EMUL(if (i->dir)
	           regs->r_areg[i->reg] = r_CTRL(R_USP);
	       else
	           r_CTRL(R_USP) = regs->r_areg[i->reg]);	         
INST_END()

#ifndef KERNEL
char movec_gregtyp[] = { 'D', 'A' };
char *movec_cregs[] =
    { "DFC", "SFC", "CACR", "USP", "VBR", "CAAR", "MSP", "ISP" };
#endif
unsigned long movec_not_020datamask[] =
    { 0, 0, 0xfffffffc, 0, 0, 0, 0, 0 };
unsigned long movec_not_030datamask[] =
    { 0, 0, 0xffffccec, 0, 0, 0, 0, 0 };

INST_ACTION_1_EXTEN(movec)
     struct {
	 unsigned short :15,
                     dir:1;
     } *i = (typeof (i))&inst;
     struct {
	 unsigned short gregtyp:1,
                           greg:3,
                           creg:12;
     } *e = (typeof (e))&exten;
     register creg;
     register struct cell *greg;
     extern is68030;
     DEBUG_INST_HALT(creg = e->creg;
		     if (creg&0x800)
		         creg -= (0x800 - 3);
		     if (i->dir)
		         printf("\tmovec %c%x, %s",
				movec_gregtyp[e->gregtyp],
				e->greg,
				movec_cregs[creg]);
		     else
		         printf("\tmovec %s, %c%x",
				movec_cregs[creg],
				movec_gregtyp[e->gregtyp],
				e->greg));
     INST_EMUL(creg = e->creg;
	       if (creg&0x800)
	           creg -= (0x800 - 3);
	       if (e->gregtyp)
	           greg = (struct cell *)&regs->r_areg[e->greg];
	       else
	           greg = (struct cell *)&regs->r_dreg[e->greg];
	       if (i->dir)
	           r_CTRL(creg) = greg->c_longword&
	       		~(is68030? movec_not_030datamask[creg]:
				    movec_not_020datamask[creg]);
	       else
	           greg->c_longword = r_CTRL(creg));
INST_END()

#ifndef KERNEL
char moves_sizes[] = { 'b', 'w', 'l' };
char moves_gregtyp[] = { 'D', 'A' };
#endif

INST_ACTION_1_EXTEN(moves)
     struct {
	 unsigned short :8,
                    size:2,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     struct {
	 unsigned short gregtyp:1,
                           greg:3,
                            dir:1,
	                       :11;
     } *e = (typeof (e))&exten;
     DEBUG_INST_HALT(printf("\tmoves%c ", moves_sizes[i->size]);
		     if (e->dir) {
			 printf("%c%x, ", moves_gregtyp[e->gregtyp], e->greg);
			 print_addr_mode(i->mode, i->reg, i->size, &PC_VALUE);
		     } else {
			 print_addr_mode(i->mode, i->reg, i->size, &PC_VALUE);
			 printf(", %c%x", moves_gregtyp[e->gregtyp], e->greg);
		     });
#ifdef notdef
     INST_EMUL(DEFINE_CELL(data);
	       if (e->dir) {
		   if (r_CTRL(R_DFC) != 5 && r_CTRL(R_DFC) != 6)
		       FATAL("Illegal destination function code");
		   if (e->gregtyp)
		       data.c_longword = regs->r_areg[e->greg];
		   else
		       data.c_longword = regs->r_dreg[e->greg];
		   store_operand(&data,
				 i->mode, i->reg,
				 i->size,
				 &PC_VALUE, regs);
	       } else {
		   if (r_CTRL(R_SFC) != 5 && r_CTRL(R_SFC) != 6)
		       FATAL("Illegal source function code");
		   if (e->gregtyp) {
		       fetch_operand(&data,
				     i->mode, i->reg,
				     i->size,
				     &PC_VALUE, regs);
		       sign_extend(&data, i->size);
		       regs->r_areg[e->greg] = data.c_longword;
		   } else {
		       fetch_operand(&regs->r_dreg[e->greg],
				     i->mode, i->reg,
				     i->size,
				     &PC_VALUE, regs);
		   }
	       });
#endif
     INST_EMUL(FATAL("moves"));
INST_END()

INST_ACTION_0_EXTEN(reset)
     unsigned long RSP, RPC, RVEC;
     DEBUG_INST_HALT(printf("\treset"));
     INST_EMUL({
	 RVEC = current_thread_pcb()->pcb_emul->reset_vector;
	 FETCH(RVEC, &RSP, RSP);
	 FETCH(RVEC+4, &RPC, RPC);

	 RSP -= sizeof (RSP);
	 STORE(RSP, &PC_VALUE, PC_VALUE);

	 RSP -= sizeof (RPC);
	 STORE(RSP, &regs->r_sp, regs->r_sp);

	 regs->r_sp = RSP;
	 PC_VALUE = RPC;

	 mac_reset(current_thread_pcb());
    });
INST_END()

INST_ACTION_0_EXTEN(rte)
     DEBUG_INST_HALT(printf("\trte"));
     INST_EMUL({
	 if (return_from_exception(frame, regs) == 0)
	     FATAL("rte")
     });
INST_END()

INST_ACTION_1_EXTEN(stop)
     DEBUG_INST_HALT(printf("\tstop #0x%x", exten));
#ifdef notdef
     INST_EMUL(r_SR = exten;
	       INST_CC(sr->sr_cc = r_SR));
#endif
     INST_EMUL(FATAL("stop"));
INST_END()

#ifndef KERNEL
char *pmmu_cond[] =
  { "bs", "bc", "ls", "lc", "ss", "sc", "as", "ac",
      "ws", "wc", "is", "ic", "gs", "gc", "cs", "cc" };

char pmmu_size[] = { 'w', 'l' };
#endif

INST_ACTION_0_EXTEN(cpBcc)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :2,
                    size:1,
                      cc:6;
     } *i = (typeof (i))&inst;
     struct cell disp;
     int pc = PC_VALUE;
     if (i->cpid)
         FATAL("priv fault cpBcc cpid not pmmu");
     DEBUG_INST_HALT(printf("\tpb%s%c ",
			    pmmu_cond[i->cc],
			    pmmu_size[i->size]);
		     if (i->size) {
			 FETCH_INST(disp.c_hiword);
			 FETCH_INST(disp.c_loword);
			 pc += (int)disp.c_longword;
			 printf("0x%x", pc);
		     } else {
			 FETCH_WORD(disp.c_loword);
			 pc += (short)disp.c_loword;
			 printf("0x%x", pc);
		     });
     INST_EMUL(FATAL("pbcc"));
INST_END()

INST_ACTION_1_EXTEN(cpDBcc)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :6,
                    dreg:3;
     } *i = (typeof (i))&inst;
     struct {
	 unsigned short :10,
                      cc:6;
     } *e = (typeof (e))&exten;
     struct cell disp;
     int pc = PC_VALUE - 2;
     if (i->cpid)
         FATAL("priv fault cpDBcc cpid not pmmu");
     DEBUG_INST_HALT(printf("\tpdb%s D%x, ", pmmu_cond[e->cc], i->dreg);
		     FETCH_INST(disp.c_loword);
		     pc += (short)disp.c_loword;
		     printf("0x%x", pc));
     INST_EMUL(FATAL("pdbcc"));
INST_END()

#ifndef KERNEL
char pmmu_rw[] = { 'r', 'w' };
char *pmmu_fcregs[] = { "SFC", "DFC" };
char *pmmu_regs1[] = { "TC", "DRP", "SRP", "CRP", "CAL", "VAL", "SCC", "AC" };
char *pmmu_regs2[] = { "PSR", "PCSR", 0, 0, "BAD", "BAC" };

print_pmmu_fc(fc)
unsigned short fc;
{
    if (fc&0x10)
	printf("#0x%x, ", fc&0xf);
    else if ((fc&0x18) == 0x8)
	printf("D%x, ", fc&0x7);
    else
	printf("%s, ", pmmu_fcregs[fc]);
}

print_pmove_addr_mode(format, preg, mode, reg, apc)
register format, mode, reg;
register unsigned short preg, **apc;
{
    switch (preg) {
      case 0:
	print_addr_mode(mode, reg,
			(format == 1)?OPSIZE_LONG:OPSIZE_WORD, apc);
	break;
	
      case 1:
      case 2:
      case 3:
	print_addr_mode(mode, reg,
			(format == 1)?OPSIZE_DBLONG:OPSIZE_WORD, apc);
	break;
	
      case 4:
      case 5:
      case 6:
	print_addr_mode(mode, reg,
			(format == 1)?OPSIZE_BYTE:OPSIZE_WORD, apc);
	break;
	
      case 7:
	print_addr_mode(mode, reg, OPSIZE_WORD, apc);
	break;
    }
}

char ptest_mode[] = { 'w', 'r' };
#endif

INST_ACTION_1_EXTEN(cpGEN)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :3,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     struct {
	 unsigned short op:3,
                     subop:3,
	                  :10;
     } *e = (typeof (e))&exten;
     if (i->cpid)
         FATAL("priv fault cpGEN cpid not pmmu");
     switch (e->op) {
       case 1:
         switch (e->subop) {
	   case 0:
	     {
		 struct {
		     unsigned short :6,
		                  rw:1,
		                    :4,
		                  fc:5;
		 } *e = (typeof (e))&exten;
		 DEBUG_INST_HALT(printf("\tpload%c ", pmmu_rw[e->rw]);
				 print_pmmu_fc(e->fc);
				 print_addr_mode(i->mode,
						 i->reg,
						 OPSIZE_NONE,
						 &PC_VALUE));
		 INST_EMUL(FATAL("pload"));
		 break;
	     }

	   case 2:
	     DEBUG_INST_HALT(printf("\tpvalid VAL, ");
			     print_addr_mode(i->mode,
					     i->reg,
					     OPSIZE_LONG,
					     &PC_VALUE));
	     INST_EMUL(FATAL("pvalid 1"));
	     break;

	   case 3:
	     {
		 struct {
		     unsigned short :13,
		                 reg:3;
		 } *e = (typeof (e))&exten;
		 DEBUG_INST_HALT(printf("\tpvalid A%x, ", e->reg);
				 print_addr_mode(i->mode,
						 i->reg,
						 OPSIZE_LONG,
						 &PC_VALUE));
		 INST_EMUL(FATAL("pvalid 2"));
		 break;
	     }

	   default:
	     {
		 struct {
		     unsigned short :3,
		                mode:3,
		                    :1,
		                mask:4,
		                  fc:5;
		 } *e = (typeof (e))&exten;
		 switch (e->mode) {
		   case 1:
		     DEBUG_INST_HALT(printf("\tpflusha"));
		     INST_EMUL(FATAL("pflusha"));
		     break;

		   case 4:
		     DEBUG_INST_HALT(printf("\tpflush ");
				     print_pmmu_fc(e->fc);
				     printf(", #0x%x", e->mask));
		     INST_EMUL(FATAL("pflush"));
		     break;

		   case 5:
		     DEBUG_INST_HALT(printf("\tpflushs ");
				     print_pmmu_fc(e->fc);
				     printf(", #0x%x", e->mask));
		     INST_EMUL(FATAL("pflushs"));
		     break;

		   case 6:
		     DEBUG_INST_HALT(printf("\tpflush ");
				     print_pmmu_fc(e->fc);
				     printf(", #0x%x", e->mask);
				     print_addr_mode(i->mode,
						     i->reg,
						     OPSIZE_NONE,
						     &PC_VALUE));
		     INST_EMUL(FATAL("pflush <ea>"));
		     break;

		   case 7:
		     DEBUG_INST_HALT(printf("\tpflushs ");
				     print_pmmu_fc(e->fc);
				     printf(", #0x%x", e->mask);
				     print_addr_mode(i->mode,
						     i->reg,
						     OPSIZE_NONE,
						     &PC_VALUE));
		     INST_EMUL(FATAL("pflushs <ea>"));
		     break;
		 }
		 break;
	     }
	 }
	 break;

       case 2:
	 {
	     struct {
		 unsigned short :3,
	                    preg:3,
	                      rw:1,
		                :9;
	     } *e = (typeof (e))&exten;
	     DEBUG_INST_HALT(if (e->rw) {
		                 printf("\tpmove %s, ", pmmu_regs1[e->preg]);
				 print_pmove_addr_mode(1,
						       e->preg,
						       i->mode,
						       i->reg,
						       &PC_VALUE);
	                     } else {
				 printf("\tpmove ");
				 print_pmove_addr_mode(1,
						       e->preg,
						       i->mode,
						       i->reg,
						       &PC_VALUE);
				 printf(", %s", pmmu_regs1[e->preg]);
			     });
	     INST_EMUL({
		 if (e->rw) {
		     /* pmove preg,<ea> */
		     switch (e->preg) {
		       case 0: /* TC */
			 {
			     DEFINE_CELL(data);
			     data.c_longword = r_TC;
			     store_operand(&data,
					   i->mode, i->reg,
					   OPSIZE_LONG,
					   &PC_VALUE, regs);
			     break;
			 }

		       case 1: /* DRP */
		       case 2: /* SRP */
			 FATAL("pmove DRP SRP");

		       case 3: /* CRP */
			 {
			     DEFINE_MULT_CELL(data, 2);
			     data[0].c_longword = r_CRP[0];
			     data[1].c_longword = r_CRP[1];
			     store_operand(data,
					   i->mode, i->reg,
					   OPSIZE_DBLONG,
					   &PC_VALUE, regs);
			     break;
			 }

		       case 4: /* CAL */
		       case 5: /* VAL */
		       case 6: /* SCC */
			 {
			     DEFINE_CELL(data);
			     store_operand(&data,
					   i->mode, i->reg,
					   OPSIZE_BYTE,
					   &PC_VALUE, regs);
			     break;
			 }

		       case 7: /* AC */
			 {
			     DEFINE_CELL(data);
			     store_operand(&data,
					   i->mode, i->reg,
					   OPSIZE_WORD,
					   &PC_VALUE, regs);
			     break;
			 }
		     }
		 }
		 else {
		     /* pmove <ea>,preg */
		     switch (e->preg) {
		       case 0: /* TC */
			 {
			     DEFINE_CELL(data);
			     fetch_operand(&data,
					   i->mode, i->reg,
					   OPSIZE_LONG,
					   &PC_VALUE, regs);
			     r_TC = data.c_longword;
			     if (data.c_longword&0x80000000) {
				 if (data.c_longword&0x000f0000)
				     mac_set_addr_mode(0);
				 else
				     mac_set_addr_mode(1);
			     }
			     else
				 mac_set_addr_mode(1);
			     break;
			 }

		       case 1: /* DRP */
		       case 2: /* SRP */
			 FATAL("pmove DRP SRP");

		       case 3: /* CRP */
			 {
			     DEFINE_MULT_CELL(data, 2);
			     fetch_operand(data,
					   i->mode, i->reg,
					   OPSIZE_DBLONG,
					   &PC_VALUE, regs);
			     r_CRP[0] = data[0].c_longword;
			     r_CRP[1] = data[1].c_longword;
			     break;
			 }

		       case 4: /* CAL */
		       case 5: /* VAL */
		       case 6: /* SCC */
			 {
			     DEFINE_CELL(data);
			     fetch_operand(&data,
					   i->mode, i->reg,
					   OPSIZE_BYTE,
					   &PC_VALUE, regs);
			     break;
			 }

		       case 7: /* AC */
			 {
			     DEFINE_CELL(data);
			     fetch_operand(&data,
					   i->mode, i->reg,
					   OPSIZE_WORD,
					   &PC_VALUE, regs);
			     break;
			 }
		     }
		 }
	     });
	 }
	 break;

       case 3:
	 {
	     struct {
		 unsigned short :3,
	                    preg:3,
	                      rw:1,
		                :4,
	                     num:3,
		                :2;
	     } *e = (typeof (e))&exten;
	     DEBUG_INST_HALT(if (e->rw) {
		                 if (e->preg == 4 || e->preg == 5)
				     printf("\tpmove %s%x, ",
					    pmmu_regs2[e->preg],
					    e->num);
				 else
				     printf("\tpmove %s, ",
					    pmmu_regs2[e->preg]);
				 print_pmove_addr_mode(2,
						       e->preg,
						       i->mode,
						       i->reg,
						       &PC_VALUE);
	                     } else {
				 printf("\tpmove ");
				 print_pmove_addr_mode(2,
						       e->preg,
						       i->mode,
						       i->reg,
						       &PC_VALUE);
				 if (e->preg == 4 || e->preg == 5)
				     printf(", %s%x",
					    pmmu_regs2[e->preg], e->num);
				 else
				     printf(", %s", pmmu_regs2[e->preg]);
			     });
	     INST_EMUL(FATAL("pmove 2"));
	 }
	 break;

       case 4:
	 {
	     struct {
		 unsigned short :3,
	                   level:3,
	                      rw:1,
	                 enaareg:1,
	                    areg:3,
	                      fc:5;
	     } *e = (typeof (e))&exten;
	     DEBUG_INST_HALT(printf("\tptest%c ", ptest_mode[e->rw]);
			     print_pmmu_fc(e->fc);
			     printf(", ");
			     print_addr_mode(i->mode, i->reg,
					     OPSIZE_NONE, &PC_VALUE);
			     printf(", #0x%x", e->level);
			     if (e->enaareg)
			         printf(", A%x", e->areg));
	     INST_EMUL(FATAL("ptest"));
	 }
	 break;

       case 5:
	 {
	     DEBUG_INST_HALT(printf("\tpflushr ");
			     print_addr_mode(i->mode,
					     i->reg,
					     OPSIZE_DBLONG,
					     &PC_VALUE));
	     INST_EMUL(FATAL("pflushr"));
	 }
	 break;
     }
INST_END()

INST_ACTION_0_EXTEN(cpRESTORE)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :3,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     if (i->cpid == 0) {
	 DEBUG_INST_HALT(printf("\tprestore ");
			 print_addr_mode(i->mode, i->reg,
					 OPSIZE_NONE, &PC_VALUE));
	 INST_EMUL(FATAL("prestore"));
     } else if (i->cpid == 1) {
	 DEBUG_INST_HALT(printf("\tfrestore ");
			 print_addr_mode(i->mode, i->reg,
					 OPSIZE_NONE, &PC_VALUE));
	 INST_EMUL(fetch_frestore_operand(&current_thread_pcb()->pcb_fp_frame,
					  i->mode, i->reg,
					  &PC_VALUE, regs);
		   fp_continue());
     } else
        FATAL("priv fault cpRESTORE not pmmu or fpu");
INST_END()

INST_ACTION_0_EXTEN(cpSAVE)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :3,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     if (i->cpid == 0) {
	 DEBUG_INST_HALT(printf("\tpsave ");
			 print_addr_mode(i->mode, i->reg,
					 OPSIZE_NONE, &PC_VALUE));
	 INST_EMUL(FATAL("psave"));
     } else if (i->cpid == 1) {
	 DEBUG_INST_HALT(printf("\tfsave ");
			 print_addr_mode(i->mode, i->reg,
					 OPSIZE_NONE, &PC_VALUE));
	 INST_EMUL(fp_stop();
		   store_fsave_operand(&current_thread_pcb()->pcb_fp_frame,
				       i->mode, i->reg,
				       &PC_VALUE, regs));
     } else
        FATAL("priv fault cpSAVE not pmmu or fpu");
INST_END()

INST_ACTION_1_EXTEN(cpScc)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :3,
                    mode:3,
                     reg:3;
     } *i = (typeof (i))&inst;
     struct {
	 unsigned short :10,
                      cc:6;
     } *e = (typeof (e))&exten;
     if (i->cpid)
         FATAL("priv fault cpSCC not pmmu");
     DEBUG_INST_HALT(printf("\tps%s ", pmmu_cond[e->cc]);
		     print_addr_mode(i->mode, i->reg,
				     OPSIZE_BYTE, &PC_VALUE));
     INST_EMUL(FATAL("pscc"));
INST_END()

INST_ACTION_1_EXTEN(cpTRAPcc)
     struct {
	 unsigned short :4,
                    cpid:3,
	                :7,
                      op:3;
     } *i = (typeof (i))&inst;
     struct {
	 unsigned short :10,
                      cc:6;
     } *e = (typeof (e))&exten;
     struct cell data;
     if (i->cpid)
         FATAL("priv fault cpTRAPcc not pmmu");
     DEBUG_INST_HALT(if (i->op == 4)
 		         printf("\tptrap%s", pmmu_cond[e->cc]);
                     else if (i->op == 2) {
			 data.c_hiword = 0;
			 FETCH_INST(data.c_loword);
			 printf("\tptrap%s #0x%x",
				pmmu_cond[e->cc],
				data.c_longword);
		     } else if (i->op == 3) {
			 FETCH_INST(data.c_hiword);
			 FETCH_INST(data.c_loword);
			 printf("\tptrap%s #0x%x",
				pmmu_cond[e->cc],
				data.c_longword);
		     });
     INST_EMUL(FATAL("ptrapcc"));
INST_END()
