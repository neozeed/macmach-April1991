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
 *	File: mac2emul/macinst_subr.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mac2/reg.h>
#include <mach/mac2/frame.h>

#include <mac2emul/macinst.h>
#include <mac2emul/macuser.h>

/*
 * Privileged instruction emulation
 * support.
 */

/*
 * Return the next
 * word from the instruction
 * stream and store it in
 * the variable x.
 */
#define FETCH_INST(x)	FETCH_INCR(*apc, &(x), unsigned short)

/*
 * Return the current
 * value of the PC
 */
#define PC_VALUE	((int)(*apc))

#define FETCH_BYTE(p, x)	FETCH((p), &(x), unsigned char)
#define STORE_BYTE(p, x)	STORE((p), &(x), unsigned char)

#define FETCH_WORD(p, x)	FETCH((p), &(x), unsigned short)
#define STORE_WORD(p, x)	STORE((p), &(x), unsigned short)

#define FETCH_LONG(p, x)	FETCH((p), &(x), unsigned long)
#define STORE_LONG(p, x)	STORE((p), &(x), unsigned long)

int inst_andi_sr(), inst_eori_sr(), inst_ori_sr();
int inst_move_f_sr(), inst_move_t_sr();
int inst_move_usp();
int inst_movec(), inst_moves();
int inst_reset(), inst_rte(), inst_stop();
int inst_cpBcc(), inst_cpDBcc(), inst_cpGEN();
int inst_cpRESTORE(), inst_cpSAVE();
int inst_cpScc(), inst_cpTRAPcc();

int (*priv_inst_action[])() = {
    inst_andi_sr,
    inst_eori_sr,
    inst_ori_sr,
    inst_move_f_sr,
    inst_move_t_sr,
    inst_move_usp,
    inst_movec,
    inst_moves,
    inst_reset,
    inst_rte,
    inst_stop,
    inst_cpBcc,
    inst_cpDBcc,
    inst_cpGEN,
    inst_cpRESTORE,
    inst_cpSAVE,
    inst_cpScc,
    inst_cpTRAPcc,
};

int priv_inst_count[18];

/*
 * Handle a privileged
 * instruction trap.
 */
macpriv_inst(frame, regs)
register normal_exception_frame_t *frame;
register struct regs *regs;
{
    unsigned short inst;
    register group;
    
    /*
     * fetch the instruction
     * word and increment the pc
     */
    FETCH_INCR(frame->f_pc, &inst, unsigned short);
    group = get_priv_inst_group(inst);
    
    if (group < 0)
	return (0);

    priv_inst_count[group]++;
    return ((*priv_inst_action[group])(inst, frame, regs));
}

/*
 * gross hack used to
 * classify instructions.
 * optimized for fewest
 * compares for typical
 * instructions.
 */
get_priv_inst_group(inst)
register unsigned short inst;
{
    register bits;
    
    bits = (inst&DIGIT_0_MASK);
    
    if (bits == 0x0000) {
	bits = (inst&DIGIT_1_MASK);
	
	if (bits == 0x0000)
	    return (INST_ORI_SR);
	
	if (bits == 0x0200)
	    return (INST_ANDI_SR);
	
	if (bits == 0x0a00)
	    return (INST_EORI_SR);
	
	return (INST_MOVES);
    }
    
    if (bits == 0x4000) {
	bits = (inst&DIGIT_1_MASK);
	
	if (bits == 0x0000)
	    return (INST_MOVE_F_SR);
	
	if (bits == 0x0600)
	    return (INST_MOVE_T_SR);
	
	bits = (inst&DIGIT_2_MASK);
	
	if (bits == 0x0060)
	    return (INST_MOVE_USP);
	
	bits = (inst&DIGIT_3_MASK);
	
	if (bits > 3)
	    return (INST_MOVEC);

	if (bits == 3)
	    return (INST_RTE);
	
	if (bits == 0x0000)
	    return (INST_RESET);
	
	return (INST_STOP);
    }
    
    if (bits == 0xf000) {
	bits = (inst&0x0180)>>7;
	
	if (bits == 0) {
	    
	    if ((inst&0x0040) == 0)
		return (INST_cpGEN);
	    
	    bits = (inst&0x0078)>>3;
	    
	    if (bits == 0x0009)
		return (INST_cpDBcc);
	    
	    if (bits == 0x000f)
		return (INST_cpTRAPcc);
	    
	    return (INST_cpScc);
	}
	
	if (bits == 1)
	    return (INST_cpBcc);
	
	if (inst&0x0040)
	    return (INST_cpRESTORE);
	    
	return (INST_cpSAVE);
    }
    
    return (-1);
}

#ifndef KERNEL
char eam_iregtyp[] = { 'D', 'A' };
char eam_isize[] = { 'W', 'L' };
char eam_scale[] = { '1', '2', '4', '8' };

print_addr_mode(mode, reg, size, apc)
register mode, reg, size;
register unsigned short **apc;
{
    register struct exten_brief *extb;
    register struct exten_full *extf;
    unsigned short exten;
    struct cell disp;
    
    switch (mode) {
      case 0:
	printf("D%x", reg);
	break;
	
      case 1:
	printf("A%x", reg);
	break;
	
      case 2:
	printf("(A%x)", reg);
	break;
	
      case 3:
	printf("(A%x)+", reg);
	break;
	
      case 4:
	printf("-(A%x)", reg);
	break;
	
      case 5:
	FETCH_INST(exten);
	printf("(0x%x, A%x)", exten, reg);
	break;
	
      case 6:
      PCspecial:
	FETCH_INST(exten);
	extb = (struct exten_brief *)&exten;
	if (extb->ext_full == 0) {
	    if (mode == 7)
		printf("(0x%x, PC, %c%x.%c*%c)",
		       (char)(exten&255),
		       eam_iregtyp[extb->ext_iregtyp],
		       extb->ext_ireg,
		       eam_isize[extb->ext_isize],
		       eam_scale[extb->ext_scale]);
	    else
		printf("(0x%x, A%x, %c%x.%c*%c)",
		       (char)(exten&255),
		       reg,
		       eam_iregtyp[extb->ext_iregtyp],
		       extb->ext_ireg,
		       eam_isize[extb->ext_isize],
		       eam_scale[extb->ext_scale]);
	} else {
	    register flags = 0;
#define F_MEMIND  0x01
#define F_PRENDX  0x02
#define F_OUTDISP 0x04
#define F_FOUND   0x80
	    
	    extf = (struct exten_full *)&exten;
	    if (extf->ext_isel)
		flags |= F_MEMIND;
	    if ((flags&F_MEMIND) && extf->ext_isel < 4)
		flags |= F_PRENDX;
	    if ((extf->ext_isel&3) > 1)
		flags |= F_OUTDISP;
	    if (extf->ext_bdsize != 1) {
		printf("(");
		if (flags&F_MEMIND)
		    printf("[");
		flags |= F_FOUND;
		switch (extf->ext_bdsize) {
		  case 2:
		    FETCH_INST(disp.c_loword);
		    printf("0x%x", (short)disp.c_loword);
		    break;
		    
		  case 3:
		    FETCH_INST(disp.c_hiword);
		    FETCH_INST(disp.c_loword);
		    printf("0x%x", disp.c_longword);
		    break;
		}
	    }
	    if (extf->ext_bs == 0) {
		if ((flags&F_FOUND) == 0) {
		    printf("(");
		    if (flags&F_MEMIND)
			printf("[");
		    flags |= F_FOUND;
		} else
		    printf(", ");
		if (mode == 7)
		    printf("PC");
		else
		    printf("A%x", reg);
	    }
	    if (extf->ext_is == 0) {
		if ((flags&F_FOUND) == 0) {
		    printf("(");
		    flags |= F_FOUND;
		    if ((flags&(F_MEMIND|F_PRENDX)) == (F_MEMIND|F_PRENDX))
			printf("[");
		} else if ((flags&F_PRENDX) == 0)
		    printf("], ");
		else
		    printf(", ");
		printf("%c%x.%c*%c",
		       eam_iregtyp[extf->ext_iregtyp],
		       extf->ext_ireg,
		       eam_isize[extf->ext_isize],
		       eam_scale[extf->ext_scale]);
		if (flags&F_PRENDX)
		    printf("]");
	    }
	    if (flags&F_OUTDISP) {
		if ((flags&F_FOUND) == 0) {
		    printf("(");
		    flags |= F_FOUND;
		} else
		    printf(", ");
		switch (extf->ext_isel&3) {
		  case 2:
		    FETCH_INST(disp.c_loword);
		    printf("0x%x", (short)disp.c_loword);
		    break;
		    
		  case 3:
		    FETCH_INST(disp.c_hiword);
		    FETCH_INST(disp.c_loword);
		    printf("0x%x", disp.c_longword);
		    break;
		}
	    }
	    if (flags&F_FOUND)
		printf(")");
	    else
		printf("0");
	}
#undef F_MEMIND
#undef F_PRENDX
#undef F_OUTDISP
#undef F_FOUND
	break;
	
      case 7:
	switch (reg) {
	  case 0:
	    disp.c_hiword = 0;
	    FETCH_INST(disp.c_loword);
	    printf("0x%x.W", disp.c_longword);
	    break;
	    
	  case 1:
	    FETCH_INST(disp.c_hiword);
	    FETCH_INST(disp.c_loword);
	    printf("0x%x.L", disp.c_longword);
	    break;
	    
	  case 4:
	    {
		struct cell hilong, lolong;
		
		switch (size) {
		  case OPSIZE_BYTE:
		  case OPSIZE_WORD:
		    hilong.c_hiword = 0;
		    FETCH_INST(hilong.c_loword);
		    printf("#0x%x", hilong.c_longword);
		    break;
		    
		  case OPSIZE_LONG:
		    FETCH_INST(hilong.c_hiword);
		    FETCH_INST(hilong.c_loword);
		    printf("#0x%x", hilong.c_longword);
		    break;
		    
		  case OPSIZE_DBLONG:
		    FETCH_INST(hilong.c_hiword);
		    FETCH_INST(hilong.c_loword);
		    FETCH_INST(lolong.c_hiword);
		    FETCH_INST(lolong.c_loword);
		    printf("#0x%08x%08x",
			   hilong.c_longword,
			   lolong.c_longword);
		    break;
		}
	    }
	    break;
	    
	  case 2:
	    FETCH_INST(exten);
	    printf("(0x%x, PC)", exten);
	    break;
	    
	  case 3:
	    goto PCspecial;
	}
	break;
    }
}
#endif

/*
 * fetch_operand is used for instruction
 * emulation.  it fetches the operand
 * based on the effective addressing
 * mode, and returns the operand value
 * in the memory cell pointed at by opnd.
 */
fetch_operand(opnd, mode, reg, size, apc, aregs)
register struct cell *opnd;
register mode, reg, size;
register unsigned short **apc;
register struct regs *aregs;
{
    switch (mode) {
      case 0:
      case 1:
	/*
	 * Register Direct modes
	 */
	{
	    register struct cell *greg;
	    
	    if (mode == 0)
		greg = (struct cell *)&aregs->r_dreg[reg];
	    else
		greg = (struct cell *)&aregs->r_areg[reg];
	    
	    switch (size) {
	      case OPSIZE_BYTE:
		opnd->c_lsbyte = greg->c_lsbyte;
		break;
		
	      case OPSIZE_WORD:
		opnd->c_loword = greg->c_loword;
		break;
		
	      case OPSIZE_LONG:
		opnd->c_longword = greg->c_longword;
		break;
	    }
	    break;
	}
	
      default:
	/*
	 * Immediate mode
	 */
	if (mode == 7 && reg == 4) {
	    switch (size) {
	      case OPSIZE_BYTE:
	      case OPSIZE_WORD:
		FETCH_INST(opnd->c_loword);
		break;
		
	      case OPSIZE_LONG:
		FETCH_INST(opnd->c_hiword);
		FETCH_INST(opnd->c_loword);
		break;
		
	      case OPSIZE_DBLONG:
		FETCH_INST(opnd->c_hiword);
		FETCH_INST(opnd->c_loword);
		FETCH_INST((opnd+1)->c_hiword);
		FETCH_INST((opnd+1)->c_loword);
		break;
	    }
	} else {
	    /*
	     * All others
	     */
	    struct cell *m;
	    
	    calc_ea(&m, mode, reg, size, apc, aregs);
	    switch (size) {
	      case OPSIZE_BYTE:
		FETCH_BYTE(&m->c_byte, opnd->c_lsbyte);
		break;
		
	      case OPSIZE_WORD:
		FETCH_WORD(&m->c_word, opnd->c_loword);
		break;
		
	      case OPSIZE_LONG:
		FETCH_LONG(&m->c_longword, opnd->c_longword);
		break;
		
	      case OPSIZE_DBLONG:
		FETCH_LONG(&m->c_longword, opnd->c_longword);
		FETCH_LONG(&(m+1)->c_longword, (opnd+1)->c_longword);
		break;
	    }
	}
    }
}

/*
 * store_operand is used for instruction
 * emulation.  a pointer to the operand
 * is passed in opnd.  the operand is stored
 * based on the effective addressing mode.
 */
store_operand(opnd, mode, reg, size, apc, aregs)
register struct cell *opnd;
register mode, reg, size;
register unsigned short **apc;
register struct regs *aregs;
{
    switch (mode) {
      case 0:
      case 1:
	/*
	 * Register Direct modes
	 */
	{
	    register struct cell *greg;
	    
	    if (mode == 0)
		greg = (struct cell *)&aregs->r_dreg[reg];
	    else
		greg = (struct cell *)&aregs->r_areg[reg];
	    
	    switch (size) {
	      case OPSIZE_BYTE:
		greg->c_lsbyte = opnd->c_lsbyte;
		break;
		
	      case OPSIZE_WORD:
		greg->c_loword = opnd->c_loword;
		break;
		
	      case OPSIZE_LONG:
		greg->c_longword = opnd->c_longword;
		break;
	    }
	    break;
	}
	
      default:
	/*
	 * All others
	 */
	{
	    struct cell *m;
	    
	    calc_ea(&m, mode, reg, size, apc, aregs);
	    switch (size) {
	      case OPSIZE_BYTE:
		STORE_BYTE(&m->c_byte, opnd->c_lsbyte);
		break;
		
	      case OPSIZE_WORD:
		STORE_WORD(&m->c_word, opnd->c_loword);
		break;
		
	      case OPSIZE_LONG:
		STORE_LONG(&m->c_longword, opnd->c_longword);
		break;
		
	      case OPSIZE_DBLONG:
		STORE_LONG(&m->c_longword, opnd->c_longword);
		STORE_LONG(&(m+1)->c_longword, (opnd+1)->c_longword);
		break;
	    }
	}
    }
}

/*
 * calc_ea is used to calculate the
 * address of an operand in memory
 * for all cases except immediate data.
 * the address of the operand is returned
 * to the cell pointer pointed at by aea.
 */
calc_ea(aea, mode, reg, size, apc, aregs)
struct cell **aea;
int mode, size;
register reg;
register unsigned short **apc;
register struct regs *aregs;
{
    register struct exten_full *extf;
    register struct exten_brief *extb;
    register unsigned long temp, index;
    register e_pc;
    unsigned long indir;
    unsigned short exten;
    struct cell disp;
    
    switch (mode) {
	/*
	 * Address register indirect
	 */
      case 2:
	*aea = (struct cell *)aregs->r_areg[reg];
	break;
	
	/*
	 * Address register indirect
	 * with postincrement
	 */
      case 3:
	*aea = (struct cell *)aregs->r_areg[reg];
	switch (size) {
	  case OPSIZE_BYTE:
	    aregs->r_areg[reg]++;
	    break;
	    
	  case OPSIZE_WORD:
	    aregs->r_areg[reg] += 2;
	    break;
	    
	  case OPSIZE_LONG:
	    aregs->r_areg[reg] += 4;
	    break;
	    
	  case OPSIZE_DBLONG:
	    aregs->r_areg[reg] += 8;
	    break;
	}
	break;
	
	/*
	 * Address register indirect
	 * with predecrement
	 */
      case 4:
	switch (size) {
	  case OPSIZE_BYTE:
	    aregs->r_areg[reg]--;
	    break;
	    
	  case OPSIZE_WORD:
	    aregs->r_areg[reg] -= 2;
	    break;
	    
	  case OPSIZE_LONG:
	    aregs->r_areg[reg] -= 4;
	    break;
	    
	  case OPSIZE_DBLONG:
	    aregs->r_areg[reg] -= 8;
	    break;
	}
	*aea = (struct cell *)aregs->r_areg[reg];
	break;
	
	/*
	 * Address register indirect
	 * with displacment
	 */
      case 5:
	temp = aregs->r_areg[reg];
	FETCH_INST(disp.c_loword);
	temp += (short)disp.c_loword;
	*aea = (struct cell *)temp;
	break;
	
	/*
	 * Address register indirect
	 * with index
	 */
      case 6:
      PCspecial:
	e_pc = PC_VALUE;
	FETCH_INST(exten);
	extb = (struct exten_brief *)&exten;
	if (extb->ext_full == 0) {
	    /*
	     * Index with 8 bit displacement
	     */
	    if (mode == 7)
		temp = e_pc;
	    else
		temp = aregs->r_areg[reg];
	    temp += (char)(extb->ext_disp);
	    index = (extb->ext_iregtyp? aregs->r_areg[reg]: aregs->r_dreg[reg]);
	    if (extb->ext_isize == 0)
		index = (short)index;
	    if (extb->ext_scale)
		index *= (1<<extb->ext_scale);
	    temp += index;
	} else {
	    /*
	     * Index with base and outer
	     * displacements
	     */
	    register flags = 0;
#define F_MEMIND  0x01
#define F_PRENDX  0x02
#define F_OUTDISP 0x04
	    temp = 0;
	    extf = (struct exten_full *)&exten;
	    if (extf->ext_isel)
		flags |= F_MEMIND;
	    if ((flags&F_MEMIND) && extf->ext_isel < 4)
		flags |= F_PRENDX;
	    if ((extf->ext_isel&3) > 1)
		flags |= F_OUTDISP;
	    if (extf->ext_bs == 0) {
		if (mode == 7)
		    temp = e_pc;
		else
		    temp = aregs->r_areg[reg];
	    }
	    switch (extf->ext_bdsize) {
	      case 2:
		FETCH_INST(disp.c_loword);
		temp += (short)disp.c_loword;
		break;
		
	      case 3:
		FETCH_INST(disp.c_hiword);
		FETCH_INST(disp.c_loword);
		temp += disp.c_longword;
		break;
	    }
	    if ((flags&(F_MEMIND|F_PRENDX)) == F_MEMIND) {
		FETCH_LONG(temp, indir);
		temp = indir;
	    }
	    if (extf->ext_is == 0) {
		index = (extf->ext_iregtyp? aregs->r_areg[reg]: aregs->r_dreg[reg]);
		if (extf->ext_isize == 0)
		    index = (short)(index&0xffff);
		if (extf->ext_scale)
		    index *= (1<<extf->ext_scale);
		temp += index;
	    }
	    if ((flags&(F_MEMIND|F_PRENDX)) == (F_MEMIND|F_PRENDX)) {
		FETCH_LONG(temp, indir);
		temp = indir;
	    }
	    if (flags&F_OUTDISP) {
		switch (extf->ext_isel&3) {
		  case 2:
		    FETCH_INST(disp.c_loword);
		    temp += (short)disp.c_loword;
		    break;
		    
		  case 3:
		    FETCH_INST(disp.c_hiword);
		    FETCH_INST(disp.c_loword);
		    temp += disp.c_longword;
		    break;
		}
	    }
	}
#undef F_MEMIND
#undef F_PRENDX
#undef F_OUTDISP
	*aea = (struct cell *)temp;
	break;
	
      case 7:
	switch (reg) {
	  case 0:
	    FETCH_INST(disp.c_loword);
	    temp = (short)disp.c_loword;
	    *aea = (struct cell *)temp;
	    break;
	    
	  case 1:
	    FETCH_INST(disp.c_hiword);
	    FETCH_INST(disp.c_loword);
	    *aea = (struct cell *)disp.c_longword;
	    break;
	    
	  case 2:
	    temp = PC_VALUE;
	    FETCH_INST(disp.c_loword);
	    temp += (short)disp.c_loword;
	    *aea = (struct cell *)temp;
	    break;
	    
	  case 3:
	    goto PCspecial;
	}
	break;
    }
}

fetch_frestore_operand(fp, mode, reg, apc, aregs)
register fp_frame_t *fp;
register mode, reg;
register unsigned short **apc;
register struct regs *aregs;
{
    struct cell *m;

    calc_ea(&m, mode, reg, OPSIZE_LONG, apc, aregs);
    FETCH(&m->c_longword, fp, unsigned long);
    FETCH_N((m+1), &fp->fpf_data, fp->fpf_size);
    if (mode == 3)
	aregs->r_areg[reg] += fp->fpf_size;
}

store_fsave_operand(fp, mode, reg, apc, aregs)
register fp_frame_t *fp;
register mode, reg;
register unsigned short **apc;
register struct regs *aregs;
{
    struct cell *m;

    if (mode == 4)
	aregs->r_areg[reg] -= (fp->fpf_size+4);
    calc_ea(&m, mode, reg, OPSIZE_NONE, apc, aregs);
    STORE_N(m, fp, fp->fpf_size+4);
}

sign_extend(opnd, size)
register struct cell *opnd;
register size;
{
    switch (size) {
      case OPSIZE_BYTE:
	opnd->c_longword = (char)opnd->c_lsbyte;
	break;
	
      case OPSIZE_WORD:
	opnd->c_longword = (short)opnd->c_loword;
	break;
    }
}
