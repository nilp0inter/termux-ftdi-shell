PREFIX?=/data/data/com.termux/files/usr
CC?=clang

CFLAGS+=-I$(PREFIX)/include -I$(PREFIX)/include/libftdi1 -Wall -Wextra -pedantic
LDFLAGS+=-L$(PREFIX)/lib -lftdi1 -lusb-1.0

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

TARGET = termux-ftdi-shell

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
