# ⚡ Voltage Sensor Driver - AC Mains

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Voltage_Sensor-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-HAL_Driver-brightgreen)

**Voltage Sensor Driver**

**Smart Energy Management System - AC Mains Measurement**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Module Overview](#-1-module-overview)
- [Architecture](#-2-architecture-diagram)
- [Hardware Interface](#-3-hardware-interface)
- [Data Flow](#-4-data-flow-diagram)
- [State Machine](#-5-state-machine)
- [Calibration](#-6-sequence-diagrams)
- [Dependencies](#-7-module-dependencies)

---

## 🔗 Related Documentation

| Document                                                                   | Description  | Status       |
| -------------------------------------------------------------------------- | ------------ | ------------ |
| **[ADC_Driver.md](../../MCAL_Layer/ADC/ADC_Driver.md)**                    | Analog Input | ✅ Available |
| **[Current_Sensor_Driver.md](../Current_Sensor/Current_Sensor_Driver.md)** | Current      | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The Voltage Sensor driver enables safe measurement of AC mains voltage (220V RMS) through a voltage divider network and precision rectifier circuit. It provides calibrated voltage readings essential for energy calculation, power quality monitoring, and overvoltage protection.

### Key Responsibilities

- Measure AC RMS voltage (0-280V range)
- Voltage divider scaling (100:1 ratio)
- Calibration factor application
- Noise filtering and averaging
- Overvoltage detection support
- Safe galvanic isolation from mains

### Hardware Components

**Stage 1 - Voltage Divider:**

- Primary resistor: 470 kΩ (1% tolerance)
- Secondary resistor: 4.7 kΩ (1% tolerance)
- Division ratio: 100:1
- 220V AC → 2.2V AC

**Stage 2 - Precision Rectifier:**

- Full-wave rectifier with op-amp
- AC to pulsating DC conversion
- Low-pass filter smoothing
- Output: 0-5V DC for ADC

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Voltage Sensor Architecture"
        direction TB

        APP[Application/<br/>Measurement Engine] -->|Read Voltage| VOLT_API[Voltage Sensor Driver]

        VOLT_API --> SAMPLE[Sample Acquisition]
        VOLT_API --> CALIB[Calibration Manager]
        VOLT_API --> FILTER[Filtering & Averaging]

        SAMPLE --> ADC[ADC Driver<br/>Channel 0]

        CALIB --> EEPROM[EEPROM<br/>Calibration Factor]

        subgraph "Physical Circuit"
            MAINS[AC Mains<br/>220V RMS] --> FUSE[Fuse<br/>Protection]
            FUSE --> MOV[MOV<br/>Transient Suppression]
            MOV --> DIV[Voltage Divider<br/>470kΩ / 4.7kΩ]
            DIV --> RECT[Precision Rectifier<br/>Op-Amp]
            RECT --> LPF[Low-Pass Filter<br/>Smoothing]
            LPF --> OUT[DC Output<br/>0-5V]
        end

        OUT --> ADC

        ADC --> RAW[Raw ADC Value<br/>0-1023]

        RAW --> SCALE[Scaling × 100]
        SCALE --> CAL_APPLY[Apply Calibration<br/>Factor]
        CALIB --> CAL_APPLY

        CAL_APPLY --> MA_FILTER[Moving Average<br/>8 samples]

        MA_FILTER --> RESULT[Final Voltage<br/>in Volts]

        RESULT --> APP
    end

    style VOLT_API fill:#4A90E2,color:#fff
    style DIV fill:#E24A4A,color:#fff
    style MA_FILTER fill:#50C878,color:#fff
```

---

## 3. Hardware Interface

### Safety Isolation

**Clearance Requirements:**

- PCB trace spacing: Minimum 5mm between high/low voltage
- Isolation voltage: 2000V+ via physical separation
- Fuse: Fast-blow 250mA on input side
- MOV: Metal Oxide Varistor for transient protection

### Pin Configuration

| Pin | Function      | Type                   |
| --- | ------------- | ---------------------- |
| PA0 | Voltage Input | Analog Input (ADC CH0) |

### Voltage Scaling

**Divider Calculation:**

```
V_adc = V_mains / 100
For 220V AC: V_adc ≈ 2.2V AC
After rectification: ~2.2V DC (average)
```

---

## 4. Data Flow Diagram

```mermaid
flowchart TB
    subgraph "Voltage Measurement Flow"
        direction TB

        AC_MAINS[AC Mains<br/>220V RMS] --> DIVIDE[Voltage Divider<br/>÷ 100]
        DIVIDE --> AC_REDUCED[2.2V AC<br/>Peak-to-Peak]

        AC_REDUCED --> RECTIFY[Precision Rectifier<br/>Full-Wave]
        RECTIFY --> DC_PULSING[Pulsating DC<br/>0-2.2V]

        DC_PULSING --> SMOOTH[Low-Pass Filter<br/>Smoothing]
        SMOOTH --> DC_AVG[DC Average<br/>~2.2V]

        DC_AVG --> ADC_CONV[ADC Conversion<br/>10-bit]
        ADC_CONV --> DIGITAL[Digital Value<br/>0-1023]

        DIGITAL --> V_ADC[Convert to Voltage<br/>V = ADC × 5.0 / 1024]

        V_ADC --> SCALE_UP[Scale Up × 100<br/>V_mains = V_adc × 100]

        SCALE_UP --> CALIB_APP[Apply Calibration<br/>V_cal = V × factor]

        CAL_FACTOR[Calibration Factor<br/>from EEPROM] --> CALIB_APP

        CALIB_APP --> MOVING_AVG[Moving Average<br/>8 samples, 80ms window]

        MOVING_AVG --> FINAL_V[Final RMS Voltage]

        FINAL_V --> APP_LAYER[Application Layer]
    end

    style RECTIFY fill:#E24A4A,color:#fff
    style MOVING_AVG fill:#50C878,color:#fff
    style FINAL_V fill:#F39C12,color:#000
```

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> Uncalibrated: Power On

    Uncalibrated --> Calibrating: User Calibration
    Calibrating --> Calibrated: Factor Stored

    Calibrated --> Measuring: Start Sampling

    Measuring --> Reading: ADC Trigger (10ms)
    Reading --> Scaling: ADC Complete
    Scaling --> Calibrating: Apply Factor
    Calibrating --> Filtering: Add to Buffer

    Filtering --> Ready: Average Calculated

    Ready --> Measuring: Next Sample

    Ready --> Calibrating: Re-calibration Request

    note right of Uncalibrated
        Default factory calibration
        Factor = 1.0
        Accuracy may be reduced
    end note

    note right of Calibrating
        Reference voltage applied
        Measured and compared
        Factor calculated and stored
    end note

    note right of Filtering
        Circular buffer (8 samples)
        Moving average filter
        80ms response time
    end note
```

---

## 6. Sequence Diagrams

### Calibration Sequence

```mermaid
sequenceDiagram
    participant USER as Technician
    participant APP as Application
    participant VOLT as Voltage Sensor
    participant ADC as ADC Driver
    participant EEPROM as EEPROM

    USER->>APP: Apply known voltage (220V)
    USER->>APP: Initiate calibration

    APP->>VOLT: VoltageSensor_Calibrate(220.0)
    activate VOLT

    loop 10 samples
        VOLT->>ADC: Read ADC Channel 0
        ADC->>VOLT: Return ADC value
        VOLT->>VOLT: Convert and average
    end

    VOLT->>VOLT: Calculate V_measured (uncalibrated)
    Note over VOLT: Example: V_measured = 215.3V

    VOLT->>VOLT: Calculate factor = 220.0 / 215.3
    Note over VOLT: Factor = 1.0218

    VOLT->>EEPROM: Write factor to 0x10-0x13
    EEPROM->>VOLT: Write complete

    VOLT->>APP: Calibration SUCCESS
    deactivate VOLT

    APP->>USER: Display: New reading = 220.0V
```

### Normal Measurement Sequence

```mermaid
sequenceDiagram
    participant TIMER as Timer1 (100 Hz)
    participant ADC as ADC Driver
    participant VOLT as Voltage Sensor
    participant ME as Measurement Engine

    Note over TIMER: Every 10ms trigger

    TIMER->>ADC: Auto-trigger ADC Channel 0

    ADC->>ADC: Convert (104 µs)

    ADC->>VOLT: Sample available
    activate VOLT

    VOLT->>ADC: Read ADC value
    VOLT->>VOLT: Convert to V_adc: ADC × 5.0 / 1024
    VOLT->>VOLT: Scale up: V_mains = V_adc × 100
    VOLT->>VOLT: Apply calibration: V_cal = V_mains × factor
    VOLT->>VOLT: Add to circular buffer[n]
    VOLT->>VOLT: Calculate moving average (8 samples)

    VOLT->>ME: Update V_RMS value

    deactivate VOLT
```

---

## 7. Module Dependencies

### Dependency Diagram

```mermaid
graph TB
    subgraph "Application Layer"
        ME[Measurement Engine]
        PROT[Protection Manager]
        CAL_MGR[Calibration Manager]
    end

    subgraph "HAL Layer"
        VOLT[Voltage Sensor Driver]
    end

    subgraph "MCAL Layer"
        ADC[ADC Driver]
        EEPROM[EEPROM Driver]
    end

    subgraph "Hardware"
        CIRCUIT[Voltage Divider<br/>+ Rectifier Circuit]
    end

    ME -->|Read Voltage| VOLT
    PROT -->|Check Overvoltage| VOLT
    CAL_MGR -->|Calibrate| VOLT

    VOLT -->|Read Channel 0| ADC
    VOLT -->|Store/Load Factor| EEPROM
    VOLT -->|Measure| CIRCUIT

    style VOLT fill:#4A90E2,color:#fff
    style CIRCUIT fill:#E24A4A,color:#fff
```

---

## 8. Configuration Parameters

### Calibration Storage

| Parameter          | EEPROM Address | Type            | Description                     |
| ------------------ | -------------- | --------------- | ------------------------------- |
| Calibration factor | 0x10-0x13      | float (4 bytes) | Multiplier for voltage readings |

### Measurement Parameters

| Parameter       | Value     | Description                |
| --------------- | --------- | -------------------------- |
| Divider ratio   | 100:1     | 470kΩ / 4.7kΩ              |
| ADC resolution  | 10-bit    | 1024 levels                |
| Voltage per LSB | ~0.488V   | After scaling              |
| Sampling rate   | 100 Hz    | Shared with current sensor |
| Filter window   | 8 samples | 80ms moving average        |

### Range and Accuracy

| Metric                  | Value                        |
| ----------------------- | ---------------------------- |
| Minimum voltage         | 0V (10V practical minimum)   |
| Nominal voltage         | 220V RMS                     |
| Maximum voltage         | 280V (overvoltage threshold) |
| Accuracy (calibrated)   | ±2% @ 220V                   |
| Accuracy (uncalibrated) | ±5%                          |

---

## 9. Error Handling Strategy

### Error Detection

**Out-of-Range Voltage:**

- ADC < 10 or > 1020 for extended period
- Indicates sensor disconnection or fault
- Response: Set error flag, notify Protection Manager

**Calibration Out of Range:**

- Factor outside 0.90-1.10 range
- Indicates EEPROM corruption or invalid calibration
- Response: Revert to default (1.0), request recalibration

### Filtering Strategy

**Moving Average Filter:**

- Purpose: Reduce noise and transients
- Window size: 8 samples (80ms)
- Trade-off: Latency vs noise reduction

**Outlier Rejection (Optional):**

- Median filter preprocessing
- Take 3 samples, keep median
- Rejects spikes and impulses

---

## 10. Performance Characteristics

### Accuracy

| Condition         | Accuracy | Notes               |
| ----------------- | -------- | ------------------- |
| After calibration | ±2%      | @ 220V, 25°C        |
| Uncalibrated      | ±5%      | Component tolerance |
| Temperature drift | ±0.5%    | 0-50°C range        |

### Response Time

- Single reading: 104 µs (ADC)
- Filtered reading: 80 ms (8 samples)
- Step response: < 100 ms

### Resource Usage

- RAM: 32 bytes (8 samples × 4 bytes float)
- Flash: ~400 bytes
- EEPROM: 4 bytes

---

## Implementation Notes

### RMS vs Average Rectified

**Assumption**: Pure sinusoidal waveform

**Conversion:**

- Average rectified value from circuit
- Form factor = 1.11 applied
- V_RMS = V_avg × 1.11

**Limitation**: Accurate only for pure sine waves

### Safety Considerations

- High-voltage circuit physically isolated
- Fuse protection prevents overcurrent
- MOV clamps transients
- Fail-safe: Disconnection reads low/zero voltage

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
| **Document Type**    | Voltage Sensor Driver Documentation |
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
