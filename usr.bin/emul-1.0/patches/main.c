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
 *	File: emul/patches/main.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach.h>

#include <mach/exception.h>

#include <sys/param.h>

#include <mac2emul/macdefs.h>

#include "mac.h"

port_t		kernel_request_port;

extern		VBLvec[], INPvec[], SLOTvec[];

unsigned long	memtop;

/*
 * main program.  Sets up/installs
 * patches.  On return, ROM entry
 * point is executed.
 */
main()
{
    kern_return_t	result;

    setup_bootglobs();

    result = env_get_port(environment_port,
			  "kernel_request",
			  &kernel_request_port);
    if (result != KERN_SUCCESS)
	_exit(0);

    result = emul_init();

    if (result != KERN_SUCCESS)
	_exit(0);
}

setup_bootglobs()
{
    *(unsigned long *)0xcfc = 0x574c5343;

    {
	struct boot_globs {
	    long		alloc_offset;
	    long		top_offset;
	    unsigned long	warm_start;
	    unsigned long	ram_start;
	    unsigned long	ram_size;
	    unsigned long	minus_one;
	    unsigned long	trash[2];
	} *bootglobs = (struct boot_globs *)(memtop - sizeof (*bootglobs));
	unsigned long		bootglobs_ptr =
	    (unsigned long)(&bootglobs->ram_start);

#define ptr(x)			((unsigned long)(&(x)))
	bootglobs->alloc_offset = ptr(bootglobs->alloc_offset) - bootglobs_ptr;
	bootglobs->top_offset = ptr(bootglobs->trash[2]) - bootglobs_ptr;
	bootglobs->warm_start = 0x574c5343;
	bootglobs->ram_start = 0;
	bootglobs->ram_size = memtop;
	bootglobs->minus_one = 0xffffffff;
	bootglobs->trash[0] = bootglobs->trash[1] = 0;
    }
}

/*
 * reset() is executed when a reset instruction
 * is executed by the emulator.  This allows us
 * to restart the emulator via the 'Restart' item
 * in the Finder 'Special' menu.
 */
struct {
    vm_address_t	vector;
    boolean_t		enabled;
} intr_reset_tbl[MAC_INTR_NUMB] =
{
#define	A	(vm_address_t)
/*
 *    vector		enabled
 */
    { 0,		TRUE	},	/* INTR		*/
    { 0,		FALSE	},	/* ETHER	*/
    { A SLOTvec,	TRUE	},	/* SLOT		*/
    { 0x68,		TRUE	},	/* ASC		*/
    { 0x64,		TRUE	},	/* TIMER2	*/
    { 0x64,		TRUE	},	/* TIMER1	*/
    { A VBLvec,		FALSE	},	/* CLK		*/
    { A INPvec,		FALSE	},	/* INP		*/
    { 0x64,		TRUE	}	/* SR		*/
#undef A
};

reset()
{
    register		i;
    vm_address_t	vector;
    kern_return_t	result;
    register		s = splhigh();

    key_reset();
    TimeReset();

    for (i = 0; i < MAC_INTR_NUMB; i++) {
	if (vector = intr_reset_tbl[i].vector) {
	    result = mac_interrupt_vector(kernel_request_port,
					  i,
					  vector);
	    if (result != KERN_SUCCESS)
		_exit(0);
	}
	result = mac_interrupt_enable(kernel_request_port,
				      i,
				      intr_reset_tbl[i].enabled);
	if (result != KERN_SUCCESS)
	    _exit(0);
    }

    splx(s);
}
