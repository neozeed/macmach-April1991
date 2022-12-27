# /usr/src/usr.lib/libtermlib/Makefile for MacMach

CFLAGS=	-O -DCM_N -DCM_GT -DCM_B -DCM_D
LIBC=	/lib/libc.a
SRCS=	termcap.c tgoto.c tputs.c
OBJS=	termcap.o tgoto.o tputs.o

all:	termcap.a termcap_p.a .depend

termcap.a termcap_p.a: ${OBJS}
	ar cr termcap.a ${OBJS}
	cd profiled; ar cr ../termcap_p.a ${OBJS}

test:	FRC
	ranlib termcap.a
	${CC} ${CFLAGS} -o tc1 tc1.c termcap.a
	${CC} ${CFLAGS} -o tc2 tc2.c termcap.a
	${CC} ${CFLAGS} -o tc3 tc3.c termcap.a

.c.o:
	${CC} ${CFLAGS} -c -p $*.c
	ld -x -r -o profiled/$*.o $*.o
	${CC} ${CFLAGS} -c $*.c
	ld -X -r $*.o
	mv a.out $*.o

install: FRC
	install -c -o bin -g bin -m 644 termcap.a ${DESTDIR}/usr/lib/libtermcap.a
	rm -f ${DESTDIR}/usr/lib/libtermlib.a
	ln -s libtermcap.a ${DESTDIR}/usr/lib/libtermlib.a
	ranlib ${DESTDIR}/usr/lib/libtermcap.a
	install -c -o bin -g bin -m 644 termcap_p.a ${DESTDIR}/usr/lib/libtermcap_p.a
	rm -f ${DESTDIR}/usr/lib/libtermlib_p.a
	ln -s libtermcap_p.a ${DESTDIR}/usr/lib/libtermlib_p.a
	ranlib ${DESTDIR}/usr/lib/libtermcap_p.a

clean:	FRC
	rm -f .depend ${OBJS} core profiled/*.o termcap.a termcap_p.a

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
