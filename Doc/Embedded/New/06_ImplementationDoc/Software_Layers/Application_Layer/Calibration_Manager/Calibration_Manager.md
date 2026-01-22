# 📏 Calibration Manager

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Application_Layer-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Software_Module-brightgreen)

**Calibration Manager**

**Smart Energy Management System - Sensor Accuracy Control**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Module Overview](#-1-module-overview)
- [Architecture](#-2-architecture-diagram)
- [Calibration Procedure](#-3-calibration-procedure)
- [State Machine](#-4-state-machine)
- [Sequence Diagrams](#-5-sequence-diagrams)
- [Data Structures](#-6-data-structures)
- [Configuration Parameters](#-7-configuration-parameters)
- [Error Handling](#-8-error-handling-strategy)
- [Performance Characteristics](#-9-performance-characteristics)
- [Dependencies](#-10-module-dependencies)

---

## 🔗 Related Documentation

| Document                                                                 | Description  | Status       |
| ------------------------------------------------------------------------ | ------------ | ------------ |
| **[Measurement_Engine.md](../Measurement_Engine/Measurement_Engine.md)** | Measurements | ✅ Available |
| **[EEPROM_Driver.md](../../MCAL_Layer/EEPROM/EEPROM_Driver.md)**         | Storage      | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The Calibration Manager is responsible for ensuring the accuracy of voltage and current measurements in the Smart Energy Management System. Low-cost sensors and electronic components often have tolerances that lead to measurement errors (offset and gain errors). This module provides a mechanism to correct these errors by comparing measured values against known reference standards and determining correction factors.

### Key Responsibilities

- **Gain Correction**: Calculates and applies multiplication factors to correct slope errors in voltage and current readings.
- **Offset Correction**: Determines and subtracts zero-current offsets to eliminate phantom readings when no load is connected.
- **Persistence**: Saves calibration coefficients to non-volatile memory (EEPROM) so they are retained after power cycles.
- **Validation**: Ensures calculated factors are within reasonable limits to prevent erroneous calibration.
- **Factory Reset**: Provides capability to restore default calibration values.

### Calibration Theory

The relationship between the raw ADC value and the physical quantity is modeled linearly:

> **Physical Value = (Raw ADC - Offset) × Gain Factor**

1.  **Offset**: The ADC reading when the physical input is zero.
2.  **Gain Factor**: The ratio between the actual physical value and the offset-corrected ADC reading.

By determining these two parameters experimentally, the system can achieve high accuracy even with inexpensive hardware components.

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Calibration Manager Architecture"
        UI[User Interface / Serial Command] -->|Trigger Calib| CAL_MGR[Calibration Manager]

        CAL_MGR -->|Read Raw Samples| ME[Measurement Engine]
        CAL_MGR -->|Store Coefficients| EEPROM[EEPROM Driver]

        subgraph "Calibration Logic"
            OFFSET_CAL[Zero Offset<br/>Calculator]
            GAIN_CAL[Gain Factor<br/>Calculator]
            VALID[Range<br/>Validator]
        end

        CAL_MGR --> OFFSET_CAL
        CAL_MGR --> GAIN_CAL
        GAIN_CAL --> VALID
        OFFSET_CAL --> VALID

        VALID -->|Valid Data| RAM_CACHE[Calibration<br/>RAM Cache]
        RAM_CACHE -->|Apply| ME

        EEPROM -.->|Load at Boot| RAM_CACHE
    end

    style CAL_MGR fill:#4A90E2,color:#fff
    style OFFSET_CAL fill:#50C878,color:#fff
    style GAIN_CAL fill:#50C878,color:#fff
    style VALID fill:#E24A4A,color:#fff
```

### Module Interactions

1.  **User Interface**: Initiates calibration commands (e.g., "Calibrate Voltage at 220V").
2.  **Measurement Engine**: Provides raw, uncorrected ADC accumulations for processing. Also receives the final calibration factors to apply during normal operation.
3.  **EEPROM Driver**: Persists the validated coefficients.
4.  **RAM Cache**: Holds active calibration values for fast access during runtime calculations, avoiding slow EEPROM reads.

---

## 3. Calibration Procedure

### 3.1 Voltage Calibration

**Objective**: Determine the scaling factor to convert ADC counts to Volts (RMS).

**Steps**:

1.  Connect a stable AC source to the system.
2.  Measure the actual voltage using a high-precision multimeter (Reference Voltage).
3.  Input the Reference Voltage value to the system via the calibration interface.
4.  The system records the average raw ADC RMS value over a set period.
5.  **Calculation**: `VoltageScale = Reference_Voltage / Raw_ADC_RMS`.
6.  The new `VoltageScale` is validated and saved.

### 3.2 Current Calibration

**Objective**: Determine the scaling factor to convert ADC counts to Amperes (RMS).

**Steps**:

1.  Connect a resistive load (e.g., heater, lamp) in series with a high-precision ammeter.
2.  Turn on the load.
3.  Measure the actual current using the ammeter (Reference Current).
4.  Input the Reference Current value to the system.
5.  The system records the average raw ADC RMS value (offset-corrected).
6.  **Calculation**: `CurrentScale = Reference_Current / Raw_ADC_RMS`.
7.  The new `CurrentScale` is validated and saved.

### 3.3 Zero Offset Calibration (Current)

**Objective**: Eliminate the "zero error" of the Hall-effect current sensor.

**Steps**:

1.  Ensure **NO LOAD** is connected (Current = 0A).
2.  Trigger the "Calibrate Zero" command.
3.  The system takes multiple samples (e.g., 1000 samples) and averages them.
4.  This average value becomes the new `ZeroOffset` (voltage center point for AC wave).
5.  **Result**: Future readings will ideally read 0A when no load is present.

---

## 4. State Machine

```mermaid
stateDiagram-v2
    [*] --> Uninitialized

    Uninitialized --> Idle: Init_Complete (Load from EEPROM)

    Idle --> Calibrating_Voltage: CMD_Calibrate_V
    Idle --> Calibrating_Current: CMD_Calibrate_I
    Idle --> Calibrating_Offset: CMD_Calibrate_Offset

    state Calibrating_Voltage {
        [*] --> Sampling_V
        Sampling_V --> Calculating_V: Samples Ready
        Calculating_V --> Validation_V: Factor Computed
        Validation_V --> Saving_V: Valid
        Validation_V --> Error_V: Out of Range
        Saving_V --> [*]
    }

    state Calibrating_Current {
        [*] --> Sampling_I
        Sampling_I --> Calculating_I: Samples Ready
        Calculating_I --> Validation_I: Factor Computed
        Validation_I --> Saving_I: Valid
        Validation_I --> Error_I: Out of Range
        Saving_I --> [*]
    }

    Calibrating_Voltage --> Idle: Success/Fail
    Calibrating_Current --> Idle: Success/Fail
    Calibrating_Offset --> Idle: Success/Fail

    Idle --> Factory_Reset: CMD_Reset
    Factory_Reset --> Idle: Defaults Loaded
```

### State Descriptions

- **Uninitialized**: System startup state. Calibration data is loaded from EEPROM validation checks are performed.
- **Idle**: Normal operation state. System uses current calibration values for measurements.
- **Calibrating_Voltage**: Active state where voltage samples are accumulated for calibration. Normal measurement reporting may be paused or flagged.
- **Calibrating_Current**: Active state for current gain calibration.
- **Calibrating_Offset**: Active state for calculating zero-current DC offset.
- **Factory_Reset**: Reverts all coefficients to hardcoded safe defaults.

---

## 5. Sequence Diagrams

### 5.1 Voltage Calibration Sequence

```mermaid
sequenceDiagram
    participant USER as User/Technician
    participant UI as User Interface
    participant CAL as Calibration Manager
    participant ME as Measurement Engine
    participant EE as EEPROM Driver

    Note over USER: Apply 220V Reference

    USER->>UI: Enter Ref Voltage (220.0V)
    UI->>CAL: CalibrateVoltage(220.0f)
    activate CAL

    CAL->>ME: Request Raw Accumulation
    activate ME
    Note over ME: Accumulate 100 cycles<br/>(approx 1 second)
    ME-->>CAL: Return Raw_ADC_RMS (e.g. 512.5)
    deactivate ME

    CAL->>CAL: Calculate New Factor<br/>k = 220.0 / 512.5 = 0.429

    CAL->>CAL: Validate Range (0.3 < k < 0.6)

    alt is Valid
        CAL->>EE: Save New Factor
        EE-->>CAL: Write Success
        CAL->>ME: Update Runtime Facor
        CAL-->>UI: Calibration Successful
        UI-->>USER: Display "Success"
    else is Invalid
        CAL-->>UI: Error: Out of Range
        UI-->>USER: Display "Calib Failed"
    end
    deactivate CAL
```

### 5.2 Current Calibration Sequence

```mermaid
sequenceDiagram
    participant USER
    participant CAL as Calibration Manager
    participant ME as Measurement Engine
    participant EE as EEPROM Driver

    Note over USER: Apply Load (e.g. 5.0A)

    USER->>CAL: CalibrateCurrent(5.0f)
    activate CAL

    CAL->>ME: Request Raw Current RMS
    ME-->>CAL: Return Raw_I_RMS (e.g. 100.2)

    CAL->>CAL: Calculate Gain<br/>k = 5.0 / 100.2 = 0.0499

    CAL->>CAL: Validate Range

    CAL->>EE: Save Current Gain
    CAL->>ME: Update Runtime Gain

    CAL-->>USER: Success
    deactivate CAL
```

### 5.3 Zero Offset Calibration Sequence

```mermaid
sequenceDiagram
    participant USER
    participant CAL as Calibration Manager
    participant ADC as ADC Driver
    participant EE as EEPROM Driver

    Note over USER: Disconnect All Loads

    USER->>CAL: CalibrateZero()
    activate CAL

    CAL->>ADC: Start Bulk Sampling (Channel 1)

    loop 1000 Samples
        ADC-->>CAL: ADC Value
        CAL->>CAL: Accumulate Sum
    end

    CAL->>CAL: Average = Sum / 1000
    Note over CAL: Expected ~512 (2.5V)<br/>for ACS712

    CAL->>EE: Save Offset Value
    CAL-->>USER: Success
    deactivate CAL
```

---

## 6. Data Structures

### Calibration Data Structure (RAM & EEPROM)

The following structure defines the layout of calibration data as stored in memory.

| Field Name       | Data Type  | Size (Bytes) | Description                                      |
| ---------------- | ---------- | ------------ | ------------------------------------------------ |
| `voltage_gain`   | `float`    | 4            | Scaling factor for voltage (Volts per ADC count) |
| `current_gain`   | `float`    | 4            | Scaling factor for current (Amps per ADC count)  |
| `current_offset` | `uint16_t` | 2            | Zero-current ADC offset (0-1023)                 |
| `checksum`       | `uint16_t` | 2            | CRC-16 or simple checksum for integrity          |
| **Total Size**   |            | **12 Bytes** |                                                  |

### EEPROM Memory Map

| Address Range   | Content           | Description                    |
| --------------- | ----------------- | ------------------------------ |
| `0x00` - `0x0B` | Calibration Block | Main calibration data struct   |
| `0x0C` - `0x0D` | Write Counter     | Track EEPROM wear (optional)   |
| `0x10` - `...`  | Energy Log        | Start of Energy Logger storage |

---

## 7. Configuration Parameters

These default values and limits are defined in `Config.h` and used for validation.

| Parameter        | Default Value | Min Limit | Max Limit | Description                                        |
| ---------------- | ------------- | --------- | --------- | -------------------------------------------------- |
| `DEFAULT_V_GAIN` | 0.488f        | 0.300f    | 0.700f    | Based on hardware divider design (e.g. 500V range) |
| `DEFAULT_I_GAIN` | 0.066f        | 0.040f    | 0.100f    | Based on ACS712 sensitivity (e.g. 66mV/A)          |
| `DEFAULT_OFFSET` | 512           | 450       | 574       | Center point of 10-bit ADC (2.5V approx)           |
| `CALIB_TIMEOUT`  | 5000 ms       | -         | -         | Max time to wait for stable measurement            |
| `STABLE_COUNT`   | 50            | -         | -         | Number of consistent samples required              |

### Calculating Default Gains (Design Time)

**Voltage Gain**:

- Max Input: 500V (Peak)
- Divider Ratio: 100:1 (500V -> 5V)
- ADC Reference: 5V
- ADC Steps: 1023
- Volts per Step: 5V / 1023 ≈ 0.00488V
- Input Voltage per Step: 0.00488V × 100 = 0.488 V/count

**Current Gain**:

- Sensor: ACS712-30A
- Sensitivity: 66 mV/A
- ADC Steps per Volt: 1023 / 5 = 204.6 steps/V
- ADC Steps per Amp: 0.066 V/A × 204.6 steps/V ≈ 13.5 steps/A
- Amps per Step (Gain): 1 / 13.5 ≈ 0.074 A/count

---

## 8. Error Handling Strategy

### Calibration Validation Logic

Before saving any new calibration factor, the system validates it against hard limits to prevent "bricking" the sensor accuracy.

**Validation Tree**:

```mermaid
graph TD
    Start[Calculate Factor] --> CheckMin{Factor > Min?}
    CheckMin -- No --> Error[Reject: Out of Range]
    CheckMin -- Yes --> CheckMax{Factor < Max?}
    CheckMax -- No --> Error
    CheckMax -- Yes --> CheckNaN{Is NaN/Inf?}
    CheckNaN -- Yes --> Error
    CheckNaN -- No --> Success[Accept & Save]

    style Error fill:#E24A4A,color:#fff
    style Success fill:#50C878,color:#fff
```

### Error Codes

| Code              | Trigger Condition                            | System Action                   |
| ----------------- | -------------------------------------------- | ------------------------------- |
| `CAL_ERR_NONE`    | Successful operation                         | Update runtime and EEPROM       |
| `CAL_ERR_RANGE`   | Calculated factor outside min/max limits     | Discard new factor, keep old    |
| `CAL_ERR_TIMEOUT` | Measurement did not stabilize in time        | Abort calibration process       |
| `CAL_ERR_EEPROM`  | Verify-after-write failed                    | Flag EEPROM error, use RAM only |
| `CAL_ERR_INPUT`   | Reference value provided is zero or negative | Reject command                  |

---

## 9. Performance Characteristics

### Accuracy and Precision

| Parameter             | Specification       | Notes                             |
| --------------------- | ------------------- | --------------------------------- |
| **ADC Resolution**    | 10-bit (1024 steps) | ~0.1% of full scale               |
| **Voltage Accuracy**  | ±1.0%               | After 1-point calibration         |
| **Current Accuracy**  | ±2.0%               | Especially at low currents (<1A)  |
| **Calibration Speed** | < 2 seconds         | Sample accumulation + calculation |
| **EEPROM Life**       | 100,000 cycles      | Updates strictly on user command  |

### Drift Factors

- **Temperature**: Reference voltage (AVCC) and resistor divider ratios may drift with heat.
- **Aging**: Capacitor aging in the power supply filter may increase noise over years.
- **Recommendation**: Re-calibrate annually for critical applications.

---

## 10. Module Dependencies

```mermaid
graph LR
    CAL[Calibration Manager] -->|Uses| ME[Measurement Engine]
    CAL -->|Reads/Writes| EE[EEPROM Driver]
    CAL -->|Direct Sample| ADC[ADC Driver]

    ME -.->|Runtime| CAL

    style CAL fill:#4A90E2,color:#fff
```

- **Measurement Engine**: Source of processed RMS values (e.g., getting the average RAW counts).
- **EEPROM Driver**: Handling byte-level storage operations.
- **ADC Driver**: For zero-offset calibration, raw ADC access is sometimes required bypassing RMS filtering.

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
