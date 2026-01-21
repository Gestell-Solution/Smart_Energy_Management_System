# Component Datasheets List

## Smart Energy Management System - Embedded Hardware Components

This document provides references to datasheets for all major hardware components used in the Smart Energy Management System.

---

## Microcontroller

### ATmega32

- **Manufacturer**: Microchip Technology (formerly Atmel)
- **Part Number**: ATmega32A-PU
- **Package**: DIP-40
- **Datasheet URL**: [ATmega32 Datasheet](https://www.microchip.com/en-us/product/ATmega32)
- **Key Specifications**:
  - 8-bit AVR RISC architecture
  - 32 KB Flash memory
  - 2 KB SRAM
  - 1 KB EEPROM
  - 32 GPIO pins (PORTA, PORTB, PORTC, PORTD)
  - 8-channel 10-bit ADC
  - 2x UART, SPI, TWI (I²C)
  - 3 Timers (Timer0, Timer1, Timer2)
  - Operating Voltage: 4.5-5.5V
  - Max Clock Frequency: 16 MHz

---

## Sensors

### ACS712 Current Sensor

- **Manufacturer**: Allegro MicroSystems
- **Part Number**: ACS712ELCTR-30A-T
- **Datasheet URL**: [ACS712 Datasheet](https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712)
- **Key Specifications**:
  - Current sensing range: ±30A
  - Sensitivity: 66 mV/A
  - Supply voltage: 5V
  - Galvanic isolation (2.1 kV RMS)
  - Output voltage centered at VCC/2 (2.5V at zero current)
  - Bandwidth: 80 kHz

### Voltage Sensor (Voltage Divider)

- **Implementation**: Resistive voltage divider
- **Components Required**:
  - High voltage resistor: 470kΩ (1W, metal film)
  - Low voltage resistor: 4.7kΩ (0.25W)
  - Divider ratio: approximately 1:100
  - Input range: 0-250V AC (RMS)
  - Output range: 0-2.5V DC (after rectification and filtering)

---

## Display

### LCD 16x2 Character Display

- **Type**: HD44780-compatible character LCD
- **Configuration**: 16 characters × 2 lines
- **Interface**: 4-bit mode (uses 6 GPIO pins from ATmega32)
- **Datasheet URL**: [HD44780U Datasheet](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)
- **Key Specifications**:
  - Supply voltage: 5V
  - Contrast control: 10kΩ potentiometer
  - Backlight: LED (with current limiting resistor)
  - Control pins: RS, RW, E
  - Data pins: D4, D5, D6, D7

---

## Communication Modules

### HC-05 Bluetooth Module (Version 1)

- **Type**: Bluetooth 2.0 Serial Port Profile (SPP)
- **Datasheet URL**: [HC-05 Datasheet](https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf)
- **Key Specifications**:
  - Operating voltage: 3.3V (requires level shifting for 5V ATmega32)
  - UART interface
  - Default baud rate: 9600 bps (configurable)
  - Range: ~10 meters (Class 2)
  - Operating frequency: 2.4 GHz ISM band

### ESP-01 WiFi Module (Version 2)

- **Type**: ESP8266-based WiFi module
- **Datasheet URL**: [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
- **Key Specifications**:
  - Operating voltage: 3.3V (requires level shifting/voltage regulator)
  - WiFi standards: 802.11 b/g/n
  - UART interface for AT commands
  - Built-in TCP/IP protocol stack
  - Operating frequency: 2.4 GHz
  - TX power: +19.5 dBm
  - Note: Higher power consumption than HC-05 (~200mA peak)

---

## Actuators

### Relay Module

- **Type**: Electromechanical relay
- **Typical Part**: SRD-05VDC-SL-C or equivalent
- **Datasheet URL**: [Generic 5V Relay Datasheet](https://components101.com/switches/5v-single-channel-relay-module-pinout-features-applications-working-datasheet)
- **Key Specifications**:
  - Coil voltage: 5V DC
  - Contact rating: 10A @ 250V AC / 10A @ 30V DC
  - Coil current: ~70mA
  - Used for: Load switching (ener gy supply control)
  - Driver: ULN2003 or equivalent Darlington array

---

## Indicators

### RGB LED

- **Type**: Common cathode RGB LED
- **Colors**: Red, Green, Blue (individually controllable)
- **Forward voltage**:
  - Red: ~2.0V
  - Green: ~3.2V
  - Blue: ~3.2V
- **Forward current**: 20mA per color
- **Current limiting resistors**:
  - Red: 150Ω
  - Green: 100Ω
  - Blue: 100Ω

### Buzzer

- **Type**: Active buzzer (with built-in oscillator)
- **Operating voltage**: 5V
- **Frequency**: ~2.3 kHz (typical)
- **Current**: ~30mA
- **Used for**: Audible alarms and notifications

---

## Power Supply Components

### Voltage Regulators

#### LM7805 - 5V Regulator

- **Datasheet URL**: [LM7805 Datasheet](https://www.ti.com/lit/ds/symlink/lm340.pdf)
- **Output voltage**: 5V
- **Input voltage range**: 7-35V
- **Output current**: Up to 1.5A
- **Used for**: Main 5V supply for ATmega32, LCD, relays

#### AMS1117-3.3 - 3.3V Regulator

- **Datasheet URL**: [AMS1117 Datasheet](http://www.advanced-monolithic.com/pdf/ds1117.pdf)
- **Output voltage**: 3.3V
- **Output current**: Up to 1A
- **Used for**: Power supply for HC-05 or ESP-01

### Capacitors

- **Input capacitor**: 100µF / 25V electrolytic (voltage regulator input)
- **Output capacitor**: 100µF / 16V electrolytic (voltage regulator output)
- **Ceramic bypass capacitors**: 100nF ceramic (near each IC VCC pin)
- **ADC filter capacitors**: 10µF + 100nF (for analog reference filtering)

---

## Passives & Miscellaneous

### Resistors

- **Pull-up resistors**: 10kΩ (for buttons, I2C lines)
- **LED current limiting resistors**: 150Ω - 330Ω
- **Voltage divider resistors**: See "Voltage Sensor" section above

### Capacitors

- **Crystal load capacitors**: 22pF ceramic (for 16 MHz crystal)
- **Power supply decoupling**: 100nF ceramic (near each IC)
- **Bulk capacitors**: 100µF electrolytic (power regulation)

### Crystal Oscillator

- **Frequency**: 16 MHz
- **Type**: HC-49S crystal
- **Load capacitance**: 20pF
- **Accuracy**: ±50 ppm
- **Used for**: ATmega32 system clock

### Push Buttons

- **Type**: Tactile push button
- **Configuration**: Normally open (NO)
- **Debouncing**: Software debouncing implemented
- **Pull-up**: 10kΩ external resistor

---

## Notes

1. **Level Shifting**: When interfacing 3.3V modules (HC-05, ESP-01) with 5V ATmega32:
   - Use voltage divider for ATmega32 TX → Module RX
   - Direct connection acceptable for Module TX → ATmega32 RX (3.3V is above TTL threshold)
   - Alternative: Use dedicated level shifter IC

2. **Decoupling**: Place 100nF ceramic capacitors as close as possible to VCC pins of all ICs

3. **Datasheet Versions**: Always verify you have the latest datasheet version from manufacturer websites

4. **Part Substitution**: When substituting components, ensure electrical and mechanical compatibility

---

**Document Version**: 1.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team

---

For component procurement and detailed specifications, please refer to individual datasheets linked above.
