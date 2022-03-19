# Building

This build setup is only working/tested for GNU/Linux systems. In order to have the required development dependencies (C bindings for SFML), install them using your system's package manager. This is `libcsfml-dev` on Debian-based systems and `csfml` on Arch-based systems. The package must be at version 2.5.

## Packaging

### Debian

To package:
```SH
./debian.sh
```