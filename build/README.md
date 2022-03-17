# Building

This build setup is only working/tested for GNU/Linux systems. In order to have the required development dependencies, install them using your system's package manager. This is `libsfml-dev` on Debian-based systems and `sfml` on Arch-based systems. The package must be at version 2.5. Since septadrop now uses Rust bindings of SFML, `libcsfml-dev` needs to also be installed on Debian-based systems, and and `csfml` on Arch-based systems.

## Packaging

### Debian

To package:
```SH
./debian.sh
```