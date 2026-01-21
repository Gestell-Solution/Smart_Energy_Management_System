# 🔌 Hardware Requirements Specification (HRS) - ATmega32

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-HRS_Document-brightgreen)

**Hardware Requirements Specification (HRS)**

**Smart Energy Management System - ATmega32 Platform**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Introduction](#-introduction)
- [MCU Specification](#-mcu-specification)
- [Power Supply Design](#-power-supply-design)
- [Sensor Circuits](#-sensor-circuits)
- [Actuator Circuits](#-actuator-circuits)
- [Display System](#-display-system)
- [Communication Modules](#-communication-modules)
- [Protection Circuits](#-protection-circuits)
- [Programming Interface](#-programming-interface)
- [LED Indicators](#-led-indicators)
- [Terminal Blocks](#-terminal-blocks)
- [PCB Design](#-pcb-design)
- [Bill of Materials](#-bill-of-materials)

---

## 📖 Introduction

### 1.1 Purpose

This document defines the **ATmega32-based hardware** for the Smart Energy Management System - a real-time energy monitoring and protection device.

### 1.2 Design Philosophy

**HW-AVR-001**: ATmega32 platform prioritizes **simplicity, cost-effectiveness, and proven reliability** using 8-bit AVR architecture.

**HW-AVR-002**: All components shall be **Through-Hole Technology (THT)** for easy assembly, repair, and prototyping.

**HW-AVR-003**: Target system cost: **Cost-effective for residential use** within Egypt market availability.

### 1.3 Related Documentation

| Document                                                                                                     | Description           | Status       |
| ------------------------------------------------------------------------------------------------------------ | --------------------- | ------------ |
| **[CRS.md](../CRS/CRS.md)**                                                                                  | Customer Requirements | ✅ Available |
| **[SRS.md](../SRS/SRS.md)**                                                                                  | Software Requirements | ✅ Available |
| **[Component_DatasheetsList.md](../../05_Hardware_Design/Component_Datasheets/Component_DatasheetsList.md)** | Component datasheets  | ✅ Available |
| **[PinOut_Reference.md](../AddationalInformation/PinOut_Reference.md)**                                      | I/O Pin Assignment    | 📅 Planned   |

---

## 🔧 MCU Specification

### 2.1 Selected Microcontroller: **ATmega32A-PU**

| Specification         | Value                                           | Justification                                     |
| --------------------- | ----------------------------------------------- | ------------------------------------------------- |
| **Architecture**      | 8-bit AVR RISC                                  | Proven for real-time monitoring, simple toolchain |
| **Package**           | DIP-40 (THT)                                    | Through-hole mounting, easy replacement           |
| **Flash Memory**      | 32 KB                                           | Sufficient for application (~20KB used)           |
| **SRAM**              | 2 KB                                            | Adequate for buffers and calculations             |
| **EEPROM**            | 1 KB                                            | Native non-volatile storage for energy logging    |
| **I/O Pins**          | 32 (PORTA-PORTD)                                | Meets ~20-pin requirement with margin             |
| **Operating Voltage** | 4.5V - 5.5V                                     | Standard 5V TTL logic                             |
| **Max Frequency**     | 16 MHz (external crystal)                       | Meets 100ms cycle time requirement                |
| **Timers**            | Timer0 (8-bit), Timer1 (16-bit), Timer2 (8-bit) | ADC sampling timing, delays, PWM                  |
| **UART**              | 1 channel                                       | Bluetooth/WiFi communication                      |
| **ADC**               | 8 channels, 10-bit                              | Voltage and current sensor inputs                 |
| **Watchdog**          | Hardware WDT                                    | Optional safety feature                           |
| **Cost (Egypt)**      | ~80-120 EGP                                     | Cost-effective                                    |
| **Availability**      | High                                            | Widely available locally                          |

**Alternative Option**: ATmega16A-PU (16 KB Flash) if cost reduction needed for low-end variant.

### 2.2 Pin Assignment Summary

> [!NOTE]
> Pin counts based on current system design. See [PinOut_Reference.md](../AddationalInformation/PinOut_Reference.md) for complete pin mapping.

| Function Group                | Pin Count    | Notes                               |
| ----------------------------- | ------------ | ----------------------------------- |
| **Sensors**                   |              |                                     |
| ADC Inputs (Voltage, Current) | 2            | PA0, PA1 (ADC channels)             |
| **Actuators**                 |              |                                     |
| Relay Control                 | 1            | PB0 (via ULN2003)                   |
| RGB LED                       | 3            | PB1, PB2, PB3 (PWM capable)         |
| Buzzer                        | 1            | PB4                                 |
| **Display (LCD 16x2)**        | 6            | 4-bit mode (PD2-PD7)                |
| **Communication**             | 2            | UART (TX: PD1, RX: PD0)             |
| **User Input**                | 1            | Button input (optional)             |
| **Programming (ISP)**         | 4            | MISO, MOSI, SCK, RESET              |
| **Crystal**                   | 2            | XTAL1, XTAL2 (16 MHz)               |
| **Power/Ground**              | Multiple     | VCC, AVCC, GND, AGND                |
| **Total Used**                | **~22 pins** | **10+ pins reserved for expansion** |

---

## ⚡ Power Supply Design

### 3.1 Input Power

**HW-AVR-PWR-001**: Input voltage: **5VDC ±10%** (4.5V - 5.5V)

**HW-AVR-PWR-002**: Power source options:

- USB power (5V, 500mA-1A)
- DC adapter (5V, 1-2A recommended)
- Battery backup (optional future feature)

**HW-AVR-PWR-003**: Input connector: **DC barrel jack (5.5mm × 2.1mm)** or screw terminal

### 3.2 Voltage Regulators

#### Option 1: Direct 5V Input (Recommended for USB/Adapter)

**Circuit**:

```
5VDC Input ──[Reverse Polarity Protection]──[100µF Cap]──┬── VCC (ATmega32)
                                                          │
                                                         GND
```

**Components**:

- D1: 1N5822 Schottky diode (reverse polarity protection, low voltage drop ~0.3V)
- C1: 100µF/16V electrolytic (input filtering)
- C2: 100nF ceramic (high-frequency decoupling)

#### Option 2: Regulated from Higher Voltage (7-12V Input)

**Component**: LM7805 or L7805CV (TO-220 package)

**Circuit**:

```
7-12VDC ──[C1 100nF]──[C2 10µF]──┬─── VIN (LM7805) ──┬─── [C3 100nF]──[C4 10µF]── 5VDC
                                  │                    │
                                 GND                  GND
                                                       │
                                                 [Heatsink]
```

**Specifications**:

- Input: 7-12VDC
- Output: 5VDC ±5%, 1A
- Dropout voltage: ~2V
- Heatsink: Required if input >9V

### 3.3 Power Distribution

**HW-AVR-PWR-004**: Separate power considerations:

- 5V digital logic (ATmega32, LCD, HC-05)
- 3.3V for ESP-01 (if WiFi version): Use AMS1117-3.3 regulator
- Relay power (same 5V rail via ULN2003 driver)

**HW-AVR-PWR-005**: Analog reference (AVCC):

- Connect AVCC to VCC via **10µH inductor** (LC filter)
- Add 100nF ceramic capacitor from AVCC to AGND
- This reduces ADC noise for accurate measurements

### 3.4 Power Consumption Estimate

| Component                 | Current (mA) | Notes                      |
| ------------------------- | ------------ | -------------------------- |
| ATmega32 @ 16MHz          | ~20          | Active mode                |
| LCD 16x2 (with backlight) | ~50          | Backlight dominates        |
| HC-05 Bluetooth           | ~30          | Average (40mA peak)        |
| ESP-01 WiFi (if used)     | ~150         | Peak during TX (70mA idle) |
| ACS712 Current Sensor     | ~10          | Quiescent current          |
| Relay (energized)         | ~70          | Via ULN2003                |
| RGB LED                   | ~20          | If all colors ON           |
| Buzzer                    | ~30          | When active                |
| **Total (HC-05)**         | **~250mA**   | Normal operation           |
| **Total (ESP-01)**        | **~370mA**   | WiFi version peak          |

**Recommended Power Supply**: **5V, 1-2A** adapter

---

## 🔍 Sensor Circuits

### 4.1 Voltage Measurement Circuit

**HW-AVR-SENS-001**: Voltage measurement uses **resistive voltage divider** with step-down from AC mains.

#### Circuit Topology

```
AC Input (220V) ──┬── [R1: 470kΩ, 1W] ──┬── [R2: 4.7kΩ] ──┬── AC_OUT
                  │                      │                  │
              [Fuse 1A]              [Rectifier          [C1: 10µF]
                  │                   Bridge]               │
                 GND                     │                  │
                                        GND           To ADC (PA0)
```

**Design Calculations**:

- Divider ratio: R1/R2 ≈ 470k/4.7k = **100:1**
- Input: 220V AC RMS → Peak = 220 × √2 ≈ **311V**
- Divided: 311V / 100 = **3.11V peak**
- After rectification and filtering: ~**2.5V DC** (at 220V AC input)
- ADC range: 0-5V → Can measure up to **~300V AC**

**Components**:

- R1: 470kΩ, **1W metal film** resistor (high voltage rating)
- R2:4.7kΩ, 0.25W resistor
- Rectifier: Bridge rectifier (e.g., MB6S or 4× 1N4007 diodes)
- C1: 10µF/16V electrolytic (smoothing capacitor)
- C2: 100nF ceramic (additional filtering)
- Fuse: 1A slow-blow fuse (safety)

**HW-AVR-SENS-002**: Safety isolation:

- Physical separation of high voltage traces
- Clearance: **Minimum 5mm** between AC and DC sides
- **WARNING labels** on PCB silkscreen

**HW-AVR-SENS-003**: ADC connection:

- Connect filtered DC output to **PA0 (ADC Channel 0)**
- Add 100Ω series resistor (current limiting for ADC protection)

### 4.2 Current Measurement Circuit

**HW-AVR-SENS-004**: Current measurement uses **ACS712-30A Hall-effect sensor** (galvanic isolation built-in).

#### Circuit Topology

```
Load Current ──[Terminal IN]── ACS712 ──[Terminal OUT]── Load
                                 │
                                 │ (Isolated)
                              VOUT (2.5V ± ΔV)
                                 │
                             [RC Filter] ──┬── To ADC (PA1)
                                          GND
```

**ACS712 Specifications**:

- Model: **ACS712ELCTR-30A-T**
- Current range: **±30A**
- Sensitivity: **66 mV/A**
- Supply voltage: **5V**
- Output at zero current: **VCC/2 = 2.5V**
- Galvanic isolation: **2.1 kV RMS**
- Bandwidth: 80 kHz

**Design Calculations**:

- Zero current: VOUT = **2.5V**
- At +10A: VOUT = 2.5V + (10A × 0.066V/A) = **3.16V**
- At +20A: VOUT = 2.5V + (20A × 0.066V/A) = **3.82V**
- At +30A: VOUT = 2.5V + (30A × 0.066V/A) = **4.48V** (still within ADC range)

**Filter Circuit** (reduce high-frequency noise):

- R_filter: 1kΩ
- C_filter: 100nF ceramic
- Cutoff frequency: f_c = 1/(2π × R × C) ≈ **1.6 kHz** (adequate for 50/60 Hz AC)

**HW-AVR-SENS-005**: ADC connection:

- Connect ACS712 VOUT (via RC filter) to **PA1 (ADC Channel 1)**
- Add 100Ω series resistor (ADC input protection)

**HW-AVR-SENS-006**: Calibration:

- Zero-current calibration: Measure VOUT with no load, store offset in EEPROM
- Sensitivity adjustment: Fine-tune via software calibration factor

---

## 🔀 Actuator Circuits

### 5.1 Relay Driver Circuit

**HW-AVR-ACT-001**: Use **ULN2003A Darlington array IC** (DIP-16 package) for driving relay.

**Circuit per Relay**:

```
MCU Output (PB0) ──[R1 1kΩ]──┬── ULN2003A Input (Pin 1)
                              │
                         [LED_IND] 3mm green + 1kΩ
                              │
                             GND

ULN2003A Output (Pin 16) ── Relay Coil Pin 1

VCC (+5V) ── Relay Coil Pin 2

Common Pin 9 (ULN2003A) ── +5V (flyback diode return)
```

**Components**:

- R1: 1kΩ (MCU output protection)
- LED_IND: 3mm green LED + 1kΩ resistor (relay status indicator)
- Flyback protection: Built into ULN2003A (internal diodes)

### 5.2 Relay Specifications

**HW-AVR-ACT-002**: Use **5VDC SPDT relay** (e.g., SRD-05VDC-SL-C or Songle SRD)

**Ratings**:

- Coil voltage: **5VDC**
- Coil current: **70-90mA** (within ULN2003A 500mA limit)
- Contact rating: **10A @ 250VAC** or **10A @ 30VDC**
- Switching time: <10ms
- Mechanical life: >10 million operations
- Package: PCB mount with solder pins

**HW-AVR-ACT-003**: Relay contacts shall include **RC snubber circuits** for AC loads:

- R: 100Ω, 1/4W
- C: 100nF, 275VAC X2-rated capacitor
- Purpose: Reduces arcing and EMI during switching

### 5.3 RGB LED Driver

**HW-AVR-ACT-004**: RGB LED uses **common cathode** configuration with PWM control.

**Circuit**:

```
MCU PB1 (Red PWM) ──[R_red 150Ω]── LED Red Anode
                                      │
MCU PB2 (Green PWM) ──[R_green 100Ω]── LED Green Anode
                                      │
MCU PB3 (Blue PWM) ──[R_blue 100Ω]── LED Blue Anode
                                      │
                                  Common Cathode ── GND
```

**Current limiting resistor calculations**:

- Red LED: V_f = 2.0V, I = 20mA → R = (5V - 2V) / 20mA = **150Ω**
- Green LED: V_f = 3.2V, I = 20mA → R = (5V - 3.2V) / 20mA = **90Ω ≈ 100Ω**
- Blue LED: V_f = 3.2V, I = 20mA → R = (5V - 3.2V) / 20mA = **90Ω ≈ 100Ω**

### 5.4 Buzzer Driver

**HW-AVR-ACT-005**: Active buzzer (built-in oscillator) with simple ON/OFF control.

**Circuit**:

```
MCU PB4 ──[R1 1kΩ]── NPN Transistor Base (e.g., 2N2222 or BC547)
                          │
                    Emitter ── GND
                          │
                    Collector ──[Buzzer+]──[VCC +5V]
                          │
                    [Flyback Diode 1N4148]
```

**Components**:

- Transistor: 2N2222 or BC547 (NPN)
- R1: 1kΩ (base current limiting)
- Buzzer: Active 5V buzzer, ~30mA
- D1: 1N4148 (flyback protection)

**HW-AVR-ACT-006**: Safe state:

- All actuator outputs default to **de-energized (OFF)** state on power-up and MCU reset

---

## 🖥️ Display System

### 6.1 LCD 16x2 Character Display

**HW-AVR-DISP-001**: Use **HD44780-compatible 16×2 character LCD**.

**Interface**: 4-bit mode (saves I/O pins)

**Pin Connections**:
| LCD Pin | Function | ATmega32 Pin |
|---------|----------|--------------|
| VSS | Ground | GND |
| VDD | Power (+5V) | VCC |
| V0 | Contrast | Potentiometer wiper |
| RS | Register Select | PD2 |
| RW | Read/Write (tie to GND for write-only) | GND |
| E | Enable | PD3 |
| D4 | Data bit 4 | PD4 |
| D5 | Data bit 5 | PD5 |
| D6 | Data bit 6 | PD6 |
| D7 | Data bit 7 | PD7 |
| A | Backlight Anode (+5V) | VCC via 10Ω resistor |
| K | Backlight Cathode | GND |

**HW-AVR-DISP-002**: Contrast control:

- Connect V0 to **10kΩ potentiometer** (variable contrast adjustment)
- Potentiometer wiper to V0, one end to GND, other end to VCC

**HW-AVR-DISP-003**: Backlight control:

- Series resistor: **10Ω, 0.5W** (limits backlight current to ~150mA)
- Alternative: PWM-controlled backlight for brightness adjustment

### 6.2 Display Content (per software)

**Screen 1** (2 seconds):

```
V: 220.5 V
I:  5.20 A
```

**Screen 2** (2 seconds):

```
P: 1146.6 W
E:    2.5 kWh
```

**Screen 3** (2 seconds):

```
Status: OK
Load: Connected
```

---

## 📡 Communication Modules

### 7.1 HC-05 Bluetooth Module (Version 1)

**HW-AVR-COMM-001**: Use **HC-05 Bluetooth 2.0 SPP module**.

**Pin Connections**:
| HC-05 Pin | Function | ATmega32 Pin |
|-----------|----------|--------------|
| VCC | Power (**3.3V**) | 3.3V regulator output |
| GND | Ground | GND |
| TXD | Transmit (3.3V level) | RX (PD0) - **Direct connection OK** |
| RXD | Receive (3.3V input) | TX (PD1) - **Voltage divider needed** |

**HW-AVR-COMM-002**: Level shifting:

- **HC-05 TXD → ATmega32 RX**: Direct connection (3.3V is above TTL threshold)
- **ATmega32 TX → HC-05 RXD**: Voltage divider required (5V to 3.3V)

**Voltage Divider Circuit** (5V → 3.3V):

```
ATmega32 TX (PD1) ──[R1 1kΩ]──┬── HC-05 RXD
                               │
                          [R2 2kΩ]
                               │
                              GND
```

- Output voltage: 5V × (2kΩ / (1kΩ + 2kΩ)) ≈ **3.3V**

**HW-AVR-COMM-003**: Power supply:

- HC-05 requires **3.3V** (not 5V tolerant on VCC)
- Use AMS1117-3.3 regulator (TO-220 or SOT-223 package)
- Input: 5V, Output: 3.3V, 1A capability

**HW-AVR-COMM-004**: Configuration:

- Default baud rate: 9600 bps (can be changed via AT commands)
- Default name: "HC-05" (can be changed via AT commands)
- Pairing PIN: 1234 or 0000 (default)

### 7.2 ESP-01 WiFi Module (Version 2 - Optional)

**HW-AVR-COMM-005**: Use **ESP-01 (ESP8266) WiFi module**.

**Pin Connections**:
| ESP-01 Pin | Function | Connection |
|------------|----------|------------|
| VCC | Power (**3.3V**) | 3.3V regulator output |
| GND | Ground | GND |
| TX | Transmit (3.3V level) | RX (PD0) - Direct connection |
| RX | Receive (3.3V input) | TX (PD1) - Voltage divider |
| CH_PD | Chip enable | VCC (via 10kΩ pull-up) |
| RST | Reset | VCC (via 10kΩ pull-up) |

**HW-AVR-COMM-006**: Level shifting:

- Same voltage divider as HC-05 (5V → 3.3V for TX)

**HW-AVR-COMM-007**: Power supply considerations:

- ESP-01 can draw up to **200mA peak** during WiFi transmission
- Use dedicated **AMS1117-3.3 regulator** with **100µF capacitor** close to module
- Consider adding **10µF tantalum capacitor** for transient response

**HW-AVR-COMM-008**: Mode selection:

- System uses **either HC-05 OR ESP-01**, not both simultaneously
- Selection via physical module installation or jumper configuration

---

## 🛡️ Protection Circuits

### 8.1 Input Protection

**Protection Layer 1: Reverse Polarity**

- **Schottky diode 1N5822** (3A, 40V) in series with +5V input
- Voltage drop: ~0.3V (better than standard diode ~0.7V)

**Protection Layer 2: Over-Voltage Clamping**

- **TVS diode P6KE6.8CA** (6.8V bidirectional) across VCC and GND
- Clamps voltage spikes above 6.8V
- Protects against ESD and transients

**Protection Layer 3: Input Filtering**

- **100µF electrolytic + 100nF ceramic** capacitors on input rail
- Filters noise and provides local energy storage

### 8.2 MCU Protection

**Power Supply Filtering**:

- **100nF ceramic capacitor** on each VCC pin (AVCC, VCC) close to IC
- **10µF electrolytic** on main VCC rail
- **LC filter** (10µH inductor + 100nF cap) between VCC and AVCC

**Reset Protection**:

- **10kΩ pull-up resistor** on RESET pin
- **100nF capacitor** to GND (power-on reset delay)
- Optional: External reset button with debouncing

**Programming Interface Protection**:

- **100Ω series resistors** on MISO, MOSI, SCK, RESET
- Limits current during hot-plugging of programmer

### 8.3 ADC Protection

**HW-AVR-PROT-001**: ADC input protection:

- **100Ω series resistor** on each ADC input (PA0, PA1)
- **100nF ceramic capacitor** to GND (noise filtering)
- Clamping diodes (optional): Schottky diodes to VCC and GND

### 8.4 Fuse Protection

**HW-AVR-PROT-002**: Main input fuse:

- **1A slow-blow fuse** on voltage divider circuit (AC side)
- **500mA fast-blow fuse** on DC power input (optional for additional safety)

---

## 💻 Programming Interface

### 9.1 ISP Header

**HW-AVR-PROG-001**: 6-pin ISP header (2×3, 2.54mm pitch, shrouded box header recommended)

**Pinout** (Standard AVR ISP):

```
   ┌─────────┐
   │ 1  2  3 │
   │ 4  5  6 │
   └─────────┘

Pin 1: MISO    (MCU Pin PB6)
Pin 2: VCC     (+5V)
Pin 3: SCK     (MCU Pin PB7)
Pin 4: MOSI    (MCU Pin PB5)
Pin 5: RESET   (MCU Pin 9)
Pin 6: GND
```

**Protection Circuit**:

```
MCU MISO ──[100Ω]── ISP Pin 1
MCU MOSI ──[100Ω]── ISP Pin 4
MCU SCK  ──[100Ω]── ISP Pin 3
MCU RST  ──[100Ω]──┬── ISP Pin 5
                   │
              [10kΩ Pull-up to VCC]
                   │
              [100nF to GND]
```

**HW-AVR-PROG-002**: ISP header shall be placed at **board edge** for easy access.

**HW-AVR-PROG-003**: Silkscreen marking: "ISP" with pin 1 indicator (square pad or arrow).

### 9.2 Programming Procedure

1. Connect USBasp programmer to ISP header
2. Power board via 5V input
3. Use **avrdude** or **Atmel Studio** to upload firmware
4. Typical command: `avrdude -c usbasp -p m32 -U flash:w:firmware.hex`

### 9.3 Fuse Bits Configuration

**Recommended Fuse Settings** for ATmega32:

| Fuse         | Value | Setting                             |
| ------------ | ----- | ----------------------------------- |
| **CKSEL**    | 1111  | External crystal 8MHz+              |
| **SUT**      | 10    | Slowly rising power                 |
| **CKOPT**    | 0     | Full rail-to-rail swing (for 16MHz) |
| **BOOTRST**  | 1     | Boot from application section       |
| **JTAGEN**   | 1     | JTAG disabled (frees up PC pins)    |
| **OCDEN**    | 1     | On-chip debug disabled              |
| **BODLEVEL** | 1     | Brown-out detection at 4.0V         |
| **BODEN**    | 0     | Brown-out detection enabled         |

**Calculated Fuse Bytes** (verify with fuse calculator):

- LOW: 0xFF (external crystal, slowly rising power)
- HIGH: 0xC9 (JTAG disabled, boot to app, BOD 4V)

---

## 💡 LED Indicators

**HW-AVR-LED-001**: Total LED count: **5 LEDs** (MCU-controlled + power indicator)

### 10.1 RGB Status LED (3 pins)

**Purpose**: System status indication via color mixing

**Connections**:

- Common cathode RGB LED
- Red: PB1 (PWM capable)
- Green: PB2 (PWM capable)
- Blue: PB3 (PWM capable)

**Color Codes**:

- **Green**: Normal operation
- **Yellow** (Red + Green): Warning
- **Red**: Fault/Protection triggered
- **Blue**: Communication active
- **Cyan** (Green + Blue): Calibration mode

### 10.2 Relay Status LED

**Purpose**: Visual indication of relay state

**Connection**:

- 3mm green LED in series with ULN2003A input
- LED ON = relay energized (load connected)

### 10.3 Power Indicator LED

**Purpose**: Power supply operational

**Connection**:

- 3mm red LED + 1kΩ resistor from 5V rail
- Always ON when powered

---

## 🔩 Terminal Blocks

**HW-AVR-TERM-001**: External connections use **removable screw terminal blocks**.

### 11.1 Terminal Specifications

| Function                   | Connector Type              | Pitch  | Wire Gauge |
| -------------------------- | --------------------------- | ------ | ---------- |
| Power Input (5V DC)        | DG301-5.08-2P               | 5.08mm | 18-24 AWG  |
| AC Input (Voltage Sensor)  | DG301-5.08-2P               | 5.08mm | 18-22 AWG  |
| Current Sensor (Load Path) | DG301-5.08-2P               | 5.08mm | 12-18 AWG  |
| Relay Output               | DG301-5.08-3P (NO, COM, NC) | 5.08mm | 14-20 AWG  |

### 11.2 Terminal Layout

#### TB1: Power Input (2-way)

1. +5VDC
2. GND

#### TB2: Voltage Sensor Input (2-way)

1. AC Input (220V Live)
2. AC Neutral

> [!CAUTION]
> **HIGH VOLTAGE**: This terminal connects to mains AC voltage. Proper clearances and safety warnings required.

#### TB3: Current Sensor (2-way per ACS712)

1. Load Input (from AC source)
2. Load Output (to appliance)

#### TB4: Relay Output (3-way SPDT)

1. NO (Normally Open)
2. COM (Common)
3. NC (Normally Closed)

**HW-AVR-TERM-002**: Terminals shall have **adhesive printed labels** for identification.

---

## 🖥️ PCB Design

### 12.1 PCB Specifications

**Material**: FR-4, **Two-Layer (double-sided)**  
**Thickness**: 1.6mm  
**Copper Weight**: 1 oz (35µm)  
**Solder Mask**: Green (both sides)  
**Silkscreen**: White (component references, polarity marks)

> [!IMPORTANT]
> This design uses **Two-Layer PCB only** (no multilayer required). This reduces manufacturing cost and enables local Egyptian PCB fabrication.

### 12.2 Dimensions

**Target Size**: 100mm × 80mm (compact for enclosure mounting)

**Mounting**: 4× M3 mounting holes at corners (3mm holes, 5mm from edges)

### 12.3 Layout Guidelines

**Component Placement**:

- ATmega32 (DIP-40 socket) at PCB center
- Voltage regulator (if used) with heatsink at board edge (airflow)
- ACS712 current sensor module mounted via header (for easy replacement)
- LCD connector at front edge (16-pin header for cable)
- HC-05/ESP-01 header at side edge (removable module)
- Relay near relay terminals (short trace to contacts)
- Programming header (ISP) at easily accessible edge

**Trace Routing**:

- Power traces (VCC, GND): Minimum 1.0mm width (2.0mm for high current paths)
- Signal traces: 0.4mm minimum width
- Clearance: 0.25mm minimum

**Ground Plane**:

- Solid GND pour on bottom layer
- Thermal reliefs on through-hole pads for easier soldering

**High Voltage Isolation**:

- **Minimum 5mm clearance** between AC mains traces and low-voltage circuits
- **Physical slot or cutout** in PCB between AC and DC sections (optional for added safety)
- Silkscreen **HIGH VOLTAGE warning** near AC terminals

**Decoupling**:

- 100nF ceramic capacitor within 5mm of every IC VCC pin
- ATmega32: 100nF on VCC, AVCC (close to pins)
- 10µF bulk capacitor on main VCC rail

---

## 📦 Bill of Materials

### 13.1 Complete BOM

| Category                                     | Component                     | Part Number    | Qty | Unit Price (EGP) | Total (EGP)  |
| -------------------------------------------- | ----------------------------- | -------------- | --- | ---------------- | ------------ |
| **MCU**                                      |                               |                |     |                  |              |
|                                              | ATmega32A-PU                  | ATMEGA32A-PU   | 1   | 100              | 100          |
|                                              | DIP-40 IC Socket              | -              | 1   | 5                | 5            |
|                                              | Crystal 16 MHz                | HC-49S         | 1   | 5                | 5            |
|                                              | 22pF Ceramic Caps             | -              | 2   | 0.5              | 1            |
| **Power Supply**                             |                               |                |     |                  |              |
|                                              | LM7805 Regulator (optional)   | TO-220         | 1   | 3                | 3            |
|                                              | AMS1117-3.3 Regulator         | TO-220/SOT-223 | 1   | 2                | 2            |
|                                              | TO-220 Heatsink               | -              | 2   | 3                | 6            |
|                                              | 1N5822 Schottky Diode         | DO-201         | 1   | 1                | 1            |
|                                              | 100µF/16V Electrolytic        | -              | 4   | 1                | 4            |
|                                              | 100nF Ceramic Caps            | -              | 15  | 0.5              | 8            |
|                                              | 10µF/16V Electrolytic         | -              | 4   | 1                | 4            |
| **Sensors**                                  |                               |                |     |                  |              |
|                                              | ACS712-30A Module             | Breakout board | 1   | 30               | 30           |
|                                              | Resistor 470kΩ 1W             | Metal film     | 1   | 2                | 2            |
|                                              | Resistor 4.7kΩ 0.25W          | -              | 1   | 0.1              | 0.1          |
|                                              | Bridge Rectifier MB6S         | -              | 1   | 2                | 2            |
|                                              | 1kΩ Resistor 1/4W             | -              | 10  | 0.1              | 1            |
|                                              | 10kΩ Resistor 1/4W            | -              | 10  | 0.1              | 1            |
|                                              | 100Ω Resistor 1/4W            | -              | 5   | 0.1              | 0.5          |
| **Actuators**                                |                               |                |     |                  |              |
|                                              | ULN2003A Driver               | DIP-16         | 1   | 3                | 3            |
|                                              | 5VDC SPDT Relay               | SRD-05VDC-SL-C | 1   | 15               | 15           |
|                                              | 100nF/275VAC X2 Cap           | -              | 1   | 2                | 2            |
|                                              | RGB LED 5mm Common Cathode    | -              | 1   | 2                | 2            |
|                                              | Active Buzzer 5V              | -              | 1   | 3                | 3            |
|                                              | 2N2222 NPN Transistor         | TO-92          | 1   | 1                | 1            |
|                                              | 1N4148 Diode                  | DO-35          | 2   | 0.5              | 1            |
| **Display**                                  |                               |                |     |                  |              |
|                                              | LCD 16x2 Character Display    | HD44780        | 1   | 25               | 25           |
|                                              | 10kΩ Potentiometer            | Trimmer        | 1   | 2                | 2            |
|                                              | 1×16 Pin Header (LCD)         | 2.54mm         | 1   | 2                | 2            |
| **Communication**                            |                               |                |     |                  |              |
|                                              | HC-05 Bluetooth Module        | -              | 1   | 40               | 40           |
|                                              | ESP-01 WiFi Module (optional) | ESP8266        | 1   | 35               | 35           |
|                                              | 10µH Inductor                 | -              | 1   | 1                | 1            |
| **LEDs**                                     |                               |                |     |                  |              |
|                                              | 3mm Red LED                   | -              | 1   | 0.5              | 0.5          |
|                                              | 3mm Green LED                 | -              | 1   | 0.5              | 0.5          |
| **Terminal Blocks**                          |                               |                |     |                  |              |
|                                              | DG301-5.08-2P                 | Phoenix style  | 3   | 3                | 9            |
|                                              | DG301-5.08-3P                 | Phoenix style  | 1   | 4                | 4            |
| **Connectors**                               |                               |                |     |                  |              |
|                                              | 6-pin ISP Header              | 2×3 shrouded   | 1   | 3                | 3            |
|                                              | 1×4 Pin Header (HC-05/ESP)    | 2.54mm         | 2   | 1                | 2            |
| **Protection**                               |                               |                |     |                  |              |
|                                              | TVS Diode P6KE6.8CA           | DO-15          | 1   | 2                | 2            |
|                                              | Fuse 1A Slow-blow             | -              | 1   | 1                | 1            |
|                                              | Fuse Holder PCB Mount         | -              | 1   | 2                | 2            |
| **PCB & Mechanical**                         |                               |                |     |                  |              |
|                                              | FR-4 PCB 100×80mm             | Double-sided   | 1   | 80               | 80           |
|                                              | M3 Standoffs 10mm             | -              | 4   | 1                | 4            |
|                                              | M3 Screws                     | -              | 8   | 0.5              | 4            |
| **Miscellaneous**                            |                               |                |     |                  |              |
|                                              | Wire, solder, labels, etc.    | -              | -   | -                | 15           |
| **TOTAL ESTIMATED BOM COST (HC-05 version)** |                               |                |     |                  | **~430 EGP** |
| **TOTAL ESTIMATED BOM COST (WiFi version)**  |                               |                |     |                  | **~425 EGP** |

### 13.2 Notes

- Prices based on 2026 Egypt market (Cairo electronics markets)
- HC-05 and ESP-01 are alternatives (not both needed)
- Bulk purchasing (10+ units) can reduce cost by 15-20%
- Alternative suppliers: Future Electronics, RAM Electronics, online retailers

---

## 📞 Support & Contact

**Project**: Smart Energy Management System - ATmega32 Platform  
**Company**: Gestell - Professional Embedded Solutions  
**Technical Support**: Hisham4Ahmed@gmail.com  
**LinkedIn**: https://www.linkedin.com/company/gestell-company

---

## 📄 Document Control

| Attribute             | Value                                       |
| --------------------- | ------------------------------------------- |
| **Document Type**     | Hardware Requirements Specification (HRS)   |
| **Variant**           | ATmega32 Platform                           |
| **Document Version**  | 1.0                                         |
| **Last Updated**      | January 2026                                |
| **Prepared By**       | Gestell Engineering Team                    |
| **Related Documents** | CRS.md, SRS.md, Component_DatasheetsList.md |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Industrial Control Solutions_

**Copyright © 2025-2026 Gestell Company**

</div>
