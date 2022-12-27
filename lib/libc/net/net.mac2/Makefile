# /usr/src/lib/libc/net/net.mac2/Makefile for MacMach

DEFS=
FLAGS=	${DEFS} -I../../mac2
CFLAGS=	-O ${FLAGS}

SRCS=	htonl.S htons.S ntohl.S ntohs.S
OBJS=	htonl.o htons.o ntohl.o ntohs.o
TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../../library; rm -f ${OBJS})
	(cd ../../profiled; rm -f ${OBJS})
	cp ${OBJS} ../../library
	(cd profiled; cp ${OBJS} ../../../profiled)

.S.o:
	@${CC} -p ${FLAGS} -c $*.S
	@-ld -X -o profiled/$*.o -r $*.o
	${CC} ${FLAGS} -c $*.S
	@-ld -x -r $*.o
	@mv a.out $*.o

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
	(P=`pwd`; \
	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SRCS} | /bin/sed \
"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
	  >> ${TAGSFILE})

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
