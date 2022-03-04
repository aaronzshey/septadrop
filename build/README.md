# Building

This build setup is only working/tested for GNU/Linux systems. In order to have the required development dependencies, install them using your system's package manager. This is `libsfml-dev` on Debian-based systems and `sfml` on Arch-based systems (untested). They package must be at version 2.5.

First, `cd` into this folder, then run Cmake to generate the Makefile. If you ever update `CMakeLists.txt`, run this command again.

```SH
cmake ..
```

To compile and run run, replacing "sfml-template" with your `ProjectName` in `CmakeLists.txt`:

```SH
make && ./sfml-template
```