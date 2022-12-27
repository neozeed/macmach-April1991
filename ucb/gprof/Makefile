# /usr/src/ucb/gprof/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
HDRS=	gprof.h ${MACHINE}.h
SRCS=	gprof.c arcs.c dfn.c lookup.c ${MACHINE}.c hertz.c \
	printgprof.c printlist.c
OBJS=	gprof.o arcs.o dfn.o lookup.o ${MACHINE}.o hertz.o \
	printgprof.o printlist.o

all:	gprof .depend

gprof:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

gcrt0.h:
	-if [ -r gcrt0.h ] && \
	  cmp -s gcrt0.h ../../lib/libc/${MACHINE}/csu/gmon.h; then \
	  :; \
	else \
	  rm -f gcrt0.h; \
	  cp ../../lib/libc/${MACHINE}/csu/gmon.h gcrt0.h; \
	fi

install: FRC
	install -c -s -o bin -g bin -m 755 gprof ${DESTDIR}/usr/ucb/gprof
	install -c -o bin -g bin -m 444 gprof.flat ${DESTDIR}/usr/lib
	install -c -o bin -g bin -m 444 gprof.callg ${DESTDIR}/usr/lib

clean:	FRC
	rm -f .depend ${OBJS} core gprof

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
