CFLAGS=-Wall -Wextra -std=c89 -O2 -DGRAPHICS `pkg-config --cflags x11`
LDFLAGS=`pkg-config --libs x11`
OFILES=turtel.o lexer.o err.o run.o fn.o x.o

PREFIX=/usr/local

.SUFFIXES: .c .o

all: turtel
turtel: ${OFILES}
	${CC} ${CFLAGS} -o $@ ${OFILES} ${LDFLAGS}
.c.o:
	${CC} ${CFLAGS} -c $<
clean:
	rm -f turtel *.o
install: all
	cp turtel ${PREFIX}/bin/turtel
	cp turtel.1 ${PREFIX}/man/man1/turtel.1
uninstall:
	rm -f ${PREFIX}/bin/turtel ${PREFIX}/man/man1/turtel.1
