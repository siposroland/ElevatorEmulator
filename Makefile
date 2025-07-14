CC = gcc
GIT_HASH := $(shell git rev-parse --short HEAD)
CFLAGS = -Wall -Wextra -std=c99 -Iinc -DGIT_COMMIT_HASH=\"$(GIT_HASH)\"

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = build/lift_emulator.exe

.PHONY: all clean

all: $(BIN) post-clean

$(BIN): $(OBJ)
	mkdir build 2>nul || true
	$(CC) $(CFLAGS) $^ -o $@

post-clean:
	del /q src\*.o 2>nul

clean:
	if exist build rmdir /s /q build
	del /q src\*.o 2>nul
