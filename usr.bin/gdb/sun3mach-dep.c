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

/*
 * HISTORY
 * $Log:	sun3mach-dep.c,v $
 * Revision 3.2  90/04/11  14:41:57  af
 * 	Moved a bunch of decls in the MI part, where they belong.
 * 
 */

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
#include <sun/core.h>

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


/* This is one way to do it, not the nicest one */

#include "mach_os.c"


/* Get all registers from the inferior */

extern char registers[];

void
fetch_inferior_registers ()
{
  register int regno;
  register int *regp;
  struct sun_thread_state state;
  int stateCnt = SUN_THREAD_STATE_REGS_COUNT;
  kern_return_t ret;  

  if (remote_debugging)
    {
      remote_fetch_registers(registers);
      return;
    }

  if ((ret = thread_get_state(current_thread, SUN_THREAD_STATE_REGS, &state,
			      &stateCnt)) != KERN_SUCCESS) 
    {
      mach_error("thread_get_state", ret);
      error("in fetch_inferior_registers.");
      return;
    }

  bcopy( &state, registers, sizeof(state));

}

/* Store our register values back into the inferior.
   If REGNO is -1, do this for all registers.
   Otherwise, REGNO specifies which register (so we can waste more time).  */

store_inferior_registers (regno)
     register int regno;
{
  register int *regp;
  struct sun_thread_state state;
  int stateCnt;
  kern_return_t ret;
  
  if (remote_debugging)
    {
      remote_store_registers(registers);
      return;
    }

  if (regno == -1)
    {
      bcopy(registers, &state, sizeof(state));

      stateCnt = SUN_THREAD_STATE_REGS_COUNT;
      if ((ret = thread_set_state(current_thread, SUN_THREAD_STATE_REGS, &state,
			      	  stateCnt)) != KERN_SUCCESS) 
        {
          mach_error("thread_set_state", ret);
          error("in store_inferior_registers.");
        }
    }
  else
    {
      stateCnt = SUN_THREAD_STATE_REGS_COUNT;
      if ((ret = thread_get_state(current_thread, SUN_THREAD_STATE_REGS, &state,
			      &stateCnt)) != KERN_SUCCESS) 
        {
           mach_error("thread_get_state", ret);
           error("in store_inferior_register.");
        }

      regp = &state.d0;
      regp[regno] = read_register(regno);

      stateCnt = SUN_THREAD_STATE_REGS_COUNT;
      if ((ret = thread_set_state(current_thread, SUN_THREAD_STATE_REGS, &state,
					stateCnt)) != KERN_SUCCESS) 
        {
          mach_error("thread_set_state", ret);
          error("in store_inferior_register.");
        }
    }
}

/* Return the thread's stack pointer.  Used when mapping
   kernel threads to Mach CThreads */

thread_stack_pointer(th)
	thread_t th;
{
  struct sun_thread_state state;
  int stateCnt = SUN_THREAD_STATE_REGS_COUNT;
  if (thread_get_state(th, SUN_THREAD_STATE_REGS, &state,&stateCnt) != KERN_SUCCESS) 
    return 0;
  return state.sp;
}



/* Machine-dependent code which would otherwise be in core.c */
/* Work with core dump and executable files, for GDB. */

#ifndef N_TXTADDR
#define N_TXTADDR(hdr) 0
#endif /* no N_TXTADDR */

#ifndef N_DATADDR
#define N_DATADDR(hdr) hdr.a_text
#endif /* no N_DATADDR */

/* Non-zero if this is an object (.o) file, rather than an executable.
   Distinguishing between the two is rarely necessary (and seems like
   a hack, but there is no other way to get the text and data
   addresses--N_TXTADDR should probably take care of
   this, but it doesn't).  */
/* This definition will not work
   if someone decides to make ld preserve relocation info.  */
#define IS_OBJECT_FILE(hdr) (hdr.a_trsize != 0)
  
/* Make COFF and non-COFF names for things a little more compatible
   to reduce conditionals later.  */

#ifdef COFF_FORMAT
#define a_magic magic
#endif

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

      {
	struct core corestr;

	val = myread (corechan, &corestr, sizeof corestr);
	if (val < 0)
	  perror_with_name (filename);
	if (corestr.c_magic != CORE_MAGIC)
	  error ("\"%s\" does not appear to be a core dump file (magic 0x%x, expected 0x%x)",
		 filename, corestr.c_magic, (int) CORE_MAGIC);
	else if (sizeof (struct core) != corestr.c_len)
	  error ("\"%s\" has an invalid struct core length (%d, expected %d)",
		 filename, corestr.c_len, (int) sizeof (struct core));

	data_start = exec_data_start;
	data_end = data_start + corestr.c_dsize;
	stack_start = stack_end - corestr.c_ssize;
	data_offset = sizeof corestr;
	stack_offset = sizeof corestr + corestr.c_dsize;

	bcopy (&corestr.c_regs, registers, 16 * 4);
	*(int *)&registers[REGISTER_BYTE (PS_REGNUM)] = corestr.c_regs.r_ps;
	*(int *)&registers[REGISTER_BYTE (PC_REGNUM)] = corestr.c_regs.r_pc;
#ifdef FP0_REGNUM
#ifdef FPU
     bcopy (corestr.c_fpu.f_fpstatus.fps_regs,
            &registers[REGISTER_BYTE (FP0_REGNUM)],
            sizeof corestr.c_fpu.f_fpstatus.fps_regs);
     bcopy (&corestr.c_fpu.f_fpstatus.fps_control,
            &registers[REGISTER_BYTE (FPC_REGNUM)],
            sizeof corestr.c_fpu.f_fpstatus - sizeof corestr.c_fpu.f_fpstatus.fps_regs);
#else
	bcopy (corestr.c_fpstatus.fps_regs,
	       &registers[REGISTER_BYTE (FP0_REGNUM)],
	       sizeof corestr.c_fpstatus.fps_regs);
	bcopy (&corestr.c_fpstatus.fps_control,
	       &registers[REGISTER_BYTE (FPC_REGNUM)],
	       sizeof corestr.c_fpstatus - sizeof corestr.c_fpstatus.fps_regs);
#endif
#endif
	bcopy (&corestr.c_aouthdr, &core_aouthdr, sizeof (struct exec));

	printf ("Core file is from \"%s\".\n", corestr.c_cmdname);
	if (corestr.c_signo > 0)
	  printf ("Program terminated with signal %d, %s.\n",
			corestr.c_signo,
			corestr.c_signo < NSIG
			? sys_siglist[corestr.c_signo]
			: "(undocumented)");
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

