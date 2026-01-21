# 🔋 Smart Energy Management System

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Embedded_System-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-CRS_Document-brightgreen)

**Customer Requirements Specification (CRS)**

**Real-Time Energy Monitoring and Protection System**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Introduction](#-introduction)
- [System Overview](#-system-overview)
- [Functional Requirements](#-functional-requirements)
- [Configuration & Parameterization](#-configuration--parameterization)
- [Hardware Requirements](#-hardware-requirements-ecu-level)
- [Firmware / Software Requirements](#-firmware--software-requirements)
- [Human Machine Interface (HMI)](#-human-machine-interface-hmi)
- [Communication Interfaces](#-communication-interfaces)
- [Safety & Protection Behavior](#-safety--protection-behavior)
- [Testing & Validation](#-testing--validation)
- [Deliverables](#-deliverables)
- [Out of Scope](#-out-of-scope)

---

## 🔗 Related Documentation

This CRS is supported by the following detailed specification documents:

| Document                                                                           | Description                         | Status       |
| ---------------------------------------------------------------------------------- | ----------------------------------- | ------------ |
| **[HRS.md](HRS/HRS.md)**                                                           | Hardware Requirements Specification | ✅ Available |
| **[SRS.md](SRS/SRS.md)**                                                           | Software Requirements Specification | ✅ Available |
| **[DashboardRequirementfromEmbedded.md](DashboardRequirementfromEmbedded.md)**     | Dashboard communication protocol    | ✅ Available |
| **[MobileRequirementfromEmbedded.md](MobileRequirementfromEmbedded.md)**           | Mobile App communication protocol   | ✅ Available |
| **[Configuration_Reference.md](AddationalInformation/Configuration_Reference.md)** | System configuration parameters     | 📅 Planned   |
| **[PinOut_Reference.md](AddationalInformation/PinOut_Reference.md)**               | ATmega32 pin assignment             | 📅 Planned   |

> 💡 **Note**: All architecture and reference documents are cross-linked with this CRS.

---

## 📖 Introduction

### 1.1 Purpose of This Document

This document defines the **Customer Requirements Specification (CRS)** for theدevelopment of a **Smart Energy Management System** based on embedded microcontroller technology.

The CRS describes **what will be designed and delivered**, not how it will be implemented internally.

It serves as the **technical and contractual reference** between the customer and the development team.

---

### 1.2 Product Scope

The product is a **standalone Energy Monitoring and Protection System** responsible for:

- Real-time AC voltage measurement
- Real-time AC current measurement
- Power and energy calculation
- Overcurrent and overvoltage protection
- Energy consumption tracking
- Data persistence (EEPROM)
- Communication with Mobile App and Web Dashboard
- Load control via relay
- User interface via LCD display

⚠️ This system **does not include**:

- Power conditioning or voltage regulation
- High-voltage power electronics
- Load equipment (appliances, machinery)
- Energy storage systems

---

### 1.3 Design Philosophy

The system shall be:

- **Cost-effective** for residential and small commercial applications
- **Modular** and easy to install
- **Configurable** via mobile app or web dashboard
- **Reliable** for 24/7 continuous operation
- Optimized for **local market availability** (Egypt)

---

## 🏗️ System Overview

### 2.1 System Functional Role

The system acts as the **central energy monitoring and protection unit**, performing:

- AC voltage and current measurement (RMS values)
- Real-time power calculation
- Cumulative energy consumption tracking
- Protection against electrical faults (overcurrent, overvoltage)
- Load control (relay switching)
- Data logging and persistence
- Remote monitoring capability

---

### 2.2 Supported System Versions

The system shall support two communication variants:

- **Version 1**: HC-05 Bluetooth (local monitoring via Mobile App)
- **Version 2**: ESP-01 WiFi (remote monitoring via Web Dashboard)

Version selection shall be configurable during installation.

---

## ✨ Functional Requirements

### 3.1 Voltage Measurement

- The system shall measure **AC RMS voltage** from 0V to 300V
- Measurement accuracy: **±2V** (≤1% at 220V)
- Update rate: **Every 100ms**
- Input method: Voltage divider circuit → ADC

---

### 3.2 Current Measurement

- The system shall measure **AC RMS current** using ACS712 Hall-effect sensor
- Current range: **0A to 30A**
- Measurement accuracy: **±0.1A** (≤1% at 10A)
- Update rate: **Every 100ms**
- Calibration: Zero-point calibration supported

---

### 3.3 Power Calculation

- The system shall calculate **instantaneous power** (P = V × I × PF)
- Power factor: Configurable (default: 0.95 for resistive loads)
- Power range: **0W to 5000W**
- Update rate: **Every 100ms**

---

### 3.4 Energy Tracking

- The system shall calculate **cumulative energy consumption**
- Formula: E = ∫P dt (integration of power over time)
- Unit: **kWh** (kilowatt-hours)
- Range: **0 to 9999 kWh**
- Persistence: Stored in EEPROM, survives power cycles
- Reset: Via command from Mobile App or Dashboard

---

### 3.5 Protection Functions

#### 3.5.1 Overcurrent Protection

- Detect when current exceeds configurable threshold (default: 20A)
- Debouncing: 3 consecutive readings above threshold
- Action: Disconnect load (relay OFF), display warning, send alert

#### 3.5.2 Overvoltage Protection

- Detect when voltage exceeds configurable threshold (default: 250V)
- Debouncing: 3 consecutive readings above threshold
- Action: Disconnect load (relay OFF), display warning, send alert

---

### 3.6 Load Control

- The system shall control load connection via relay
- Relay states: ON (load connected), OFF (load disconnected)
- Control sources:
  - Automatic (protection system)
  - Manual (via Mobile/Dashboard commands)
- Safety: Default state is OFF on power-up

---

## 🔧 Configuration & Parameterization

### 4.1 Configuration Concept

The system shall be **fully configurable** without hardware modification.

Configuration shall include:

- Protection thresholds (overcurrent, overvoltage)
- Power factor
- Display units
- Communication settings

Configuration shall be accessible via Mobile App or Web Dashboard.

---

### 4.2 Parameter Storage

- Parameters shall be stored in **EEPROM** (non-volatile memory)
- Parameters shall persist after power loss
- Factory reset shall be supported via protected command

---

## 🛠️ Hardware Requirements (System-Level)

### 5.1 Microcontroller

- Platform: **ATmega32** (8-bit AVR)
- Clock speed: **16 MHz**
- Flash: 32 KB
- SRAM: 2 KB
- EEPROM: 1 KB

---

### 5.2 Power Supply

- The system shall operate from **5V DC regulated supply**
- Input voltage tolerance: **4.5V to 5.5V**
- Reverse polarity protection required
- Over-voltage protection required

---

### 5.3 Sensors

- **Current sensor**: ACS712 (±30A, Hall-effect, isolated)
- **Voltage sensor**: Resistive voltage divider (step-down from 220V AC)
- **Sensor isolation**: Galvanic isolation for current measurement

---

### 5.4 Actuators

- **Relay**: 10A@ 250VAC SPDT relay for load control
- **Relay coil**: 5V or 12V DC (compatible with system power)
- **Driver**: Relay driver circuit (e.g., ULN2003/ULN2803)

---

### 5.5 Display

- **LCD**: 16×2 character LCD (HD44780-compatible)
- **Interface**: 4-bit mode (6 GPIO pins)
- **Content**: Voltage, current, power, energy, status

---

## 💻 Firmware / Software Requirements

### 6.1 Architecture

- Firmware shall be structured in **modular layers**:
  - MCAL: Microcontroller Abstraction Layer
  - HAL: Hardware Abstraction Layer
  - Application Layer: Measurement, protection, communication
- State machine-based control logic
- Real-time operation (100ms update cycle)

---

### 6.2 Measurement Algorithms

- **RMS calculation**: Sum of squares method for AC waveforms
- **Sampling rate**: 100 samples per second minimum
- **Digital filtering**: Moving average or low-pass filtering for noise reduction

---

### 6.3 Data Persistence

- Energy counter shall be saved to EEPROM every 1 minute or on significant change (>0.1 kWh)
- Calibration data shall be stored in EEPROM
- Configuration parameters shall be stored in EEPROM

---

### 6.4 Watchdog & Fault Recovery

- Optional: Hardware watchdog timer for system reliability
- Fault recovery strategy: Safe state (relay OFF) on critical error

---

## 🖥️ Human Machine Interface (HMI)

### 7.1 Local Display (LCD)

The system shall display:

- **Screen 1**: Voltage (V) and Current (A)
- **Screen 2**: Power (W) and Energy (kWh)
- **Screen 3**: System status (OK / OVERLOAD / OVERVOLT / FAULT)
- **Rotation**: Screens rotate every 2 seconds

---

### 7.2 Status Indicators

- **RGB LED**:
  - Green: Normal operation
  - Yellow: Warning
  - Red: Fault
- **Buzzer**: Audible alarms for protection events

---

## 📡 Communication Interfaces

### 8.1 Mobile App (Version 1)

- **Protocol**: Bluetooth Classic (HC-05 module)
- **Data format**: Scaled integers, Big-Endian
- **Baud rate**: 9600 bps
- **Update rate**: 1 second

See [MobileRequirementfromEmbedded.md](MobileRequirementfromEmbedded.md) for detailed protocol specification.

---

### 8.2 Web Dashboard (Version 2)

- **Protocol**: WiFi (ESP-01 module) + WebSocket
- **Data format**: Float32, Little-Endian
- **Baud rate**: 9600 bps (UART to ESP-01)
- **Update rate**: 1 second

See [DashboardRequirementfromEmbedded.md](DashboardRequirementfromEmbedded.md) for detailed protocol specification.

---

## 🛡️ Safety & Protection Behavior

### 9.1 Fail-Safe Principles

- Any protection trigger shall:
  - Disconnect load (relay OFF)
  - Display fault message
  - Log fault event
  - Send alert notification (if connected)
- System shall **never energize relay** under unsafe conditions
- Default power-up state: **Relay OFF** (safe idle)

---

### 9.2 Protection Response Time

- Maximum response time: **100ms** from fault detection to relay disconnection
- Debouncing ensures no false positives from noise or transients

---

## 🧪 Testing & Validation

### 10.1 Factory Acceptance Testing (FAT)

- Sensor calibration verification
- Protection threshold testing (overcurrent, overvoltage)
- Communication protocol validation
- EEPROM read/write testing
- Relay operation verification

---

### 10.2 Site Acceptance Testing (SAT)

- Integration with real AC load
- Accuracy verification against reference meter
- Protection function testing with actual faults
- Long-term stability test (24-hour continuous operation)

---

## 📦 Deliverables

The supplier shall deliver:

- Complete system hardware (PCB with components)
- Firmware binary (HEX file)
- User manual (operation and configuration)
- Service manual (troubleshooting and maintenance)
- Component datasheets
- FAT and SAT test reports

---

## 🚫 Out of Scope

The following are explicitly excluded from this project:

- Power quality analysis (harmonics, power factor correction)
- Multi-phase power measurement (3-phase systems)
- Energy billing or tariff calculation
- Grid synchronization or interconnection
- Battery backup or UPS functionality

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

**LinkedIn Company Page**: https://www.linkedin.com/company/gestell-company

---

## 📄 Document Control

| Attribute            | Value                                     |
| -------------------- | ----------------------------------------- |
| **Document Type**    | Customer Requirements Specification (CRS) |
| **Document Status**  | Approved                                  |
| **Document Version** | 1.0                                       |
| **Last Updated**     | January 2026                              |
| **Prepared By**      | Gestell Engineering Team                  |
| **Target Platform**  | ATmega32 Microcontroller                  |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
