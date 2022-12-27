# /usr/src/usr.bin/emul/patches/Makefile for MacMach

DESTDIR=
CFLAGS=	-O -I. -I../common

SRCS=	patches.S emul.c time.c rtc.c main.c sbrk.c input.c \
	key_map.c mac_user.c start.S

OBJS=	patches.o emul.o time.o rtc.o main.o sbrk.o input.o \
	key_map.o mac_user.o KCHR_0.o

LIBS=	/usr/lib/libmach.a /lib/libc.a

RTOBJ=	start.o

all:	macpatches .depend

macpatches: mac2emul mac.h ${RTOBJ} ${OBJS}
	ld -N -T 40880000 -o macpatches ${RTOBJ} ${OBJS} ${LIBS}

KCHR_0.o: KCHR_0.b
	uudecode KCHR_0.b

mac_user.c mac.h: mac2emul/macserver.defs
	mig $? -header mac.h -user mac_user.c -server /dev/null

mac2emul: ${SYS}/mac2emul
	ln -s ${SYS}/mac2emul mac2emul

install: FRC
	install -c -s -o bin -g bin -m 755 macpatches ${DESTDIR}/usr/lib

clean:	FRC
	rm -f .depend ${OBJS} ${RTOBJ} core macpatches \#* *~
	rm -f mac2emul mac_user.c mac.h

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
