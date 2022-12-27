# /usr/src/etc/ipcexecd/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	main.c assert.c error.c patch.c printmsg.c procs.c ipcxServer.c
OBJS=	main.o assert.o error.o patch.o printmsg.o procs.o ipcxServer.o

all:	ipcexecd .depend

ipcexecd: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -lmach

ipcxServer.c ipcx.h: ipcx.defs
	mig $? -header ipcx.h -server ipcxServer.c -user /dev/null

install: FRC
	install -c -s -o bin -g bin -m 755 ipcexecd ${DESTDIR}/usr/etc/ipcexecd
	rm -f ${DESTDIR}/etc/ipcexecd
	ln -s ../usr/etc/ipcexecd ${DESTDIR}/etc/ipcexecd

clean:	FRC
	rm -f .depend ${OBJS} ipcexecd ipcx.h ipcxServer.c core

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
