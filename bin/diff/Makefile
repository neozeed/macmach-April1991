# /usr/src/bin/diff/Makefile for MacMach

DIFF=	/bin/diff
DIFFH=	/usr/lib/diffh
PR=	/bin/pr
CFLAGS=	-O -DDIFF='"${DIFF}"' -DDIFFH='"${DIFFH}"' -DPR='"${PR}"'
LIBC=	/lib/libc.a
SRCS=	diff.c diffdir.c diffreg.c
OBJS=	diff.o diffdir.o diffreg.o
HSRCS=	diffh.c
HOBJS=	diffh.o

all:	diff diffh .depend

diff:	${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

diffh:	${HOBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${HOBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 diff ${DESTDIR}/bin/diff
	install -c -s -o bin -g bin -m 755 diffh ${DESTDIR}/usr/lib/diffh

clean:	FRC
	rm -f .depend ${HOBJS} ${OBJS} core diff diffh

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${HSRCS} ${SRCS}

-include .depend

FRC:
