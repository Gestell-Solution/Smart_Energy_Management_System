# Configuration Reference

**Project**: Smart Energy Management System  
**Component**: Embedded System Configuration Parameters  
**Version**: 1.0

---

## 1. Overview

This document defines all configurable parameters for the Smart Energy Management System. Parameters are stored in EEPROM and can be modified via Mobile App or Web Dashboard.

---

## 2. Configuration Parameters

### 2.1 Measurement Calibration

| Parameter                  | Type   | Range     | Default | Unit       | EEPROM Address |
| -------------------------- | ------ | --------- | ------- | ---------- | -------------- |
| Voltage Calibration Factor | float  | 0.5 - 2.0 | 1.0     | multiplier | 0x0004         |
| Current Calibration Factor | float  | 0.5 - 2.0 | 1.0     | multiplier | 0x0006         |
| Current Zero Offset        | uint16 | 0 - 1023  | 512     | ADC counts | 0x0008         |
| Power Factor               | float  | 0.5 - 1.0 | 0.95    | -          | 0x000A         |

**Description**:

- **Voltage Calibration Factor**: Multiplier to correct voltage readings against reference meter
- **Current Calibration Factor**: Multiplier to correct current readings
- **Current Zero Offset**: ADC value at zero current (ACS712 output at no load)
- **Power Factor**: Assumed power factor for power calculation (P = V × I × PF)

---

### 2.2 Protection Thresholds

| Parameter                 | Type  | Range      | Default | Unit     | EEPROM Address |
| ------------------------- | ----- | ---------- | ------- | -------- | -------------- |
| Overcurrent Threshold     | float | 5.0 - 30.0 | 20.0    | A        | 0x000C         |
| Overvoltage Threshold     | float | 200 - 300  | 250     | V        | 0x000E         |
| Protection Debounce Count | uint8 | 1 - 10     | 3       | readings | 0x0010         |

**Description**:

- **Overcurrent Threshold**: Current level above which overcurrent protection triggers
- **Overvoltage Threshold**: Voltage level above which overvoltage protection triggers
- **Protection Debounce Count**: Number of consecutive readings above threshold required before triggering

---

### 2.3 User Interface Configuration

| Parameter             | Type  | Range   | Default | Unit      | EEPROM Address |
| --------------------- | ----- | ------- | ------- | --------- | -------------- |
| Display Rotation Time | uint8 | 1 - 10  | 2       | seconds   | 0x0011         |
| Buzzer Enable         | bool  | ON/OFF  | ON      | -         | 0x0012 (bit 0) |
| RGB Brightness        | uint8 | 0 - 255 | 255     | PWM value | 0x0013         |

**Description**:

- **Display Rotation Time**: Time interval between LCD screen rotation (Screen 1, 2, 3)
- **Buzzer Enable**: Enable/disable audible alerts
- **RGB Brightness**: LED brightness (0 = OFF, 255 = full brightness)

---

### 2.4 Communication Configuration

| Parameter              | Type   | Range                 | Default | Unit    | EEPROM Address |
| ---------------------- | ------ | --------------------- | ------- | ------- | -------------- |
| UART Baud Rate         | uint32 | 9600, 19200, 38400    | 9600    | bps     | 0x0014         |
| Communication Protocol | uint8  | 0=Mobile, 1=Dashboard | 0       | -       | 0x0018         |
| Data Update Rate       | uint8  | 1 - 10                | 1       | seconds | 0x0019         |

**Description**:

- **UART Baud Rate**: Serial communication speed (9600 recommended for HC-05/ESP-01)
- **Communication Protocol**: Default data format (Mobile = integers, Dashboard = floats)
- **Data Update Rate**: Frequency of automatic data transmission (if enabled)

---

### 2.5 Energy Logging Configuration

| Parameter             | Type   | Range      | Default | Unit    | EEPROM Address |
| --------------------- | ------ | ---------- | ------- | ------- | -------------- |
| EEPROM Save Interval  | uint16 | 10 - 600   | 60      | seconds | 0x001A         |
| Energy Save Threshold | float  | 0.01 - 1.0 | 0.1     | kWh     | 0x001C         |

**Description**:

- **EEPROM Save Interval**: Time interval for periodic energy counter save to EEPROM
- **Energy Save Threshold**: Minimum energy change to trigger immediate EEPROM save

---

## 3. Factory Defaults

All parameters can be reset to factory defaults via:

- **Mobile App**: Factory Reset command
- **Web Dashboard**: Factory Reset button
- **Hardware**: Press and hold reset button for 10 seconds (if implemented)

---

## 4. Configuration Access

### 4.1 Via Mobile App

- Navigate to Settings screen
- Adjust parameters using sliders/input fields
- Tap "Save" to write to EEPROM

### 4.2 Via Web Dashboard

- Navigate to Configuration page
- Modify parameters in web form
- Click "Apply" to send to device and save to EEPROM

### 4.3 Via UART Commands

- Send calibration command (0x02) with new values
- Parameters take effect immediately and are saved to EEPROM

---

## 5. Configuration Validation

All configuration changes are validated:

- **Range checking**: Values outside valid range are rejected
- **Type checking**: Invalid data types are rejected
- **CRC verification**: Optional CRC check for data integrity (future enhancement)

Invalid configuration values default to factory settings.

---

**Document Version**: 1.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
