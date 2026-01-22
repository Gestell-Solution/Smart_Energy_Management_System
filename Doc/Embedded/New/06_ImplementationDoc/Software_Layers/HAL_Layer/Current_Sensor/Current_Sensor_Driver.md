# ⚡ Current Sensor Driver - ACS712

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-ACS712_HAL-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-HAL_Driver-brightgreen)

**Current Sensor Driver**

**Smart Energy Management System - Non-Invasive Sensing**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Module Overview](#-1-module-overview)
- [Architecture](#-2-architecture-diagram)
- [Hardware Interface](#-3-hardware-interface)
- [Data Flow](#-4-data-flow-diagram)
- [State Machine](#-5-state-machine)
- [Sequence Diagrams](#-6-sequence-diagrams)
- [Dependencies](#-7-module-dependencies)

---

## 🔗 Related Documentation

| Document                                                     | Description  | Status       |
| ------------------------------------------------------------ | ------------ | ------------ |
| **[ADC_Driver.md](../../MCAL_Layer/ADC/ADC_Driver.md)**      | Analog Input | ✅ Available |
| **[Voltage_Sensor.md](../Voltage_Sensor/Voltage_Sensor.md)** | Voltage      | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The Current Sensor driver provides accurate AC current measurement through a Hall-effect sensor that measures magnetic field strength proportional to current flow. This enables the Smart Energy Management System to monitor load current, calculate power consumption, and detect overcurrent conditions without breaking the electrical circuit.

### Key Responsibilities

- Measure AC RMS current (0-30A range)
- Provide calibrated current readings in Amperes
- Zero-current offset calibration
- Digital filtering and noise reduction
- Overcurrent detection support
- Interface with ADC for analog-to-digital conversion

### Hardware Component

**ACS712-30A Specifications:**

- Range: -30A to +30A (bidirectional)
- Sensitivity: 66 mV/A (typical)
- Zero-current output: 2.5V (VCC/2)
- Isolation: 2.1 kV RMS minimum
- Bandwidth: 80 kHz
- Supply voltage: 5V DC

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Current Sensor Architecture"
        direction TB

        APP[Application/<br/>Measurement Engine] -->|Read Current| CURR_API[Current Sensor Driver]

        CURR_API --> SAMPLE[Sample Acquisition]
        CURR_API --> CALIB[Calibration Manager]
        CURR_API --> FILTER[Noise Filtering]

        SAMPLE --> ADC[ADC Driver<br/>Channel 1]

        CALIB --> EEPROM[EEPROM<br/>Zero Offset Storage]

        subgraph "Physical Sensor"
            CONDUCTOR[Current Path<br/>AC Mains Wire] --> HALL[Hall Effect<br/>IC]
            HALL --> AMP[Signal Amplifier]
            AMP --> OUT[Analog Output<br/>0.5V - 4.5V]
        end

        OUT --> ADC

        ADC --> RAW[Raw ADC Value<br/>0-1023]

        RAW --> CALC[Current Calculation]
        CALIB --> CALC

        CALC --> RMS[RMS Calculator<br/>128 samples]

        RMS --> FILTER

        FILTER --> RESULT[Final Current<br/>in Amperes]

        RESULT --> APP
    end

    style CURR_API fill:#4A90E2,color:#fff
    style HALL fill:#E24A4A,color:#fff
    style RMS fill:#50C878,color:#fff
```

---

## 3. Hardware Interface

### Sensor Connections

**Electrical Interface:**

- VCC: 5V power supply
- GND: Ground reference
- VOUT: Analog voltage output (connected to PA1/ADC1)
- IP+/IP-: Current path terminals (AC mains wire passes through)

### Operating Principle

**Hall-Effect Detection:**

1. Current flows through internal copper conductor
2. Magnetic field generated proportional to current
3. Hall-effect IC detects magnetic field strength
4. Output voltage varies with magnetic field intensity
5. Galvanic isolation maintained (no electrical connection)

### Output Voltage Relationship

**Formula:**

```
V_out = V_zero + (I × Sensitivity)
V_out = 2.5V + (I × 0.066 V/A)
```

**Examples:**

- 0A: V_out = 2.5V
- +10A: V_out = 2.5V + 0.66V = 3.16V
- -10A: V_out = 2.5V - 0.66V = 1.84V
- +20A: V_out = 2.5V + 1.32V = 3.82V

---

## 4. Data Flow Diagram

```mermaid
flowchart TB
    subgraph "Current Measurement Flow"
        direction TB

        AC_CURRENT[AC Current<br/>in Wire] --> MAG_FIELD[Magnetic Field<br/>Generated]
        MAG_FIELD --> HALL_DET[Hall Effect<br/>Detection]
        HALL_DET --> ANALOG_V[Analog Voltage<br/>0.5V - 4.5V]

        ANALOG_V --> ADC_CONV[ADC Conversion<br/>10-bit]
        ADC_CONV --> DIGITAL[Digital Value<br/>0-1023]

        DIGITAL --> VOLTAGE_CALC[Convert to Voltage<br/>V = ADC × 5.0 / 1024]

        VOLTAGE_CALC --> ZERO_SUB[Subtract Zero Offset<br/>ΔV = V - V_zero]

        ZERO_OFFSET[Zero Offset<br/>from EEPROM] --> ZERO_SUB

        ZERO_SUB --> CURR_CALC[Calculate Current<br/>I = ΔV / 0.066]

        CURR_CALC --> SAMPLE_BUF[Sample Buffer<br/>128 samples]

        SAMPLE_BUF --> RMS_CALC[RMS Calculation<br/>√(Σ(I²)/N)]

        RMS_CALC --> OUTLIER[Outlier Rejection<br/>±3σ filter]

        OUTLIER --> FINAL[Final RMS Current]

        FINAL --> APP_LAYER[Application Layer]
    end

    style HALL_DET fill:#E24A4A,color:#fff
    style RMS_CALC fill:#50C878,color:#fff
    style FINAL fill:#F39C12,color:#000
```

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> Uncalibrated: Power On

    Uncalibrated --> Calibrating: Start Calibration
    Calibrating --> Calibrated: Zero Offset Stored

    Calibrated --> Sampling: Begin Measurement

    Sampling --> Acquiring: Timer1 Trigger (10ms)
    Acquiring --> Processing: ADC Complete
    Processing --> BufferFull: Store Sample

    BufferFull --> Calculating: 128 Samples Collected
    Calculating --> Ready: RMS Calculated

    Ready --> Sampling: Next Cycle

    Ready --> Calibrating: Re-calibration Request

    note right of Uncalibrated
        No zero offset loaded
        Cannot provide accurate readings
        Calibration required
    end note

    note right of Calibrating
        No load connected
        100 samples averaged
        Zero offset calculated
        Stored in EEPROM
    end note

    note right of Calculating
        RMS formula applied
        Outliers filtered
        Final value available
    end note
```

---

## 6. Sequence Diagrams

### Calibration Sequence

```mermaid
sequenceDiagram
    participant USER as User/Installer
    participant APP as Application
    participant CURR as Current Sensor
    participant ADC as ADC Driver
    participant EEPROM as EEPROM

    USER->>APP: Initiate Calibration<br/>(Ensure NO LOAD)

    APP->>CURR: CurrentSensor_Calibrate()
    activate CURR

    Note over CURR: Wait 30s for thermal stability

    loop 100 Samples
        CURR->>ADC: Read ADC Channel 1
        ADC->>CURR: Return ADC value
        CURR->>CURR: Store in buffer
    end

    CURR->>CURR: Calculate average ADC value
    CURR->>CURR: Convert to voltage: V_zero

    CURR->>EEPROM: Write V_zero to address 0x04
    EEPROM->>CURR: Write complete

    CURR->>APP: Calibration SUCCESS
    deactivate CURR

    APP->>USER: Display: Calibration Complete
```

### Normal Measurement Sequence

```mermaid
sequenceDiagram
    participant TIMER as Timer1 (100 Hz)
    participant ADC as ADC Driver
    participant CURR as Current Sensor
    participant ME as Measurement Engine

    Note over TIMER: Every 10ms trigger

    TIMER->>ADC: Auto-trigger ADC Channel 1

    ADC->>ADC: Convert (104 µs)

    ADC->>CURR: Interrupt: Conversion Complete
    activate CURR

    CURR->>ADC: Read ADC value
    CURR->>CURR: Convert to voltage
    CURR->>CURR: Apply zero offset
    CURR->>CURR: Calculate instantaneous current
    CURR->>CURR: Store in sample buffer[n]
    CURR->>CURR: Increment index (n++)

    alt Buffer Full (n = 128)
        CURR->>CURR: Calculate RMS
        CURR->>CURR: Filter outliers
        CURR->>ME: Update I_RMS value
        CURR->>CURR: Reset buffer index
    end

    deactivate CURR
```

---

## 7. Module Dependencies

### Dependency Diagram

```mermaid
graph TB
    subgraph "Application Layer"
        ME[Measurement Engine]
        PROT[Protection Manager]
        CAL[Calibration Manager]
    end

    subgraph "HAL Layer"
        CURR[Current Sensor Driver]
    end

    subgraph "MCAL Layer"
        ADC[ADC Driver]
        EEPROM[EEPROM Driver]
    end

    subgraph "Hardware"
        ACS712[ACS712-30A<br/>Hall Sensor]
    end

    ME -->|Read Current| CURR
    PROT -->|Check Threshold| CURR
    CAL -->|Calibrate| CURR

    CURR -->|Read Channel 1| ADC
    CURR -->|Store/Load Offset| EEPROM
    CURR -->|Measure| ACS712

    style CURR fill:#4A90E2,color:#fff
    style ACS712 fill:#E24A4A,color:#fff
```

---

## 8. Configuration Parameters

### Calibration Data

| Parameter           | Storage          | Description                     |
| ------------------- | ---------------- | ------------------------------- |
| Zero offset voltage | EEPROM 0x04-0x07 | Voltage at 0A current           |
| Calibration factor  | EEPROM 0x08-0x0B | Sensitivity correction (future) |

### Measurement Parameters

| Parameter       | Value   | Description                           |
| --------------- | ------- | ------------------------------------- |
| Sensitivity     | 66 mV/A | Nominal (datasheet)                   |
| Zero voltage    | 2.5V    | Nominal (adjusted by calibration)     |
| ADC resolution  | 10-bit  | 1024 levels                           |
| Voltage per LSB | 4.88 mV | 5V / 1024                             |
| Current per LSB | 74 mA   | 4.88mV / 66mV/A                       |
| Sampling rate   | 50 Hz   | Per channel (alternates with voltage) |
| Samples per RMS | 128     | 2.56 seconds of data                  |

### Range and Resolution

| Metric               | Value                 |
| -------------------- | --------------------- |
| Minimum detectable   | ~0.1A (noise limited) |
| Maximum rated        | 30A                   |
| Recommended range    | 0.5A - 25A            |
| Practical resolution | 0.1A                  |

---

## 9. Error Handling Strategy

### Error Sources

**Temperature Drift:**

- Zero offset varies ±4mV/°C
- Mitigation: Periodic re-calibration

**Magnetic Interference:**

- External magnetic fields affect reading
- Mitigation: Shield sensor, keep away from transformers

**Offset Error:**

- Unit-to-unit variation in zero output
- Mitigation: Mandatory calibration during setup

### Outlier Rejection

**Method**: Statistical filtering

- Calculate mean and standard deviation of 128 samples
- Reject samples > 3× standard deviation
- Recalculate RMS without outliers

---

## 10. Performance Characteristics

### Accuracy

| Condition         | Accuracy | Notes             |
| ----------------- | -------- | ----------------- |
| After calibration | ±1%      | @ 10A, 25°C       |
| Uncalibrated      | ±3%      | Factory tolerance |
| Temperature drift | ±0.5%    | Over 0-50°C       |
| Linearity         | ±1%      | Full scale        |

### Response Time

- Single sample: 104 µs (ADC conversion)
- RMS update: 2.56 seconds (128 samples)
- Step response: < 3 seconds

### Resource Usage

- RAM: 512 bytes (128 samples × 4 bytes)
- Flash: ~350 bytes
- EEPROM: 8 bytes

---

## Implementation Notes

### RMS Calculation

**Formula:**

```
I_RMS = √(Σ(I_sample²) / N)
where N = 128 samples
```

**Purpose**: RMS represents equivalent DC current producing same heating effect

### Noise Filtering

**Hardware:**

- RC filter on output: R=1kΩ, C=100nF (fc ≈ 1.6 kHz)
- Twisted pair wiring
- Physical separation from switching components

**Software:**

- 128-sample averaging
- Outlier rejection
- Optional moving average for display

---

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

---

## 📄 Document Control

| Attribute            | Value                               |
| -------------------- | ----------------------------------- |
| **Document Type**    | Current Sensor Driver Documentation |
| **Document Status**  | Active                              |
| **Document Version** | 2.0                                 |
| **Last Updated**     | January 2026                        |
| **Prepared By**      | Gestell Engineering Team            |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
