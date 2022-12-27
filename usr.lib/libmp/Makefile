# /usr/src/usr.lib/libmp/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	pow.c gcd.c msqrt.c mdiv.c mout.c mult.c madd.c util.c
OBJS=	pow.o gcd.o msqrt.o mdiv.o mout.o mult.o madd.o util.o

all:	libmp.a .depend

libmp.a: ${OBJS}
	ar cr $@ ${OBJS}

install: FRC
	install -c -o bin -g bin -m 644 libmp.a ${DESTDIR}/usr/lib/libmp.a
	ranlib ${DESTDIR}/usr/lib/libmp.a

clean:	FRC
	rm -f .depend ${OBJS} core libmp.a

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
