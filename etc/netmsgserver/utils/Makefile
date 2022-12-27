# /usr/src/etc/netmsgserver/utils/Makefile for MacMach

# removed vmtptest since IPPROTO_VMTP is no longer in netinet/in.h 9/5/90 zw

CFLAGS=	-O -I../conf -I../server
SRCS=	nmcontrol.c nmtest.c nmtime.c nmxlog.c nmxportstat.c \
	../server/ls_rec.c logstatUser.c
STD=	nmcontrol nmtest nmtime nmxlog nmxportstat

all:	${STD} .depend

nmcontrol: nmcontrol.c ../server/ls_rec.c logstatUser.c
	${CC} -o $@ ${CFLAGS} nmcontrol.c ../server/ls_rec.c logstatUser.c -lmach

nmtest: nmtest.c
	${CC} -o $@ ${CFLAGS} nmtest.c -lthreads -lmach

nmtime: nmtime.c
	${CC} -o $@ ${CFLAGS} nmtime.c -lmach

nmxlog: nmxlog.c
	${CC} -o $@ ${CFLAGS} nmxlog.c

nmxportstat: nmxportstat.c
	${CC} -o $@ ${CFLAGS} nmxportstat.c

vmtptest: vmtptest.c ../lib/libvmtp.a
	${CC} -o $@ ${CFLAGS} vmtptest.c ../lib/libvmtp.a -lthreads -lmach

logstatUser.c logstat.h: ../server/logstat.defs
	mig $? -header logstat.h -server /dev/null -user logstatUser.c

install: FRC

clean:	FRC
	rm -f .depend *.o ${STD} core
	rm -f logstatServer.c logstat.h

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
