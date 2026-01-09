# 🔋 Smart Energy Management System

<div align="center">

![Status](https://img.shields.io/badge/Status-Complete-success)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)

**Advanced IoT-Enabled Energy Monitoring & Control System**

*Developed by Gestell Company - Professional Embedded Solutions*

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Features](#-features)
- [Hardware Components](#-hardware-components)
- [Software Components](#-software-components)
- [Getting Started](#-getting-started)
- [Project Structure](#-project-structure)
- [Development Team](#-development-team)
- [Documentation](#-documentation)
- [License](#-license)

---

## 🎯 Overview

The **Smart Energy Management System** is a comprehensive solution for real-time electricity monitoring and intelligent load control. Built on the ATmega32 microcontroller, it combines embedded firmware, mobile application, and web dashboard to provide complete energy visibility and control.

### Key Capabilities

- ⚡ **Real-time Monitoring**: Voltage, Current, Power, and Energy consumption
- 📱 **Mobile App**: Native Flutter application for Android/iOS
- 🌐 **Web Dashboard**: Professional browser-based monitoring interface
- 🔌 **Smart Control**: 4-channel relay control with overload protection
- 📊 **Data Logging**: Historical energy consumption tracking
- 🔐 **Protection**: Automatic overload and overvoltage cutoff
- 📡 **Dual Connectivity**: Bluetooth (local) + WiFi (remote IoT)

---

## 🏗️ System Architecture

```text
┌─────────────────────────────────────────────────────────────┐
│                     USER INTERFACES                          │
├──────────────────┬──────────────────┬───────────────────────┤
│  Mobile App      │  Web Dashboard   │  LCD Display          │
│  (Flutter)       │  (HTML/JS/CSS)   │  (16x4 Character)     │
│  📱 Local BT     │  🌐 Remote IoT   │  🔍 Local Display     │
└────────┬─────────┴────────┬─────────┴──────────┬────────────┘
         │                  │                     │
         │ Bluetooth        │ WiFi (Optional)     │ Direct
         │ (HC-05)          │ (ESP-01)            │
         │                  │                     │
┌────────▼──────────────────▼─────────────────────▼────────────┐
│              EMBEDDED SYSTEM (ATmega32)                      │
├──────────────────────────────────────────────────────────────┤
│  Application Layer:                                          │
│  • Measurement Engine  • Protection Manager                  │
│  • Display Manager     • Communication Manager               │
│  • Energy Logger       • System Controller                   │
├──────────────────────────────────────────────────────────────┤
│  HAL Layer:                                                  │
│  • ACS712 Current Sensor  • Voltage Divider                  │
│  • LCD Driver            • HC-05 Bluetooth                    │
│  • Relay Module (4ch)    • RGB LED + Buzzer                  │
├──────────────────────────────────────────────────────────────┤
│  MCAL Layer:                                                 │
│  • ADC  • UART  • Timers  • DIO  • EEPROM  • Interrupts     │
└──────────────────────────────────────────────────────────────┘
         │                  │                     │
         │ ADC              │ Protection          │ Control
         │                  │                     │
┌────────▼──────────────────▼─────────────────────▼────────────┐
│                    POWER SYSTEM                              │
│  220V AC → Sensors → Measurement → 4× Relays → Loads        │
└──────────────────────────────────────────────────────────────┘
```

---

## ✨ Features

### 🔬 Measurement & Monitoring

- **Voltage Measurement**: 0-250V AC with 0.1V resolution
- **Current Measurement**: 0-20A with 0.01A resolution using ACS712-20A
- **Power Calculation**: Real-time active power (W)
- **Energy Logging**: Cumulative consumption in kWh
- **RMS Calculation**: True RMS for AC signals
- **Sampling Rate**: 100 Hz (configurable)

### 🛡️ Protection Features

- **Overcurrent Protection**: Automatic cutoff at configurable threshold (default: 10A)
- **Overvoltage Protection**: Configurable limit (default: 250V)
- **Overpower Protection**: Maximum power limit
- **Visual Alerts**: RGB LED status indication
- **Audio Alerts**: Buzzer for critical events
- **Manual Reset**: Safety button for system restart

### 📱 Mobile Application (Flutter)

- **Live Dashboard**: Real-time energy metrics with animated gauges
- **Historical Charts**: Power consumption graphs
- **Alert System**: Push notifications for overload events
- **Relay Control**: Toggle individual loads remotely
- **Energy Reset**: Clear accumulated energy counter
- **Bluetooth Connectivity**: Direct device pairing (HC-05)
- **Cross-Platform**: Android & iOS support

### 🌐 Web Dashboard

- **Professional UI**: Modern, responsive design
- **Dual Mode**: 
  - Local (Bluetooth via Web Serial API)
  - Remote (WiFi via WebSocket)
- **Live Charts**: Real-time power visualization
- **Statistics**: Uptime, average power, peak power, cost estimation
- **4× Relay Control**: Independent channel control
- **WiFi Provisioning**: Configure ESP-01 credentials
- **Calibration**: Sensor calibration interface
- **System Logs**: Event logging and debugging

### 🔌 Load Control

- **4-Channel Relays**: Independent control of 4 loads
- **Automatic Cutoff**: Protection-triggered disconnect
- **Manual Override**: User-controlled switching
- **Runtime Tracking**: Per-relay usage statistics
- **Master Control**: All-on / All-off commands

### 💾 Data Persistence

- **EEPROM Storage**: Configuration and calibration data
- **Energy Counter**: Persistent across power cycles
- **Configuration Backup**: Factory reset capability
- **Calibration Factors**: Stored voltage/current correction

---

## 🛠️ Hardware Components

### Core Components

| Component            | Specification                       | Quantity |
|----------------------|-------------------------------------|----------|
| **Microcontroller**  | ATmega32-16MHz                      | 1        |
| **Current Sensor**   | ACS712-20A                          | 1        |
| **Voltage Sensor**   | Resistive Divider (1MΩ/10kΩ)        | 1        |
| **Display**          | LCD 16×4 Character (I2C compatible) | 1        |
| **Relay Module**     | 5V 4-Channel Opto-isolated          | 1        |
| **Bluetooth Module** | HC-05 (SPP profile)                 | 1        |
| **WiFi Module**      | ESP-01 (ESP8266) *Optional*         | 1        |
| **Status LED**       | RGB Common Cathode                  | 1        |
| **Alert Buzzer**     | 5V Active Buzzer                    | 1        |
| **Push Button**      | Reset/Mode Switch                   | 2        |
| **Power Supply**     | 220V AC → 5V DC 2A Regulated        | 1        |

### Pin Mapping (ATmega32)

| Function | Pin | Port | Note |
|----------|-----|------|------|
| **Current Sensor (ACS712)** | PA0 | ADC0 | Analog input |
| **Voltage Divider** | PA1 | ADC1 | Analog input |
| **LCD Data D4-D7** | PC0-PC3 | PORTC | 4-bit mode |
| **LCD RS** | PC4 | PORTC | Register select |
| **LCD EN** | PC5 | PORTC | Enable |
| **Relay 1** | PD4 | PORTD | Active HIGH |
| **Relay 2** | PD5 | PORTD | Active HIGH |
| **Relay 3** | PD6 | PORTD | Active HIGH |
| **Relay 4** | PD7 | PORTD | Active HIGH |
| **RGB LED R** | PB0 | PORTB | PWM capable |
| **RGB LED G** | PB1 | PORTB | PWM capable |
| **RGB LED B** | PB2 | PORTB | PWM capable |
| **Buzzer** | PB3 | PORTB | PWM for tones |
| **Reset Button** | PD2 | PORTD | INT0 (External interrupt) |
| **HC-05 TX** | PD0 | UART RX | Serial communication |
| **HC-05 RX** | PD1 | UART TX | Serial communication |

---

## 💻 Software Components

### 1. Embedded Firmware (C - AVR)

**Architecture**: Layered (MCAL → HAL → App)

**MCAL Layer** (Microcontroller Abstraction):

- ADC Driver (Asynchronous, Round-Robin)
- UART Driver (Interrupt-based)
- Timer0 (Scheduling)
- Timer1 (ADC Triggering)
- DIO (Digital I/O)
- EEPROM (Non-volatile storage)
- External Interrupts
- SPI, I2C (TWI)

**HAL Layer** (Hardware Abstraction):

- Current Sensor (ACS712)
- Voltage Sensor
- LCD Driver (16×4)
- HC-05 Bluetooth
- ESP-01 WiFi *(optional)*
- Relay Control
- RGB LED
- Buzzer
- Push Button

**Application Layer**:

- **Measurement Engine**: RMS calculation, power/energy computation
- **Protection Manager**: Threshold monitoring, safety cutoff
- **Display Manager**: LCD UI, menu system
- **Communication Manager**: Protocol handling (Bluetooth/WiFi)
- **Energy Logger**: EEPROM data logging
- **Calibration Manager**: Sensor calibration
- **System Controller**: Main state machine and coordinator

**Protocol**: Custom framed protocol

```text
[0xAA][LENGTH][COMMAND][DATA...]
```

### 2. Mobile Application (Flutter/Dart)

**Platform**: Android & iOS (Flutter 3.x)

**Key Packages**:

- `flutter_blue_plus`: Bluetooth Low Energy
- `fl_chart`: Charts and graphs
- `provider`: State management
- `shared_preferences`: Local storage

**Screens**:

- Dashboard: Live metrics, gauges
- History: Power consumption charts
- Alerts: Event notifications
- Settings: Configuration, relay control

**Communication**:

- Bluetooth SPP (Classic Bluetooth via HC-05)
- Frame-based protocol
- Periodic data polling (1 Hz)

### 3. Web Dashboard (HTML/CSS/JavaScript)

**Technologies**:

- HTML5 + CSS3 (Modern responsive design)
- Vanilla JavaScript (ES6+)
- Chart.js (Real-time graphs)
- Socket.io (WebSocket for remote mode)
- Web Serial API (Chrome/Edge for local Bluetooth)

**Modes**:

- **Local Mode**: Direct serial connection via Web Serial API (Bluetooth)
- **Remote Mode**: WebSocket connection to IoT server (WiFi)

**Features**:

- Real-time monitoring dashboard
- Interactive relay controls
- Live power chart
- Statistics panel
- WiFi provisioning interface
- Calibration tools
- Event logging

### 4. IoT Server (Node.js)

**Location**: `Dashboard/server/`

**Purpose**: Bridge between ESP-01 (WiFi) and Web Dashboard

**Functionality**:

- TCP Server (Port 3001): Receives data from ESP-01
- HTTP/WebSocket Server (Port 3000): Serves Dashboard
- Bidirectional relay: Dashboard ↔ ESP-01

**Dependencies**:

- `express`: HTTP server
- `socket.io`: WebSocket communication
- `net`: TCP server

---

## 🚀 Getting Started

### Prerequisites

**For Embedded Development**:

- AVR Toolchain (avr-gcc, avr-libc)
- Programmer (USBasp, Arduino as ISP)
- Proteus (for simulation) or physical hardware

**For Mobile App**:

- Flutter SDK (3.x or higher)
- Android Studio / Xcode
- Connected Android/iOS device

**For Web Dashboard**:

- Modern web browser (Chrome/Edge for Web Serial)
- Node.js 14+ (for remote mode server)

### Installation

#### 1. Embedded Firmware

```bash
# Clone repository
git clone <repository-url>
cd Smart_Energy_Management_System

# Compile firmware
cd <project-root>
make clean
make all

# Flash to ATmega32
make flash
```

#### 2. Mobile Application

```bash
# Navigate to mobile app
cd mobile_app

# Get dependencies
flutter pub get

# Run on connected device
flutter run

# Build APK (Android)
flutter build apk

# Build iOS
flutter build ios
```

See `mobile_app/FLUTTER_INSTALL.md` for detailed setup.

#### 3. Web Dashboard

**Local Mode** (Bluetooth):

```bash
cd Dashboard
open index.html  # Or use Live Server in VS Code
```

**Remote Mode** (WiFi):

```bash
# Install server dependencies
cd Dashboard/server
npm install

# Start IoT server
node index.js
# Server runs on http://localhost:3000

# Open dashboard
cd ..
open index.html
```

See `Dashboard/README.md` for full setup guide.

---

## 📁 Project Structure

```text
Smart_Energy_Management_System/
├── 📂 App/                          # Application Layer
│   ├── CalibrationManager/
│   ├── CommunicationManager/
│   ├── DisplayManager/
│   ├── EnergyLogger/
│   ├── MeasurementEngine/
│   ├── ProtectionManager/
│   └── System_Controller/
├── 📂 Hal/                          # Hardware Abstraction Layer
│   ├── ACS712/
│   ├── VoltageSensor/
│   ├── LCD/
│   ├── HC05/
│   ├── ESP01/
│   ├── Relay/
│   ├── RGB_LED/
│   ├── Buzzer/
│   └── PushButton/
├── 📂 Mcal/                         # Microcontroller Abstraction Layer
│   ├── ADC/
│   ├── UART/
│   ├── Timer0/
│   ├── Timer1/
│   ├── DIO/
│   ├── EEPROM/
│   ├── EXTI/
│   ├── SPI/
│   ├── TWI/
│   └── GIE/
├── 📂 Common/                       # Shared utilities
│   ├── SystemDataManager/
│   └── Config/
├── 📂 Dashboard/                    # Web Dashboard
│   ├── index.html
│   ├── script.js
│   ├── style.css
│   ├── server/                      # IoT Server (Node.js)
│   │   ├── index.js
│   │   └── package.json
│   └── README.md
├── 📂 mobile_app/                   # Flutter Mobile App
│   ├── lib/
│   │   ├── screens/
│   │   ├── services/
│   │   ├── models/
│   │   ├── config/
│   │   └── main.dart
│   ├── android/
│   ├── ios/
│   ├── pubspec.yaml
│   └── README.md
├── 📂 Doc/                          # Documentation
│   ├── Embedded/
│   │   ├── Code_Analysis_ReportV2.md
│   │   ├── DashboardRequirementfromEmbedded.md
│   │   ├── MobileRequirementfromEmbedded.md
│   │   └── Training_Learning_Outcomes.md
│   ├── Dashboard/
│   ├── Mobile/
│   ├── Requirment/
│   └── Doxygen/                     # Generated API docs
├── 📂 Design/                       # Circuit diagrams, schematics
├── 📂 Tests/                        # Unit tests
├── main.c                           # Firmware entry point
├── Makefile
└── Readme.md                        # This file
```

---

## 👥 Development Team

### Gestell Company - Embedded Training Program

**Duration**: 2 Months (8 Weekly Sprints)  
**Methodology**: Agile/Scrum  
**Project Lead**: Eng. Hesham Ahmed

### Core Development Team

1. **Mohamed Diaa** - mohammediaato@gmail.com
   - Communication Manager, Timer Drivers
2. **Ahmed Ashraf** - ahmedashraf2022222@gmail.com
   - ADC Driver, UART, EEPROM
3. **Mohamed Abdelgaber** - mohamedabdelgaber247@gmail.com
   - DIO Driver, External Interrupts, TWI
4. **Basma Khaled** - basmak55@gmail.com
   - UART, SPI, Code Reviews
5. **Ahmed Twap** - ahmedtwap2@gmail.com
   - SPI Driver

### Gestell Team

- **Hesham Ahmed** - (Lead) 
- **Alaa Emad** - (Scrum Master)
- **Aya Gamal** - (Mobile App Developer)
- **Hassan Elsayed** - (Web Dashboard Developer)

---

## 📚 Documentation

Comprehensive documentation is available in the `Doc/` directory:

### Embedded System

- **Code Analysis Report**: `Doc/Embedded/Code_Analysis_ReportV2.md`
  - In-depth analysis of all MCAL, HAL, and App modules
  - Bug reports and optimization recommendations
  - Performance metrics and testing strategies
  
- **Dashboard Integration Guide**: `Doc/Embedded/DashboardRequirementfromEmbedded.md`
  - Protocol specifications for web dashboard
  - Required command implementations
  - Data format specifications
  
- **Mobile Integration Guide**: `Doc/Embedded/MobileRequirementfromEmbedded.md`
  - Mobile app communication protocol
  - Binary data format and byte ordering
  - Command reference

- **Training Outcomes**: `Doc/Embedded/Training_Learning_Outcomes.md`
  - Skills developed during training
  - Project milestones and achievements

### API Documentation

- **Doxygen HTML**: `Doc/Doxygen/html/index.html`
  - Complete API reference for all embedded modules
  - Generated from inline code documentation

### Application Guides

- **Mobile App**: `mobile_app/README.md`
  - Setup instructions
  - Build and deployment guide
  - Feature documentation

- **Web Dashboard**: `Dashboard/README.md`
  - Installation and configuration
  - Usage guide for both local and remote modes

---

## 🔧 Configuration

### System Parameters (Embedded)

Located in `Common/SystemDataManager/SystemDataManager.h`:

```c
#define DEFAULT_OVERVOLTAGE_LIMIT    250   // Volts
#define DEFAULT_OVERCURRENT_LIMIT    10    // Amperes
#define DEFAULT_VOLTAGE_CALIB        1000  // Calibration factor
#define DEFAULT_CURRENT_CALIB        1000  // Calibration factor
```

### Communication Settings

**Bluetooth (HC-05)**:

- Baud Rate: 9600
- Mode: Slave
- Name: "HC-05" (configurable)

**Protocol**:

- Frame Header: 0xAA
- Commands: See `App/CommunicationManager/App_CommManager.h`

---

## 🧪 Testing

### Unit Tests

Located in `Tests/` directory with Google Test framework.

### Integration Tests

- Bluetooth connectivity test
- ADC calibration verification
- Protection threshold testing
- EEPROM read/write verification

### System Tests

- 24-hour continuous operation test
- Overload protection simulation
- Energy accuracy verification
- Multi-client connectivity test

---

## 📊 Performance Metrics

- **ADC Sampling**: 100 Hz per channel (configurable)
- **CPU Utilization**: ~15% average
- **Response Time**: <100ms for commands
- **Energy Accuracy**: ±2% (after calibration)
- **Voltage Accuracy**: ±0.5V
- **Current Accuracy**: ±50mA

---

## 🐛 Known Issues & Limitations

### Critical (Fixed in Latest Version)

- ~~Timer1 duplicate ISR callback~~ ✅ Fixed
- ~~Frame parsing hardcoded length~~ ✅ Fixed

### Limitations

- Single-phase AC only (no 3-phase support)
- Maximum load: 20A per channel
- WiFi requires external ESP-01 module
- No power factor measurement (active power only)

---

## 🛣️ Roadmap

### Future Enhancements

- [ ] Power factor measurement
- [ ] Harmonic analysis
- [ ] Cloud integration (AWS IoT, Azure IoT Hub)
- [ ] Machine learning for consumption prediction
- [ ] Mobile app for iOS
- [ ] RESTful API for third-party integrations
- [ ] OTA (Over-The-Air) firmware updates
- [ ] Multi-device support (gateway mode)

---

## 📄 License

**Copyright © 2025 Gestell Company**

This project is developed as part of Gestell's professional embedded training program.

**Proprietary Software** - All rights reserved.

For licensing inquiries, contact: Hisham4Ahmed@gmail.com

---

## 📞 Support & Contact

**Technical Support**: Hisham4Ahmed@gmail.com

**Project Repository**: https://github.com/Gestell-Solution/Smart_Energy_Management_System

**LinkedIn Company Page**: https://www.linkedin.com/company/gestell-company 

---

## 🙏 Acknowledgments

- **Gestell Company** for providing resources and mentorship
- **Development Team** for their dedication and hard work
- **Reviewers** for ensuring code quality and best practices
- **Open Source Community** for tools and libraries used

---

<div align="center">

**Built with ❤️ by Gestell Team**

*Empowering the next generation of embedded systems engineers*

</div>
