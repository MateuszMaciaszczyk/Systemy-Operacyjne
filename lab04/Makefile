CC = gcc
CFLAGS = -Wall -std=c11 -g
TARGET1 = create_child_process
TARGET2 = error_service
.PHONY: all clean

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

clean:
	rm -f $(TARGET1)
	rm -f $(TARGET2)