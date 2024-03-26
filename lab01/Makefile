CC = gcc
CFLAGS = -Wall -std=c11 -g
.PHONY: clean
all : countdown
countdown : countdown.c
	$(CC) $(CFLAGS) -o countdown countdown.c
clean :
	rm -f countdown
run : countdown
	./countdown