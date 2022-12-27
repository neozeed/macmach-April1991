# /usr/src/man/Makefile

DIRS=	man.old man1 man2 man3 man3f man4 man4/vax man4/tahoe man5 man6 \
	man7 man8 man8/vax man8/tahoe

all:	FRC
	for i in ${DIRS}; do \
		(cd $$i; make ${MFLAGS}); \
	done

whatis:
	sh mkwhatis.sh ${DESTDIR}/usr/man ${DIRS} manx >${DESTDIR}/usr/man/whatis
	chown bin.bin ${DESTDIR}/usr/man/whatis
	chmod 444 ${DESTDIR}/usr/man/whatis

install: FRC
	for i in ${DIRS}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	install -c -o bin -g bin -m 444 man.template ${DESTDIR}/usr/man/man.template
	tar -chf - doc | (cd ${DESTDIR}/usr/man; tar -xf -)
	find ${DESTDIR}/usr/man/doc -type d -exec chmod 555 {} \;
	find ${DESTDIR}/usr/man/doc -type f -exec chmod 444 {} \;
	chown -R bin.bin ${DESTDIR}/usr/man/doc
	install -c -o bin -g bin -m 644 mandesc ${DESTDIR}/usr/man/mandesc
	install -c -o bin -g bin -m 755 manroff.sh ${DESTDIR}/usr/man/manroff

clean:

FRC:
