# /usr/src/games/games.mac2/jivval/Makefile for MacMach

# Makefile for jive and valspeak programs

CFLAGS= -O

all: valspeak jive

valspeak:	valspeak.l
	lex valspeak.l
	cc $(CFLAGS) -o valspeak lex.yy.c -ll
	rm -f lex.yy.c

jive:	jive.l
	lex jive.l
	cc $(CFLAGS) -o jive lex.yy.c -ll
	rm -f lex.yy.c

install: FRC
	install -c -s -o games -g bin -m 755 jive ${DESTDIR}/usr/games
	install -c -s -o games -g bin -m 755 valspeak ${DESTDIR}/usr/games

clean:	FRC
	-rm -f *.o core

FRC:
