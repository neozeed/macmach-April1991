# /usr/src/etc/envmgr/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	envmgrmain.c envprocs.c envmgrServer.c
OBJS=	envmgrmain.o envprocs.o envmgrServer.o

all:	envmgr .depend

envmgr:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -lmach

envmgrServer.c envmgr.h: env_mgr.defs
	mig $? -header envmgr.h -server envmgrServer.c -user /dev/null

install: FRC
	install -c -s -o bin -g bin -m 755 envmgr ${DESTDIR}/usr/etc/envmgr
	rm -f ${DESTDIR}/etc/envmgr
	ln -s ../usr/etc/envmgr ${DESTDIR}/etc/envmgr

clean:	FRC
	rm -f .depend ${OBJS} envmgr envmgr.h envmgrServer.c core

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
