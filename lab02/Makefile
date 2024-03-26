CC = gcc
CFLAGS = -Wall -std=c11 -g
.PHONY: clean static shared dynamic

all: static shared dynamic

static:
	$(CC) $(CFLAGS) -c collatz.c -o collatz.o
	ar rcs libcollatz.a collatz.o
	$(CC) $(CFLAGS) -o client_static client.c -L. -lcollatz

shared:
	$(CC) $(CFLAGS) -fPIC -c collatz.c -o collatz.o
	$(CC) -shared -o libcollatz.so collatz.o
	$(CC) $(CFLAGS) -o client_shared client.c -L. -lcollatz

dynamic:
	$(CC) $(CFLAGS) -fPIC -c collatz.c -o collatz.o
	$(CC) -shared -o libcollatz.so collatz.o
	$(CC) $(CFLAGS) -D DYNAMIC -o client_dynamic client.c -ldl

clean:
	rm -f *.o *.a *.so client_static client_shared client_dynamic
