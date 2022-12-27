# /usr/src/usr.bin/lex/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	lmain.c parser.c sub1.c sub2.c header.c
OBJS=	lmain.o parser.o sub1.o sub2.o header.o

all:	lex .depend

lex:	${OBJS} ${LIBC}
	${CC} ${CFLAGS} ${OBJS} -o $@

smallex: ${SRCS} ${LIBC}
	${CC} -DSMALL ${CFLAGS} ${SRCS} -o $@

install: FRC
	-[ -d ${DESTDIR}/usr/lib/lex ] || mkdir ${DESTDIR}/usr/lib/lex
	-chmod 755 ${DESTDIR}/usr/lib/lex
	-chown root.bin ${DESTDIR}/usr/lib/lex
	install -c -s -o bin -g bin -m 755 lex ${DESTDIR}/usr/bin/lex
	install -c -o bin -g bin -m 755 ncform ${DESTDIR}/usr/lib/lex/ncform
	install -c -o bin -g bin -m 755 nrform ${DESTDIR}/usr/lib/lex/nrform

clean:	FRC
	rm -f .depend ${OBJS} core lex smallex parser.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
