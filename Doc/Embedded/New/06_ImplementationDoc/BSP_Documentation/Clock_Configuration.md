# ⏱️ Clock Configuration

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Implementation_Doc-brightgreen)

**Clock Configuration**

**Smart Energy Management System - System Clock Setup**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Crystal Oscillator](#-crystal-oscillator-configuration)
- [CPU Clock](#-cpu-clock)
- [Peripheral Clocks](#-peripheral-clocks)
- [Fuse Configuration](#-fuse-configuration-for-clock)

---

## 🔗 Related Documentation

| Document                                                                   | Description      | Status       |
| -------------------------------------------------------------------------- | ---------------- | ------------ |
| **[BSP_Overview.md](BSP_Overview.md)**                                     | BSP Overview     | ✅ Available |
| **[Peripheral_Configuration_Guide.md](Peripheral_Configuration_Guide.md)** | Peripheral Setup | ✅ Available |

---

---

## Crystal Oscillator Configuration

**Type**: External crystal oscillator  
**Frequency**: 16 MHz  
**Load Capacitors**: 22pF each (to ground)

**Connections**:

- Crystal between XTAL1 (Pin 13) and XTAL2 (Pin 12)
- 22pF capacitor from XTAL1 to GND
- 22pF capacitor from XTAL2 to GND

---

## CPU Clock

**F_CPU**: 16,000,000 Hz (16 MHz)  
**Prescaler**: None (CLKDIV8 fuse unprogrammed)

---

## Peripheral Clocks

All peripherals derive from CPU clock:

| Peripheral | Clock Source | Prescaler         | Effective Frequency |
| ---------- | ------------ | ----------------- | ------------------- |
| **CPU**    | Crystal      | 1:1               | 16 MHz              |
| **ADC**    | CPU          | /128              | 125 kHz             |
| **Timer0** | CPU          | Configurable      | Variable            |
| **Timer1** | CPU          | Configurable      | Variable            |
| **UART**   | CPU          | Baud rate divider | -                   |

---

## Fuse Configuration for Clock

**CKSEL[3:0]** = 1111: External crystal oscillator, >8 MHz  
**SUT[1:0]** = 10: Slowly rising power  
**CKOPT** = 0: Enable full swing for high frequencies

**Result**: Stable 16 MHz operation with optimal startup time.

---

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
| **Document Type**    | Clock Configuration      |
| **Document Status**  | Active                   |
| **Document Version** | 1.0                      |
| **Last Updated**     | January 2026             |
| **Prepared By**      | Gestell Engineering Team |
| **Target Platform**  | ATmega32 Microcontroller |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
