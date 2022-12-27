# /usr/src/lib/libc/compat-sys5/compat-sys5.mac2/Makefile for MacMach

DEFS=
FLAGS=	-I../../mac2 ${DEFS}
CFLAGS=	-O ${FLAGS}
SSRCS=
CSRCS=
SRCS=	${SSRCS} ${CSRCS}
OBJS=
TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../../library; rm -f ${OBJS})
	(cd ../../profiled; rm -f ${OBJS})
#	cp ${OBJS} ../../library
#	(cd profiled; cp ${OBJS} ../../../profiled)

.S.o:
	@${CC} -p ${FLAGS} -c $*.S
	@-ld -X -o profiled/$*.o -r $*.o
	${CC} ${FLAGS} -c $*.S
	@-ld -x -r $*.o
	@mv a.out $*.o

.c.o:
	@${CC} -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	${CC} ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
#	cwd=`pwd`; \
#	for i in ${SRCS}; do \
#	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
#	done
#	(P=`pwd`; \
#	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SSRCS} | sed \
#"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
#	>> ${TAGSFILE})

.depend:
	@echo "#`date`" >.depend
#	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
