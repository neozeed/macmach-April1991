# /usr/src/usr.bin/tac/Makefile for MacMach

DESTDIR=
BINDIR=	/usr/bin
MANTYPE= 1

CFLAGS=	-O

STD=	tac
SCRIPT=	tmail
MAN=	tac.1 tmail.1

all:	${STD} .depend

${STD}:
	cc ${CFLAGS} -o $@ $@.c

install: all
	install -c -s tac ${DESTDIR}${BINDIR}/tac
	install -c -m 755 -c tmail.sh ${DESTDIR}${BINDIR}/tmail
	-for i in ${MAN}; do \
	    (/usr/man/manroff $$i > \
	    ${DESTDIR}/usr/man/man${MANTYPE}/`basename $$i .1`.${MANTYPE}); \
	    done

clean:	FRC
	rm -f .depend *.o ${STD} Makefile.bak makedep eddep a.out core errs 

.depend:
	@echo "#`date`" >.depend
	for i in ${STD}; do (${CC} -M $$i.c |sed 's/.o: /: /' >>.depend); done

-include .depend

FRC:
