# /usr/src/usr.bin/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	at bison calendar dc diction diff3 efl emacs emul eqn find gdb \
	graph kermit learn lex lint m4 patch plot prof ratfor rcs refer \
	roff sc spell sup struct tac tbl tip uemacs uucp xlisp xsend yacc

# Shell scripts that need only be installed and are never removed.
SCRIPT=	lorder mkdep nohup mig

# C source that lives in the current directory
SRCS=	ar11.c basename.c bc.c cal.c cb.c checkeq.c col.c comm.c crypt.c \
	deroff.c file.c fixit.c xinstall.c join.c look.c mesg.c ptx.c \
	ranlib.c rev.c sleep.c sort.c spline.c split.c sum.c tabs.c \
	tc.c tk.c touch.c tr.c tsort.c tty.c uniq.c units.c wh.c

# C programs that live in the current directory and do not need
# explicit make lines.
STD=	ar11 basename bc cal cb checkeq col comm crypt deroff file fixit \
	join look mesg ptx ranlib rev sleep sort spline split sum tabs tc tk \
	touch tr tsort tty uniq units wh

# C programs that live in the current directory and need explicit make lines.
NSTD=	xinstall

# Programs that get installed with special permissions
PERM=	iostat

all:	${SUBDIR} ${STD} ${PERM} ${NSTD} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

bc: bc.c ${LIBC}

${STD} ${PERM}: ${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${SCRIPT}; do \
	  (install -c -o bin -g bin -m 555 $$i.sh ${DESTDIR}/usr/bin/$$i); \
	done
	install -c -s -o bin -g bin -m 755 ${STD} ${DESTDIR}/usr/bin
	install -c -s -o bin -g bin -m 755 xinstall ${DESTDIR}/usr/bin/install
	install -c -s -o bin -g kmem -m 2755 iostat ${DESTDIR}/usr/bin/iostat
	install -c -o bin -g bin -m 755 units.lib ${DESTDIR}/usr/lib/units

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
	rm -f .depend a.out core *.s *.o bc.c y.tab.c install ${STD} ${NSTD} ${PERM}

# Files listed in ${NSTD} have explicit make lines given below.

xinstall: ${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
