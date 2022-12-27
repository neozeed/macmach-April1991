# /usr/src/etc/netmsgserver/Makefile for MacMach

DESTDIR=

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	lib server utils

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	 done

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
