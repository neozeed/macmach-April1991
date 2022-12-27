# /usr/src/usr.bin/emul/server/Makefile for MacMach

DESTDIR=
CFLAGS=	-O -I. -I../common

SRCS=	catch_exception.c console.c debug.c input.c mac_user.c main.c \
	opset.c pager_mappings.c pager_routines.c \
	pager_support.c server.c signal.c util_server.c

OBJS=	catch_exception.o console.o debug.o input.o mac_user.o main.o \
	opset.o pager_mappings.o pager_routines.o \
	pager_support.o server.o signal.o util_server.o

all:	macserver .depend

macserver: kern mac2emul ${OBJS}
	cc -o macserver ${OBJS} -lthreads -lmach

mac_user.c mac.h: mac2emul/macserver.defs
	mig $? -header mac.h -user mac_user.c -server /dev/null

kern:	${SYS}/kern kern
	ln -s ${SYS}/kern

mac2emul: ${SYS}/mac2emul
	ln -s ${SYS}/mac2emul mac2emul

install: FRC
	install -c -s -o bin -g bin -m 755 macserver ${DESTDIR}/usr/bin

clean:	FRC
	rm -f .depend ${OBJS} core macserver \#* *~
	rm -f kern mac2emul mac_user.c mac.h

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
