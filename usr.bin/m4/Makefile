# /usr/src/usr.bin/m4/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	m4.c m4y.c
OBJS=	m4.o m4y.o

all:	m4 .depend

m4:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 m4 ${DESTDIR}/usr/bin/m4

clean:	FRC
	rm -f .depend ${OBJS} core m4 m4y.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
