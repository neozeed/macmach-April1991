# /usr/src/etc/netmemoryserver/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	netmemory.c object.c scheduler.c \
	netmemory_aux.c netname_aux.c \
	internalUser.c internalServer.c \
	netmemoryServer.c \
	netmemstatServer.c
MIG=	internal.h internalUser.c internalServer.c \
	netmemory.h netmemoryServer.c \
	netmemstat.h netmemstatServer.c
OBJS=	netmemory.o object.o scheduler.o \
	netmemory_aux.o netname_aux.o \
	internalUser.o internalServer.o \
	netmemoryServer.o netmemstatServer.o

all:	netmemoryserver .depend

netmemoryserver: ${MIG} ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -lthreads -lmach

internalUser.c internalServer.c internal.h: internal.defs
	mig $? -header internal.h -server internalServer.c -user internalUser.c

netmemoryServer.c netmemory.h: netmemory.defs
	mig $? -header netmemory.h -server netmemoryServer.c -user /dev/null

netmemstatServer.c netmemstat.h: netmemstat.defs
	mig $? -header netmemstat.h -server netmemstatServer.c -user /dev/null

install: FRC
	install -c -s -o bin -g bin -m 755 netmemoryserver ${DESTDIR}/usr/etc/netmemoryserver
	rm -f ${DESTDIR}/etc/netmemoryserver
	ln -s ../usr/etc/netmemoryserver ${DESTDIR}/etc/netmemoryserver

clean:	FRC
	rm -f .depend ${OBJS} netmemoryserver core ${MIG}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
