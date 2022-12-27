# /usr/src/bin/make/Makefile for MacMach

CFLAGS=	-O -DASCARCH
LIBC=	/lib/libc.a
SRCS=	main.c doname.c misc.c files.c dirs.c libs.c rules.c \
	dosys.c gram.c dyndep.c
OBJS=	main.o doname.o misc.o files.o dirs.o libs.o rules.o \
	dosys.o gram.o dyndep.o

all:	make .depend

make:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

gram.c:	gram.y
	yacc gram.y
	mv y.tab.c gram.c

install: FRC
	install -c -s -o bin -g bin -m 755 make ${DESTDIR}/bin/make

clean:	FRC
	rm -f .depend ${OBJS} core make gram.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} gram.c ${SRCS}

-include .depend

FRC:
