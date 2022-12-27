# /usr/src/usr.lib/migcom/Makefile for MacMach

CFLAGS=	-O

LIBDIR=	${DESTDIR}/usr/lib

SRCS= 	parser.c lexxer.c mig.c error.c string.c type.c routine.c \
	statement.c global.c header.c user.c server.c utils.c

OBJS= 	parser.o lexxer.o mig.o error.o string.o type.o routine.o \
	statement.o global.o header.o user.o server.o utils.o

all:	migcom .depend

migcom:	${OBJS}
	${CC} -o $@ ${OBJS} -ll

parser.c parser.h :	parser.y
	yacc -d parser.y && mv y.tab.c parser.c && mv y.tab.h parser.h

lexxer.c :	lexxer.l
	lex lexxer.l &&	mv lex.yy.c lexxer.c

install: FRC
	install -c -s -o root -g bin -m 755 migcom ${LIBDIR}/migcom

clean:	FRC
	rm -f .depend migcom parser.c parser.h lexxer.c ${OBJS} core

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
