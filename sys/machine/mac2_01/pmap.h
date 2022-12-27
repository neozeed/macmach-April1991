/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	pmap.h,v $
 * Revision 2.2  90/09/04  17:29:11  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmap.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Machine-dependent structures
 * for the physical map module.
 */

#ifndef	_MAC2_PMAP_H_
#define	_MAC2_PMAP_H_

#ifdef	KERNEL
#include <cpus.h>
#endif	KERNEL

#include <kern/lock.h>
#include <mach/mac2/vm_param.h>
#include <mach/vm_statistics.h>

#ifdef	KERNEL
#include <mac2/pcb.h>
#endif	KERNEL

/*
 * A vm_offset_t which allows access
 * to various fields.
 */
typedef union vm_offset_fields {
    vm_offset_t		full_offset;

    struct {
	vm_offset_t	index:8,
			     :24;
    } ptptbl;
#define ptptbl_index	ptptbl.index

    struct {
	vm_offset_t	     :8,
		        index:11,
			     :13;
    } pptbl;
#define pptbl_index	pptbl.index

    struct {
	vm_offset_t	index:19,
		       offset:13;
    } pg;
#define pg_index	pg.index
#define pg_offset	pg.offset
} vm_offset_fields_t;

typedef	unsigned long	phys_offset_t;

/*
 * MC68851 PMMU data/register structures
 */

/*
 * Root Pointers
 */
#define	PMMU_RP_LIMIT	0x8000	/* no limit */
#define PMMU_RP_SG	0x0200  /* shared globally */

#define PMMU_VALID_RP	2	/* valid root pointer */

struct root_ptr_reg {
    unsigned short	limit;		/* limit reg not used (above) */
    unsigned short	valid;		/* should be PMMU_VALID_RP */
    phys_offset_t	phys;		/* phys address of tree root */
};

typedef	struct root_ptr_reg	PMMU_RP_reg;

/*
 * Translation Control
 */

/*
 * pagesize Field Values
 */
#define	PMMU_PAGESIZE	0xd	/* 8k bytes */

/*
 * addrsize Field Value
 */
#define	PMMU_IS_32b	0x0	/* 32 bit virtual address */

/*
 * Table index maxes
 */
#define	PMMU_TIA	0x8
#define	PMMU_TIB	0xb

struct trans_ctrl_reg {
    unsigned long
	    enable:1,		/* mapping enable */
	    unused:5,
	    sre:1,		/* supervisor root enable */
	    mbz0:1,
	    pagesize:4,		/* page size (bytes) */
	    addrsize:4,		/* size of virt addr (bits) */
	    tia:4,		/* max tbl indexes/level (bits) */
	    tib:4,
	    mbz1:8;
};

typedef	struct trans_ctrl_reg	PMMU_TC_reg;

/*
 * Status register
 */
#define PMMU_SR_BUSERR	0x8000
#define PMMU_SR_WRPROT	0x0800
#define PMMU_SR_INVALID	0x0400

/*
 * Translation Tables
 */

/*
 * Descriptor Types
 */
#define	PMMU_INVALID	    0	    /* invalid entry */
#define	PMMU_VALID_PP	    1	    /* valid page pointer */
#define	PMMU_VALID_PTP	    2	    /* valid page table pointer */

/*
 * Page Protection Codes
 */
#define	PMMU_RW		    0	    /* page is read/write */
#define	PMMU_WP		    1	    /* page is read-only */

/*
 * Descriptors
 */
struct page_tbl_ptr {
    unsigned long
	    pfn:19,		    /* page frame number of page table */
	    mbz:11,
	    valid:2;
};

typedef	struct page_tbl_ptr ptp_t;
#define	PTP_NULL    ((ptp_t *) 0)

struct page_ptr {
    unsigned long
	    pfn:19,		    /* page frame number of page */
	    unused0:4,
	    wired:1,		    /* page is wired */
	    mbz0:1,
	    ci:1,		    /* cache inhibit */
	    mbz1:1,
	    mod:1,		    /* page modified */
	    ref:1,		    /* page referenced */
	    prot:1,		    /* page protection */
	    valid:2;
};

typedef	struct page_ptr	pp_t;
#define	PP_NULL	    ((pp_t *) 0)

pp_t	*pmap_pp();

typedef	unsigned long	cpu_set;	/* set of CPUs - must be <= 32 */

struct pmap {
    phys_offset_t	root;	/* phys addr of translation tree root */
    struct pt_root_page *	/* pt root page descriptor */
			ptrt;
    phys_offset_t		/* phys addr of alternate root */
			alt_root;
    struct pt_root_page *	/* pt root page descriptor */
			alt_ptrt;
    int				/* pmap reference count */
			ref_count;
    unsigned long	flags;
#define PMAP_F_24BIT 0x00000001	/* task is using folded address space */
    simple_lock_data_t	lock;	/* lock on pmap */
    struct pmap_statistics	/* map statistics */
			stats;
    cpu_set			/* bitmap of cpus using pmap */
			cpus_using;
};

typedef struct pmap	*pmap_t;
#define	PMAP_NULL	((pmap_t) 0)

#include <mac2/pmmu_inline.c>
#include <macos_emul.h>

/*
 *	Macros for speed.
 */
#if	MACOS_EMUL
#define PMAP_ACTIVATE(pmap_, thread, cpu)	\
{									\
    extern vm_map_t mac_task_compat_map();				\
\
    if ((pmap_) != kernel_pmap && (thread)->task->kernel_vm_space == FALSE) { \
	pmap_t		pmap;						\
	PMMU_RP_reg	rp;						\
\
	if ((thread)->pcb->pcb_flags&MAC_24BIT)				\
	    pmap = vm_map_pmap(mac_task_compat_map(thread));		\
	else								\
	    pmap = (pmap_);						\
									\
	if (pmap->flags&PMAP_F_24BIT)					\
	    rp.phys = pmap->alt_root;					\
	else								\
	    rp.phys = pmap->root;					\
\
	rp.limit = PMMU_RP_LIMIT;					\
	rp.valid = PMMU_VALID_RP;					\
	pmmu_set_crp(&rp);						\
\
	flush_cpu_caches();						\
\
	pmap->cpus_using = TRUE;					\
    }									\
}

#define PMAP_DEACTIVATE(pmap_, thread, cpu)	\
{									\
    extern vm_map_t mac_task_compat_map();				\
\
    if ((pmap_) != kernel_pmap && (thread)->task->kernel_vm_space == FALSE) { \
	pmap_t		pmap;						\
\
	if ((thread)->pcb->pcb_flags&MAC_24BIT)				\
	    pmap = vm_map_pmap(mac_task_compat_map(thread));		\
	else								\
	    pmap = (pmap_);						\
	pmap->cpus_using = FALSE;					\
    }									\
}

#define PMAP_CONTEXT(pmap_, thread)	\
{								\
    thread_t	old_thread = current_thread();			\
\
    if ((thread)->pcb->pcb_flags&MAC_24BIT !=			\
	old_thread->pcb->pcb_flags&MAC_24BIT) {			\
	PMAP_DEACTIVATE((pmap_), old_thread, cpu_number());	\
	PMAP_ACTIVATE((pmap_), (thread), cpu_number());		\
    }								\
}
#else
#define PMAP_ACTIVATE(pmap_, thread, cpu)	\
{									\
    if ((pmap_) != kernel_pmap && (thread)->task->kernel_vm_space == FALSE) { \
	pmap_t		pmap = (pmap_);					\
	PMMU_RP_reg	rp;						\
\
	if (pmap->flags&PMAP_F_24BIT)					\
	    rp.phys = pmap->alt_root;					\
	else								\
	    rp.phys = pmap->root;					\
\
	rp.limit = PMMU_RP_LIMIT;					\
	rp.valid = PMMU_VALID_RP;					\
	pmmu_set_crp(&rp);						\
\
	flush_cpu_caches();						\
\
	pmap->cpus_using = TRUE;					\
    }									\
}

#define PMAP_DEACTIVATE(pmap, thread, cpu)	\
{									\
    if ((pmap) != kernel_pmap && (thread)->task->kernel_vm_space == FALSE) \
	(pmap)->cpus_using = FALSE;					\
}

#define PMAP_CONTEXT(pmap_, thread)

#endif

#define	pmap_resident_count(pmap)	((pmap)->stats.resident_count)
#define	pmap_phys_address(frame)	((vm_offset_t) (mac2_ptob(frame)))
#define pmap_phys_to_frame(phys)	((int) (mac2_btop(phys)))
#define pmap_attribute(pmap, addr, size, attr, value)	(0)

#endif	_MAC2_PMAP_H_
