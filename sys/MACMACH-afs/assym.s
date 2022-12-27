#define	PMMU_RP_LIMIT 0x8000
#define	PMMU_VALID_RP 0x2
#define	SLOAD 0x1
#define	PCB_REGS 0x0
#define	PCB_FRAME 0x40
#define	PCB_D0 0x0
#define	PCB_D1 0x4
#define	PCB_A0 0x20
#define	PCB_A1 0x24
#define	PCB_SP 0x3c
#define	R_SP 0x3c
#define	F_PC 0x2
#define	F_SR 0x0
#define	F_VOR 0x6
#define	PCB_RETURN_FRAME_SIZE 0x1be
#define	PCB_RETURN_FRAME_DATA 0x1c2
#define	PCB_K_REGS 0x188
#define	PCB_K_SP 0x1b4
#define	PCB_K_PC 0x1ba
#define	PCB_K_SR 0x1b8
#define	PCB_FP_FRAME 0xb0
#define	PCB_FPS_REGS 0x44
#define	PCB_FPS_CTRL 0xa4
#define	PCB_FLAGS 0x220
#define	PCB_AST 0x21e
#define	PCB_THREAD 0x25a
#define	PMAP_ROOT 0x0
#define	PMAP_FLAGS 0x14
#define	PMAP_ALT_ROOT 0x8
#define	PMAP_24BIT_BIT 0x0
#define	RET_FRAME_BIT 15
#define	MAC_EMULATION_BIT 11
#define	MAC_24BIT_BIT 10
#define	PCB_EMUL 0x256
#define	MAC_TASK_COMPAT_MAP 0x106
#define	MAC_VBR 0xaa
#define	MAC_SR 0x98
#define	MAC_OS_MAP 0x10e
#define	MAC_TBOX_MAP 0x132
#define	MAC_OS_VECTOR 0x12e
#define	MAC_TBOX_VECTOR 0x1b2
#define	AST_SCHED_BIT 15
#define	TRACE_USER_BIT 14
#define	TRACE_AST_BIT 13
#define	SR_SUPR_BIT 13
#define	SR_TRACE_BIT 15
#define	SR_T0_BIT 14
#define	FP_RESTORE_BIT 12
#define	TASK_MAP 0x8
#define	TASK_KERNEL_VM_SPACE 0x44
#define	THREAD_PCB 0x24
#define	THREAD_RECOVER 0x68
#define	THREAD_TASK 0xc
#define	THREAD_STATE 0x48
#define	TH_WAIT 0x1
#define	TH_RUN 0x4
#define	TH_IDLE 0x10
#define	VM_MAP_PMAP 0x30
#define	VIA_REGB 0x0
#define	VIA_IFR 0x1a00
#define	VIA_T2CL 0x1000
#define	VIA_DDRB 0x400
#define	VIA_SR 0x1400
#define	VIA_ACR 0x1600
#define	VIA_IER 0x1c00
#define	PR_BASE 4
#define	PR_SIZE 8
#define	PR_OFF 12
#define	PR_SCALE 16
