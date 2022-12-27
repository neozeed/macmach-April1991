# /usr/src/lib/libc/inet/Makefile for MacMach

DEFS=
CFLAGS=	-O ${DEFS}
SRCS=	inet_addr.c inet_network.c inet_netof.c inet_ntoa.c inet_lnaof.c \
	inet_makeaddr.c
OBJS=	inet_addr.o inet_network.o inet_netof.o inet_ntoa.o inet_lnaof.o \
	inet_makeaddr.o
TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../library; rm -f ${OBJS})
	(cd ../profiled; rm -f ${OBJS})
	cp ${OBJS} ../library
	(cd profiled; cp ${OBJS} ../../profiled)

.c.o:
	@${CC} -p -c ${CFLAGS} $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	${CC} ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
	cwd=`pwd`; \
	for i in ${SRCS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
