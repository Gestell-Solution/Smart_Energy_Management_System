# 🏗️ Build Instructions

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-AVR_GCC-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Guide-brightgreen)

**Build Instructions**

**Smart Energy Management System - Firmware Build Process**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Quick Start](#-quick-start)
- [Requirements](#-requirements)
- [Build Process](#-build-process)

---

## 🔗 Related Documentation

| Document                                                     | Description         | Status       |
| ------------------------------------------------------------ | ------------------- | ------------ |
| **[Makefile_Documentation.md](./Makefile_Documentation.md)** | Variables & Targets | ✅ Available |
| **[Build_System_Setup.md](./Build_System_Setup.md)**         | Installation        | ✅ Available |

---

## 🚀 Quick Start

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

## 🛠️ Requirements

| Tool         | Description             | Need     |
| ------------ | ----------------------- | -------- |
| **avr-gcc**  | AVR C compiler          | Required |
| **avr-libc** | AVR C library           | Required |
| **avrdude**  | AVR programming utility | Required |
| **make**     | Build automation tool   | Required |

> [!TIP]
> See **[Build_System_Setup.md](Build_System_Setup.md)** for detailed installation instructions.

---

## 🔄 Build Process

1. **Compile**: C Source Files (`.c`) → Object Files (`.o`)
2. **Link**: Object Files (`.o`) → ELF Executable (`.elf`)
3. **Convert**: ELF Executable (`.elf`) → HEX File (`.hex`)

> [!NOTE]
> See **[Compilation_Guide.md](Compilation_Guide.md)** for detailed compiler flags and steps.

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
| **Document Type**    | Build Instructions       |
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
