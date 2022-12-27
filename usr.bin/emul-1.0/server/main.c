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
 *	File: emul/server/main.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"
#include "pager_defs.h"

#include <sys/errno.h>
#include <sys/resource.h>

task_t		task;
thread_t	thread;
int		pid;

int		file;
struct exec	filehdr;

int		nolog, pagingfile_is_ephemeral;

unsigned long	memtop = 8*1024*1024;

stop()
{
    task_suspend(task);
    thread_abort(thread);

    debugger();

    task_resume(task);
}

main(argc, argv)
int argc;
char **argv;
{
    stdout->_flag |= _IONBF;

    argc--; argv++;
    while (argc > 0 && (*argv)[0] == '-') {
	switch ((*argv)[1]) {
	  case 'l':
	    argv++; argc--;
	    strcpy(logfilename, *argv);
	    break;

	  case 'p':
	    argv++; argc--;
	    strcpy(pagingfilename, *argv);
	    break;

	  case 'e':
	    pagingfile_is_ephemeral++;
	    break;

	  case 'n':
	    nolog++;
	    break;
	}
	argv++; argc--;
    }

    if (argc < 1) {
	fprintf(stderr, "usage: [options] <patch file> [<memtop>]\n");
	fprintf(stderr, "options:\n\t-n(no log file)\n\t-l <log filename>\n\t-p <paging filename>\n\t-e(ephemeral paging file)\n");
	exit(0);
    }
    file = open(*argv, 0);
    if (file < 0)
	unix_error_exit("open patches file");

    argc--; argv++;
    if (argc >= 1)
	sscanf(*argv, "%x", &memtop);

    (void) rpause(ENOSPC, RPAUSE_ALL, RPAUSE_DISABLE);

    signal_init(1);

    pager_setup();
  
    setup_task();

    setup_thread();

    server_main();

    normal_exit();
}

unsigned long memtop_allowed[] =
{
    0x00100000, 0x00200000,
    0x00400000, 0x00500000, 0x00800000,
    0x01000000, 0x01100000, 0x01400000, 0x02000000,
    0x04000000, 0x04100000, 0x04400000, 0x05000000, 0x08000000,
    0
};

set_memtop()
{
    register i;
    
    for (i = 0; memtop_allowed[i] != 0; i++) {
	if (memtop_allowed[i] == memtop) {
	    return;
	}
    }
}    

setup_task()
{
    kern_return_t	result;
    vm_offset_t		lock_page;
    unsigned long	*lockp;

    set_memtop();

    result = vm_allocate(task_self(),
			 &lock_page,
			 sizeof (*lockp),
			 TRUE);
    if (result != KERN_SUCCESS)
	mach_error_exit("vm_allocate lock_page", result);

    result = vm_inherit(task_self(),
			lock_page,
			sizeof (*lockp),
			VM_INHERIT_SHARE);
    if (result != KERN_SUCCESS)
	mach_error_exit("vm_inherit lock_page", result);

    (vm_offset_t)lockp = lock_page;
    
    if (pid = fork()) {
	if (pid < 0)
	    unix_error_exit("fork");

	while (*lockp == 0)
	    swtch();

	vm_deallocate(task_self(),
		      lock_page,
		      sizeof (*lockp));

	result = task_by_unix_pid(task_self(), pid, &task);
	if (result != KERN_SUCCESS)
	    mach_error_exit("task_by_unix_pid", result);

	result = task_suspend(task);
	if (result != KERN_SUCCESS)
	    mach_error_exit("task_suspend", result);

	for (;;) {
	    vm_address_t address = 0;
	    vm_size_t size;
	    vm_prot_t protection, max_protection;
	    vm_inherit_t inheritance;
	    boolean_t shared;
	    port_t object_name;
	    vm_offset_t offset;

	    if (vm_region(task,
			  &address,
			  &size,
			  &protection, &max_protection,
			  &inheritance, &shared,
			  &object_name,
			  &offset) != KERN_SUCCESS)
		break;

	    vm_deallocate(task, address, size);
	    address += size;
	}
    }
    else {
	signal_init(0);
	*lockp = 1;
	thread_terminate(thread_self());
    }
}

setup_thread()
{
    thread_state_regs_t		regs;
    thread_state_frame_t	frame;
    kern_return_t	result;    

    result = thread_create(task, &thread);
    if (result != KERN_SUCCESS)
	mach_error_exit("thread create", result);

    regs.r_sp = EMUL_PATCHES_START+EMUL_PATCHES_SIZE;
    regs.r_areg[0] = memtop;
    result = thread_set_state(thread,
			      THREAD_STATE_REGS,
			      (thread_state_t)&regs,
			      THREAD_STATE_REGS_COUNT);
    if (result != KERN_SUCCESS)
	mach_error_exit("thread set regs", result);

    frame.f_normal.f_fmt = frame.f_normal.f_vector = frame.f_normal.f_sr = 0;
    frame.f_normal.f_pc = EMUL_PATCHES_START;
    result = thread_set_state(thread,
			      THREAD_STATE_FRAME,
			      (thread_state_t)&frame,
			      THREAD_STATE_FRAME_COUNT);
    if (result != KERN_SUCCESS)
	mach_error_exit("thread set frame", result);

    thread_resume(thread);
}
