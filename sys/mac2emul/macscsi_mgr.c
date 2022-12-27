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
 *	File: mac2emul/macscsi_mgr.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Emulate the scsi manager.
 * This should be moved out
 * to the patches.
 */

#include <sys/param.h>
#include <sys/conf.h>

#include <kern/thread.h>

#include <vm/vm_kern.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macscsi_mgr.h>
#include <mac2emul/macuser.h>

int macscsidebug = 0;

SCSIDispatch(frame, regs)
register struct regs *regs;
{
    register struct scsi_data *scsi = &current_thread_pcb()->pcb_emul->scsi;
    register vm_offset_t *asp = (vm_offset_t *)&regs->r_sp;
#define POP(x)					\
    {						\
        FETCH_N(*asp, &(x), sizeof (x));	\
        *asp += sizeof (x);			\
    }
    unsigned short result, code;
    static char rdy[7];
    
    POP(code);

    if (macscsidebug)
	printf("[%x]", code);

    switch (code) {
      case scsiReset:
	scsi->sc_flags &= ~(SCSI_F_READ|SCSI_F_WRITE);
	scsi->sc_buf.b_flags = 0;
	result = noErr;
	break;
	
      case scsiGet:
	result = noErr;
	break;
	
      case scsiSelect:
	{
	    unsigned short ctrl;
	    
	    POP(ctrl);
	    if (ctrl < 0 || ctrl >= 7)
		result = scCommErr;
	    else if (!rdy[ctrl] && test_unit_rdy(ctrl))
		result = scCommErr;
	    else {
		rdy[ctrl] = 1;
		scsi->sc_buf.b_dev = makedev(7, ctrl << 5);
		result = noErr;
	    }
	}
	break;
	
      case scsiCmd:
	{
	    vm_offset_t cmdaddr;
	    unsigned short count, macscsi_cmd();
	    struct scsiRWCmd cmd;
	    
	    POP(count);
	    POP(cmdaddr);
	    FETCH(cmdaddr, &cmd, cmd);
	    result = macscsi_cmd(scsi, &cmd, count);
	}
	break;
	
      case scsiComplete:
	{
	    unsigned long wait;
	    vm_offset_t stat, message;
	    unsigned short zero = 0;
	    
	    POP(wait);
	    POP(message);
	    POP(stat);
	    STORE(message, &zero, zero);
	    STORE(stat, &zero, zero);
	    result = noErr;
	}
	break;
	
      case scsiRead:
      case scsiRBlind:
	{
	    vm_offset_t prg;
	    unsigned short macscsi_read();
	    
	    POP(prg);
	    result = macscsi_read(scsi, prg);
	    if (scsi->sc_buf.b_resid == 0)
		scsi->sc_flags &= ~SCSI_F_READ;
	}
	break;
	
      case scsiWrite:
      case scsiWBlind:
	{
	    vm_offset_t prg;
	    unsigned short macscsi_write();
	    
	    POP(prg);
	    result = macscsi_write(scsi, prg);
	    if (scsi->sc_buf.b_resid == 0)
		scsi->sc_flags &= ~SCSI_F_WRITE;
	}
	break;
	
      case scsiStat:
	result = 0;
	break;
	
      default:
	result = scCommErr;
	break;
    }

    if (macscsidebug)
	printf("(%d)", result);

    STORE(*asp, &result, result);
#undef POP
}

macscsi_end(scsi)
register struct scsi_data *scsi;
{
    struct buf *bp = &scsi->sc_buf;

    if (bp->b_bufsize != 0)
	kmem_free(kernel_map, bp->b_un.b_addr, bp->b_bufsize);
}

unsigned short
macscsi_cmd(scsi, sc, n)
register struct scsi_data *scsi;
register struct scsiRWCmd *sc;
register unsigned long n;
{
    if (scsi->sc_flags&(SCSI_F_READ|SCSI_F_WRITE))
	return (scCommErr);
    
    switch (sc->cmd_op) {
      case 0x0: /* check unit ready */
	break;
	
      case 0x8: /* read */
	scsi->sc_flags |= SCSI_F_READ;
	break;
	
      case 0x0a: /* write */
	scsi->sc_flags |= SCSI_F_WRITE;
	break;
	
      default:
	if (macscsidebug)
	    printf("{%x}", sc->cmd_op);
	return (scCommErr);
    }
    
    if (sc->cmd_lun)
	return (scCommErr);
    
    if (scsi->sc_flags&(SCSI_F_READ|SCSI_F_WRITE)) {
	register struct buf *bp = &scsi->sc_buf;
	register size = scsiNBlk(sc) * SCSI_NBBLK;

	if (size > bp->b_bufsize) {
	    if (bp->b_bufsize != 0)
		kmem_free(kernel_map, bp->b_un.b_addr, bp->b_bufsize);

	    if (bp->b_un.b_addr = (caddr_t)kmem_alloc(kernel_map, size))
		bp->b_bufsize = size;
	    else {
		bp->b_bufsize = 0;
		return (scCommErr);
	    }
	}

	bp->b_blkno = scsiBlkno(sc);
	bp->b_resid = bp->b_bcount = size;

	if (macscsidebug)
	    printf("{%x, %x, %x}", sc->cmd_op, bp->b_blkno, bp->b_bcount);

	if (scsi->sc_flags & SCSI_F_READ)
	    macscsi_bread(scsi);

	if (bp->b_flags & B_ERROR)
	    return (scCommErr);
    }
    
    return (noErr);
}

unsigned short
macscsi_read(scsi, ip)
register struct scsi_data *scsi;
register struct SCSIInstr *ip;
{
    register vm_offset_t r_addr;
    register vm_size_t r_size;
    register m32bit = !(vm_map_pmap(current_task()->map)->flags&PMAP_F_24BIT);
    register struct buf *bp = &scsi->sc_buf;
#define RETURN(x)					\
    {							\
	return (x);					\
    }
    struct SCSIInstr inst;

    if ((scsi->sc_flags&(SCSI_F_READ|SCSI_F_WRITE)) != SCSI_F_READ)
	RETURN (scPhaseErr);

    if (bp->b_flags & B_ERROR)
	RETURN (scCommErr);

    for (;; ip++) {
      nextinst:
	FETCH(ip, &inst, inst);

	if (macscsidebug)
	    printf("<%x, %x, %x>",
		   inst.scOpcode, inst.scParam1, inst.scParam2);

	switch (inst.scOpcode) {
	  case scInc:
	  case scNoInc:
	    if (m32bit)
		r_addr = inst.scParam1;
	    else
		r_addr = inst.scParam1 & 0x00ffffff;
	    
	    r_size = inst.scParam2;

	    if (r_size > bp->b_resid)
		RETURN (scPhaseErr);

	    STORE_N(r_addr,
		    bp->b_un.b_addr + bp->b_bcount - bp->b_resid,
		    r_size);
	    
	    bp->b_resid -= r_size;
	    
	    r_addr += r_size;
	    
	    if (inst.scOpcode == scInc) {
		if (m32bit)
		    inst.scParam1 = r_addr;
		else
		    inst.scParam1 = (inst.scParam1&~0x00ffffff) |
			(r_addr&0x00ffffff);
		STORE(ip, &inst, inst);
	    }
	    break;
	    
	  case scAdd:
	    {
		unsigned long temp;

		FETCH(inst.scParam1, &temp, temp);
		temp += inst.scParam2;
		STORE(inst.scParam1, &temp, temp);
	    }
	    break;
	    
	  case scMove:
	    {
		unsigned long temp;

		FETCH(inst.scParam1, &temp, temp);
		STORE(inst.scParam2, &temp, temp);
	    }
	    break;
	    
	  case scLoop:
	    inst.scParam2--;
	    STORE(ip, &inst, inst);
	    if ((int)inst.scParam2 > 0) {
		(unsigned long)ip += inst.scParam1;
		goto nextinst;
	    } else if (inst.scParam2 == 0)
		break;
	    else
		RETURN (scCommErr);
	    break;
	    
	  case scNOp:
	    break;
	    
	  case scStop:
	    RETURN (noErr);
	    
	  default:
	    RETURN (scBadParamsErr);
	}
    }
#undef RETURN
}

macscsi_bread(scsi)
register struct scsi_data *scsi;
{
    register struct buf *bp = &scsi->sc_buf;

    bp->b_flags = B_READ;

    (*bdevsw[major(bp->b_dev)].d_strategy)(bp);
    biowait(bp);

    bp->b_resid = bp->b_bcount - bp->b_resid;
}

unsigned short
macscsi_write(scsi, ip)
register struct scsi_data *scsi;
register struct SCSIInstr *ip;
{
    register vm_offset_t r_addr;
    register vm_size_t r_size;
    register m32bit = !(vm_map_pmap(current_task()->map)->flags&PMAP_F_24BIT);
    register struct buf *bp = &scsi->sc_buf;
#define RETURN(x)				\
    {						\
	if ((x) == noErr && bp->b_resid == 0)	\
	    macscsi_bwrite(scsi);		\
	return (x);				\
    }
    struct SCSIInstr inst;

    if ((scsi->sc_flags&(SCSI_F_READ|SCSI_F_WRITE)) != SCSI_F_WRITE)
	RETURN (scPhaseErr);

    for (;; ip++) {
      nextinst:
	FETCH(ip, &inst, inst);

	if (macscsidebug)
	    printf("<%x, %x, %x>",
		   inst.scOpcode, inst.scParam1, inst.scParam2);

	switch (inst.scOpcode) {
	  case scInc:
	  case scNoInc:
	    if (m32bit)
		r_addr = inst.scParam1;
	    else
		r_addr = inst.scParam1 & 0x00ffffff;
	    
	    r_size = inst.scParam2;

	    if (r_size > bp->b_resid)
		RETURN (scPhaseErr);

	    FETCH_N(r_addr,
		    bp->b_un.b_addr + bp->b_bcount - bp->b_resid,
		    r_size);

	    bp->b_resid -= r_size;

	    r_addr += r_size;
	    
	    if (inst.scOpcode == scInc) {
		if (m32bit)
		    inst.scParam1 = r_addr;
		else
		    inst.scParam1 = (inst.scParam1&~0x00ffffff) |
			(r_addr&0x00ffffff);
		STORE(ip, &inst, inst);
	    }
	    break;
	    
	  case scAdd:
	    {
		unsigned long temp;

		FETCH(inst.scParam1, &temp, temp);
		temp += inst.scParam2;
		STORE(inst.scParam1, &temp, temp);
	    }
	    break;
	    
	  case scMove:
	    {
		unsigned long temp;

		FETCH(inst.scParam1, &temp, temp);
		STORE(inst.scParam2, &temp, temp);
	    }
	    break;
	    
	  case scLoop:
	    inst.scParam2--;
	    STORE(ip, &inst, inst);
	    if ((int)inst.scParam2 > 0) {
		(unsigned long)ip += inst.scParam1;
		goto nextinst;
	    } else if (inst.scParam2 == 0)
		break;
	    else
		RETURN (scCommErr);
	    break;
	    
	  case scNOp:
	    break;
	    
	  case scStop:
	    RETURN (noErr);
	    
	  default:
	    RETURN (scBadParamsErr);
	}
    }
#undef RETURN
}

macscsi_bwrite(scsi)
register struct scsi_data *scsi;
{
    register struct buf *bp = &scsi->sc_buf;;

    bp->b_flags = B_WRITE;

    (*bdevsw[major(bp->b_dev)].d_strategy)(bp);
    biowait(bp);
}
