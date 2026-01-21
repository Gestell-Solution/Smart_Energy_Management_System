# Hardware-Software Partitioning

**Project**: Smart Energy Management System  
**Version**: 1.0

## Hardware Responsibilities

**Sensors**:
- Voltage divider (passive circuit) - steps down 220V AC to measurable range
- ACS712 sensor - measures current, provides galvanic isolation (2.1 kV)

**Signal Conditioning**:
- Rectifier bridge - converts AC to pulsating DC for voltage measurement
- RC filters - noise reduction on ADC inputs

**Actuators**:
- Relay (electromechanical) - physically disconnects load
- ULN2003 driver - provides sufficient current to energize relay coil

**Display**:
- LCD module (HD44780) - character display with built-in controller

**Communication**:
- HC-05/ESP-01 modules - handle Bluetooth/WiFi protocol stacks  
- Level shifters - convert 5V ↔ 3.3V logic levels

## Software Responsibilities

**Measurement**:
- ADC sampling at precise intervals (Timer1 interrupt)
- RMS calculation (sum of squares algorithm)
- Calibration factor application
- Power/energy calculation

**Protection**:
- Threshold comparison (overcurrent, overvoltage)
- Debouncing logic (3 consecutive readings)
- Fault state management
- Relay control decision

**Data Management**:
- EEPROM read/write (energy counter, calibration data)
- Data formatting for communication (integer/float conversion)
- Command parsing and response generation

**User Interface**:
- LCD content composition and screen rotation
- RGB LED color control (status indication)
- Buzzer pattern generation

**Communication Protocol**:
- Frame parsing (header, length, command, data)
- Dual format support (Mobile vs Dashboard)
- Command routing and execution

## Partitioning Rationale

**Hardware handles**:
- Physical signal conditioning (cannot be done in software)
- Galvanic isolation (safety requirement)
- High-power switching (relay contacts)

**Software handles**:
- Complex calculations (RMS, power, energy)
- Decision making (protection logic, thresholds)
- Protocol implementation (flexible, updatable)
- User interface logic (easily customizable)

**Document Version**: 1.0  
**Last Updated**: January 2026
