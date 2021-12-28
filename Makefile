CFLAGS=-std=c89 -Wall -Wextra -Ofast
PREFIX=/usr

all: turtel turtel_lex
clean:
	rm turtel turtel_lex
install: all
	cp turtel $(PREFIX)/bin/turtel
	cp turtel_lex $(PREFIX)/bin/turtel_lex
uninstall:
	rm $(PREFIX)/bin/turtel
	rm $(PREFIX)/bin/turtel_lex
