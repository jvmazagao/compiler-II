# lamp Makefile

CC =		cc
CFLAGS =	-g -O2 -DHAVE_STRTONUM

PROG =	main
OBJS =	main.o strtonum.o

all: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG} ${OBJS}

.PHONY:
test:
	cd tests && ./test.sh

clean:
	rm -f ${PROG} ${OBJS} ${PROG}.core

run:
	-./main $(ARGS)
