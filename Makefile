CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -Iinclude
LIBS = include/*.c
TARGETS = cat echo wc head tail pwd true false

all: $(TARGETS)

debug: CFLAGS += -g -O0 -fno-omit-frame-pointer
debug: all

$(TARGETS): %: %.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
