# 🎯 Hardware Architecture

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Architecture_Document-brightgreen)

**Hardware Architecture**

**Smart Energy Management System - Hardware Components Overview**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [System Components](#-system-components)
- [Power Architecture](#-power-architecture)
- [Sensor Architecture](#-sensor-architecture)
- [Control Architecture](#-control-architecture)
- [Communication Architecture](#-communication-architecture)

---

## 🔗 Related Documentation

| Document                                                                       | Description                      | Status       |
| ------------------------------------------------------------------------------ | -------------------------------- | ------------ |
| **[HRS.md](../../02_Requirements/HRS/HRS.md)**                                 | Complete hardware specifications | ✅ Available |
| **[CRS.md](../../02_Requirements/CRS/CRS.md)**                                 | Customer Requirements            | ✅ Available |
| **[System_Block_Diagram.md](../System_Block_Diagram/System_Block_Diagram.md)** | System block diagrams            | ✅ Available |

---

## 🛠️ System Components

### 1. Microcontroller

- **ATmega32A-PU** - 8-bit AVR, 32 KB Flash, 16 MHz

### 2. Sensors

- **Voltage Divider** (470kΩ/4.7kΩ) + Rectifier → 0-5V DC
- **ACS712-30A** Hall-effect current sensor → 0-5V analog

### 3. Actuators

- **5V SPDT Relay** (10A @ 250VAC) via ULN2003 driver

### 4. Display & Indicators

- **LCD 16×2** (HD44780 compatible)
- **RGB LED** (common cathode)
- **Active Buzzer** (5V)

### 5. Communication

- **HC-05** Bluetooth (Version 1)
- **ESP-01** WiFi (Version 2)

### 6. Power Supply

- 5V main rail (LM7805 or direct input)
- 3.3V rail for comm modules (AMS1117-3.3)

---

## ⚡ Power Architecture

> **TBD**: Detailed power distribution architecture

---

## 🔍 Sensor Architecture

> **TBD**: Detailed sensor interfacing architecture

---

## 🎛️ Control Architecture

> **TBD**: Detailed control circuit architecture

---

## 📡 Communication Architecture

> **TBD**: Detailed communication module architecture

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

**LinkedIn Company Page**: https://www.linkedin.com/company/gestell-company

---

## 📄 Document Control

| Attribute            | Value                    |
| -------------------- | ------------------------ |
| **Document Type**    | Hardware Architecture    |
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
