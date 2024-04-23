CC = gcc
CFLAGS = -Wall -std=c11 -g
TARGET1 = zad1
TARGET2 = catcher
TARGET3 = sender
.PHONY: all clean

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

$(TARGET3): $(TARGET3).c
	$(CC) $(CFLAGS) -o $(TARGET3) $(TARGET3).c

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)
