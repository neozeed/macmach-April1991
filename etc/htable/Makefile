# /usr/src/etc/htable/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	htable.c parse.c scan.c
OBJS=	htable.o parse.o scan.o

all:	htable .depend

htable: y.tab.h ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

y.tab.h:
	yacc -d parse.y
	rm -f y.tab.c

install: FRC
	rm -f ${DESTDIR}/etc/htable ${DESTDIR}/usr/etc/htable
	install -c -s -o bin -g bin -m 755 htable ${DESTDIR}/usr/etc/htable
	ln -s ../usr/etc/htable ${DESTDIR}/etc/htable

clean:	FRC
	rm -f .depend ${OBJS} core htable parse.c scan.c y.tab.h

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
