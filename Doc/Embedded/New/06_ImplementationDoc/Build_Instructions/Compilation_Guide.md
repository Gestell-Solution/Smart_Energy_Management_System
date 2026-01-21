# Compilation Guide

**Compiler**: avr-gcc  
**Target**: ATmega32

---

## Compiler Flags

**Optimization**: `-Os` (optimize for size)  
**Warnings**: `-Wall -Wextra` (all warnings)  
**MCU**: `-mmcu=atmega32`  
**Clock**: `-DF_CPU=16000000UL`

---

## Compilation Steps

### 1. Compile Source Files
```bash
avr-gcc -c -Os -Wall -Wextra -mmcu=atmega32 -DF_CPU=16000000UL main.c -o main.o
```

### 2. Link Object Files
```bash
avr-gcc -mmcu=atmega32 main.o [other .o files] -o firmware.elf
```

### 3. Generate HEX File
```bash
avr-objcopy -O ihex -R .eeprom firmware.elf firmware.hex
```

---

## Memory Usage

View memory usage:
```bash
avr-size -C --mcu=atmega32 firmware.elf
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026
