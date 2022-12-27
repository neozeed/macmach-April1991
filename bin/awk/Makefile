# /usr/src/bin/awk/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
YFLAGS=	-d
OBJS=	awk.lx.o b.o main.o token.o tran.o lib.o run.o parse.o proctab.o \
	freeze.o
SRCS=	awk.lx.c b.c lib.c main.c parse.c proctab.c freeze.c run.c \
	token.c tran.c

all:	awk .depend

awk:	awk.h ${OBJS} awk.g.o ${LIBC}
	${CC} ${CFLAGS} awk.g.o  ${OBJS} -lm -o $@

y.tab.h: awk.g.o

awk.h:	y.tab.h
	-cmp -s y.tab.h awk.h || cp y.tab.h awk.h

token.c: awk.h
	ex - < tokenscript

profile: awk.g.o ${OBJS} mon.o
	${CC} -p -i awk.g.o ${OBJS} mon.o -lm

proctab.c: proc
	./proc > proctab.c

proc:	awk.h proc.o token.o
	${CC} -o $@ proc.o token.o

install: FRC
	install -c -s -o bin -g bin -m 755 awk ${DESTDIR}/bin/awk

clean:	FRC
	rm -f .depend *.o core awk
	rm -f awk.h awk.lx.c y.tab.h proc proctab.c yacc* temp

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
