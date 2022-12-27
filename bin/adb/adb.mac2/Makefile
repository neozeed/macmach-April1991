# /usr/src/bin/adb/adb.mac2/Makefile for MacMach

CFLAGS=	-O -DM68881 -DVFORK
LIBC=	/lib/libc.a
SRCS=	access.c command.c expr.c format.c input.c main.c message.c \
	opset.c optab.c output.c pcs.c print.c runpcs.c setup.c sym.c trace.c
OBJS=	access.o command.o expr.o format.o input.o main.o message.o \
	opset.o optab.o output.o pcs.o print.o runpcs.o setup.o sym.o trace.o

all:	adb .depend

adb:	${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

message.o:
	${CC} -R ${CFLAGS} -c message.c

optab.o:
	${CC} -R ${CFLAGS} -c optab.c

install: FRC
	install -c -s -o bin -g bin -m 755 adb ${DESTDIR}/bin/adb

clean:	FRC
	rm -f .depend ${OBJS} core adb

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
