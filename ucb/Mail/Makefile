# /usr/src/ucb/Mail/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	aux.c cmd1.c cmd2.c cmd3.c cmdtab.c collect.c config.c edit.c fio.c \
	getname.c head.c v7.local.c lex.c list.c main.c names.c optim.c \
	popen.c quit.c send.c strings.c temp.c tty.c vars.c version.c
OBJS=	version.o aux.o cmd1.o cmd2.o cmd3.o cmdtab.o collect.o config.o \
	edit.o fio.o getname.o head.o v7.local.o lex.o list.o main.o \
	names.o optim.o popen.o quit.o send.o strings.o temp.o tty.o \
	vars.o str.o flsbuf.o

all:	Mail fmt .depend

Mail:	${OBJS} ${LIBC}
	${CC} -o Mail ${OBJS} ${LIBES}

fmt: fmt.o xhead.o ${LIBC}
	${CC} ${CFLAGS} fmt.o xhead.o -o $@

# Special massaging of C files for sharing of strings
.c.o:
	${CC} -E ${CFLAGS} $*.c | xstr -c -
	${CC} -c -o $*.o ${CFLAGS} x.c
	@rm -f x.c

version.o: version.c
	${CC} -c -R version.c

xhead.c:
	ln -s head.c xhead.c

xhead.o:
	${CC} -c ${CFLAGS} xhead.c

fmt.o:
	${CC} -c ${CFLAGS} fmt.c

str.o: strings
	xstr
	${CC} -R -c -o str.o xs.c
	rm -f xs.c

cmdtab.o: cmdtab.c
	${CC} -R -c ${CFLAGS} cmdtab.c

install: FRC
	install -c -s -o bin -g bin -m 755 Mail ${DESTDIR}/usr/ucb/Mail
	install -c -s -o bin -g bin -m 755 fmt ${DESTDIR}/usr/ucb/fmt
	install -c -o bin -g bin -m 444 misc/Mail.* ${DESTDIR}/usr/lib
	rm -f ${DESTDIR}/usr/ucb/mail
	ln -s Mail ${DESTDIR}/usr/ucb/mail

clean:	FRC
	rm -f .depend *.o Mail a.out fmt x.c xs.c tags core xhead.c strings

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
