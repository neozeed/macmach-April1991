# /usr/src/usr.bin/prof/Makefile for MacMach

# If you don't want to plot, take out the -Dplot and the loader
# reference to plot.a
CFLAGS=	-O -Dplot
LIBC=	/lib/libc.a
SRCS=	prof.c
OBJS=	prof.o

all:	prof .depend

prof:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -lplot

install: FRC
	install -c -s -o bin -g bin -m 755 prof ${DESTDIR}/usr/bin/prof

clean:	FRC
	rm -f .depend ${OBJS} core prof

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
