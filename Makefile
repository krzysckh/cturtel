CFLAGS=-std=c89 -Wall -Wextra -Ofast
LDFLAGS=
PREFIX=/usr

all: turtel turtel_lex turtel.1.gz turtel_lex.1.gz
%.gz: %
	gzip -k $<
clean:
	rm -f turtel turtel_lex turtel.1.gz turtel_lex.1.gz
install: all
	install -Dm755 -s turtel -t $(PREFIX)/bin/
	install -Dm755 -s turtel_lex -t $(PREFIX)/bin/
	install -Dm644 turtel.1.gz -t $(PREFIX)/share/man/man1/
	install -Dm644 turtel_lex.1.gz -t $(PREFIX)/share/man/man1/
uninstall:
	rm -f $(PREFIX)/bin/turtel
	rm -f $(PREFIX)/bin/turtel_lex
	rm -f $(PREFIX)/share/man/man1/turtel.1
	rm -f $(PREFIX)/share/man/man1/turtel_lex.1
