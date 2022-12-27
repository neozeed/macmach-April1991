# /usr/src/ucb/tn3270/telnet/Makefile for MacMach

# The following is the telnet makefile for tn3270, using the shared telnet
# sources.

VPATH	=	../../telnet/Source
INCLUDES=	-I../../telnet/Source -I.
DEFINES	=	-DTN3270
CFLAGS	=	-O ${INCLUDES} ${DEFINES}
LD	=	ld
LDFLAGS	=	-r
LIBC=	/lib/libc.a
SRCS=	commands.c main.c network.c ring.c \
	sys_bsd.c sys_dos.c telnet.c terminal.c \
	tn3270.c utilities.c
OBJS=	commands.o main.o network.o ring.o sys_bsd.o sys_dos.o \
	telnet.o terminal.o tn3270.o utilities.o

all:	telnet .depend

telnet:	${OBJS} ${LIBC}
	${LD} ${LDFLAGS} -o $@ ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 telnet ${DESTDIR}/usr/ucb/telnet

clean:	FRC
	rm -f .depend ${OBJS} core telnet

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
