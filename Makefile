SRCS=	doless.c

PROG=	doless
MAN=	doless.1

COPTS+=	-Wall -Wextra

install:
	install -o root -g bin -m 555 ${PROG} /usr/local/bin
	install -o root -g bin -m 444 ${MAN} /usr/local/man/man1

.include <bsd.prog.mk>

