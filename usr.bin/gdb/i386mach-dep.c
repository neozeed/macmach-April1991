/* Low level interface to ptrace, for GDB when running on the Intel 386.
   Copyright (C) 1988 Free Software Foundation, Inc.

GDB is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone
for the consequences of using it or for whether it serves any
particular purpose or works at all, unless he says so in writing.
Refer to the GDB General Public License for full details.

Everyone is granted permission to copy, modify and redistribute GDB,
but only under the conditions described in the GDB General Public
License.  A copy of this license is supposed to have been given to you
along with GDB so you can know your rights and responsibilities.  It
should be in a file named COPYING.  Among other things, the copyright
notice and this notice must be preserved on all copies.

In other words, go ahead and share GDB, but don't try to stop
anyone else from sharing it farther.  Help stamp out software hoarding!
*/
/* 
 * $Log:	i386mach-dep.c,v $
 * Revision 1.1  90/03/08  13:45:20  orr
 * Initial revision
 * 
 * Revision 1.2  90/02/16  17:34:59  orr
 * clean up read/write_inferior_memory so it's a bit
 * more robust.
 * 
 * Revision 1.1  90/02/06  16:33:29  orr
 * Initial revision
 * 
 * Revision 1.6  89/12/06  14:22:31  kupfer
 * Track CMU's source tree reorganization.  Ditch the hacks for using
 * config.gdb to control a.out versus COFF.  Use private routines to
 * detect NaN's and Infinity.  Other small fixes.
 * 
 * Revision 1.5  89/11/16  23:02:28  kupfer
 * Tweaks. Better use of Mach exceptions. Formatting changes.
 * 
 * Revision 1.4  89/11/03  18:45:49  kupfer
 * Change so that when you run config.gdb you can specify either COFF or
 * Berkeley-style a.out.  Cleaned up some configuration ifdef's.
 * 
 * Revision 1.3  89/11/02  21:31:37  kupfer
 * Initial floating point support under Mach.  Change "core" command so
 * that it prints the name of the program that generated the core file.
 * Numerous style changes.
 * 
 */


#include "defs.h"
#include "param.h"
#include "frame.h"
#include "inferior.h"
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
#include <sys/user.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <a.out.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <machine/reg.h>
#include <machine/psl.h>

#include <mach/message.h>
#include <mach/exception.h>
#include <mach/notify.h>
#include <mach_error.h>

#define	MDEBUG(x)

#define DUMB_STDIO	1	/* stdio can't handle NaN's & infinity */

extern int errno;

static void load_register();
static void store_registers();
void fetch_inferior_registers();

#include <cthreads.h>
#define	MTHREAD 1
#include <cthread_internals.h>

extern thread_t current_thread;

#include "mach_os.c"


void
fetch_inferior_registers ()
{
  extern char registers[]; /* findvar.c */
  thread_state_data_t state;
  kern_return_t ret;
  int stateCnt = i386_THREAD_STATE_COUNT;
#ifdef DEBUG
  int *reg_block;             /* address of register block */
  register int regno;
#endif


  /* see <mach/machine/thread_status.h> */
  if ((ret = thread_get_state(current_thread, i386_THREAD_STATE, state,
			      &stateCnt)) != KERN_SUCCESS) 
    {
      fprintf(stderr, "fetch_inferior_registers: ");
      mach_error("thread_get_state", ret);
      return;
    }

#ifdef DEBUG
  reg_block = (int *) state;
  for ( regno = 0; regno < NUM_REGS; regno++) 
    fprintf( stderr, "regno %d = %x\n", regno, *(reg_block + regno));
#endif DEBUG

  store_registers(state, registers); /* move values from state to registers */

}

/* Store our register values back into the inferior.
   If REGNO is -1, do this for all registers.
   Otherwise, REGNO specifies which register (so we can save time).  */

void
store_inferior_registers (regno)
     int regno;
{
  register int cnt;
  thread_state_data_t state;
  int stateCnt = i386_THREAD_STATE_COUNT;
  kern_return_t ret;

  /* see <mach/machine/thread_status.h> */
  if ((ret = thread_get_state(current_thread, i386_THREAD_STATE, state,
			      &stateCnt)) != KERN_SUCCESS) 
    {
      fprintf(stderr, "store_inferior_registers: ");
      mach_error("thread_get_state", ret);
      return;
    }

  /* move gdb's registers to thread_state's registers */
  if (regno == -1) 
    {
      for (cnt = 0; cnt < NUM_REGS; cnt++) 
	load_register(cnt, read_register(cnt), state);
    } 
  else
    load_register(regno, read_register(regno), state);

  /* now set regs in child task */
  if ((ret = thread_set_state(current_thread, i386_THREAD_STATE, state,
			      i386_THREAD_STATE_COUNT)) != KERN_SUCCESS)
    {
      fprintf(stderr, "store_inferior_registers: ");
      mach_error("thread_set_state", ret);
      return;
    }

}



/* Return the thread's stack pointer.  Used when mapping
   kernel threads to Mach CThreads */

thread_stack_pointer(th)
	thread_t th;
{
  struct i386_thread_state state;
  int stateCnt = i386_THREAD_STATE_COUNT;
  if (thread_get_state(th, i386_THREAD_STATE, &state, &stateCnt) != KERN_SUCCESS) 
    return 0;

  return state.uesp;
}


/* Work with core dump and executable files, for GDB. 
   This code would be in core.c if it weren't machine-dependent. */

/* Recognize COFF format systems because a.out.h defines AOUTHDR.  */
#ifdef AOUTHDR
#define COFF_FORMAT
#endif

#ifndef N_TXTADDR
#define N_TXTADDR(hdr) 0x10000
#endif /* no N_TXTADDR */

#undef	N_TXTOFF
#define	N_TXTOFF(x) \
	((x).a_magic==ZMAGIC ? 0 : sizeof (struct exec))

#ifndef N_DATADDR
#define N_DATADDR(hdr) round_page(hdr.a_text + N_TXTADDR(hdr))
#endif /* no N_DATADDR */

/* Make COFF and non-COFF names for things a little more compatible
   to reduce conditionals later.  */

#ifdef COFF_FORMAT
#define a_magic magic
#endif

#ifndef COFF_FORMAT
#define AOUTHDR struct exec
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

void
core_file_command (filename, from_tty)
     char *filename;
     int from_tty;
{
  int val;

#ifdef DEBUG
  fprintf( stderr, "entered core_file_command\n");
#endif

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
      if (have_inferior_p ())
	error ("To look at a core file, you must kill the inferior with \"kill\".");
      corechan = open (filename, O_RDONLY, 0);
      if (corechan < 0)
	perror_with_name (filename);
      /* 4.2-style (and perhaps also sysV-style) core dump file.  */
      {
	struct user u;

	int reg_offset;

	val = myread (corechan, &u, sizeof u);
	if (val < 0)
	  perror_with_name (filename);
#ifdef DEBUG
	fprintf(stderr, "read user struct\n");
#endif DEBUG
	/* these are virtual addresses in user space */
	data_start = exec_data_start;
	data_end = data_start + ctob( u.u_dsize);
	stack_start = stack_end - ctob( u.u_ssize);
	/* offsets into core file */
	data_offset = ctob(UPAGES);
	stack_offset = ctob(UPAGES + u.u_dsize);
	/* registers are at bottom of kernel stack */
        /* see .../mk/bsd/kern_sig.c */
	reg_offset = ctob(UPAGES) - SAVED_NUM_REGS * 4;
#ifdef DEBUG
	fprintf(stderr, "data_start: %x, data_end: %x\n", data_start,
		data_end);
	fprintf(stderr, "stack_start: %x, stack_end: %x\n", stack_start,
		stack_end);
	fprintf(stderr, "reg_offset: %x\n", reg_offset);
#endif DEBUG

	/* I don't know where to find this info.
	   So, for now, mark it as not available.  */
	core_aouthdr.a_magic = 0;

	/* Read the register values out of the core file and store
	   them where `read_register' will find them.  */

	{
	  register int regno;

	  for (regno = 0; regno < NUM_REGS; regno++)
	    {
	      char buf[MAX_REGISTER_RAW_SIZE];

	      val = lseek (corechan, i386_register_u_addr (regno, reg_offset), 0);
	      if (val < 0)
		perror_with_name (filename);

 	      val = myread (corechan, buf, sizeof buf);
	      if (val < 0)
		perror_with_name (filename);
	      supply_register (regno, buf);
	    }
	}
	printf("Core file is from \"%s\".\n", u.u_comm);
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

void
exec_file_command (filename, from_tty)
     char *filename;
     int from_tty;
{
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
      execchan = openp (getenv ("PATH"), 1, filename, O_RDONLY, 0,
			&execfile);
      if (execchan < 0)
	perror_with_name (filename);

#ifdef COFF_FORMAT
      {
	int aout_hdrsize;
	int num_sections;

	if (read_file_hdr (execchan, &file_hdr) < 0)
	  error ("\"%s\": not in executable format.", execfile);

	aout_hdrsize = file_hdr.f_opthdr;
	num_sections = file_hdr.f_nscns;

	if (read_aout_hdr (execchan, &exec_aouthdr, aout_hdrsize) < 0)
	  error ("\"%s\": can't read optional aouthdr", execfile);

	if (read_section_hdr (execchan, _TEXT, &text_hdr, num_sections) < 0)
	  error ("\"%s\": can't read text section header", execfile);

	if (read_section_hdr (execchan, _DATA, &data_hdr, num_sections) < 0)
	  error ("\"%s\": can't read data section header", execfile);

	text_start = exec_aouthdr.text_start;
	text_end = text_start + exec_aouthdr.tsize;
	text_offset = text_hdr.s_scnptr;
	exec_data_start = exec_aouthdr.data_start;
	exec_data_end = exec_data_start + exec_aouthdr.dsize;
	exec_data_offset = data_hdr.s_scnptr;
	data_start = exec_data_start;
	data_end += exec_data_start;
	exec_mtime = file_hdr.f_timdat;
      }
#else /* not COFF_FORMAT */
      {
	struct stat st_exec;

	if (myread (execchan, &exec_aouthdr, sizeof (AOUTHDR)) < 0)
	  perror_with_name (filename);

        text_start = N_TXTADDR (exec_aouthdr);
        exec_data_start = N_DATADDR (exec_aouthdr);

	text_offset = N_TXTOFF (exec_aouthdr) ;
	exec_data_offset = N_TXTOFF (exec_aouthdr) + exec_aouthdr.a_text;

	text_end = text_start + exec_aouthdr.a_text;
        exec_data_end = exec_data_start + exec_aouthdr.a_data;
	data_start = exec_data_start;
	data_end += exec_data_start;

	fstat (execchan, &st_exec);
	exec_mtime = st_exec.st_mtime;
      }
#endif /* not COFF_FORMAT */

      validate_files ();
    }
  else if (from_tty)
    printf ("No exec file now.\n");

  /* Tell display code (if any) about the changed file name.  */
  if (exec_file_display_hook)
    (*exec_file_display_hook) (filename);
}

/* helper functions for m-i386.h */

/* stdio style buffering to minimize calls to ptrace */
static CORE_ADDR codestream_next_addr;
static CORE_ADDR codestream_addr;
static unsigned char codestream_buf[sizeof (int)];
static int codestream_off;
static int codestream_cnt;

#define codestream_tell() (codestream_addr + codestream_off)
#define codestream_peek() (codestream_cnt == 0 ? \
			   codestream_fill(1): codestream_buf[codestream_off])
#define codestream_get() (codestream_cnt-- == 0 ? \
			 codestream_fill(0) : codestream_buf[codestream_off++])

static unsigned char 
codestream_fill (peek_flag)
     int peek_flag;
{
  codestream_addr = codestream_next_addr;
  codestream_next_addr += sizeof (int);
  codestream_off = 0;
  codestream_cnt = sizeof (int);
  read_memory (codestream_addr,
	       (unsigned char *)codestream_buf,
	       sizeof (int));
  
  if (peek_flag)
    return (codestream_peek());
  else
    return (codestream_get());
}

static void
codestream_seek (place)
     int place;
{
  codestream_next_addr = place & -sizeof (int);
  codestream_cnt = 0;
  codestream_fill (1);
  while (codestream_tell() != place)
    codestream_get ();
}

static void
codestream_read (buf, count)
     unsigned char *buf;
     int count;
{
  unsigned char *p;
  int i;
  p = buf;
  for (i = 0; i < count; i++)
    *p++ = codestream_get ();
}

/* next instruction is a jump, move to target */
static void
i386_follow_jump ()
{
  int long_delta;
  short short_delta;
  char byte_delta;
  int data16;
  int pos;
  
  pos = codestream_tell ();
  
  data16 = 0;
  if (codestream_peek () == 0x66)
    {
      codestream_get ();
      data16 = 1;
    }
  
  switch (codestream_get ())
    {
    case 0xe9:
      /* relative jump: if data16 == 0, disp32, else disp16 */
      if (data16)
	{
	  codestream_read ((unsigned char *)&short_delta, 2);
	  pos += short_delta + 3; /* include size of jmp inst */
	}
      else
	{
	  codestream_read ((unsigned char *)&long_delta, 4);
	  pos += long_delta + 5;
	}
      break;
    case 0xeb:
      /* relative jump, disp8 (ignore data16) */
      codestream_read ((unsigned char *)&byte_delta, 1);
      pos += byte_delta + 2;
      break;
    }
  codestream_seek (pos + data16);
}

/*
 * find & return amound a local space allocated, and advance codestream to
 * first register push (if any)
 *
 * if entry sequence doesn't make sense, return -1, and leave 
 * codestream pointer random
 */
static long
i386_get_frame_setup (pc)
     int pc;
{
  unsigned char op;
  
  codestream_seek (pc);
  
  i386_follow_jump ();
  
  op = codestream_get ();
  
  if (op == 0x58) /* popl %eax */
    {
      /*
       * this function must start with
       * 
       *    popl %eax		  0x58
       *    xchgl %eax, (%esp)  0x87 0x04 0x24
       * or xchgl %eax, 0(%esp) 0x87 0x44 0x24 0x00
       *
       * (the system 5 compiler puts out the second xchg
       * inst, and the assembler doesn't try to optimize it,
       * so the 'sib' form gets generated)
       * 
       * this sequence is used to get the address of the return
       * buffer for a function that returns a structure
       */
      int pos;
      unsigned char buf[4];
      static unsigned char proto1[3] = { 0x87,0x04,0x24 };
      static unsigned char proto2[4] = { 0x87,0x44,0x24,0x00 };
      pos = codestream_tell ();
      codestream_read (buf, 4);
      if (bcmp (buf, proto1, 3) == 0)
	pos += 3;
      else if (bcmp (buf, proto2, 4) == 0)
	pos += 4;
      
      codestream_seek (pos);
      op = codestream_get (); /* update next opcode */
    }
  
  if (op == 0x55) 			/* pushl %esp */
    {
      int subop;
      /* movl %esp, %ebp (2bytes) */
      if ( (subop = codestream_get ()) == 0x8b) {
	if (codestream_get () != 0xec)
	  return (-1);
      } else if (subop == 0x89) {
	if (codestream_get () != 0xe5)
	  return (-1);
      } else
	return (-1);
      /*
       * check for stack adjustment 
       *
       *  subl $XXX, %esp
       *
       * note: you can't subtract a 16 bit immediate
       * from a 32 bit reg, so we don't have to worry
       * about a data16 prefix 
       */
      op = codestream_peek ();
      if (op == 0x83)  /* subl with 8 bit immed */
	{
	  codestream_get ();
	  if (codestream_get () != 0xec)
	    return (-1);
	  /* subl with signed byte immediate 
	   * (though it wouldn't make sense to be negative)
	   */
	  return (codestream_get());
	}
      else if (op == 0x81)  /* subl with 32 bit immed */
	{
	  int locals;
	  if (codestream_get () != 0xec)
	    return (-1);
	  /* subl with 32 bit immediate */
	  codestream_read ((unsigned char *)&locals, 4);
	  return (locals);
	} 
      else 
	{
	  return (0);
	}
    } 
  else if (op == 0xc8) 
    {
      /* enter instruction: arg is 16 unsigned immed */
      unsigned short slocals;
      codestream_read ((unsigned char *)&slocals, 2);
      codestream_get (); /* flush final byte of enter instruction */
      return (slocals);
    }
  return (-1);
}

/*
 * parse the first few instructions of the function to see
 * what registers were stored.
 *
 * We handle these cases:
 *
 * The startup sequence can be at the start of the function,
 * or the function can start with a branch to startup code at the end.
 *
 * %ebp can be set up with either the 'enter' instruction, or 
 * 'pushl %ebp, movl %esp, %ebp' (enter is too slow to be useful,
 * but was once used in the sys5 compiler)
 *
 * Local space is allocated just below the saved %ebp by either the
 * 'enter' instruction, or by 'subl $<size>, %esp'.  'enter' has
 * a 16 bit unsigned argument for space to allocate, and the
 * 'addl' instruction could have either a signed byte, or
 * 32 bit immediate.
 *
 * Next, the registers used by this function are pushed.  In
 * the sys5 compiler they will always be in the order: %edi, %esi, %ebx
 * (and sometimes a harmless bug causes it to also save but not restore %eax);
 * however, the code below is willing to see the pushes in any order,
 * and will handle up to 8 of them.
 *
 * If the setup sequence is at the end of the function, then the
 * next instruction will be a branch back to the start.
 */

void
i386_frame_find_saved_regs (fip, fsrp)
     struct frame_info *fip;
     struct frame_saved_regs *fsrp;
{
  unsigned long locals;
  unsigned char op;
  CORE_ADDR dummy_bottom;
  CORE_ADDR adr;
  int i;
  
  bzero (fsrp, sizeof *fsrp);
  
  /* if frame is the end of a dummy, compute where the
   * beginning would be
   */
  dummy_bottom = fip->frame - 4 - NUM_REGS*4 - CALL_DUMMY_LENGTH;
  
  /* check if the PC is in the stack, in a dummy frame */
  if (dummy_bottom <= fip->pc && fip->pc <= fip->frame) 
    {
      /* all regs were saved by push_call_dummy () */
      adr = fip->frame - 4;
      for (i = 0; i < NUM_REGS; i++) 
	{
	  fsrp->regs[i] = adr;
	  adr -= 4;
	}
      return;
    }
  
  locals = i386_get_frame_setup (get_pc_function_start (fip->pc));
  
  if (locals >= 0) 
    {
      adr = fip->frame - 4 - locals;
      for (i = 0; i < 8; i++) 
	{
	  op = codestream_get ();
	  if (op < 0x50 || op > 0x57)
	    break;
	  fsrp->regs[op - 0x50] = adr;
	  adr -= 4;
	}
    }
  
  fsrp->regs[PC_REGNUM] = fip->frame + 4;
  fsrp->regs[FP_REGNUM] = fip->frame;
}

/* return pc of first real instruction */
i386_skip_prologue (pc)
     int pc;
{
  unsigned char op;
  int i;
  
  if (i386_get_frame_setup (pc) < 0)
    return (pc);
  
  /* found valid frame setup - codestream now points to 
   * start of push instructions for saving registers
   */
  
  /* skip over register saves */
  for (i = 0; i < 8; i++)
    {
      op = codestream_peek ();
      /* break if not pushl inst */
      if (op < 0x50 || op > 0x57) 
	break;
      codestream_get ();
    }
  
  i386_follow_jump ();
  
  return (codestream_tell ());
}

void
i386_push_dummy_frame ()
{
  CORE_ADDR sp = read_register (SP_REGNUM);
  int regnum;
  
  sp = push_word (sp, read_register (PC_REGNUM));
  sp = push_word (sp, read_register (FP_REGNUM));
  write_register (FP_REGNUM, sp);
  for (regnum = 0; regnum < NUM_REGS; regnum++)
    sp = push_word (sp, read_register (regnum));
  write_register (SP_REGNUM, sp);
}

void
i386_pop_frame ()
{
  FRAME frame = get_current_frame ();
  CORE_ADDR fp;
  int regnum;
  struct frame_saved_regs fsr;
  struct frame_info *fi;
  
  fi = get_frame_info (frame);
  fp = fi->frame;
  get_frame_saved_regs (fi, &fsr);
  for (regnum = 0; regnum < NUM_REGS; regnum++) 
    {
      CORE_ADDR adr;
      adr = fsr.regs[regnum];
      if (adr)
	write_register (regnum, read_memory_integer (adr, 4));
    }
  write_register (FP_REGNUM, read_memory_integer (fp, 4));
  write_register (PC_REGNUM, read_memory_integer (fp + 4, 4));
  write_register (SP_REGNUM, fp + 8);
  flush_cached_frames ();
  set_current_frame ( create_new_frame (read_register (FP_REGNUM),
					read_pc ()));
}

/* this table must line up with REGISTER_NAMES in m-i386.h */
/* symbols like 'EAX' come from <sys/reg.h> */
/* notice that gdb's layout of registers is different from i386_thread_state */

/* at gdb registers[ i] sits machine register regmap[ i] */
static int regmap[] = 
{
  EAX, ECX, EDX, EBX,
  UESP, EBP, ESI, EDI,
  EIP, EFL, CS, SS,
  DS, ES, FS, GS
};


static void
load_register( regno, val, state)
int regno, val;
struct i386_thread_state *state; 
/* load value of gdb's register regno into thread status block field */
/* taking into account regmap translation and the fact that there are */
/* holes in mapping between reg.h & struct thread_state_data */
{ 
  switch ( regmap[regno]) { 
    case EAX: state->eax = val; break;
    case ECX: state->ecx = val; break;
    case EDX: state->edx = val; break;
    case EBX: state->ebx = val; break;
    case UESP: state->uesp = val; break;
    case EBP: state->ebp = val; break;
    case ESI: state->esi = val; break;
    case EDI: state->edi = val; break;
    case EIP: state->eip = val; break;
    case EFL: state->efl = val; break;
    case CS: state->cs = val; break;
    case SS: state->ss = val; break;
    case DS: state->ds = val; break;
    case ES: state->es = val; break;
    case FS: state->fs = val; break;
    case GS: state->gs = val; break;
    default: 
    fprintf( stderr, "dep.c: load_register: bad register?\n"); exit(0);
  }
}

/* machine register i maps into gdb registers[ inv_regmap[i]] */
static int inv_regmap[ NUM_REGS + 1] =
{ /* gs */ 15,   /* fs */ 14,  /* es */ 13,  /* ds */ 12,
  /* edi */ 7,   /* esi */ 6,  /* ebp */ 5,  /* esp */ -1,
  /* ebx */ 3,   /* edx */ 2,  /* ecx */ 1,  /* eax */ 0,
  /* eip */ 8,   /* cs */ 10,  /* efl */ 9,  /* uesp */ 4,
  /* ss */ 11,
};

/* assign gdb's registers from the thread state */
static void
store_registers(state, regs)
     unsigned int state[], regs[];
{
  short i, j;

  for (i = 0; i < NUM_REGS + 1; i++)
    {
      j = inv_regmap[i];  /* esp is not part of registers... */
      if (j >= 0)
	regs[j] = state[i];
    }
}


/* blockend is the value of u.u_ar0, and points to the
 * place where GS is stored
 */
i386_register_u_addr (blockend, regnum)
     int blockend;
     int regnum;
{
#if 0
  /* this will be needed if fp registers are reinstated */
  /* for now, you can look at them with 'info float'
   * sys5 wont let you change them with ptrace anyway
   */
  if (regnum >= FP0_REGNUM && regnum <= FP7_REGNUM) 
    {
      int ubase, fpstate;
      struct user u;
      ubase = blockend + 4 * (SS + 1) - KSTKSZ;
      fpstate = ubase + ((char *)&u.u_fpstate - (char *)&u);
      return (fpstate + 0x1c + 10 * (regnum - FP0_REGNUM));
    } 
  else
#endif
    return (blockend + 4 * regmap[regnum]);
}

void
i387_to_double (from, to)
     char *from;
     char *to;
{
  long *lp;				/* is this necessary? */
  /* push extended mode on 387 stack, then pop in double mode
   *
   * first, set exception masks so no error is generated -
   * number will be rounded to inf or 0, if necessary 
   */
  asm ("pushl %eax");			/* grab a stack slot */
  asm ("fstcw (%esp)");			/* get 387 control word */
  asm ("movl (%esp),%eax");		/* save old value */
  asm ("orl $0x3f,%eax");		/* mask all exceptions */
  asm ("pushl %eax");
  asm ("fldcw (%esp)");			/* load new value into 387 */
  
  asm ("movl 8(%ebp),%eax");
  asm ("fldt (%eax)");			/* push extended number on 387 stack */
  asm ("fwait");
  asm ("movl 12(%ebp),%eax");
  asm ("fstpl (%eax)");			/* pop double */
  asm ("fwait");
  
  asm ("popl %eax");			/* flush modified control word */
  asm ("fnclex");			/* clear exceptions */
  asm ("fldcw (%esp)");			/* restore original control word */
  asm ("popl %eax");			/* flush saved copy */
}

void
double_to_i387 (from, to)
     char *from;
     char *to;
{
  /* push double mode on 387 stack, then pop in extended mode
   * no errors are possible because every 64-bit pattern
   * can be converted to an extended
   */
  asm ("movl 8(%ebp),%eax");
  asm ("fldl (%eax)");
  asm ("fwait");
  asm ("movl 12(%ebp),%eax");
  asm ("fstpt (%eax)");
  asm ("fwait");
}

/* 
 * XXX - This struct more or less duplicates the one in 
 *       <i386/fpreg.h>.  I assume that it was created because System 
 *       V didn't provide one.
 */
struct env387 
{
  unsigned short control;
  unsigned short r0;
  unsigned short status;
  unsigned short r1;
  unsigned short tag;
  unsigned short r2;
  unsigned long eip;
  unsigned short code_seg;
  unsigned short opcode;
  unsigned long operand;
  unsigned short operand_seg;
  unsigned short r3;
  unsigned char regs[8][10];		/* physical registers 0 - 7 */
};

static void
print_387_control_word (control)
unsigned short control;
{
  printf ("control 0x%04x: ", control);
  printf ("compute to ");
  switch ((control >> 8) & 3) 
    {
    case 0: printf ("24 bits; "); break;
    case 1: printf ("(bad); "); break;
    case 2: printf ("53 bits; "); break;
    case 3: printf ("64 bits; "); break;
    }
  printf ("round ");
  switch ((control >> 10) & 3) 
    {
    case 0: printf ("NEAREST; "); break;
    case 1: printf ("DOWN; "); break;
    case 2: printf ("UP; "); break;
    case 3: printf ("CHOP; "); break;
    }
  if (control & 0x3f) 
    {
      printf ("mask:");
      if (control & 0x0001) printf (" INVALID");
      if (control & 0x0002) printf (" DENORM");
      if (control & 0x0004) printf (" DIVZ");
      if (control & 0x0008) printf (" OVERF");
      if (control & 0x0010) printf (" UNDERF");
      if (control & 0x0020) printf (" LOS");
      printf (";");
    }
  printf ("\n");
  if (control & 0xe080) printf ("warning: reserved bits on 0x%x\n",
				control & 0xe080);
}

static void
print_387_status_word (status)
     unsigned short status;
{
  printf ("status 0x%04x: ", status);
  if (status & 0xff) 
    {
      printf ("exceptions:");
      if (status & 0x0001) printf (" INVALID");
      if (status & 0x0002) printf (" DENORM");
      if (status & 0x0004) printf (" DIVZ");
      if (status & 0x0008) printf (" OVERF");
      if (status & 0x0010) printf (" UNDERF");
      if (status & 0x0020) printf (" LOS");
      if (status & 0x0040) printf (" FPSTACK");
      printf ("; ");
    }
  printf ("flags: %d%d%d%d; ",
	  (status & 0x4000) != 0,
	  (status & 0x0400) != 0,
	  (status & 0x0200) != 0,
	  (status & 0x0100) != 0);
  
  printf ("top %d\n", (status >> 11) & 7);
}

static void
print_387_status (status, ep)
     unsigned short status;
     struct env387 *ep;
{
  int i;
  int bothstatus;
  int top;
  int fpreg;
  double val;
  
  bothstatus = ((status != 0) && (ep->status != 0));
  if (status != 0) 
    {
      if (bothstatus)
	printf ("exc status: ");
      print_387_status_word (status);
    }
  
  if (ep->status != 0) 
    {
      if (bothstatus)
	printf ("env status: ");
      print_387_status_word (ep->status);
    }
  
  print_387_control_word (ep->control);
  printf ("last exception: ");
  printf ("opcode 0x%x; ", ep->opcode);
  printf ("pc 0x%x:0x%x; ", ep->code_seg, ep->eip);
  printf ("operand 0x%x:0x%x\n", ep->operand_seg, ep->operand);
  
  top = (ep->status >> 11) & 7;
  
  printf ("regno  tag  msb              lsb  value\n");
  for (fpreg = 7; fpreg >= 0; fpreg--) 
    {
      printf ("%s %d: ", fpreg == top ? "=>" : "  ", fpreg);
      
      switch ((ep->tag >> (fpreg * 2)) & 3) 
	{
	case 0: printf ("valid "); break;
	case 1: printf ("zero  "); break;
	case 2: printf ("trap  "); break;
	case 3: printf ("empty "); break;
	}
      for (i = 9; i >= 0; i--)
	printf ("%02x", ep->regs[fpreg][i]);
      i387_to_double (ep->regs[fpreg], (char *)&val);
#if DUMB_STDIO
      if (is_nan(val))
	printf("  NaN\n");
      else if (is_inf(val))
	printf("  Infinity\n");
      else
	printf("  %g\n", val);
#else /* DUMB_STDIO */
      printf ("  %g\n", val);
#endif /* DUMB_STDIO */
    }
  if (ep->r0)
    printf ("warning: reserved0 is 0x%x\n", ep->r0);
  if (ep->r1)
    printf ("warning: reserved1 is 0x%x\n", ep->r1);
  if (ep->r2)
    printf ("warning: reserved2 is 0x%x\n", ep->r2);
  if (ep->r3)
    printf ("warning: reserved3 is 0x%x\n", ep->r3);
}
	
void
i386_float_info()
{
  struct fpstate *fpstatep;	/* fpstate defined in <machine/pcb.h> */
  char buf[sizeof (struct fpstate) + 2 * sizeof (int)];
  char fpvalid;
  char get_live_fp_info(), get_core_fp_info();
  void fp_reorder();
  
  if (have_inferior_p())
    fpvalid = get_live_fp_info(buf);
  else
    fpvalid = get_core_fp_info(buf);
  if (fpvalid == 0) 
    {
      printf("no floating point status saved\n");
      return;
    }
  
  fpstatep = (struct fpstate *)buf;

  /* 
   * Rearrange the registers so that they're in physical order.  Under 
   * Mach, we got them with the first one being the top of the stack.
   */
  fp_reorder((struct env387 *)fpstatep->state);

  print_387_status(fpstatep->status, (struct env387 *)fpstatep->state);
}

void
fp_reorder(ep)
     struct env387 *ep;
{
  int top;				/* which register is stack top */
  int stack, phys;
  unsigned char tmp_regs[8][10];

  bcopy(ep->regs, tmp_regs, sizeof(tmp_regs));
  top = (ep->status >> 11) & 7;
  for (stack = 0, phys = top; phys < 8; stack++, phys++)
    bcopy(tmp_regs[stack], ep->regs[phys], sizeof(tmp_regs[stack]));
  for (phys = 0; stack < 8; phys++, stack++)
    bcopy(tmp_regs[stack], ep->regs[phys], sizeof(tmp_regs[stack]));
}

/* 
 * Copy the fpstate struct into "buf" and return a flag telling
 * whether it's useful.
 */

char
get_live_fp_info(buf)
     char *buf;
{
#ifdef	i386_FLOAT_STATE_COUNT
  kern_return_t ret;
  int stateCnt = i386_FLOAT_STATE_COUNT;
  thread_state_data_t tstate;
  struct i386_float_state *state;
  struct fpstate *fps = (struct fpstate *)buf;

  if ((ret = thread_get_state(current_thread, i386_FLOAT_STATE, tstate,
			      &stateCnt)) != KERN_SUCCESS)
    {
      mach_error("can't get floating pt. state", ret);
      return 0;
    }
  state = (struct i386_float_state *)tstate;
  if (stateCnt < i386_FLOAT_STATE_COUNT)
    {
      fprintf(stderr, "unexpected floating point state size.\n");
      return 0;				/* old kernel? */
    }
  if (state->fpkind != FP_387 && state->fpkind != FP_SW)
    return 0;
  if (!state->initialized)
    return 0;

  /* 
   * Cons up an fpstate record, which contains the hardware state info 
   * followed by a second copy of the status word.  In this case we 
   * signify that the status word is to be ignored by making it zero.
   */
  bzero(fps, sizeof(struct fpstate));
  fps->status = state->exc_status;
  bcopy((char *)state->hw_state, fps->state, FP_STATE_BYTES);
  return 1;
#else
  return 0;
#endif	i386_FLOAT_STATE_COUNT
}


char
get_core_fp_info(buf)
     char *buf;
{
  struct user u; /* just for address computations */
  unsigned int uaddr;
  extern int corechan;
  int fpvalid = 0;

  /* 
   * If we can't read the "valid" flag, then assume it's 0.  If we
   * can't read the actual fp state, then quit now.
   */
  uaddr = (char *)&u.u_pcb.pcb_fpvalid - (char *)&u;
  if (lseek(corechan, uaddr, 0) < 0)
    perror("seek on core file");
  if (myread(corechan, &fpvalid, 1) < 0) 
    perror("read on core file");

  uaddr = (char *)&u.u_pcb.pcb_fps - (char *)&u;
  if (lseek(corechan, uaddr, 0) < 0)
    perror_with_name("seek on core file");
  if (myread(corechan, buf, sizeof (struct fpstate)) < 0)
    perror_with_name("read from core file");

  return (char)fpvalid;
}
  
/* Return number of args passed to a frame.
   Can return -1, meaning no way to tell.  */

/* on the 386, the instruction following the call could be:
 *  popl %ecx        -  one arg
 *  addl $imm, %esp  -  imm/4 args; imm may be 8 or 32 bits
 *  anything else    -  zero args
 */

int i386_frame_num_args( fi)
     struct frame_info *fi;
{
  int retpc;
  int numargs = -1;
  unsigned char op, mod_rm;
  struct frame_info *pfi;

  pfi = get_prev_frame_info ( fi);
  if ( pfi == NULL) return( -1); /* outermost call frame, i.e. main() */
  retpc = pfi->pc;
  op = read_memory_integer (retpc, 1);
  if (op == 0x59)  /* pop %ecx */ numargs = 1;
  else if (op == 0x83) {  /* group 1 opcode */
     mod_rm = read_memory_integer (retpc+1, 1);
     if ( mod_rm == 0xc4) /* addl $<signed imm 8 bits>, %esp */
       numargs = (read_memory_integer ( retpc + 2, 1) & 0xff)/4;
  } else if ( op == 0x81) { /* group 1 opcode */
    mod_rm = read_memory_integer (retpc+1, 1);
    if (mod_rm == 0xc4) /* addl $<imm 32>, %esp */
      numargs = read_memory_integer ( retpc + 2, 4) / 4;
  }
  return( numargs);
}

_initialize_signals() { }	/* I think egrep is hosed up */
