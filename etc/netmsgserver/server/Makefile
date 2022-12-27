# /usr/src/etc/netmsgserver/server/Makefile for MacMach

CFLAGS=	-O -I../conf
LIBC=	/lib/libc.a
SRCS=	nmserver.c cam_stuff.c crypt.c crypt_des.c \
	crypt_multperm.c crypt_newdes.c crypt_xor.c \
	datagram.c deltat.c deltat_handler.c \
	deltat_send.c deltat_utils.c des.c \
	dispatcher.c ipc.c ipc_block.c ipc_swap.c \
	keyman.c km_procs.c km_utils.c lock_queue.c \
	logstat.c ls_rec.c mem.c multperm.c netipc.c \
	netname.c network.c nm_enable.c nm_extra.c \
	nm_init.c nn_handler.c nn_procs.c pc_handler.c \
	pc_startup.c po_message.c po_notify.c \
	po_utils.c portcheck.c portops.c portrec.c \
	portsearch.c ps_auth.c rwlock.c sbuf.c \
	srr.c srr_handler.c srr_utils.c timer.c \
	transport.c uid.c netnameServer.c logstatServer.c \
	ipc_exception.c ipc_msg.c ipc_rpc.c tcp.c vmtp2.c \
	ipc_in.c ipc_out.c vmtp1.c
MIG=	netnameServer.c netname.h \
	logstatServer.c logstat.h
RPCMOD_OBJS= ipc_exception.o ipc_msg.o ipc_rpc.o tcp.o vmtp2.o
#RPCMOD_OBJS= ipc_in.o ipc_out.o vmtp1.o
OBJS=	nmserver.o cam_stuff.o crypt.o crypt_des.o \
	crypt_multperm.o crypt_newdes.o crypt_xor.o \
	datagram.o deltat.o deltat_handler.o \
	deltat_send.o deltat_utils.o des.o \
	dispatcher.o ipc.o ipc_block.o ipc_swap.o \
	keyman.o km_procs.o km_utils.o lock_queue.o \
	logstat.o ls_rec.o mem.o multperm.o netipc.o \
	netname.o network.o nm_enable.o nm_extra.o \
	nm_init.o nn_handler.o nn_procs.o pc_handler.o \
	pc_startup.o po_message.o po_notify.o \
	po_utils.o portcheck.o portops.o portrec.o \
	portsearch.o ps_auth.o rwlock.o sbuf.o \
	srr.o srr_handler.o srr_utils.o timer.o \
	transport.o uid.o netnameServer.o logstatServer.o \
	${RPCMOD_OBJS}

all:	netmsgserver .depend

netmsgserver: ${MIG} ${OBJS} ${LIBC} ../lib/libvmtp.a
	${CC} -o $@ ${CFLAGS} ${OBJS} ../lib/libvmtp.a -lthreads -lmach

netnameServer.c netname.h: netname.defs
	mig $? -header netname.h -server netnameServer.c -user /dev/null

logstatServer.c logstat.h: logstat.defs
	mig $? -header logstat.h -server logstatServer.c -user /dev/null

install: FRC
	install -c -s -o bin -g bin -m 755 netmsgserver ${DESTDIR}/usr/etc/netmsgserver
	rm -f ${DESTDIR}/etc/netmsgserver
	ln -s ../usr/etc/netmsgserver ${DESTDIR}/etc/netmsgserver

clean:	FRC
	rm -f .depend ${OBJS} netmsgserver core ${MIG}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
