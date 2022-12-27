# /usr/src/ucb/grep/Makefile for MacMach

# -DEGREPSTD=path	location of std egrep (normally /usr/bin/egrep).
# -DGREPSTD=path	location of std grep (normally /bin/grep).
# -DFGREPSTD=path	location of std fgrep (normally /usr/bin/fgrep).
# -Dstrrchr=rindex, -Dstrchr=index	for troglodytes.
# -DSLOWSYS		invoke xread() for system time quirk on PDP, others? 
# -DNOKANJI		default is for Japanese Unix.  undef only for raw
#			 parity-marked search capability, not standard w/grep.
# -DCHINESE		for systems using EUC Chinese2 codes

CFLAGS=	-O -Dstrrchr=rindex -Dstrchr=index -DNOKANJI \
	-DEGREPSTD=\"/usr/lib/old.egrep\" -DGREPSTD=\"/usr/lib/old.bin.grep\" \
	-DFGREPSTD=\"/usr/lib/old.fgrep\"
LIBC=	/lib/libc.a
STD=	egrep old.ucb.grep old.bin.grep old.fgrep old.egrep
SRCS=	egrep.c old.ucb.grep.c old.bin.grep.c old.fgrep.c old.egrep.c
OBJS=

all:	${STD} .depend

egrep: egrep.c ${LIBC}
	${CC} -o $@ ${CFLAGS} egrep.c

oegrep: oegrep.c ${LIBC}
	${CC} -o $@ ${CFLAGS} oegrep.c

old.ucb.grep: ${LIBC}
	${CC} -o $@ ${CFLAGS} old.ucb.grep.c

old.bin.grep: ${LIBC}
	${CC} -o $@ ${CFLAGS} old.bin.grep.c

old.fgrep: ${LIBC}
	${CC} -o $@ ${CFLAGS} old.fgrep.c

old.egrep: old.egrep.c ${LIBC}
	${CC} -o $@ ${CFLAGS} old.egrep.c

old.egrep.c: old.egrep.y
	yacc old.egrep.y
	mv y.tab.c old.egrep.c

install: FRC
	install -c -s -o bin -g bin -m 755 egrep ${DESTDIR}/usr/ucb/egrep
	rm -f ${DESTDIR}/usr/ucb/fgrep ${DESTDIR}/usr/ucb/grep
	ln -s egrep ${DESTDIR}/usr/ucb/grep
	ln -s egrep ${DESTDIR}/usr/ucb/fgrep
	install -c -s -o bin -g bin -m 755 old.bin.grep ${DESTDIR}/usr/lib/old.bin.grep
	install -c -s -o bin -g bin -m 755 old.ucb.grep ${DESTDIR}/usr/lib/old.ucb.grep
	install -c -s -o bin -g bin -m 755 old.fgrep ${DESTDIR}/usr/lib/old.fgrep
	install -c -s -o bin -g bin -m 755 old.egrep ${DESTDIR}/usr/lib/old.egrep

clean:	FRC
	rm -f .depend ${OBJS} core ${STD} old.egrep.c

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
