# 💻 Build System Setup

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Linux_Windows-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Setup_Guide-brightgreen)

**Build System Setup**

**Smart Energy Management System - Environment Configuration**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Linux/Ubuntu Installation](#-linuxubuntu-installation)
- [Verify Installation](#-verify-installation)
- [Alternative: Windows](#-alternative-windows)

---

## 🔗 Related Documentation

| Document                                             | Description   | Status       |
| ---------------------------------------------------- | ------------- | ------------ |
| **[Build_Instructions.md](./Build_Instructions.md)** | Build Process | ✅ Available |

---

## 🐧 Linux/Ubuntu Installation

Install the AVR toolchain and build utilities via APT package manager.

```bash
sudo apt-get update
sudo apt-get install avr-gcc avr-libc avrdude make
```

**Packages Included**:

- `avr-gcc`: Compiler
- `avr-libc`: Standard C library for AVR
- `avrdude`: Flashing tool
- `make`: Build automation

---

## ✅ Verify Installation

Run these commands to ensure tools are correctly installed and in your PATH.

```bash
# Check compiler version
avr-gcc --version

# Check programmer connection (ensure USBasp is plugged in)
avrdude -c usbasp -p m32
```

---

## 🪟 Alternative: Windows

For Windows development environments, install the following tools:

| Tool               | Recommended Source              |
| ------------------ | ------------------------------- |
| **WinAVR**         | Standard AVR toolchain package  |
| **Atmel Studio**   | Official IDE (Microchip Studio) |
| **USBasp Drivers** | Zadig (install libusb driver)   |

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
| **Document Type**    | Setup Guide              |
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
