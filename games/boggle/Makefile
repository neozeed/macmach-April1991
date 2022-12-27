# /usr/src/games/boggle/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	comp.c boggle.c

all:	boggle bogdict .depend

bogdict: comp sfile
	-if [ -f /usr/local/lib/w2a ]; \
	  then dict=/usr/local/lib/w2a; \
	  else dict=/usr/dict/words; fi; \
	sed -f sfile $$dict | uniq | ./comp >bogdict

boggle: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c

comp: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c

install: FRC
	install -c -s -o games -g bin -m 4744 boggle ${DESTDIR}/usr/games/hide/boggle
	install -c -o games -g bin -m 444 bogdict ${DESTDIR}/usr/games/lib/bogdict
	(cd ${DESTDIR}/usr/games; rm -f boggle; ln -s dm boggle; chown games.bin boggle)

clean:	FRC
	rm -f .depend ${OBJS} core comp boggle bogdict

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
