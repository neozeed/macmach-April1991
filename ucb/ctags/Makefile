# /usr/src/ucb/ctags/Makefile for MacMach

DESTDIR=
CFLAGS=-O -I.
LIBC=	/lib/libc.a
SRCS=	C.c ctags.c fortran.c lisp.c print.c tree.c yacc.c
OBJS=	C.o ctags.o fortran.o lisp.o print.o tree.o yacc.o

all:	ctags .depend

ctags: ${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 ctags ${DESTDIR}/usr/ucb/ctags

clean:	FRC
	rm -f .depend ${OBJS} core ctags

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
