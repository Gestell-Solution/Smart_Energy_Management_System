# ⚡ Interrupt Management

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-AVR_Interrupts-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Configuration-brightgreen)

**Interrupt Management**

**Smart Energy Management System - ISR Configuration**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Enabled Interrupts](#-enabled-interrupts)
- [ISR Rules](#-isr-rules)
- [Priority](#-priority)

---

## 🔗 Related Documentation

| Document                                                    | Description | Status       |
| ----------------------------------------------------------- | ----------- | ------------ |
| **[Timing_and_Scheduling.md](Timing_and_Scheduling.md)**    | Scheduling  | ✅ Available |
| **[BSP_Overview.md](../BSP_Documentation/BSP_Overview.md)** | BSP         | ✅ Available |

---

## Enabled Interrupts

1. **Timer1 COMPA**: ADC sampling trigger (100 Hz)
2. **UART RX**: Command reception (asynchronous)

---

## ISR Rules

- Keep **short** (<100 µs execution)
- **No blocking** operations
- Set flags only, process in main loop
- **No floating-point** in ISR (use integer math)

---

## Priority

Hardware fixed priority (lowest vector number = highest priority):

1. RESET
2. INT0/INT1
3. Timer1 COMPA ← **Used (ADC sampling)**
4. ...
5. UART RX ← **Used (commands)**

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
| **Document Type**    | Interrupt Management     |
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
