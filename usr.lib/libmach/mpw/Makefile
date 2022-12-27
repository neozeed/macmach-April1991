# gather the MACH sources to be used by MPW

# mach_traps.a must be made by hand from /usr/src/libmach/mach_traps.S
# this is essentially the result of doing "cc -E mach_traps.S", removing
# the "_" prefixes and replacing ".globl" with "PROC EXPORT ... ENDPROC"
# make sure that the lower case identifiers are exported as such

# be sure to change all of the "/" in includes to ":", with initial ":"

# SRC is the root of the system source directory
SRC=	..

# MACH is a list of mach includes normally found in /usr/include
MACH=	${SRC}/include/mach.h \
	${SRC}/include/mach_error.h \
	${SRC}/include/mach_exception.h \
	${SRC}/include/mach_init.h \
	${SRC}/include/exception_all.h \
	${SRC}/sys/mach/mig_errors.h \
	mach_interface.h
# NOTE: mig and libmach also use stdio.h, strings.h and varargs.h

# SYS is a list of include files normally found in /usr/include/sys
SYS=	${SRC}/sys/sys/syscall.h \
	${SRC}/sys/sys/types.h

# MAC2 is a list of include files normally found in /usr/include/mac2
MAC2=	${SRC}/sys/mac2/machparam.h \
	${SRC}/sys/mac2/endian.h

# SERVERS is a list of mach includes normally found in /usr/include/servers
SERVERS= ${SRC}/usr.lib/libmach/errorlib.h \
	${SRC}/etc/netmsgserver/server/netname_defs.h \
	${SRC}/etc/netmemoryserver/netmemory_defs.h \
	${SRC}/etc/envmgr/emdefs.h \
	${SRC}/etc/ipcexecd/ipcx_types.h \
	netname.h \
	env_mgr.h \
	service.h \
	netmemory.h 

# MACHDIRS is a list of mach directories taken from the kernel sources
MACHDIRS=mach mach_debug mach/mac2 mach/sun3 mach/vax mach/ca

# LIBMACH is a list of the libmach sources
LIBMACH=${SRC}/usr.lib/libmach/mach_init.c \
	${SRC}/usr.lib/libmach/sbrk.c \
	${SRC}/usr.lib/libmach/error_codes.c \
	${SRC}/usr.lib/libmach/mach_error.c \
	${SRC}/usr.lib/libmach/Ovfork.c \
	${SRC}/usr.lib/libmach/slot_name.c \
	${SRC}/usr.lib/libmach/mig_support.c \
	${SRC}/usr.lib/libmach/mig_strncpy.c \
	${SRC}/usr.lib/libmach/msg.c \
	${SRC}/usr.lib/libmach/XXX_pdp.c \
	${SRC}/usr.lib/libmach/XXX_pds.c \
	${SRC}/usr.lib/libmach/XXX_pdu.c \
	${SRC}/usr.lib/libmach/XXX_plr.c \
	${SRC}/usr.lib/libmach/mach_exception.c \
	${SRC}/usr.lib/libmach/XXX_new_vawp.c \
	${SRC}/usr.lib/libmach/new_mach_user.c \
	${SRC}/usr.lib/libmach/err_kern.sub \
	${SRC}/usr.lib/libmach/err_us.sub \
	${SRC}/usr.lib/libmach/err_server.sub \
	${SRC}/usr.lib/libmach/err_ipc.sub \
	${SRC}/usr.lib/libmach/SYS.h \
	${SRC}/etc/netmemoryserver/netmemory_defs.h \
	${SRC}/etc/netmsgserver/server/netname_defs.h \
	${SRC}/lib/libc/gen/bcopy.c \
	env_mgrUser.c \
	ipcxUser.c \
	netnameUser.c \
	serviceUser.c \
	excUser.c \
	excServer.c \
	netmemoryUser.c \
	netmemstatUser.c \
	memory_objectServer.c \
	mach_user.c \
	mach_debugUser.c \
	monitorUser.c \
	mach_hostUser.c \
	mach_interface.h \
	mach_debug_interface.h \
	exc.h \
	ipcx.h \
	mach_host.h \
	monitor.h \
	netmemstat.h \
	env_mgr.h \
	netmemory.h \
	netname.h \
	service.h

# the default is to build everything
all:	include libmach
	echo "Be sure to change '/' to ':' and initial ':'."

libmach.mpw: libmach.sh
	sh libmach.sh >libmach.mpw

# gather all mach includes
include: FRC ${MACH} ${SERVERS}
	-[ -d include ] || mkdir include
	cp ${MACH} include/
	-[ -d include/sys ] || mkdir include/sys
	cp ${SYS} include/sys/
	-[ -d include/mac2 ] || mkdir include/mac2
	cp ${MAC2} include/mac2/
	-[ -d include/servers ] || mkdir include/servers
	cp ${SERVERS} include/servers/
	-for i in ${MACHDIRS}; do \
	  [ -d include/$$i ] || mkdir include/$$i; \
	  for j in ${SRC}/sys/$$i/*.[ihd]*; do \
	    cp $$j include/$$i; \
	  done; \
	done
	-[ -d include/machine ] || mkdir include/machine
	cp include/mac2/* include/machine/
	-[ -d include/mach/machine ] || mkdir include/mach/machine
	cp include/mach/mac2/* include/mach/machine/

# gather all libmach sources
libmach: FRC ${LIBMACH}
	-[ -d libmach ] || mkdir libmach
	cp ${LIBMACH} libmach/
#	cd libmach; cc -I../include -c *.c *.S

# install stuff to DESTDIR
install: FRC
	rm -f libmach/*.o
	-[ -d ${DESTDIR}/mpw_mach ] || mkdir ${DESTDIR}/mpw_mach
	find include libmach -print | cpio -pduv ${DESTDIR}/mpw_mach

# clean out everything than can be rebuilt
clean:	FRC
	rm -rf include libmach
	rm -f env_mgr.h env_mgrUser.c exc.h excServer.c excUser.c ipcx.h \
	  ipcxUser.c mach_debugUser.c \
	  mach_debug_interface.h mach_host.h mach_hostUser.c \
	  mach_interface.h mach_user.c memory_object.h memory_objectServer.c \
	  monitor.h monitorUser.c netmemory.h netmemoryUser.c \
	  netmemstat.h netmemstatUser.c netname.h netnameUser.c \
	  service.h serviceUser.c

# the following build source files using yacc, lex or mig as needed


mach_interface.h mach_user.c: ${SRC}/sys/mach/mach.defs
	mig -I${SRC}/sys $? \
	  -header mach_interface.h \
	  -server /dev/null \
	  -user mach_user.c

mach_debug_interface.h mach_debugUser.c: ${SRC}/sys/mach_debug/mach_debug.defs
	mig -I${SRC}/sys $? \
	  -header mach_debug_interface.h \
	  -server /dev/null \
	  -user mach_debugUser.c

service.h serviceUser.c: ${SRC}/etc/mach_init/service.defs
	mig -I${SRC}/sys $? \
	  -header service.h \
	  -server /dev/null \
	  -user serviceUser.c

exc.h excUser.c excServer.c: ${SRC}/sys/mach/exc.defs
	mig -I${SRC}/sys $? \
	  -header exc.h \
	  -server excServer.c \
	  -user excUser.c

netmemory.h netmemoryUser.c: ${SRC}/etc/netmemoryserver/netmemory.defs
	mig -I${SRC}/sys $? \
	  -header netmemory.h \
	  -server /dev/null \
	  -user netmemoryUser.c

netmemstat.h netmemstatUser.c: ${SRC}/etc/netmemoryserver/netmemstat.defs
	mig -I${SRC}/sys $? \
	  -header netmemstat.h \
	  -server /dev/null \
	  -user netmemstatUser.c

memory_object.h memory_objectServer.c: ${SRC}/usr.lib/libmach/memory_object.defs
	mig -I${SRC}/sys $? \
	  -header memory_object.h \
	  -server memory_objectServer.c \
	  -user /dev/null

netname.h netnameUser.c: ${SRC}/etc/netmsgserver/server/netname.defs
	mig -I${SRC}/sys $? \
	  -header netname.h \
	  -server /dev/null \
	  -user netnameUser.c

env_mgr.h env_mgrUser.c: ${SRC}/etc/envmgr/env_mgr.defs
	mig -I${SRC}/sys $? \
	  -header env_mgr.h \
	  -server /dev/null \
	  -user env_mgrUser.c

ipcx.h ipcxUser.c: ${SRC}/etc/ipcexecd/ipcx.defs
	mig -I${SRC}/sys $? \
	  -header ipcx.h \
	  -server /dev/null \
	  -user ipcxUser.c

mach_host.h mach_hostUser.c: ${SRC}/sys/mach/mach_host.defs
	mig -I${SRC}/sys $? \
	  -header mach_host.h \
	  -server /dev/null \
	  -user mach_hostUser.c

monitor.h monitorUser.c: ${SRC}/sys/mach/monitor.defs
	mig -I${SRC}/sys $? \
	  -header monitor.h \
	  -server /dev/null \
	  -user monitorUser.c

FRC:
