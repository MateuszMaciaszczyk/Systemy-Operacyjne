CC = gcc
CFLAGS = -Wall -std=c11 -g

all: santa

santa: santa.c
	$(CC) $(CFLAGS) -o santa santa.c -lpthread

clean:
	rm -f santa
