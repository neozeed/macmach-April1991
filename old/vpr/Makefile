# /usr/src/old/vpr/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SUBDIR=	vtools vtools/fed man
SHELLS=	rvtroff.sh vprint.sh vtroff.sh vpq.sh vpr.sh vprm.sh
SRCS=	vpac.c vpd.c vpq.c vpr.c vprm.c
OBJS=	vpac.o vpd.o vpq.o vpr.o vprm.o
STD=	vpac vpd vpq vpr vprm

all:	${STD} subdir .depend

subdir:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS}); done

vpac: ${LIBC}
	${CC} -O -o $@ vpac.c

vpd: ${LIBC}
	${CC} -DVERSATEC -O -o $@ vpd.c

vpq: ${LIBC}
	${CC} -O -o $@ vpq.c

vpr: ${LIBC}
	${CC} -O -o $@ vpr.c

vprm: ${LIBC}
	${CC} -O -o $@ vprm.c

install:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} install); done
	install -c -s -o bin -g bin -m 755 vpac ${DESTDIR}/usr/old/vpac
	install -c -s -o bin -g bin -m 755 vpd ${DESTDIR}/usr/lib/vpd
	install -c -s -o bin -g bin -m 755 vpq ${DESTDIR}/usr/lib/vpq
	install -c -s -o bin -g bin -m 755 vpr ${DESTDIR}/usr/lib/vpr
	install -c -s -o bin -g bin -m 755 vprm ${DESTDIR}/usr/lib/vprm
	install -c -o bin -g bin -m 755 vprint.sh ${DESTDIR}/usr/old/vprint
	install -c -o bin -g bin -m 755 vtroff.sh ${DESTDIR}/usr/old/vtroff
	install -c -o bin -g bin -m 755 vpq.sh ${DESTDIR}/usr/old/vpq
	install -c -o bin -g bin -m 755 vpr.sh ${DESTDIR}/usr/old/vpr
	install -c -o bin -g bin -m 755 vprm.sh ${DESTDIR}/usr/old/vprm

clean:	FRC
	rm -f ${OBJS} core ${STD}
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
