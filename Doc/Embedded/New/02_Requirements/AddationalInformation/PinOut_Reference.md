# PinOut Reference - ATmega32

**Project**: Smart Energy Management System  
**Component**: ATmega32 Pin Assignment  
**Version**: 1.0

---

## 1. Overview

This document provides the complete pin assignment for the ATmega32 microcontroller in the Smart Energy Management System.

---

## 2. Pin Assignment Summary

| Pin #                        | Port Pin | Function   | Direction                                | Description                                        |
| ---------------------------- | -------- | ---------- | ---------------------------------------- | -------------------------------------------------- |
| **ADC Inputs**               |
| 40                           | PA0      | ADC0       | Input                                    | Voltage Sensor (after voltage divider & rectifier) |
| 39                           | PA1      | ADC1       | Input                                    | Current Sensor (ACS712 output)                     |
| 38                           | PA2      | -          | -                                        | Reserved                                           |
| 37                           | PA3      | -          | -                                        | Reserved                                           |
| 36                           | PA4      | -          | -                                        | Reserved                                           |
| 35                           | PA5      | -          | -                                        | Reserved                                           |
| 34                           | PA6      | -          | -                                        | Reserved                                           |
| 33                           | PA7      | -          | -                                        | Reserved                                           |
| **Relay & Actuators**        |
| 1                            | PB0      | RELAY_CTRL | Output                                   | Relay control (via ULN2003)                        |
| 2                            | PB1      | RGB_RED    | Output                                   | RGB LED - Red (PWM: OC1A)                          |
| 3                            | PB2      | RGB_GREEN  | Output                                   | RGB LED - Green (PWM: OC1B)                        |
| 4                            | PB3      | RGB_BLUE   | Output                                   | RGB LED - Blue (PWM compatible)                    |
| 5                            | PB4      | BUZZER     | Output                                   | Active buzzer control                              |
| 6                            | PB5      | MOSI       | I/O                                      | SPI MOSI (ISP programming)                         |
| 7                            | PB6      | MISO       | I/O                                      | SPI MISO (ISP programming)                         |
| 8                            | PB7      | SCK        | I/O                                      | SPI SCK (ISP programming)                          |
| **Communication**            |
| 14                           | PD0      | RXD        | Input                                    | UART Receive (from HC-05/ESP-01)                   |
| 15                           | PD1      | TXD        | Output                                   | UART Transmit (to HC-05/ESP-01)                    |
| **LCD Display (4-bit mode)** |
| 16                           | PD2      | LCD_RS     | Output                                   | LCD Register Select                                |
| 17                           | PD3      | LCD_E      | Output                                   | LCD Enable                                         |
| 18                           | PD4      | LCD_D4     | Output                                   | LCD Data bit 4                                     |
| 19                           | PD5      | LCD_D5     | Output                                   | LCD Data bit 5                                     |
| 20                           | PD6      | LCD_D6     | Output                                   | LCD Data bit 6                                     |
| 21                           | PD7      | LCD_D7     | Output                                   | LCD Data bit 7                                     |
| **Optional Inputs**          |
| 22-29                        | PC0-PC7  | -          | -                                        | Reserved for future expansion (buttons, etc.)      |
| **Power & Clock**            |
| 10                           | VCC      | -          | Power                                    | +5V supply                                         |
| 11                           | GND      | -          | Ground                                   | Ground reference                                   |
| 30                           | AVCC     | -          | Power                                    | Analog VCC (filtered from VCC)                     |
| 31                           | GND      | -          | Ground                                   | Analog ground                                      |
| 32                           | AREF     | -          | Input                                    | ADC reference (connected to AVCC via capacitor)    |
| 12                           | XTAL2    | -          | Output                                   | Crystal oscillator output (16 MHz)                 |
| 13                           | XTAL1    | -          | Input                                    | Crystal oscillator input (16 MHz)                  |
| **Reset**                    |
| 9                            | RESET    | Input      | Active LOW reset (with pull-up resistor) |

---

## 3. Detailed Pin Descriptions

### 3.1 ADC Inputs (PORTA)

**PA0 (Pin 40) - Voltage Sensor**:

- Connected to voltage divider output (after rectification)
- Input range: 0-5V DC (represents 0-300V AC)
- ADC resolution: 10-bit (0-1023 counts)
- External filtering: 10µF + 100nF capacitors
- Series protection: 100Ω resistor

**PA1 (Pin 39) - Current Sensor**:

- Connected to ACS712 VOUT pin
- Input range: 0-5V (2.5V at zero current)
- Sensitivity: 66 mV/A
- External filtering: 1kΩ + 100nF RC filter
- Series protection: 100Ω resistor

---

### 3.2 Relay & Actuators (PORTB)

**PB0 (Pin 1) - Relay Control**:

- Drives ULN2003 Darlington array input
- Logic HIGH = Relay energized (load connected)
- Logic LOW = Relay de-energized (load disconnected)
- Safe state: LOW (power-up default)

**PB1 (Pin 2) - RGB Red Channel**:

- PWM output (OC1A from Timer1)
- Drives red LED cathode via current-limiting resistor (150Ω)
- PWM frequency: ~1 kHz (adjustable)

**PB2 (Pin 3) - RGB Green Channel**:

- PWM output (OC1B from Timer1)
- Drives green LED cathode via current-limiting resistor (100Ω)

**PB3 (Pin 4) - RGB Blue Channel**:

- Digital or PWM output
- Drives blue LED cathode via current-limiting resistor (100Ω)

**PB4 (Pin 5) - Buzzer Control**:

- Drives NPN transistor base (via 1kΩ resistor)
- Logic HIGH = Buzzer ON
- Logic LOW = Buzzer OFF

**PB5-PB7 (Pins 6-8) - ISP Programming**:

- Used for in-system programming (USBasp)
- Must remain accessible for firmware updates

---

### 3.3 Communication (PORTD)

**PD0 (Pin 14) - UART RX**:

- Receives data from HC-05 (Bluetooth) or ESP-01 (WiFi)
- Input level: 3.3V (TTL compatible, no level shifting needed)
- Interrupt-driven reception

**PD1 (Pin 15) - UART TX**:

- Transmits data to HC-05 or ESP-01
- Output level: 5V (requires voltage divider to 3.3V for module protection)
- Voltage divider: 1kΩ + 2kΩ resistors

---

### 3.4 LCD Display (PORTD)

**4-bit mode interface** (saves I/O pins):

- **PD2 (Pin 16)**: RS - Register Select (0 = command, 1 = data)
- **PD3 (Pin 17)**: E - Enable (falling edge triggers LCD action)
- **PD4-PD7 (Pins 18-21)**: D4-D7 - Data lines (4-bit nibbles)

**Note**: RW pin of LCD is tied to GND (write-only mode).

---

### 3.5 Power Pins

**VCC (Pin 10)**: Main +5V supply

- Decoupling: 100nF ceramic capacitor (close to pin)
- Bulk: 10µF electrolytic capacitor on PCB

**AVCC (Pin 30)**: Analog +5V supply

- LC filter from VCC: 10µH inductor + 100nF capacitor
- Reduces digital noise on ADC measurements

**AREF (Pin 32)**: ADC reference voltage

- Connected to AVCC via 100nF capacitor
- Internal reference disabled (ADMUX.REFS = 01 for AVCC reference)

---

### 3.6 Oscillator

**16 MHz External Crystal**:

- XTAL1 (Pin 13): Crystal input
- XTAL2 (Pin 12): Crystal output
- Load capacitors: 22pF each (to ground)
- Fuse bits: External crystal mode (CKSEL = 1111)

---

## 4. Unused Pins (Reserved)

**PORTC (PC0-PC7)**: All 8 pins reserved for future expansion

- Can be used for:
  - User input buttons
  - Additional sensors
  - I2C communication (PC0=SCL, PC1=SDA via TWI)
  - Status LEDs
  - External interrupts

---

## 5. Pin Protection

All I/O pins include:

- **ESD protection**: Internal diodes to VCC and GND
- **External protection** (where needed): 100Ω series resistors
- **Overvoltage clamping**: TVS diodes on vulnerable inputs

---

## 6. Default Pin States

**On Power-Up / Reset**:

- All pins default to **INPUT with pull-up disabled** (Hi-Z)
- Firmware initialization sets pins to correct modes
- Safe state: All outputs LOW (relays OFF, LEDs OFF, buzzer OFF)

---

**Document Version**: 1.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
