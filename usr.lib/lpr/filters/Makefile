# /usr/src/usr.lib/lpr/filters/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a
LIBDIR=	/usr/lib
SRCS=	lpf.c
OBJS=	lpf.o

all:	lpf .depend

lpf:	lpf.c
	${CC} -o $@ ${CFLAGS} lpf.c

install: FRC
	install -c -s -o bin -g bin -m 755 lpf ${DESTDIR}/${LIBDIR}/lpf

clean:	FRC
	rm -f .depend ${OBJS} core lpf

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
