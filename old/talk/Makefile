# /usr/src/old/talk/Makefile for MacMach

DESTDIR=
DOBJECT=talkd.o announce.o process.o table.o print.o
TOBJECT=talk.o get_names.o display.o io.o ctl.o init_disp.o \
	msgs.o get_addrs.o ctl_transact.o invite.o look_up.o
DSOURCE=talkd.c announce.c process.c table.c print.c
TSOURCE=talk.c get_names.c display.c io.c ctl.c init_disp.c \
	msgs.c get_addrs.c ctl_transact.c invite.c look_up.c
INCLUDE=talk.h ctl.h talk_ctl.h
SOURCE= ${DSOURCE} ${TSOURCE}
CFLAGS= -O 

all:	talk talkd .depend

talkd:	${DOBJECT}
	cc ${CFLAGS} -o talkd ${DOBJECT}

talk:	${TOBJECT}
	cc -o talk ${TOBJECT} -lcurses -ltermlib

${DOBJECT}: ctl.h
${TOBJECT}: talk.h ctl.h talk_ctl.h

install:	FRC
	install -c -g tty -m 2755 -s talk ${DESTDIR}/usr/old/talk
	install -c -s talkd ${DESTDIR}/etc/talkd

clean:	FRC
	rm -f .depend *.o talk talkd

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${DSOURCE} ${TSOURCE}

-include .depend

FRC:
