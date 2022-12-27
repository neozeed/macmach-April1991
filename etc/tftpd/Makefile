# /usr/src/etc/tftpd/Makefile for MacMach

VPATH=	../../ucb/tftp
CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	tftpd.c tftpsubs.c
OBJS=	tftpd.o tftpsubs.o

all:	tftpd .depend

tftpd:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	rm -f ${DESTDIR}/etc/tftpd ${DESTDIR}/usr/etc/tftpd
	install -c -s -o bin -g bin -m 755 tftpd ${DESTDIR}/usr/etc/tftpd
	ln -s ../usr/etc/tftpd ${DESTDIR}/etc/tftpd

clean:	FRC
	rm -f .depend ${OBJS} core tftpd

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
