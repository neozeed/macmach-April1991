# /usr/src/lib/libc/mac2/csu/Makefile for MacMach

#	crt0	Normal C run time startoff
#	mcrt0	C run time start off for profiling, ``prof'' conventions
#	gcrt0	C run time start off for profiling, ``gprof'' conventions

DEFS=

VERSION= -DVERSION="`cat ../../../../VERSION`"

SRCS=	crt0.c mon.c gmon.c
OBJS=	crt0.o mcrt0.o gcrt0.o mon.o gmon.o
TAGSFILE=tags

all link: ${OBJS} .depend

crt0.o: crt0.c
	${CC} ${VERSION} ${DEFS} -DCRT0 crt0.c -c -o x.o
	ld -x -r -o crt0.o x.o
	rm -f x.o

moncrt0.o: crt0.c
	${CC} ${VERSION} ${DEFS} -DMCRT0 -c crt0.c -o moncrt0.o

mon.o: mon.c
	${CC} ${DEFS} -c mon.c -o x.o
	ld -x -r -o mon.o x.o
	rm -f x.o

gmon.o: gmon.c
	${CC} ${DEFS} -c gmon.c

gcrt0.o: moncrt0.o gmon.o
	ld -x -r -o gcrt0.o moncrt0.o gmon.o

mcrt0.o: moncrt0.o mon.o
	ld -x -r -o mcrt0.o moncrt0.o mon.o

install: FRC
	install -c -o bin -g bin -m 444 crt0.o ${DESTDIR}/lib/crt0.o
	install -c -o bin -g bin -m 444 mcrt0.o ${DESTDIR}/lib/mcrt0.o
	install -c -o bin -g bin -m 444 gcrt0.o ${DESTDIR}/usr/lib/gcrt0.o

clean:	FRC
	rm -f .depend *.o a.out core ${TAGSFILE}

tags:	FRC
	cwd=`pwd`; \
	for i in ${SRCS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
