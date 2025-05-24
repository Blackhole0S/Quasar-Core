# Quasar-core

**Quasar-Core** is the ultra-lightweight, ultra-fast init and service management system for [BlackholeOS](https://github.com/blackhole-os). It is a `systemd`-free replacement, designed to boot in milliseconds and manage services with minimal overhead — ideal for hardened, sandboxed, or anonymous systems.

---

## 🌌 Features

- ⚡ Blazing-fast init process (boot in under 1s)
- 🧠 Process supervision with zero-respawn lag
- 🔐 Built-in security context enforcement
- 📜 Fully scriptable with `quasarctl` (custom control CLI)
- 🧭 Seamless integration with `logincore`, `journalcore`, and `quasar-core-*` tools
- 🛡️ Designed for sandboxed + secure environments (AppArmor, seccomp, namespaces, etc.)

---

## 🛠 Build Instructions

### Prerequisites

- A Linux system with `make`, `gcc` or `clang`
- Optional: `musl`, `clang`, `LLVM` for hardening

### Build

```bash
make
sudo make install
