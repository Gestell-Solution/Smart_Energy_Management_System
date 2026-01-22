# 🔨 Compilation Guide

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-AVR_GCC-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Technical_Guide-brightgreen)

**Compilation Guide**

**Smart Energy Management System - Compiler Settings & Process**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Compiler Flags](#-compiler-flags)
- [Compilation Steps](#-compilation-steps)
- [Memory Usage](#-memory-usage)

---

## 🚩 Compiler Flags

**Toolchain**: `avr-gcc`  
**Target MCU**: `ATmega32`

| Flag             | Value                | Description                                 |
| ---------------- | -------------------- | ------------------------------------------- |
| **Optimization** | `-Os`                | Optimize for size (critical for 32KB Flash) |
| **Warnings**     | `-Wall -Wextra`      | Enable all standard warnings                |
| **MCU Target**   | `-mmcu=atmega32`     | Specify microcontroller architecture        |
| **Clock Def**    | `-DF_CPU=16000000UL` | Define system clock frequency (16 MHz)      |

---

## ⚙️ Compilation Steps

### 1. Compile Source Files

Compiles C source code into object files.

```bash
avr-gcc -c -Os -Wall -Wextra -mmcu=atmega32 -DF_CPU=16000000UL main.c -o main.o
```

### 2. Link Object Files

Links all object files into a single executable (ELF).

```bash
avr-gcc -mmcu=atmega32 main.o [other .o files] -o firmware.elf
```

### 3. Generate HEX File

Extracts binary code from ELF to specialized HEX format for flashing.

```bash
avr-objcopy -O ihex -R .eeprom firmware.elf firmware.hex
```

---

## 📊 Memory Usage

Check the RAM and Flash usage of the compiled firmware.

```bash
avr-size -C --mcu=atmega32 firmware.elf
```

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

---

## 📄 Document Control

| Attribute            | Value                    |
| -------------------- | ------------------------ |
| **Document Type**    | Compilation Guide        |
| **Document Status**  | Active                   |
| **Document Version** | 2.0                      |
| **Last Updated**     | January 2026             |
| **Prepared By**      | Gestell Engineering Team |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
