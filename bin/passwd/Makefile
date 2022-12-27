# /usr/src/bin/passwd/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	passwd.c

all:	passwd .depend

passwd: ${LIBC}
	${CC} -o $@ ${CFLAGS} ${SRCS}

install: FRC
	install -c -s -o root -g bin -m 4755 passwd ${DESTDIR}/bin/passwd
	rm -f ${DESTDIR}/bin/chfn; ln -s passwd ${DESTDIR}/bin/chfn
	rm -f ${DESTDIR}/bin/chsh; ln -s passwd ${DESTDIR}/bin/chsh

clean:	FRC
	rm -f .depend core passwd

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
