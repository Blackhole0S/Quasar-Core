# quasar-core Makefile

CC = clang
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS =
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/quasar-core

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
test_service: tests/test_service.c src/service.c src/logging.c src/sandbox.c src/utils.c
	$(CC) $^ -Iinclude -o bin/test_service

test: test_service
	./bin/test_service

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
