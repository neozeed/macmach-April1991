# /usr/src/usr.bin/dc/Makefile for MacMach

CFLAGS=	-O -I.
LIBC=	/lib/libc.a
SRCS=	dc.c
OBJS=

all:	dc .depend

dc:	dc.c ${LIBC}
	${CC} ${CFLAGS} dc.c -o $@

install: FRC
	install -c -s -o bin -g bin -m 755 dc ${DESTDIR}/usr/bin/dc

clean:	FRC
	rm -f .depend ${OBJS} core dc

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
