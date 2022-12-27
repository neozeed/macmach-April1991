# /usr/src/usr.bin/ratfor/Makefile for MacMach

CFLAGS=	-O
YFLAGS=	-d
LIBC=	/lib/libc.a
SRCS=	r.c r0.c r1.c r2.c rio.c rlook.c rlex.c
OBJS=	r.o r0.o r1.o r2.o rio.o rlook.o rlex.o

all:	ratfor .depend

ratfor:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 ratfor ${DESTDIR}/usr/bin/ratfor

clean:	FRC
	rm -f .depend ${OBJS} core ratfor r.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
