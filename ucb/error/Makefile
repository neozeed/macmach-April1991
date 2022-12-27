# /usr/src/ucb/error/Makefile for MacMach

CFLAGS=	-O -DDEBUG
LIBC=	/lib/libc.a
SRCS=	errormain.c errorinput.c errorpi.c errorsubr.c errorfilter.c \
	errortouch.c
OBJS=	errormain.o errorinput.o errorpi.o errorsubr.o errorfilter.o \
	errortouch.o

all:	error .depend

error:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 error ${DESTDIR}/usr/ucb/error

clean:	FRC
	rm -f .depend ${OBJS} core error

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
