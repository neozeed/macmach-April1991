/* Definitions to make GDB run on a mips box under 4.3bsd.
   Copyright (C) 1986, 1987, 1989 Free Software Foundation, Inc.
   Contributed by Per Brothner(bothner@cs.wisc.edu) at U.Wisconsin
   and by Alessandro Forin(af@cs.cmu.edu) at CMU

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

#ifndef mips
#define mips
#endif

#ifndef	MACH
#define MACH 1
#endif	MACH

/* Define the bit, byte, and word ordering of the machine.  */
#ifdef  MIPSEB
#define BITS_BIG_ENDIAN
#define BYTES_BIG_ENDIAN
#define WORDS_BIG_ENDIAN
#endif

/* Floating point is IEEE compliant */
#define IEEE_FLOAT

/* wait.h */
#define HAVE_WAIT_STRUCT 1

/* we can do it */
#define ATTACH_DETACH 1

/* Get rid of any system-imposed stack limit if possible */

/*#define	SET_STACK_LIMIT_HUGE	no need to */

/* Define this if the C compiler puts an underscore at the front
   of external names before giving them to the linker.  */

/*#define NAMES_HAVE_UNDERSCORE*/

/* Debugger information will be in mips' format */

#define READ_MIPS_FORMAT

/* File format is coff, but with additions */

#define COFF_FORMAT

/* Use the symtab as much as possible, e.g. no misc_functions */

#define USE_SYMBOLS_FULLY

/* Only read in the global symbols in our psymtabs */

#define USE_ONLY_GLOBAL_PSYMS

/* Offset from address of function to start of its code.
   Zero on most machines.  */

#define FUNCTION_START_OFFSET 0

/* Advance PC across any function entry prologue instructions
   to reach some "real" code.  */

#define SKIP_PROLOGUE(pc)	pc = mips_skip_prologue(pc)

/* Immediately after a function call, return the saved pc.
   Can't always go through the frames for this because on some machines
   the new frame is not set up until the new function executes
   some instructions.  */

#define SAVED_PC_AFTER_CALL(frame)	read_register(31)

/* Are we currently handling a signal */

#define IN_SIGTRAMP(pc, name)	in_sigtramp(pc, name)

/* Address of end of stack space.  */

#define STACK_END_ADDR (0x7ffff000)

/* Stack grows downward.  */

#define INNER_THAN <

/* Sequence of bytes for breakpoint instruction.  */

#define BREAKPOINT {0xd, 0, 0x5, 0}

/* Amount PC must be decremented by after a breakpoint.
   This is often the number of bytes in BREAKPOINT
   but not always.  */

#define DECR_PC_AFTER_BREAK 0

/* Nonzero if instruction at PC is a return instruction. "j ra" on mips. */

#define ABOUT_TO_RETURN(pc) (read_memory_integer (pc, 4) == 0x3e00008)

/* Return 1 if P points to an invalid floating point value. */

#define INVALID_FLOAT(p,l)	isa_NAN(p,l)

/* Largest integer type */
#define LONGEST long

/* Name of the builtin type for the LONGEST type above. */
#define BUILTIN_TYPE_LONGEST builtin_type_long

/* Say how long (all) registers are.  */

#define REGISTER_TYPE long

/* Number of machine registers */

#define NUM_REGS 72

/* Initializer for an array of names of registers.
   There should be NUM_REGS strings in this initializer.  */

#define REGISTER_NAMES 	\
    {	"zero",	"at",	"v0",	"v1",	"a0",	"a1",	"a2",	"a3", \
	"t0",	"t1",	"t2",	"t3",	"t4",	"t5",	"t6",	"t7", \
	"s0",	"s1",	"s2",	"s3",	"s4",	"s5",	"s6",	"s7", \
	"t8",	"t9",	"k0",	"k1",	"gp",	"sp",	"s8",	"ra", \
	"sr",	"lo",	"hi",	"bad",	"cause","pc",    \
	"f0",   "f1",   "f2",   "f3",   "f4",   "f5",   "f6",   "f7", \
	"f8",   "f9",   "f10",   "f11",   "f12",   "f13",   "f14",   "f15", \
	"f16",   "f17",   "f18",   "f19",   "f20",   "f21",   "f22",   "f23",\
	"f24",   "f25",   "f26",   "f27",   "f28",   "f29",   "f30",   "f31",\
	"fsr",   "fir" \
    }

/* Register numbers of various important registers.
   Note that some of these values are "real" register numbers,
   and correspond to the general registers of the machine,
   and some are "phony" register numbers which are too large
   to be actual register numbers as far as the user is concerned
   but do serve to get the desired values when passed to read_register.  */

#define SP_REGNUM 29		/* Contains address of top of stack */
#define FP_REGNUM SP_REGNUM	/* None on mips: we fake it */
#define PC_REGNUM 37		/* Contains program counter */
#define PS_REGNUM 32		/* Contains processor status */

/* Total amount of space needed to store our copies of the machine's
   register state, the array `registers'.  */
#define REGISTER_BYTES (NUM_REGS*4)

/* Index within `registers' of the first byte of the space for
   register N.  */

#define REGISTER_BYTE(N) ((N) * 4)

/* Number of bytes of storage in the actual machine representation
   for register N.  On mips, all regs are 4 bytes.  */

#define REGISTER_RAW_SIZE(N) 4

/* Number of bytes of storage in the program's representation
   for register N.  On mips, all regs are 4 bytes.  */

#define REGISTER_VIRTUAL_SIZE(N) 4

/* Largest value REGISTER_RAW_SIZE can have.  */

#define MAX_REGISTER_RAW_SIZE 4

/* Largest value REGISTER_VIRTUAL_SIZE can have.  */

#define MAX_REGISTER_VIRTUAL_SIZE 4

/* Nonzero if register N requires conversion
   from raw format to virtual format.  */

#define REGISTER_CONVERTIBLE(N) 0

/* Convert data from raw format for register REGNUM
   to virtual format for register REGNUM.  */

#define REGISTER_CONVERT_TO_VIRTUAL(REGNUM,FROM,TO)	\
  bcopy ((FROM), (TO), 4);

/* Convert data from virtual format for register REGNUM
   to raw format for register REGNUM.  */

#define REGISTER_CONVERT_TO_RAW(REGNUM,FROM,TO)	\
  bcopy ((FROM), (TO), 4);

/* Return the GDB type object for the "standard" data type
   of data in register N.  */

#define REGISTER_VIRTUAL_TYPE(N) builtin_type_int
/* Store the address of the place in which to copy the structure the
   subroutine will return.  This is called from call_function. */

#define STORE_STRUCT_RETURN(addr, sp) \
  { sp = push_word(sp, addr);}

/* Extract from an array REGBUF containing the (raw) register state
   a function return value of type TYPE, and copy that, in virtual format,
   into VALBUF.  XXX floats */

#define EXTRACT_RETURN_VALUE(TYPE,REGBUF,VALBUF) \
  bcopy (REGBUF+REGISTER_BYTE (TYPE_CODE (TYPE) == TYPE_CODE_FLT ? 38 : 2), VALBUF, TYPE_LENGTH (TYPE))

/* Write into appropriate registers a function return value
   of type TYPE, given in virtual format.  */

#define STORE_RETURN_VALUE(TYPE,VALBUF) \
  write_register_bytes (REGISTER_BYTE (TYPE_CODE (TYPE) == TYPE_CODE_FLT ? 38 : 2), VALBUF, TYPE_LENGTH (TYPE))

/* Extract from an array REGBUF containing the (raw) register state
   the address in which a function should return its structure value,
   as a CORE_ADDR (or an expression that can be used as one).  */

#define EXTRACT_STRUCT_VALUE_ADDRESS(REGBUF) (*(int *)(REGBUF+16))

/* Structures are returned by ref in extra arg0 */
#define RETURN_STRUCT_BY_REF(code,block,value_type)	1

/* Compensate for lack of `vprintf' function.  */
#ifndef HAVE_VPRINTF
#define vprintf(format, ap) _doprnt (format, ap, stdout)
#endif /* not HAVE_VPRINTF */


/* Describe the pointer in each stack frame to the previous stack frame
   (its caller).  */

/* FRAME_CHAIN takes a frame's nominal address
   and produces the frame's chain-pointer.

   FRAME_CHAIN_COMBINE takes the chain pointer and the frame's nominal address
   and produces the nominal address of the caller frame.

   However, if FRAME_CHAIN_VALID returns zero,
   it means the given frame is the outermost one and has no caller.
   In that case, FRAME_CHAIN_COMBINE is not used.  */

/* In the case of MIPS, the frame's nominal address is the SP value plus
   the function's framesize.  The previous value is just not saved */

#define FRAME_CHAIN(thisframe)  (mips_frame_chain(thisframe->frame))

#define FRAME_CHAIN_VALID(chain, thisframe) \
  (chain != 0 && (outside_startup_file (FRAME_SAVED_PC (thisframe))))

#define FRAME_CHAIN_COMBINE(chain, thisframe) (chain)

/* Define other aspects of the stack frame.  */


/* A macro that tells us whether the function invocation represented
   by FI does not have a frame on the stack associated with it.  If it
   does not, FRAMELESS is set to 1, else 0.  */
/* We handle this differently for mips, and maybe we should not */

#define FRAMELESS_FUNCTION_INVOCATION(FI, FRAMELESS)  {(FRAMELESS) = 0;}

/* Saved Pc.  */

#define FRAME_SAVED_PC(fi)	(mips_frame_saved_pc(fi->frame))

#define FRAME_ARGS_ADDRESS(fi)	(fi)->frame

#define FRAME_LOCALS_ADDRESS(fi) (fi)->frame

/* Return number of args passed to a frame.
   Can return -1, meaning no way to tell.  */

#define FRAME_NUM_ARGS(num, fi)	(num = mips_frame_num_args(fi))

/* Return number of bytes at start of arglist that are not really args.  */

#define FRAME_ARGS_SKIP 0

/* Put here the code to store, into a struct frame_saved_regs,
   the addresses of the saved registers of frame described by FRAME_INFO.
   This includes special registers such as pc and fp saved in special
   ways in the stack frame.  sp is even more special:
   the address we return for it IS the sp for the next frame.  */

#define FRAME_FIND_SAVED_REGS(frame_info, frame_saved_regs)  \
		(mips_frame_find_saved_regs((frame_info),&(frame_saved_regs)))


/* Things needed for making the inferior call functions.  */

/* Stack must be aligned, at least on a word boundary.
   Or else trouble when passing shorts around */

#define STACK_ALIGN(addr)	(((addr)+3)&~3)

/* Push an empty stack frame, to record the current PC, etc.  */

#define PUSH_DUMMY_FRAME 	mips_push_dummy_frame()

/* Pop the above */

#define POP_DUMMY_FRAME		mips_pop_dummy_frame()

/* Discard from the stack the innermost frame, restoring all registers.  */

#define POP_FRAME		mips_pop_frame()

/* On MIPS, this sequence of words is the instructions
	j	ra		-- to stop raw backtrace
	nop
	addu	sp,?0		-- pseudo prologue
	sw	ra,?0(sp)	-- this is where we jump
	lw	a0,0(sp)	-- reload first 4 args
	lw	a1,4(sp)
	lw	a2,8(sp)
	lw	a3,12(sp)
	lwc1	$f12,0(sp)	-- just in case
	lwc1	$f13,4(sp)
	lwc1	$f14,8(sp)
	lwc1	$f15,12(sp)
	lui	AT,?0
	ori	AT,?0
	jalr	AT
	nop
	break	5
  */

#define CALL_DUMMY {0x03e00008, 0x00000000, 0x27bd0000, 0xafbf0000, \
		    0x8fa40000, 0x8fa50004, 0x8fa60008, 0x8fa7000c, \
		    0xc7ac0000, 0xc7ad0004, 0xc7ae0008, 0xc7af000c, \
		    0x3c010000, 0x34210000, 0x0020f809, 0x00000000, \
		    0x5000d}
#define CALL_DUMMY_START_OFFSET 12

/* Insert the specified number of args and function address
   into a call sequence of the above form stored at DUMMYNAME.  */

#define FIX_CALL_DUMMY_ALIGNED(dummyname, start_sp, fun, nargs, args,rettype)\
	mips_fix_call_dummy_a(dummyname, start_sp, fun, nargs, args, rettype)
#define FIX_CALL_DUMMY(dummyname, start_sp, fun, nargs, rettype)	\
	mips_fix_call_dummy(dummyname, start_sp, fun, nargs, rettype)

/* Specific information about a procedure.
   This is identical to the MIPS's PDR records,
   mipsread.c (ab)uses this to save memory */
typedef struct mips_extra_func_info {
	unsigned long	adr;	/* memory address of start of procedure */
	long	pad1;		/* isym: start of local symbol entries */
	long	pad2;		/* iline: start of line number entries*/
	long	regmask;	/* save register mask */
	long	regoffset;	/* save register offset */
	long	numargs;	/* number of args to procedure (was iopt) */
	long	fregmask;	/* save floating point register mask */
	long	fregoffset;	/* save floating point register offset */
	long	framesize;	/* frameoffset: frame size */
	short	framereg;	/* frame pointer register */
	short	pcreg;		/* offset or reg of return pc */
	long	lnLow;		/* lowest line in the procedure */
	long	lnHigh;		/* highest line in the procedure */
	long	pad3;		/* cbLineOffset: byte offset for this procedure from the fd base */
} *mips_extra_func_info_t;

#define EXTRA_FRAME_INFO struct mips_extra_func_info extra_info;

#define INIT_EXTRA_FRAME_INFO(fci) \
	if (fci->next == 0) validate_frame(fci)


/* Mach globals and hooks */

#include <mach.h>

/* Task port of our debugged inferior. */

extern task_t inferior_task;

/* Thread port of the current thread in the inferior. */

extern thread_t current_thread;

/* Exception port connected to our debugged inferior task. */

extern port_t inferior_exception_port;

/* What used to be our inferior's exception port */

extern port_t inferior_old_exception_port;

/* are we stopped by mach or ptrace? */

extern boolean_t stopped_in_ptrace;

/* interface to catch_exception_raise */

extern int stop_exception, stop_code, stop_subcode;

/* Did WE stop all of the other threads before we did the 
   task_resume?.  Used for single-stepping. */

extern boolean_t other_threads_suspended;

/* [un]restrict the exception port we're waiting on.  Used for 
   single-stepping. */

void set_exception_port(), clear_exception_port();

/* Get relevant ports when creating a new inferior */

#define CREATE_INFERIOR_HOOK(pid)	mach_create_inferior_hook(pid)

/* Stop waiting for the inferior to change state */

#define WAIT_HOOK(pid,w)	pid = mach_really_wait(&w);

/* I-Cache flushing */

#define FLUSH_INFERIOR_CACHE(addr,size)	flush_inferior_icache(addr,size)

/* Which function pretty-prints an exception value */

#define MACH_ARCH_EXCEPTION_STRING	mach_mips_exception_string

/* Thread flavors used in re-setting the T bit */

#define TBIT_FLAVOR		MIPS_EXC_STATE
#define TBIT_FLAVOR_SIZE	MIPS_EXC_STATE_COUNT
#define TBIT_SET(x,state)	((struct mips_exc_state *)state)->cause = EXC_SST;
#define TBIT_CLEAR(x,state)	if (1) return;


/* Interface definitions for kernel debugger KDB */

/* I am not going to pretend I've done anything about this */
