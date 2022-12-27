# /usr/src/usr.bin/at/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	atq.c atrm.c at.c atrun.c
OBJS=	atq.o atrm.o at.o atrun.o
ATRUNDIR=	/usr/lib
SPOOLDIRS=	/usr/spool/at /usr/spool/at/past
SPOOLFILES=	/usr/spool/at/lasttimedone

all:	atq atrm at atrun .depend

at: at.c ${LIBC}
	${CC} ${CFLAGS} -o $@ at.c

atq: atq.c ${LIBC}
	${CC} ${CFLAGS} -o $@ atq.c

atrm: atrm.c ${LIBC}
	${CC} ${CFLAGS} -o $@ atrm.c

atrun: atrun.c ${LIBC}
	${CC} ${CFLAGS} -o $@ atrun.c

install: FRC
	-for i in ${SPOOLDIRS}; do \
	  mkdir ${DESTDIR}/$$i > /dev/null 2>&1; \
	  chown root ${DESTDIR}/$$i; \
	  chgrp daemon ${DESTDIR}/$$i; \
	  chmod 755 ${DESTDIR}/$$i; \
	done
	-for i in ${SPOOLFILES}; do \
	  if [ ! -f ${DESTDIR}/$$i ]; then \
	    echo 0 > ${DESTDIR}/$$i; \
	  fi; \
	  chown root ${DESTDIR}/$$i; \
	  chgrp daemon ${DESTDIR}/$$i; \
	done
	install -c -s -o bin -g bin -m 755 atrun ${DESTDIR}/${ATRUNDIR}
	install -c -s -o root -g bin -m 4755 atq ${DESTDIR}/usr/bin
	install -c -s -o root -g bin -m 4755 at ${DESTDIR}/usr/bin
	install -c -s -o root -g bin -m 4755 atrm ${DESTDIR}/usr/bin

clean:	FRC
	rm -f .depend ${OBJS} core atq atrm at atrun

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
