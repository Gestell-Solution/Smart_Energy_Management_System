# 🔧 Makefile Documentation

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-AVR_Build_System-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Build_Documentation-brightgreen)

**Makefile Documentation**

**Smart Energy Management System - Build System**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Makefile Targets](#-makefile-targets)
- [Key Variables](#-key-variables)
- [Example Makefile Snippet](#-example-makefile-snippet)

---

## 🔗 Related Documentation

| Document                                                       | Description    | Status       |
| -------------------------------------------------------------- | -------------- | ------------ |
| **[Build_Instructions.md](./Build_Instructions.md)**           | Build Process  | ✅ Available |
| **[Compilation_Guide.md](./Compilation_Guide.md)**             | Compiler Flags | ✅ Available |
| **[Flash_Programming_Guide.md](./Flash_Programming_Guide.md)** | Programming    | ✅ Available |

---

## 🎯 Makefile Targets

| Target  | Description                       |
| ------- | --------------------------------- |
| `all`   | Build firmware.hex (default)      |
| `flash` | Upload firmware to MCU via USBasp |
| `clean` | Remove all build files            |
| `size`  | Display memory usage              |
| `fuses` | Program fuse bits                 |

---

## ⚙️ Key Variables

```makefile
MCU = atmega32
F_CPU = 16000000UL
PROGRAMMER = usbasp
BAUDRATE = 9600
```

**Variable Descriptions**:

| Variable   | Value      | Purpose                      |
| ---------- | ---------- | ---------------------------- |
| MCU        | atmega32   | Target microcontroller model |
| F_CPU      | 16000000UL | CPU frequency (16 MHz)       |
| PROGRAMMER | usbasp     | Programmer hardware type     |
| BAUDRATE   | 9600       | Serial communication speed   |

---

## 📝 Example Makefile Snippet

```makefile
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -Os -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU)

firmware.hex: firmware.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: firmware.hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$<
```

**Makefile Components**:

| Component       | Description                                                                                           |
| --------------- | ----------------------------------------------------------------------------------------------------- |
| **Compiler**    | `avr-gcc` - AVR cross-compiler                                                                        |
| **Object Copy** | `avr-objcopy` - Creates HEX files from ELF                                                            |
| **Programmer**  | `avrdude` - Flash programming utility                                                                 |
| **CFLAGS**      | `-Os` (optimize size), `-Wall` (all warnings), `-mmcu` (target MCU), `-DF_CPU` (define CPU frequency) |

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
| **Document Type**    | Makefile Documentation   |
| **Document Status**  | Active                   |
| **Document Version** | 1.0                      |
| **Last Updated**     | January 2026             |
| **Prepared By**      | Gestell Engineering Team |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
