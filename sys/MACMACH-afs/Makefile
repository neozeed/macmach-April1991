SOURCE_DIR=..
IDENT=-DCMU -DINET -DMACH -DCMUCS -DCMU_MACMACH -DMM_XXX -DMM_BUGFIX
#
# Mach Operating System
# Copyright (c) 1989 Carnegie-Mellon University
# Copyright (c) 1988 Carnegie-Mellon University
# Copyright (c) 1987 Carnegie-Mellon University
# Copyright (c) 1986 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement specifies
# the terms and conditions for use and redistribution.
#  
#
# HISTORY
# $Log:	Makefile.template,v $
# Revision 2.33  90/07/03  16:36:41  mrt
# 	Added . and ${SOURCE_DIR}/sys to the INCLUDES flags so
# 	[90/05/21  17:06:29  mrt]
# 
#	that BSD style #includes can be used.
# 	[90/05/17  17:54:53  mrt]
# 
# Revision 2.32  89/10/11  13:57:51  dlb
# 	Added build commands for kernel monitor.
# 	Add mach_host.defs and related files.
# 
# Revision 2.31  89/10/10  10:49:37  mwyoung
# 	Fixes to allow lint to work.
# 	[89/08/07            mwyoung]
# 
# Revision 2.30  89/06/12  14:54:14  jsb
# 	Changed mach/mach.h is MACH_GENFILES to mach/mach_interface.h
# 	[89/06/05            mrt]
# 
# Revision 2.29  89/05/11  14:40:50  gm0w
# 	Removed explicit dependencies on file generated in ./mach/.
# 	These dependencies were previously required to trigger
# 	the ./mach rules; now mig{,debug}{,machine}files does this.
# 	[89/05/07  20:10:16  rpd]
# 
# Revision 2.28  89/05/06  02:56:49  rpd
# 	Removed references to ./mach/mach_user.c, which is no longer generated.
# 	[89/05/06  02:39:13  rpd]
# 
# Revision 2.27  89/05/01  15:23:05  rpd
# 	Changed remaining references to ./mach/memory_object.h to
# 	./mach/memory_object_user.h.
# 	[89/05/01  13:57:19  rpd]
# 
# Revision 2.26  89/04/09  00:06:29  rpd
# 	Removed MIG_BOGUS_FILES and other Mig crap.
# 	[89/04/09  00:05:53  rpd]
# 
# Revision 2.25  89/02/25  17:39:48  gm0w
# 	Removed obsolete rules for links, tags, printing, etc.
# 	[89/02/19            rpd]
# 
# 	Added MIG_BOGUS_FILES and rules to create the bogus files.
# 	The Mig-generated files use these old names.
# 	[89/02/19            rpd]
# 
# 	Converted migfiles/migdebugfiles rules to new format.
# 	[89/02/18            rpd]
# 
# Revision 2.24  89/01/31  01:50:24  rpd
# 	Picked up dlb's migfiles/migdebugfiles changes, but flushed
# 	the mach_host additions.  (They will get merged later.)
# 	[89/01/31            rpd]
# 
# 	Add mach_host.defs and related files.
# 	Add intermediate targets to kick off interface mig runs early.
# 	[89/01/25            dlb]
# 
# Revision 2.23  89/01/24  17:00:55  af
# 	Fixed the broken set of rules for mach_debug.   No comment.
# 
# Revision 2.22  89/01/23  22:16:16  af
# 	The basic change is that locore.s and ioconf.c are no longer
# 	special cased.  ioconf.c is now specified in files.  If you need
# 	some special treatment for locore.s, then do it in the machine
# 	dependent Makefile rules.  These changes where necessary to be
# 	more general in order to accomodate mips.
# 	
# 	The %<foo> order was changed to have %MACHDEP last.  So
# 	that it could have rules that dependended on SOBJS and COBJS.
# 	
# 	"makevariables" get emited after the %MACHDEP expansion.
# 	This means that the %MACHDEP can have variables that overide the
# 	defaults in the Makefile.template and that a given configuration
# 	file can overide the variables in the %MACHDEP (local Makefile).
# 	This works wonderfully for gcc.
# 	
# 	The text following a "|" in the files file, will be
# 	emitted as part of the rule for the given file.
# 	
# 	Lastly, you might want to look at the comments in the src/config
# 	program, especially in mkmakefile.c
# 	[89/01/09            rvb]
# 
#
# Revision 2.21  89/01/15  16:19:56  rpd
# 	Moved interfaces from kern/ to mach/.
# 	Moved rules for the mach_debug_ca interface to Makefile.ca.
# 	[89/01/15  14:53:06  rpd]
# 
# Revision 2.20  89/01/12  07:42:30  rpd
# 	Updated for new Mig files in kern/, mach_debug/, ca/ directories.
# 	[89/01/12  04:34:35  rpd]
# 
# Revision 2.19  88/12/19  02:40:21  mwyoung
# 	Split out ALLOPTS from old LINTOPTS, and provide VOLATILE only
# 	for lint.  I'm not sure why it's not safe to always use
# 	VOLATILE, but Bob didn't set things up that way.
# 	[88/12/19            mwyoung]
# 	
# 	Add volatile definition for lint.  It should be machine-dependent,
# 	but all forms of lint are based on pcc currently.
# 	[88/12/17            mwyoung]
# 
# Revision 2.18  88/11/23  16:35:34  rpd
# 	Added comments explaining PRELDDEPS.
# 	[88/11/05  13:45:58  rpd]
# 
# Revision 2.17  88/10/20  18:19:42  mwyoung
# 	Move the "default" target ahead of .PRECIOUS.
# 	[88/10/18            mwyoung]
# 
# Revision 2.16  88/10/18  03:17:29  mwyoung
# 	Update Makefile (with "md") only upon exit.
# 	[88/10/17            mwyoung]
# 
# Revision 2.15  88/09/07  15:47:41  rpd
# 	Explicitly use generated make script when checking dependencies.
# 	[88/09/05  21:32:22  rpd]
# 
# Revision 2.14  88/08/06  18:07:21  rpd
# Convert to use new memory_object files.
# No longer need -I flags, because the generated make script
# does it all with CPATH.
# 
# Revision 2.13  88/07/17  20:38:52  mwyoung
# Fix typo in previous change.
# 
# Revision 2.12  88/07/17  20:28:25  mwyoung
# Add mach_types.defs to the list of things to get from kern/.
# 
# Revision 2.11  88/07/17  18:48:49  mwyoung
# Convert to use new memory_object files.
# 
# Revision 2.10  88/07/15  15:48:26  mja
# Don't reset CPATH here since it is now set in the calling
# ./make script to track VPATH;  pass ../conf/copyright to
# ../conf/newvers.sh for VPATH;  define MD variable;  correct
# dependencies on kern/* and replace wildcards with explicit
# names all for VPATH.
# 
# 20-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Always remove and rebuild kern/ when dependencies change.
#
# 17-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	More dependencies on kern/ files.
#
# 11-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Simplify B_RULEs to decode directly from the source directory.
#
#  7-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Build MiG-generated interfaces in the build directory.
#	Unfortunately, the generated file "mach.h" conflicts with the
#	option file of the same name, so the interfaces get built in a
#	subdirectory.
#
#  7-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Decode binary (".b") files into object files.
#
#  2-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Temporarily added rules to make "strings.h" until MiG can be
#	convinced not to include it unless strings are being used.
#
# 18-May-88  Mike Accetta (mja) at Carnegie-Mellon University
#	Add .PRECIOUS target to prevent Makefile from ever being
#	deleted on interrupt.
#	[ V5.1(XF25) ]
#
# 09-Apr-88  Mike Accetta (mja) at Carnegie-Mellon University
#	Corrected object and binary rules to permit errors and remove
#	target before copying in new version (in case it is not
#	writeable);  added version file for patch level;  changed "md"
#	invocation to simply wildcard all files rather than only those
#	newer than "Makefile" since this can often cause files to be
#	overlooked if "Makefile" is touched before they are
#	incorporated.
#	[ V5.1(XF23) ]
#
# 31-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Allow source directory to be other than "..".
#
# 31-Mar-88  Robert Baron (rvb) at Carnegie-Mellon University
#	Need to have ${VOLATILE} for genassym
#
# 28-Mar-88  Robert Baron (rvb) at Carnegie-Mellon University
#	Add macros VOLATILE and FORCE_VOLATILE as per change in
#	conf/files.<machine> file:
#	VOLATILE	is included if the file contains the ansi C
#			 volatile keyword
#	FORCE_VOLATILE	is included if the file has volatile memory 
#			references, but does not use declare them
#			correctly.
#	(also see comment in config/mkmakefile.c)
#
# 29-Feb-88  Robert Baron (rvb) at Carnegie-Mellon University
#	Add VOLATILE macro which is fed to the preprocessor to macro define
#	the "keyword" volatile to an empty string.  On gcc builds, the
#	local Makefile will define VOLATILE= which lets the volatile
#	definitions show thru.
#
#  6-Jan-88  Michael Young (mwyoung) at Carnegie-Mellon University
#	Change references from h/ to sys/.
#	Remove all but the most pertinent history.
#
# 15-Dec-87  Robert Baron (rvb) at Carnegie-Mellon University
#	Add rules to profile a system as it is build.  The target is
#	gprof.
#
# 09-Nov-87  Mike Accetta (mja) at Carnegie-Mellon University
#	Update LDDEPS to include remaining files used by .swap.sys rule
#	in order to work with RCS; add CFILES dependency to "tags"
#	rule;  added new SYSDEPS definition;  change to create
#	symbols.sort in build directory;  fix to use explicit names in
#	version files to work with RCS;  add new "checkout" rule to
#	permit manual extraction of files from RCS when the
#	dependencies aren't current;  added KCC and KCPP definitions
#	for compiling kernel modules to clear the default search list
#	to prevent accidental inclusion of external include files
#	(especially important when working with header files in RCS
#	where this can easily occur from a missing dependency and a
#	checked-in source).
#	[ V5.1(XF21) ]
#
# 24-Oct-87  David Black (dlb) at Carnegie-Mellon University
#	Name version files explicitly to avoid .BAK .CKP etc.
#
# 10-Oct-87  Mike Accetta (mja) at Carnegie-Mellon University
#	Updated to pass all four version component files to the
#	"newvers.sh" script (which now includes them as kernel
#	variables and creates a version suffix) and changed SYS_RULE_4
#	to also link kernel images with the version suffix provided by
#	this script;  corrected "clean" rule to use new "LOAD" symbol
#	(provided by the "config" program) to remove all configured
#	kernel images related files;  revised symbols.sort rule to
#	use a new MACHINEDIR variable so that it can be overridden
#	for use with directories other than ../machine
#	[ V5.1(XF18) ]
#
# 11-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
#	Flushed all the "features.h" stuff since this is now a machine
#	independent file.  The equivalent function is now performed by
#	the "doconf" script if an appropriate features file exists in
#	the "machine" directory.
#	[ V5.1(F6) ]
#
# 19-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
#	Created (from old Makefile.vax).
#	[ V5.1(F1) ]
#
# 01-Oct-86  Mike Accetta (mja) at Carnegie-Mellon University
#	Reorganized to support simplified config program:
#	- COPTS_P variable defines the options used differently when
#	  compiling profiled kernels
#	- LDOBJS is the set of special objects used by the kernel load(s)
#	- LDDEPS is the set of dependencies for the kernel load(s)
#	- SWAPDEPS is the set of dependencies for the swapping module(s)
#	- Kernel configurations are built from a default rule defined here
#	  instead of inside config;  an artificial .swap.sys rule is used 
#	  to trigger this for each swap configuration
#	- Standard rules for building objects in the various normal, driver,
#	  and profiling cases are also defined here instead of inside config.
#	
#	Removed special rules for param.c and replaced PARAM variable
#	with include of new "confdep.h" file now generated by config;
#	also changed ioconf.c to use standard rules.
#	
#	Revised version number processing to eliminate newvers.c program and
#	only increment the version number as part of the archival process
#       rather than on every build.
#
# 18-Apr-86  Robert V. Baron (rvb) at Carnegie-Mellon University
#	Introduce new make depend stuff.
#

#
# Copyright (c) 1980, 1986 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#

#
#  This must be here before any rules are possibly defined by the
#  machine dependent makefile fragment so that a plain "make" command
#  always works.  The config program will emit an appropriate rule to
#  cause "all" to depend on every kernel configuration it generates.
#

default: all

#
#  In order to move the link commands out of config and back into the
#  template makefile, config has been changed to emit a dendency rule
#  of the form *vmunix.sys: *vmunix.swap for each configuration so that
#  the common commands for building each configuration can be placed
#  directly in the template in the default rule for the .sys.swap
#  dependency.
#
.SUFFIXES: .swap .sys

#
#  Make sure we don't remove this by accident if interrupted at the wrong
#  time.
#
.PRECIOUS: Makefile


#
#  Compilation components
#
CC=	cc
CPP=	${CC} -ES
C2=	/lib/c2
LD=	ld
MD=	/usr/mach/bin/md
ENCODE=	uuencode
DECODE=	uudecode
VOLATILE="-Dvolatile=" 
FORCE_VOLATILE=

KCC=	${CC}
KCPP=	${KCC} -ES

#
#  This changed in the 4.3 CPP from .n to .d
#
MD_SUFFIX=d

#
#  Pre-processor environment
#
DEPENDS=  -MD
INCLUDES= -I. -I$(SOURCE_DIR)/sys
ALLOPTS= ${INCLUDES} ${IDENT} -DKERNEL
LINTOPTS=${ALLOPTS} ${VOLATILE}

#
#  Common C-compilation flags
#
COPTS_P=${DEPENDS} ${ALLOPTS}
COPTS=	${COPTS_P}
CFLAGS=	-O ${COPTS}

#
#  Common assembly flags
#
SFLAGS=	${DEPENDS} ${INCLUDES} ${IDENT} -DKERNEL -DASSEMBLER


#
#  Path name of the version identification file.
#
FEATURES=$(SOURCE_DIR)/machine/FEATURES.h
VERSION_FILES= \
	$(SOURCE_DIR)/conf/version.major \
	$(SOURCE_DIR)/conf/version.minor \
	$(SOURCE_DIR)/conf/version.variant \
	$(SOURCE_DIR)/conf/version.edit \
	$(SOURCE_DIR)/conf/version.patch

#
#  LDOBJS is the set of object files which comprise the kernel.
#  It is used both in the dependency list for each *vmunix.swap
#  rule emitted by config and also in the .sys.swap rule
#  below which links the kernel from these objects.
#  
#  LDOBJS_PREFIX and LDOBJS_SUFFIX are defined in the machine
#  dependent Makefile (if necessary).
#
LDOBJS=${LDOBJS_PREFIX} ${OBJS} ${LDOBJS_SUFFIX}

#
#  LDDEPS is the set of extra dependencies associated with each
#  *vmunix.swap rule (in addition to $LDOBJS and the swap*.o file).
#
#  LDDEPS_PREFIX is defined in the machine dependent Makefile
#  (if necessary).
#
LDDEPS=${LDDEPS_PREFIX} ${VERSION_FILES} \
	$(SOURCE_DIR)/conf/newvers.sh $(SOURCE_DIR)/conf/copyright symbols.sort

#
#  PRELDDEPS is another set of extra dependencies associated with each
#  *vmunix.swap rule (in addition to $LDOBJS and the swap*.o file).
#  It is defined in the machine dependent Makefile (if necessary).
#
#  The generated rule looks like
#	vmunix.sys : ${PRELDDEPS} ${LDOBJS} ${LDDEPS}
#

#
#  SWAPDEPS is the set of extra dependencies associated with each
#  swap*.o rule emitted by config (in addition to the
#  ../machine/swap*.c file).
#
#  SWAPDEPS_PREFIX is defined in the machine dependent Makefile
#  (if necessary).
#
SWAPDEPS=${SWAPDEPS_PREFIX} 

#
#  SYSDEPS is the set of extra dependencies associated with each
#  *vmunix rule (in addition to *vmunix.sys).
#
#  SYSDEPS_PREFIX is defined in the machine dependent Makefile
#  (if necessary).
#
SYSDEPS=${SYSDEPS_PREFIX}



#
#  Compilation rules to generate *vmunix from *vmunix.sys
#
SYS_RULE_1=@eval `awk 'NR==1{S=$$1;next;}\
END {\
  C = "ln $@.sys $@";\
  if (S != "") {\
    C = C "; ln $@ $@" S; \
    printf "rm -f $@%s; ", S; \
  }\
  printf "echo \"%s\"; %s\n", C, C; \
}' vers.suffix`
SYS_RULE_2=
SYS_RULE_3=
SYS_RULE_4=

#
#  Things to be done just before exiting
#
EXIT_RULE=${MD} -f -d `ls *.${MD_SUFFIX}`

#
# Compilation rules to generate .o from .b
#
B_RULE_1A=rm -f $*.o; ${DECODE} 
B_RULE_1B=$*.b

#
# Compilation rules to generate .o from .o
#
O_RULE_1A=rm -f $*.o; cp 
O_RULE_1B=$*.o .

#
# Compilation rules to generate .o from .s
#
S_RULE_1A=${KCPP} ${SFLAGS} 
S_RULE_1B=$*.s >$*.i 
S_RULE_2=${AS} -o $*.o $*.i
S_RULE_3=rm -f $*.i

#
# Compilation rules to generate .o from .c for normal files
#
C_RULE_1A=${KCC} -c ${CFLAGS} 
C_RULE_1B=$*.c
C_RULE_2=
C_RULE_3=
C_RULE_4=

#
# Compilation rules to generate .o from .c for driver files
#
C_RULE_1A_D=${C_RULE_1A}
C_RULE_1B_D=${C_RULE_1B}
C_RULE_2_D=${C_RULE_2}
C_RULE_3_D=${C_RULE_3}
C_RULE_4_D=${C_RULE_4}

#
# Compilation rules to generate .o from .c for profiling routine files
#
C_RULE_1A_P=${KCC} -c -S ${COPTS_P} 
C_RULE_1B_P=${C_RULE_1B}
C_RULE_2_P=ex - $*.s < ${GPROF.EX}
C_RULE_3_P=${AS} -o $*.o $*.s
C_RULE_4_P=rm -f $*.s



gprof: kgmon_on all kgmon_off

kgmon_on:
	kgmon -r -b

kgmon_off:
	kgmon -h -p
	gprof /mach >GPROF


#
#  Theses macros are filled in by the config program depending on the
#  current configuration.  The MACHDEP macro is replaced by the
#  contents of the machine dependent makefile template and the others
#  are replaced by the corresponding symbol definitions for the
#  configuration.
#


OBJS=cmu_syscalls.o init_main.o init_sysent.o kern_acct.o kern_clock.o \
	kern_descrip.o kern_exec.o kern_exit.o kern_fork.o kern_mman.o \
	kern_proc.o kern_prot.o kern_resource.o kern_sig.o kern_subr.o \
	kern_synch.o kern_time.o kern_xxx.o mach_process.o mach_signal.o \
	quota_sys.o subr_log.o subr_prf.o subr_rmap.o subr_xxx.o \
	sys_generic.o sys_socket.o tty.o tty_cmupty.o tty_conf.o \
	tty_pty.o tty_subr.o tty_tty.o ufs_alloc.o ufs_disksubr.o \
	ufs_inode.o ufs_physio.o ufs_subr.o ufs_tables.o ufs_xxx.o \
	uipc_domain.o uipc_mbuf.o uipc_proto.o uipc_socket.o \
	uipc_socket2.o uipc_syscalls.o uipc_usrreq.o device_pager.o \
	inode_pager.o ux_exception.o param.o ast.o clock_prim.o debug.o \
	exception.o host.o iprintf.o lock.o mach_factor.o ipc_basics.o \
	ipc_copyin.o ipc_copyout.o ipc_host.o ipc_kport.o ipc_kset.o \
	ipc_pobj.o ipc_port.o ipc_tt.o ipc_globals.o ipc_mports.o \
	ipc_prims.o ipc_ptraps.o kalloc.o mach_net.o mach_timedev.o \
	mach_user_internal.o machine.o mfs_prim.o mig_support.o \
	parallel.o processor.o queue.o sched_prim.o slave.o \
	syscall_emulation.o syscall_subr.o syscall_sw.o task.o thread.o \
	thread_swap.o timer.o zalloc.o exc_server.o exc_user.o \
	mach_host_server.o mach_server.o memory_object_default_user.o \
	memory_object_user.o mach_debug_server.o af.o dli.o if.o \
	if_loop.o netisr.o raw_cb.o raw_usrreq.o route.o if_ether.o \
	igmp.o in.o in_pcb.o in_proto.o ip_icmp.o ip_input.o ip_mroute.o \
	ip_output.o raw_ip.o tcp_debug.o tcp_input.o tcp_output.o \
	tcp_subr.o tcp_timer.o tcp_usrreq.o udp_usrreq.o xdr.o \
	xdr_array.o xdr_mem.o bdev_vnodeops.o fifo_vnodeops.o \
	spec_subr.o spec_vnodeops.o ufs_bmap.o ufs_dir.o ufs_vfsops.o \
	ufs_vnodeops.o vfs.o vfs_bio.o vfs_conf.o vfs_dnlc.o vfs_io.o \
	vfs_lookup.o vfs_pathname.o vfs_syscalls.o vfs_sysnames.o \
	vfs_vnode.o memory_object.o vm_fault.o vm_init.o vm_kern.o \
	vm_map.o vm_object.o vm_pageout.o vm_resident.o vm_unix.o \
	vm_user.o ioconf.o conf.o cons.o kb.o fdb.o fdbs.o key.o mouse.o \
	font1.o font_524.o font_521.o video.o drvr_120.o via.o scc.o \
	mscsi.o mhd.o if_en.o if_enintr_catch.o mactrap.o mackern.o \
	macserver_main.o macserver_routines.o macserver_phys_pages.o \
	macServer.o macmemory.o macostrap.o macostrap_tables.o \
	macexception.o macintr.o macuser.o macinst.o macinst_subr.o \
	macbus_cycle.o macvia.o macdevice.o macpmap.o macscsi_mgr.o \
	macinput.o macscc.o macether_routines.o macether_interface.o \
	macether_hash.o slotmgr_principal.o slotmgr_special.o \
	slotmgr_advanced.o slotmgr_subr.o slotmgr_SExecCode.o \
	slotmgr_alloc.o slotmgr_dispatch.o slotmgr_extra.o \
	slotmgr_interrupts.o slotmgr_SCallIntr.o slotmgr_CallDriver.o \
	ostrap.o ostrap_tables.o mem.o virt.o setjmp.o ivect.o \
	mac2_mach_process.o ufs_machdep.o autoconf.o clock.o machdep.o \
	pcb.o pmap.o act.o softint.o mac2_init.o mac2_exception.o trap.o \
	in_cksum.o vm_machdep.o strings.o 

CFILES=\
	$(SOURCE_DIR)/bsd/cmu_syscalls.c \
	$(SOURCE_DIR)/bsd/init_main.c \
	$(SOURCE_DIR)/bsd/init_sysent.c \
	$(SOURCE_DIR)/bsd/kern_acct.c \
	$(SOURCE_DIR)/bsd/kern_clock.c \
	$(SOURCE_DIR)/bsd/kern_descrip.c \
	$(SOURCE_DIR)/bsd/kern_exec.c \
	$(SOURCE_DIR)/bsd/kern_exit.c \
	$(SOURCE_DIR)/bsd/kern_fork.c \
	$(SOURCE_DIR)/bsd/kern_mman.c \
	$(SOURCE_DIR)/bsd/kern_proc.c \
	$(SOURCE_DIR)/bsd/kern_prot.c \
	$(SOURCE_DIR)/bsd/kern_resource.c \
	$(SOURCE_DIR)/bsd/kern_sig.c \
	$(SOURCE_DIR)/bsd/kern_subr.c \
	$(SOURCE_DIR)/bsd/kern_synch.c \
	$(SOURCE_DIR)/bsd/kern_time.c \
	$(SOURCE_DIR)/bsd/kern_xxx.c \
	$(SOURCE_DIR)/bsd/mach_process.c \
	$(SOURCE_DIR)/bsd/mach_signal.c \
	$(SOURCE_DIR)/bsd/quota_sys.c \
	$(SOURCE_DIR)/bsd/subr_log.c \
	$(SOURCE_DIR)/bsd/subr_prf.c \
	$(SOURCE_DIR)/bsd/subr_rmap.c \
	$(SOURCE_DIR)/bsd/subr_xxx.c \
	$(SOURCE_DIR)/bsd/sys_generic.c \
	$(SOURCE_DIR)/bsd/sys_socket.c \
	$(SOURCE_DIR)/bsd/tty.c \
	$(SOURCE_DIR)/bsd/tty_cmupty.c \
	$(SOURCE_DIR)/bsd/tty_conf.c \
	$(SOURCE_DIR)/bsd/tty_pty.c \
	$(SOURCE_DIR)/bsd/tty_subr.c \
	$(SOURCE_DIR)/bsd/tty_tty.c \
	$(SOURCE_DIR)/bsd/ufs_alloc.c \
	$(SOURCE_DIR)/bsd/ufs_disksubr.c \
	$(SOURCE_DIR)/bsd/ufs_inode.c \
	$(SOURCE_DIR)/bsd/ufs_physio.c \
	$(SOURCE_DIR)/bsd/ufs_subr.c \
	$(SOURCE_DIR)/bsd/ufs_tables.c \
	$(SOURCE_DIR)/bsd/ufs_xxx.c \
	$(SOURCE_DIR)/bsd/uipc_domain.c \
	$(SOURCE_DIR)/bsd/uipc_mbuf.c \
	$(SOURCE_DIR)/bsd/uipc_proto.c \
	$(SOURCE_DIR)/bsd/uipc_socket.c \
	$(SOURCE_DIR)/bsd/uipc_socket2.c \
	$(SOURCE_DIR)/bsd/uipc_syscalls.c \
	$(SOURCE_DIR)/bsd/uipc_usrreq.c \
	$(SOURCE_DIR)/builtin/device_pager.c \
	$(SOURCE_DIR)/builtin/inode_pager.c \
	$(SOURCE_DIR)/builtin/ux_exception.c \
	$(SOURCE_DIR)/conf/param.c \
	$(SOURCE_DIR)/kern/ast.c \
	$(SOURCE_DIR)/kern/clock_prim.c \
	$(SOURCE_DIR)/kern/debug.c \
	$(SOURCE_DIR)/kern/exception.c \
	$(SOURCE_DIR)/kern/host.c \
	$(SOURCE_DIR)/kern/iprintf.c \
	$(SOURCE_DIR)/kern/lock.c \
	$(SOURCE_DIR)/kern/mach_factor.c \
	$(SOURCE_DIR)/kern/ipc_basics.c \
	$(SOURCE_DIR)/kern/ipc_copyin.c \
	$(SOURCE_DIR)/kern/ipc_copyout.c \
	$(SOURCE_DIR)/kern/ipc_host.c \
	$(SOURCE_DIR)/kern/ipc_kport.c \
	$(SOURCE_DIR)/kern/ipc_kset.c \
	$(SOURCE_DIR)/kern/ipc_pobj.c \
	$(SOURCE_DIR)/kern/ipc_port.c \
	$(SOURCE_DIR)/kern/ipc_tt.c \
	$(SOURCE_DIR)/kern/ipc_globals.c \
	$(SOURCE_DIR)/kern/ipc_mports.c \
	$(SOURCE_DIR)/kern/ipc_prims.c \
	$(SOURCE_DIR)/kern/ipc_ptraps.c \
	$(SOURCE_DIR)/kern/kalloc.c \
	$(SOURCE_DIR)/kern/mach_net.c \
	$(SOURCE_DIR)/kern/mach_timedev.c \
	$(SOURCE_DIR)/kern/mach_user_internal.c \
	$(SOURCE_DIR)/kern/machine.c \
	$(SOURCE_DIR)/kern/mfs_prim.c \
	$(SOURCE_DIR)/kern/mig_support.c \
	$(SOURCE_DIR)/kern/parallel.c \
	$(SOURCE_DIR)/kern/processor.c \
	$(SOURCE_DIR)/kern/queue.c \
	$(SOURCE_DIR)/kern/sched_prim.c \
	$(SOURCE_DIR)/kern/slave.c \
	$(SOURCE_DIR)/kern/syscall_emulation.c \
	$(SOURCE_DIR)/kern/syscall_subr.c \
	$(SOURCE_DIR)/kern/syscall_sw.c \
	$(SOURCE_DIR)/kern/task.c \
	$(SOURCE_DIR)/kern/thread.c \
	$(SOURCE_DIR)/kern/thread_swap.c \
	$(SOURCE_DIR)/kern/timer.c \
	$(SOURCE_DIR)/kern/zalloc.c \
	./mach/exc_server.c \
	./mach/exc_user.c \
	./mach/mach_host_server.c \
	./mach/mach_server.c \
	./mach/memory_object_default_user.c \
	./mach/memory_object_user.c \
	./mach_debug/mach_debug_server.c \
	$(SOURCE_DIR)/net/af.c \
	$(SOURCE_DIR)/net/dli.c \
	$(SOURCE_DIR)/net/if.c \
	$(SOURCE_DIR)/net/if_loop.c \
	$(SOURCE_DIR)/net/netisr.c \
	$(SOURCE_DIR)/net/raw_cb.c \
	$(SOURCE_DIR)/net/raw_usrreq.c \
	$(SOURCE_DIR)/net/route.c \
	$(SOURCE_DIR)/netinet/if_ether.c \
	$(SOURCE_DIR)/netinet/igmp.c \
	$(SOURCE_DIR)/netinet/in.c \
	$(SOURCE_DIR)/netinet/in_pcb.c \
	$(SOURCE_DIR)/netinet/in_proto.c \
	$(SOURCE_DIR)/netinet/ip_icmp.c \
	$(SOURCE_DIR)/netinet/ip_input.c \
	$(SOURCE_DIR)/netinet/ip_mroute.c \
	$(SOURCE_DIR)/netinet/ip_output.c \
	$(SOURCE_DIR)/netinet/raw_ip.c \
	$(SOURCE_DIR)/netinet/tcp_debug.c \
	$(SOURCE_DIR)/netinet/tcp_input.c \
	$(SOURCE_DIR)/netinet/tcp_output.c \
	$(SOURCE_DIR)/netinet/tcp_subr.c \
	$(SOURCE_DIR)/netinet/tcp_timer.c \
	$(SOURCE_DIR)/netinet/tcp_usrreq.c \
	$(SOURCE_DIR)/netinet/udp_usrreq.c \
	$(SOURCE_DIR)/rpc/xdr.c \
	$(SOURCE_DIR)/rpc/xdr_array.c \
	$(SOURCE_DIR)/rpc/xdr_mem.c \
	$(SOURCE_DIR)/specfs/bdev_vnodeops.c \
	$(SOURCE_DIR)/specfs/fifo_vnodeops.c \
	$(SOURCE_DIR)/specfs/spec_subr.c \
	$(SOURCE_DIR)/specfs/spec_vnodeops.c \
	$(SOURCE_DIR)/vfs/ufs_bmap.c \
	$(SOURCE_DIR)/vfs/ufs_dir.c \
	$(SOURCE_DIR)/vfs/ufs_vfsops.c \
	$(SOURCE_DIR)/vfs/ufs_vnodeops.c \
	$(SOURCE_DIR)/vfs/vfs.c \
	$(SOURCE_DIR)/vfs/vfs_bio.c \
	$(SOURCE_DIR)/vfs/vfs_conf.c \
	$(SOURCE_DIR)/vfs/vfs_dnlc.c \
	$(SOURCE_DIR)/vfs/vfs_io.c \
	$(SOURCE_DIR)/vfs/vfs_lookup.c \
	$(SOURCE_DIR)/vfs/vfs_pathname.c \
	$(SOURCE_DIR)/vfs/vfs_syscalls.c \
	$(SOURCE_DIR)/vfs/vfs_sysnames.c \
	$(SOURCE_DIR)/vfs/vfs_vnode.c \
	$(SOURCE_DIR)/vm/memory_object.c \
	$(SOURCE_DIR)/vm/vm_fault.c \
	$(SOURCE_DIR)/vm/vm_init.c \
	$(SOURCE_DIR)/vm/vm_kern.c \
	$(SOURCE_DIR)/vm/vm_map.c \
	$(SOURCE_DIR)/vm/vm_object.c \
	$(SOURCE_DIR)/vm/vm_pageout.c \
	$(SOURCE_DIR)/vm/vm_resident.c \
	$(SOURCE_DIR)/vm/vm_unix.c \
	$(SOURCE_DIR)/vm/vm_user.c \
	./ioconf.c \
	$(SOURCE_DIR)/mac2/conf.c \
	$(SOURCE_DIR)/mac2dev/cons.c \
	$(SOURCE_DIR)/mac2dev/kb.c \
	$(SOURCE_DIR)/mac2dev/fdb.c \
	$(SOURCE_DIR)/mac2dev/key.c \
	$(SOURCE_DIR)/mac2dev/mouse.c \
	$(SOURCE_DIR)/mac2dev/video.c \
	$(SOURCE_DIR)/mac2dev/via.c \
	$(SOURCE_DIR)/mac2dev/scc.c \
	$(SOURCE_DIR)/mac2dev/mscsi.c \
	$(SOURCE_DIR)/mac2dev/mhd.c \
	$(SOURCE_DIR)/mac2if/if_en.c \
	$(SOURCE_DIR)/mac2emul/mackern.c \
	$(SOURCE_DIR)/mac2emul/macserver_main.c \
	$(SOURCE_DIR)/mac2emul/macserver_routines.c \
	$(SOURCE_DIR)/mac2emul/macserver_phys_pages.c \
	$(SOURCE_DIR)/mac2emul/macServer.c \
	$(SOURCE_DIR)/mac2emul/macmemory.c \
	$(SOURCE_DIR)/mac2emul/macostrap.c \
	$(SOURCE_DIR)/mac2emul/macostrap_tables.c \
	$(SOURCE_DIR)/mac2emul/macexception.c \
	$(SOURCE_DIR)/mac2emul/macintr.c \
	$(SOURCE_DIR)/mac2emul/macinst.c \
	$(SOURCE_DIR)/mac2emul/macinst_subr.c \
	$(SOURCE_DIR)/mac2emul/macbus_cycle.c \
	$(SOURCE_DIR)/mac2emul/macvia.c \
	$(SOURCE_DIR)/mac2emul/macdevice.c \
	$(SOURCE_DIR)/mac2emul/macpmap.c \
	$(SOURCE_DIR)/mac2emul/macscsi_mgr.c \
	$(SOURCE_DIR)/mac2emul/macinput.c \
	$(SOURCE_DIR)/mac2emul/macscc.c \
	$(SOURCE_DIR)/mac2emul/macether_routines.c \
	$(SOURCE_DIR)/mac2emul/macether_interface.c \
	$(SOURCE_DIR)/mac2emul/macether_hash.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_principal.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_special.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_advanced.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_subr.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_alloc.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_dispatch.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_extra.c \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_interrupts.c \
	$(SOURCE_DIR)/mac2/ostrap.c \
	$(SOURCE_DIR)/mac2/ostrap_tables.c \
	$(SOURCE_DIR)/mac2/mem.c \
	$(SOURCE_DIR)/mac2/ivect.c \
	$(SOURCE_DIR)/mac2/mac2_mach_process.c \
	$(SOURCE_DIR)/mac2/ufs_machdep.c \
	$(SOURCE_DIR)/mac2/autoconf.c \
	$(SOURCE_DIR)/mac2/clock.c \
	$(SOURCE_DIR)/mac2/machdep.c \
	$(SOURCE_DIR)/mac2/pcb.c \
	$(SOURCE_DIR)/mac2/pmap.c \
	$(SOURCE_DIR)/mac2/act.c \
	$(SOURCE_DIR)/mac2/softint.c \
	$(SOURCE_DIR)/mac2/mac2_init.c \
	$(SOURCE_DIR)/mac2/mac2_exception.c \
	$(SOURCE_DIR)/mac2/trap.c \
	$(SOURCE_DIR)/mac2/in_cksum.c \
	$(SOURCE_DIR)/mac2/vm_machdep.c \
	$(SOURCE_DIR)/mac2/strings.c 
COBJS=cmu_syscalls.o init_main.o init_sysent.o kern_acct.o kern_clock.o \
	kern_descrip.o kern_exec.o kern_exit.o kern_fork.o kern_mman.o \
	kern_proc.o kern_prot.o kern_resource.o kern_sig.o kern_subr.o \
	kern_synch.o kern_time.o kern_xxx.o mach_process.o mach_signal.o \
	quota_sys.o subr_log.o subr_prf.o subr_rmap.o subr_xxx.o \
	sys_generic.o sys_socket.o tty.o tty_cmupty.o tty_conf.o \
	tty_pty.o tty_subr.o tty_tty.o ufs_alloc.o ufs_disksubr.o \
	ufs_inode.o ufs_physio.o ufs_subr.o ufs_tables.o ufs_xxx.o \
	uipc_domain.o uipc_mbuf.o uipc_proto.o uipc_socket.o \
	uipc_socket2.o uipc_syscalls.o uipc_usrreq.o device_pager.o \
	inode_pager.o ux_exception.o param.o ast.o clock_prim.o debug.o \
	exception.o host.o iprintf.o lock.o mach_factor.o ipc_basics.o \
	ipc_copyin.o ipc_copyout.o ipc_host.o ipc_kport.o ipc_kset.o \
	ipc_pobj.o ipc_port.o ipc_tt.o ipc_globals.o ipc_mports.o \
	ipc_prims.o ipc_ptraps.o kalloc.o mach_net.o mach_timedev.o \
	mach_user_internal.o machine.o mfs_prim.o mig_support.o \
	parallel.o processor.o queue.o sched_prim.o slave.o \
	syscall_emulation.o syscall_subr.o syscall_sw.o task.o thread.o \
	thread_swap.o timer.o zalloc.o exc_server.o exc_user.o \
	mach_host_server.o mach_server.o memory_object_default_user.o \
	memory_object_user.o mach_debug_server.o af.o dli.o if.o \
	if_loop.o netisr.o raw_cb.o raw_usrreq.o route.o if_ether.o \
	igmp.o in.o in_pcb.o in_proto.o ip_icmp.o ip_input.o ip_mroute.o \
	ip_output.o raw_ip.o tcp_debug.o tcp_input.o tcp_output.o \
	tcp_subr.o tcp_timer.o tcp_usrreq.o udp_usrreq.o xdr.o \
	xdr_array.o xdr_mem.o bdev_vnodeops.o fifo_vnodeops.o \
	spec_subr.o spec_vnodeops.o ufs_bmap.o ufs_dir.o ufs_vfsops.o \
	ufs_vnodeops.o vfs.o vfs_bio.o vfs_conf.o vfs_dnlc.o vfs_io.o \
	vfs_lookup.o vfs_pathname.o vfs_syscalls.o vfs_sysnames.o \
	vfs_vnode.o memory_object.o vm_fault.o vm_init.o vm_kern.o \
	vm_map.o vm_object.o vm_pageout.o vm_resident.o vm_unix.o \
	vm_user.o ioconf.o conf.o cons.o kb.o fdb.o key.o mouse.o \
	video.o via.o scc.o mscsi.o mhd.o if_en.o mackern.o \
	macserver_main.o macserver_routines.o macserver_phys_pages.o \
	macServer.o macmemory.o macostrap.o macostrap_tables.o \
	macexception.o macintr.o macinst.o macinst_subr.o macbus_cycle.o \
	macvia.o macdevice.o macpmap.o macscsi_mgr.o macinput.o macscc.o \
	macether_routines.o macether_interface.o macether_hash.o \
	slotmgr_principal.o slotmgr_special.o slotmgr_advanced.o \
	slotmgr_subr.o slotmgr_alloc.o slotmgr_dispatch.o \
	slotmgr_extra.o slotmgr_interrupts.o ostrap.o ostrap_tables.o \
	mem.o ivect.o mac2_mach_process.o ufs_machdep.o autoconf.o \
	clock.o machdep.o pcb.o pmap.o act.o softint.o mac2_init.o \
	mac2_exception.o trap.o in_cksum.o vm_machdep.o strings.o 

SFILES=\
	$(SOURCE_DIR)/mac2dev/fdbs.s \
	$(SOURCE_DIR)/mac2dev/font1.s \
	$(SOURCE_DIR)/mac2if/if_enintr_catch.s \
	$(SOURCE_DIR)/mac2emul/mactrap.s \
	$(SOURCE_DIR)/mac2emul/macuser.s \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_SExecCode.s \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_SCallIntr.s \
	$(SOURCE_DIR)/mac2slotmgr/slotmgr_CallDriver.s \
	$(SOURCE_DIR)/mac2/virt.s \
	$(SOURCE_DIR)/mac2/setjmp.s 
SOBJS=fdbs.o font1.o if_enintr_catch.o mactrap.o macuser.o \
	slotmgr_SExecCode.o slotmgr_SCallIntr.o slotmgr_CallDriver.o \
	virt.o setjmp.o 

BFILES=\
	$(SOURCE_DIR)/mac2dev/font_524.b \
	$(SOURCE_DIR)/mac2dev/font_521.b \
	$(SOURCE_DIR)/mac2dev/drvr_120.b 

ORDERED=
#
# Mach Operating System
# Copyright (c) 1986 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement
# specifies the terms and conditions for use and redistribution.
#
######################################################################
# HISTORY
# $Log:	Makefile.mac2,v $
# Revision 2.2  90/09/04  16:36:32  bohman
# 	Created.
# 
######################################################################

######################################################################
#BEGIN	Machine dependent Makefile fragment for the Macintosh II
######################################################################

CC=	cc -m68020
AS=	as -m68020

LD=	ld

MD=	md

KCC=	${CC}
KCPP=	${KCC} -E

VOLATILE=

LDFLAGS=-r -d -N -e _start -X
# LDFLAGS=-N -e _start -X -T 4000
LIBS=

LDDEPS_PREFIX=
SWAPDEPS_PREFIX=
OBJSDEPS=

INCLUDES=	-I$(SOURCE_DIR) ${INCLUDES}

AHEADS=

LDOBJS_PREFIX=locore.o
LDOBJS_SUFFIX=

#
#  Special rule to generate the locore.c from ../mac2/locore.c
#
LOCORE_DEPS=
LOCORE_HEADS=

locore.c: $(SOURCE_DIR)/mac2/locore.s ${LOCORE_DEPS}
	@rm -f $@
	cp $(SOURCE_DIR)/mac2/locore.s $@

locore.o: assym.s locore.c ${LOCORE_HEADS}
	${KCC} -E -DLOCORE -I$(SOURCE_DIR)/machine ${SFLAGS} locore.c >locore.i
	${AS} -o locore.o ${LOCORE_HEADS} locore.i
	rm locore.i

#
#  Compilation rules to generate .o from .c for normal files
#
C_RULE_1A=${KCC} -c -O ${COPTS} -fno-defer-pop 
C_RULE_1B=$*.c
C_RULE_2=
C_RULE_3=
C_RULE_4=

######################################################################
#END	 Machine dependent Makefile fragment for the Macintosh II
######################################################################


#
#  Default rule used to build a *vmunix.sys configuration from the
#  object list and a particular *vmunix.swap module.  The *vmunix.swap
#  module is a normal object file compiled from the appropriate swap*.c
#  file and then copied to the *vmunix.swap name to trigger the full
#  kernel link using this default rule.
#

.swap.sys:
	@/bin/sh $(SOURCE_DIR)/conf/newvers.sh \
			 $(SOURCE_DIR)/conf/copyright \
			`cat ${VERSION_FILES}`
	@${CC} -c vers.c
	@rm -f $* $*.sys
	@echo loading $*.sys
	@${LD} ${LDFLAGS} -x ${LDOBJS} vers.o $*.swap ${LIBS}
	@echo rearranging symbols
	@-grep -v '^#' $(SOURCE_DIR)/machine/symbols.raw \
	    | sed -e '/^$$/d' -e 's/^	//' | sort -u > symbols.tmp
	@-mv -f symbols.tmp symbols.sort
	@-symorder symbols.sort a.out
	@size a.out
	@chmod 755 a.out
	-${SWAPSYS}
	@-mv a.out $*.sys



#
#  OBJSDEPS is the set of files (defined in the machine dependent
#  template if necessary) which all objects depend on (such as an
#  in-line assembler expansion filter
#

${OBJS}: ${OBJSDEPS}



#
#  This macro is replaced by three rules per configuration, plus a
#  final definition for the LOAD symbol consisting of all configuration
#  names and a dependency of "all" on this symbol (so that the
#  "default" rule above will build everything).  The three rules in
#  each configuration are:
#
#  - a *vmunix from *vmunix.sys dependency using the SYS_RULE_* rules,
#  - a *vmunix.swap from LDDEPS, swap*.o, and LDDEPS dependency that
#    simply copies the swap file to the target name, and
#  - a swap*.o from swap*.c and SWAPDEPS dependency using the C_RULE_* rules.
#

migfiles ::

migmachinefiles ::

migdebugfiles ::

migdebugmachinefiles ::

vmunix .ORDER: migfiles migmachinefiles migdebugfiles migdebugmachinefiles vmunix.sys ${SYSDEPS}
	${SYS_RULE_1}
	${SYS_RULE_2}
	${SYS_RULE_3}
	${SYS_RULE_4}

vmunix.sys:${P} ${PRELDDEPS} ${LDOBJS} ${LDDEPS}

vmunix.swap: swapgeneric.o
	@cp swapgeneric.o $@

swapgeneric.o: $(SOURCE_DIR)/machine/swapgeneric.c ${SWAPDEPS}
	${C_RULE_1A}$(SOURCE_DIR)/machine/${C_RULE_1B}
	${C_RULE_2}
	${C_RULE_3}
	${C_RULE_4}

LOAD = vmunix

all: ${LOAD}




#
#  Rules for components which are not part of the kernel proper or that
#  need to be built in a special manner.
#

genassym.o: $(SOURCE_DIR)/machine/genassym.c
	${KCC} -c ${COPTS_P} ${VOLATILE} $(SOURCE_DIR)/machine/genassym.c

assym.s: genassym.o
	${KCC} genassym.o
	./a.out > assym.s; rm -f a.out

locore.o: assym.s ${LOCORE_HEADS}

#
#  Mach IPC-based interfaces
#

MACH_DEFSFILES=\
	$(SOURCE_DIR)/mach/std_types.defs \
	$(SOURCE_DIR)/mach/mach_types.defs \
	${SOURCE_DIR}/mach/mach.defs \
	${SOURCE_DIR}/mach/mach_host.defs \
	${SOURCE_DIR}/mach/exc.defs \
	${SOURCE_DIR}/mach/memory_object.defs \
	${SOURCE_DIR}/mach/memory_object_default.defs \
	${SOURCE_DIR}/mach/device_types.defs \
	${SOURCE_DIR}/mach/device.defs \
	${SOURCE_DIR}/mach/monitor.defs

MACH_GENFILES=\
	./mach/mach_server.c \
	./mach/mach_user_internal.h \
	./mach/mach_interface.h \
	./mach/mach_host_server.c \
	./mach/mach_host.h \
	./mach/memory_object_server.c \
	./mach/memory_object_user.c \
	./mach/memory_object_user.h \
	./mach/memory_object_default_server.c \
	./mach/memory_object_default_user.c \
	./mach/memory_object_default.h \
	./mach/device_server.c \
	./mach/exc_server.c \
	./mach/exc_user.c \
	./mach/exc.h \
	./mach/monitor_server.c \
	./mach/monitor.h

MACH_MKFILE=	$(SOURCE_DIR)/mach/Makefile

$(MACH_GENFILES) : migfiles

migfiles :: $(MACH_DEFSFILES) $(MACH_MKFILE)
	rm -rf ./mach
	mkdir ./mach
	cp -p ${MACH_DEFSFILES} ./mach/
	cp -p $(MACH_MKFILE) ./mach/
	cd ./mach && make && touch ../migfiles


MACH_DEBUG_DEFSFILES=\
	$(SOURCE_DIR)/mach_debug/mach_debug_types.defs \
	$(SOURCE_DIR)/mach_debug/mach_debug.defs

MACH_DEBUG_GENFILES=\
	./mach_debug/mach_debug_server.c

MACH_DEBUG_MKFILE=	$(SOURCE_DIR)/mach_debug/Makefile

$(MACH_DEBUG_GENFILES) : migdebugfiles

migdebugfiles :: $(MACH_DEBUG_DEFSFILES) $(MACH_DEBUG_MKFILE)
	rm -rf ./mach_debug
	mkdir ./mach_debug
	cp -p $(MACH_DEBUG_DEFSFILES) ./mach_debug/
	cp -p $(MACH_DEBUG_MKFILE) ./mach_debug/
	cd ./mach_debug && make && touch ../migdebugfiles

MIG_ALL_FILES =	migfiles \
		migmachinefiles \
		migdebugfiles \
		migdebugmachinefiles

#
#  Update the Makefile when any dependency files exist rather than only
#  when they are newer than the makefile (since it often gets touched
#  during reconfigurations when dependency files have yet to be
#  integrated).  The makefile is copied before doing any potential
#  integration so that the new and old versions can be compared when
#  done (by the SYS_RULE_* rules) to prevent re-building things when no
#  dependencies change.
#
#  N.B.  There is always an empty artificial dependency file M.d
#  (created by config) which will never be integrated and simply
#  prevents the wild-card expansion below from failing.
#
#  The default exit rule now brings the Makefile up to date, so it
#  shouldn't be necessary to execute this rule manually.
#

Makefile: *.${MD_SUFFIX}
	${MD} -f -d `ls *.${MD_SUFFIX}`

.EXIT:
	${EXIT_RULE}

depend: 
	@echo you don\'t make depend any more.



#
#  Permit manually forced checkout of modules when the dependency
#  information is not current.
#

checkout:	${USE}



#
#  Clean up the current build directory.
#

clean:
	@for f in ${LOAD}; do echo rm -f $$f $$f.* && rm -f $$f $$f.*; done
	rm -f eddep tags *.o locore.i \
		$(MIG_ALL_FILES) \
		errs linterrs makelinks *.log *LOG* ${CLEAN_EXTRA}


#
#  Run "lint" on the current build directory.  This will often be done
#  with a special configuration that defines the union of all options
#  used in all configurations so that all combinations are checked.
#  The LINTFILES variable allows lint to be easily run on an optional
#  sub-set of files specified on the make command line when this is
#  desired.
#

LINTFILES=$(SOURCE_DIR)/machine/Locore.c ${CFILES} $(SOURCE_DIR)/machine/swapgeneric.c

lint .ORDER:	$(EXTERNAL_FILES) \
		$(MIG_ALL_FILES) \
		assym.s \
		ALWAYS
	@lint -n -hbxn -DGENERIC ${LINTOPTS} ${LINTFILES} | \
	    egrep -v 'struct/union .* never defined' | \
	    egrep -v 'possible pointer alignment problem'

ALWAYS:

MACHINEDIR=$(SOURCE_DIR)/machine

symbols.sort: ${MACHINEDIR}/symbols.raw
	-grep -v '^#' ${MACHINEDIR}/symbols.raw \
	    | sed 's/^	//' | sort -u > symbols.tmp
	mv -f symbols.tmp symbols.sort

#
#  This macro is replaced by the set of rules which build everything in
#  OBJS.  The extra dependencies for each object file are inserted
#  following this by md which uses the keys on the sentinel line
#  below.
#

cmu_syscalls.o: $(SOURCE_DIR)/bsd/cmu_syscalls.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

init_main.o: $(SOURCE_DIR)/bsd/init_main.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

init_sysent.o: $(SOURCE_DIR)/bsd/init_sysent.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_acct.o: $(SOURCE_DIR)/bsd/kern_acct.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_clock.o: $(SOURCE_DIR)/bsd/kern_clock.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_descrip.o: $(SOURCE_DIR)/bsd/kern_descrip.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_exec.o: $(SOURCE_DIR)/bsd/kern_exec.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_exit.o: $(SOURCE_DIR)/bsd/kern_exit.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_fork.o: $(SOURCE_DIR)/bsd/kern_fork.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_mman.o: $(SOURCE_DIR)/bsd/kern_mman.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_proc.o: $(SOURCE_DIR)/bsd/kern_proc.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_prot.o: $(SOURCE_DIR)/bsd/kern_prot.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_resource.o: $(SOURCE_DIR)/bsd/kern_resource.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_sig.o: $(SOURCE_DIR)/bsd/kern_sig.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_subr.o: $(SOURCE_DIR)/bsd/kern_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_synch.o: $(SOURCE_DIR)/bsd/kern_synch.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_time.o: $(SOURCE_DIR)/bsd/kern_time.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kern_xxx.o: $(SOURCE_DIR)/bsd/kern_xxx.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_process.o: $(SOURCE_DIR)/bsd/mach_process.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_signal.o: $(SOURCE_DIR)/bsd/mach_signal.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

quota_sys.o: $(SOURCE_DIR)/bsd/quota_sys.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

subr_log.o: $(SOURCE_DIR)/bsd/subr_log.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

subr_prf.o: $(SOURCE_DIR)/bsd/subr_prf.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

subr_rmap.o: $(SOURCE_DIR)/bsd/subr_rmap.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

subr_xxx.o: $(SOURCE_DIR)/bsd/subr_xxx.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

sys_generic.o: $(SOURCE_DIR)/bsd/sys_generic.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

sys_socket.o: $(SOURCE_DIR)/bsd/sys_socket.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tty.o: $(SOURCE_DIR)/bsd/tty.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tty_cmupty.o: $(SOURCE_DIR)/bsd/tty_cmupty.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tty_conf.o: $(SOURCE_DIR)/bsd/tty_conf.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tty_pty.o: $(SOURCE_DIR)/bsd/tty_pty.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tty_subr.o: $(SOURCE_DIR)/bsd/tty_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tty_tty.o: $(SOURCE_DIR)/bsd/tty_tty.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_alloc.o: $(SOURCE_DIR)/bsd/ufs_alloc.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_disksubr.o: $(SOURCE_DIR)/bsd/ufs_disksubr.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_inode.o: $(SOURCE_DIR)/bsd/ufs_inode.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_physio.o: $(SOURCE_DIR)/bsd/ufs_physio.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_subr.o: $(SOURCE_DIR)/bsd/ufs_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_tables.o: $(SOURCE_DIR)/bsd/ufs_tables.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_xxx.o: $(SOURCE_DIR)/bsd/ufs_xxx.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_domain.o: $(SOURCE_DIR)/bsd/uipc_domain.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_mbuf.o: $(SOURCE_DIR)/bsd/uipc_mbuf.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_proto.o: $(SOURCE_DIR)/bsd/uipc_proto.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_socket.o: $(SOURCE_DIR)/bsd/uipc_socket.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_socket2.o: $(SOURCE_DIR)/bsd/uipc_socket2.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_syscalls.o: $(SOURCE_DIR)/bsd/uipc_syscalls.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

uipc_usrreq.o: $(SOURCE_DIR)/bsd/uipc_usrreq.c
	${C_RULE_1A}$(SOURCE_DIR)/bsd/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

device_pager.o: $(SOURCE_DIR)/builtin/device_pager.c
	${C_RULE_1A}$(SOURCE_DIR)/builtin/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

inode_pager.o: $(SOURCE_DIR)/builtin/inode_pager.c
	${C_RULE_1A}$(SOURCE_DIR)/builtin/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ux_exception.o: $(SOURCE_DIR)/builtin/ux_exception.c
	${C_RULE_1A}$(SOURCE_DIR)/builtin/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

param.o: $(SOURCE_DIR)/conf/param.c
	${C_RULE_1A}$(SOURCE_DIR)/conf/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ast.o: $(SOURCE_DIR)/kern/ast.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

clock_prim.o: $(SOURCE_DIR)/kern/clock_prim.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

debug.o: $(SOURCE_DIR)/kern/debug.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

exception.o: $(SOURCE_DIR)/kern/exception.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

host.o: $(SOURCE_DIR)/kern/host.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

iprintf.o: $(SOURCE_DIR)/kern/iprintf.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

lock.o: $(SOURCE_DIR)/kern/lock.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_factor.o: $(SOURCE_DIR)/kern/mach_factor.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_basics.o: $(SOURCE_DIR)/kern/ipc_basics.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_copyin.o: $(SOURCE_DIR)/kern/ipc_copyin.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_copyout.o: $(SOURCE_DIR)/kern/ipc_copyout.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_host.o: $(SOURCE_DIR)/kern/ipc_host.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_kport.o: $(SOURCE_DIR)/kern/ipc_kport.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_kset.o: $(SOURCE_DIR)/kern/ipc_kset.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_pobj.o: $(SOURCE_DIR)/kern/ipc_pobj.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_port.o: $(SOURCE_DIR)/kern/ipc_port.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_tt.o: $(SOURCE_DIR)/kern/ipc_tt.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_globals.o: $(SOURCE_DIR)/kern/ipc_globals.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_mports.o: $(SOURCE_DIR)/kern/ipc_mports.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_prims.o: $(SOURCE_DIR)/kern/ipc_prims.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ipc_ptraps.o: $(SOURCE_DIR)/kern/ipc_ptraps.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kalloc.o: $(SOURCE_DIR)/kern/kalloc.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_net.o: $(SOURCE_DIR)/kern/mach_net.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_timedev.o: $(SOURCE_DIR)/kern/mach_timedev.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_user_internal.o: $(SOURCE_DIR)/kern/mach_user_internal.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

machine.o: $(SOURCE_DIR)/kern/machine.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mfs_prim.o: $(SOURCE_DIR)/kern/mfs_prim.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mig_support.o: $(SOURCE_DIR)/kern/mig_support.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

parallel.o: $(SOURCE_DIR)/kern/parallel.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

processor.o: $(SOURCE_DIR)/kern/processor.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

queue.o: $(SOURCE_DIR)/kern/queue.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

sched_prim.o: $(SOURCE_DIR)/kern/sched_prim.c
	${C_RULE_1A_D}${VOLATILE}$(SOURCE_DIR)/kern/${C_RULE_1B_D}; \
	${C_RULE_2_D}; \
	${C_RULE_3_D}; \
	${C_RULE_4_D}

slave.o: $(SOURCE_DIR)/kern/slave.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

syscall_emulation.o: $(SOURCE_DIR)/kern/syscall_emulation.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

syscall_subr.o: $(SOURCE_DIR)/kern/syscall_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

syscall_sw.o: $(SOURCE_DIR)/kern/syscall_sw.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

task.o: $(SOURCE_DIR)/kern/task.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

thread.o: $(SOURCE_DIR)/kern/thread.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

thread_swap.o: $(SOURCE_DIR)/kern/thread_swap.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

timer.o: $(SOURCE_DIR)/kern/timer.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

zalloc.o: $(SOURCE_DIR)/kern/zalloc.c
	${C_RULE_1A}$(SOURCE_DIR)/kern/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

exc_server.o: mach/exc_server.c
	${C_RULE_1A}mach/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

exc_user.o: mach/exc_user.c
	${C_RULE_1A}mach/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_host_server.o: mach/mach_host_server.c
	${C_RULE_1A}mach/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_server.o: mach/mach_server.c
	${C_RULE_1A}mach/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

memory_object_default_user.o: mach/memory_object_default_user.c
	${C_RULE_1A}mach/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

memory_object_user.o: mach/memory_object_user.c
	${C_RULE_1A}mach/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mach_debug_server.o: mach_debug/mach_debug_server.c
	${C_RULE_1A}mach_debug/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

af.o: $(SOURCE_DIR)/net/af.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

dli.o: $(SOURCE_DIR)/net/dli.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

if.o: $(SOURCE_DIR)/net/if.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

if_loop.o: $(SOURCE_DIR)/net/if_loop.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

netisr.o: $(SOURCE_DIR)/net/netisr.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

raw_cb.o: $(SOURCE_DIR)/net/raw_cb.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

raw_usrreq.o: $(SOURCE_DIR)/net/raw_usrreq.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

route.o: $(SOURCE_DIR)/net/route.c
	${C_RULE_1A}$(SOURCE_DIR)/net/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

if_ether.o: $(SOURCE_DIR)/netinet/if_ether.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

igmp.o: $(SOURCE_DIR)/netinet/igmp.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

in.o: $(SOURCE_DIR)/netinet/in.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

in_pcb.o: $(SOURCE_DIR)/netinet/in_pcb.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

in_proto.o: $(SOURCE_DIR)/netinet/in_proto.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ip_icmp.o: $(SOURCE_DIR)/netinet/ip_icmp.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ip_input.o: $(SOURCE_DIR)/netinet/ip_input.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ip_mroute.o: $(SOURCE_DIR)/netinet/ip_mroute.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ip_output.o: $(SOURCE_DIR)/netinet/ip_output.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

raw_ip.o: $(SOURCE_DIR)/netinet/raw_ip.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tcp_debug.o: $(SOURCE_DIR)/netinet/tcp_debug.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tcp_input.o: $(SOURCE_DIR)/netinet/tcp_input.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tcp_output.o: $(SOURCE_DIR)/netinet/tcp_output.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tcp_subr.o: $(SOURCE_DIR)/netinet/tcp_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tcp_timer.o: $(SOURCE_DIR)/netinet/tcp_timer.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

tcp_usrreq.o: $(SOURCE_DIR)/netinet/tcp_usrreq.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

udp_usrreq.o: $(SOURCE_DIR)/netinet/udp_usrreq.c
	${C_RULE_1A}$(SOURCE_DIR)/netinet/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

xdr.o: $(SOURCE_DIR)/rpc/xdr.c
	${C_RULE_1A}$(SOURCE_DIR)/rpc/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

xdr_array.o: $(SOURCE_DIR)/rpc/xdr_array.c
	${C_RULE_1A}$(SOURCE_DIR)/rpc/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

xdr_mem.o: $(SOURCE_DIR)/rpc/xdr_mem.c
	${C_RULE_1A}$(SOURCE_DIR)/rpc/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

bdev_vnodeops.o: $(SOURCE_DIR)/specfs/bdev_vnodeops.c
	${C_RULE_1A}$(SOURCE_DIR)/specfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

fifo_vnodeops.o: $(SOURCE_DIR)/specfs/fifo_vnodeops.c
	${C_RULE_1A}$(SOURCE_DIR)/specfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

spec_subr.o: $(SOURCE_DIR)/specfs/spec_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/specfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

spec_vnodeops.o: $(SOURCE_DIR)/specfs/spec_vnodeops.c
	${C_RULE_1A}$(SOURCE_DIR)/specfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_bmap.o: $(SOURCE_DIR)/vfs/ufs_bmap.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_dir.o: $(SOURCE_DIR)/vfs/ufs_dir.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_vfsops.o: $(SOURCE_DIR)/vfs/ufs_vfsops.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_vnodeops.o: $(SOURCE_DIR)/vfs/ufs_vnodeops.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs.o: $(SOURCE_DIR)/vfs/vfs.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_bio.o: $(SOURCE_DIR)/vfs/vfs_bio.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_conf.o: $(SOURCE_DIR)/vfs/vfs_conf.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_dnlc.o: $(SOURCE_DIR)/vfs/vfs_dnlc.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_io.o: $(SOURCE_DIR)/vfs/vfs_io.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_lookup.o: $(SOURCE_DIR)/vfs/vfs_lookup.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_pathname.o: $(SOURCE_DIR)/vfs/vfs_pathname.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_syscalls.o: $(SOURCE_DIR)/vfs/vfs_syscalls.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_sysnames.o: $(SOURCE_DIR)/vfs/vfs_sysnames.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vfs_vnode.o: $(SOURCE_DIR)/vfs/vfs_vnode.c
	${C_RULE_1A}$(SOURCE_DIR)/vfs/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

memory_object.o: $(SOURCE_DIR)/vm/memory_object.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_fault.o: $(SOURCE_DIR)/vm/vm_fault.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_init.o: $(SOURCE_DIR)/vm/vm_init.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_kern.o: $(SOURCE_DIR)/vm/vm_kern.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_map.o: $(SOURCE_DIR)/vm/vm_map.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_object.o: $(SOURCE_DIR)/vm/vm_object.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_pageout.o: $(SOURCE_DIR)/vm/vm_pageout.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_resident.o: $(SOURCE_DIR)/vm/vm_resident.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_unix.o: $(SOURCE_DIR)/vm/vm_unix.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_user.o: $(SOURCE_DIR)/vm/vm_user.c
	${C_RULE_1A}$(SOURCE_DIR)/vm/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ioconf.o: ioconf.c
	${C_RULE_1A}${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

conf.o: $(SOURCE_DIR)/mac2/conf.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

cons.o: $(SOURCE_DIR)/mac2dev/cons.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

kb.o: $(SOURCE_DIR)/mac2dev/kb.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

fdb.o: $(SOURCE_DIR)/mac2dev/fdb.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

fdbs.o: $(SOURCE_DIR)/mac2dev/fdbs.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2dev/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

key.o: $(SOURCE_DIR)/mac2dev/key.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mouse.o: $(SOURCE_DIR)/mac2dev/mouse.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

font1.o: $(SOURCE_DIR)/mac2dev/font1.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2dev/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

font_524.o: $(SOURCE_DIR)/mac2dev/font_524.b
	${B_RULE_1A}$(SOURCE_DIR)/mac2dev/${B_RULE_1B}

font_521.o: $(SOURCE_DIR)/mac2dev/font_521.b
	${B_RULE_1A}$(SOURCE_DIR)/mac2dev/${B_RULE_1B}

video.o: $(SOURCE_DIR)/mac2dev/video.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

drvr_120.o: $(SOURCE_DIR)/mac2dev/drvr_120.b
	${B_RULE_1A}$(SOURCE_DIR)/mac2dev/${B_RULE_1B}

via.o: $(SOURCE_DIR)/mac2dev/via.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

scc.o: $(SOURCE_DIR)/mac2dev/scc.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mscsi.o: $(SOURCE_DIR)/mac2dev/mscsi.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mhd.o: $(SOURCE_DIR)/mac2dev/mhd.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2dev/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

if_en.o: $(SOURCE_DIR)/mac2if/if_en.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2if/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

if_enintr_catch.o: $(SOURCE_DIR)/mac2if/if_enintr_catch.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2if/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

mactrap.o: $(SOURCE_DIR)/mac2emul/mactrap.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2emul/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

mackern.o: $(SOURCE_DIR)/mac2emul/mackern.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macserver_main.o: $(SOURCE_DIR)/mac2emul/macserver_main.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macserver_routines.o: $(SOURCE_DIR)/mac2emul/macserver_routines.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macserver_phys_pages.o: $(SOURCE_DIR)/mac2emul/macserver_phys_pages.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macServer.o: $(SOURCE_DIR)/mac2emul/macServer.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macmemory.o: $(SOURCE_DIR)/mac2emul/macmemory.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macostrap.o: $(SOURCE_DIR)/mac2emul/macostrap.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macostrap_tables.o: $(SOURCE_DIR)/mac2emul/macostrap_tables.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macexception.o: $(SOURCE_DIR)/mac2emul/macexception.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macintr.o: $(SOURCE_DIR)/mac2emul/macintr.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macuser.o: $(SOURCE_DIR)/mac2emul/macuser.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2emul/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

macinst.o: $(SOURCE_DIR)/mac2emul/macinst.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macinst_subr.o: $(SOURCE_DIR)/mac2emul/macinst_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macbus_cycle.o: $(SOURCE_DIR)/mac2emul/macbus_cycle.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macvia.o: $(SOURCE_DIR)/mac2emul/macvia.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macdevice.o: $(SOURCE_DIR)/mac2emul/macdevice.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macpmap.o: $(SOURCE_DIR)/mac2emul/macpmap.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macscsi_mgr.o: $(SOURCE_DIR)/mac2emul/macscsi_mgr.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macinput.o: $(SOURCE_DIR)/mac2emul/macinput.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macscc.o: $(SOURCE_DIR)/mac2emul/macscc.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macether_routines.o: $(SOURCE_DIR)/mac2emul/macether_routines.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macether_interface.o: $(SOURCE_DIR)/mac2emul/macether_interface.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

macether_hash.o: $(SOURCE_DIR)/mac2emul/macether_hash.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2emul/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_principal.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_principal.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_special.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_special.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_advanced.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_advanced.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_subr.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_subr.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_SExecCode.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_SExecCode.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

slotmgr_alloc.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_alloc.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_dispatch.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_dispatch.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_extra.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_extra.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_interrupts.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_interrupts.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

slotmgr_SCallIntr.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_SCallIntr.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

slotmgr_CallDriver.o: $(SOURCE_DIR)/mac2slotmgr/slotmgr_CallDriver.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2slotmgr/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

ostrap.o: $(SOURCE_DIR)/mac2/ostrap.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ostrap_tables.o: $(SOURCE_DIR)/mac2/ostrap_tables.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mem.o: $(SOURCE_DIR)/mac2/mem.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

virt.o: $(SOURCE_DIR)/mac2/virt.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

setjmp.o: $(SOURCE_DIR)/mac2/setjmp.s
	${S_RULE_1A}$(SOURCE_DIR)/mac2/${S_RULE_1B}; \
	${S_RULE_2}; \
	${S_RULE_3}

ivect.o: $(SOURCE_DIR)/mac2/ivect.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mac2_mach_process.o: $(SOURCE_DIR)/mac2/mac2_mach_process.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

ufs_machdep.o: $(SOURCE_DIR)/mac2/ufs_machdep.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

autoconf.o: $(SOURCE_DIR)/mac2/autoconf.c
	${C_RULE_1A_D}$(SOURCE_DIR)/mac2/${C_RULE_1B_D}; \
	${C_RULE_2_D}; \
	${C_RULE_3_D}; \
	${C_RULE_4_D}

clock.o: $(SOURCE_DIR)/mac2/clock.c
	${C_RULE_1A_D}$(SOURCE_DIR)/mac2/${C_RULE_1B_D}; \
	${C_RULE_2_D}; \
	${C_RULE_3_D}; \
	${C_RULE_4_D}

machdep.o: $(SOURCE_DIR)/mac2/machdep.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

pcb.o: $(SOURCE_DIR)/mac2/pcb.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

pmap.o: $(SOURCE_DIR)/mac2/pmap.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

act.o: $(SOURCE_DIR)/mac2/act.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

softint.o: $(SOURCE_DIR)/mac2/softint.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mac2_init.o: $(SOURCE_DIR)/mac2/mac2_init.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

mac2_exception.o: $(SOURCE_DIR)/mac2/mac2_exception.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

trap.o: $(SOURCE_DIR)/mac2/trap.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

in_cksum.o: $(SOURCE_DIR)/mac2/in_cksum.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

vm_machdep.o: $(SOURCE_DIR)/mac2/vm_machdep.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}

strings.o: $(SOURCE_DIR)/mac2/strings.c
	${C_RULE_1A}$(SOURCE_DIR)/mac2/${C_RULE_1B}; \
	${C_RULE_2}; \
	${C_RULE_3}; \
	${C_RULE_4}



# DO NOT DELETE THIS LINE -- "make Makefile" uses it

# Dependencies for File: act.o:
act.o: ../mac2/act.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2/act.h \
  ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../kern/macro_help.h 
# Dependencies for File: af.o:
af.o: ../net/af.c ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h \
  ./vice.h ../sys/socketvar.h ../net/af.h 
# Dependencies for File: ast.o:
ast.o: ../kern/ast.c ./cpus.h ./hw_ast.h ./mach_fixpri.h ../machine/cpu.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/queue.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/param.h ../machine/machparam.h ../sys/signal.h \
  ../sys/proc.h ../sys/time.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h ../sys/errno.h \
  ./mach_nfs.h ../sys/systm.h 
# Dependencies for File: autoconf.o:
autoconf.o: ../mac2/autoconf.c ../sys/types.h ../mach/machine/vm_types.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../mac2/autoconf.h ../mac2/cpu.h \
  ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h ../mac2slotmgr/ROMDefs.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/slotmgr.h ../kern/queue.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/macro_help.h 
# Dependencies for File: bdev_vnodeops.o:
bdev_vnodeops.o: ../specfs/bdev_vnodeops.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/conf.h ../sys/buf.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/stat.h ../kern/mfs.h ./mach_afs.h ../kern/kalloc.h 
# Dependencies for File: clock.o:
clock.o: ../mac2/clock.c ./cputypes.h ./macos_emul.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/time.h ../sys/kernel.h ../mach/mac2/frame.h ../mac2/clock.h \
  ../mac2/act.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/queue.h \
  ../kern/macro_help.h ../mac2dev/via.h 
# Dependencies for File: clock_prim.o:
clock_prim.o: ../kern/clock_prim.c ./cpus.h ./mach_fixpri.h \
  ../machine/cpu.h ../sys/types.h ../mach/machine/vm_types.h \
  ../mach/machine.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../kern/processor.h ../kern/lock.h ./mach_ldebug.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/thread.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h \
  ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../machine/machparam.h 
# Dependencies for File: cmu_syscalls.o:
cmu_syscalls.o: ../bsd/cmu_syscalls.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/buf.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/file.h ../sys/proc.h ../sys/conf.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/tty.h ../sys/mount.h ../sys/fs.h ../sys/table.h ../vm/vm_user.h \
  ../vm/vm_kern.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../sys/vmmac.h ../machine/vmparam.h ../sys/version.h 
# Dependencies for File: conf.o:
conf.o: ../mac2/conf.c ./cputypes.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/buf.h \
  ../kern/macro_help.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/time.h ../sys/tty.h ../sys/conf.h \
  ./ramdisk.h ./pty.h 
# Dependencies for File: cons.o:
cons.o: ../mac2dev/cons.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/conf.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../sys/dir.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/tty.h ../mac2/act.h \
  ../mac2dev/font.h ../mac2dev/key.h ../mac2slotmgr/Types.h \
  ../mac2slotmgr/Devices.h ../mac2slotmgr/Video.h ../mac2dev/video.h 
# Dependencies for File: debug.o:
debug.o: ../kern/debug.c ./mach_kdb.h 
# Dependencies for File: device_pager.o:
device_pager.o: ../builtin/device_pager.c ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/machine/vm_types.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../sys/types.h ../machine/psl.h ../mac2/endian.h ../vm/vm_page.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/vm_prot.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/queue.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../kern/macro_help.h ../kern/assert.h ./mach_assert.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h ../vm/vm_kern.h \
  ../kern/kern_port.h ../kern/port_object.h ./mach_net.h ../kern/ipc_copyin.h \
  ../kern/ipc_copyout.h ./mach/mach_user_internal.h ../kern/server_loop.c \
  ../mach/notify.h ./mach/memory_object_server.c ../mach/mig_errors.h \
  ../mach/std_types.h ../mach/mach_types.h ../mach/vm_attributes.h \
  ../mach/machine.h ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/thread_special_ports.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../sys/time.h \
  ../mach/kernel_event.h ../vm/vm_user.h ../kern/host.h 
# Dependencies for File: dli.o:
dli.o: ../net/dli.c ./dli.h ./mach_device.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/mbuf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/socket.h ./mach_vmtp.h ./vice.h ../sys/protosw.h ../sys/socketvar.h \
  ../sys/errno.h ./mach_nfs.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/time.h ../net/if.h \
  ../net/if_arp.h ../net/route.h ../net/raw_cb.h ../net/dli_var.h 
# Dependencies for File: exc_server.o:
exc_server.o: mach/exc_server.c ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/message.h ./mach_ipc_xxxhack.h ../mach/port.h ../mach/mig_errors.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/std_types.h \
  ../mach/machine/vm_types.h 
# Dependencies for File: exc_user.o:
exc_user.o: mach/exc_user.c mach/exc.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../mach/std_types.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../mach/mach_types.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine.h ./cpus.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../sys/types.h ../vm/vm_map.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/mig_errors.h ../mach/msg_type.h 
# Dependencies for File: exception.o:
exception.o: ../kern/exception.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/message.h ./mach_ipc_xxxhack.h \
  ../mach/port.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../kern/macro_help.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h \
  ./mach_assert.h ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h ../sys/errno.h \
  ./mach_nfs.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ./mach/exc.h ../mach/std_types.h ../kern/ipc_copyout.h ../kern/parallel.h 
# Dependencies for File: fdb.o:
fdb.o: ../mac2dev/fdb.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2dev/fdb.h \
  ../mac2dev/via.h 
# Dependencies for File: fdbs.o:
fdbs.o: ../mac2dev/fdbs.s ./assym.s ../mac2/psl.h 
# Dependencies for File: fifo_vnodeops.o:
fifo_vnodeops.o: ../specfs/fifo_vnodeops.c 
# Dependencies for File: font1.o:
font1.o: ../mac2dev/font1.s 
# Dependencies for File: genassym.o:
genassym.o: ../machine/genassym.c ./macos_emul.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/buf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/vmparam.h ../machine/vmparam.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../sys/dir.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/cmap.h ../sys/map.h ../sys/proc.h ../sys/mbuf.h \
  ../sys/msgbuf.h ../sys/vmmac.h ../mac2dev/via.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/socket.h ./mach_vmtp.h ../net/if.h \
  ../net/if_arp.h ../mac2emul/macserver_types.h ../mac2emul/macinst.h 
# Dependencies for File: host.o:
host.o: ../kern/host.c ./cpus.h ./mach_host.h ../kern/host.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/host_info.h ../mach/machine.h \
  ../mach/machine/vm_types.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/lock.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../sys/types.h ../machine/psl.h ../mac2/endian.h ../vm/vm_kern.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/assert.h ./mach_assert.h ../vm/vm_user.h 
# Dependencies for File: if.o:
if.o: ../net/if.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/mbuf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/systm.h ../sys/socket.h ./mach_vmtp.h ./vice.h ../sys/socketvar.h \
  ../sys/protosw.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../net/if.h ../net/if_arp.h \
  ../net/af.h ./ether.h 
# Dependencies for File: if_en.o:
if_en.o: ../mac2if/if_en.c ./macos_emul.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/kernel.h \
  ../sys/time.h ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/socket.h ./mach_vmtp.h ./vice.h \
  ../sys/socketvar.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/errno.h ./mach_nfs.h ../net/if.h \
  ../net/if_arp.h ../net/route.h ../net/netisr.h ../netinet/in.h \
  ../machine/endian.h ../netinet/in_systm.h ../netinet/in_var.h \
  ../netinet/ip.h ../netinet/ip_var.h ../netinet/if_ether.h ./dli.h \
  ../net/dli_var.h ../mac2if/if_enregs.h ../mac2if/if_en.h ../mac2/clock.h \
  ../mac2/act.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: if_enintr_catch.o:
if_enintr_catch.o: ../mac2if/if_enintr_catch.s 
# Dependencies for File: if_ether.o:
if_ether.o: ../netinet/if_ether.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h ../sys/time.h \
  ../sys/kernel.h ../sys/errno.h ./mach_nfs.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/syslog.h \
  ../net/if.h ../net/if_arp.h ../netinet/in.h \
  ../machine/endian.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/if_ether.h ./multicast.h ./sun_rpc.h ../netinet/if_lan.h 
# Dependencies for File: if_loop.o:
if_loop.o: ../net/if_loop.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h ../sys/errno.h \
  ./mach_nfs.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/time.h ../net/if.h \
  ../net/if_arp.h ../net/netisr.h ../net/route.h \
  ../netinet/in.h ../machine/endian.h ../netinet/in_systm.h \
  ../netinet/in_var.h ../netinet/ip.h 
# Dependencies for File: igmp.o:
igmp.o: ../netinet/igmp.c ./multicast.h 
# Dependencies for File: in.o:
in.o: ../netinet/in.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/time.h ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h \
  ./vice.h ../sys/socketvar.h ../sys/uio.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ../sys/resource.h ../sys/namei.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../netinet/in_systm.h ../net/if.h \
  ../net/if_arp.h ../net/route.h \
  ../net/af.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_var.h 
# Dependencies for File: in_cksum.o:
in_cksum.o: ../mac2/in_cksum.c ../sys/types.h ../mach/machine/vm_types.h \
  ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_systm.h 
# Dependencies for File: in_pcb.o:
in_pcb.o: ../netinet/in_pcb.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/user.h ./mach_rfs.h \
  ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../sys/dir.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/mbuf.h ../sys/socket.h ./mach_vmtp.h \
  ../sys/socketvar.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_systm.h ../net/if.h \
  ../net/if_arp.h ../net/route.h \
  ../netinet/in_pcb.h ../netinet/in_var.h ../sys/protosw.h 
# Dependencies for File: in_proto.o:
in_proto.o: ../netinet/in_proto.c ./multicast.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/socket.h ./mach_vmtp.h ./vice.h ../sys/protosw.h ../sys/domain.h \
  ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_systm.h ./dli.h ./imp.h 
# Dependencies for File: init_main.o:
init_main.o: ../bsd/init_main.c ./mach_rfs.h ./mach_device.h ./mach_host.h \
  ./mach_km.h ./mach_net.h ./quota.h ./vice.h ./xpr_debug.h ./cpus.h \
  ./mach_emulation.h ./mach_nfs.h ./macos_emul.h ./cputypes.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./mach_ldebug.h ../sys/errno.h ../kern/thread.h ./mach_fixpri.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/fs.h ../sys/mount.h ../sys/map.h \
  ../sys/proc.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/conf.h ../sys/buf.h ../sys/vm.h ../sys/vmparam.h \
  ../machine/vmparam.h ../sys/vmmac.h ../sys/clist.h ../sys/protosw.h \
  ./mach_vmtp.h ../sys/quota.h ../sys/dk.h ../sys/table.h ../machine/reg.h \
  ../mach/machine.h ../sys/version.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_page.h ../kern/sched_prim.h \
  ../vm/vm_kern.h ../kern/thread_swap.h ../kern/kern_port.h \
  ../kern/port_object.h ../kern/ipc_copyout.h ../builtin/inode_pager.h \
  ./mach_xp.h ../builtin/device_pager.h ../mach/task_special_ports.h \
  ../builtin/ux_exception.h ./loop.h ./sl.h 
# Dependencies for File: init_sysent.o:
init_sysent.o: ../bsd/init_sysent.c ./cputypes.h ./mach_vmtp.h ./vice.h \
  ./mach_afs.h ./mach_nfs.h ./mach_vfs.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h 
# Dependencies for File: inode_pager.o:
inode_pager.o: ../builtin/inode_pager.c ./mach_nbc.h ./mach_vfs.h \
  ./mach_assert.h ../mach/std_types.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/machine/vm_types.h ../mach/memory_object.h ../mach/message.h \
  ../mach/notify.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../mac2/machparam.h ../mach/mac2/vm_param.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../sys/types.h ../machine/psl.h ../mac2/endian.h \
  ../mach/vm_prot.h ../vm/vm_map.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ./cpus.h ../kern/lock.h ./mach_ldebug.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ../vm/vm_kern.h ../kern/parallel.h ../kern/zalloc.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../kern/kern_port.h ../kern/port_object.h ./mach_km.h ./mach_net.h \
  ../kern/mfs.h ./mach_afs.h ../kern/xpr.h ./xpr_debug.h ../machine/xpr.h \
  ../kern/thread.h ./mach_host.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/host.h ../sys/buf.h ../sys/errno.h ./mach_nfs.h \
  ../sys/inode.h ./mach_rfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../sys/time.h ../vfs/vfs.h ../vfs/pathname.h ../sys/uio.h \
  ../sys/user.h ./vice.h ../sys/param.h ../machine/machparam.h \
  ../sys/signal.h ../sys/resource.h ../sys/namei.h ../sys/dir.h \
  ../builtin/inode_pager.h ./mach_xp.h ../vm/vm_page.h ../kern/sched_prim.h \
  ./mach/mach_user_internal.h ../kern/server_loop.c \
  ./mach/memory_object_server.c ../mach/mig_errors.h ../mach/mach_types.h \
  ../mach/vm_attributes.h ../mach/machine.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/host_info.h ../mach/processor_info.h \
  ../kern/kern_mon.h ../mach/kernel_event.h ../vm/vm_user.h \
  ./mach/memory_object_default_server.c 
# Dependencies for File: ioconf.o:
ioconf.o: ioconf.c ../sys/types.h ../mach/machine/vm_types.h \
  ../mac2/autoconf.h 
# Dependencies for File: ip_icmp.o:
ip_icmp.o: ../netinet/ip_icmp.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h \
  ./vice.h ../sys/time.h ../sys/kernel.h ../net/route.h \
  ../net/if.h ../net/if_arp.h ../netinet/in.h \
  ../machine/endian.h ../netinet/in_systm.h ../netinet/in_var.h \
  ../netinet/ip.h ../netinet/ip_icmp.h ../netinet/icmp_var.h 
# Dependencies for File: ip_input.o:
ip_input.o: ../netinet/ip_input.c ./mach_net.h ./multicast.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/domain.h ../sys/protosw.h ./mach_vmtp.h \
  ../sys/socket.h ./vice.h ../sys/errno.h ./mach_nfs.h ../sys/time.h \
  ../sys/kernel.h ../net/if.h ../net/if_arp.h \
  ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/in_var.h \
  ../netinet/ip.h ../netinet/ip_var.h ../netinet/ip_icmp.h ../netinet/tcp.h 
# Dependencies for File: ip_mroute.o:
ip_mroute.o: ../netinet/ip_mroute.c ./mrouting.h ./multicast.h 
# Dependencies for File: ip_output.o:
ip_output.o: ../netinet/ip_output.c ./multicast.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/errno.h ./mach_nfs.h ../sys/protosw.h \
  ./mach_vmtp.h ../sys/socket.h ./vice.h ../sys/socketvar.h \
  ../net/if.h ../net/if_arp.h \
  ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/in_var.h \
  ../netinet/ip.h ../netinet/ip_var.h 
# Dependencies for File: ipc_basics.o:
ipc_basics.o: ../kern/ipc_basics.c ./mach_fixpri.h ./mach_km.h ./mach_net.h \
  ./mach_np.h ./mach_ipc_xxxhack.h ./mach_ipc_tcache.h ./mach_ipc_wwa.h \
  ./mach_debug.h ../kern/assert.h ./mach_assert.h ../kern/macro_help.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/queue.h \
  ../mach/machine/vm_types.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/zalloc.h ../vm/vm_map.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/vm_prot.h ../mach/vm_inherit.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_kern.h ../kern/task.h \
  ./mach_emulation.h ../mach/time_value.h ../kern/mach_param.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../mach/notify.h \
  ../kern/kern_port.h ../kern/port_object.h ../kern/sched_prim_macros.h \
  ../kern/sched_prim.h ../kern/thread_swap.h ../machine/machparam.h \
  ../mach_debug/ipc_statistics.h ./mach_ipc_stats.h ../kern/ipc_hash.h \
  ../kern/ipc_cache.h ../kern/ipc_copyin.h ../kern/ipc_signal.h \
  ./mach_ipc_sighack.h ../sys/signal.h ../sys/proc.h ../sys/time.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/param.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../sys/dir.h ../sys/errno.h ./mach_nfs.h \
  ../kern/ipc_prims.h ../kern/ipc_globals.h ../kern/ipc_kmesg.h \
  ../kern/ipc_copyout.h ../kern/ipc_basics.h 
# Dependencies for File: ipc_copyin.o:
ipc_copyin.o: ../kern/ipc_copyin.c ./mach_np.h ./mach_ipc_stats.h \
  ./mach_ipc_xxxhack.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/types.h ../mach/machine/vm_types.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/port.h ../mach/time_value.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h \
  ../kern/mach_param.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/processor.h ./mach_fixpri.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/kern_port.h \
  ../kern/port_object.h ./mach_net.h ../kern/ipc_kmesg.h \
  ../mach_debug/ipc_statistics.h ../kern/ipc_globals.h ../mach/notify.h \
  ../kern/ipc_hash.h ../kern/ipc_copyout.h ../kern/ipc_prims.h \
  ../kern/ipc_cache.h ../kern/ipc_copyin.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h 
# Dependencies for File: ipc_copyout.o:
ipc_copyout.o: ../kern/ipc_copyout.c ./mach_np.h ./mach_ipc_xxxhack.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/types.h \
  ../mach/machine/vm_types.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/port.h ../mach/time_value.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h \
  ../kern/mach_param.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/processor.h ./mach_fixpri.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/kern_port.h \
  ../kern/port_object.h ./mach_net.h ../kern/ipc_kmesg.h \
  ../mach_debug/ipc_statistics.h ./mach_ipc_stats.h ../kern/ipc_globals.h \
  ../mach/notify.h ../kern/ipc_hash.h ../kern/ipc_copyout.h \
  ../kern/ipc_prims.h ../kern/ipc_cache.h ../kern/ipc_copyin.h 
# Dependencies for File: ipc_globals.o:
ipc_globals.o: ../kern/ipc_globals.c ./mach_ipc_stats.h \
  ./mach_ipc_xxxhack.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/port.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../mach/machine/vm_types.h ../kern/macro_help.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/processor.h ./mach_fixpri.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/notify.h ../mach/message.h ../kern/kern_port.h \
  ../kern/port_object.h ./mach_km.h ./mach_net.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_kern.h ../kern/ipc_hash.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../kern/ipc_copyin.h ../kern/ipc_copyout.h ../kern/ipc_globals.h 
# Dependencies for File: ipc_host.o:
ipc_host.o: ../kern/ipc_host.c ../kern/host.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/kern_port.h ./mach_np.h ../kern/kern_obj.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../kern/zalloc.h ../mach/machine/vm_types.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/assert.h ./mach_assert.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ../mach/mach_param.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/processor.h ./mach_fixpri.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/port_object.h ./mach_km.h ./mach_net.h ../mach/message.h \
  ../kern/thread.h ./mach_host.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/ipc_host.h ../machine/machparam.h 
# Dependencies for File: ipc_kport.o:
ipc_kport.o: ../kern/ipc_kport.c ./mach_ipc_xxxhack.h ./mach_net.h \
  ./mach_np.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/zalloc.h ../mach/machine/vm_types.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../mach/boolean.h ../mach/machine/boolean.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/port.h ../mach/time_value.h ../kern/mach_param.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/kern_port.h ../kern/port_object.h ../mach/notify.h \
  ../kern/sched_prim_macros.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../machine/machparam.h ../kern/ipc_hash.h ../kern/ipc_prims.h \
  ../kern/ipc_globals.h ../kern/ipc_kmesg.h ../mach_debug/ipc_statistics.h \
  ./mach_ipc_stats.h ../kern/ipc_copyout.h 
# Dependencies for File: ipc_kset.o:
ipc_kset.o: ../kern/ipc_kset.c ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/zalloc.h ../mach/machine/vm_types.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../kern/macro_help.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_xxxhack.h ./mach_ipc_tcache.h \
  ../mach/port.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../kern/sched_prim_macros.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../machine/machparam.h ../kern/ipc_hash.h ../kern/ipc_prims.h \
  ../kern/ipc_globals.h ../mach/notify.h ../kern/ipc_signal.h \
  ./mach_ipc_sighack.h ../sys/signal.h ../sys/proc.h ../sys/time.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/param.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../sys/dir.h ../sys/errno.h ./mach_nfs.h 
# Dependencies for File: ipc_mports.o:
ipc_mports.o: ../kern/ipc_mports.c ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/time_value.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../kern/queue.h ../mach/machine/vm_types.h \
  ../kern/macro_help.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/kern_port.h \
  ../kern/port_object.h ./mach_km.h ./mach_net.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../kern/ipc_globals.h ../mach/notify.h \
  ../mach/message.h ../kern/ipc_hash.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../kern/ipc_mports.h 
# Dependencies for File: ipc_pobj.o:
ipc_pobj.o: ../kern/ipc_pobj.c ./mach_km.h ../kern/host.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/kern_port.h ./mach_np.h ../kern/kern_obj.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../kern/zalloc.h ../mach/machine/vm_types.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/assert.h ./mach_assert.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ../mach/mach_param.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/processor.h ./mach_fixpri.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/port_object.h ./mach_net.h ../kern/thread.h ./mach_host.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../kern/ipc_pobj.h \
  ../machine/machparam.h 
# Dependencies for File: ipc_port.o:
ipc_port.o: ../kern/ipc_port.c ./mach_ipc_xxxhack.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/port.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../mach/machine/vm_types.h ../kern/macro_help.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../sys/types.h ../machine/psl.h ../mac2/endian.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../kern/sched_prim_macros.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../machine/machparam.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../kern/ipc_cache.h ../mach_debug/ipc_statistics.h ./mach_ipc_stats.h \
  ../kern/ipc_copyin.h ../kern/ipc_hash.h ../kern/ipc_globals.h \
  ../mach/notify.h ../kern/ipc_prims.h ../vm/vm_kern.h 
# Dependencies for File: ipc_prims.o:
ipc_prims.o: ../kern/ipc_prims.c ./mach_ipc_xxxhack.h ../kern/assert.h \
  ./mach_assert.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../mach/machine/vm_types.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/zalloc.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/port.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../sys/types.h ../machine/psl.h ../mac2/endian.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../kern/sched_prim_macros.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../machine/machparam.h ../mach/notify.h ../mach_debug/ipc_statistics.h \
  ./mach_ipc_stats.h ../kern/ipc_signal.h ./mach_ipc_sighack.h \
  ../sys/signal.h ../sys/proc.h ../sys/time.h ../sys/user.h ./mach_rfs.h \
  ./vice.h ../sys/param.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../sys/dir.h ../sys/errno.h ./mach_nfs.h ../kern/ipc_hash.h \
  ../kern/ipc_cache.h ../kern/ipc_copyin.h ../kern/ipc_kmesg.h \
  ../kern/ipc_globals.h ../kern/ipc_copyout.h ../kern/ipc_prims.h 
# Dependencies for File: ipc_ptraps.o:
ipc_ptraps.o: ../kern/ipc_ptraps.c ../kern/host.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/time_value.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../mach/machine/vm_types.h ../kern/macro_help.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h \
  ../kern/assert.h ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/kern_port.h \
  ../kern/port_object.h ./mach_net.h ../kern/ipc_copyout.h \
  ../kern/ipc_ptraps.h 
# Dependencies for File: ipc_tt.o:
ipc_tt.o: ../kern/ipc_tt.c ./mach_km.h ./mach_ipc_tcache.h \
  ./mach_ipc_xxxhack.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/port.h ../kern/queue.h ../mach/machine/vm_types.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../kern/macro_help.h ../kern/task.h \
  ./mach_emulation.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h \
  ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../kern/ipc_copyout.h ../kern/ipc_hash.h ../kern/ipc_cache.h \
  ../mach_debug/ipc_statistics.h ./mach_ipc_stats.h ../kern/ipc_copyin.h \
  ../kern/ipc_mports.h ../kern/ipc_globals.h ../mach/notify.h \
  ../kern/ipc_tt.h ../sys/user.h ./mach_rfs.h ./vice.h ../sys/param.h \
  ../machine/machparam.h ../sys/signal.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../sys/dir.h ../sys/errno.h ./mach_nfs.h 
# Dependencies for File: iprintf.o:
iprintf.o: ../kern/iprintf.c ./mach_kdb.h 
# Dependencies for File: ivect.o:
ivect.o: ../mac2/ivect.c ./macos_emul.h ../machine/cpu.h ../sys/types.h \
  ../mach/machine/vm_types.h 
# Dependencies for File: kalloc.o:
kalloc.o: ../kern/kalloc.c ../sys/types.h ../mach/machine/vm_types.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../kern/zalloc.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../mach/boolean.h ../mach/machine/boolean.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/kalloc.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../kern/assert.h ./mach_assert.h 
# Dependencies for File: kb.o:
kb.o: ../mac2dev/kb.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/conf.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../sys/dir.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/tty.h ../sys/proc.h \
  ../sys/file.h ../mac2/act.h ../mac2dev/key.h ../mac2dev/mouse.h 
# Dependencies for File: kern_acct.o:
kern_acct.o: ../bsd/kern_acct.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/proc.h \
  ../sys/time.h ../sys/user.h ./mach_rfs.h ./vice.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/fs.h ../sys/kernel.h ../sys/acct.h 
# Dependencies for File: kern_clock.o:
kern_clock.o: ../bsd/kern_clock.c ./simple_clock.h ./stat_time.h \
  ./mach_co_stats.h ./cpus.h ../machine/reg.h ../mach/mac2/reg.h \
  ../machine/psl.h ../kern/assert.h ./mach_assert.h ../kern/macro_help.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h \
  ../machine/cpu.h ../sys/types.h ../mach/machine/vm_types.h ../machine/ast.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/queue.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/processor.h ../kern/sched.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../mac2/endian.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../sys/param.h ../machine/machparam.h \
  ../sys/signal.h ../sys/systm.h ../sys/dk.h ../sys/callout.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../sys/errno.h ./mach_nfs.h ../sys/kernel.h \
  ../sys/proc.h ../sys/table.h ../mach/machine.h 
# Dependencies for File: kern_descrip.o:
kern_descrip.o: ../bsd/kern_descrip.c ./mach_rfs.h ./vice.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/proc.h ../sys/file.h ../sys/socket.h ./mach_vmtp.h \
  ../sys/socketvar.h ../sys/mount.h ../sys/stat.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h 
# Dependencies for File: kern_exec.o:
kern_exec.o: ../bsd/kern_exec.c ./cputypes.h ./mach_rfs.h ./vice.h \
  ./bsd_a_out.h ./sysv_coff.h ../machine/reg.h ../mach/mac2/reg.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/map.h ../vm/vm_map.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/vm_prot.h ../mach/vm_inherit.h \
  ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h ./mach_ldebug.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/assert.h ./mach_assert.h ../sys/dir.h \
  ../sys/user.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/kernel.h ../sys/proc.h \
  ../sys/buf.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/vm.h ../sys/vmparam.h ../machine/vmparam.h ../sys/vmmac.h \
  ../sys/file.h ../sys/acct.h ../sys/exec.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../builtin/inode_pager.h ./mach_xp.h \
  ../vm/vm_kern.h ../vm/vm_user.h ../kern/parallel.h ../sys/reboot.h \
  ./mach_kdb.h 
# Dependencies for File: kern_exit.o:
kern_exit.o: ../bsd/kern_exit.c ./cputypes.h ./mach_rfs.h ./quota.h \
  ./vice.h ./ild.h ../machine/reg.h ../mach/mac2/reg.h ../machine/psl.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/map.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../sys/dir.h ../sys/user.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/proc.h ../sys/buf.h \
  ../sys/wait.h ../sys/vm.h ../sys/vmparam.h ../machine/vmparam.h \
  ../sys/vmmac.h ../sys/file.h ../sys/mbuf.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/syslog.h ../kern/parallel.h ../kern/sched_prim.h 
# Dependencies for File: kern_fork.o:
kern_fork.o: ../bsd/kern_fork.c ./cputypes.h ./mach_rfs.h ./quota.h \
  ./vice.h ../machine/reg.h ../mach/mac2/reg.h ../machine/psl.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/map.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../sys/dir.h ../sys/user.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/proc.h ../sys/inode.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../vfs/vfs.h ../vfs/pathname.h ../sys/vm.h ../sys/vmparam.h \
  ../machine/vmparam.h ../sys/vmmac.h ../sys/file.h ../sys/acct.h \
  ../sys/quota.h 
# Dependencies for File: kern_mman.o:
kern_mman.o: ../bsd/kern_mman.c ../machine/reg.h ../mach/mac2/reg.h \
  ../machine/psl.h ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/map.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/proc.h ../sys/buf.h \
  ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h ../sys/acct.h ../sys/wait.h \
  ../sys/vm.h ../sys/vmparam.h ../machine/vmparam.h ../sys/vmmac.h \
  ../sys/file.h ../sys/vadvise.h ../sys/trace.h ../sys/mman.h ../sys/conf.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../builtin/inode_pager.h \
  ./mach_xp.h ../builtin/device_pager.h 
# Dependencies for File: kern_proc.o:
kern_proc.o: ../bsd/kern_proc.c ../machine/reg.h ../mach/mac2/reg.h \
  ../machine/psl.h ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/map.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/kernel.h ../sys/proc.h \
  ../sys/buf.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/acct.h ../sys/wait.h ../sys/file.h ../sys/quota.h ./quota.h \
  ../sys/mbuf.h 
# Dependencies for File: kern_prot.o:
kern_prot.o: ../bsd/kern_prot.c ./quota.h ./mach_assert.h ../machine/reg.h \
  ../mach/mac2/reg.h ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/acct.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/proc.h ../sys/timeb.h ../sys/times.h ../sys/reboot.h ./mach_kdb.h \
  ../sys/fs.h ../sys/buf.h ../sys/mount.h ../sys/quota.h 
# Dependencies for File: kern_resource.o:
kern_resource.o: ../bsd/kern_resource.c ./mach_host.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/proc.h ../sys/fs.h ../sys/kernel.h ./mach/mach_interface.h \
  ../mach/std_types.h ../mach/mach_types.h ../mach/vm_attributes.h \
  ../mach/machine.h ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/thread_special_ports.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../vm/vm_kern.h \
  ../mach/kernel_event.h ../vm/vm_user.h ../kern/host.h ../mach/mach_extra.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h 
# Dependencies for File: kern_sig.o:
kern_sig.o: ../bsd/kern_sig.c ./cputypes.h ./mach_host.h ./vice.h \
  ../machine/reg.h ../mach/mac2/reg.h ../machine/psl.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_km.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/timeb.h ../sys/times.h \
  ../sys/buf.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/mount.h ../machine/vmparam.h ../sys/acct.h ../sys/kernel.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../kern/parallel.h \
  ../kern/sched_prim.h ../vm/vm_kern.h 
# Dependencies for File: kern_subr.o:
kern_subr.o: ../bsd/kern_subr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h 
# Dependencies for File: kern_synch.o:
kern_synch.o: ../bsd/kern_synch.c ./cputypes.h ./cpus.h ./mach_km.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./mach_ldebug.h \
  ../sys/errno.h ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/proc.h ../sys/file.h \
  ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h ../sys/vm.h ../sys/vmparam.h \
  ../machine/vmparam.h ../sys/vmmac.h ../sys/kernel.h ../sys/buf.h \
  ../sys/table.h ../sys/callout.h ../kern/sched_prim.h ../mach/machine.h \
  ../kern/parallel.h ../vm/vm_kern.h 
# Dependencies for File: kern_time.o:
kern_time.o: ../bsd/kern_time.c ./cputypes.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/proc.h ../machine/reg.h 
# Dependencies for File: kern_xxx.o:
kern_xxx.o: ../bsd/kern_xxx.c ./cputypes.h ./quota.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/kernel.h ../sys/proc.h \
  ../sys/reboot.h ./mach_kdb.h 
# Dependencies for File: key.o:
key.o: ../mac2dev/key.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/kernel.h \
  ../sys/time.h ../mac2/clock.h ../mac2/act.h ../kern/zalloc.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../kern/queue.h ../kern/macro_help.h ../mac2dev/fdb.h ../mac2dev/key.h 
# Dependencies for File: lock.o:
lock.o: ../kern/lock.c ./cpus.h ../kern/lock.h ./mach_ldebug.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/machine/vm_types.h \
  ../kern/ast.h ./hw_ast.h ../machine/cpu.h ../sys/types.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h 
# Dependencies for File: locore.o:
locore.o: locore.c ./macos_emul.h ../sys/errno.h ./mach_nfs.h \
  ../mach/mac2/exception.h ../mac2/asm.h ../mac2/vmparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../mac2/endian.h ../mac2/cpu.h \
  ../sys/types.h ../mac2/pcb.h ../mac2/psl.h ../mac2/MacGlobals.h \
  ../mac2/trap.h ./cputypes.h ./assym.s 
# Dependencies for File: mac2_exception.o:
mac2_exception.o: ../mac2/mac2_exception.c ../sys/signal.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/exception.h \
  ../mach/machine/exception.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h 
# Dependencies for File: mac2_init.o:
mac2_init.o: ../mac2/mac2_init.c ../sys/types.h ../mach/machine/vm_types.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_prot.h ../vm/vm_page.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/memory_object.h ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/macro_help.h \
  ../kern/assert.h ./mach_assert.h ../kern/thread.h ./mach_fixpri.h \
  ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_inherit.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h 
# Dependencies for File: mac2_mach_process.o:
mac2_mach_process.o: ../mac2/mac2_mach_process.c ./cputypes.h \
  ../machine/reg.h ../mach/mac2/reg.h ../machine/psl.h ../machine/vmparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../sys/types.h ../mach/machine/vm_types.h ../mac2/endian.h \
  ../sys/param.h ../machine/machparam.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/user.h ./mach_rfs.h \
  ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../sys/dir.h ../machine/pcb.h ../mach/mac2/frame.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_kern.h ../mac2/ptrace.h 
# Dependencies for File: macServer.o:
macServer.o: ../mac2emul/macServer.c ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/message.h ./mach_ipc_xxxhack.h \
  ../mach/port.h ../mach/mig_errors.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/std_types.h \
  ../mach/machine/vm_types.h ../mach/mach_types.h ../mach/vm_inherit.h \
  ../mach/vm_prot.h ../mach/memory_object.h ../mach/vm_statistics.h \
  ../mach/vm_attributes.h ../mach/machine.h ./cpus.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../sys/types.h ../vm/vm_map.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mac2emul/macserver_types.h ../mac2emul/macether_types.h 
# Dependencies for File: macbus_cycle.o:
macbus_cycle.o: ../mac2emul/macbus_cycle.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2dev/via.h \
  ../mac2emul/macdefs.h ../mac2emul/macinternal.h ../sys/buf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/socket.h ./mach_vmtp.h ./vice.h ../net/if.h ../net/if_arp.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h \
  ../kern/assert.h ./mach_assert.h ../mac2emul/macserver_types.h \
  ../mac2emul/macvia.h ../mac2emul/macuser.h ../mac2emul/macuser_inline.c 
# Dependencies for File: macdevice.o:
macdevice.o: ../mac2emul/macdevice.c ../sys/types.h \
  ../mach/machine/vm_types.h ../vm/vm_map.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h \
  ../kern/assert.h ./mach_assert.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../sys/socket.h ./mach_vmtp.h \
  ./vice.h ../net/if.h ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macdevice.h 
# Dependencies for File: macether_hash.o:
macether_hash.o: ../mac2emul/macether_hash.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2emul/macdefs.h ../mac2emul/macinternal.h \
  ../sys/buf.h ../sys/socket.h ./mach_vmtp.h ./vice.h ../net/if.h \
  ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macether_hash.h 
# Dependencies for File: macether_interface.o:
macether_interface.o: ../mac2emul/macether_interface.c ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/mbuf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/mach_types.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine.h ./cpus.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/timer.h ./stat_time.h ../sys/time.h \
  ../vm/vm_kern.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../vm/vm_map.h ../vm/pmap.h ../machine/pmap.h ../mac2/pcb.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2emul/macdefs.h ../mac2emul/macinternal.h \
  ../sys/buf.h ../sys/socket.h ./mach_vmtp.h ./vice.h ../net/if.h \
  ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macether_types.h ../mac2emul/macether_hash.h ../netinet/in.h \
  ../machine/endian.h ../netinet/if_ether.h ../netinet/in_systm.h \
  ../netinet/ip.h ../netinet/in_var.h 
# Dependencies for File: macether_routines.o:
macether_routines.o: ../mac2emul/macether_routines.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2emul/macserver_hash.h ../mac2emul/mac.h \
  ../mac2emul/macserver_types.h ../mac2emul/macether_types.h 
# Dependencies for File: macexception.o:
macexception.o: ../mac2emul/macexception.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/psl.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../kern/macro_help.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/socket.h ./mach_vmtp.h \
  ./vice.h ../net/if.h ../net/if_arp.h ../vm/vm_map.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/assert.h \
  ./mach_assert.h ../mac2emul/macserver_types.h ../mac2emul/macinst.h \
  ../mac2emul/macuser.h ../mac2emul/macuser_inline.c 
# Dependencies for File: mach_debug_server.o:
mach_debug_server.o: mach_debug/mach_debug_server.c ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/message.h ./mach_ipc_xxxhack.h \
  ../mach/port.h ../mach/mig_errors.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/std_types.h \
  ../mach/machine/vm_types.h ../kern/type_conversion.h ./mach_km.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../kern/host.h \
  ../mach/mach_types.h ../mach/vm_attributes.h ../mach/machine.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/thread_special_ports.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h \
  ../mach/kernel_event.h ../vm/vm_user.h ../mach_debug/mach_debug_types.h \
  ../mach_debug/ipc_statistics.h ./mach_ipc_stats.h \
  ../mach_debug/callout_statistics.h ./mach_co_stats.h \
  ../mach_debug/zone_info.h 
# Dependencies for File: mach_factor.o:
mach_factor.o: ../kern/mach_factor.c ./cpus.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/time.h \
  ../sys/kernel.h ../kern/sched.h ./mach_fixpri.h ./simple_clock.h \
  ./stat_time.h ../kern/queue.h ../kern/lock.h ./mach_ldebug.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/macro_help.h \
  ../mach/machine.h ../kern/processor.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/processor_info.h 
# Dependencies for File: mach_host_server.o:
mach_host_server.o: mach/mach_host_server.c ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/message.h ./mach_ipc_xxxhack.h \
  ../mach/port.h ../mach/mig_errors.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/std_types.h \
  ../mach/machine/vm_types.h ../kern/type_conversion.h ./mach_km.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../kern/host.h \
  ../mach/mach_types.h ../mach/vm_attributes.h ../mach/machine.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/thread_special_ports.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h \
  ../mach/kernel_event.h ../vm/vm_user.h 
# Dependencies for File: mach_net.o:
mach_net.o: ../kern/mach_net.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/mbuf.h ../sys/socket.h ./mach_vmtp.h ../net/if.h \
  ../net/if_arp.h ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../vm/vm_kern.h ../kern/parallel.h ../mach_debug/ipc_statistics.h \
  ./mach_ipc_stats.h 
# Dependencies for File: mach_process.o:
mach_process.o: ../bsd/mach_process.c ../machine/reg.h ../mach/mac2/reg.h \
  ../machine/psl.h ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/ptrace.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_kern.h 
# Dependencies for File: mach_server.o:
mach_server.o: mach/mach_server.c ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/message.h ./mach_ipc_xxxhack.h \
  ../mach/port.h ../mach/mig_errors.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/std_types.h \
  ../mach/machine/vm_types.h ../kern/type_conversion.h ./mach_km.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_host.h \
  ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../kern/host.h \
  ../mach/mach_types.h ../mach/vm_attributes.h ../mach/machine.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/thread_special_ports.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h \
  ../mach/kernel_event.h ../vm/vm_user.h ../kern/mach_redefines.h 
# Dependencies for File: mach_signal.o:
mach_signal.o: ../bsd/mach_signal.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/proc.h ../sys/kernel.h 
# Dependencies for File: mach_timedev.o:
mach_timedev.o: ../kern/mach_timedev.c ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../mac2/machparam.h ../mach/mac2/vm_param.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/psl.h ../mac2/endian.h \
  ../vm/vm_kern.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../vm/vm_map.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h \
  ../kern/assert.h ./mach_assert.h ../sys/time.h ../sys/errno.h ./mach_nfs.h 
# Dependencies for File: mach_user_internal.o:
mach_user_internal.o: ../kern/mach_user_internal.c \
  ./mach/mach_user_internal.h ./mach/memory_object_data_error.c \
  ./mach/mach_interface.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/port.h ../mach/message.h ../mach/std_types.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../mach/mach_types.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../mach/mach_extra.h ../mach/mig_errors.h \
  ../mach/msg_type.h ./mach/memory_object_data_supply.c \
  ./mach/memory_object_data_provided.c \
  ./mach/memory_object_data_unavailable.c \
  ./mach/memory_object_set_attributes.c ./mach/port_allocate.c \
  ./mach/port_deallocate.c ./mach/port_set_add.c ./mach/port_set_allocate.c \
  ./mach/port_set_backlog.c ./mach/vm_allocate.c ./mach/vm_deallocate.c \
  ./mach/vm_set_default_memory_manager.c 
# Dependencies for File: machdep.o:
machdep.o: ../mac2/machdep.c ./cpus.h ./confdep.h ./cputypes.h ./mach_rfs.h \
  ./mach_load.h ./quota.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/map.h ../sys/vm.h \
  ../sys/vmparam.h ../machine/vmparam.h ../sys/vmmac.h ../sys/proc.h \
  ../sys/buf.h ../sys/reboot.h ./mach_kdb.h ../sys/conf.h ../sys/inode.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../vfs/vfs.h ../vfs/pathname.h ../sys/file.h ../sys/clist.h \
  ../sys/callout.h ../sys/cmap.h ../sys/mbuf.h ../sys/msgbuf.h ../sys/quota.h \
  ../sys/mount.h ../sys/fs.h ../sys/socket.h ./mach_vmtp.h ../vm/vm_kern.h \
  ../mach/machine.h ../mach/vm_param.h ../mach/machine/vm_param.h ../net/if.h \
  ../net/if_arp.h ../netinet/in.h ../machine/endian.h \
  ../netinet/if_ether.h ../machine/reg.h 
# Dependencies for File: machine.o:
machine.o: ../kern/machine.c ./cpus.h ./mach_host.h ./mach_km.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/ipc_host.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/machine.h ../mach/machine/vm_types.h \
  ../kern/processor.h ./mach_fixpri.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../kern/queue.h ../kern/macro_help.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ../mach/message.h \
  ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h \
  ../machine/machparam.h 
# Dependencies for File: macinput.o:
macinput.o: ../mac2emul/macinput.c ../sys/types.h \
  ../mach/machine/vm_types.h ../sys/tty.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/time.h \
  ../sys/param.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../kern/thread.h ./cpus.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../sys/socket.h ./mach_vmtp.h \
  ./vice.h ../net/if.h ../net/if_arp.h ../mac2emul/macserver_types.h 
# Dependencies for File: macinst.o:
macinst.o: ../mac2emul/macinst.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../kern/thread.h ./cpus.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../mach/mac2/exception.h \
  ../mac2emul/macdefs.h ../mac2emul/macinternal.h ../sys/buf.h \
  ../sys/socket.h ./mach_vmtp.h ./vice.h ../net/if.h ../net/if_arp.h \
  ../mac2emul/macserver_types.h ../mac2emul/macinst.h ../mac2emul/macuser.h \
  ../mac2emul/macuser_inline.c 
# Dependencies for File: macinst_subr.o:
macinst_subr.o: ../mac2emul/macinst_subr.c ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mac2emul/macinst.h ../mac2emul/macuser.h \
  ../mac2emul/macuser_inline.c 
# Dependencies for File: macintr.o:
macintr.o: ../mac2emul/macintr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/proc.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/time.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../sys/dir.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../mach/exception.h ../mach/machine/exception.h \
  ../mac2/psl.h ../mac2emul/macdefs.h ../mac2emul/macinternal.h ../sys/buf.h \
  ../sys/socket.h ./mach_vmtp.h ../net/if.h ../net/if_arp.h \
  ../mac2emul/macserver_types.h ../mac2emul/macinst.h ../mac2emul/macuser.h \
  ../mac2emul/macuser_inline.c 
# Dependencies for File: mackern.o:
mackern.o: ../mac2emul/mackern.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../mac2/clock.h ../mac2/act.h ../mac2dev/via.h ../mac2slotmgr/Types.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/slotmgr.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../sys/socket.h ./mach_vmtp.h \
  ../net/if.h ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macinst.h ../mac2emul/macvia.h 
# Dependencies for File: macmemory.o:
macmemory.o: ../mac2emul/macmemory.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../mac2slotmgr/Types.h ../mac2slotmgr/Slots.h ../mac2slotmgr/slotmgr.h \
  ../mac2emul/macdefs.h ../mac2emul/macinternal.h ../sys/buf.h \
  ../sys/socket.h ./mach_vmtp.h ../net/if.h ../net/if_arp.h \
  ../mac2emul/macserver_types.h 
# Dependencies for File: macostrap.o:
macostrap.o: ../mac2emul/macostrap.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../kern/thread.h ./cpus.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../sys/socket.h ./mach_vmtp.h \
  ./vice.h ../net/if.h ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macinst.h ../mac2emul/macostrap.h ../mac2emul/macuser.h \
  ../mac2emul/macuser_inline.c ../sys/kernel.h ../sys/time.h ../mac2/clock.h 
# Dependencies for File: macostrap_tables.o:
macostrap_tables.o: ../mac2emul/macostrap_tables.c 
# Dependencies for File: macpmap.o:
macpmap.o: ../mac2emul/macpmap.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_xxxhack.h ./mach_ipc_tcache.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/port.h \
  ../mach/time_value.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../kern/macro_help.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h \
  ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h 
# Dependencies for File: macscc.o:
macscc.o: ../mac2emul/macscc.c ../sys/types.h ../mach/machine/vm_types.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../mac2dev/scc.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../sys/socket.h ./mach_vmtp.h \
  ./vice.h ../net/if.h ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macdevice.h 
# Dependencies for File: macscsi_mgr.o:
macscsi_mgr.o: ../mac2emul/macscsi_mgr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/conf.h \
  ../kern/thread.h ./cpus.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_kern.h ../mac2emul/macdefs.h \
  ../mac2emul/macinternal.h ../sys/buf.h ../sys/socket.h ./mach_vmtp.h \
  ./vice.h ../net/if.h ../net/if_arp.h ../mac2emul/macserver_types.h \
  ../mac2emul/macscsi_mgr.h ../mac2emul/macuser.h \
  ../mac2emul/macuser_inline.c 
# Dependencies for File: macserver_main.o:
macserver_main.o: ../mac2emul/macserver_main.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../kern/ipc_pobj.h ../kern/port_object.h ./mach_net.h \
  ../kern/kern_port.h ../mac2emul/macserver_hash.h \
  ./mach/mach_user_internal.h ../kern/server_loop.c ../mach/notify.h 
# Dependencies for File: macserver_phys_pages.o:
macserver_phys_pages.o: ../mac2emul/macserver_phys_pages.c \
  ../mach/mach_types.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine/vm_types.h \
  ../mach/machine.h ./cpus.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../vm/vm_page.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../kern/sched_prim.h 
# Dependencies for File: macserver_routines.o:
macserver_routines.o: ../mac2emul/macserver_routines.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../mac2emul/macserver_hash.h ../mac2emul/mac.h \
  ../mac2emul/macserver_types.h ../mac2emul/macether_types.h 
# Dependencies for File: mactrap.o:
mactrap.o: ../mac2emul/mactrap.s ../mac2/cpu.h ../sys/types.h \
  ../mac2/trap.h ./assym.s 
# Dependencies for File: macuser.o:
macuser.o: ../mac2emul/macuser.s ../mac2/asm.h ../mac2/cpu.h ../sys/types.h \
  ./assym.s 
# Dependencies for File: macvia.o:
macvia.o: ../mac2emul/macvia.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2dev/via.h \
  ../mac2emul/macdefs.h ../mac2emul/macinternal.h ../sys/buf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/socket.h ./mach_vmtp.h ./vice.h ../net/if.h ../net/if_arp.h \
  ../vm/vm_map.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h \
  ../kern/assert.h ./mach_assert.h ../mac2emul/macserver_types.h \
  ../mac2emul/macvia.h 
# Dependencies for File: mem.o:
mem.o: ../mac2/mem.c ./ramdisk.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/errno.h \
  ./mach_nfs.h ../sys/buf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/uio.h ../sys/mman.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../mac2/pmap_kmap.h ./cputypes.h 
# Dependencies for File: memory_object.o:
memory_object.o: ../vm/memory_object.c ../mach/std_types.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/machine/vm_types.h ../mach/mach_types.h ../mach/vm_inherit.h \
  ../mach/vm_prot.h ../mach/memory_object.h ../mach/vm_statistics.h \
  ../mach/vm_attributes.h ../mach/machine.h ./cpus.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../sys/types.h ../vm/vm_map.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ./mach/memory_object_user.h ../kern/mig_user_hack.h ../vm/vm_page.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../kern/sched_prim.h \
  ../vm/vm_pageout.h ../kern/ipc_globals.h ../mach/notify.h \
  ../kern/ipc_hash.h ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ../kern/xpr.h ./xpr_debug.h ../machine/xpr.h 
# Dependencies for File: memory_object_default_user.o:
memory_object_default_user.o: mach/memory_object_default_user.c \
  mach/memory_object_default.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../mach/std_types.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../mach/mach_types.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine.h ./cpus.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../sys/types.h ../vm/vm_map.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../kern/mig_user_hack.h ../mach/mig_errors.h ../mach/msg_type.h 
# Dependencies for File: memory_object_user.o:
memory_object_user.o: mach/memory_object_user.c mach/memory_object_user.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/std_types.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/machine/vm_types.h \
  ../mach/mach_types.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine.h ./cpus.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../sys/types.h ../vm/vm_map.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../kern/mig_user_hack.h ../mach/mig_errors.h ../mach/msg_type.h 
# Dependencies for File: mfs_prim.o:
mfs_prim.o: ../kern/mfs_prim.c ./mach_nbc.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../mach/boolean.h ../mach/machine/boolean.h ../kern/mfs.h \
  ./mach_afs.h ../mach/memory_object.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../kern/queue.h ../mach/machine/vm_types.h ../kern/macro_help.h \
  ../kern/zalloc.h ../vm/vm_object.h ./mach_pagemap.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/assert.h ./mach_assert.h \
  ../sys/types.h ../sys/param.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../sys/mount.h ./vice.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h ../mac2/pcb.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/syscall_emulation.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h ../sys/inode.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../vfs/vfs.h ../vfs/pathname.h ../vm/vm_kern.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../kern/xpr.h ./xpr_debug.h ../machine/xpr.h \
  ../kern/sched_prim.h 
# Dependencies for File: mhd.o:
mhd.o: ../mac2dev/mhd.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/buf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/conf.h ../sys/dir.h ../sys/errno.h ./mach_nfs.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/time.h ../sys/disklabel.h 
# Dependencies for File: mig_support.o:
mig_support.o: ../kern/mig_support.c ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_xxxhack.h ./mach_ipc_tcache.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/port.h ../mach/time_value.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../mach/machine/vm_types.h ../kern/macro_help.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h \
  ../kern/assert.h ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ./mach/mach_user_internal.h 
# Dependencies for File: mouse.o:
mouse.o: ../mac2dev/mouse.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2dev/fdb.h \
  ../mac2dev/mouse.h 
# Dependencies for File: mscsi.o:
mscsi.o: ../mac2dev/mscsi.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/errno.h \
  ./mach_nfs.h ../sys/kernel.h ../sys/time.h ../mac2/clock.h ../mac2/act.h \
  ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../kern/macro_help.h \
  ../mac2dev/via.h ../mac2dev/mscsi.h ../mac2dev/ncr5380.h 
# Dependencies for File: netisr.o:
netisr.o: ../net/netisr.c ./imp.h ../net/netisr.h ../kern/processor.h \
  ./cpus.h ./mach_fixpri.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../kern/lock.h ./mach_ldebug.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../kern/queue.h ../mach/machine/vm_types.h ../kern/macro_help.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/thread.h \
  ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/cpu.h ../sys/types.h ../machine/ast.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h \
  ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/sched_prim.h ../machine/machparam.h 
# Dependencies for File: ostrap.o:
ostrap.o: ../mac2/ostrap.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../kern/thread.h ./cpus.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../mac2/psl.h 
# Dependencies for File: ostrap_tables.o:
ostrap_tables.o: ../mac2/ostrap_tables.c 
# Dependencies for File: parallel.o:
parallel.o: ../kern/parallel.c ./cpus.h ./mach_host.h 
# Dependencies for File: param.o:
param.o: ../conf/param.c ./quota.h ./confdep.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/socket.h \
  ./mach_vmtp.h ./vice.h ../sys/dir.h ../sys/user.h ./mach_rfs.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/file.h ../sys/callout.h ../sys/clist.h \
  ../sys/mbuf.h ../sys/quota.h ../sys/kernel.h 
# Dependencies for File: pcb.o:
pcb.o: ../mac2/pcb.c ../kern/thread.h ./cpus.h ./mach_fixpri.h \
  ./mach_host.h ./mach_km.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/machine/vm_types.h ../kern/ast.h ./hw_ast.h ../machine/cpu.h \
  ../sys/types.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/queue.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_kern.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../mach/mac2/kern_return.h ../mac2/psl.h \
  ../sys/param.h ../machine/machparam.h ../sys/signal.h ../sys/systm.h \
  ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/errno.h ./mach_nfs.h \
  ../sys/proc.h 
# Dependencies for File: pmap.o:
pmap.o: ../mac2/pmap.c ./cpus.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/machine/vm_types.h \
  ../kern/ast.h ./hw_ast.h ../machine/cpu.h ../sys/types.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../vm/vm_kern.h ../vm/vm_page.h ../kern/sched_prim.h ../vm/vm_user.h \
  ../mac2/pmap_kmap.h ./cputypes.h ../mac2/MacGlobals.h \
  ../machine/pmmu_inline.c ../machine/bitmap_inline.c \
  ../machine/pmap_inline.c 
# Dependencies for File: processor.o:
processor.o: ../kern/processor.c ./cpus.h ./mach_fixpri.h ./mach_host.h \
  ../kern/ipc_globals.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/notify.h ../mach/message.h \
  ../kern/zalloc.h ../mach/machine/vm_types.h ../kern/lock.h ./mach_ldebug.h \
  ../kern/queue.h ../kern/macro_help.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/ipc_hash.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../kern/kern_port.h ../kern/port_object.h ./mach_km.h \
  ./mach_net.h ../kern/host.h ../mach/policy.h ../mach/processor_info.h \
  ../mach/machine.h ../kern/thread.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h ../machine/thread.h \
  ../machine/pcb.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../kern/ipc_host.h 
# Dependencies for File: queue.o:
queue.o: ../kern/queue.c ../kern/queue.h ../mach/machine/vm_types.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/macro_help.h 
# Dependencies for File: quota_sys.o:
quota_sys.o: ../bsd/quota_sys.c ./quota.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/quota.h ../sys/mount.h ../sys/fs.h 
# Dependencies for File: raw_cb.o:
raw_cb.o: ../net/raw_cb.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h \
  ../sys/socketvar.h ../sys/domain.h ../sys/protosw.h ../sys/errno.h \
  ./mach_nfs.h ../net/if.h ../net/if_arp.h ../net/route.h \
  ../net/raw_cb.h ../netinet/in.h ../machine/endian.h 
# Dependencies for File: raw_ip.o:
raw_ip.o: ../netinet/raw_ip.c ./multicast.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/mbuf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/socket.h ./mach_vmtp.h ./vice.h ../sys/protosw.h ../sys/socketvar.h \
  ../sys/errno.h ./mach_nfs.h ../net/if.h \
  ../net/if_arp.h ../net/route.h \
  ../net/raw_cb.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_systm.h ../netinet/ip.h ../netinet/ip_var.h 
# Dependencies for File: raw_usrreq.o:
raw_usrreq.o: ../net/raw_usrreq.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/mbuf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/domain.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h ./vice.h \
  ../sys/socketvar.h ../sys/errno.h ./mach_nfs.h ../net/if.h \
  ../net/if_arp.h ../net/route.h ../net/netisr.h ../net/raw_cb.h \
  ./dli.h ./mach_device.h 
# Dependencies for File: route.o:
route.o: ../net/route.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h \
  ./vice.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../net/if.h ../net/if_arp.h \
  ../net/af.h ../net/route.h 
# Dependencies for File: scc.o:
scc.o: ../mac2dev/scc.c ../sys/types.h ../mach/machine/vm_types.h \
  ../mac2dev/scc.h ../mac2/autoconf.h 
# Dependencies for File: sched_prim.o:
sched_prim.o: ../kern/sched_prim.c ./cpus.h ./simple_clock.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ./mach_nfs.h ./fast_csw.h \
  ./hw_footprint.h ../sys/types.h ../mach/machine/vm_types.h ../kern/assert.h \
  ./mach_assert.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/ast.h ./hw_ast.h ../machine/cpu.h \
  ../machine/ast.h ../kern/thread.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../kern/queue.h ../kern/lock.h ./mach_ldebug.h \
  ../kern/processor.h ../kern/sched.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../mach/machine.h \
  ../vm/vm_kern.h ../kern/parallel.h ../machine/machparam.h \
  ../kern/sched_prim.h ../kern/thread_swap.h 
# Dependencies for File: setjmp.o:
setjmp.o: ../mac2/setjmp.s ../mac2/asm.h 
# Dependencies for File: slave.o:
slave.o: ../kern/slave.c ./cpus.h ./simple_clock.h ../machine/reg.h \
  ../mach/mac2/reg.h ../machine/psl.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../mac2/endian.h ../sys/signal.h ../sys/systm.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/dk.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../kern/lock.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/proc.h ../machine/clock.h \
  ../mach/machine.h 
# Dependencies for File: slotmgr_CallDriver.o:
slotmgr_CallDriver.o: ../mac2slotmgr/slotmgr_CallDriver.s 
# Dependencies for File: slotmgr_SCallIntr.o:
slotmgr_SCallIntr.o: ../mac2slotmgr/slotmgr_SCallIntr.s 
# Dependencies for File: slotmgr_SExecCode.o:
slotmgr_SExecCode.o: ../mac2slotmgr/slotmgr_SExecCode.s 
# Dependencies for File: slotmgr_advanced.o:
slotmgr_advanced.o: ../mac2slotmgr/slotmgr_advanced.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/ROMDefs.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: slotmgr_alloc.o:
slotmgr_alloc.o: ../mac2slotmgr/slotmgr_alloc.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h 
# Dependencies for File: slotmgr_dispatch.o:
slotmgr_dispatch.o: ../mac2slotmgr/slotmgr_dispatch.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: slotmgr_extra.o:
slotmgr_extra.o: ../mac2slotmgr/slotmgr_extra.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/ROMDefs.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: slotmgr_interrupts.o:
slotmgr_interrupts.o: ../mac2slotmgr/slotmgr_interrupts.c \
  ../mach/mach_types.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine/vm_types.h \
  ../mach/machine.h ./cpus.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../machine/ffsl_inline.c ../mac2/act.h ../mac2dev/via.h \
  ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h ../mac2slotmgr/Slots.h \
  ../mac2slotmgr/ROMDefs.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: slotmgr_principal.o:
slotmgr_principal.o: ../mac2slotmgr/slotmgr_principal.c \
  ../mach/mach_types.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine/vm_types.h \
  ../mach/machine.h ./cpus.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/ROMDefs.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: slotmgr_special.o:
slotmgr_special.o: ../mac2slotmgr/slotmgr_special.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/ROMDefs.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: slotmgr_subr.o:
slotmgr_subr.o: ../mac2slotmgr/slotmgr_subr.c ../mach/mach_types.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/ROMDefs.h ../mac2slotmgr/slotmgr.h 
# Dependencies for File: softint.o:
softint.o: ../mac2/softint.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../mac2/act.h \
  ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../kern/macro_help.h \
  ../mac2dev/via.h ./cputypes.h 
# Dependencies for File: spec_subr.o:
spec_subr.o: ../specfs/spec_subr.c ./streams.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../specfs/snode.h ../kern/mfs.h ./mach_afs.h \
  ../kern/kalloc.h 
# Dependencies for File: spec_vnodeops.o:
spec_vnodeops.o: ../specfs/spec_vnodeops.c ./sun_lock.h ./streams.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/buf.h ../sys/kernel.h ../sys/stat.h \
  ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h ../sys/conf.h ../sys/file.h \
  ../specfs/snode.h ../kern/mfs.h ./mach_afs.h ../krpc/lockmgr.h 
# Dependencies for File: strings.o:
strings.o: ../mac2/strings.c 
# Dependencies for File: subr_log.o:
subr_log.o: ../bsd/subr_log.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/msgbuf.h \
  ../sys/file.h 
# Dependencies for File: subr_prf.o:
subr_prf.o: ../bsd/subr_prf.c ./cpus.h ./cputypes.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/buf.h \
  ../kern/macro_help.h ../sys/conf.h ../sys/reboot.h ./mach_kdb.h \
  ../sys/msgbuf.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/tty.h \
  ../sys/syslog.h 
# Dependencies for File: subr_rmap.o:
subr_rmap.o: ../bsd/subr_rmap.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/map.h ../vm/vm_map.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/proc.h ../sys/kernel.h 
# Dependencies for File: subr_xxx.o:
subr_xxx.o: ../bsd/subr_xxx.c ./cputypes.h ../vm/vm_map.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../vm/vm_kern.h ../sys/param.h ../machine/machparam.h \
  ../sys/signal.h ../sys/systm.h ../sys/conf.h ../sys/inode.h ./mach_rfs.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../sys/time.h ../kern/zalloc.h ../vfs/vfs.h ../vfs/pathname.h ../sys/dir.h \
  ../sys/user.h ./vice.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/buf.h ../sys/proc.h \
  ../sys/fs.h 
# Dependencies for File: swapgeneric.o:
swapgeneric.o: ../machine/swapgeneric.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/conf.h \
  ../sys/buf.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/vm.h ../sys/vmparam.h ../machine/vmparam.h \
  ../sys/vmmac.h ../sys/systm.h ../sys/reboot.h ./mach_kdb.h ../sys/file.h 
# Dependencies for File: sys_generic.o:
sys_generic.o: ../bsd/sys_generic.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/file.h ../sys/proc.h ../sys/stat.h \
  ../kern/sched_prim.h ../kern/parallel.h 
# Dependencies for File: sys_socket.o:
sys_socket.o: ../bsd/sys_socket.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/file.h ../sys/mbuf.h ../sys/protosw.h \
  ./mach_vmtp.h ../sys/socket.h ../sys/socketvar.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/stat.h ../net/if.h ../net/if_arp.h ../net/route.h 
# Dependencies for File: syscall_emulation.o:
syscall_emulation.o: ../kern/syscall_emulation.c ./mach_emulation.h \
  ../mach/mach_types.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine/vm_types.h \
  ../mach/machine.h ./cpus.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h ../mach/mac2/reg.h \
  ../mach/mac2/frame.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/time_value.h ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../sys/types.h ../vm/vm_map.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_ipc_tcache.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h 
# Dependencies for File: syscall_subr.o:
syscall_subr.o: ../kern/syscall_subr.c ./mach_emulation.h ./mach_fixpri.h \
  ./mach_sctimes.h ./mach_vfs.h ./stat_time.h ./cpus.h ../kern/thread.h \
  ./mach_host.h ./mach_km.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/machine/vm_types.h ../kern/ast.h ./hw_ast.h ../machine/cpu.h \
  ../sys/types.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/queue.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ../kern/task.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../mach/thread_switch.h ../sys/user.h ./mach_rfs.h \
  ./vice.h ../sys/param.h ../machine/machparam.h ../sys/signal.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h \
  ../sys/errno.h ./mach_nfs.h ../sys/proc.h ../kern/ipc_pobj.h ../kern/host.h \
  ../kern/port_object.h ./mach_net.h ../kern/kern_port.h \
  ../kern/sched_prim_macros.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../sys/systm.h ../sys/mount.h ../sys/fs.h ../sys/buf.h ../sys/inode.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/quota.h ./quota.h ../sys/kernel.h ../sys/file.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../builtin/inode_pager.h \
  ./mach_xp.h 
# Dependencies for File: syscall_sw.o:
syscall_sw.o: ../kern/syscall_sw.c ./mach_rfs.h ./mach_ipc_xxxhack.h \
  ./mach_emulation.h ./mach_sctimes.h ../mach/port.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/syscall_sw.h ../mach/mach_traps.h \
  ../mach/mach_types.h ../mach/vm_inherit.h ../mach/vm_prot.h \
  ../mach/memory_object.h ../mach/vm_statistics.h ../mach/vm_attributes.h \
  ../mach/machine/vm_types.h ../mach/machine.h ./cpus.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../sys/time.h ../vm/vm_kern.h ../sys/types.h ../vm/vm_map.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_ipc_tcache.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../kern/ipc_basics.h ../kern/syscall_subr.h \
  ../builtin/inode_pager.h ./mach_xp.h 
# Dependencies for File: task.o:
task.o: ../kern/task.c ./mach_host.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/task.h ./mach_emulation.h ./mach_ipc_xxxhack.h \
  ./mach_ipc_tcache.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/port.h ../mach/time_value.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../mach/machine/vm_types.h ../kern/macro_help.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../sys/types.h ../machine/psl.h ../mac2/endian.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/thread.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_kern.h ../kern/ipc_tt.h \
  ../kern/ipc_globals.h ../mach/notify.h ../kern/ipc_hash.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../kern/sched_prim.h \
  ../machine/machparam.h 
# Dependencies for File: tcp_debug.o:
tcp_debug.o: ../netinet/tcp_debug.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h \
  ../sys/socketvar.h ../sys/protosw.h ../sys/errno.h ./mach_nfs.h \
  ../net/route.h ../net/if.h \
  ../net/if_arp.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../netinet/tcp.h ../netinet/tcp_fsm.h \
  ../netinet/tcp_seq.h ../netinet/tcp_timer.h ../netinet/tcp_var.h \
  ../netinet/tcpip.h ../netinet/tcp_debug.h 
# Dependencies for File: tcp_input.o:
tcp_input.o: ../netinet/tcp_input.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h \
  ./vice.h ../sys/socketvar.h ../sys/errno.h ./mach_nfs.h \
  ../net/if.h ../net/if_arp.h \
  ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../netinet/tcp.h ../netinet/tcp_fsm.h \
  ../netinet/tcp_seq.h ../netinet/tcp_timer.h ../netinet/tcp_var.h \
  ../netinet/tcpip.h ../netinet/tcp_debug.h 
# Dependencies for File: tcp_output.o:
tcp_output.o: ../netinet/tcp_output.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/protosw.h ./mach_vmtp.h ../sys/socket.h \
  ./vice.h ../sys/socketvar.h ../sys/errno.h ./mach_nfs.h \
  ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../netinet/tcp.h ../netinet/tcp_fsm.h \
  ../netinet/tcp_seq.h ../netinet/tcp_timer.h ../netinet/tcp_var.h \
  ../netinet/tcpip.h ../netinet/tcp_debug.h 
# Dependencies for File: tcp_subr.o:
tcp_subr.o: ../netinet/tcp_subr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h \
  ../sys/socketvar.h ../sys/protosw.h ../sys/errno.h ./mach_nfs.h \
  ../net/route.h ../net/if.h \
  ../net/if_arp.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../netinet/ip_icmp.h ../netinet/tcp.h \
  ../netinet/tcp_fsm.h ../netinet/tcp_seq.h ../netinet/tcp_timer.h \
  ../netinet/tcp_var.h ../netinet/tcpip.h 
# Dependencies for File: tcp_timer.o:
tcp_timer.o: ../netinet/tcp_timer.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h \
  ../sys/socketvar.h ../sys/protosw.h ../sys/errno.h ./mach_nfs.h \
  ../net/if.h ../net/if_arp.h \
  ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../netinet/tcp.h ../netinet/tcp_fsm.h \
  ../netinet/tcp_seq.h ../netinet/tcp_timer.h ../netinet/tcp_var.h \
  ../netinet/tcpip.h 
# Dependencies for File: tcp_usrreq.o:
tcp_usrreq.o: ../netinet/tcp_usrreq.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mbuf.h \
  ../kern/macro_help.h ../sys/socket.h ./mach_vmtp.h ./vice.h \
  ../sys/socketvar.h ../sys/protosw.h ../sys/errno.h ./mach_nfs.h \
  ../sys/stat.h ../net/if.h ../net/if_arp.h \
  ../net/route.h ../netinet/in.h ../machine/endian.h \
  ../netinet/in_pcb.h ../netinet/in_systm.h ../netinet/ip.h \
  ../netinet/ip_var.h ../netinet/tcp.h ../netinet/tcp_fsm.h \
  ../netinet/tcp_seq.h ../netinet/tcp_timer.h ../netinet/tcp_var.h \
  ../netinet/tcpip.h ../netinet/tcp_debug.h 
# Dependencies for File: thread.o:
thread.o: ../kern/thread.c ./cpus.h ./hw_footprint.h ./mach_km.h \
  ./mach_host.h ./mach_fixpri.h ./simple_clock.h ../kern/ast.h ./hw_ast.h \
  ../machine/cpu.h ../sys/types.h ../mach/machine/vm_types.h ../machine/ast.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../kern/thread.h ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h \
  ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../mach/policy.h \
  ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/thread_special_ports.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_kern.h ../kern/parallel.h \
  ../kern/ipc_tt.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../machine/machparam.h 
# Dependencies for File: thread_swap.o:
thread_swap.o: ../kern/thread_swap.c ../kern/thread.h ./cpus.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../kern/ast.h \
  ./hw_ast.h ../machine/cpu.h ../sys/types.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../vm/vm_kern.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../kern/sched_prim.h ../kern/thread_swap.h \
  ../machine/machparam.h 
# Dependencies for File: timer.o:
timer.o: ../kern/timer.c ./cpus.h ./stat_time.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/kernel.h ../sys/time.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/macro_help.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h 
# Dependencies for File: trap.o:
trap.o: ../mac2/trap.c ./mach_rfs.h ./vice.h ./xpr_debug.h ./macos_emul.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/dir.h ../sys/user.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/vm.h ../sys/vmparam.h \
  ../machine/vmparam.h ../sys/vmmac.h ../sys/kernel.h ../bsd/syscalls.c \
  ./mach_vmtp.h ./quota.h ./sun_rpc.h ../kern/xpr.h ../machine/xpr.h \
  ../mac2/pmap.h ../machine/reg.h ../machine/trap.h ../machine/call_frame.h \
  ../vm/vm_kern.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../kern/syscall_sw.h ../mach/exception.h ../mach/machine/exception.h \
  ../builtin/ux_exception.h ../mac2emul/macdefs.h ../mac2emul/macinternal.h \
  ../sys/buf.h ../sys/socket.h ../net/if.h ../net/if_arp.h \
  ../mac2emul/macserver_types.h 
# Dependencies for File: tty.o:
tty.o: ../bsd/tty.c ./cputypes.h ./vice.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/tty.h ../sys/proc.h ../sys/inode.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../vfs/vfs.h ../vfs/pathname.h ../sys/file.h ../sys/conf.h ../sys/buf.h \
  ../sys/dk.h ../machine/reg.h 
# Dependencies for File: tty_cmupty.o:
tty_cmupty.o: ../bsd/tty_cmupty.c ./cmupty.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/tty.h ../sys/proc.h ../sys/cmupty.h \
  ../sys/conf.h ../sys/file.h ../sys/acct.h 
# Dependencies for File: tty_conf.o:
tty_conf.o: ../bsd/tty_conf.c ./fe.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/buf.h \
  ../kern/macro_help.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/time.h ../sys/tty.h ../sys/conf.h \
  ./bk.h ./tb.h ./sl.h 
# Dependencies for File: tty_pty.o:
tty_pty.o: ../bsd/tty_pty.c ./pty.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/time.h ../sys/tty.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/conf.h ../sys/file.h ../sys/proc.h \
  ../sys/cmupty.h 
# Dependencies for File: tty_subr.o:
tty_subr.o: ../bsd/tty_subr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/buf.h \
  ../kern/macro_help.h ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h \
  ../sys/ttyloc.h ../sys/kernel.h ../sys/time.h ../sys/tty.h ../sys/clist.h 
# Dependencies for File: tty_tty.o:
tty_tty.o: ../bsd/tty_tty.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/conf.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../sys/tty.h ../sys/proc.h 
# Dependencies for File: udp_usrreq.o:
udp_usrreq.o: ../netinet/udp_usrreq.c ./multicast.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/mbuf.h ../sys/protosw.h ./mach_vmtp.h \
  ../sys/socket.h ../sys/socketvar.h ../sys/stat.h ../net/if.h \
  ../net/if_arp.h ../net/route.h ../netinet/in.h \
  ../machine/endian.h ../netinet/in_pcb.h ../netinet/in_systm.h \
  ../netinet/ip.h ../netinet/ip_var.h ../netinet/ip_icmp.h ../netinet/udp.h \
  ../netinet/udp_var.h 
# Dependencies for File: ufs_alloc.o:
ufs_alloc.o: ../bsd/ufs_alloc.c ./quota.h ./mach_fastlink.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/mount.h \
  ./vice.h ../sys/fs.h ../sys/buf.h ../kern/macro_help.h ../sys/inode.h \
  ./mach_rfs.h ./mach_vfs.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../sys/time.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../vfs/vfs.h ../vfs/pathname.h ../sys/dir.h ../sys/user.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/quota.h ../sys/kernel.h \
  ../sys/syslog.h 
# Dependencies for File: ufs_bmap.o:
ufs_bmap.o: ../vfs/ufs_bmap.c ./mach_nbc.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/inode.h ./mach_rfs.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../sys/time.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../kern/macro_help.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/dir.h ../sys/user.h ./vice.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../sys/errno.h ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h \
  ./mach_km.h ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h \
  ../kern/ast.h ./hw_ast.h ../machine/ast.h ../mac2/pcb.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/assert.h ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/buf.h ../sys/proc.h ../sys/fs.h ../sys/kernel.h 
# Dependencies for File: ufs_dir.o:
ufs_dir.o: ../vfs/ufs_dir.c ./mach_nbc.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/kernel.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/buf.h ../vfs/dnlc.h ../sys/fs.h ../sys/mount.h 
# Dependencies for File: ufs_disksubr.o:
ufs_disksubr.o: ../bsd/ufs_disksubr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/buf.h \
  ../kern/macro_help.h ../sys/disklabel.h ../sys/syslog.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h 
# Dependencies for File: ufs_inode.o:
ufs_inode.o: ../bsd/ufs_inode.c ./mach_nbc.h ./mach_vfs.h ./mach_fastlink.h \
  ./quota.h ./vice.h ./xpr_debug.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../builtin/inode_pager.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ./mach_xp.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../sys/mount.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/inode.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../vfs/vfs.h ../vfs/pathname.h ../sys/fs.h ../sys/buf.h ../kern/mfs.h \
  ./mach_afs.h ../sys/kernel.h 
# Dependencies for File: ufs_machdep.o:
ufs_machdep.o: ../mac2/ufs_machdep.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/user.h ./mach_rfs.h \
  ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../sys/dir.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/buf.h ../sys/conf.h ../sys/proc.h ../sys/vm.h \
  ../sys/vmparam.h ../machine/vmparam.h ../sys/vmmac.h 
# Dependencies for File: ufs_physio.o:
ufs_physio.o: ../bsd/ufs_physio.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/buf.h ../sys/conf.h ../sys/proc.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h 
# Dependencies for File: ufs_subr.o:
ufs_subr.o: ../bsd/ufs_subr.c ./cputypes.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/mount.h ./vice.h \
  ../sys/fs.h ../sys/buf.h ../kern/macro_help.h ../sys/inode.h ./mach_rfs.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../sys/time.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../vfs/vfs.h ../vfs/pathname.h ../sys/dir.h ../sys/user.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/quota.h ./quota.h \
  ../sys/kernel.h 
# Dependencies for File: ufs_tables.o:
ufs_tables.o: ../bsd/ufs_tables.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h 
# Dependencies for File: ufs_vfsops.o:
ufs_vfsops.o: ../vfs/ufs_vfsops.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/buf.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/file.h ../sys/conf.h ../sys/fs.h ../sys/mount.h \
  ../vm/vm_kern.h 
# Dependencies for File: ufs_vnodeops.o:
ufs_vnodeops.o: ../vfs/ufs_vnodeops.c ./mach_fastlink.h ./xpr_debug.h \
  ./mach_nbc.h ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/buf.h ../sys/inode.h \
  ./mach_vfs.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/proc.h ../sys/file.h ../sys/conf.h ../sys/kernel.h \
  ../sys/fs.h ../sys/mount.h ../krpc/lockmgr.h ../kern/xpr.h ../machine/xpr.h \
  ../kern/kalloc.h ../kern/mfs.h ./mach_afs.h ../kern/parallel.h 
# Dependencies for File: ufs_xxx.o:
ufs_xxx.o: ../bsd/ufs_xxx.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/inode.h ./mach_rfs.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../sys/time.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../kern/queue.h ../kern/macro_help.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/fs.h ../sys/mount.h ./vice.h ../sys/dir.h ../sys/user.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/buf.h ../sys/conf.h 
# Dependencies for File: uipc_domain.o:
uipc_domain.o: ../bsd/uipc_domain.c ./dli.h ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/socket.h \
  ./mach_vmtp.h ./vice.h ../sys/protosw.h ../sys/domain.h ../sys/time.h \
  ../sys/kernel.h ../kern/macro_help.h ../mach/boolean.h \
  ../mach/machine/boolean.h ./imp.h 
# Dependencies for File: uipc_mbuf.o:
uipc_mbuf.o: ../bsd/uipc_mbuf.c ./cputypes.h ../vm/vm_map.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../vm/vm_kern.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../kern/kalloc.h ../sys/param.h \
  ../machine/machparam.h ../sys/signal.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/proc.h ../sys/map.h \
  ../sys/mbuf.h ../sys/kernel.h ../sys/syslog.h ../sys/domain.h \
  ../sys/protosw.h ./mach_vmtp.h 
# Dependencies for File: uipc_proto.o:
uipc_proto.o: ../bsd/uipc_proto.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/socket.h \
  ./mach_vmtp.h ./vice.h ../sys/protosw.h ../sys/domain.h ../sys/mbuf.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h 
# Dependencies for File: uipc_socket.o:
uipc_socket.o: ../bsd/uipc_socket.c ./vice.h ./mach_vmtp.h ./mach_np.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/dir.h ../sys/user.h ./mach_rfs.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ./mach_nfs.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h \
  ../kern/queue.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/assert.h ./mach_assert.h \
  ../kern/kern_set.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/vm_statistics.h ../mac2/pmmu_inline.c ./macos_emul.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../sys/proc.h ../sys/file.h \
  ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h ../sys/buf.h ../sys/mbuf.h \
  ../sys/un.h ../sys/domain.h ../sys/protosw.h ../sys/socket.h \
  ../sys/socketvar.h ../sys/stat.h ../sys/ioctl.h ../sys/ttychars.h \
  ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h ../net/route.h \
  ../netinet/in.h ../machine/endian.h ../net/if.h ../net/if_arp.h \
  ../mach/exception.h ../mach/machine/exception.h ../builtin/ux_exception.h 
# Dependencies for File: uipc_socket2.o:
uipc_socket2.o: ../bsd/uipc_socket2.c ./mach_nfs.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h \
  ../sys/namei.h ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/file.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/buf.h ../sys/mbuf.h ../sys/protosw.h ./mach_vmtp.h \
  ../sys/socket.h ../sys/socketvar.h 
# Dependencies for File: uipc_syscalls.o:
uipc_syscalls.o: ../bsd/uipc_syscalls.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/file.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/buf.h ../sys/mbuf.h ../sys/protosw.h ./mach_vmtp.h \
  ../sys/socket.h ../sys/socketvar.h 
# Dependencies for File: uipc_usrreq.o:
uipc_usrreq.o: ../bsd/uipc_usrreq.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/mbuf.h ../sys/domain.h ../sys/protosw.h \
  ./mach_vmtp.h ../sys/socket.h ../sys/socketvar.h ../sys/unpcb.h ../sys/un.h \
  ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h ../sys/file.h ../sys/stat.h 
# Dependencies for File: ux_exception.o:
ux_exception.o: ../builtin/ux_exception.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../mach/exception.h ../mach/machine/exception.h \
  ../mach/mig_errors.h ../kern/sched_prim.h ../kern/ipc_pobj.h ../kern/host.h \
  ../kern/port_object.h ./mach_net.h ../kern/ipc_copyin.h \
  ../builtin/ux_exception.h ./mach/exc.h ../mach/std_types.h \
  ./mach/mach_user_internal.h 
# Dependencies for File: vfs.o:
vfs.o: ../vfs/vfs.c ./mach_nbc.h ./mach_nfs.h ./mach_afs.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/file.h ../sys/socket.h ./mach_vmtp.h \
  ../sys/systm.h ../sys/mount.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h 
# Dependencies for File: vfs_bio.o:
vfs_bio.o: ../vfs/vfs_bio.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h ../sys/user.h \
  ./mach_rfs.h ./vice.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/buf.h ../sys/conf.h ../sys/proc.h ../sys/vm.h \
  ../sys/vmparam.h ../machine/vmparam.h ../sys/vmmac.h ../sys/trace.h \
  ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h \
  ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h 
# Dependencies for File: vfs_conf.o:
vfs_conf.o: ../vfs/vfs_conf.c ./mach_afs.h ./mach_cfs.h ./mach_nfs.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/inode.h ./mach_rfs.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../sys/time.h ../kern/zalloc.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../kern/macro_help.h ../vfs/vfs.h \
  ../vfs/pathname.h 
# Dependencies for File: vfs_dnlc.o:
vfs_dnlc.o: ../vfs/vfs_dnlc.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/dir.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/systm.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../vfs/dnlc.h 
# Dependencies for File: vfs_io.o:
vfs_io.o: ../vfs/vfs_io.c ./mach_rfs.h ./mach_nbc.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/dir.h ../sys/user.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/fs.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../sys/file.h ../sys/stat.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/mount.h 
# Dependencies for File: vfs_lookup.o:
vfs_lookup.o: ../vfs/vfs_lookup.c ./mach_rfs.h ./mach_nfs.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/dir.h ../sys/user.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/table.h 
# Dependencies for File: vfs_pathname.o:
vfs_pathname.o: ../vfs/vfs_pathname.c ./mach_rfs.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/systm.h ../mach/boolean.h ../mach/machine/boolean.h ../sys/dir.h \
  ../sys/uio.h ../sys/errno.h ./mach_nfs.h ../vfs/pathname.h ../kern/zalloc.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h 
# Dependencies for File: vfs_syscalls.o:
vfs_syscalls.o: ../vfs/vfs_syscalls.c ./mach_nbc.h ./mach_rfs.h \
  ../sys/param.h ../sys/types.h ../mach/machine/vm_types.h \
  ../machine/machparam.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../sys/signal.h ../sys/dir.h ../sys/user.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/file.h ../sys/stat.h ../sys/ioctl.h \
  ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h ../sys/kernel.h \
  ../sys/tty.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../sys/proc.h 
# Dependencies for File: vfs_sysnames.o:
vfs_sysnames.o: ../vfs/vfs_sysnames.c ./mach_afs.h ./mach_nfs.h \
  ../sys/user.h ./mach_rfs.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/types.h ../mach/machine/vm_types.h \
  ../sys/param.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/time.h \
  ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/dir.h ../machine/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../sys/errno.h ../kern/thread.h ./mach_fixpri.h \
  ./mach_host.h ./mach_km.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/inode.h ./mach_vfs.h ./mach_fastlink.h \
  ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h ../vfs/pathname.h \
  ../vfs/vfs_sysnames.h 
# Dependencies for File: vfs_vnode.o:
vfs_vnode.o: ../vfs/vfs_vnode.c ./mach_nbc.h ./mach_rfs.h ../sys/param.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/machparam.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../machine/psl.h ../mac2/endian.h ../sys/signal.h \
  ../sys/dir.h ../sys/user.h ./vice.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../sys/time.h ../sys/resource.h ../sys/namei.h \
  ../sys/uio.h ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/macro_help.h ../mac2/pcb.h ../kern/queue.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/file.h ../sys/inode.h ./mach_vfs.h \
  ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h ../vfs/vfs.h \
  ../vfs/pathname.h ../kern/mfs.h ./mach_afs.h ../builtin/inode_pager.h \
  ./mach_xp.h 
# Dependencies for File: via.o:
via.o: ../mac2dev/via.c ./macos_emul.h ../sys/types.h \
  ../mach/machine/vm_types.h ../mach/mac2/frame.h ../mac2/autoconf.h \
  ../mac2dev/via.h ../machine/ffsl_inline.c 
# Dependencies for File: video.o:
video.o: ../mac2dev/video.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/conf.h \
  ../sys/ioctl.h ../sys/ttychars.h ../sys/ttydev.h ../sys/ttyloc.h \
  ../sys/kernel.h ../sys/time.h ../sys/errno.h ./mach_nfs.h \
  ../mach/mach_types.h ../mach/port.h ./mach_ipc_xxxhack.h \
  ../mach/vm_inherit.h ../mach/vm_prot.h ../mach/memory_object.h \
  ../mach/vm_statistics.h ../mach/vm_attributes.h ../mach/machine.h ./cpus.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/mac2/reg.h ../mach/mac2/frame.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/time_value.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/netport.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../kern/zalloc.h ../kern/lock.h \
  ./mach_ldebug.h ../kern/queue.h ../kern/macro_help.h ../kern/timer.h \
  ./stat_time.h ../vm/vm_kern.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../vm/vm_map.h ../vm/pmap.h ../machine/pmap.h \
  ../mac2/pcb.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../kern/assert.h ./mach_assert.h ../mach/kernel_event.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/processor.h ./mach_fixpri.h ../kern/sched.h \
  ./simple_clock.h ../kern/syscall_emulation.h ../kern/thread.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../machine/thread.h ../machine/pcb.h ../vm/vm_user.h ../kern/host.h \
  ../mach/std_types.h ../mac2slotmgr/Types.h ../mac2slotmgr/Errors.h \
  ../mac2slotmgr/Slots.h ../mac2slotmgr/Video.h ../mac2slotmgr/ROMDefs.h \
  ../mac2slotmgr/Devices.h ../mac2slotmgr/slotmgr.h ../mac2dev/video.h 
# Dependencies for File: virt.o:
virt.o: ../mac2/virt.s ../mac2/asm.h ./assym.s ../sys/errno.h ./mach_nfs.h 
# Dependencies for File: vm_fault.o:
vm_fault.o: ../vm/vm_fault.c ./mach_pagemap.h ../vm/vm_fault.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/message.h \
  ./mach_ipc_xxxhack.h ../mach/port.h ../kern/thread.h ./cpus.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/machine/vm_types.h ../kern/ast.h \
  ./hw_ast.h ../machine/cpu.h ../sys/types.h ../machine/ast.h \
  ../kern/macro_help.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/queue.h ../kern/lock.h ./mach_ldebug.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../vm/vm_map.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ../mach/mac2/vm_param.h ../mac2/machparam.h ../machine/cpu_inline.c \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h ../vm/vm_page.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../vm/vm_pageout.h \
  ./mach/memory_object_user.h ../mach/std_types.h ../mach/mach_types.h \
  ../mach/vm_attributes.h ../mach/machine.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/host_info.h ../mach/processor_info.h \
  ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h ../mach/kernel_event.h \
  ../vm/vm_user.h ../kern/host.h ../kern/mig_user_hack.h ../kern/xpr.h \
  ./xpr_debug.h ../machine/xpr.h 
# Dependencies for File: vm_init.o:
vm_init.o: ../vm/vm_init.c ../mach/machine/vm_types.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../vm/vm_object.h ./mach_pagemap.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h ../kern/assert.h \
  ./mach_assert.h ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_page.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/processor.h ../kern/sched.h \
  ./simple_clock.h ./stat_time.h ../kern/task.h ./mach_emulation.h \
  ./mach_ipc_tcache.h ../mach/time_value.h ../kern/mach_param.h ./mach_np.h \
  ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h \
  ../kern/msg_queue.h ../kern/syscall_emulation.h ../kern/timer.h \
  ../kern/thread_modes.h ../kern/kern_msg.h ../kern/ipc_netport.h \
  ../mach/netport.h ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h \
  ../vm/vm_kern.h ../builtin/inode_pager.h ./mach_xp.h 
# Dependencies for File: vm_kern.o:
vm_kern.o: ../vm/vm_kern.c ../kern/assert.h ./mach_assert.h \
  ../kern/macro_help.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../sys/types.h \
  ../mach/machine/vm_types.h ../vm/vm_kern.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ./cpus.h ../kern/lock.h \
  ./mach_ldebug.h ../mach/mac2/vm_param.h ../mac2/machparam.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../machine/psl.h ../mac2/endian.h \
  ../mach/vm_statistics.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h \
  ./mach_pagemap.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../vm/vm_page.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../kern/thread.h ./mach_fixpri.h ./mach_host.h \
  ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h \
  ../vm/vm_pageout.h 
# Dependencies for File: vm_machdep.o:
vm_machdep.o: ../mac2/vm_machdep.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../sys/buf.h \
  ../kern/macro_help.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h \
  ../machine/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../kern/lock.h \
  ./cpus.h ./mach_ldebug.h ../sys/errno.h ./mach_nfs.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../kern/queue.h ../kern/processor.h \
  ../kern/sched.h ./simple_clock.h ./stat_time.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/assert.h ./mach_assert.h ../kern/kern_set.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/memory_object.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../sys/proc.h ../sys/vm.h ../sys/vmparam.h \
  ../machine/vmparam.h ../sys/vmmac.h ../machine/reg.h ../vm/vm_page.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../kern/sched_prim.h \
  ../vm/vm_kern.h 
# Dependencies for File: vm_map.o:
vm_map.o: ../vm/vm_map.c ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../mac2/machparam.h ../mach/mac2/vm_param.h ../machine/cpu_inline.c \
  ../machine/cpu.h ../sys/types.h ../mach/machine/vm_types.h ../machine/psl.h \
  ../mac2/endian.h ../vm/vm_map.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/boolean.h ../mach/machine/boolean.h \
  ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h \
  ./cpus.h ../kern/lock.h ./mach_ldebug.h ../mach/vm_statistics.h \
  ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/macro_help.h \
  ../kern/assert.h ./mach_assert.h ../kern/zalloc.h ../vm/vm_page.h \
  ../kern/thread.h ./mach_fixpri.h ./mach_host.h ./mach_km.h \
  ../mach/message.h ../kern/ast.h ./hw_ast.h ../machine/ast.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/kern_set.h ../kern/msg_queue.h ../kern/syscall_emulation.h \
  ../kern/timer.h ../kern/thread_modes.h ../kern/kern_msg.h \
  ../kern/ipc_netport.h ../mach/netport.h ../machine/thread.h \
  ../machine/pcb.h ../kern/sched_prim.h ../mach/vm_attributes.h \
  ../vm/vm_fault.h ./mach_kdb.h 
# Dependencies for File: vm_object.o:
vm_object.o: ../vm/vm_object.c ./mach_pagemap.h ../kern/queue.h \
  ../mach/machine/vm_types.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/macro_help.h \
  ../vm/vm_page.h ../mach/vm_prot.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../mac2/machparam.h ../mach/mac2/vm_param.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h ../machine/psl.h \
  ../mac2/endian.h ../vm/vm_object.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/assert.h ./mach_assert.h ../kern/thread.h \
  ./mach_fixpri.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../mac2/pcb.h ../mach/mac2/frame.h \
  ../mach/mac2/reg.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_inherit.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h ../kern/xpr.h \
  ./xpr_debug.h ../machine/xpr.h ../vm/vm_pageout.h ../vm/vm_fault.h \
  ../kern/kern_port.h ../kern/port_object.h ./mach_net.h \
  ./mach/memory_object_user.h ../mach/std_types.h ../mach/mach_types.h \
  ../mach/vm_attributes.h ../mach/machine.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/host_info.h ../mach/processor_info.h \
  ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h ../mach/kernel_event.h \
  ../vm/vm_user.h ../kern/host.h ../kern/mig_user_hack.h \
  ./mach/memory_object_default.h ../kern/ipc_pobj.h ./mach_kdb.h 
# Dependencies for File: vm_pageout.o:
vm_pageout.o: ../vm/vm_pageout.c ./mach_pagemap.h ../vm/vm_page.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/vm_prot.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../sys/types.h ../mach/machine/vm_types.h ../machine/psl.h ../mac2/endian.h \
  ../vm/vm_object.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../mach/memory_object.h ../mach/port.h ./mach_ipc_xxxhack.h ../kern/queue.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/macro_help.h \
  ../kern/assert.h ./mach_assert.h ../kern/thread.h ./mach_fixpri.h \
  ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../mac2/pcb.h ../mach/mac2/frame.h ../mach/mac2/reg.h \
  ../kern/processor.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h ../mach/time_value.h \
  ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h \
  ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_inherit.h \
  ../vm/pmap.h ../machine/pmap.h ../mach/vm_statistics.h \
  ../mac2/pmmu_inline.c ./macos_emul.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h \
  ../vm/vm_pageout.h ../kern/thread_swap.h ../kern/kern_port.h \
  ../kern/port_object.h ./mach_net.h ../kern/ipc_pobj.h ../kern/host.h \
  ../kern/xpr.h ./xpr_debug.h ../machine/xpr.h ../mach/mach_types.h \
  ../mach/vm_attributes.h ../mach/machine.h ../mach/thread_status.h \
  ../mach/machine/thread_status.h ../mach/thread_info.h ../mach/policy.h \
  ../mach/thread_special_ports.h ../mach/task_info.h \
  ../mach/task_special_ports.h ../mach/host_info.h ../mach/processor_info.h \
  ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h ../mach/kernel_event.h \
  ../vm/vm_user.h ../mach/std_types.h ./mach/memory_object_user.h \
  ../kern/mig_user_hack.h ./mach/memory_object_default.h \
  ../machine/machparam.h ../machine/vm_tuning.h 
# Dependencies for File: vm_resident.o:
vm_resident.o: ../vm/vm_resident.c ./cpus.h ../kern/task.h \
  ./mach_emulation.h ./mach_ipc_xxxhack.h ./mach_ipc_tcache.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../mach/port.h \
  ../mach/time_value.h ../kern/lock.h ./mach_ldebug.h ../kern/queue.h \
  ../mach/machine/vm_types.h ../kern/macro_help.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h \
  ../kern/assert.h ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../vm/vm_page.h ../mach/vm_param.h ../mach/machine/vm_param.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h \
  ../vm/vm_pageout.h ../machine/machparam.h ../kern/xpr.h ./xpr_debug.h \
  ../machine/xpr.h ./mach_kdb.h 
# Dependencies for File: vm_unix.o:
vm_unix.o: ../vm/vm_unix.c ./cputypes.h ./mach_debug.h ./mach_emulation.h \
  ../kern/task.h ./mach_ipc_xxxhack.h ./mach_ipc_tcache.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/port.h ../mach/time_value.h \
  ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../mach/machine/vm_types.h ../kern/macro_help.h ../kern/mach_param.h \
  ./mach_np.h ../mach/mach_param.h ../kern/kern_obj.h ../kern/zalloc.h \
  ../kern/assert.h ./mach_assert.h ../kern/kern_set.h ../mach/kern_return.h \
  ../mach/machine/kern_return.h ../kern/msg_queue.h ../kern/processor.h \
  ./mach_fixpri.h ../kern/sched.h ./simple_clock.h ./stat_time.h \
  ../kern/syscall_emulation.h ../vm/vm_map.h ../mach/vm_prot.h \
  ../mach/vm_inherit.h ../vm/pmap.h ../machine/pmap.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../machine/psl.h ../mac2/endian.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../kern/thread.h ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h \
  ./hw_ast.h ../machine/ast.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../vm/vm_page.h ../kern/sched_prim.h \
  ../kern/parallel.h ../sys/param.h ../machine/machparam.h ../sys/signal.h \
  ../sys/systm.h ../sys/dir.h ../sys/user.h ./mach_rfs.h ./vice.h \
  ../sys/time.h ../sys/resource.h ../sys/namei.h ../sys/uio.h ../sys/errno.h \
  ./mach_nfs.h ../sys/proc.h ../sys/vm.h ../sys/vmparam.h \
  ../machine/vmparam.h ../sys/vmmac.h ../sys/file.h ../sys/inode.h \
  ./mach_vfs.h ./mach_fastlink.h ../vfs/inode_macros.h ../vfs/vnode.h \
  ../vfs/vfs.h ../vfs/pathname.h ../sys/buf.h ../sys/mount.h ../sys/trace.h \
  ../sys/map.h ../sys/kernel.h ../vm/vm_kern.h ../kern/ipc_pobj.h \
  ../kern/host.h ../kern/port_object.h ./mach_net.h ../kern/ipc_copyout.h 
# Dependencies for File: vm_user.o:
vm_user.o: ../vm/vm_user.c ../sys/types.h ../mach/machine/vm_types.h \
  ../mach/vm_param.h ../mach/machine/vm_param.h ../mac2/machparam.h \
  ../mach/mac2/vm_param.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../vm/vm_object.h ./mach_pagemap.h \
  ../mach/kern_return.h ../mach/machine/kern_return.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../mach/memory_object.h ../mach/port.h \
  ./mach_ipc_xxxhack.h ../kern/queue.h ../kern/lock.h ./cpus.h \
  ./mach_ldebug.h ../kern/macro_help.h ../kern/assert.h ./mach_assert.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_page.h ../kern/thread.h ./mach_fixpri.h \
  ./mach_host.h ./mach_km.h ../mach/message.h ../kern/ast.h ./hw_ast.h \
  ../machine/ast.h ../kern/processor.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/zalloc.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/syscall_emulation.h ../kern/timer.h ../kern/thread_modes.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h \
  ../machine/thread.h ../machine/pcb.h ../kern/sched_prim.h \
  ../mach/std_types.h ../mach/mach_types.h ../mach/vm_attributes.h \
  ../mach/machine.h ../mach/thread_status.h ../mach/machine/thread_status.h \
  ../mach/thread_info.h ../mach/policy.h ../mach/thread_special_ports.h \
  ../mach/task_info.h ../mach/task_special_ports.h ../mach/host_info.h \
  ../mach/processor_info.h ../kern/kern_mon.h ../sys/time.h ../vm/vm_kern.h \
  ../mach/kernel_event.h ../vm/vm_user.h ../kern/host.h ../kern/ipc_globals.h \
  ../mach/notify.h ../kern/ipc_hash.h 
# Dependencies for File: xdr.o:
xdr.o: ../rpc/xdr.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../rpc/types.h ../sys/time.h \
  ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h ../rpc/xdr.h 
# Dependencies for File: xdr_array.o:
xdr_array.o: ../rpc/xdr_array.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../sys/systm.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../rpc/types.h ../sys/time.h \
  ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../kern/queue.h \
  ../kern/macro_help.h ../rpc/xdr.h ../netinet/in.h ../machine/endian.h 
# Dependencies for File: xdr_mem.o:
xdr_mem.o: ../rpc/xdr_mem.c ../sys/param.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/machparam.h ../mach/mac2/vm_param.h \
  ../mac2/machparam.h ../machine/cpu_inline.c ../machine/cpu.h \
  ../machine/psl.h ../mac2/endian.h ../sys/signal.h ../rpc/types.h \
  ../sys/time.h ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h \
  ../mach/boolean.h ../mach/machine/boolean.h ../kern/queue.h \
  ../kern/macro_help.h ../rpc/xdr.h ../netinet/in.h ../machine/endian.h 
# Dependencies for File: zalloc.o:
zalloc.o: ../kern/zalloc.c ./mach_debug.h ../mach/vm_param.h \
  ../mach/machine/vm_param.h ../mac2/machparam.h ../mach/mac2/vm_param.h \
  ../machine/cpu_inline.c ../machine/cpu.h ../sys/types.h \
  ../mach/machine/vm_types.h ../machine/psl.h ../mac2/endian.h \
  ../kern/zalloc.h ../kern/lock.h ./cpus.h ./mach_ldebug.h ../mach/boolean.h \
  ../mach/machine/boolean.h ../kern/queue.h ../kern/macro_help.h \
  ../vm/vm_kern.h ../mach/kern_return.h ../mach/machine/kern_return.h \
  ../vm/vm_map.h ../mach/vm_prot.h ../mach/vm_inherit.h ../vm/pmap.h \
  ../machine/pmap.h ../mach/vm_statistics.h ../mac2/pcb.h \
  ../mach/mac2/frame.h ../mach/mac2/reg.h ../mac2/pmmu_inline.c \
  ./macos_emul.h ../vm/vm_object.h ./mach_pagemap.h ../mach/memory_object.h \
  ../mach/port.h ./mach_ipc_xxxhack.h ../kern/assert.h ./mach_assert.h \
  ../mach_debug/zone_info.h ../kern/ipc_globals.h ../mach/notify.h \
  ../mach/message.h ../kern/task.h ./mach_emulation.h ./mach_ipc_tcache.h \
  ../mach/time_value.h ../kern/mach_param.h ./mach_np.h ../mach/mach_param.h \
  ../kern/kern_obj.h ../kern/kern_set.h ../kern/msg_queue.h \
  ../kern/processor.h ./mach_fixpri.h ../kern/sched.h ./simple_clock.h \
  ./stat_time.h ../kern/syscall_emulation.h ../kern/ipc_hash.h \
  ../kern/kern_msg.h ../kern/ipc_netport.h ../mach/netport.h ../vm/vm_user.h 
