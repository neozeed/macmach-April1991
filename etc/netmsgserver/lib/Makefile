# /usr/src/etc/netmsgserver/lib/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	vmtplib.c forward.S invoke.S recvreq.S getreply.S probeentity.S \
	sendreply.S
OBJS=	vmtplib.o forward.o invoke.o recvreq.o getreply.o probeentity.o \
	sendreply.o

all:	libvmtp.a .depend

libvmtp.a: ${OBJS}
	@ar cu libvmtp.a ${OBJS}
	ranlib libvmtp.a

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} libvmtp.a

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
