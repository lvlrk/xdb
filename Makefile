CC = gcc
CFLAGS = -O2 -c
LDFLAGS = -lraylib -lm
TARGET = xdb

all: $(TARGET)

build: src/main.c src/xdb.c src/debug.c
	$(CC) $(CFLAGS) $^

$(TARGET): build
	$(CC) $(LDFLAGS) *.o -o $(TARGET)
