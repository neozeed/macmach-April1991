/* Print and select threads for GDB, the GNU debugger, on Mach 2.5 systems.
   Copyright (C) 1986, 1987 Free Software Foundation, Inc.

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
 * $Log:	mach_os.c,v $
 * Revision 2.2  90/02/22  10:18:44  kupfer
 * New file for Mach support.
 * 
 * Revision 1.4  89/11/03  18:46:08  kupfer
 * Change so that when you run config.gdb you can specify either COFF or
 * Berkeley-style a.out.  Cleaned up some configuration ifdef's.
 * 
 * Revision 1.3  89/10/10  16:09:27  kupfer
 * Hacks to get working with Mach 2.5.
 * 
 * Revision 1.2  89/08/08  20:49:45  kupfer
 * Header files moved for X79.
 * 
 */

/* Exported globals */

boolean_t inferior_is_stopped;	/* Has the inferior task been suspended? */

/* interface with catch_exception_raise: */
int stop_exception, stop_code, stop_subcode;

task_t inferior_task;
thread_t current_thread;
thread_t stop_thread;

port_t inferior_exception_port, inferior_old_exception_port;
boolean_t stopped_in_ptrace, other_threads_suspended;

/* 
 * When single-stepping, we wait for an exception from the 
 * single-stepped thread.  Otherwise, we do an unrestricted wait.  
 * Rather than creating and destroying a port each time we do a 
 * single-step, we cache a port here and reuse it as necessary. 
 */
port_t cached_exception_port = PORT_NULL;
port_t current_wait_port;
port_set_name_t all_enabled_ports;


/* Forward decls */

void		free_cprocs();
struct cproc	*get_cprocs();
cthread_t	selected_thread;



/* This function calls ptrace to enable debugging of the inferior
   process.  It is called by the inferior itself after forking.
   It sets up some U*x state that has nothing to do with Mach. */
int
call_ptrace (request, pid, arg3, arg4)
     int request, pid, arg3, arg4;
{
  if (request != 0 /* TRACE_ME */)
    fatal("Bogus ptrace call");
  return ptrace (0,0,0,0);
}

kill_inferior ()
{
  kern_return_t ret;
  WAITTYPE w;

  if (remote_debugging || inferior_pid == 0 || inferior_task == PORT_NULL)
    return;

  while (1) 
    {
      kill (inferior_pid, SIGHUP);
      (void) task_resume(inferior_task);
      if ((wait (&w) == inferior_pid) && (w.w_stopval != WSTOPPED))
	{
	  inferior_task = PORT_NULL;
	  inferior_died ();
	  return;
	}
    }
}

/* This is used when GDB is exiting.  It gives less chance of error.*/

kill_inferior_fast ()
{
  if (remote_debugging)
    return;
  if (inferior_pid == 0)
    return;
  ptrace (8, inferior_pid, 0, 0);	/* XXX Cannot use task_terminate() XXX */
  wait (0);
}


void
set_exception_port(thread)
     thread_t thread;
{
  port_t exc_port;
  kern_return_t ret;
  
  exc_port = cached_exception_port;
  if (exc_port == PORT_NULL)
    {
      ret = port_allocate(task_self(), &exc_port);
      if (ret != KERN_SUCCESS)
	  fatal("set_exc_port: port_allocate [%d] %s\n",
		  ret, mach_error_string(ret));

      cached_exception_port = exc_port;
    }

  ret = thread_set_special_port(thread, THREAD_EXCEPTION_PORT, exc_port);
  if (ret != KERN_SUCCESS)
      fatal("set_exc_port: thread_set_special_port [%d] %s\n",
	      ret, mach_error_string(ret));

  current_wait_port = exc_port;	
}

void
clear_exception_port(thread)
     thread_t thread;
{
  port_t exc_port;
  kern_return_t ret;
  
  ret = thread_get_special_port(thread, THREAD_EXCEPTION_PORT, &exc_port);
  if (ret != KERN_SUCCESS)
    fatal("clear_exc_port: thread_get_special_port [%d] %s\n",
	      ret, mach_error_string(ret));

  if (exc_port == PORT_NULL)
    {
      fprintf(stderr, "clear_exc_port: wrong thread!\n");
    }
  ret = thread_set_special_port(thread, THREAD_EXCEPTION_PORT, PORT_NULL);
  if (ret != KERN_SUCCESS)
    fatal("clear_exc_port: thread_set_special_port [%d] %s\n",
	      ret, mach_error_string(ret));

  current_wait_port = (port_t) all_enabled_ports;
}


mach_create_inferior_hook(pid)
	int pid;
{
  thread_array_t thread_list;
  kern_return_t ret;
#ifdef mac2
  unsigned int thread_count;
#else
  int thread_count;
#endif
  char *err = 0;
#define CHECK(s) if (ret != KERN_SUCCESS) {err = s; goto error_exit;}

  ret = port_allocate(task_self(), &inferior_exception_port);
  CHECK("port_allocate")

  ret = port_set_add(task_self(), all_enabled_ports, inferior_exception_port);
  CHECK("port_set_add")

  ret = task_by_unix_pid(task_self(), pid, &inferior_task);
  CHECK("task_by_unix_pid")

  ret = task_threads (inferior_task, &thread_list, &thread_count);
  CHECK ("task_threads")

  current_thread = thread_list[0];

#ifdef mac2
  (void) vm_deallocate(task_self(), (vm_address_t)thread_list, (thread_count * sizeof(int)));
#else
  (void) vm_deallocate(task_self(), thread_list, (thread_count * sizeof(int)));
#endif

  ret = task_get_special_port (inferior_task, TASK_EXCEPTION_PORT,
			       &inferior_old_exception_port);
  CHECK ("task_get_special_port(old exc)")

  ret = task_set_special_port(inferior_task, TASK_EXCEPTION_PORT, 
			      inferior_exception_port);
  CHECK("task_set_special_port")

  return;
error_exit:
  mach_error(err, ret);
  error("in create_inferior_hook");  
}

/* Wait for some action by the inferior.  
   
   If the inferior gets an exception, then we return 0.  If the
   inferior dies, then inferior_task will be null.  Otherwise, the
   exception will be stored in stop_exception et al.
   
   Otherwise, we return non-zero (meaning that the inferior was
   stopped by something other than an exception, e.g., a signal), and
   the status from wait() is stored in "w". */
int
mach_really_wait (w)
     WAITTYPE *w;
{
  int pid;
  kern_return_t ret;

  struct msg {
    msg_header_t    header;
    int             data[1024];
  } in_msg, out_msg;

  /* Here's where we do the waiting part. */
  stop_exception = stop_code = stop_subcode = -1;
  while (1)
    {
      QUIT;

      stopped_in_ptrace = 0;

      /* Check whether it sent us an exception */
      in_msg.header.msg_local_port = current_wait_port; 
      in_msg.header.msg_size = sizeof(in_msg); 
      ret = msg_receive (&in_msg.header, RCV_TIMEOUT, 1000); 
      if (ret == KERN_SUCCESS)
	{
	  /* If the message came on our notify port someone may 
	     have died. */
	  if (in_msg.header.msg_local_port == task_notify())
	    {
	      process_notify_msg (&in_msg);
	      /* If inferior_task is null then the inferior has
	         gone away and we want to return to command level.
	         Otherwise it was just an informative message and we
	         need to look to see if there are any more. */
	      if (inferior_task == PORT_NULL)
	        {
		  wait3(w, WNOHANG, 0);
		  stopped_in_ptrace = 1;
		  return inferior_pid;
	        }
	      else
		continue;
	    }

	  /* Not a notify message.  The server will decide if the user
	     needs to be involved and either suspend the task or just
	     get it ready to go. */
	  exc_server (&in_msg.header, &out_msg.header);

	  /* He will try and run, but he is suspended, so all he will
	     do is be ready to leave the rpc.  We would prefer to do
	     this in resume(), but this keeps all of the message stuff
	     localized. */
	  msg_send (&out_msg.header, MSG_OPTION_NONE, 0);

	  if (inferior_is_stopped)
	    return inferior_pid;
	  else
	    continue;
	} /* if ret == RCV_SUCCESS ... */
      
      /* Notice that on the first SIGTRAPs sent from execve we
         don't get a msg on the exception port. Thus all of this
         business with wait3() is still necessary. */
      pid = wait3 (w, WNOHANG, 0);
      if (pid != 0) {
	  stopped_in_ptrace = 1;
	  return pid;
      }
    } /* while (1) ... */
}


/* this stuff here is an upcall via libmach/excServer.c 
   and mach_really_wait which does the actual upcall.  */

kern_return_t
catch_exception_raise (port, thread, task, exception, code, subcode)
     port_t port;
     thread_t thread;
     task_t task;
     int exception, code, subcode;
{
  kern_return_t ret;
  extern running_in_shell;
  int fwd_exc_p();			/* do we forward this exception? */

  inferior_task = task;
  stop_thread = current_thread = thread;
  stop_exception = exception;
  stop_code = code;
  stop_subcode = subcode;  

  /* If the shell hasn't exec'd the debuggee yet and the exception is
     an address fault, convert it to a segmentation violation.  This
     is to accomodate /bin/sh's strange memory management techniques.  */

  if (running_in_shell && exception == EXC_BAD_ACCESS)
    {
      kill(inferior_pid, SIGSEGV);
      inferior_is_stopped = FALSE;
      return KERN_SUCCESS;
    }
    
  /* Otherwise, check whether the (gdb) user wants this exception 
     forwarded to the old exception handler.  This is normally used to
     pass exceptions to the default exception server, which maps the
     exception into a Unix signal.  The user might want to do this in
     case the inferior has a signal handler written to deal with the
     exception (e.g., segmentation violation or floating point
     exception).  Note, however, that if you're debugging gdb, then
     the target gdb's old exception handler is the top gdb.  */

  if (fwd_exc_p (exception, code))
    {
      port_t reply_port;
      
      port_allocate (task_self(), &reply_port);
      ret = exception_raise (inferior_old_exception_port, reply_port, thread, 
			     task, exception, code, subcode);
      port_deallocate (task_self(), reply_port);
      inferior_is_stopped = FALSE;
      return ret;
    }

  /* Otherwise, handle the exception directly.
     Suspend the inferior so the other threads don't run away.  */
  if ((ret = task_suspend (task)) != KERN_SUCCESS)
    {
      mach_error ("task_suspend", ret);
      error ("Error suspending victim after exception.\n");
    }
  inferior_is_stopped = 1;

  /* If the exception is from a single-step command, we need to put
     things back.  */
  if (other_threads_suspended == 1)
    {
      if ((ret = thread_suspend (current_thread)) != KERN_SUCCESS) {
	mach_error ("thread_suspend", ret);
	error("in catch_exception_raise");
      }
      clear_exception_port(current_thread);
      clear_trace_bit (current_thread);
      resume_all_threads ();
      other_threads_suspended = FALSE;
    }

  return KERN_SUCCESS;
}


process_notify_msg (msg)
     notification_t *msg;
{
  extern int attach_flag;

  if (msg->notify_header.msg_id == NOTIFY_PORT_DELETED)
    {
      /* did the whole task sink?  */
      if (msg->notify_port == inferior_task)
	{
	  inferior_died ();
	  inferior_task = PORT_NULL;
	  return;
	}
      /* how about one of the threads?  */
      if (msg->notify_port == current_thread)
	{
	  if (attach_flag)
	    printf ("The current thread is dead.\n");
	  return;
	}

      /* was it just a thread?  If so, adjust the non-existant thread
       * table.  */
#if 0
      LOOP_THROUGH_THREAD_TABLE()
	{
	  if (t->port == msg->notify_port)
	    {
	      t->port = PORT_NULL;
	      return;
	    }
	}
#endif	/* 0 */
    }

  /* Hmmm: we don't know how to deal with anything else.  */
  return;
}


/* Resume execution of the inferior process.
   If STEP is nonzero, single-step it.
   If SIGNAL is nonzero, give it that signal.  */


resume (step, signal)
     int step;
     int signal;
{
  kern_return_t	ret;
  extern boolean_t stopped_in_ptrace, vm_read_cache_valid;

  errno = 0;
  vm_read_cache_valid = FALSE;

  if (stopped_in_ptrace) 
    {
      if (remote_debugging)
	remote_resume (step, signal);
      else
	{
#ifdef	NO_SINGLE_STEP
	  if (step)
	    single_step(signal);
	  else
#endif
	  ptrace (step ? 9 : 7, inferior_pid, 1, signal);
	  if (errno)
	    perror_with_name ("ptrace");
	}
    }
  else
    {
      if (step)
	{
	  suspend_all_threads ();
	  set_trace_bit (current_thread);
	  set_exception_port(current_thread);
	  if ((ret = thread_resume (current_thread)) != KERN_SUCCESS)
	    {
	      mach_error ("thread_resume", ret);
	      error("in resume.");
	    }
	  other_threads_suspended = TRUE;
	}
      else
	other_threads_suspended = FALSE;
      if ((ret = task_resume (inferior_task)) != KERN_SUCCESS)
	{
	  mach_error("task_resume", ret);
	  error("in resume.");
	}
      /* HACK HACK This is needed by the multiserver system HACK HACK */
      while ((ret = task_resume(inferior_task)) == KERN_SUCCESS)
	      /* make sure it really runs */;
      /* HACK HACK This is needed by the multiserver system HACK HACK */
    }
}


#ifdef ATTACH_DETACH

/* Start debugging the process whose number is PID.  */

attach (pid)
     int pid;
{
  void fetch_inferior_registers();
  thread_array_t thread_list;
#ifdef mac2
  unsigned int thread_count;
#else
  int thread_count;
#endif
  kern_return_t ret;
#ifdef mac2
  extern int attach_flag;
#endif

  ret = task_by_unix_pid(task_self(), pid, &inferior_task);
  if (ret != KERN_SUCCESS)
    error("Could not get %d's task port", pid);
  ret = task_suspend (inferior_task);
  if (ret != KERN_SUCCESS)
    error ("task_suspend", mach_error_string (ret));

  stopped_in_ptrace = 0;

  ret = task_threads (inferior_task, &thread_list, &thread_count);
  if (ret != KERN_SUCCESS)
	error("task_threads", mach_error_string ( ret));

  current_thread = thread_list[0];
#ifdef mac2
  vm_deallocate (task_self(), (vm_address_t)thread_list, (thread_count * sizeof(port_t)));
#else
  vm_deallocate (task_self(), thread_list, (thread_count * sizeof(port_t)));
#endif

  (void) port_allocate(task_self(), &inferior_exception_port);
  (void) port_set_add(task_self(), all_enabled_ports, inferior_exception_port);

  ret = task_get_special_port(inferior_task, TASK_EXCEPTION_PORT,
			      &inferior_old_exception_port);
  if (ret != KERN_SUCCESS)
    error("task_get_special_port", mach_error_string ( ret));

  ret = task_set_special_port(inferior_task, TASK_EXCEPTION_PORT, 
			      inferior_exception_port);
  if (ret != KERN_SUCCESS)
    error("task_set_special_port", mach_error_string ( ret));
  
  /* The task is attached allright: the following
     makes happy have_inferior_p() */
  inferior_pid = pid;

  fetch_inferior_registers();
  stop_pc = read_pc();
  set_current_frame (create_new_frame (read_register (FP_REGNUM), stop_pc));
  stop_frame_address = FRAME_FP (get_current_frame ());

  attach_flag = 1;
  return pid;
}

/* Stop debugging the process whose number is PID
   and continue it with signal number SIGNAL.
   SIGNAL = 0 means just continue it.  */

void
detach (signal)
     int signal;
{
  kern_return_t ret;
#ifdef mac2
  extern int attach_flag;
#endif

  ret = task_set_special_port (inferior_task, TASK_EXCEPTION_PORT, 
			       inferior_old_exception_port);
  if (ret != KERN_SUCCESS)
    error("task_set_special_port", mach_error_string ( ret));

  attach_flag = 0;

  if (signal)
    kill(inferior_pid,signal);

  /* Now, on a multi the task might be dead by now */
  (void) task_resume (inferior_task);
}
#endif /* ATTACH_DETACH */

/* Copy LEN bytes from inferior's memory starting at MEMADDR
   to debugger memory starting at MYADDR. 
   On failure (cannot read from inferior, usually because address is out
   of bounds) calls error(), because too many invocations do not
   check the return value anyways!. */

boolean_t vm_read_cache_valid = FALSE;

read_inferior_memory (memaddr, myaddr, len)
     CORE_ADDR memaddr;
     char *myaddr;
     int len;
{
  vm_offset_t vm_addr;
  vm_size_t vm_size;
  kern_return_t ret;
  static pointer_t buff = 0;
#ifdef mac2
  static unsigned int buff_size = 0;
#else
  static int buff_size = 0;
#endif
  static vm_offset_t last_vm_addr = 0;
  static vm_size_t last_vm_size = 0;
  static boolean_t vm_cache_in_use = FALSE;

  /* compute page aligned numbers */
  vm_addr = trunc_page(memaddr);
  vm_size = round_page(len);
  
  if ( ((vm_offset_t) ((vm_offset_t) memaddr + (vm_size_t) len)) > (
  		vm_addr + vm_size)) {
      vm_size += vm_page_size;
  }

  /* Do we really need to do a vm_read? */
  if ((vm_read_cache_valid == FALSE)
      || (vm_cache_in_use == FALSE)
      || (vm_addr != last_vm_addr)
      || (vm_size != last_vm_size)) {

    /* 
      * deallocate the old cache if it is being used
     */
    if (vm_cache_in_use) {
      vm_cache_in_use = FALSE;
      ret = vm_deallocate(task_self(), (vm_offset_t) buff,
			  (vm_size_t) buff_size);
      if (ret != KERN_SUCCESS) {
	mach_error("vm_deallocate", ret);
	error("Error handling gdb's memory cache.");
      }
    }

    ret = vm_read (inferior_task, vm_addr, vm_size, &buff, &buff_size);
    if (ret != KERN_SUCCESS) {
      vm_read_cache_valid = FALSE;
      mach_error ("vm_read", ret);
      *myaddr = 0;
      error("Error reading inferior's memory at %x.", vm_addr);
    }
  }

  /* copy the relevant bytes to our caller */
  bcopy((char *)(buff + (memaddr % vm_page_size)), myaddr, len);
  last_vm_addr = vm_addr;
  last_vm_size = vm_size;
  vm_cache_in_use = TRUE;
  vm_read_cache_valid = TRUE;
  return (0);
}



vm_get_prot(task, where, how_much, start, size, protection, max_protection)
	task_t		task;
	vm_offset_t where;
	vm_size_t how_much;
	vm_offset_t *start;
	vm_size_t *size;
	vm_prot_t	*protection;
	vm_prot_t	*max_protection;
{
	vm_inherit_t	inheritance;
	boolean_t	shared;
	port_t		object_name;
	vm_offset_t	offset;
	kern_return_t	r;

	*start = where;
	*size  = how_much;
	(void) vm_region(task, start, size,
				protection, max_protection,
				&inheritance, &shared,
				&object_name, &offset);
}

/* Copy LEN bytes of data from debugger memory at MYADDR
   to inferior's memory at MEMADDR.
   On failure (cannot write the inferior)
   returns EFAULT (the expected bad value of errno).  */

int
write_inferior_memory (memaddr, myaddr, len)
     CORE_ADDR memaddr;
     char *myaddr;
     int len;
{
  vm_offset_t vm_addr;
  vm_size_t vm_size;
  pointer_t buff;
#ifdef mac2
  unsigned int buff_size;
#else
  int buff_size;
#endif
  vm_prot_t prot;
  kern_return_t ret;
  char *err = 0;

  if (remote_debugging)
    {
      register int i;
      /* Round starting address down to longword boundary.  */
      register CORE_ADDR addr = memaddr & - sizeof (int);
      /* Round ending address up; get number of longwords that makes.  */
      register int count
        = (((memaddr + len) - addr) + sizeof (int) - 1) / sizeof (int);
      /* Allocate buffer of that many longwords.  */
      register int *buffer = (int *) xmalloc (count * sizeof (int));
      extern int errno;

      /* Fill start and end extra bytes of buffer with existing memory data.  */

      buffer[0] = remote_fetch_word (addr);
      if (count > 1)
	buffer[count - 1]
	  = remote_fetch_word (addr + (count - 1) * sizeof (int));

      /* Copy data to be written over corresponding part of buffer */

      bcopy (myaddr, (char *) buffer + (memaddr & (sizeof (int) - 1)), len);

      /* Write the entire buffer.  */

      errno = 0;
      for (i = 0; i < count; i++, addr += sizeof (int))
        {
	  remote_store_word (addr, buffer[i]);
          if (errno)
	    break;
        }

      free(buffer);
      return errno;
    }

  /* compute page-aligned numbers */
  vm_addr = trunc_page(memaddr);
  vm_size = round_page(len);

  /* read the whole thing */
  ret = vm_read (inferior_task, vm_addr, vm_size, &buff, &buff_size);
  if (ret != KERN_SUCCESS) {
    mach_error ("write_inferior_memory: vm_read", ret);
    return EFAULT;
  }

  /* diddle just what we want */
  bcopy(myaddr, (char *) buff + (memaddr %vm_page_size), len);

  /* Write the whole thing back.
   * If we get a protection failure, up the protection so that
   * we CAN write, do the write, and put it back.
   */
  
  ret = vm_write (inferior_task, vm_addr, buff, vm_size);
  if (ret == KERN_PROTECTION_FAILURE) {    
    vm_offset_t start, where;
    vm_size_t size, how_much;
    vm_prot_t prot, maxp;

again:
    vm_get_prot(inferior_task, vm_addr, vm_size, &start, &size, &prot, &maxp);
    ret = vm_protect (inferior_task, start, size, TRUE,  maxp | VM_PROT_WRITE);
    ret = vm_protect (inferior_task, start, size, FALSE, prot | VM_PROT_WRITE);
    if (ret != KERN_SUCCESS) {
      err = "write_inferior_memory: vm_protect";
      goto out;
    }
    /* Find out how much this time through */
    how_much = vm_size;
    if (start + size < vm_addr + vm_size)
      how_much = (start + size) - vm_addr;

    ret = vm_write (inferior_task, vm_addr, buff, how_much);
    if (ret != KERN_SUCCESS) {
      err = "write_inferior_memory: vm_write";
      goto out;
    }

    ret = vm_protect (inferior_task, start, size, TRUE,  maxp);
    ret = vm_protect (inferior_task, start, size, FALSE, prot);
    if (ret != KERN_SUCCESS) {
      err = "write_inferior_memory: vm_protect";
      goto out;
    }
    /* All done ? */
    if (how_much != vm_size)
      {
	vm_size -= how_much;
	vm_addr += how_much;
        goto again;
      }
  } else if (ret != KERN_SUCCESS) {
    err = "write_inferior_memory: vm_write";
    goto out;
  }

out:
  if (err)
    mach_error(err, ret);

  /* deallocate that buffer we needed.. */
  (void) vm_deallocate(task_self(), (vm_offset_t) buff, (vm_size_t) buff_size);

  /* just in case.. */
  vm_read_cache_valid = FALSE;

  return (err) ? EFAULT : 0;
}


static char
translate_state(state)
int	state;
{
  switch (state) {
  case TH_STATE_RUNNING:	return('R');
  case TH_STATE_STOPPED:	return('S');
  case TH_STATE_WAITING:	return('W');
  case TH_STATE_UNINTERRUPTIBLE: return('U');
  case TH_STATE_HALTED:		return('H');
  default:			return('?');
  }
}

static char buf[5];

static char *
get_thread_name(th, n, cprocs)
thread_t th;
int n;
cproc_t cprocs;
{
    unsigned sp;
    sp = thread_stack_pointer(th);
    while (cprocs != NO_CPROC) {
	if (sp > cprocs->stack_base &&
	    sp <= cprocs->stack_base + cprocs->stack_size) {
	    if (cprocs->incarnation != NULL) {
		return (cprocs->incarnation->name);
	    } else {
		sprintf(buf, "_t%d", n);
		return buf;
	    }
	}
	cprocs = cprocs->link;
    }
    sprintf(buf, "_t%d", n);
    return buf;
}


void
thread_list_command()
{
    int i, current_thread_index;
    thread_array_t thread_table;
#ifdef mac2
    unsigned int thread_count;
#else
    int thread_count;
#endif
    thread_basic_info_data_t ths;
    cproc_t cprocs, my_cproc;
    char *name;
#ifdef mac2
    unsigned int size;
#else
    int size;
#endif
    kern_return_t ret;

    ERROR_NO_INFERIOR;

    ret = task_threads(inferior_task, &thread_table, &thread_count);
    if (ret != KERN_SUCCESS) {
	fprintf(stderr, "Error getting inferior's thread list.\n");
	mach_error("task_threads", ret);
	return;
    }
    printf("There are %d threads.\n", thread_count);
    printf("Thread\tName\tState\tSuspend\tPort\n");
    cprocs = get_cprocs();
    for (i = 0; i < thread_count; i++) {
	if (thread_table[i] == current_thread)
	    current_thread_index = i;

	size = THREAD_BASIC_INFO_COUNT;
	ret = thread_info(thread_table[i], THREAD_BASIC_INFO,
			  (thread_info_t)&ths, &size);
	if (ret != KERN_SUCCESS) {
	    fprintf(stderr, "Unable to get info on thread 0x%x.\n",
		    thread_table[i]);
	    mach_error("thread_statistics", ret);
	    continue;
	}

	name = get_thread_name(thread_table[i], i, cprocs);

	printf ("%d\t%s\t%c\t%d\t0x%x\n", i, name,
		translate_state (ths.run_state), ths.suspend_count,
		thread_table[i]);
    }

    printf("\nThe current thread is thread %d.\n", current_thread_index);
    free_cprocs (cprocs);

    ret = vm_deallocate(task_self(), (vm_address_t)thread_table, 
			(thread_count * sizeof(int)));
    if (ret != KERN_SUCCESS) {
	fprintf(stderr, "Error trying to deallocate thread list.\n");
	mach_error("vm_deallocate", ret);
    }
}


void
free_cprocs( cprocs )
register cproc_t cprocs;
{
    register cproc_t my_cproc;

    while (cprocs != NO_CPROC) {
	if (cprocs->incarnation != NULL) {
	    if (cprocs->incarnation->name != NULL)
		free (cprocs->incarnation->name);
	    free (cprocs->incarnation);
	}
	my_cproc = cprocs->link;
	free (cprocs);
	cprocs = my_cproc;
    }
}


void
thread_select_command(args, from_tty)
     char *args;
     int from_tty;
{
  int thread_index;
  thread_array_t thread_list;
#ifdef mac2
  unsigned int thread_count;
#else
  int thread_count;
#endif
  kern_return_t ret;

  ERROR_NO_INFERIOR;

  if (!args)
    error_no_arg ("thread to select");

  thread_index = atoi(args);
  if (thread_index < 0) {
    error ("no such thread");
    return;
  }
  ret = task_threads (inferior_task, &thread_list, &thread_count);
  if (ret != KERN_SUCCESS) {
      mach_error ("task_threads", ret);
      exit (1);
  }
  if (thread_index > thread_count) {
    error ("no such thread");
    return;
  }

  current_thread = thread_list[thread_index];
  fetch_inferior_registers ();
  stop_pc = read_pc ();
  set_current_frame (create_new_frame (read_register (FP_REGNUM), stop_pc));
  stop_frame_address = FRAME_FP (get_current_frame ());

  ret = vm_deallocate(task_self(), (vm_address_t)thread_list, 
		      (thread_count * sizeof(int)));
  if (ret != KERN_SUCCESS) {
    fprintf(stderr, "Error trying to deallocate thread list.\n");
    mach_error("vm_deallocate", ret);
  }
}


cproc_t
get_cprocs()
{
    register struct symbol *sym;
    cproc_t their_cprocs, curr_cproc, cproc_copy, cproc_head;
    char *name;
    cthread_t cthread;

    their_cprocs = NO_CPROC;
    sym = lookup_symbol("cprocs", 0, VAR_NAMESPACE, 0);
    if (sym == 0)
    	return(NO_CPROC);
    read_inferior_memory(sym->value.value, &their_cprocs, sizeof(cproc_t));
    if (their_cprocs == NO_CPROC)
      return( NO_CPROC );
    cproc_head = (cproc_t)malloc(sizeof(struct cproc));
    read_inferior_memory(their_cprocs, cproc_head, sizeof(struct cproc));
    if (cproc_head->incarnation != NULL) {
      cthread = (cthread_t) malloc(sizeof(struct cthread));
      read_inferior_memory(cproc_head->incarnation, cthread,
			   sizeof(struct cthread));
      cproc_head->incarnation = cthread;
      name = malloc(50);
      read_inferior_memory(cthread->name, name, 50);
      cthread->name = name;
    }

    if (their_cprocs == NO_CPROC)
	return (NO_CPROC);

    curr_cproc = cproc_head;
    while (curr_cproc->link != NO_CPROC) {
	cproc_copy = (struct cproc *)malloc(sizeof(struct cproc));
	read_inferior_memory(curr_cproc->link, cproc_copy,
			     sizeof(struct cproc));
	/* add to tail, order is important */
	curr_cproc->link = cproc_copy;
	curr_cproc = curr_cproc->link;
	if (curr_cproc->incarnation != NULL) {
	    cthread = (cthread_t)malloc(sizeof(struct cthread));
	    read_inferior_memory(curr_cproc->incarnation, cthread,
				 sizeof(struct cthread));
	    curr_cproc->incarnation = cthread;
	    name = malloc(50);
	    read_inferior_memory(cthread->name, name, 50);
	    cthread->name = name;
	}
    }
    return(cproc_head);
}

set_trace_bit (thread)
{
  int			flavor = TBIT_FLAVOR;
  unsigned int		stateCnt = TBIT_FLAVOR_SIZE;
  kern_return_t		ret;
  thread_state_data_t	state;
  int bla;

  ret = thread_get_state(thread, flavor, state, &stateCnt);
  if (ret != KERN_SUCCESS) {
    fprintf(stderr, "set_trace_bit: error reading status register\n");
    mach_error("thread_get_state", ret);
    exit (1);
  }

  TBIT_SET(thread,state);

  ret = thread_set_state(thread, flavor, state, stateCnt);
  if (ret != KERN_SUCCESS) {
    fprintf(stderr, "set_trace_bit: error writing status register\n");
    mach_error("thread_set_state", ret);
    exit (1);
  }  
}

clear_trace_bit (thread)
{
  int			flavor = TBIT_FLAVOR;
  unsigned int		stateCnt = TBIT_FLAVOR_SIZE;
  kern_return_t		ret;
  thread_state_data_t	state;

  ret = thread_get_state(thread, flavor, state, &stateCnt);
  if (ret != KERN_SUCCESS) {
    fprintf(stderr, "clear_trace_bit: error reading status register\n");
    mach_error("thread_get_state", ret);
    exit (1);
  }

  TBIT_CLEAR(thread,state);

  ret = thread_set_state(thread, flavor, state, stateCnt);
  if (ret != KERN_SUCCESS) {
    fprintf(stderr, "set_trace_bit: error writing status register\n");
    mach_error("thread_set_state", ret);
    exit (1);
  }  

}

#ifdef	FLUSH_INFERIOR_CACHE

/* When over-writing code on some machines the I-Cache must be flushed
   explicitly, because it is not kept coherent by the lazy hardware.
   This definitely includes breakpoints, for instance, or else we
   endup looping in misterious Bpt traps */

#ifndef MATTR_CACHE	/* fornow */
#include <mach/vm_attributes.h>
#endif

flush_inferior_icache(pc,amount)
	CORE_ADDR pc;
{
	vm_machine_attribute_val_t flush = MATTR_VAL_ICACHE_FLUSH;
	kern_return_t   ret;

	ret = vm_machine_attribute(inferior_task, pc, amount,
				   MATTR_CACHE, &flush);
	if (ret != KERN_SUCCESS) {
		mach_error("vm_machine_attribute", ret);
		fprintf(stderr, "Error flushing inferior's cache.\n");
	}
}

#endif	FLUSH_INFERIOR_CACHE


suspend_command (args, from_tty)
     char *args;
     int from_tty;
{
  int thread_index;
  thread_array_t thread_list;
#ifdef mac2
  unsigned int thread_count;
#else
  int thread_count;
#endif
  kern_return_t ret;

  if (!args)
    error_no_arg ("thread to suspend or ALL");

  if (strcmp(args, "all") == 0) {
    suspend_all_threads();
    printf("WARNING: you have just suspended the thread that ptrace is\n");
    printf("using.  Type \"info ptrace\" for details.");
    return;
  }

  thread_index = atoi(args);
  if (thread_index < 0) {
    error ("no such thread");
    return;
  }

  ret = task_threads (inferior_task, &thread_list, &thread_count);
  if (ret != KERN_SUCCESS) {
      mach_error ("task_threads", ret);
      exit (1);
  }

  if (thread_index > thread_count) {
    error ("no such thread");
    return;
  }

  ret = thread_suspend (thread_list[thread_index]);
  if (ret != KERN_SUCCESS) {
      mach_error ("thread_suspend", ret);
  }

  if (thread_list[thread_index] == current_thread) {
    printf("WARNING: you have just suspended the thread that ptrace is\n");
    printf("using.  Type \"info ptrace\" for details.\n");
  }

  ret = vm_deallocate(task_self(), (vm_address_t)thread_list, 
		      (thread_count * sizeof(int)));
  if (ret != KERN_SUCCESS) {
    fprintf(stderr, "Error trying to deallocate thread list.\n");
    mach_error("vm_deallocate", ret);
  }
}

suspend_all_threads ()
{
    thread_array_t	thread_list;
#ifdef mac2
    unsigned int	thread_count;
#else
    int			thread_count;
#endif
    kern_return_t	ret;

    ret = task_threads(inferior_task, &thread_list, &thread_count);
    if (ret != KERN_SUCCESS) {
#ifdef mac2
	fprintf(stderr, "Error trying to suspend all threads.\n");
#else
	fprintf("Error trying to suspend all threads.\n");
#endif
	mach_error("task_threads", ret);
	exit (1);
    }
    while (thread_count > 0) {
#ifdef mac2
	ret = thread_suspend((thread_t)thread_list[--thread_count]);
#else
	ret = thread_suspend(thread_list[--thread_count]);
#endif
	if (ret != KERN_SUCCESS) {
#ifdef mac2
	    fprintf(stderr, "Error trying to suspend thread 0x%x.\n", 
#else
	    fprintf("Error trying to suspend thread 0x%x.\n", 
#endif
		    thread_list[--thread_count]);
	    mach_error("thread_suspend", ret);
	}
    }
    ret = vm_deallocate(task_self(), (vm_address_t)thread_list, 
			(thread_count * sizeof(int)));
    if (ret != KERN_SUCCESS) {
	fprintf(stderr, "Error trying to deallocate thread list.\n");
	mach_error("vm_deallocate", ret);
    }
}

resume_command (args, from_tty)
     char *args;
     int from_tty;
{
  int thread_index;
  thread_array_t thread_list;
#ifdef mac2
  unsigned int thread_count;
#else
  int thread_count;
#endif
  kern_return_t ret;

  if (!args)
    error_no_arg ("thread to resume or ALL");

  if (strcmp(args, "all") == 0) {
    resume_all_threads();
    return;
  }

  thread_index = atoi(args);
  if (thread_index < 0) {
    error ("no such thread");
    return;
  }

  ret = task_threads (inferior_task, &thread_list, &thread_count);
  if (ret != KERN_SUCCESS)
      error("task_threads", mach_error_string ( ret));

  if (thread_index > thread_count) {
    error ("no such thread");
    return;
  }

  ret = thread_resume (thread_list[thread_index]);
  if (ret != KERN_SUCCESS)
      mach_error ("thread_resume", ret);

  ret = vm_deallocate(task_self(), (vm_address_t)thread_list, 
		      (thread_count * sizeof(int)));
}


resume_all_threads ()
{
    thread_array_t	thread_list;
#ifdef mac2
    unsigned int	thread_count;
#else
    int			thread_count;
#endif
    kern_return_t	ret;

    ret = task_threads(inferior_task, &thread_list, &thread_count);
    if (ret != KERN_SUCCESS)
	error("task_threads", mach_error_string( ret));

    while (thread_count > 0) {
	ret = thread_resume(thread_list[--thread_count]);
	if (ret != KERN_SUCCESS)
	    error("thread_resume", mach_error_string( ret));
    }
    ret = vm_deallocate(task_self(), (vm_address_t)thread_list, 
			(thread_count * sizeof(int)));
}


#if	never_mind
ptrace_info ()
{
  printf("ptrace is the unix function which all debuggers use to\n");
  printf("manipulate their inferiors.  It is implemented in such a\n");
  printf("way that if a debugger calls ptrace on a thread which is\n");
  printf("suspended, or which is in a task which is suspended, ptrace\n");
  printf("will block.  All subsequent calls to ptrace made by any other\n");
  printf("debugger on the system will also block.  In other words, if\n");
  printf("you don't resume that thread before continuing the inferior,\n");
  printf("you could inhibit everyone from using debuggers on this machine\n");
  printf("until it is rebooted.");
}
#endif

void
_initialize_mach_os ()
{
  kern_return_t ret;

  ret = port_set_allocate(task_self(), &all_enabled_ports);
  if (ret != KERN_SUCCESS)
    fatal(mach_error_string(ret));
  port_set_add(task_self(), all_enabled_ports, task_notify());

  current_wait_port = (port_t) all_enabled_ports;

  add_com ("thread-select", class_stack, thread_select_command,
	   "Select and print a thread.");
  add_com_alias ("ts", "thread-select", class_stack, 0);

  add_com ("thread-list", class_stack, thread_list_command,
	   "List all of threads.");
  add_com_alias ("tl", "thread-list", class_stack, 0);

  add_com ("suspend", class_run, suspend_command,
	   "Suspend one or all of the threads in the inferior.");
  add_com ("resume", class_run, resume_command,
	   "Resume one or all of the threads in the inferior.");
#if never_mind
  add_info ("ptrace", ptrace_info, "Warning about suspend and ptrace.");
#endif
}

