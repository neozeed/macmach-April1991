# /usr/src/etc/getty/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	main.c init.c subr.c gettytab.c get_date.c
OBJS=	main.o init.o subr.o gettytab.o get_date.o

all:	getty .depend

getty:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 getty ${DESTDIR}/usr/etc/getty
	rm -f ${DESTDIR}/etc/getty
	ln -s ../usr/etc/getty ${DESTDIR}/etc/getty

clean:	FRC
	rm -f .depend ${OBJS} core getty

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
