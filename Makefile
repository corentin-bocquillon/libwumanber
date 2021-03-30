.POSIX:
.SUFFIXES: .o .c

PREFIX=/usr/local

CFLAGS=-fPIC -std=c99 -Wall -g

TARGET=libwumanber
SRC=wumanber.c vector.c
OBJ=${SRC:.c=.o}
HEADERS=wumanber.h

all: $(TARGET).a $(TARGET).so

$(TARGET).a: ${OBJ}
	$(AR) $(ARFLAGS) $@ ${OBJ}
	ranlib $@

$(TARGET).so: ${OBJ}
	gcc -shared -o $@ ${OBJ}

clean:
	rm -f *.o *.a *.so
	$(MAKE) -C test clean

check: $(TARGET).a
	$(MAKE) -C test

install: $(TARGET).a $(TARGET).so
	install -d $(DESTDIR)$(PREFIX)/lib/ $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(TARGET).{a,so} $(DESTDIR)$(PREFIX)/lib/
	install -m 644 ${HEADERS} $(DESTDIR)$(PREFIX)/include/

.PHONY: all clean check install
