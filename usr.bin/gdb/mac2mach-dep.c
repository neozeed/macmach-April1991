/* Machine-dependent code which would otherwise be in inflow.c and core.c,
   for GDB, the GNU debugger.
   Copyright (C) 1986, 1987, 1989 Free Software Foundation, Inc.

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

/* mac2mach-dep.c */

#include <stdio.h>
#include "defs.h"
#include "param.h"
#include "frame.h"
#include "inferior.h"
#include "symtab.h"
#include "wait.h"

#include <sys/param.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <a.out.h>
#include <sys/file.h>
#include <sys/stat.h>

extern int errno;

#include <mach/message.h>
#include <mach/exception.h>
#include <mach/notify.h>
#include <mach/port.h>
#include <mach/machine/thread_status.h>
#include <mach/machine/frame.h>

#include <machine/pcb.h>

#include <cthreads.h>

/* *** WART WART WART *** */
/* the files cthread_internals.h and options.h are from libthreads sources */
#define	MTHREAD 1
#include "cthread_internals.h"
/* *** WART WART WART *** */

extern thread_t current_thread;

extern void mach_error();

/* This is one way to do it, not the nicest one */

void fetch_inferior_registers();
void store_inferior_registers();

#include "mach_os.c"


/* Get all registers from the inferior */

extern char registers[];

/* registers[] looks like this (see /usr/include/mac2/reg.h) */
/*
typedef struct { long fp[3]; } fp_reg_t;
  struct registers {
    long d0, d1, d2, d3, d4, d5, d6, d7;
    long a0, a1, a2, a3, a4, a5, fp, sp;
    long pc, sr;
    fp_reg_t f0, f1, f2, f3, f4, f5, f6, f7;
    unsigned long fps_control;
    unsigned long fps_status;
    unsigned long fps_iaddr;
  };
*/

/* The struct thread_state contains all of the thread state information
 * we will use.  The elements of this structure are read and written by
 * the thread_get_state() and thread_put_state() functions.  We then
 * convert between this and the GDB registers[] structure noted above.
 */
static struct thread_state {
  thread_state_regs_t s_regs;
  thread_state_fpregs_t s_fpregs;
  union {
    thread_state_frame_t raw;
    normal_exception_frame_t normal;
  } s_frame;
} state;

/* Read registers[] from the current_thread. */
void fetch_inferior_registers()
{
  register int regno;
  register int *regp;
  unsigned int stateCnt_regs = THREAD_STATE_REGS_COUNT;
  unsigned int stateCnt_fpregs = THREAD_STATE_FPREGS_COUNT;
  unsigned int stateCnt_frame = THREAD_STATE_FRAME_COUNT;
  kern_return_t ret;  

  /* if remote, someone else will do it */
  if (remote_debugging) {
    remote_fetch_registers(registers);
    return;
  }
  /* first, load the thread_state structure */
  if ((ret = thread_get_state(current_thread, THREAD_STATE_REGS,
      (thread_state_t)&state.s_regs, &stateCnt_regs)) != KERN_SUCCESS) {
    mach_error("thread_get_state (regs)", ret);
    error("in fetch_inferior_registers.");
    return;
  }
  if ((ret = thread_get_state(current_thread, THREAD_STATE_FPREGS,
      (thread_state_t)&state.s_fpregs, &stateCnt_fpregs)) != KERN_SUCCESS) {
    mach_error("thread_get_state (fpregs)", ret);
    error("in fetch_inferior_registers.");
    return;
  }
  if ((ret = thread_get_state(current_thread, THREAD_STATE_FRAME,
      (thread_state_t)&state.s_frame.raw, &stateCnt_frame)) != KERN_SUCCESS) {
    mach_error("thread_get_state (frame)", ret);
    error("in fetch_inferior_registers.");
    return;
  }
  /* copy thread_state into registers[] format */
  bcopy(&state.s_regs, registers, sizeof(state.s_regs));
  *((long *)&registers[REGISTER_BYTE(PC_REGNUM)]) = state.s_frame.normal.f_pc;
  *((long *)&registers[REGISTER_BYTE(PS_REGNUM)]) = state.s_frame.normal.f_sr;
  bcopy(&state.s_fpregs, &registers[REGISTER_BYTE(FP0_REGNUM)],
    sizeof(state.s_fpregs));
}

/* Write registers[] to the current_thread.
 * If REGNO is -1, write all registers, otherwise write just one.
 * Note that the entire thread_state structure will be written.  We assume
 * that the thread_state structure has been previously loaded by a call
 * to fetch_inferior_registers() above.
 */
void store_inferior_registers(regno)
register int regno;
{
  register int *regp;
  unsigned int stateCnt_regs = THREAD_STATE_REGS_COUNT;
  unsigned int stateCnt_fpregs = THREAD_STATE_FPREGS_COUNT;
  unsigned int stateCnt_frame = THREAD_STATE_FRAME_COUNT;
  kern_return_t ret;
  char registers_to_write[REGISTER_BYTES];
  
  /* if remote, someone else will do it */
  if (remote_debugging) {
    remote_store_registers(registers);
    return;
  }
  /* registers_to_write is copy of registers[] */
  if (regno == -1) bcopy(registers, registers_to_write, REGISTER_BYTES);
  else {
    /* when writing one register, use other values from current thread_state */
    bcopy(&state.s_regs, registers_to_write, sizeof(state.s_regs));
    *((long *)&registers[REGISTER_BYTE(PC_REGNUM)]) = state.s_frame.normal.f_pc;
    *((long *)&registers[REGISTER_BYTE(PS_REGNUM)]) = state.s_frame.normal.f_sr;
    bcopy(&state.s_fpregs, &registers[REGISTER_BYTE(FP0_REGNUM)],
      sizeof(state.s_fpregs));
    /* this is the one register we are interested in */
    bcopy(&registers[REGISTER_BYTE(regno)],
      &registers_to_write[REGISTER_BYTE(regno)],
      REGISTER_RAW_SIZE(regno));
  }
  /* convert registers_to_write[] into thread_state structure */
  bcopy(registers_to_write, &state.s_regs, sizeof(state.s_regs));
  state.s_frame.normal.f_pc =
    *((long *)&registers_to_write[REGISTER_BYTE(PC_REGNUM)]);
  state.s_frame.normal.f_sr =
    *((long *)&registers_to_write[REGISTER_BYTE(PS_REGNUM)]);
  bcopy(&registers_to_write[REGISTER_BYTE(FP0_REGNUM)], &state.s_fpregs,
    sizeof(state.s_fpregs));
  /* write thread_state to current thread */
  if ((ret = thread_set_state(current_thread, THREAD_STATE_REGS,
      (thread_state_t)&state.s_regs, stateCnt_regs)) != KERN_SUCCESS) {
    mach_error("thread_set_state (regs)", ret);
    error("in store_inferior_registers.");
  }
  if ((ret = thread_set_state(current_thread, THREAD_STATE_FPREGS,
      (thread_state_t)&state.s_fpregs, stateCnt_fpregs)) != KERN_SUCCESS) {
    mach_error("thread_set_state (fpregs)", ret);
    error("in store_inferior_registers.");
  }
  if ((ret = thread_set_state(current_thread, THREAD_STATE_FRAME,
      (thread_state_t)&state.s_frame, stateCnt_frame)) != KERN_SUCCESS) {
    mach_error("thread_set_state (frame)", ret);
    error("in store_inferior_registers.");
  }
}

/* Return the thread's stack pointer.  Used when mapping
 * kernel threads to Mach CThreads */
thread_stack_pointer(th)
thread_t th;
{
  thread_state_regs_t state;
  unsigned int stateCnt = THREAD_STATE_REGS_COUNT;
  if (thread_get_state(th, THREAD_STATE_REGS,
    (thread_state_t)&state, &stateCnt) != KERN_SUCCESS) return 0;
  return state.r_sp;
}

/* Machine-dependent code which would otherwise be in core.c */
/* Work with core dump and executable files, for GDB. */

/* See /usr/src/kernel/bsd/kern_sig.c for the routine that makes core files.
 * A core file consists of UPAGES block of the user.h area followed by the
 * entire data+stack segments.  In other words, it looks like this:
 *   0x0000: user
 *           .
 *           .
 *   0x2000: pcb
 *           .
 *           .
 *           kernel stack
 *   0x4000:
 */

#define	core_user ((struct user *) \
  &corestr[ctob(0)])
#define core_pcb ((struct pcb *) \
  &corestr[ctob(1)])
#define core_pcb_frame ((normal_exception_frame_t *) \
  (&corestr[ctob(1)] + (0x1FFF & (int)core_pcb->pcb_frame)))

#ifndef N_TXTADDR
#define N_TXTADDR(hdr) 0
#endif /* no N_TXTADDR */

#ifndef N_DATADDR
#define N_DATADDR(hdr) hdr.a_text
#endif /* no N_DATADDR */

/* Non-zero if this is an object (.o) file, rather than an executable.
 * Distinguishing between the two is rarely necessary (and seems like
 * a hack, but there is no other way to get the text and data
 * addresses--N_TXTADDR should probably take care of this, but it doesn't).
 * This definition will not work if someone decides to make ld preserve
 * relocation info.
 */
#define IS_OBJECT_FILE(hdr) (hdr.a_trsize != 0)
  
/* Make COFF and non-COFF names for things a little more compatible
 * to reduce conditionals later.
 */
#ifdef COFF_FORMAT
#define a_magic magic
#endif
#ifndef COFF_FORMAT
#ifndef AOUTHDR
#define AOUTHDR struct exec
#endif
#endif

/* Signal names. */
extern char *sys_siglist[];

/* Hook for `exec_file_command' command to call.  */
extern void (*exec_file_display_hook) ();
   
/* File names of core file and executable file.  */
extern char *corefile;
extern char *execfile;

/* Descriptors on which core file and executable file are open.
 * Note that the execchan is closed when an inferior is created
 * and reopened if the inferior dies or is killed.
 */
extern int corechan;
extern int execchan;

/* Last modification time of executable file.
 * Also used in source.c to compare against mtime of a source file.
 */
extern int exec_mtime;

/* Virtual addresses of bounds of the two areas of memory in the core file.  */
extern CORE_ADDR data_start;
extern CORE_ADDR data_end;
extern CORE_ADDR stack_start;
extern CORE_ADDR stack_end;

/* Virtual addresses of bounds of two areas of memory in the exec file.
 * Note that the data area in the exec file is used only when there is
 * no core file.
 */
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

/* a.out header of exec file.  */
extern AOUTHDR exec_aouthdr;

extern void validate_files ();

core_file_command (filename, from_tty)
char *filename;
int from_tty;
{
  int i;
  char corestr[ctob(UPAGES)];
  extern char registers[];

  /* Discard all vestiges of any previous core file
   * and mark data and stack spaces as empty.
   */
  if (corefile) free(corefile);
  corefile = 0;
  if (corechan >= 0) close(corechan);
  corechan = -1;
  data_start = 0;
  data_end = 0;
  stack_start = STACK_END_ADDR;
  stack_end = STACK_END_ADDR;
  /* Now, if a new core file was specified, open it and digest it.  */
  if (!filename) {
    if (from_tty) printf ("No core file now.\n");
    return;
  }
  filename = tilde_expand (filename);
  make_cleanup (free, filename);
  if (have_inferior_p ())
    error("To look at a core file, you must kill the inferior with \"kill\".");
  if ((corechan = open(filename, O_RDONLY, 0)) < 0)
    perror_with_name (filename);
  if ((i = myread(corechan, corestr, sizeof(corestr))) < 0)
    perror_with_name (filename);
  if (i < sizeof(corestr))
    error ("\"%s\" does not appear to be a core dump file (size < %d)",
      filename, sizeof(corestr));
  bcopy(&core_pcb->pcb_user, registers, sizeof(core_pcb->pcb_user));
  *(long *)&registers[REGISTER_BYTE(PC_REGNUM)] = core_pcb_frame->f_pc;
  *(long *)&registers[REGISTER_BYTE(PS_REGNUM)] = core_pcb_frame->f_sr;
  bcopy(&core_pcb->pcb_fp_state,
    &registers[REGISTER_BYTE(FP0_REGNUM)], sizeof(core_pcb->pcb_fp_state));
  data_start = exec_data_start;
  data_end = data_start + NBPG * core_user->u_dsize;
  data_offset = NBPG * UPAGES;
  stack_start = stack_end - NBPG * core_user->u_ssize;
  stack_offset = NBPG * (UPAGES + core_user->u_dsize);
  printf ("Core file is from \"%s\".\n", core_user->u_comm);
  if (core_user->u_arg[0] > 0)
    printf ("Program terminated with signal %d, %s.\n",
      core_user->u_arg[0],
      core_user->u_arg[0] < NSIG ?
        sys_siglist[core_user->u_arg[0]] : "(undocumented)");
  if (filename[0] == '/') corefile = savestring(filename, strlen(filename));
  else corefile = concat(current_directory, "/", filename);
  set_current_frame(create_new_frame(read_register(FP_REGNUM), read_pc()));
  select_frame(get_current_frame(), 0);
  validate_files();
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

	if (read_section_hdr (execchan, _TEXT, &text_hdr, num_sections,
			      aout_hdrsize) < 0)
	  error ("\"%s\": can't read text section header", execfile);

	if (read_section_hdr (execchan, _DATA, &data_hdr, num_sections,
			      aout_hdrsize) < 0)
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
	val = myread (execchan, &exec_aouthdr, sizeof (AOUTHDR));

	if (val < 0)
	  perror_with_name (filename);

	text_start =
	  IS_OBJECT_FILE (exec_aouthdr) ? 0 : N_TXTADDR (exec_aouthdr);
        exec_data_start = IS_OBJECT_FILE (exec_aouthdr)
	  ? exec_aouthdr.a_text : N_DATADDR (exec_aouthdr);
	text_offset = N_TXTOFF (exec_aouthdr);
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

_initialize_signals() { }	/* I think egrep is hosed up */

