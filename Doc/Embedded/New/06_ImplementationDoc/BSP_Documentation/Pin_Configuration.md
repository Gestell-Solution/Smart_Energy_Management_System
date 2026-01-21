# Pin Configuration

**Project**: Smart Energy Management System  
**Version**: 1.0

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

**Document Version**: 1.0  
**Last Updated**: January 2026
