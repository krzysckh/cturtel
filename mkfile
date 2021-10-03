CC="cc"
CFLAGS="-std=c89"

all:
	$CC -o turtel turtel.c $CFLAGS
	$CC -o turtel_lex turtel_lex.c $CFLAGS
clean:
	rm turtel turtel_lex
