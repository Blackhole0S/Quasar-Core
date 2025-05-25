#!/bin/bash

set -e

BIN_PATH="../bin/init"
INSTALL_DIR="/sbin"
CONFIG_DIR="/etc/quasar"

echo "[*] Deploying Quasar-Core..."

if [[ $EUID -ne 0 ]]; then
   echo "Please run as root to deploy."
   exit 1
fi

install -m 0755 "$BIN_PATH" "$INSTALL_DIR/init"
mkdir -p "$CONFIG_DIR"
cp -r ../configs/* "$CONFIG_DIR/"

echo "[✓] Deployed to $INSTALL_DIR/init and configs to $CONFIG_DIR"
