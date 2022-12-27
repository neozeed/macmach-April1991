# /usr/src/old/vpr/vtools/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	fcvt.c rotate.c rotprt.c vfontinfo.c vfw.c vwidth.c
OBJS=	fcvt.o rotate.o rotprt.o vfontinfo.o vfw.o vwidth.o
STD=	fcvt rotate rotprt vfontinfo vfw vwidth

all:	${STD} .depend

fcvt: ${LIBC}
	${CC} -o $@ ${CFLAGS} fcvt.c

rotate: ${LIBC}
	${CC} -o $@ ${CFLAGS} rotate.c

rotprt: ${LIBC}
	${CC} -o $@ ${CFLAGS} rotprt.c

vfontinfo: ${LIBC}
	${CC} -o $@ ${CFLAGS} vfontinfo.c

vfw: ${LIBC}
	${CC} -o $@ ${CFLAGS} vfw.c

vwidth: ${LIBC}
	${CC} -o $@ ${CFLAGS} vwidth.c

install:	FRC
	install -c -s -o bin -g bin -m 755 fcvt ${DESTDIR}/usr/old/fcvt
	install -c -s -o bin -g bin -m 755 rotate ${DESTDIR}/usr/old/rotate
	install -c -s -o bin -g bin -m 755 rotprt ${DESTDIR}/usr/old/rotprt
	install -c -s -o bin -g bin -m 755 vfontinfo ${DESTDIR}/usr/old/vfontinfo
	install -c -s -o bin -g bin -m 755 vfw ${DESTDIR}/usr/old/vfw
	install -c -s -o bin -g bin -m 755 vwdith ${DESTDIR}/usr/old/vwdith

clean:	FRC
	rm -f .depend ${OBJS} core ${STD}

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
