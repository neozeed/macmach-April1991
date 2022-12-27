# /usr/src/etc/talkd/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	talkd.c announce.c process.c table.c print.c
OBJS=	talkd.o announce.o process.o table.o print.o

all:	talkd .depend

talkd:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	rm -f ${DESTDIR}/etc/ntalkd ${DESTDIR}/usr/etc/ntalkd
	install -c -s -o bin -g bin -m 755 talkd ${DESTDIR}/usr/etc/ntalkd
	ln -s ../usr/etc/ntalkd ${DESTDIR}/etc/ntalkd

clean:	FRC
	rm -f .depend ${OBJS} core talkd

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
