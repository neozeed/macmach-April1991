# /usr/src/man/man6/Makefile

MDIR=	/usr/man/man6
SRCS=	aardvark.6 adventure.6 arithmetic.6 atc.6 backgammon.6 \
	banner.6 battlestar.6 bcd.6 boggle.6 canfield.6 chess.6 \
	ching.6 cribbage.6 doctor.6 fish.6 fortune.6 gnuchess.6 \
	hack.6 hangman.6 hunt.6 mille.6 monop.6 number.6 quiz.6 \
	rain.6 robots.6 rogue.6 sail.6 snake.6 trek.6 worm.6 \
	worms.6 wump.6 zork.6 jive.6 valspeak.6
LINKS=	snscore.6 cfscores.6 ppt.6

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s snake.6 ${DESTDIR}${MDIR}/snscore.6
	ln -s canfield.6 ${DESTDIR}${MDIR}/cfscores.6
	ln -s bcd.6 ${DESTDIR}${MDIR}/ppt.6

FRC:
