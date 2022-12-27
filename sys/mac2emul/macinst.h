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
 *	File: mac2emul/macinst.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Privileged Instruction
 * emulation defines
 */

/*
 * Format of
 * status register
 */
struct status_reg {
    union {
	struct {
	    unsigned short t1:1,
			   t0:1,
			    s:1,
			    m:1,
			     :1,
			  ipl:3,
			     :3,
			    x:1,
			    n:1,
			    z:1,
			    v:1,
			    c:1;
	} bits;
	struct {
	    unsigned char system;
	    unsigned char user;
	} bytes;
    } sr_un;
};
#define sr_t1 sr_un.bits.t1
#define sr_t0 sr_un.bits.t0
#define sr_s sr_un.bits.s
#define sr_m sr_un.bits.m
#define sr_ipl sr_un.bits.ipl
#define sr_x sr_un.bits.x
#define sr_n sr_un.bits.n
#define sr_z sr_un.bits.z
#define sr_v sr_un.bits.v
#define sr_c sr_un.bits.c
#define sr_sys sr_un.bytes.system
#define sr_user sr_un.bytes.user
#define sr_cc sr_user

/*
 * Generic operation
 * sizes
 */
#define OPSIZE_BYTE		0x0
#define OPSIZE_WORD		0x1
#define OPSIZE_LONG		0x2
#define OPSIZE_DBLONG		0x3
#define OPSIZE_NONE		0x4

/*
 * Instruction extension words
 */
struct exten_brief {
    unsigned short ext_iregtyp:1,
  		      ext_ireg:3,
		     ext_isize:1,
		     ext_scale:2,
		      ext_full:1,
		      ext_disp:8;
};

struct exten_full {
    unsigned short ext_iregtyp:1,
		      ext_ireg:3,
		     ext_isize:1,
		     ext_scale:2,
		      ext_full:1,
			ext_bs:1,
			ext_is:1,
		    ext_bdsize:2,
			      :1,
		      ext_isel:3;
};

/*
 * Memory Cell
 * allows access to
 * shorts and bytes.
 */
struct cell {
    union {
	struct {
	    unsigned char msbyte;
	    unsigned char msmbyte;
	    unsigned char lsmbyte;
	    unsigned char lsbyte;
	} bytes;
	struct {
	    unsigned short hiword;
	    unsigned short loword;
	} shortwords;
	unsigned char byte;
	unsigned short word;
	unsigned long longword;
    } un;
};

#define c_lsbyte un.bytes.lsbyte
#define c_msmbyte un.bytes.msmbyte
#define c_lsmbyte un.bytes.lsmbyte
#define c_lsbyte un.bytes.lsbyte
#define c_hiword un.shortwords.hiword
#define c_loword un.shortwords.loword
#define c_byte un.byte
#define c_word un.word
#define c_longword un.longword

/*
 * Instruction decode
 */
#define DIGIT_0_MASK    0xf000
#define DIGIT_1_MASK    0x0f00
#define DIGIT_2_MASK    0x00f0
#define DIGIT_3_MASK    0x000f

/*
 * privileged instruction groups
 */
#define INST_ANDI_SR    0
#define INST_EORI_SR    1
#define INST_ORI_SR     2
#define INST_MOVE_F_SR  3
#define INST_MOVE_T_SR  4
#define INST_MOVE_USP   5
#define INST_MOVEC      6
#define INST_MOVES      7
#define INST_RESET      8
#define INST_RTE        9
#define INST_STOP       10
#define INST_cpBcc      11
#define INST_cpDBcc     12
#define INST_cpGEN      13
#define INST_cpRESTORE  14
#define INST_cpSAVE     15
#define INST_cpScc      16
#define INST_cpTRAPcc   17

/*
 * Processor control registers
 */
#define R_SFC		0
#define R_DFC		1
#define R_CACR		2
#define R_USP		3
#define R_VBR		4
#define R_CAAR		5
#define R_MSP		6
#define R_ISP		7
