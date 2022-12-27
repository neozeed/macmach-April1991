# /usr/src/games/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a

# Programs that live in subdirectories, and have makefiles of their own.
# Note: "doctor" removed until lisp works
SUBDIR=	dm adventure atc backgammon battlestar boggle btlgammon canfield \
	cribbage fortune hack hangman hunt mille monop \
	quiz robots rogue sail snake trek \
	games.${MACHINE}

# Shell scripts that need only be installed and are never removed.
SCRIPT=	wargames

# C sources that live in the current directory
SRCS=	arithmetic.c banner.c bcd.c factor.c fish.c number.c ppt.c primes.c \
	rain.c worm.c worms.c wump.c

# C programs that live in the current directory and do not need
# explicit make lines.
STD=	arithmetic banner bcd factor fish number ppt wump

# C programs that live in the current directory and need explicit make lines.
NSTD=	primes rain worm worms

# programs that get hidden
HIDE=	fish wump worm

# programs that don't
NOHIDE=	arithmetic banner bcd factor number ppt primes rain worms

# Programs that require a lisp interpreter or compiler
LISP=	doctor

all:	${SUBDIR} ${STD} ${NSTD} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

${STD}: ${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${SCRIPT}; do \
	  (install -c -o games -g bin -m 755 $$i.sh ${DESTDIR}/usr/games/$$i); \
	done
	install -c -s -o games -g bin -m 744 ${HIDE} ${DESTDIR}/usr/games/hide
	install -c -s -o games -g bin -m 755 ${NOHIDE} ${DESTDIR}/usr/games
	-for i in ${HIDE}; do \
	  (cd ${DESTDIR}/usr/games; rm -f $$i; ln -s dm $$i; chown games.bin $$i) \
	done

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
	rm -f .depend ${STD} ${NSTD} a.out core *.s *.o

# Files listed in ${NSTD} have explicit make lines given below.

primes:
	${CC} -o $@ ${CFLAGS} primes.c -lm

rain:
	${CC} -o $@ ${CFLAGS} rain.c -lcurses -ltermcap

worm:
	${CC} -o $@ ${CFLAGS} worm.c -lcurses -ltermcap

worms:
	${CC} -o $@ ${CFLAGS} worms.c -lcurses -ltermcap

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
