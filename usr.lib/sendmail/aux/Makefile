# /usr/src/usr.lib/sendmail/aux/Makefile for MacMach

CFLAGS=	-O -I../src -I../include -DVMUNIX
STD=	mconnect mailstats praliases mail rmail
SRCS=	mconnect.c mail.c mailstats.c praliases.c rmail.c
OBJS=	mconnect.o mail.o mailstats.o praliases.o rmail.o

.c.o:
	${CC} -S ${CFLAGS} $*.c
	sed -f ../include/asm.sed.${MACHINE} $*.s > x.s && as x.s -o $*.o
	rm -f $*.s x.s

all:	${STD} .depend

mail: mail.o
	${CC} -o $@ mail.o

rmail: rmail.o
	${CC} -o $@ rmail.o

mconnect: mconnect.o
	${CC} -o $@ mconnect.o

mailstats: mailstats.o
	${CC} -o $@ mailstats.o

praliases: praliases.o
	${CC} -o $@ praliases.o -ldbm

install: FRC
	install -c -s -o root -g bin -m 4755 mail ${DESTDIR}/bin/mail
	install -c -s -o bin -g bin -m 755 rmail ${DESTDIR}/bin/rmail
	install -c -s -o bin -g bin -m 755 mconnect mailstats praliases ${DESTDIR}/usr/lib

clean:	FRC
	rm -f .depend core ${STD} ${OBJS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
