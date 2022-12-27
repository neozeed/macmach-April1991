/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_exception.c,v $
 * Revision 1.7  90/03/21  16:12:00  mrt
 * 		Picked up i386 stuff
 * 		Add mach_mmax_exception() and mach_mmax_exception_string().
 * 	[90/03/21            mrt]
 * 
 * Revision 1.6  90/03/08  11:14:22  mrt
 * 	Added mach_mips_exception() and mach_mips_exception_string().
 * 	Corrected the returned string to be in malloc'ed memory instead
 * 	of on the stack.
 * 	[90/03/07            af]
 * 
 * Revision 1.5  90/03/05  18:30:49  mrt
 * 	Added mach_sun4_exception() and mach_sun4_exception_string().
 * 	[90/02/20            jjc]
 * 
 * Revision 1.4  89/10/31  23:45:43  mrt
 * 	Included <exeption_all.h> instead of
 * 	mach/exception.h, mach/ca/exception.h, mach/sun3/exception.h
 * 	and mach/vax/exception.h.
 * 	[89/10/28            mrt]
 * 
 * Revision 1.3  89/05/05  18:45:05  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 12-May-88  Mary R. Thompson (mrt) at Carnegie Mellon
 *	added inlcude of <mach_error.h> for mach_error_string.
 * 29-Mar-88  Karl Hauth (hauth) at Carnegie-Mellon University
 *	Created from mach_error.c
 */
/*
 *	File:	mach_exception.c
 *	Author:	Karl Hauth
 *
 *	Standard error printing routines for Mach errors.
 */

#ifndef mac2_mpw_drvr

#include <stdio.h>
#include <mach.h>
#include <mach/message.h>
#include <mig_errors.h>
#include <servers/netname.h>
#include <servers/env_mgr.h>
#include <servers/ipcx_types.h>
#include <exception_all.h>
#include <mach_exception.h>
#include <mach_error.h>

char *mach_exception_string(exception)
	int exception;
{
	switch (exception) {
	case EXC_BAD_ACCESS:
		return("Memory Access");

	case EXC_BAD_INSTRUCTION:
		return("Illegal Instruction");

	case EXC_ARITHMETIC:
		return("Arithmetic");

	case EXC_EMULATION:
		return("Emulation");

	case EXC_SOFTWARE:
		return("Software");

	case EXC_BREAKPOINT:
		return("Breakpoint");

	default:
		return("Unknown exception");
	}
}

void mach_exception(str, exception)
	char *str;
	int exception;
{
	char	*msg;

	msg = mach_exception_string(exception);
	fprintf(stderr, "%s: %s(%d)\n", str, msg, exception);
}

char *mach_sun3_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);

	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_SUN3_ILLEGAL_INSTRUCTION:
			return("Illegal or undefined instruction or operand");

		case EXC_SUN3_PRIVILEGE_VIOLATION:
			return("Illegal instruction, privilege violation");

		case EXC_SUN3_COPROCESSOR:
			return("Illegal instruction, coprocessor");

		case EXC_SUN3_TRAP1:
			return("Illegal instruction, trap 1");

		case EXC_SUN3_TRAP2:
			return("Illegal instruction, trap 2");

		case EXC_SUN3_TRAP3:
			return("Illegal instruction, trap 3");

		case EXC_SUN3_TRAP4:
			return("Illegal instruction, trap 4");

		case EXC_SUN3_TRAP5:
			return("Illegal instruction, trap 5");

		case EXC_SUN3_TRAP6:
			return("Illegal instruction, trap 6");

		case EXC_SUN3_TRAP7:
			return("Illegal instruction, trap 7");

		case EXC_SUN3_TRAP8:
			return("Illegal instruction, trap 8");

		case EXC_SUN3_TRAP9:
			return("Illegal instruction, trap 9");

		case EXC_SUN3_TRAP10:
			return("Illegal instruction, trap 10");

		case EXC_SUN3_TRAP11:
			return("Illegal instruction, trap 11");

		case EXC_SUN3_TRAP12:
			return("Illegal instruction, trap 12");

		case EXC_SUN3_TRAP13:
			return("Illegal instruction, trap 13");

		case EXC_SUN3_TRAP14:
			return("Illegal instruction, trap 14");

		case EXC_SUN3_FLT_BSUN:
			return("Illegal instruction, code == FLT_BSUN");

		case EXC_SUN3_FLT_OPERAND_ERROR:
			return("Illegal instruction, illegal floating operand");

		default:
			return("Illegal or undefined instruction or operand");
		}


	case EXC_ARITHMETIC:
		switch (code) {
		case EXC_SUN3_ZERO_DIVIDE:
			return("Arithmetic exception, divide by zero");

		case EXC_SUN3_FLT_INEXACT:
			return("Arithmetic exception, floating inexact");

		case EXC_SUN3_FLT_ZERO_DIVIDE:
			return("Arithmetic exception, floating divide by zero");

		case EXC_SUN3_FLT_UNDERFLOW:
			return("Arithmetic exception, floating underflow");

		case EXC_SUN3_FLT_OVERFLOW:
			return("Arithmetic exception, floating overflow");

		case EXC_SUN3_FLT_NOT_A_NUMBER:
			return("Arithmetic exception, not a floating number");

		default:
			return("Arithmetic error with undefined code");
		}

	case EXC_EMULATION:
		switch (code) {
		case EXC_SUN3_LINE_1010:
			return("Emulation exception, line 1010");

		case EXC_SUN3_LINE_1111:
			return("Emulation exception, line 1111");

		default:
			return("Emulation error with undefined code");
		}

	case EXC_SOFTWARE:
		switch (code) {
		case EXC_SUN3_CHK:
			return("Software exception, CHK");

		case EXC_SUN3_TRAPV:
			return("Software exception, TRAPV");

		default:
			return("Software generated exception (undefigned type)");
		}

	case EXC_BREAKPOINT:
		switch (code) {
		case EXC_SUN3_TRACE:
			return("Breakpoint exception, trace");

		case EXC_SUN3_BREAKPOINT:
			return("Breakpoint exception, breakpoint");

		default:
			return("Breakpoint exception, undefined code.");
		}

	default:
		return("unknown exception");
	}
}


void mach_sun3_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_sun3_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}


char *mach_romp_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);
	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_ROMP_PRIV_INST:
			return("Bad instruction, privileged");

		case EXC_ROMP_ILLEGAL_INST:
			return("Bad instruction, illegal");

		default:
			return("Bad instruction, undefined exception code");
		}

	case EXC_BREAKPOINT:
		switch (code) {
		case EXC_ROMP_TRAP_INST:
			return("Breakpoint exception, trap");

		case EXC_ROMP_INST_STEP:
			return("Breakpoint exception, step");

		default:
			return("Breakpoint exception, undefined exception code");
		}

	case EXC_ARITHMETIC:
		switch (code) {
		case EXC_ROMP_FPA_EMUL:
			return("Arithmetic exception, FPA emulation");

		case EXC_ROMP_68881:
			return("Arithmetic exception, 68881");

		case EXC_ROMP_68881_TIMEOUT:
			return("Arithmetic exception, 68881 timeout");

		case EXC_ROMP_FLOAT_SPEC:
			return("Arithmetic exception, floating specification");

		default:
			return("Arithmetic exception, undefined exception code");
		}

	case EXC_SOFTWARE:
		return("Software exception.");

	case EXC_EMULATION:
		return("Emulation exception.");

	default:
		return("Undefined exception code");
	}
}


void mach_romp_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_romp_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}

char *mach_vax_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);

	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_VAX_PRIVINST:
			return("Bad Instruction exception, privileged instruction");

		case EXC_VAX_RESOPND:
			return("Bad Instruction exception, reserved operand");

		case EXC_VAX_RESADDR:
			return("Bad Instruction exception, reserved address");

		case EXC_VAX_COMPAT:
			switch (subcode) {
				case EXC_VAX_COMPAT_RESINST:
					return("Bad Instruction exception, compat, reserved instruction");

				case EXC_VAX_COMPAT_BPT:
					return("Bad Instruction exception, compat, breakpoint");

				case EXC_VAX_COMPAT_IOT:
					return("Bad Instruction exception, compat, IOT");

				case EXC_VAX_COMPAT_EMT:
					return("Bad Instruction exception, compat, emulation");
				case EXC_VAX_COMPAT_TRAP:
					return("Bad Instruction exception, compat, trap");

				case EXC_VAX_COMPAT_RESOP:
					return("Bad Instruction exception, compat, reserved operation");

				case EXC_VAX_COMPAT_ODDADDR:
					return("Bad Instruction exception, compat, odd address");

				default:
					return("Bad Instruction exception, compat, invalid exception subcode");
			}

		default:
			return("Bad Instruction exception, invalid exception code");
		}

	case EXC_ARITHMETIC:
		switch (code) {

		case EXC_VAX_INT_OVF:
			return("Arithmetic exception, integer overflow");

		case EXC_VAX_INT_DIV:
			return("Arithmetic exception, integer division");

		case EXC_VAX_FLT_OVF_T:
			return("Arithmetic exception, floating overflow T");

		case EXC_VAX_FLT_DIV_T:
			return("Arithmetic exception, floating division T");

		case EXC_VAX_FLT_UND_T:
			return("Arithmetic exception, floating underflow T");

		case EXC_VAX_DEC_OVF:
			return("Arithmetic exception, decimal overflow T");

		case EXC_VAX_FLT_OVF_F:
			return("Arithmetic exception, floating overflow F");

		case EXC_VAX_FLT_DIV_F:
			return("Arithmetic exception, floating division F");

		case EXC_VAX_FLT_UND_F:
			return("Arithmetic exception, floating underflow F");

		default:
			return("Arithmetic exception, undefined exception code");
		}

	case EXC_SOFTWARE:
		switch (code) {
		case EXC_VAX_SUB_RNG:
			return("Software exception, subrange");

		default:
			return("Software exception, undefined exception code");
		}

	case EXC_EMULATION:
		return("Emulation exception.");

	case EXC_BREAKPOINT:
		switch (code) {

		case EXC_VAX_BPT:
			return("Breakpoint exception, breakpoint");

		case EXC_VAX_TRACE:
			return("Breakpoint exception, trace");

		default:
			return("Breakpoint exception, undefined exception code");
		}

	default:
		return("Undefined exception number");
	}
}

void mach_vax_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_vax_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}

char *mach_sun4_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);

	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_SUN4_WINDOW_OVERFLOW:
			return("User window overflow failed");

		case EXC_SUN4_PRIVILEGE_VIOLATION:
			return("Illegal instruction, privilege violation");

		case EXC_SUN4_UNIMPLEMENTED:
			return("Unimplemented instruction");

		default:
			buf = (char*)malloc(100);
			sprintf(buf, "Illegal or undefined instruction, trap 0x%x", code);
			return(buf);
		}


	case EXC_ARITHMETIC:
		switch (code) {
		case EXC_SUN4_ZERO_DIVIDE:
			return("Arithmetic exception, divide by zero");

		case EXC_SUN4_FLT_EXCEPTION:
			return("Arithmetic exception, floating point");

		default:
			return("Arithmetic error with undefined code");
		}

	case EXC_EMULATION:
		switch (code) {
		case EXC_SUN4_TAG_OVERFLOW:
			return("Emulation exception, tag overflow");

		default:
			return("Emulation error with undefined code");
		}

	case EXC_SOFTWARE:
		switch (code) {
		default:
			return("Software generated exception (undefined type)");
		}

	case EXC_BREAKPOINT:
		switch (code) {
		case EXC_SUN4_BREAKPOINT:
			return("Breakpoint exception, breakpoint");

		default:
			return("Breakpoint exception, undefined code.");
		}

	default:
		return("unknown exception");
	}
}


void mach_sun4_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_sun4_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}

char *mach_mips_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);

	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_MIPS_PRIVINST:
			return("Bad Instruction exception, privileged instruction");

		case EXC_MIPS_RESOPND:
			return("Bad Instruction exception, reserved operand");

		case EXC_MIPS_RESADDR:
			return("Bad Instruction exception, reserved address");

		default:
			return("Bad Instruction exception, invalid exception code");
		}

	case EXC_ARITHMETIC:
		switch (code) {

		case EXC_MIPS_FLT_UNIMP:
			return("Arithmetic exception, floating unimplemented T");

		case EXC_MIPS_FLT_INVALID:
			return("Arithmetic exception, floating invalid T");

		case EXC_MIPS_FLT_DIVIDE0:
			return("Arithmetic exception, floating zerodivide T");

		case EXC_MIPS_FLT_OVERFLOW:
			return("Arithmetic exception, floating overflow F");

		case EXC_MIPS_FLT_UNDERFLOW:
			return("Arithmetic exception, floating underflow F");

		case EXC_MIPS_FLT_INEXACT:
			return("Arithmetic exception, floating inexact F");

		default:
			return("Arithmetic exception, undefined exception code");
		}

	case EXC_SOFTWARE:
		switch (code) {
		case EXC_MIPS_SOFT_CPU:
			return("Software exception, coprocessor unusable");
#ifdef EXC_MIPS_SOFT_SEGV /* temp hack until kernel bug fix */
		case EXC_MIPS_SOFT_SEGV:
			buf = (char*)malloc(100);
			sprintf(buf,
				"Memory access exception, cause 0x%x", subcode);
			return(buf);
#endif
		default:
			return("Software exception, undefined exception code");
		}

	case EXC_EMULATION:
		return("Emulation exception.");

	case EXC_BREAKPOINT:
		switch (code) {

		case EXC_MIPS_BPT:
			return("Breakpoint exception, breakpoint");

		case EXC_MIPS_TRACE:
			return("Breakpoint exception, trace");

		default:
			return("Breakpoint exception, undefined exception code");
		}

	default:
		return("Undefined exception number");
	}
}

void mach_mips_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_mips_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}

char *mach_mmax_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);

	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_NS32K_FPU:
			switch(subcode) {
				case EXC_NS32K_FPU_ILLEGAL:
    return("Bad Instruction exception, illegal floating point instruction");

				case EXC_NS32K_FPU_INVALID:
    return("Bad Instruction exception, invalid floating point operation");

    				default:
    return("Bad Instruction exception, undefined floating point subcode");
			}

		case EXC_NS32K_ILL:
			return("Bad Instruction exception, illegal operation");

		case EXC_NS32K_UND:
			return("Bad Instruction exception, undefined opcode");

		default:
			return("Bad Instruction exception, undefined exception code");
		}

	case EXC_ARITHMETIC:
		switch (code) {

		case EXC_NS32K_FPU:
			switch(subcode) {
				case EXC_NS32K_FPU_UNDERFLOW:
		return("Arithmetic exception, floating point underflow");

				case EXC_NS32K_FPU_OVERFLOW:
		return("Arithmetic exception, floating point overflow");

				case EXC_NS32K_FPU_DVZ:
		return("Arithmetic exception, floating point divide by zero");

				case EXC_NS32K_FPU_INEXACT:
		return("Arithmetic exception, inexact floating point result");

				case EXC_NS32K_FPU_OPERAND:
		return("Arithmetic exception, floating point operand error");

				default:
		return("Arithmetic exception, undefined floating point subcode");
			}

		default:
			return("Arithmetic exception, undefined exception code");
		}

	case EXC_SOFTWARE:
		switch (code) {
		case EXC_NS32K_FLG:
			return("Software exception, FLAG instruction trap");
		default:
			return("Software exception, undefined exception code");
		}

	case EXC_EMULATION:
		return("Emulation exception.");

	case EXC_BREAKPOINT:
		switch (code) {

		case EXC_NS32K_BPT:
			return("Breakpoint exception, breakpoint");

		case EXC_NS32K_TRC:
			return("Breakpoint exception, trace trap");

		default:
			return("Breakpoint exception, undefined exception code");
		}

	default:
		return("Undefined exception number");
	}
}

void mach_mmax_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_mmax_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}

char *mach_i386_exception_string(exception, code, subcode)
	int exception, code, subcode;
{
	char	*buf;

	switch (exception) {
	case EXC_BAD_ACCESS:
		buf = (char*)malloc(100);
		if (!buf) {
			fprintf(stderr,
				"mach_i386_exception_string: no memory!\n");
			abort();
		}
		sprintf(buf,
			"Memory access exception on address 0x%x (%s)",
			subcode, mach_error_string(code));
		return(buf);		/* XXX - core leak */

	case EXC_BAD_INSTRUCTION:
		switch (code) {
		case EXC_I386_INVOP:
			return("Bad Instruction exception, invalid opcode");

		default:
			return("Bad Instruction exception, invalid exception code");
		}

	case EXC_ARITHMETIC:
		switch (code) {

		case EXC_I386_DIV:
			return("Arithmetic exception, divide error");
		case EXC_I386_INTO:
			return("Arithmetic exception, INTO overflow fault");
		case EXC_I386_NOEXT:
			return("Arithmetic exception, coprocessor not available fault");
		case EXC_I386_EXTOVR:
			return("Arithmetic exception, coprocessor overrun fault");
		case EXC_I386_EXTERR:
			return("Arithmetic exception, coprocessor error");
		case EXC_I386_EMERR:
			return("Arithmetic exception, emulated extension fault");
		case EXC_I386_BOUND:
			return("Arithmetic exception, BOUND instruction fault");
		default:
			return("Arithmetic exception, undefined exception code");
		}

	case EXC_SOFTWARE:
		return("Software exception, undefined exception code");

	case EXC_BREAKPOINT:
		switch (code) {

		  case EXC_I386_SGL:
			return("Breakpoint exception, trace");

  		  case EXC_I386_BPT:
			return("Breakpoint exception, breakpoint");

		  default:
			return("Breakpoint exception, undefined exception code");
		}

	default:
		return("Undefined exception number");
	}
}

void mach_i386_exception(str, exception, code, subcode)
	char *str;
	int exception, code, subcode;
{
	char	*msg;

	msg = mach_i386_exception_string(exception, code, subcode);
	fprintf(stderr, "%s: %s(%d %d %d)\n",
		str, msg, exception, code, subcode);
}

char *mach_mac2_exception_string(exception, code, subcode)
int exception, code, subcode;
{
  char      *buf;

  switch (exception) {
    case EXC_BAD_ACCESS:
      /* Could not access memory */
      /* Code contains kern_return_t describing error. */
      /* Subcode contains bad memory address. */
      buf = (char*)malloc(100);
      sprintf(buf, "Memory access exception on address 0x%x (%s)",
        subcode, mach_error_string(code));
      return buf;
    case EXC_BAD_INSTRUCTION:
      /* Instruction failed */
      /* Illegal or undefined instruction or operand */
      switch (code) {
        case EXC_MAC2_ILLEGAL_INSTRUCTION:
          return "Illegal or undefined instruction or operand";
        case EXC_MAC2_PRIVILEGE_VIOLATION:
          return "Illegal instruction, privilege violation";
        case EXC_MAC2_COPROCESSOR:
          return "Illegal instruction, coprocessor";
        case EXC_MAC2_TRAP1:
          return "Illegal instruction, trap 1";
        case EXC_MAC2_TRAP2:
          return "Illegal instruction, trap 2";
        case EXC_MAC2_TRAP3:
          return "Illegal instruction, trap 3";
        case EXC_MAC2_TRAP4:
          return "Illegal instruction, trap 4";
        case EXC_MAC2_TRAP5:
          return "Illegal instruction, trap 5";
        case EXC_MAC2_TRAP6:
          return "Illegal instruction, trap 6";
        case EXC_MAC2_TRAP7:
          return "Illegal instruction, trap 7";
        case EXC_MAC2_TRAP8:
          return "Illegal instruction, trap 8";
        case EXC_MAC2_TRAP9:
          return "Illegal instruction, trap 9";
        case EXC_MAC2_TRAP10:
          return "Illegal instruction, trap 10";
        case EXC_MAC2_TRAP11:
          return "Illegal instruction, trap 11";
        case EXC_MAC2_TRAP12:
          return "Illegal instruction, trap 12";
        case EXC_MAC2_TRAP13:
          return "Illegal instruction, trap 13";
        case EXC_MAC2_TRAP14:
          return "Illegal instruction, trap 14";
        case EXC_MAC2_FLT_BSUN:
          return "Illegal instruction, code == FLT_BSUN";
        case EXC_MAC2_FLT_OPERAND_ERROR:
          return "Illegal instruction, illegal floating operand";
        default:
          return "Illegal or undefined instruction or operand";
      }
    case EXC_ARITHMETIC:
      /* Arithmetic exception */
      /* Exact nature of exception is in code field */
      switch (code) {
        case EXC_MAC2_ZERO_DIVIDE:
          return "Arithmetic exception, divide by zero";
        case EXC_MAC2_FLT_INEXACT:
          return "Arithmetic exception, floating inexact";
        case EXC_MAC2_FLT_ZERO_DIVIDE:
          return "Arithmetic exception, floating divide by zero";
        case EXC_MAC2_FLT_UNDERFLOW:
          return "Arithmetic exception, floating underflow";
        case EXC_MAC2_FLT_OVERFLOW:
          return "Arithmetic exception, floating overflow";
        case EXC_MAC2_FLT_NOT_A_NUMBER:
          return "Arithmetic exception, not a floating number";
        default:
          return "Arithmetic error with undefined code";
      }
    case EXC_EMULATION:
      /* Emulation instruction */
      /* Emulation support instruction encountered */
      /* Details in code and subcode fields      */
      switch (code) {
        case EXC_MAC2_LINE_1010:
          return "Emulation exception, line 1010";
        case EXC_MAC2_LINE_1111:
          return "Emulation exception, line 1111";
        default:
          return "Emulation error with undefined code";
      }
    case EXC_SOFTWARE:
      /* Software generated exception */
      /* Exact exception is in code field. */
      /* Codes 0 - 0xFFFF reserved to hardware */
      /* Codes 0x10000 - 0x1FFFF reserved for OS emulation (Unix) */
      switch (code) {
        case EXC_MAC2_CHK:
          return "Software exception, CHK";
        case EXC_MAC2_TRAPV:
          return "Software exception, TRAPV";
        default:
          return "Software generated exception (undefigned type)";
      }
    case EXC_BREAKPOINT:
      /* Trace, breakpoint, etc. */
      /* Details in code field. */
      switch (code) {
        case EXC_MAC2_TRACE:
          return "Breakpoint exception, trace";
        case EXC_MAC2_BREAKPOINT:
          return "Breakpoint exception, breakpoint";
        default:
          return "Breakpoint exception, undefined code.";
      }
    default:
      return "unknown exception";
  }
}

void mach_mac2_exception(str, exception, code, subcode)
char *str;
int exception, code, subcode;
{
  char *msg;

  msg = mach_mac2_exception_string(exception, code, subcode);
  fprintf(stderr, "%s: %s(%d %d %d)\n", str, msg, exception, code, subcode);
}

#endif /* mac2_mpw_drvr */
