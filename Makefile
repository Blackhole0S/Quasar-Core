# quasar-core Makefile

# Compiler and flags
CC       ?= clang
LD       ?= $(CC)
CFLAGS   = -std=c99 -Wall -Wextra -Wpedantic -O2 -Iinclude \
           -fstack-protector-strong -fPIE -D_FORTIFY_SOURCE=2 -Wno-unused-parameter
LDFLAGS  = -Wl,-O1 -Wl,--as-needed -Wl,-z,relro,-z,now -pie

# Optional: for static init binary
STATIC_FLAGS = -static

# Targets
TARGET_INIT = quasar-init
TARGET_CTL  = quasarctl

# Source and object files
SRC_INIT = $(wildcard init/*.c)
SRC_CTL  = $(wildcard ctl/*.c)

OBJ_INIT = $(SRC_INIT:.c=.o)
OBJ_CTL  = $(SRC_CTL:.c=.o)

BIN_DIR  = /usr/local/sbin

.PHONY: all clean install uninstall

all: $(TARGET_INIT) $(TARGET_CTL)

$(TARGET_INIT): $(OBJ_INIT)
	$(CC) $(CFLAGS) $(STATIC_FLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_CTL): $(OBJ_CTL)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install:
	install -Dm755 $(TARGET_INIT) $(BIN_DIR)/$(TARGET_INIT)
	install -Dm755 $(TARGET_CTL)  $(BIN_DIR)/$(TARGET_CTL)

uninstall:
	rm -f $(BIN_DIR)/$(TARGET_INIT) $(BIN_DIR)/$(TARGET_CTL)

clean:
	rm -f $(OBJ_INIT) $(OBJ_CTL) $(TARGET_INIT) $(TARGET_CTL)
