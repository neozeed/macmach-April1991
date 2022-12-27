# /usr/src/usr.lib/libmach/Makefile for MacMach

# /usr/src/usr.lib/libmach/Makefile for mac2
#
CFLAGS=	-O

SRCS=	mach_init.c sbrk.c error_codes.c \
	mach_error.c Ovfork.c slot_name.c \
	mig_support.c mig_strncpy.c \
	msg.c XXX_pdp.c XXX_pds.c XXX_pdu.c \
	XXX_plr.c

OBJS=	mach_traps.o mach_init.o sbrk.o error_codes.o \
	mach_error.o fork.o Ovfork.o slot_name.o \
	mig_support.o mig_strncpy.o \
	msg.o mach_except.o XXX_n_vawp.o n_mach_user.o \
	XXX_pdp.o XXX_pds.o XXX_pdu.o XXX_plr.o \
	env_mgrUser.o ipcxUser.o netnameUser.o \
	serviceUser.o excUser.o excServer.o \
	netmemUser.o netmemsUser.o memobjServ.o \
	machdbgUser.o monitorUser.o mach_hostUser.o

all:	libmach libmach_p .depend

SHORTNAMES		= mach_except.c XXX_n_vawp.c n_mach_user.c \
			  netmemUser.c netmemsUser.c memobjServ.c \
			  machdbgUser.c
mach_except.c_LONGNAME	= mach_exception.c
XXX_n_vawp.c_LONGNAME	= XXX_new_vawp.c
n_mach_user.c_LONGNAME	= new_mach_user.c
netmemUser.c_LONGNAME	= netmemoryUser.c
netmemsUser.c_LONGNAME	= netmemstatUser.c
memobjServ.c_LONGNAME	= memory_objectServer.c
machdbgUser.c_LONGNAME	= mach_debugUser.c

${SHORTNAMES}: $${$$@_LONGNAME}
	rm -f $@; cp $? $@

libmach libmach_p: profiled ${OBJS}
	@echo building profiled libmach
	@cd profiled; ar cu ../libmach_p ${OBJS}
	ranlib libmach_p
	@echo building normal libmach
	@ar cu libmach ${OBJS}
	ranlib libmach

profiled: FRC
	-[ -d profiled ] || mkdir profiled

.S.o:
	@cc -c -DPROF ${CFLAGS} $*.S
	@ld -x -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.S
	@ld -x -r $*.o
	@mv a.out $*.o

.c.o:
	@cc -c -pg ${CFLAGS} $*.c
	@ld -x -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@ld -x -r $*.o
	@mv a.out $*.o

mach_interface.h mach_user.c: /usr/include/mach/mach.defs
	mig $? -header mach_interface.h -server /dev/null -user mach_user.c

mach_debug_interface.h mach_debugUser.c: /usr/include/mach_debug/mach_debug.defs
	mig $? -header mach_debug_interface.h -server /dev/null \
	  -user mach_debugUser.c

env_mgr.h env_mgrUser.c: ../../etc/envmgr/env_mgr.defs
	mig $? -header env_mgr.h -server /dev/null -user env_mgrUser.c

ipcx.h ipcxUser.c: ../../etc/ipcexecd/ipcx.defs
	mig $? -header ipcx.h -server /dev/null -user ipcxUser.c

netname.h netnameUser.c: ../../etc/netmsgserver/server/netname.defs
	mig $? -header netname.h -server /dev/null -user netnameUser.c

netname_defs.h:	../../etc/netmsgserver/server/netname_defs.h
	cp $? $@

service.h serviceUser.c: ../../etc/mach_init/service.defs
	mig $? -header service.h -server /dev/null -user serviceUser.c

exc.h excUser.c excServer.c: /usr/include/mach/exc.defs
	mig $? -header exc.h -server excServer.c -user excUser.c

netmemory.h netmemoryUser.c: ../../etc/netmemoryserver/netmemory.defs
	mig $? -header netmemory.h -server /dev/null -user netmemoryUser.c

netmemstat.h netmemstatUser.c: ../../etc/netmemoryserver/netmemstat.defs
	mig $? -header netmemstat.h -server /dev/null -user netmemstatUser.c

memory_object.h memory_objectServer.c: memory_object.defs
	mig $? -header memory_object.h -server memory_objectServer.c \
	  -user /dev/null

mach_host.h mach_hostUser.c: /usr/include/mach/mach_host.defs
	mig $? -header mach_host.h -server /dev/null -user mach_hostUser.c

monitor.h monitorUser.c: /usr/include/mach/monitor.defs
	mig $? -header monitor.h -server /dev/null -user monitorUser.c

netmemory_defs.h: ../../etc/netmemoryserver/netmemory_defs.h
	cp $? $@

netnameUser.o: netname_defs.h

netmemoryUser.o: netmemory_defs.h

excUser.o: netmemory_defs.h

error_codes.o: err_ipc.sub err_kern.sub err_server.sub err_us.sub

n_mach_user.o: mach_user.c

install: FRC
	install -c -o bin -g bin -m 644 libmach ${DESTDIR}/usr/lib/libmach.a
	ranlib ${DESTDIR}/usr/lib/libmach.a
	install -c -o bin -g bin -m 644 libmach_p ${DESTDIR}/usr/lib/libmach_p.a
	ranlib ${DESTDIR}/usr/lib/libmach_p.a

clean:	FRC
	rm -f .depend ${OBJS} libmach libmach_p ${SHORTNAMES}
	rm -rf profiled
	rm -f mach_user.c mach_interface.h
	rm -f mach_debugUser.c mach_debug_interface.h
	rm -f env_mgrUser.c env_mgr.h
	rm -f ipcx.h ipcxUser.c
	rm -f netname.h netnameUser.c netname_defs.h
	rm -f service.h serviceUser.c
	rm -f exc.h excUser.c excServer.c
	rm -f netmemory.h netmemoryUser.c netmemory_defs.h
	rm -f netmemstat.h netmemstatUser.c
	rm -f memory_object.h memory_objectServer.c
	rm -f mach_host.h mach_hostUser.c
	rm -f monitor.h monitorUser.c

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
