# /usr/src/lib/cpp/Makefile for MacMach

CFLAGS=	-O -Dunix=1 -DFLEXNAMES
LIBC=	/lib/libc.a
YYFIX=	../pcc/pcc.${MACHINE}/:yyfix
SRCS=	cpp.c cpy.c rodata.c
OBJS=	cpp.o cpy.o rodata.o

all:	cpp .depend

cpp:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

cpy.c:	cpy.y
	yacc cpy.y
	${YYFIX} yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c cpy.c

cpy.o:	cpy.c yylex.c

rodata.o: cpy.c
	${CC} ${CFLAGS} -R -c rodata.c

install: FRC
	install -c -s -o bin -g bin -m 755 cpp ${DESTDIR}/lib/cpp

clean:	FRC
	rm -f .depend ${OBJS} cpp core cpy.c rodata.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
