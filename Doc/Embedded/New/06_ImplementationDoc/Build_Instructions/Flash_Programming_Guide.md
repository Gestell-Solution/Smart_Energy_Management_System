# ⚡ Flash Programming Guide

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-AVRDUDE-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Operation_Guide-brightgreen)

**Flash Programming Guide**

**Smart Energy Management System - Firmware Uploading**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Hardware Connection](#-hardware-connection)
- [Programming Command](#-programming-command)
- [Fuse Programming](#-fuse-programming)
- [Troubleshooting](#-troubleshooting)

---

## 🔌 Hardware Connection

**Programmer**: USBasp (ISP)  
**Header**: standard 6-pin ISP

| Pin | Color (Typical) | Function                         |
| --- | --------------- | -------------------------------- |
| 1   | Red             | **VCC** (Target Power Detection) |
| 2   | -               | **MISO** (Master In Slave Out)   |
| 3   | -               | **SCK** (Serial Clock)           |
| 4   | -               | **MOSI** (Master Out Slave In)   |
| 5   | -               | **RESET** (Target Reset)         |
| 6   | Black           | **GND** (Ground)                 |

> [!IMPORTANT]
> **Power Supply**: The board must be powered separately (5V external supply). The USBasp may not provide sufficient current for the entire system.

---

## 💻 Programming Command

Upload the compiled firmware HEX file to the microcontroller.

```bash
avrdude -c usbasp -p m32 -U flash:w:firmware.hex
```

**Command Options**:

| Option | Value              | Description                          |
| ------ | ------------------ | ------------------------------------ |
| `-c`   | `usbasp`           | Programmer hardware type             |
| `-p`   | `m32`              | Target MCU (ATmega32)                |
| `-U`   | `flash:w:file.hex` | Memory operation: Write HEX to Flash |

---

## ⚙️ Fuse Programming

Fuses configure system clock and boot options. **Set these only once.**

### Set Fuses

External Crystal (16 MHz), JTAG Disabled, BOD Enabled.

```bash
avrdude -c usbasp -p m32 -U lfuse:w:0xFF:m -U hfuse:w:0xC9:m
```

### Verify Fuses

```bash
avrdude -c usbasp -p m32 -U lfuse:r:-:h -U hfuse:r:-:h
```

**Expected Values**:

- **LFUSE**: `0xFF`
- **HFUSE**: `0xC9`

---

## 🔧 Troubleshooting

| Error Message          | Possible Cause      | Solution                                            |
| ---------------------- | ------------------- | --------------------------------------------------- |
| `Device not found`     | Connection issue    | Check 6-pin cable, verify GND connection            |
| `Target not compliant` | Power issue         | Ensure board has external 5V power                  |
| `Verification failed`  | Clock issue         | Check crystal oscillator and capacitors             |
| `Permission denied`    | Drivers/Permissions | Use `sudo` (Linux) or check Zadig drivers (Windows) |

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
| **Document Type**    | Programming Guide        |
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
