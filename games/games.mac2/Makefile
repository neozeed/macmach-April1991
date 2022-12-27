# /usr/src/games/games.mac2/Makefile for MacMach

CFLAGS=	-O

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	jivval moria nethack

# C sources that live in the current directory
SRCS=	bench.c

# C programs that live in the current directory and do not need
# explicit make lines.
#STD=	

NSTD=	bench

all:	${SUBDIR} ${STD} ${NSTD} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

#${STD}:
#	${CC} ${CFLAGS} -o $@ $@.c

bench:	bench.c
	${CC} ${CFLAGS} -o $@ $@.c -lmach

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${STD} ${NSTD}; do \
	  (install -c -s -o bin -g bin -m 755 $$i ${DESTDIR}/usr/games/$$i); \
	done
	install -c -o bin -g bin -m 644 bench.c ${DESTDIR}/usr/games/bench.c

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
	rm -f .depend ${STD} ${NSTD} a.out core *.s *.o

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
