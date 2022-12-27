# /usr/src/games/adventure/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	done.c init.c io.c main.c save.c subr.c vocab.c wizard.c
OBJS=	done.o init.o io.o main.o save.o subr.o vocab.o wizard.o

all:	advent .depend

advent: ${OBJS} ${LIBC}
	${CC} -n ${CFLAGS} ${OBJS}
	rm -f $@
	echo y | ./a.out

save.o: save.c # vax C optimizer bug - infinite loop
	${CC} -c save.c

install: FRC
	install -c -s -o games -g bin -m 744 advent ${DESTDIR}/usr/games/hide/adventure
	(cd ${DESTDIR}/usr/games; rm -f adventure; ln -s dm adventure; chown games.bin adventure)

clean:	FRC
	rm -f .depend ${OBJS} core advent a.out

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
