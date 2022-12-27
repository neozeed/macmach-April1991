/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	float.h,v $
 * Revision 2.6  89/03/09  20:18:37  rpd
 * 	More cleanup.
 * 
 * Revision 2.5  89/02/25  18:12:26  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.4  89/02/08  17:54:32  mwyoung
 * Relocated from ca/float.h.  This file should be pared down to
 * reflect only the Mach interface.
 * 
 * Revision 2.3  88/11/23  16:19:17  rpd
 * 	More Acis merge.
 * 	[88/11/13  22:18:24  rpd]
 * 	
 * 	Merged with Acis.
 * 	[88/11/04  16:58:08  rpd]
 * 
 * 25-Apr-88  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Added AFPA support.  
 *
 * 28-Aug-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Added #ifndef ASSEMBLER around C defintions in order for this
 *	file to be included into locore.c.
 *
 * 26-Aug-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Merged in latest ACIS changes.  
 *
 */
/*
 * 5799-WZQ (C) COPYRIGHT IBM CORPORATION 1987,1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/* $ Header:float.h 11.1$ */
/* $ ACIS:float.h 11.1$ */
/* $ Source: /ibm/acis/usr/sys/ca/RCS/float.h,v $ */

#ifndef	_MACH_CA_FLOAT_H_
#define _MACH_CA_FLOAT_H_

#if	!defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidfloat = "$Header: float.h,v 2.6 89/03/09 20:18:37 rpd Exp $";
#endif

#ifndef	FLOAT_H_
#define FLOAT_H_

/*
 * this file contains those defines and declarations required 
 * for the floating point accelerator 
 */
#ifndef	_FLOATSAVE_

/* 
 * floatsave is the save area for the floating point registers
 * used when it is necessary to take back a register set from
 * a process. these are kept in kernel memory so that it is
 * easy to do so. The space is allocated only when the FPA is 
 * present.
 * the current register set is given in the proc structure.
 *
 */

#define _FLOATSAVE_

#ifndef	LOCORE
/* saved float register sets are indexed by p-proc. */

/*
 * For each type of register set, we define the number of words necessary to
 * save the state of that type of hardware.
 */
#define FPA_SAVE_WORDS		(64+3)		/* FPA1 or 2 */
#define MC881_SAVE_WORDS	(300/4)
#define EMUL_SAVE_WORDS		34

/*
 * For each type of register set, we define the number of legal registers
 * which may be set/fetched via ptrace.
 */
#define FPA_NUM_REG	16
#define MC881_NUM_REG	(3*8)	/* 8 registers, each extended precision */
#define EMUL_NUM_REG	16

/* saved float register sets are stored in the pcb in the thread structure. */

struct floatsave 
{
	union {
		int fpa_int[FPA_SAVE_WORDS];
		double fpa_double[FPA_SAVE_WORDS/2];
		float fpa_float[FPA_SAVE_WORDS];
		int mc881_int[MC881_SAVE_WORDS];
		int emul_int[EMUL_SAVE_WORDS];
	} float_un;
#define fpa_intreg float_un.fpa_int
#define fpa_doublereg float_un.fpa_double
#define fpa_floatreg float_un.fpa_float
#define mc881_intreg	float_un.mc881_int
#define emul_intreg	float_un.emul_int
};

#ifdef	KERNEL
extern struct floatsave *floatsave;	/* NPROC of these allocated */
extern int	float_hardware;		/* What hardware is on the system */
#endif	KERNEL

/*
 * The following structure is what the kernel and user process use
 * to pass back and forth the state of the system and process
 * floating point structures.
 */

struct floatstate {
    int
	hardware_state,		/* What exists on the system */
	process_state;		/* What is allocated to this process */
    void
	(*emulator)(),		/* Address of floating point emulator */
	(*code_generator)();	/* Address of floating point code generator */
    int
	fpa_registerset;	/* (At this time) which fpa register set */
};

#define float_has_881(x)	((x)&FLOAT_MC881)
#define float_has_fpa(x)	((x)&FLOAT_FPA)
#define float_has_afpa(x)	((x)&FLOAT_AFPA)
#define float_has_fpa_or_afpa(x)	((x)&(FLOAT_FPA|FLOAT_AFPA))
#define float_has_any(x)	((x)&(FLOAT_FPA|FLOAT_AFPA|FLOAT_MC881))
#define float_has_emul(x)	((x)&FLOAT_EMUL)
#define float_has_afpa_hardware(x)	((x)&FLOAT_AFPA_HARDWARE)
#define float_has_e_afpa_hardware(x)	(((x)&FLOAT_AFPA_HARDWARE) && \
					((x)&FLOAT_AFPA_PASS_D))

/*
 * These are the values which exist in current_thread()->pcb->floatmask,
 * and in float_hardware.
 */
#define FLOAT_MC881			0x01	/* Using mc881/mc881 exists */
#define FLOAT_FPA			0x02	/* Using fpa/afpa exists */
#define FLOAT_AFPA			0x04	/* Using afpa/afpa operational*/
#define FLOAT_EMUL			0x08	/* Using emulator */
#define FLOAT_AFPA_HARDWARE		0x10	/* Afpa hardware exists */
	/*
	 * Control store accesses to afpa enabled.
	 * In this state, the only legal afpa commands are
	 * those to load the microcode.
	 */
#define FLOAT_AFPA_CONTROL_STORE_ENABLE	0x20
#define FLOAT_AFPA_DMA			0x40	/* Afpa with DMA hardware */
#define FLOAT_AFPA_PASS_D		0x80	/* hardware type is E_AFPA */

#define FLOAT_FMT "\20\1MC881\2FPA\3AFPA_UP\4EMUL\5AFPA_HW\6AFPA_CTL\7AFPA_DMA"

/*
 * This constant is used in the current_thread()->pcb->pcb_floatinfo to
 * indicate that the fpa requires an exception.
 */
#define	 SOWEFPA	0x01

#endif	LOCORE

#endif	/* endef _FLOATSAVE_ */
#endif	FLOAT_H_

#endif	_MACH_CA_FLOAT_H_
