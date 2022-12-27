# /usr/src/usr.bin/diction/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	diction.c nwords.c end.c part.c pscan.c outp.c
OBJS=	nwords.o end.o part.o pscan.o outp.o

all:	dprog style1 style2 style3 .depend

dprog: diction.c ${LIBC}
	${CC} ${CFLAGS} -DDICT=\"/usr/lib/dict.d\" diction.c -o $@

style1: nwords.o ${LIBC}
	${CC} nwords.o -o $@ -ll

style2: end.o ${LIBC}
	${CC} end.o -o $@ -ll

style3: part.o pscan.o outp.o ${LIBC}
	${CC} part.o pscan.o outp.o -o $@ -ll

install: FRC
	-[ -d ${DESTDIR}/usr/dict ] || mkdir ${DESTDIR}/usr/dict
	-chmod 755 ${DESTDIR}/usr/dict
	-chown root.bin ${DESTDIR}/usr/dict
	install -c -s -o bin -g bin -m 755 style1 ${DESTDIR}/usr/lib
	install -c -s -o bin -g bin -m 755 style2 ${DESTDIR}/usr/lib
	install -c -s -o bin -g bin -m 755 style3 ${DESTDIR}/usr/lib
	install -c -s -o bin -g bin -m 755 dprog ${DESTDIR}/usr/lib
	install -c -o bin -g bin -m 755 style.sh ${DESTDIR}/usr/bin/style
	install -c -o bin -g bin -m 755 diction.sh ${DESTDIR}/usr/bin/diction
	install -c -o bin -g bin -m 755 explain.sh ${DESTDIR}/usr/bin/explain
	install -c -o bin -g bin -m 755 dict.d ${DESTDIR}/usr/lib
	install -c -o bin -g bin -m 755 explain.d ${DESTDIR}/usr/lib

clean:	FRC
	rm -f .depend ${OBJS} part.c nwords.c end.c style1 style2 style3 dprog

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
