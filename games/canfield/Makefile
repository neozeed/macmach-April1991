# /usr/src/games/canfield/Makefile for MacMach

include /usr/src/COPYRIGHTS
CFLAGS=	-O
LIBC=	/lib/libc.a
CSRCS=	canfield.c
CFSRCS=	cfscores.c

all:	canfield cfscores .depend

canfield: ${LIBC}
	${CC} -o $@ ${CFLAGS} ${CSRCS} -lcurses -ltermcap

cfscores: ${LIBC}
	${CC} -o $@ ${CFLAGS} ${CFSRCS}

install: FRC
	install -c -s -o games -g bin -m 744 canfield ${DESTDIR}/usr/games/hide
	install -c -s -o games -g bin -m 744 cfscores ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f canfield; ln -s dm canfield; chown games.bin canfield)
	(cd ${DESTDIR}/usr/games; rm -f cfscores; ln -s dm cfscores; chown games.bin cfscores)
	-[ -f ${DESTDIR}/usr/games/lib/cfscores ] || install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/cfscores

clean:	FRC
	rm -f .depend core canfield cfscores

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${CSRCS} ${CFSRCS}

-include .depend

FRC:
