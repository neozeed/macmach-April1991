# /usr/src/ucb/telnet/Makefile for MacMach

# The following is the telnet makefile.  The sources are down one level
# in directory "Source" because the sources are shared between telnet
# and tn3270.

VPATH	=	Source
INCLUDES=	-ISource
CFLAGS=	-O ${INCLUDES}
LIBC=	/lib/libc.a
SRCS=	commands.c main.c network.c ring.c \
	sys_bsd.c sys_dos.c telnet.c terminal.c \
	tn3270.c utilities.c

ALLHC=	${SRCS} \
	Source/defines.h Source/externs.h Source/fdset.h Source/general.h \
	Source/ring.h Source/types.h

OBJS=	commands.o main.o network.o ring.o sys_bsd.o sys_dos.o \
	telnet.o terminal.o tn3270.o utilities.o

all:	telnet .depend

telnet:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

clean:	FRC
	rm -f .depend ${OBJS} core errs l.errs tags telnet

clist:	${SRCS}
	@for i in ${SRCS} ; \
		do (echo ${DIRPATH}$$i); done

hclist:	${ALLHC}
	@for i in ${ALLHC} ; \
		do (echo ${DIRPATH}$$i); done

depend: ${SRCS} ${SRCS}
	mkdep ${CFLAGS} `make clist`

install: FRC
	install -c -s -o bin -g bin -m 755 telnet ${DESTDIR}/usr/ucb/telnet

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} `make clist`

-include .depend

FRC:
