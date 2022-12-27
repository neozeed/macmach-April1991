# /usr/src/ucb/indent/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	indent.c io.c lexi.c parse.c pr_comment.c args.c
OBJS=	indent.o io.o lexi.o parse.o pr_comment.o args.o

all:	indent .depend

indent:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 indent ${DESTDIR}/usr/ucb/indent

clean:	FRC
	rm -f .depend ${OBJS} core indent

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
