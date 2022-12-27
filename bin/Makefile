# /usr/src/bin/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	adb as awk cpio csh diff ld make passwd sed sh tp

# Shell scripts that need only be installed and are never removed.
SCRIPT=	false true

# C source that lives in the current directory
SRCS=	ar.c cat.c chmod.c cmp.c cp.c date.c dd.c df.c du.c \
	echo.c ed.c expr.c hostid.c hostname.c kill.c ln.c login.c \
	ls.c machine.c md.c mkdir.c mt.c mv.c nice.c nm.c od.c pagesize.c \
	pr.c ps.c pwd.c rcp.c rm.c rmdir.c size.c sony.c strip.c \
	stty.c su.c sync.c tar.c tee.c test.c time.c wall.c who.c write.c

# C programs that live in the current directory and do not need
# explicit make lines.
STD=	ar cat chmod cmp cp date dd du echo ed hostid \
	hostname kill ln ls machine md mkdir mt mv nice nm od pagesize pr \
	pwd rm rmdir size sony strip stty sync tar tee test time who

# C programs that live in the current directory and need explicit make lines.
NSTD=	expr ps

# Programs that get installed with special permissions
PERM=	df login rcp su wall write

all:	${SUBDIR} ${STD} ${NSTD} ${PERM} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

${STD} ${PERM}: ${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${SCRIPT}; do \
	  (install -c -o bin -g bin -m 755 $$i.sh ${DESTDIR}/bin/$$i); \
	done
	install -c -s -o bin -g bin -m 755 ${STD} ${DESTDIR}/bin
# programs that run with special permissions
	install -c -s -o root -g bin -m 4755 login ${DESTDIR}/bin/login
	install -c -s -o root -g bin -m 4755 rcp ${DESTDIR}/bin/rcp
	install -c -s -o root -g bin -m 4755 su ${DESTDIR}/bin/su
	install -c -s -o bin -g operator -m 2755 df ${DESTDIR}/bin/df
	install -c -s -o bin -g kmem -m 2755 ps ${DESTDIR}/bin/ps
	install -c -s -o bin -g tty -m 2755 wall ${DESTDIR}/bin/wall
	install -c -s -o bin -g tty -m 2755 write ${DESTDIR}/bin/write
	install -c -s -o bin -g bin -m 755 expr ${DESTDIR}/bin/expr
#	install -c -s -o bin -g bin -m 755 cc ${DESTDIR}/bin/cc
	rm -f ${DESTDIR}/bin/cc
	ln -s gcc ${DESTDIR}/bin/cc
	rm -f ${DESTDIR}/bin/as
	ln -s gas ${DESTDIR}/bin/as
# directories and links
	rm -f ${DESTDIR}/bin/[; ln -s test ${DESTDIR}/bin/[
	rm -f ${DESTDIR}/bin/e; ln -s ed ${DESTDIR}/bin/e
	rm -f ${DESTDIR}/bin/eject; ln -s sony ${DESTDIR}/bin/eject

clean:	FRC
	rm -f .depend ${STD} ${NSTD} ${PERM} expr.c a.out core *.s *.o errs
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

# Files listed in ${NSTD} have explicit make lines given below.

expr:	expr.o
	${CC} ${CFLAGS} -o $@ expr.o

ps:
	$(CC) $(CFLAGS) -o ps ps.c -ltermlib -lmach

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} expr.c ${SRCS}

-include .depend

FRC:
