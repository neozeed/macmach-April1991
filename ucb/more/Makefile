# /usr/src/ucb/more/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	more.c
OBJS=	more.o

all:	more .depend

more:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -ltermcap

install: FRC
	install -c -s -o bin -g bin -m 755 more ${DESTDIR}/usr/ucb/more
	rm -f ${DESTDIR}/usr/ucb/page
	ln -s more ${DESTDIR}/usr/ucb/page
	install -c -o bin -g bin -m 644 more.help ${DESTDIR}/usr/lib/more.help

clean:	FRC
	rm -f .depend ${OBJS} core more

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
