CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra
LIBS = include/*.c

all: cat echo wc head
cat: cat.c
	$(CC) $(CFLAGS) -o cat cat.c $(LIBS)
echo: echo.c
	$(CC) $(CFLAGS) -o echo echo.c 
wc: wc.c
	$(CC) $(CFLAGS) -o wc wc.c $(LIBS)
head: head.c
	$(CC) $(CFLAGS) -o head head.c $(LIBS)
clean:
	rm -f echo cat wc head
