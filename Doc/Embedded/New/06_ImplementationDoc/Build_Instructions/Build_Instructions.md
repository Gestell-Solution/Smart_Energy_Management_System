# Build Instructions

**Project**: Smart Energy Management System  
**Toolchain**: AVR-GCC  
**Programmer**: USBasp

---

## Quick Start

```bash
# Build firmware
make all

# Upload to MCU
make flash

# Clean build files
make clean

# View memory usage
make size
```

---

## Requirements

- **avr-gcc**: AVR C compiler
- **avr-libc**: AVR C library  
- **avrdude**: AVR programming utility
- **make**: Build automation tool

See [Build_System_Setup.md](Build_System_Setup.md) for installation.

---

## Build Process

1. **Compile**: C files → Object files (.o)
2. **Link**: Object files → ELF executable
3. **Convert**: ELF → HEX file for programming

See [Compilation_Guide.md](Compilation_Guide.md) for details.

---

**Document Version**: 1.0  
**Last Updated**: January 2026
