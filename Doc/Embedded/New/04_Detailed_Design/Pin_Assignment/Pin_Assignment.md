# 📍 Pin Assignment - ATmega32

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Design_Document-brightgreen)

**Pin Assignment - ATmega32**

**Smart Energy Management System - Complete MCU Pin Mapping**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Complete Pin Mapping](#-complete-pin-mapping-table)
- [Port Summary](#-port-summary)
- [References](#-related-documentation)

---

## 🔗 Related Documentation

| Document                                                                                                    | Description               | Status       |
| ----------------------------------------------------------------------------------------------------------- | ------------------------- | ------------ |
| **[PinOut_Reference.md](../../02_Requirements/AddationalInformation/PinOut_Reference.md)**                  | Detailed Pinout Reference | ✅ Available |
| **[Hardware_Architecture.md](../../03_System_Architecture/Hardware_Architecture/Hardware_Architecture.md)** | Hardware Overview         | ✅ Available |
| **[Schematics](../../05_Hardware_Design/Schematics)**                                                       | Schematic Diagrams        | 📅 Planned   |

---

## 📑 Complete Pin Mapping Table

| Pin#  | Port.Pin | Function       | Dir | Type    | Description               |
| ----- | -------- | -------------- | --- | ------- | ------------------------- |
| 1     | PB0      | RELAY_CTRL     | OUT | Digital | Relay control via ULN2003 |
| 2     | PB1/OC1A | RGB_RED        | OUT | PWM     | RGB Red channel (PWM)     |
| 3     | PB2/OC1B | RGB_GREEN      | OUT | PWM     | RGB Green channel (PWM)   |
| 4     | PB3      | RGB_BLUE       | OUT | Digital | RGB Blue channel          |
| 5     | PB4      | BUZZER         | OUT | Digital | Active buzzer control     |
| 6     | PB5/MOSI | ISP_MOSI       | I/O | Digital | Programming interface     |
| 7     | PB6/MISO | ISP_MISO       | I/O | Digital | Programming interface     |
| 8     | PB7/SCK  | ISP_SCK        | I/O | Digital | Programming interface     |
| 9     | RESET    | RESET          | IN  | Digital | MCU reset (active LOW)    |
| 10    | VCC      | POWER          | PWR | -       | +5V supply                |
| 11    | GND      | GROUND         | GND | -       | Ground                    |
| 12    | XTAL2    | OSC_OUT        | OUT | Analog  | 16MHz crystal output      |
| 13    | XTAL1    | OSC_IN         | IN  | Analog  | 16MHz crystal input       |
| 14    | PD0/RXD  | UART_RX        | IN  | Digital | UART receive              |
| 15    | PD1/TXD  | UART_TX        | OUT | Digital | UART transmit             |
| 16    | PD2      | LCD_RS         | OUT | Digital | LCD Register Select       |
| 17    | PD3      | LCD_E          | OUT | Digital | LCD Enable                |
| 18    | PD4      | LCD_D4         | OUT | Digital | LCD Data bit 4            |
| 19    | PD5      | LCD_D5         | OUT | Digital | LCD Data bit 5            |
| 20    | PD6      | LCD_D6         | OUT | Digital | LCD Data bit 6            |
| 21    | PD7      | LCD_D7         | OUT | Digital | LCD Data bit 7            |
| 22-29 | PC0-PC7  | RESERVED       | -   | -       | Future expansion          |
| 30    | AVCC     | AVCC           | PWR | -       | Analog VCC (5V filtered)  |
| 31    | GND      | AGND           | GND | -       | Analog ground             |
| 32    | AREF     | AREF           | IN  | Analog  | ADC reference (→AVCC)     |
| 33    | PA7      | RESERVED       | -   | -       | Unused                    |
| 34    | PA6      | RESERVED       | -   | -       | Unused                    |
| 35    | PA5      | RESERVED       | -   | -       | Unused                    |
| 36    | PA4      | RESERVED       | -   | -       | Unused                    |
| 37    | PA3      | RESERVED       | -   | -       | Unused                    |
| 38    | PA2      | RESERVED       | -   | -       | Unused                    |
| 39    | PA1/ADC1 | CURRENT_SENSOR | IN  | Analog  | ACS712 output (0-5V)      |
| 40    | PA0/ADC0 | VOLTAGE_SENSOR | IN  | Analog  | Voltage divider (0-5V)    |

---

## Port Summary

**PORTA (8 pins)**: ADC inputs + reserved

- Used: PA0, PA1 (2 pins)
- Reserved: PA2-PA7 (6 pins)

**PORTB (8 pins)**: Actuators + ISP

- Used: PB0-PB7 (8 pins - all utilized)

**PORTC (8 pins)**: Reserved for expansion

- Used: None
- Reserved: PC0-PC7 (8 pins)

**PORTD (8 pins)**: Communication + LCD

- Used: PD0-PD7 (8 pins - all utilized)

**Total Used**: 22 pins  
**Total Reserved**: 14 pins  
**Expansion Capacity**: Good

---

See [PinOut_Reference.md](../../02_Requirements/AddationalInformation/PinOut_Reference.md) for detailed descriptions.

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
| **Document Type**    | Pin Assignment           |
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
