/* Low level interface to ptrace, for GDB when running under Unix.
   Copyright (C) 1988, 1989 Free Software Foundation, Inc.
   Contributed by Alessandro Forin(af@cs.cmu.edu) at CMU
   and by Per Brothner(bothner@cs.wisc.edu) at U.Wisconsin.

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
 * $Log:	mipsmach-dep.c,v $
 * Revision 3.2  90/04/11  13:35:05  af
 * 	Moved a bunch of decls in the MI side, where they belong.
 * 
 */

#include "defs.h"
#include "param.h"
#include "frame.h"
#include "inferior.h"
#include "mips/inst.h"
#include "symtab.h"
#include "value.h"
#include "wait.h"

#ifdef USG
#include <sys/types.h>
#endif

#include <stdio.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <signal.h>
#include <sys/ioctl.h>

#ifdef COFF_ENCAPSULATE
#include "a.out.encap.h"
#else
#include <a.out.h>
#endif
#ifndef N_SET_MAGIC
#define N_SET_MAGIC(exec, val) ((exec).magic = (val))
#endif

#ifndef	MIPSMAGIC
#ifdef MIPSEL
#define MIPSMAGIC	MIPSELMAGIC
#else
#define MIPSMAGIC	MIPSEBMAGIC
#endif
#endif

#include <sys/user.h>		/* After a.out.h  */
#include <sys/file.h>
#include <sys/stat.h>

extern int errno;

#include <mach/message.h>
#include <mach/exception.h>
#include <mach/notify.h>
#include <mach/port.h>
#include <mach/machine/thread_status.h>

#if defined(vax) || defined(i386)
#include <machine/psl.h>
#endif /* vax || i386 */

#include <cthreads.h>
#define	MTHREAD 1
#include <cthread_internals.h>

extern thread_t current_thread;

extern void mach_error();


/* This is one way to do it, not the best one */

#include "mach_os.c"


/* Get all registers from the inferior */

void
fetch_inferior_registers ()
{
  register int regno;
  int zero = 0;
  register int *regp;
  struct mips_thread_state state;
  struct mips_exc_state exc_state;
  int stateCnt = MIPS_THREAD_STATE_COUNT;
  kern_return_t ret;  

  if ((ret = thread_get_state(current_thread, MIPS_THREAD_STATE, &state,
			      &stateCnt)) != KERN_SUCCESS) 
    {
      mach_error("thread_get_state", ret);
      error("in fetch_inferior_registers.");
      return;
    }

  supply_register(0, &zero);

  for (regno = 1, regp = &state.r1; regno < 32; regno++, regp++)
    supply_register (regno, regp);

  supply_register(33, &state.mdhi);
  supply_register(34, &state.mdlo);
  supply_register(37, &state.pc);

  stateCnt = MIPS_EXC_STATE_COUNT;
  if ((ret = thread_get_state(current_thread, MIPS_EXC_STATE, &exc_state,
			      &stateCnt)) != KERN_SUCCESS) 
    {
      mach_error("thread_get(exc)_state", ret);
      error("in fetch_inferior_registers.");
    }

  supply_register(35, &exc_state.address);
  supply_register(36, &exc_state.cause);
  /* This is imprecise, but useful later */
  supply_register(32, &exc_state.coproc_state);

  if (exc_state.coproc_state & MIPS_STATUS_USE_COP1)
    {
      /* spare some stack space.. */
      struct mips_float_state *f_state = (struct mips_float_state *)&state;

      stateCnt = MIPS_FLOAT_STATE_COUNT;
      if ((ret = thread_get_state(current_thread, MIPS_FLOAT_STATE, f_state,
			      &stateCnt)) != KERN_SUCCESS) 
        {
          mach_error("thread_get(float)_state", ret);
          error("in fetch_inferior_registers.");
	}
      for (regno = 38, regp = (int*)f_state; regno < NUM_REGS; regno++, regp++)
        supply_register (regno, regp);
    }
  else
    {
      for (regno = 38; regno < NUM_REGS; regno++)
        supply_register (regno, &zero);
    }
}

/* Store our register values back into the inferior.
   If REGNO is -1, do this for all registers.
   Otherwise, REGNO specifies which register (so we can waste more time).  */

store_inferior_registers (regno)
     register int regno;
{
  register int *regp;
  struct mips_thread_state state;
  struct mips_exc_state exc_state;
  int stateCnt;
  kern_return_t ret;
  
  if (regno == 0)
    return;

  if (regno == -1)
    {
      for (regno = 1, regp = &state.r1; regno < 32; regno++, regp++)
        *regp = read_register (regno);

      state.mdhi = read_register(33);
      state.mdlo = read_register(34);
      state.pc = read_register(37);

      stateCnt = MIPS_THREAD_STATE_COUNT;
      if ((ret = thread_set_state(current_thread, MIPS_THREAD_STATE, &state,
			      	  stateCnt)) != KERN_SUCCESS) 
        {
          mach_error("thread_set_state", ret);
          error("in store_inferior_registers.");
        }

      regno = read_register(32);
      if (regno & MIPS_STATUS_USE_COP1)
        {
          /* spare some stack space.. */
          struct mips_float_state *f_state = (struct mips_float_state *)&state;

          for (regno = 38, regp = (int*)f_state; regno < NUM_REGS; regno++, regp++)
            *regp = read_register (regno);

          stateCnt = MIPS_FLOAT_STATE_COUNT;
          if ((ret = thread_set_state(current_thread, MIPS_FLOAT_STATE, f_state,
					&stateCnt)) != KERN_SUCCESS) 
            {
              mach_error("thread_set(float)_state", ret);
              error("in store_inferior_registers.");
	    }
	}
    }
  else
    {
      if (regno == 32 || regno == 35 || regno == 36 || regno == 71)
        return;
      if (regno < 38)
        {
          stateCnt = MIPS_THREAD_STATE_COUNT;
          if ((ret = thread_get_state(current_thread, MIPS_THREAD_STATE, &state,
			      &stateCnt)) != KERN_SUCCESS) 
            {
              mach_error("thread_get_state", ret);
              error("in store_inferior_register.");
	    }

	  if (regno == 37)
	    state.pc = read_register(regno);
	  else
	    {
	      if (regno < 32)
	        regp = ((int*)&state) - 1;	/* no r0 in state!! */
	      else
	        regp = (int*)&state;	/* "sr" compensates for "r0" */

	      regp[regno] = read_register(regno);
	    }

          stateCnt = MIPS_THREAD_STATE_COUNT;
          if ((ret = thread_set_state(current_thread, MIPS_THREAD_STATE, &state,
					stateCnt)) != KERN_SUCCESS) 
            {
              mach_error("thread_set_state", ret);
              error("in store_inferior_register.");
            }
	}
      else
        {
          /* spare some stack space.. */
          struct mips_float_state *f_state = (struct mips_float_state *)&state;

          stateCnt = MIPS_FLOAT_STATE_COUNT;
          if ((ret = thread_get_state(current_thread, MIPS_FLOAT_STATE, f_state,
			      &stateCnt)) != KERN_SUCCESS) 
            {
              mach_error("thread_get(float)_state", ret);
              error("in store_inferior_register.");
	    }

	  regp = (int*)f_state;
	  regp[regno-38] = read_register(regno);

          stateCnt = MIPS_FLOAT_STATE_COUNT;
          if ((ret = thread_set_state(current_thread, MIPS_FLOAT_STATE, &state,
					stateCnt)) != KERN_SUCCESS) 
            {
              mach_error("thread_set(float)_state", ret);
              error("in store_inferior_register.");
            }
	}
    }
}

/* Return the thread's stack pointer.  Used when mapping
   kernel threads to Mach CThreads */

thread_stack_pointer(th)
	thread_t th;
{
  struct mips_thread_state state;
  int stateCnt = MIPS_THREAD_STATE_COUNT;
  if (thread_get_state(th, MIPS_THREAD_STATE, &state,&stateCnt) != KERN_SUCCESS) 
    return 0;
  return state.r29;
}



/* Work with core dump and executable files, for GDB. 
   This code would be in core.c if it weren't machine-dependent. */

#ifndef N_TXTADDR
#define N_TXTADDR(hdr) 0
#endif /* no N_TXTADDR */

#ifndef N_DATADDR
#define N_DATADDR(hdr) hdr.a_text
#endif /* no N_DATADDR */

/* Make COFF and non-COFF names for things a little more compatible
   to reduce conditionals later.  */

#ifndef COFF_FORMAT
#ifndef AOUTHDR
#define AOUTHDR struct exec
#endif
#endif

extern char *sys_siglist[];


/* Hook for `exec_file_command' command to call.  */

extern void (*exec_file_display_hook) ();
   
/* File names of core file and executable file.  */

extern char *corefile;
extern char *execfile;

/* Descriptors on which core file and executable file are open.
   Note that the execchan is closed when an inferior is created
   and reopened if the inferior dies or is killed.  */

extern int corechan;
extern int execchan;

/* Last modification time of executable file.
   Also used in source.c to compare against mtime of a source file.  */

extern int exec_mtime;

/* Virtual addresses of bounds of the two areas of memory in the core file.  */

extern CORE_ADDR data_start;
extern CORE_ADDR data_end;
extern CORE_ADDR stack_start;
extern CORE_ADDR stack_end;

/* Virtual addresses of bounds of two areas of memory in the exec file.
   Note that the data area in the exec file is used only when there is no core file.  */

extern CORE_ADDR text_start;
extern CORE_ADDR text_end;

extern CORE_ADDR exec_data_start;
extern CORE_ADDR exec_data_end;

/* Address in executable file of start of text area data.  */

extern int text_offset;

/* Address in executable file of start of data area data.  */

extern int exec_data_offset;

/* Address in core file of start of data area data.  */

extern int data_offset;

/* Address in core file of start of stack area data.  */

extern int stack_offset;

#ifdef COFF_FORMAT
/* various coff data structures */

extern FILHDR file_hdr;
extern SCNHDR text_hdr;
extern SCNHDR data_hdr;

#endif /* not COFF_FORMAT */

/* a.out header saved in core file.  */
  
extern AOUTHDR core_aouthdr;

/* a.out header of exec file.  */

extern AOUTHDR exec_aouthdr;

extern void validate_files ();

core_file_command (filename, from_tty)
     char *filename;
     int from_tty;
{
  int val;
  extern char registers[];

  /* Discard all vestiges of any previous core file
     and mark data and stack spaces as empty.  */

  if (corefile)
    free (corefile);
  corefile = 0;

  if (corechan >= 0)
    close (corechan);
  corechan = -1;

  data_start = 0;
  data_end = 0;
  stack_start = STACK_END_ADDR;
  stack_end = STACK_END_ADDR;

  /* Now, if a new core file was specified, open it and digest it.  */

  if (filename)
    {
      filename = tilde_expand (filename);
      make_cleanup (free, filename);
      
      if (have_inferior_p ())
	error ("To look at a core file, you must kill the inferior with \"kill\".");
      corechan = open (filename, O_RDONLY, 0);
      if (corechan < 0)
	perror_with_name (filename);
      /* 4.2-style (and perhaps also sysV-style) core dump file.  */
      {
	struct user u;

	unsigned int reg_offset;

	val = myread (corechan, &u, sizeof u);
	if (val < 0)
	  perror_with_name ("Not a core file: reading upage");
	if (val != sizeof u)
	  error ("Not a core file: could only read %d bytes", val);

	/* We are depending on exec_file_command having been called
	   previously to set exec_data_start.  Since the executable
	   and the core file share the same text segment, the address
	   of the data segment will be the same in both.  */
	data_start = exec_data_start;

	data_end = data_start + NBPG * u.u_dsize;
	stack_start = stack_end - NBPG * u.u_ssize;
	data_offset = NBPG * UPAGES;
	stack_offset = NBPG * (UPAGES + u.u_dsize);

	/* Some machines put an absolute address in here and some put
	   the offset in the upage of the regs.  */
	reg_offset = 0;
#define register_addr(regno,xxx) ((NBPG*UPAGES) + (regno - 38)*sizeof(int))

	/* I don't know where to find this info.
	   So, for now, mark it as not available.  */
	N_SET_MAGIC (core_aouthdr, 0);

	/* Read the register values out of the core file and store
	   them where `read_register' will find them.  */

	{
	  register int regno;

	  for (regno = 1; regno < NUM_REGS; regno++)
	    {
	      char buf[MAX_REGISTER_RAW_SIZE];

	      val = lseek (corechan, register_addr (regno, reg_offset), 0);
	      if (val < 0
		  || (val = myread (corechan, buf, sizeof buf)) < 0)
		{
		  char * buffer = (char *) alloca (strlen (reg_names[regno])
						   + 30);
		  strcpy (buffer, "Reading register ");
		  strcat (buffer, reg_names[regno]);
						   
		  perror_with_name (buffer);
		}

	      supply_register (regno, buf);
	    }
	}
      }
      if (filename[0] == '/')
	corefile = savestring (filename, strlen (filename));
      else
	{
	  corefile = concat (current_directory, "/", filename);
	}

      set_current_frame ( create_new_frame (read_register (FP_REGNUM),
					    read_pc ()));
      select_frame (get_current_frame (), 0);
      validate_files ();
    }
  else if (from_tty)
    printf ("No core file now.\n");
}

exec_file_command (filename, from_tty)
     char *filename;
     int from_tty;
{
  int val;

  /* Eliminate all traces of old exec file.
     Mark text segment as empty.  */

  if (execfile)
    free (execfile);
  execfile = 0;
  data_start = 0;
  data_end -= exec_data_start;
  text_start = 0;
  text_end = 0;
  exec_data_start = 0;
  exec_data_end = 0;
  if (execchan >= 0)
    close (execchan);
  execchan = -1;

  /* Now open and digest the file the user requested, if any.  */

  if (filename)
    {
      filename = tilde_expand (filename);
      make_cleanup (free, filename);
      
      execchan = openp (getenv ("PATH"), 1, filename, O_RDONLY, 0,
			&execfile);
      if (execchan < 0)
	perror_with_name (filename);

      {
	int aout_hdrsize;
	int num_sections;

	lseek(execchan, 0L, 0);
	if ((myread(execchan, &file_hdr, sizeof file_hdr) < 0) ||
	    (file_hdr.f_magic != MIPSMAGIC))
	  error ("\"%s\": not in executable format.", execfile);

	aout_hdrsize = sizeof exec_aouthdr;
	num_sections = file_hdr.f_nscns;

	if (myread (execchan, &exec_aouthdr, aout_hdrsize) < 0)
	  error ("\"%s\": can't read optional aouthdr", execfile);

	bzero(&text_hdr, sizeof text_hdr);
	if (read_section_hdr (execchan, _TEXT, &text_hdr, num_sections) < 0)
	  printf ("\"%s\": can't read text section header\n", execfile);

	bzero(&data_hdr, sizeof data_hdr);
	if (read_section_hdr (execchan, _DATA, &data_hdr, num_sections) < 0)
	  printf ("\"%s\": can't read data section header\n", execfile);

	text_start = exec_aouthdr.text_start;
	text_end = text_start + exec_aouthdr.tsize;
	if (exec_aouthdr.magic == 0413)
		text_offset = 0;
	else
		text_offset = text_hdr.s_scnptr;
	exec_data_start = exec_aouthdr.data_start;
	exec_data_end = exec_data_start + exec_aouthdr.dsize;
	exec_data_offset = data_hdr.s_scnptr;
	data_start = exec_data_start;
	data_end += exec_data_start;
	exec_mtime = file_hdr.f_timdat;
      }

      validate_files ();
    }
  else if (from_tty)
    printf ("No exec file now.\n");

  /* Tell display code (if any) about the changed file name.  */
  if (exec_file_display_hook)
    (*exec_file_display_hook) (filename);
}


read_section_hdr (chan, section_name, section_hdr, nsects)
    register int chan;
    register char *section_name;
    SCNHDR *section_hdr;
    register int nsects;
{
  register int i;

  if (lseek (chan, FILHSZ + sizeof (AOUTHDR), 0) < 0)
    return -1;

  for (i = 0; i < nsects; i++)
    {
      if (myread (chan, (char *)section_hdr, SCNHSZ) < 0)
	return -1;
      if (strncmp (section_hdr->s_name, section_name, 8) == 0)
	return 0;
    }
    return -1;
}


/* Mips specific additions, mostly expansions of standard macros */

/* NOTE: this code was written for the 2.4 version, which did not
   have frame caching of its own.  Now that 3.4/5 does have it, it
   would be better not to duplicate caching at this level.
   I'll fix it, sometimes. */

#define VM_MIN_ADDRESS (unsigned)0x400000
#define HEAP_START	(unsigned)0x10000000
#define RA_UNDEF -1

#define CH_STATIC_SIZE 3
struct chain_info_t {
    	struct chain_info_t 		*next;
	int				 valid;
	FRAME_ADDR			 the_frame;
	CORE_ADDR 			 the_pc;
	CORE_ADDR			 the_ra;
	struct mips_extra_func_info	 the_info;
} chain_info[CH_STATIC_SIZE];

/* Re-evaluate the virtual frame pointer for the current frame,
   backtrace a number of frames, so that chaining will later work.
   Return the correct frame address in FCI */

validate_frame(fci)
	FRAME fci;
{
	extern CORE_ADDR sigtramp_address;
	struct chain_info_t *ci;
	CORE_ADDR       pc = fci->pc;

	/* process exited, or something screwed */
	if (pc < VM_MIN_ADDRESS)
		return;

	/* auto-initialize */
	if (chain_info[0].next == 0)
		for (ci = &chain_info[0]; ci < &chain_info[CH_STATIC_SIZE - 1]; ci++)
			ci->next = ci + 1;

	/* Auto-initialize the sigtramp bounds too. This is
	   where we MUST have it right */
	if (!sigtramp_address)
		in_sigtramp(0,0);

	/* Now fill our cache. Only do the minimum now */
	ci = &chain_info[0];
	ci->the_pc = pc;
	ci->the_ra = RA_UNDEF;	/* fix later */
	mips_frame_info(ci->the_pc, 1, &ci->the_info);
	fci->frame += ci->the_info.framesize;
	ci->the_frame = fci->frame;
	ci->valid = 1;

	for (ci = &chain_info[1]; ci < &chain_info[CH_STATIC_SIZE - 1]; ci++)
		if (mips_extend_chain(ci - 1, ci)) {
			ci++;
			break;
		}
	/* sentinel */
	ci->valid = 0;
	ci->the_frame = 0;
}

/* Lookup FRAME in our cache. It better be there */

static struct chain_info_t *
mips_find_cached_frame(frame)
	FRAME_ADDR frame;
{
	struct chain_info_t *ci = chain_info;
	while (ci->valid && ci->the_frame != frame)
		ci = ci->next;
	return ci;
}

/* Extend our cache of frames by appending the NXT one to CUR */

mips_extend_chain(cur, nxt)
	struct chain_info_t *cur, *nxt;
{
	extern CORE_ADDR sigtramp_address, sigtramp_end;
#define SIGFRAME_BASE	sizeof(struct sigcontext)
#define SIGFRAME_PC_OFF	(-SIGFRAME_BASE+ 2*sizeof(int))
#define SIGFRAME_SP_OFF	(-SIGFRAME_BASE+32*sizeof(int))
#define SIGFRAME_RA_OFF	(-SIGFRAME_BASE+34*sizeof(int))
	register int sigframe = 0;

	/* The signal handler saves all registers, must handle it specially */
	if (cur->the_pc > sigtramp_address &&
	    cur->the_pc < sigtramp_end)
		sigframe = 1;

	/* fixup ra, if needed */
	if (cur->the_ra == RA_UNDEF)
		/* Is it in the signal ctxt.. */
		if (sigframe)
			cur->the_ra = read_memory_integer(
				cur->the_frame + SIGFRAME_PC_OFF, 4);
		/* .. or is it on the stack .. */
		else if (cur->the_info.regmask & (1 << 31))
			cur->the_ra = read_memory_integer(cur->the_frame +
						    cur->the_info.regoffset, 4);
		/* .. or still in the register ? */
		else if (cur == chain_info)
			cur->the_ra = (CORE_ADDR) read_register(31);
		else {
			/*
			 * This is a leaf procedure calling ?  The only case
			 * where this is legal is if we are invoking functions
			 * in the inferior 
			 */
			cur->the_ra = (CORE_ADDR) read_register(31);
		}

	/* Might use it just for updates of CUR */
	if (nxt == 0)
		return 0;

	if (cur->the_ra < VM_MIN_ADDRESS || cur->the_ra & 0x3) {
		nxt->valid = 1;	/* end of stack */
		nxt->the_pc = 0;
		nxt->the_ra = 0;
		nxt->the_frame = 0;
		return 1;
	}
	/* Normally the pc that called CUR is 2 instructions off RA,
	   unless we are handling a signal, in which case all info
	   is in the sigcontext structure on the stack */
	if (sigframe) {
		nxt->the_pc = cur->the_ra;
		nxt->the_ra = read_memory_integer(cur->the_frame + SIGFRAME_RA_OFF, 4);
	} else {
		nxt->the_pc = cur->the_ra - 8;
		nxt->the_ra = RA_UNDEF;
	}
	mips_frame_info(nxt->the_pc, 0, &nxt->the_info);
	if (sigframe)
		nxt->the_frame = read_memory_integer(cur->the_frame-SIGFRAME_SP_OFF, 4);
	else
		nxt->the_frame = cur->the_frame;
	nxt->the_frame += nxt->the_info.framesize;
	nxt->valid = 1;
	return 0;
}

/* Get all possible info about the procedure containing PC.
   Might either use the symtab (if available) or poke around in
   the inferior to get its answers.  Rumaging is more expensive
   but not that much. CHECKFRAME is true when we first get back in touch
   with the inferior, e.g. for the top stack frame. The information
   is returned in the structure RET points to. */

#define MAX_PROLOGUE_BYTES	400

mips_frame_info(pc, checkframe, ret)
	CORE_ADDR pc;
	mips_extra_func_info_t ret;	/* IN/OUT */
{
	extern struct block *block_for_pc();
	extern int skip_prologue_p;
	CORE_ADDR       spc = get_pc_function_start(pc);
	struct block   *b = block_for_pc(pc);
	unsigned int    mask;

	/* Checkframe : Anything could have happened to the inferior! */

	if (spc == 0 ||
	    (checkframe && !skip_prologue_p)) {
raw_mode:
		spc = mips_function_start(pc);
	}
	if (b) {
		struct symbol  *s = lookup_symbol(".gdbinfo.", b, LABEL_NAMESPACE, 0);

		/* This is the easy case, but be real careful: the function
		   info might be largely unknown (stripped) or what we saw
		   in the inferior does not agree with the symtab.
		   Note that when the shell traps we do get here */
		if (s == 0 || (checkframe && spc != b->startaddr)) {
			/* Try again in raw mode */
			b = 0;
			goto raw_mode;
		}

		*ret = *(struct mips_extra_func_info *) (s->value.value);

		/* Adjust info if in prologue */
		if (checkframe) {
#if 1
			/* PerfOpt: Prologue cannot be too big */
			if (pc - spc > MAX_PROLOGUE_BYTES)
				return;
#endif
			for (mask = 0; spc < pc; spc += 4) {
				unsigned int    m0;
				register union mips_instruction i;

				i.word = read_memory_integer(spc, 4);
				if (i.word == 0x5000d)
					i.word = breakpoint_shadow_val(spc, i.word);
				m0 = mips_frame_op(i);
				if (m0 & (1 << SP_REGNUM))
					m0 = m0 ^ mask;
				mask |= m0;
			}

			if ((mask & (1 << SP_REGNUM)) == 0)
				ret->framesize = 0;

			ret->regmask &= mask;
		}
		return;
	}
	/*
	 * Ahi ahi ahi, need to work real hard on this one, but at least we
	 * can work on tighter bounds now 
	 */
	bzero(ret, sizeof *ret);
#if 1
	if (pc - spc > MAX_PROLOGUE_BYTES)
		pc = spc + MAX_PROLOGUE_BYTES;
#endif
	for (mask = 0; spc < pc; spc += 4) {
		register union mips_instruction i;
		unsigned int    m0;

		i.word = read_memory_integer(spc, 4);
		if (i.word == 0x5000d)
			i.word = breakpoint_shadow_val(spc, i.word);
		m0 = mips_frame_op(i);
		mask |= m0;
		/*
		 * Sometimes we might be in the middle of the function`s
		 * epilogue with an updated SP but still with the frame
		 * intact.  We must force FP == SP in this case. 
		 */
		if (m0 & (1 << SP_REGNUM))
			if (i.j_format.opcode == addiu_op ||
			    i.j_format.opcode == addi_op)
				ret->framesize -= i.i_format.simmediate;	/* !! */
		if (m0 & (1 << 31))
			ret->regoffset = i.i_format.simmediate - ret->framesize;
		/*
		 * Guessing games: if it stores a0-a3 someplace it must be
		 * saving an argument 
		 */
		if (m0 & 0xf0)
			ret->numargs++;
	}
	ret->regmask = mask & 0xc0ff0000;	/* ra, s0-s8 */
	ret->framereg = SP_REGNUM;		/* reasonable guesses */
	ret->pcreg = 31;
	if (ret->numargs == 0)
		ret->numargs = -1;
}

/* Rumage backwards from PC until the end of previous procedure.
   This is sometimes incorrect because of padding. */

static
mips_function_start(pc)
	register unsigned pc;
{
	register unsigned min_pc = VM_MIN_ADDRESS;

	if (pc >= HEAP_START) {
		/* Oh ho, this might be trouble coming */
		min_pc = HEAP_START;
#define MAX_FUNCTION_SIZE 100*1024	/* arbitrary!! */
		if ((pc - min_pc) > MAX_FUNCTION_SIZE)
			min_pc = pc - MAX_FUNCTION_SIZE;
#undef	MAX_FUNCTION_SIZE
	}
	for (pc -= 4; pc >= min_pc; pc -= 4) {
		unsigned int val;
		if (read_memory(pc, &val, sizeof(val)))
			return pc + 4;	/* error */
		if (val == 0x3e00008)
			break;
	}
	if (pc < min_pc)
		return min_pc;
	return pc + 8;
}

/* Return a mask describing which frame-related operations
   the instruction X performs.  These include changing the
   stack pointer or storing registers */

static
mips_frame_op(x)
	register union mips_instruction x;
{
	/* frame is mods by add and sub instructions. else you loose */
	switch (x.j_format.opcode) {
	    case spec_op:
		switch (x.r_format.func) {
		    case add_op:
		    case addu_op:
		    case sub_op:
		    case subu_op:
			if (x.r_format.rd == SP_REGNUM)
				return (1 << SP_REGNUM);
		    default:
			break;
		}
		break;
	    case addi_op:
	    case addiu_op:
		if (x.i_format.rt == SP_REGNUM)
			return (1 << SP_REGNUM);
		break;
	    case sw_op:
		return (1 << x.i_format.rt);
	    default:
		break;
	}
	return 0;
}


/* Describe the pointer in each stack frame to the previous stack frame
   (its caller).

   "FRAME_CHAIN takes a frame's nominal address
    and produces the frame's chain-pointer."

   In the case of MIPS, the frame's nominal address is the SP value plus
   the function's framesize.  The previous value is just not saved */

FRAME_ADDR
mips_frame_chain(frame)
	FRAME_ADDR frame;
{
	register struct chain_info_t *ci, *cn;

	/* stop at the last VALID frame */
	/* assert chain_info[0].valid */
	for (ci = chain_info; ci->the_frame; ci = ci->next)
		if (ci->the_frame == frame)
			break;

	/* Cache hit cases : got it or end of stack */
	if (ci->the_frame == frame && ci->next->valid)
		return ci->next->the_frame;	/* got it */
	if (!ci->the_frame)
		return 0;			/* EOS, shouldn't happen */

	/* Two possibilities remain: bogus frame, or not yet reached.
	   Try extending the stack until we reach it, or all the
	   way to the top. */

	/* assert ci->valid && c->the_frame */
	while (1) {
		cn = ci->next;
		if (cn == 0) {
			cn = (struct chain_info_t*)xzalloc(sizeof(struct chain_info_t));
			ci->next = cn;
		}
		/* assert cn->valid == 0 */
		if (mips_extend_chain(ci,cn) || ci->the_frame == frame)
			break;
		ci = cn;
	}

	if (cn->valid && cn->next == 0) {
		/* add sentinel */
		ci = (struct chain_info_t*)xzalloc(sizeof(struct chain_info_t));
		cn->next = ci;
	}

	return cn->the_frame;	/* 0 for bogus requests */
}

/* Return the saved PC in a non-terminal FRAME. */

mips_frame_saved_pc(frame)
	FRAME_ADDR frame;
{
	struct chain_info_t *ci = mips_find_cached_frame(frame);

	if (ci->the_frame != frame)
		return 0;

	if (ci->the_ra == RA_UNDEF)
		mips_extend_chain(ci, ci->next);
	return ci->the_ra;
}

/* Return number of args passed to a frame. described by FIP.
   Can return -1, meaning no way to tell.  */

mips_frame_num_args(fip)
	FRAME fip;
{
	struct chain_info_t *p;

	p = mips_find_cached_frame(FRAME_FP(fip));
	if (p->valid)
		return p->the_info.numargs;
	return -1;
}


/* Put here the code to store, into a struct frame_saved_regs FRP,
   the addresses of the saved registers of frame described by FIP.
   This includes special registers such as pc and fp saved in special
   ways in the stack frame.  sp is even more special:
   the address we return for it IS the sp for the next frame.  */

mips_frame_find_saved_regs(fip,frp)
	FRAME fip;
	struct frame_saved_regs *frp;
{
	register int    regnum;
	register int    regmask;
	register CORE_ADDR next_addr;
	struct chain_info_t *ci;

	bzero(frp, sizeof(struct frame_saved_regs));

	ci = mips_find_cached_frame(FRAME_FP(fip));

	/*
	 * Regmask`s high bit is for register 31, which is the first one that
	 * would be pushed.   
	 */
	next_addr = FRAME_FP(fip) + ci->the_info.regoffset + 4;
	regmask = ci->the_info.regmask;
	for (regnum = 31; regnum > 0; regnum--, regmask <<= 1)
		frp->regs[regnum] = (regmask < 0) ? (next_addr -= 4) : 0;
	frp->regs[SP_REGNUM] = FRAME_FP(fip) - ci->the_info.framesize;

	/*
	 * Floats now.  The "fregoff" is the displacement for the last
	 * saved register.
	 */
	regmask = ci->the_info.fregmask;
	if (regmask == 0)
		return;
	next_addr = ci->the_info.fregoffset + FRAME_FP(fip) - 4;
	for (regnum = 38; regnum < 70; regnum++, regmask >>= 1)
		if (regmask & 1) break;
	for (; regnum < 70; regnum++, regmask >>= 1)
		frp->regs[regnum] = (regmask & 1) ? (next_addr += 4) : 0;

}


/* Things needed for making the inferior call functions.  */

/* Push an empty stack frame, to record the current PC, etc.  */

mips_push_dummy_frame()
{
	register CORE_ADDR sp = read_register(SP_REGNUM);
	register int    regnum;

	sp = push_word(sp, 0);	/* pseudo-ra */
	for (regnum = 30; regnum > 0; regnum--)
		sp = push_word(sp, read_register(regnum));
	sp = push_word(sp, read_register(PC_REGNUM));
	sp = push_word(sp, read_register(33));
	sp = push_word(sp, read_register(34));
	sp = push_word(sp, read_register(31));	/* real-ra */
	write_register(SP_REGNUM, sp);
	write_register(31, read_register(PC_REGNUM));
}

/* Insert the specified number NARGS of args for a call to FUN
   into a call sequence of the above form stored at DUMMY, which
   is located at START_SP in the inferior.  RETTYPE is the type
   of the return value */

mips_fix_call_dummy(dummy, start_sp, fun, nargs, rettype)
	register union mips_instruction *dummy;
	CORE_ADDR start_sp, fun;
	struct type *rettype;
{
	CORE_ADDR sp = read_register(SP_REGNUM);
	static unsigned YAD[] = CALL_DUMMY;
	unsigned framesize = (35 + nargs) * sizeof(int) + sizeof(YAD);

	/* Allocate space for return value, if necessary */
	if (TYPE_CODE(rettype) == TYPE_CODE_STRUCT ||
	    TYPE_CODE(rettype) == TYPE_CODE_ARRAY ||
	    TYPE_CODE(rettype) == TYPE_CODE_UNION) {
		framesize += 4 + ((TYPE_LENGTH(rettype) + 3) & ~3);
	}

	dummy[2].u_format.uimmediate = framesize;
	dummy[3].u_format.uimmediate = framesize - 4;
	dummy[12].u_format.uimmediate = (fun & 0xffff0000) >> 16; /* lui */
	dummy[13].u_format.uimmediate = (fun & 0xffff);		 /* ori */
	mips_create_dummy_symbol( sp, sizeof(YAD) - 8, nargs, framesize);
}

/* Same as above, but computes the size of the stack adjustment
   precisely, so that passing strange args will work.  Needs
   an extra ARGS parameter, the list of types for the arguments */

mips_fix_call_dummy_a(dummy, start_sp, fun, nargs, args, rettype)
	register union mips_instruction *dummy;
	CORE_ADDR start_sp, fun;
	value *args;
	struct type *rettype;
{
	extern value value_arg_coerce();
	CORE_ADDR sp = read_register(SP_REGNUM);
	static unsigned YAD[] = CALL_DUMMY;
	unsigned framesize = (35) * sizeof(int) + sizeof(YAD);
	int i;

	/* Compute stack space needed for args and return value.
	   Note that doubles are returned in a register pair */
	if (TYPE_LENGTH(rettype) > 4 && TYPE_CODE(rettype) != TYPE_CODE_FLT)
		/* One pointer + value */
		framesize += 4 + ((TYPE_LENGTH(rettype) + 3) & ~3);

	/* Argument list size */
	i = 0;
	while (--nargs >= 0)
		i += TYPE_LENGTH (VALUE_TYPE (value_arg_coerce (args[nargs])));
	
	framesize += i;

	dummy[2].u_format.uimmediate = framesize;
	dummy[3].u_format.uimmediate = framesize - 4;
	dummy[12].u_format.uimmediate = (fun & 0xffff0000) >> 16; /* lui */
	dummy[13].u_format.uimmediate = (fun & 0xffff);		 /* ori */
	mips_create_dummy_symbol( sp, sizeof(YAD) - 8, nargs, framesize);
}

/* Pop a regular frame, e.g. the innermost one.
   Also used to pop the dummy frame. */
mips_pop_frame()
{
	FRAME current_frame = get_current_frame();
	struct chain_info_t *ci = mips_find_cached_frame(FRAME_FP(current_frame));
	CORE_ADDR sp = read_register(SP_REGNUM);
	CORE_ADDR pc = read_register(PC_REGNUM);
	CORE_ADDR addr;
	register int regnum;

	if (pc > sp) {	/* popping a dummy frame */
		ci->the_info.regmask = -1;
		mips_destroy_dummy_symbol(pc-8);
	}
	addr = FRAME_FP(current_frame) + ci->the_info.regoffset;
	for (regnum = 31; regnum > 0; regnum--)
		if (ci->the_info.regmask & (1 << regnum)) {
			write_register(regnum, read_memory_integer( addr, 4));
			addr -= 4;
		}

	if (pc > sp) {
		/* follow: pc,33,34,ra */
		pc = read_memory_integer(addr,4);
		write_register(33, read_memory_integer(addr-4,4));
		write_register(34, read_memory_integer(addr-8,4));
		write_register(31, read_memory_integer(addr-12,4));
	} else
		pc = read_register(31);
	write_register (PC_REGNUM, pc);
	write_register (SP_REGNUM, sp + ci->the_info.framesize);
	current_frame->pc = pc;
	validate_frame(current_frame);
}


/* Other things */

/* Bad floats: Returns 0 if P points to a valid IEEE floating point number,
   1 if P points to a denormalized number or a NaN. LEN says whether this is
   a single-precision or double-precision float */
#define SINGLE_EXP_BITS  8
#define DOUBLE_EXP_BITS 11
int
isa_NAN(p, len)
     int *p, len;
{
  int exponent;
  if (len == 4)
    {
      exponent = *p;
      exponent = exponent << 1 >> (32 - SINGLE_EXP_BITS - 1);
      return ((exponent == -1) || (! exponent && *p));
    }
  else if (len == 8)
    {
      exponent = *(p+1);
      exponent = exponent << 1 >> (32 - DOUBLE_EXP_BITS - 1);
      return ((exponent == -1) || (! exponent && *p * *(p+1)));
    }
  else return 1;
}


/* Extra fancy features */

int skip_prologue_p = 1;
int use_hex_unasm_p = 1;

/* Setf method for skip_prologue_p */

static void
set_prologue (arg, from_tty)
     char *arg;
     int from_tty;
{
	skip_prologue_p = parse_binary_operation ("set command-editing", arg);
}

/* Setf method for use_hex_unasm_p */

static void
set_hex_unasm(arg, from_tty)
     char *arg;
     int from_tty;
{
	use_hex_unasm_p = parse_binary_operation ("set command-editing", arg);
}

/* To skip prologues, I use this predicate. Returns either PC
   itself if the code at PC does not look like a function prologue,
   PC+4 if it does (our caller does not need any more fancy) */

mips_skip_prologue(pc)
	CORE_ADDR pc;
{
	if (skip_prologue_p) {
		struct symbol *f;
		struct block *b;
		unsigned long inst;

		/* For -g modules and most functions anyways the
		   first instruction adjusts the stack. */
		inst = read_memory_integer(pc, 4);
		if ((inst & 0xffff0000) == 0x27bd0000)
			return pc + 4;

		/* Well, it looks like a frameless. Let's make sure.
		   Note that we are not called on the current PC,
		   but on the function`s start PC, and I have definitely
		   seen optimized code that adjusts the SP quite later */
		b = block_for_pc(pc);
		if (!b) return pc;

		f = lookup_symbol(".gdbinfo.", b, LABEL_NAMESPACE,0);
		if (!f) return pc;

		/* Ideally, I would like to use the adjusted info
		   from mips_frame_info(), but for all practical
		   purposes it will not matter (and it would require
		   a different definition of SKIP_PROLOGUE())

		   Actually, it would not hurt to skip the storing
		   of arguments on the stack as well. */
		if (((struct mips_extra_func_info *)f->value.value)->framesize)
			return pc + 4;
	}
	return pc;
}

/* Initialization code for this module */

_initialize_mipsdep()
{
  extern struct cmd_list_element *setlist;

  add_cmd ("skip_prologue", class_support, set_prologue,
	   "Enable or disable skipping of the function's prologue.\n\
Use \"on\" to enable the skipping, and \"off\" to disable it.\n\
Without an argument, prologue skipping is enabled.", &setlist);

  add_cmd ("hex_disassemble", class_support, set_hex_unasm,
	   "Enable or disable use of hex numbers in disassembler.\n\
Use \"on\" to enable hex numbers, and \"off\" to disable it.\n\
Without an argument, hex numbers are enabled.", &setlist);

}

