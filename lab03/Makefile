CC = gcc
CFLAGS = -Wall -std=c11 -g
TARGET = flipper
SRC = flipper.c
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)