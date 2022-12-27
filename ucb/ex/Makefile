# /usr/src/ucb/ex/Makefile for MacMach

VERSION=3.7
#
# Ex skeletal makefile for VAX VM/Unix 4.2BSD
#
# Ex is very large - this version will not fit on PDP-11's without overlay
# software.  Things that can be turned off to save
# space include LISPCODE (-l flag, showmatch and lisp options), UCVISUAL
# (visual \ nonsense on upper case only terminals), CHDIR (the undocumented
# chdir command.)  CRYPT includes the code to edit encrypted files (the -x
# option, like ed.)  VMUNIX makes ex considerably larger, raising many limits
# and improving speed and simplicity of maintenance.  It is suitable only
# for a VAX or other large machine, and then probably only in a paged system.
#
# Don't define VFORK unless your system has the VFORK system call,
# which is like fork but the two processes have only one data space until the
# child execs. This speeds up ex by saving the memory copy.
#
# If your system expands tabs to 4 spaces you should -DTABS=4 below
#
BINDIR=		/usr/ucb
NBINDIR=	/usr/new
LIBDIR=		/usr/lib
OPTIONS=	-DCRYPT -DLISPCODE -DCHDIR -DUCVISUAL -DVFORK -DVMUNIX \
		-DFASTTAG -DUNIX_SBRK
CFLAGS=		-DTABS=8 -O ${OPTIONS}
LDFLAGS=	-z				# or -i or -n
LIBC=		/lib/libc.a

OBJS=	ex.o ex_addr.o ex_cmds.o ex_cmds2.o ex_cmdsub.o ex_data.o ex_extern.o \
	ex_get.o ex_io.o ex_put.o ex_re.o ex_set.o ex_subr.o ex_tagio.o \
	ex_temp.o ex_tty.o ex_unix.o ex_v.o ex_vadj.o ex_vget.o ex_vmain.o \
	ex_voper.o ex_vops.o ex_vops2.o ex_vops3.o ex_vput.o ex_vwind.o \
	printf.o strings.o
HDRS=	ex.h ex_argv.h ex_re.h ex_temp.h ex_tty.h ex_tune.h ex_vars.h ex_vis.h
SRCS=	ex.c ex_addr.c ex_cmds.c ex_cmds2.c ex_cmdsub.c ex_data.c ex_get.c \
	ex_io.c ex_put.c ex_re.c ex_set.c ex_subr.c ex_tagio.c ex_temp.c \
	ex_tty.c ex_unix.c ex_v.c ex_vadj.c ex_vget.c ex_vmain.c ex_voper.c \
	ex_vops.c ex_vops2.c ex_vops3.c ex_vput.c ex_vwind.c printf.c

all:	ex exrecover expreserve .depend

ex: ${OBJS} ${LIBC}
	${CC} ${LDFLAGS} ${OBJS} -ltermcap

.c.o:
# ifdef VMUNIX
	${CC} -E ${CFLAGS} $*.c | xstr -c -
# else
#	mkstr - ex${VERSION}strings x $*.c
#	${CC} -E ${CFLAGS} x$*.c | xstr -c -
#	rm -f x$*.c
# endif
	${CC} ${CFLAGS} -c x.c 
	mv x.o $*.o

ex_vars.h: ex_data.c
	csh makeoptions ${CFLAGS}

# xstr: hands off!
strings.o: strings
	xstr
	${CC} -c -S xs.c
	ed - <rofix xs.s
	${AS} -o strings.o xs.s
	rm xs.s

exrecover: exrecover.o ex_extern.o
	${CC} ${CFLAGS} exrecover.o ex_extern.o -o $@

exrecover.o: exrecover.c
	${CC} ${CFLAGS} -c -O exrecover.c

expreserve: expreserve.c
	${CC} ${CFLAGS} expreserve.c -o $@

expreserve.o: expreserve.c
	${CC} ${CFLAGS} -c -O expreserve.c

# install in standard place (/usr/ucb)
install: a.out exrecover expreserve
	rm -f ${DESTDIR}/${BINDIR}/ex ${DESTDIR}/${BINDIR}/vi \
		${DESTDIR}/${BINDIR}/view ${DESTDIR}/${BINDIR}/edit \
		${DESTDIR}/${BINDIR}/e ${DESTDIR}/usr/bin/ex
	install -c -s -o bin -g bin -m 1755 a.out ${DESTDIR}/${BINDIR}/ex
	install -c -s -o root -m 4755 exrecover ${DESTDIR}/${LIBDIR}/ex${VERSION}recover
	install -c -s -o root -m 4755 expreserve ${DESTDIR}/${LIBDIR}/ex${VERSION}preserve
#	cp ex${VERSION}strings ${DESTDIR}/${LIBDIR}/ex${VERSION}strings
	ln -s ex ${DESTDIR}/${BINDIR}/edit
	ln -s ex ${DESTDIR}/${BINDIR}/e
	ln -s ex ${DESTDIR}/${BINDIR}/vi
	ln -s ex ${DESTDIR}/${BINDIR}/view
# assume BINDIR=/usr/ucb
	ln -s ../ucb/ex ${DESTDIR}/usr/bin/ex
	-[ -d ${DESTDIR}/usr/preserve ] || mkdir ${DESTDIR}/usr/preserve

clean:	FRC
#	If we dont have ex we can't make it so don't rm ex_vars.h
	rm -f .depend a.out exrecover expreserve strings core errs trace
	rm -f .depend *.o x*.[cs]

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
