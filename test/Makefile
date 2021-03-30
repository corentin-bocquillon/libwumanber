.POSIX:
.SUFFIXES: .o .c

CFLAGS  = -std=c99 -I.. -g -static
LDFLAGS = -L.. -lwumanber

SRC_TEST_WUMANBER = wumanber_test.c
OBJ_TEST_WUMANBER = ${SRC_TEST_WUMANBER:.c=.o} ../vector.o

SRC_TEST_VECTOR = vector_test.c
OBJ_TEST_VECTOR = ${SRC_TEST_VECTOR:.c=.o} ../vector.o

all: check

check: wumanber_test vector_test
	./wumanber_test
	./vector_test

wumanber_test: ${OBJ_TEST_WUMANBER}
	$(CC) $(CFLAGS) -o $@ ${OBJ_TEST_WUMANBER} $(LDFLAGS)

vector_test: ${OBJ_TEST_VECTOR}
	$(CC) $(CFLAGS) -o $@ ${OBJ_TEST_VECTOR} $(LDFLAGS)

clean:
	rm -f *.o wumanber_test vector_test

.PHONY: all check clean
