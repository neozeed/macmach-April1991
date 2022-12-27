# /usr/src/ucb/ftp/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	cmds.c cmdtab.c ftp.c getpass.c glob.c main.c pclose.c ruserpass.c              domacro.c
OBJS=	cmds.o cmdtab.o ftp.o getpass.o glob.o main.o pclose.o ruserpass.o              domacro.o

all:	ftp .depend

ftp:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 ftp ${DESTDIR}/usr/ucb/ftp

clean:	FRC
	rm -f .depend ${OBJS} core ftp

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
