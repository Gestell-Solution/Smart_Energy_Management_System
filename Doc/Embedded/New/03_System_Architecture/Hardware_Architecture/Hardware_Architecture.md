# Hardware Architecture

**Project**: Smart Energy Management System  
**Version**: 1.0

## System Components

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

## See Also
- [HRS.md](../../02_Requirements/HRS/HRS.md) - Complete hardware specifications
- [System_Block_Diagram.md](../System_Block_Diagram/System_Block_Diagram.md)
