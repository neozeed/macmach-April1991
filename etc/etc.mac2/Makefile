# /usr/src/etc/etc.mac2/Makefile for MacMach

DESTDIR=
CFLAGS=	-O

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	newconfig

# Shell scripts that need only be installed and are never removed.
SCRIPT=	newsys

# C sources that live in the current directory
SRCS=	mac2part.c getpass.c

# C programs that live in the current directory and do not need
# explicit make lines.
STD=	mac2part getpass

all:	${SUBDIR} ${STD} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${SCRIPT}; do \
	  (install -c -o bin -g bin -m 755 $$i.sh ${DESTDIR}/etc/$$i); \
	done
	install -c -s -o bin -g bin -m 755 ${STD} ${DESTDIR}/etc

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
	rm -f .depend a.out core *.s *.o ${STD}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
