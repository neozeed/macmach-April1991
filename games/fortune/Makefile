# /usr/src/games/fortune/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SFLAGS=	-r
TDEV=	-Pver
TROFF=	ditroff ${TDEV}
SRCS=	fortune.c rnd.c strfile.c unstr.c

all:	fortune strfile unstr fortunes.dat .depend

fortune: fortune.o rnd.o ${LIBC}
	${CC} ${CFLAGS} -o $@ fortune.o rnd.o

strfile: strfile.o rnd.o ${LIBC}
	${CC} ${CFLAGS} -o $@ strfile.o rnd.o

unstr:	unstr.o ${LIBC}
	${CC} ${CFLAGS} -o $@ unstr.o

fortunes.dat: fortunes strfile
	./strfile ${SFLAGS} fortunes

fortunes: scene obscene
	(cat scene; echo "%-"; cat obscene) > fortunes

install: FRC
	install -c -s -o games -g bin -m 4755 fortune ${DESTDIR}/usr/games
	install -c -o games -g bin -m 644 fortunes.dat ${DESTDIR}/usr/games/lib

clean:	FRC
	rm -f .depend fortune fortunes fortunes.dat strfile unstr core *.o
	rm -f Oscene Oobscene

TROFF:	FRC
	./Do_troff scene ${TROFF}
	./Do_troff obscene ${TROFF}

sort: sort.scene sort.obscene

sort.scene: strfile unstr
	strfile -oi scene
	mv scene Oscene
	unstr -o scene

sort.obscene: strfile unstr
	strfile -oi obscene
	mv obscene Oobscene
	unstr -o obscene

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
