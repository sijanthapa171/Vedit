CC = cc
CFLAGS = -D_GNU_SOURCE -std=c11 -Wall -Wextra -pedantic -O2
TARGET = notepad
SRC = src/main.c src/terminal.c src/buffer.c src/fileio.c src/screen.c src/input.c src/ex.c
INCLUDES = -Iinclude

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
