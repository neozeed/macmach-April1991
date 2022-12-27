# /usr/src/usr.bin/eqn/eqn/Makefile for MacMach

COMMON=	../common_source
CFLAGS=	-O -I. -I${COMMON}
YFLAGS=	-d
VPATH=	${COMMON}
LIBC=	/lib/libc.a
SRCS=	e.c diacrit.c eqnbox.c font.c fromto.c funny.c glob.c integral.c \
	io.c lex.c lookup.c mark.c matrix.c move.c over.c paren.c pile.c \
	shift.c size.c sqrt.c text.c
OBJS=	e.o diacrit.o eqnbox.o font.o fromto.o funny.o glob.o integral.o \
	io.o lex.o lookup.o mark.o matrix.o move.o over.o paren.o pile.o \
	shift.o size.o sqrt.o text.o

all:	eqn .depend

eqn: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

e.c: ${COMMON}/e.y
	${YACC} ${YFLAGS} ${COMMON}/e.y
	mv y.tab.h e.def
	mv y.tab.c e.c

install: FRC
	install -c -s -o bin -g bin -m 755 eqn ${DESTDIR}/usr/bin/eqn

clean:	FRC
	rm -f .depend ${OBJS} core e.def e.c eqn

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
