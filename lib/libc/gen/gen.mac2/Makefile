# /usr/src/lib/libc/gen/gen.mac2/Makefile for MacMach

DEFS=
FLAGS=	-I../../mac2 ${DEFS}
CFLAGS=	-O ${FLAGS}

CSRCS=	atof.c index.c rindex.c strcat.c strcmp.c strcpy.c strlen.c \
	strncat.c strncmp.c strncpy.c strtod.c
SSRCS=	_setjmp.S abs.S alloca.S bcmp.S bcopy.S bzero.S fabs.S ffs.S frexp.S \
	insque.S ldexp.S modf.S remque.S setjmp.S
SRCS=	${CSRCS} ${SSRCS}

OBJS=	_setjmp.o abs.o alloca.o atof.o bcmp.o bcopy.o bzero.o fabs.o ffs.o \
	frexp.o index.o insque.o ldexp.o modf.o remque.o rindex.o setjmp.o \
	strcat.o strcmp.o strcpy.o strlen.o strncat.o strncmp.o strncpy.o \
	strtod.o

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

.c.o:
	@${CC} -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	${CC} ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
	cwd=`pwd`; \
	for i in ${SRCS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done
	(P=`pwd`; \
	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SSRCS} | sed \
"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
	>> ${TAGSFILE})

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
