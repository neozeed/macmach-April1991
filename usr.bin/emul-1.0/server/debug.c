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
 *	File: emul/server/debug.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"

static thread_state_regs_t	regs;
static thread_state_frame_t	frame;

int	outfile;

debugger()
{
    kern_return_t	result;
    unsigned		count;

    console_restore();
    close_input();
    outfile = fileno(stderr);

    count = THREAD_STATE_REGS_COUNT;
    result = thread_get_state(thread,
			      THREAD_STATE_REGS,
			      &regs, &count);
    if (result != KERN_SUCCESS)
	mach_error_exit("get regs", result);

    count = THREAD_STATE_FRAME_COUNT;
    result = thread_get_state(thread,
			      THREAD_STATE_FRAME,
			      &frame, &count);
    if (result != KERN_SUCCESS)
	mach_error_exit("get frame", result);

    action();

    print_frame();

    debugger_cmd();

    open_input();
    console_redirect();
}


unsigned char kbuf[256];

confirm(s)
register char *s;
{
    printf("%s", s);
    if (gets(kbuf) == 0)
	return (0);
    else
	return (kbuf[0]&255);
}

debugger_cmd()
{
    int c;
    
    clearerr(stdin);

    while (c = confirm(" (r(egs), (e)x(amine), s(tack), e(xit), <cr>)?  "))
	switch (c) {
	  case 'z':
	    zone();
	    break;

	  case 's':
	    stack();
	    break;

	  case 'f':
	    fault();
	    return;

	  case 'b':
	    set_breakpoint();
	    break;

	  case 'r':
	    print_regs();
	    break;

	  case 't':
	    trace();
	    return;
	    
	  case 'x':
	    examine_mem();
	    break;
	    
	  case 'e':
	    normal_exit();
	}
    untrace();
}

trace()
{
    frame.f_normal.f_sr |= 0x8000;

    thread_set_state(thread,
		     THREAD_STATE_FRAME,
		     &frame, THREAD_STATE_FRAME_COUNT);
}

untrace()
{
    frame.f_normal.f_sr &= ~0x8000;

    thread_set_state(thread,
		     THREAD_STATE_FRAME,
		     &frame, THREAD_STATE_FRAME_COUNT);
}

action()
{
    switch (frame.f_normal.f_vector) {
      case 0xbc:
	exec_breakpoint();
	break;
    }
}

fault()
{
    unsigned long data = 0;

    if (frame.f_normal.f_vector != 0x08) {
	fprintf(stderr, "wrong vector\n");
	return;
    }

    sscanf(kbuf, "f %x", &data);
    switch (frame.f_normal.f_fmt) {
      case STKFMT_SHORT_BUSERR:
#define fp	((short_buserr_exception_frame_t *)&frame)
	fp->f_dfault = 0;
	break;
#undef fp

      case STKFMT_LONG_BUSERR:
#define fp	((long_buserr_exception_frame_t *)&frame)
	if (fp->f_rw == BUSERR_READ)
	    fp->f_dib = data;
	fp->f_dfault = 0;
	break;
#undef fp
    }

    thread_set_state(thread,
		     THREAD_STATE_FRAME,
		     &frame, THREAD_STATE_FRAME_COUNT);
}

vm_offset_t	red_zone;

zone()
{
    unsigned long addr;
    kern_return_t	result;

    sscanf(kbuf, "z %x", &addr);
    if (addr == 0) {
	fprintf(stderr, "bad address\n");
	return;
    }

    red_zone = addr;

    result = memory_object_lock_request(memory_object_control,
					(vm_offset_t)addr,
					memory_object_page_size,
					FALSE, TRUE,
					VM_PROT_NONE,
					PORT_NULL);

    if (result != KERN_SUCCESS)
	fprintf(stderr, "lock_request %08x returned %x\n", addr, result);
}	

static char *F_size[] = { "LONG", "BYTE", "WORD" };
static unsigned long F_size_mask[] = { 0xffffffff, 0xff, 0xffff };

print_frame()
{
    printf("FMT %01x VECTOR %03x SR 0x%04x PC 0x%08x\n",
	   frame.f_normal.f_fmt,
	   frame.f_normal.f_vector,
	   frame.f_normal.f_sr,
	   frame.f_normal.f_pc);
    switch (frame.f_normal.f_vector) {
      default:
	print_inst(frame.f_normal.f_pc);
	break;

      case 0xbc:
	printf("breakpoint exec at %08x\n", frame.f_normal.f_pc);
	print_inst(frame.f_normal.f_pc);
	break;

      case 0x08:
      case 0x0c:
	switch (frame.f_normal.f_fmt) {
	  case STKFMT_SHORT_BUSERR:
#define fp	((short_buserr_exception_frame_t *)&frame)
	    printf("fault status %04x\n", *(unsigned short *)&fp->f_ssw);
	    printf("%s data fault %08x %s %s data %08x\n",
		   fp->f_dfault? "UN-FIXED": "FIXED",
		   fp->f_fault,
		   fp->f_rw? "READ": "WRITE",
		   F_size[fp->f_size],
		   fp->f_dob & F_size_mask[fp->f_size]);
	    break;
#undef fp
	    
	  case STKFMT_LONG_BUSERR:
#define fp	((long_buserr_exception_frame_t *)&frame)
	    printf("fault status %04x\n", *(unsigned short *)&fp->f_ssw);
	    printf("%s data fault %08x %s %s data %08x\n",
		   fp->f_dfault? "UN-FIXED": "FIXED",
		   fp->f_fault,
		   fp->f_rw? "READ": "WRITE",
		   F_size[fp->f_size],
		   fp->f_rw?
		   fp->f_dib & F_size_mask[fp->f_size]:
		   fp->f_dob & F_size_mask[fp->f_size]);
	    break;
#undef fp
	}
    }
}

print_inst(pc)
register unsigned long	pc;
{
    if (pc >= 0x00800000 && pc < 0x00880000)
	pc |= 0x40000000;
    else if (pc >= 0x20000000 && (pc&0x00f00000) != 0x00800000)
	pc &= 0x00ffffff;

    examine_ins(pc);
}

print_regs()
{
    register r;
    
    for (r = 0; r < 4; r++)
	printf("D%x 0x%08x ", r, regs.r_dreg[r]);
    printf("\n");
    for (; r < 8; r++)
	printf("D%x 0x%08x ", r, regs.r_dreg[r]);
    printf("\n");
    for (r = 0; r < 4; r++)
	printf("A%x 0x%08x ", r, regs.r_areg[r]);
    printf("\n");
    for (; r < 7; r++)
	printf("A%x 0x%08x ", r, regs.r_areg[r]);
    printf("SP 0x%08x\n", regs.r_sp);
    print_frame();
}

static vm_offset_t	break_addr;
static unsigned short	break_word;

set_breakpoint()
{
    if (break_addr) {
	printf("breakpoint already set at %08x\n", break_addr);
	return;
    }
	
    printf("address? ");
    if (gets(kbuf) != 0) {
	sscanf(kbuf, "%x", &break_addr);

	break_word = 0x4e4f;
	if (break_addr != 0 &&
	    insert_breakpoint_task(break_addr,
				   &break_word) == TRUE)
	    printf("%04x breakpoint set at %08x\n", break_word, break_addr);
	else {
	    printf("breakpoint not set\n");
	    break_addr = 0;
	}
    }

    clearerr(stdin);
}

exec_breakpoint()
{
    if (frame.f_normal.f_pc != break_addr) {
	frame.f_normal.f_pc += sizeof (short);
	thread_set_state(thread,
			 THREAD_STATE_FRAME,
			 &frame, THREAD_STATE_FRAME_COUNT);
    }
    else {
	write_task(break_addr, sizeof (short), &break_word);
	break_addr = 0;
    }
}

boolean_t
insert_breakpoint_task(addr, word)
vm_offset_t	addr;
unsigned short	*word;
{
    vm_size_t		size;
    pointer_t		data;
    unsigned int	count;
    kern_return_t	result;
    unsigned short	*p, tmp;

    size = round_page(addr + sizeof (*word)) - trunc_page(addr);
    result = vm_read(task,
		     trunc_page(addr),
		     size,
		     &data,
		     &count);

    if (result != KERN_SUCCESS)
	return (FALSE);

    p =	(unsigned short *)&(((unsigned char *)data)[addr &
						    (vm_page_size - 1)]);

    tmp = *p;
    *p = *word;
    *word = tmp;

    result = vm_write(task,
		      trunc_page(addr),
		      data,
		      count);

    (void) vm_deallocate(task_self(), data, count);

    return (result == KERN_SUCCESS);
}


examine_mem()
{
    unsigned long	addr, data;
    char 		c = 'X';
    int			inc, n, w;
    
    printf("address/format? ");
    addr = inc = w = 0;
    for (;;) {
	if (gets(kbuf) == 0)
	    break;
	w = 0;
	if (kbuf[0] == '.') {
	    n = sscanf(kbuf, "./%c %x", &c, &data);
	    if (n == 2)
		w = 1;
	}
	else {
	    n = sscanf(kbuf, "%x/%c %x", &addr, &c, &data);
	    if (n == 0 || n == -1 /* bug in sscanf */)
		addr += inc;
	    else if (n == 3)
		w = 1;
	}
	inc = 0;
	switch (c) {
	  default:
	    fprintf(stderr, "format err %c\n", c);
	    break;

#define readORwrite_task(addr, size, x) \
	    (w?					\
	     write_task((addr), (size), (x)):	\
	     read_task((addr), (size), (x)))
	     
	  case 'X':
	    {
		unsigned long x = data;

		if (readORwrite_task(addr, sizeof (long), &x) == FALSE)
		    break;
		if (!w) printf("%08x: %08x\n", addr, x);
		inc = sizeof (long);
	    }
	    break;
	    
	  case 'x':
	    {
		unsigned short x = data;

		if (readORwrite_task(addr, sizeof (short), &x) == FALSE)
		    break;
		if (!w) printf("%08x: %04x\n", addr, x);
		inc = sizeof (short);
	    }
	    break;

	  case 'b':
	    {
		unsigned char x = data;

		if (readORwrite_task(addr, sizeof (char), &x) == FALSE)
		    break;
		if (!w) printf("%08x: %02x\n", addr, x);
		inc = sizeof (char);
	    }
	    break;
	    
	  case 'i':
	    if (!w) inc = print_inst(addr);
	    break;
	}
    }
    putchar('\n');
    clearerr(stdin);
}

stack()
{
    struct call_frame {
	unsigned long		f_fp;
	unsigned long		f_pc;
	unsigned long		f_param[0];
    } call_frame;
    unsigned long	fp, stack_base, stack_size, heap_limit;

    read_task(0x908, sizeof (stack_base), &stack_base);
    stack_size = stack_base - regs.r_sp;
    read_task(0x130, sizeof (heap_limit), &heap_limit);
    fp = regs.r_areg[6];
    printf("stack trace:\nSTACK BASE %08x STACK TOP %08x HEAP LIMIT %08x\n",
	   stack_base, regs.r_sp, heap_limit);
    if (stack_base == 0xffffffff) {
	printf("bad stack base\n");
	return;
    }
    printf("STACK SIZE %08x STACK EXTENT %08x\n",
	   stack_base - heap_limit, stack_base - regs.r_sp);
    while (fp < stack_base && (stack_base - fp) < stack_size) {
	read_task(fp, sizeof (call_frame), &call_frame);
	printf("Called from %08x fp %08x\n", call_frame.f_pc, call_frame.f_fp);
	fp = call_frame.f_fp;
    }
    printf("end of stack trace\n");
}

boolean_t
read_task(addr, size, dest)
vm_offset_t	addr;
vm_size_t	size;
pointer_t	dest;
{
    vm_offset_t		data = 0;
    vm_size_t		count;
    kern_return_t	result;
    unsigned char	*p;

    count = round_page(addr + size) - trunc_page(addr);
    result = vm_map(task_self(),
		    &data,
		    count,
		    0, TRUE,
		    memory_object,
		    trunc_page(addr),
		    FALSE,
		    VM_PROT_ALL, VM_PROT_ALL, VM_INHERIT_NONE);
    if (result != KERN_SUCCESS) {
	mach_error("vm map", result);
	fprintf(stderr, "read - map failure\n");
	return (FALSE);
    }

    p = &(((unsigned char *)data)[addr & (vm_page_size - 1)]);

    switch (size) {
      case sizeof (char):
	  *(unsigned char *)dest = *(unsigned char *)p;
	  break;

      case sizeof (short):
	  *(unsigned short *)dest = *(unsigned short *)p;
	break;

      case sizeof (long):
	  *(unsigned long *)dest = *(unsigned long *)p;
	break;

      default:
	  bcopy(p, dest, size);
    }

    (void) vm_deallocate(task_self(), data, count);

    return (TRUE);
}

boolean_t
write_task(addr, size, src)
vm_offset_t	addr;
vm_size_t	size;
pointer_t	src;
{
    vm_offset_t		data = 0;
    vm_size_t		count;
    kern_return_t	result;
    unsigned char	*p;

    count = round_page(addr + size) - trunc_page(addr);
    result = vm_map(task_self(),
		    &data,
		    count,
		    0, TRUE,
		    memory_object,
		    trunc_page(addr),
		    FALSE,
		    VM_PROT_ALL, VM_PROT_ALL, VM_INHERIT_NONE);
    if (result != KERN_SUCCESS) {
	fprintf(stderr, "write - map failure\n");
	return (FALSE);
    }

    p = &(((unsigned char *)data)[addr & (vm_page_size - 1)]);

    switch (size) {
      case sizeof (char):
	  *(unsigned char *)p = *(unsigned char *)src;
	  break;

      case sizeof (short):
	  *(unsigned short *)p = *(unsigned short *)src;
	break;

      case sizeof (long):
	  *(unsigned long *)p = *(unsigned long *)src;
	break;

      default:
	  bcopy(src, p, size);
    }

    (void) vm_deallocate(task_self(), data, count);

    return (TRUE);
}
