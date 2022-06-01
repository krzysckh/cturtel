CFLAGS=-Wall -Wextra -std=c89 -ggdb
OFILES=turtel.o lexer.o err.o run.o fn.o

.SUFFIXES: .c .o

all: turtel
turtel: ${OFILES}
	${CC} ${CFLAGS} -o $@ ${OFILES}
.c.o:
	${CC} ${CFLAGS} -c $< ${LDFLAGS}

clean:
	rm -f turtel *.o
