# /usr/src/bin/sed/Makefile for MacMach

CFLAGS=	-O -w
LIBC=	/lib/libc.a
SRCS=	sed0.c sed1.c
OBJS=	sed0.o sed1.o

all:	sed .depend

sed:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 sed ${DESTDIR}/bin/sed

clean:	FRC
	rm -f .depend ${OBJS} core sed

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
