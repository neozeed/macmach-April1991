# /usr/src/usr.bin/roff/troff/Makefile for MacMach

COMMON=	../common_source
CFLAGS=	-O -DVMUNIX -I${COMMON}
VPATH=	${COMMON}
LIBC=	/lib/libc.a
SRCS=	ni.c nii.c n1.c n2.c n3.c n4.c n5.c t6.c n7.c n8.c n9.c t10.c \
	hytab.c suftab.c tab3.c
OBJS=	ni.o nii.o n1.o n2.o n3.o n4.o n5.o t6.o n7.o n8.o n9.o t10.o \
	hytab.o suftab.o tab3.o

all:	troff .depend

troff:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

hytab.o: ${COMMON}/hytab.c
	${CC} -R -c ${COMMON}/hytab.c

suftab.o: ${COMMON}/suftab.c
	${CC} -R -c ${COMMON}/suftab.c

install: FRC
	install -c -s -o bin -g bin -m 755 troff ${DESTDIR}/usr/bin/troff

clean:	FRC
	rm -f .depend ${OBJS} core troff

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
