# /usr/src/old/libom/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	asin.c atan.c hypot.c jn.c j0.c j1.c pow.c gamma.c \
	log.c sin.c sqrt.c tan.c tanh.c sinh.c erf.c exp.c floor.c
OBJS=	asin.o atan.o hypot.o jn.o j0.o j1.o pow.o gamma.o \
	log.o sin.o sqrt.o tan.o tanh.o sinh.o erf.o exp.o floor.o

all:	libom.a libom_p.a .depend

libom.a libom_p.a: ${OBJS}
	cd profiled; ar cru ../libom_p.a ${OBJS}
	ar cru libom.a ${OBJS}

.c.o:
	${CC} -p ${CFLAGS} -c $*.c
	-ld -X -r $*.o
	mv a.out profiled/$*.o
	${CC} ${CFLAGS} -c $*.c
	-ld -x -r $*.o
	mv a.out $*.o

install:	FRC
	rm -f ${DESTDIR}/usr/lib/libnm.a ${DESTDIR}/usr/lib/libnm_p.a
	install -c -o bin -g bin -m 644 libom.a ${DESTDIR}/usr/lib/libom.a
	ln -s libom.a ${DESTDIR}/usr/lib/libnm.a
	ranlib ${DESTDIR}/usr/lib/libom.a
	install -c -o bin -g bin -m 644 libom_p.a ${DESTDIR}/usr/lib/libom_p.a
	ln -s libom_p.a ${DESTDIR}/usr/lib/libnm_p.a
	ranlib ${DESTDIR}/usr/lib/libom_p.a

clean:	FRC
	rm -f .depend ${OBJS} core libom.a libom_p.a profiled/*

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
