# /usr/src/usr.bin/xsend/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	enroll.c lib.c xget.c xsend.c
OBJS=	enroll.o lib.o xget.o xsend.o

all:	enroll xsend xget .depend

enroll: enroll.o lib.o ${LIBC}
	${CC} enroll.o lib.o -lmp -o $@

xget: xget.o lib.o ${LIBC}
	${CC} xget.o lib.o -lmp -o $@

xsend: xsend.o lib.o ${LIBC}
	${CC} xsend.o lib.o -lmp -o $@

install: FRC
	install -c -s -o bin -g bin -m 755 enroll ${DESTDIR}/usr/bin/enroll
	install -c -s -o bin -g bin -m 755 xget ${DESTDIR}/usr/bin/xget
	install -c -s -o bin -g bin -m 755 xsend ${DESTDIR}/usr/bin/xsend

clean:	FRC
	rm -f .depend ${OBJS} core enroll xsend xget

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
