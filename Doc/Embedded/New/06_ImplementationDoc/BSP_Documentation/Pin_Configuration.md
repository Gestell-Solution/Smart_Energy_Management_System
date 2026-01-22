# 📍 Pin Configuration

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Implementation_Doc-brightgreen)

**Pin Configuration**

**Smart Energy Management System - BSP Pin Mapping**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Complete Pin Mapping](#-complete-pin-mapping)
- [Key Pin Groups](#-key-pin-groups)
- [Initialization](#-initialization)

---

## 🔗 Related Documentation

| Document                                                                                   | Description           | Status       |
| ------------------------------------------------------------------------------------------ | --------------------- | ------------ |
| **[PinOut_Reference.md](../../02_Requirements/AddationalInformation/PinOut_Reference.md)** | Full Pin Reference    | ✅ Available |
| **[Pin_Assignment.md](../../04_Detailed_Design/Pin_Assignment/Pin_Assignment.md)**         | Design Pin Assignment | ✅ Available |

---

---

## Overview

All pin assignments are defined in `BSP_Config.h` and match the hardware design.

---

## Complete Pin Mapping

See [PinOut_Reference.md](../../02_Requirements/AddationalInformation/PinOut_Reference.md) for complete pin assignment table.

---

## Key Pin Groups

### Analog Inputs (PORTA)

- **PA0 (ADC0)**: Voltage sensor input
- **PA1 (ADC1)**: Current sensor (ACS712) input
- **PA2-PA7**: Reserved for expansion

### Digital Outputs (PORTB)

- **PB0**: Relay control (via ULN2003)
- **PB1 (OC1A)**: RGB Red (PWM)
- **PB2 (OC1B)**: RGB Green (PWM)
- **PB3**: RGB Blue
- **PB4**: Buzzer control
- **PB5-PB7**: ISP programming interface

### Reserved (PORTC)

- **PC0-PC7**: All reserved for future use (buttons, I2C, etc.)

### Communication & Display (PORTD)

- **PD0 (RXD)**: UART receive
- **PD1 (TXD)**: UART transmit
- **PD2-PD7**: LCD interface (4-bit mode)

---

## Initialization

All pins configured in `DIO_Init()` called from `BSP_Init()`.

**Safe State**: All outputs default to LOW on power-up.

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
| **Document Type**    | Pin Configuration        |
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
