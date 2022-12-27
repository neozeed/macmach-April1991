# /usr/src/ucb/talk/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	talk.c get_names.c display.c io.c ctl.c init_disp.c \
	msgs.c get_addrs.c ctl_transact.c invite.c look_up.c
OBJS=	talk.o get_names.o display.o io.o ctl.o init_disp.o \
	msgs.o get_addrs.o ctl_transact.o invite.o look_up.o

all:	talk .depend

talk:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -lcurses -ltermlib

install: FRC
	install -c -s -o bin -g bin -m 755 talk ${DESTDIR}/usr/ucb/talk

clean:	FRC
	rm -f .depend ${OBJS} core talk

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
