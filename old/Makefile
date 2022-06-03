CFLAGS=-std=c89 -Wall -Wextra -Ofast -DGRAPHICS `pkg-config --cflags x11`
LDFLAGS=-lX11 `pkg-config --libs x11`
PREFIX=/usr

.SUFFIXES: .1 .1.gz

.1.1.gz: turtel.1 turtel_lex.1
	gzip -k $<

all: turtel turtel_lex turtel.1.gz turtel_lex.1.gz

clean:
	rm -f turtel turtel_lex turtel.1.gz turtel_lex.1.gz
install: all
	cp turtel $(PREFIX)/bin/
	cp turtel_lex $(PREFIX)/bin/
	cp turtel.1.gz $(PREFIX)/share/man/man1/
	cp turtel_lex.1.gz $(PREFIX)/share/man/man1/
uninstall:
	rm -f $(PREFIX)/bin/turtel
	rm -f $(PREFIX)/bin/turtel_lex
	rm -f $(PREFIX)/share/man/man1/turtel.1
	rm -f $(PREFIX)/share/man/man1/turtel_lex.1
