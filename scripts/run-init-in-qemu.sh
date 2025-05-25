#!/bin/bash

set -e

INIT_BIN="../bin/init"
ROOTFS="alpine-rootfs.img"
MEM="256M"

if [ ! -f "$INIT_BIN" ]; then
  echo "Build init first with scripts/build.sh"
  exit 1
fi

echo "[*] Running init in QEMU..."

qemu-system-x86_64 \
  -m "$MEM" \
  -kernel /boot/vmlinuz-linux \
  -append "console=ttyS0 init=/init" \
  -initrd /boot/initramfs-linux.img \
  -nographic \
  -drive file="$ROOTFS",format=raw

# Replace with real kernel/initrd or a custom BlackholeOS image later
