CC = gcc
CFLAGS = -Wall -std=c11 -g
LDFLAGS = -lrt
TARGET1 = printer
TARGET2 = user

.PHONY: all clean

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c $(LDFLAGS)

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c $(LDFLAGS)

clean:
	rm -f $(TARGET1)
	rm -f $(TARGET2)