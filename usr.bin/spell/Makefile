# /usr/src/usr.bin/spell/Makefile for MacMach

SPECIAL=	special.4bsd

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	spell.c spellin.c spellout.c
OBJS=	spell.o spellin.o spellout.o

all:	spell hlist hlista hlistb hstop spellin spellout .depend

spell:	spell.c
	${CC} ${CFLAGS} spell.c -o $@

spellin: spellin.c
	${CC} ${CFLAGS} spellin.c -o $@

spellout: spellout.c
	${CC} ${CFLAGS} spellout.c -o $@

hlist:	/usr/dict/words spellin
	spellin < words >hlist

hlista: american ${SPECIAL} ${LOCAL} hlist spellin
	(cat american ${SPECIAL} ${LOCAL}) | spellin hlist > hlista

hlistb: british ${SPECIAL} ${LOCAL} hlist spellin
	(cat british ${SPECIAL} ${LOCAL}) | spellin hlist > hlistb

hstop: stop spellin
	spellin < stop >hstop

install: FRC
	-[ -d ${DESTDIR}/usr/dict/special ] || mkdir ${DESTDIR}/usr/dict/special
	install -c -s -o bin -g bin -m 755 spell ${DESTDIR}/usr/lib/spell
	install -c -s -o bin -g bin -m 755 spellin ${DESTDIR}/usr/bin/spellin
	install -c -s -o bin -g bin -m 755 spellout ${DESTDIR}/usr/bin/spellout
	install -c -o bin -g bin -m 755 spell.sh ${DESTDIR}/usr/bin/spell
	install -c -o bin -g bin -m 444 hlista ${DESTDIR}/usr/dict/hlista
	install -c -o bin -g bin -m 444 hlistb ${DESTDIR}/usr/dict/hlistb
	install -c -o bin -g bin -m 444 hstop ${DESTDIR}/usr/dict/hstop
	install -c -o bin -g bin -m 444 hlist ${DESTDIR}/usr/dict/hlist
	install -c -o bin -g bin -m 444 words ${DESTDIR}/usr/dict/words
	install -c -o bin -g bin -m 444 american ${DESTDIR}/usr/dict/american
	install -c -o bin -g bin -m 444 british ${DESTDIR}/usr/dict/british
	install -c -o bin -g bin -m 444 stop ${DESTDIR}/usr/dict/stop
	install -c -o bin -g bin -m 444 README ${DESTDIR}/usr/dict/README
	install -c -o bin -g bin -m 444 special.4bsd ${DESTDIR}/usr/dict/special/4bsd
	install -c -o bin -g bin -m 444 special.math ${DESTDIR}/usr/dict/special/math

clean:	FRC
	rm -f .depend ${OBJS} core spell spellin hlista hlistb hlist hstop spellout

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
