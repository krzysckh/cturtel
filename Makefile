CFLAGS=-std=c89 -Wall -Wextra -Ofast
LDFLAGS=
PREFIX=/usr

all: turtel turtel_lex
clean:
	rm turtel turtel_lex
install: all
	cp turtel $(PREFIX)/bin/turtel
	cp turtel_lex $(PREFIX)/bin/turtel_lex
	cp turtel.1 $(PREFIX)/share/man/man1/turtel.1
	cp turtel_lex.1 $(PREFIX)/share/man/man1/turtel_lex.1
uninstall:
	rm $(PREFIX)/bin/turtel
	rm $(PREFIX)/bin/turtel_lex
	rm $(PREFIX)/share/man/man1/turtel.1
	rm $(PREFIX)/share/man/man1/turtel_lex.1
