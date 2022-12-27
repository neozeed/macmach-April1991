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
 * $Log:	genassym.c,v $
 * Revision 2.2  90/09/04  17:20:25  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/genassym.c
 */

#include <macos_emul.h>

#include <sys/param.h>
#include <sys/buf.h>
#include <sys/vmparam.h>
#include <sys/user.h>
#include <sys/cmap.h>
#include <sys/map.h>
#include <sys/proc.h>
#include <sys/mbuf.h>
#include <sys/msgbuf.h>
#include <sys/vmmac.h>

#include <machine/psl.h>
#include <machine/cpu.h>

#include <machine/pmap.h>
#include <kern/thread.h>

#include <mac2dev/via.h>
#if	MACOS_EMUL
#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macinst.h>
#endif

thread_t	active_threads[1];

main()
{
	register struct regs *rp = (struct regs *)0;
	struct pcb	*pcb = (struct pcb *)0;
	struct pmap	*pmap = (struct pmap *)0;
	struct task	*task = (struct task *)0;
	struct thread	*thread = (struct thread *)0;
	struct vm_map	*vm_map = (struct vm_map *)0;
	normal_exception_frame_t *frame = (normal_exception_frame_t *)0;
	struct uuprof *uprof = (struct uuprof *)0;
#if	MACOS_EMUL
	struct mac_internal *mac = (struct mac_internal *)0;
#endif
	via_reg_t vp = (via_reg_t)0;

	printf("#define\tPMMU_RP_LIMIT 0x%x\n", PMMU_RP_LIMIT);
	printf("#define\tPMMU_VALID_RP 0x%x\n", PMMU_VALID_RP);
	printf("#define\tSLOAD 0x%x\n", SLOAD);
	printf("#define\tPCB_REGS 0x%x\n", &pcb->pcb_user);
	printf("#define\tPCB_FRAME 0x%x\n", &pcb->pcb_frame);
	printf("#define\tPCB_D0 0x%x\n", &pcb->pcb_user.r_dreg[0]);
	printf("#define\tPCB_D1 0x%x\n", &pcb->pcb_user.r_dreg[1]);
	printf("#define\tPCB_A0 0x%x\n", &pcb->pcb_user.r_areg[0]);
	printf("#define\tPCB_A1 0x%x\n", &pcb->pcb_user.r_areg[1]);
	printf("#define\tPCB_SP 0x%x\n", &pcb->pcb_user.r_sp);
	printf("#define\tR_SP 0x%x\n", &rp->r_sp);
	printf("#define\tF_PC 0x%x\n", &frame->f_pc);
	printf("#define\tF_SR 0x%x\n", &frame->f_sr);
	printf("#define\tF_VOR 0x%x\n", ((long)&frame->f_pc)+sizeof (long));
	printf("#define\tPCB_RETURN_FRAME_SIZE 0x%x\n", &pcb->pcb_return_frame_size);
	printf("#define\tPCB_RETURN_FRAME_DATA 0x%x\n", &pcb->pcb_return_frame_data);
	printf("#define\tPCB_K_REGS 0x%x\n", &pcb->pcb_kernel);
	printf("#define\tPCB_K_SP 0x%x\n", &pcb->pcb_kernel.r_ksp);
	printf("#define\tPCB_K_PC 0x%x\n", &pcb->pcb_kernel.r_pc);
	printf("#define\tPCB_K_SR 0x%x\n", &pcb->pcb_kernel.r_sr);
	printf("#define\tPCB_FP_FRAME 0x%x\n", &pcb->pcb_fp_frame);
	printf("#define\tPCB_FPS_REGS 0x%x\n", pcb->pcb_fp_state.fps_regs);
	printf("#define\tPCB_FPS_CTRL 0x%x\n", &pcb->pcb_fp_state.fps_control);
	printf("#define\tPCB_FLAGS 0x%x\n", &pcb->pcb_flags);
	printf("#define\tPCB_AST 0x%x\n", &pcb->pcb_ast);
	printf("#define\tPCB_THREAD 0x%x\n", &pcb->pcb_thread);
	printf("#define\tPMAP_ROOT 0x%x\n", &pmap->root);
	printf("#define\tPMAP_FLAGS 0x%x\n", &pmap->flags);
	printf("#define\tPMAP_ALT_ROOT 0x%x\n", &pmap->alt_root);
	printf("#define\tPMAP_24BIT_BIT 0x%x\n", bit(PMAP_F_24BIT));
	printf("#define\tRET_FRAME_BIT %d\n", bit(RET_FRAME));
#if	MACOS_EMUL
	printf("#define\tMAC_EMULATION_BIT %d\n", bit(MAC_EMULATION));
	printf("#define\tMAC_24BIT_BIT %d\n", bit(MAC_24BIT));
	printf("#define\tPCB_EMUL 0x%x\n", &pcb->pcb_emul);
	printf("#define\tMAC_TASK_COMPAT_MAP 0x%x\n", &mac->task_compat_map);
	printf("#define\tMAC_VBR 0x%x\n", &mac->ctrl[R_VBR]);
	printf("#define\tMAC_SR 0x%x\n", &mac->sr);
	printf("#define\tMAC_OS_MAP 0x%x\n", mac->os_map);
	printf("#define\tMAC_TBOX_MAP 0x%x\n", mac->tbox_map);
	printf("#define\tMAC_OS_VECTOR 0x%x\n", &mac->os_vector);
	printf("#define\tMAC_TBOX_VECTOR 0x%x\n", &mac->tbox_vector);
#endif
	printf("#define\tAST_SCHED_BIT %d\n", bit(AST_SCHED));
	printf("#define\tTRACE_USER_BIT %d\n", bit(TRACE_USER));
	printf("#define\tTRACE_AST_BIT %d\n", bit(TRACE_AST));
	printf("#define\tSR_SUPR_BIT %d\n", bit(SR_SUPR));
	printf("#define\tSR_TRACE_BIT %d\n", bit(SR_TRACE));
	printf("#define\tSR_T0_BIT %d\n", bit(SR_T0));
	printf("#define\tFP_RESTORE_BIT %d\n", bit(FP_RESTORE));

	printf("#define\tTASK_MAP 0x%x\n", &task->map);
	printf("#define\tTASK_KERNEL_VM_SPACE 0x%x\n", &task->kernel_vm_space);
	printf("#define\tTHREAD_PCB 0x%x\n", &thread->pcb);
	printf("#define\tTHREAD_RECOVER 0x%x\n", &thread->recover);
	printf("#define\tTHREAD_TASK 0x%x\n", &thread->task);
	printf("#define\tTHREAD_STATE 0x%x\n", &thread->state);
	printf("#define\tTH_WAIT 0x%x\n", TH_WAIT);
	printf("#define\tTH_RUN 0x%x\n", TH_RUN);
	printf("#define\tTH_IDLE 0x%x\n", TH_IDLE);
	printf("#define\tVM_MAP_PMAP 0x%x\n", &vm_map->pmap);
	printf("#define\tVIA_REGB 0x%x\n", &vp->regb);
	printf("#define\tVIA_IFR 0x%x\n", &vp->ifr);
	printf("#define\tVIA_T2CL 0x%x\n", &vp->t2cl);
	printf("#define\tVIA_DDRB 0x%x\n", &vp->ddrb);
	printf("#define\tVIA_SR 0x%x\n", &vp->sr);
	printf("#define\tVIA_ACR 0x%x\n", &vp->acr);
	printf("#define\tVIA_IER 0x%x\n", &vp->ier);
	printf("#define\tPR_BASE %d\n", &uprof->pr_base);
	printf("#define\tPR_SIZE %d\n", &uprof->pr_size);
	printf("#define\tPR_OFF %d\n", &uprof->pr_off);
	printf("#define\tPR_SCALE %d\n", &uprof->pr_scale);
	exit(0);
}

bit(mask)
	register long mask;
{
	register int i;

	for (i = 0; i < 32; i++) {
		if (mask & 1)
			return (i);
		mask >>= 1;
	}
	exit (1);
}
