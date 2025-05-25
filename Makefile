# Makefile for Quasar-Core Init System

PROJECT_NAME := quasar-core
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O2
INCLUDES := -Iinclude
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
TARGET := $(BIN_DIR)/init

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^
	@echo "[✓] Built $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "[✓] Cleaned build artifacts"

run: all
	@echo "[i] Running init (simulated)..."
	./$(TARGET)

test:
	@echo "[i] Running tests..."
	$(CC) $(CFLAGS) $(INCLUDES) tests/test_service.c -o bin/test_service
	$(CC) $(CFLAGS) $(INCLUDES) tests/test_utils.c -o bin/test_utils
	@bin/test_service
	@bin/test_utils
