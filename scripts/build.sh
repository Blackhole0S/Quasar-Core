#!/bin/bash

set -e

PROJECT_ROOT=$(dirname "$(realpath "$0")")/..
BUILD_DIR="$PROJECT_ROOT/bin"

mkdir -p "$BUILD_DIR"

echo "[*] Building Quasar-Core..."

gcc -Wall -Wextra -std=c99 \
    "$PROJECT_ROOT/src/main.c" \
    "$PROJECT_ROOT/src/service.c" \
    "$PROJECT_ROOT/src/signal.c" \
    "$PROJECT_ROOT/src/logging.c" \
    "$PROJECT_ROOT/src/sandbox.c" \
    "$PROJECT_ROOT/src/utils.c" \
    -I"$PROJECT_ROOT/include" \
    -o "$BUILD_DIR/init"

echo "[✓] Build complete: $BUILD_DIR/init"
