CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinc

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = build/lift_emulator

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf build src/*.o
