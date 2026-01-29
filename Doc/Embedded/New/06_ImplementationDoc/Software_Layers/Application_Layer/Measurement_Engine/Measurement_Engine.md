# 📊 Measurement Engine

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Application_Layer-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Core_Module-brightgreen)

**Measurement Engine**

**Smart Energy Management System - Digital Signal Processing Core**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Module Overview](#-1-module-overview)
- [Architecture](#-2-architecture-diagram)
- [Processing Pipeline](#-3-signal-processing-pipeline)
- [Algorithms](#-4-algorithms-and-math)
- [State Machine](#-5-state-machine)
- [Sequence Diagrams](#-6-sequence-diagrams)
- [Data Structures](#-7-data-structures)
- [Configuration](#-8-configuration-parameters)
- [Accuracy Analysis](#-9-accuracy-and-performance)
- [Dependencies](#-10-module-dependencies)

---

## 🔗 Related Documentation

| Document                                                                    | Description      | Status       |
| --------------------------------------------------------------------------- | ---------------- | ------------ |
| **[ADC_Driver.md](../../MCAL_Layer/ADC/ADC_Driver.md)**                     | Analog Hardware  | ✅ Available |
| **[Calibration_Manager.md](../Calibration_Manager/Calibration_Manager.md)** | Accuracy Control | ✅ Available |
| **[Voltage_Sensor.md](../../HAL_Layer/Voltage_Sensor/Voltage_Sensor.md)**   | Sensor Specs     | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The Measurement Engine (ME) is the system's "mathematical heart." It converts raw analog signals sampled by the ADC into meaningful electrical units (Volts, Amps, Watts, kWh). It implements Digital Signal Processing (DSP) algorithms to calculate Root Mean Square (RMS) values for alternating current (AC) signals, ensuring accurate readings even for non-sinusoidal loads.

### Key Responsibilities

- **Data Acquisition**: Orchestrates sampling of Voltage and Current channels.
- **RMS Calculation**: Computes True RMS values for Voltage and Current.
- **Power Calculation**: Derives Active Power (Watts), Apparent Power (VA), and Power Factor (PF).
- **Energy Integration**: Accumulates power over time to calculate Energy consumption (kWh).
- **Zero-Crossing Detection**: Synchronizes measurements with the AC mains frequency (50/60Hz).

### Requirements Traceability

| Requirement ID   | Description        | Implementation                       |
| :--------------- | :----------------- | :----------------------------------- |
| **REQ-MEAS-016** | RMS Calculation    | Sum of Squares Algorithm implemented |
| **REQ-MEAS-011** | Power Calculation  | P = V _ I _ PF implemented           |
| **REQ-MEAS-015** | Energy Integration | E = P \* dt implemented              |

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Measurement Engine Pipeline"
        ADC[ADC Driver] -->|Raw Samples| ISR[Sample ISR]

        ISR -->|Fill| BUF_RAW["Raw Sample Buffers<br/>(Ping-Pong)"]

        BUF_RAW -->|Process| DSP[DSP Core]

        subgraph "DSP Phases"
            OFFSET[DC Offset Removal]
            SQUARE[Square & Sum]
            ROOT[Mean & Root]
            SCALE[Apply Calibration]
        end

        DSP --> OFFSET
        OFFSET --> SQUARE
        SQUARE --> ROOT
        ROOT --> SCALE

        SCALE -->|Results| DATA_STORE[Measurement Struct]

        DATA_STORE -->|Reads| APP[Application Layer]
        CAL[Calibration Manager] -->|Factors| SCALE
    end

    style DSP fill:#4A90E2,color:#fff
    style BUF_RAW fill:#F39C12,color:#000
    style DATA_STORE fill:#2ECC71,color:#fff
```

### Pipeline Concept

1.  **Acquisition Phase** (Interrupt Context): High-speed sampling (e.g., 2.5 kHz) triggered by timer. Stores raw ADC counts (0-1023).
2.  **Processing Phase** (Main Loop): Triggered when a buffer is full. Performs math heavy lifting (float/long calculations) to avoid blocking interrupts.
3.  **Publication Phase**: Updates global data structures available to Display and Communications.

---

## 3. Signal Processing Pipeline

### Detailed Data Flow

```mermaid
flowchart LR
    Channel_0["Voltage ADC"] -->|Sample| Array_V["V_Buffer[64]"]
    Channel_1["Current ADC"] -->|Sample| Array_I["I_Buffer[64]"]

    Array_V -->|High Pass Filter| Fltr_V["Filtered V"]
    Array_I -->|High Pass Filter| Fltr_I["Filtered I"]

    Fltr_V -->|Square| Sq_V["V²"]
    Fltr_I -->|Square| Sq_I["I²"]

    Sq_V -->|Accumulate| Sum_V["∑V²"]
    Sq_I -->|Accumulate| Sum_I["∑I²"]

    Sum_V -->|Divide & Sqrt| RMS_V["V_RMS Raw"]
    Sum_I -->|Divide & Sqrt| RMS_I["I_RMS Raw"]

    RMS_V -->|Multiply Gain| Result_V["Voltage (V)"]
    RMS_I -->|Multiply Gain| Result_I["Current (A)"]

    Result_V & Result_I -->|Multiply| Power["Power (W)"]
    Power -->|Integrate| Energy["Energy (Wh)"]
```

---

## 4. Algorithms and Math

Since the MCU (ATmega32) has no FPU (Floating Point Unit), algorithms must be optimized.

### 4.1 RMS Calculation (True RMS)

The Root Mean Square value is defined as:

$$ V\_{RMS} = \sqrt{ \frac{1}{N} \sum\_{n=0}^{N-1} V[n]^2 } $$

**Implementation Steps**:

1.  **Remove DC Offset**: $V\_{AC} = V\_{Raw} - V\_{Offset}$ (where offset is approx 512).
2.  **Square**: $Accumulator += V\_{AC} \times V\_{AC}$.
3.  **Average**: $MeanSquare = Accumulator / SampleCount$.
4.  **Root**: $RMS\_{Raw} = \sqrt{MeanSquare}$.

### 4.2 Power Calculation

**Active Power (Real Power)**:
$$ P = V\_{RMS} \times I\_{RMS} \times \cos(\phi) $$

_Note: For Phase 1 implementation, assuming resistive load or calculating Apparent Power (VA) directly as $V \times I$._

### 4.3 Energy Calculation

Energy is the integral of power over time.

$$ E = \int P(t) dt \approx \sum (P\_{avg} \times \Delta t) $$

**Accumulator Logic**:

- `EnergyAccumulater += Power_Watts * UpdateInterval_Seconds`
- Convert unit: `Energy_Wh = EnergyAccumulator / 3600`
- Since 3600 is large and Watt-Hours accumulate slowly, we use internal `micro_watt_hours` or `milli_joules` counter to preserve precision.

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> Idle

    Idle --> Sampling: Timer Trigger

    state Sampling {
        [*] --> Capture_V
        Capture_V --> Capture_I: ADC Complete
        Capture_I --> Check_Count: ADC Complete
        Check_Count --> Capture_V: Count < Window
        Check_Count --> Processing: Count == Window
    }

    Processing --> Calculating_RMS
    Calculating_RMS --> Calculating_Power
    Calculating_Power --> Updating_Energy
    Updating_Energy --> Ready

    Ready --> Idle: Data Published
```

### State Descriptions

- **Idle**: Waiting for next 20ms AC cycle window.
- **Sampling**: Fast acquisition loop (Interleaved V/I samples).
- **Processing**: Calculations performed on "snapshot" data buffers. V and I buffers are locked.
- **Ready**: New valid measurements are available for other modules.

---

## 6. Sequence Diagrams

### 6.1 Sample Acquisition

```mermaid
sequenceDiagram
    participant TMR as Timer0
    participant ISR as ADC ISR
    participant BUF as Raw Buffer
    participant ME as Main Loop

    TMR->>ISR: Interrupt (2.5kHz)
    activate ISR

    ISR->>ISR: Read ADC Ch0 (V)
    ISR->>BUF: Store V[i]

    ISR->>ISR: Read ADC Ch1 (I)
    ISR->>BUF: Store I[i]

    ISR->>ISR: Increment Index

    alt Index == 50
        ISR->>ME: Set Flag: DATA_READY
        ISR->>ISR: Reset Index
    end

    deactivate ISR
```

### 6.2 Data Processing Workflow

```mermaid
sequenceDiagram
    participant ME as Measure Engine
    participant CAL as Calibration Mgr
    participant APP as Display/Protection

    Loop MainTask
        ME->>ME: Check DATA_READY

        opt Data is Ready
            ME->>ME: Calculate Raw RMS (V & I)

            ME->>CAL: Get Calibration Factors
            CAL-->>ME: V_Gain, I_Gain, V_Offset

            ME->>ME: Apply Scaling (Raw * Gain)

            ME->>ME: Calculate Power (V * I)
            ME->>ME: Accumulate Energy (+ P*dt)

            ME->>APP: Update Shared Struct
        end
    end
```

---

## 7. Data Structures

### Measurement Output Structure

| Field          | Type     | Unit  | Description                 |
| -------------- | -------- | ----- | --------------------------- |
| `voltage_rms`  | `float`  | Volts | RMS Voltage (e.g., 220.5)   |
| `current_rms`  | `float`  | Amps  | RMS Current (e.g., 5.42)    |
| `power_active` | `float`  | Watts | Active Power (e.g., 1195.2) |
| `energy_total` | `double` | Wh    | Accumulated Energy          |
| `p_factor`     | `float`  | 0-1.0 | Power Factor (Optional)     |
| `frequency`    | `float`  | Hz    | Mains Frequency (Optional)  |

### Internal Buffers

| Buffer Name | Size | Type       | Usage                       |
| ----------- | ---- | ---------- | --------------------------- |
| `raw_volts` | 64   | `uint16_t` | Raw ADC samples for Voltage |
| `raw_amps`  | 64   | `uint16_t` | Raw ADC samples for Current |
| `sq_sum_v`  | 1    | `uint32_t` | Sum of squares accumulator  |

---

## 8. Configuration Parameters

Configured in `Measurement_Config.h`.

| Parameter        | Value   | Description                                            |
| ---------------- | ------- | ------------------------------------------------------ |
| `SAMPLE_RATE`    | 2500 Hz | Samples per second (for 50Hz, gives 50 samples/cycle)  |
| `WINDOW_SIZE`    | 50      | Number of samples per calculation window (20ms @ 50Hz) |
| `ADC_REF`        | 5.0 V   | ADC reference voltage                                  |
| `V_SENSOR_RATIO` | 100.0   | Resistor divider ratio                                 |
| `I_SENSOR_SENS`  | 66 mV/A | Current sensor sensitivity                             |
| `CALC_INTERVAL`  | 1000 ms | Frequency of updating user-facing values               |

---

## 9. Accuracy and Performance

### Accuracy Factors

1.  **ADC Quantization**: 10-bit ADC has 1024 steps.
    - Voltage Range 0-500V. Resolution $\approx 0.5V$.
    - Current Range 0-30A. Resolution $\approx 0.03A$.
    - _Improvement_: Oversampling helps improve effective resolution.
2.  **Noise**: System noise floor affects zero-current reading.
    - _Solution_: "Zero-Clamp" logic. Force current to 0A if read value < 0.05A.
3.  **Timing Jitter**: Variations in sample timing affect non-sinusoidal accuracy.
    - _Solution_: Use hardware timer interrupts for consistent sampling.

### Performance

- **CPU Load**: Interrupt runs every 400µs. ISR duration < 50µs (~12% load).
- **Math Overhead**: `sqrt()` is expensive. Processing runs only once per 20ms (AC cycle) or once per 1s (Display update).
- **Memory**:
  - Buffers: 64 _ 2 _ 2 = 256 bytes SRAM.
  - Structs: ~40 bytes.
  - Total: ~300 bytes (Fits comfortably in 2KB SRAM).

---

## 10. Module Dependencies

```mermaid
graph LR
    ME[Measurement Engine] -->|Hardware| ADC[ADC Drivers]
    ME -->|Factors| CAL[Calibration Manager]
    ME -->|Timebase| TMR[Timer0 / System Time]

    PROT[Protection Manager] -->|Reads| ME
    DISP[Display Manager] -->|Reads| ME
    LOG[Energy Logger] -->|Reads| ME

    style ME fill:#4A90E2,color:#fff
```

- **Hardware**: Strictly dependent on ADC configuration.
- **Consumers**: Provides data to almost all Application Layer modules.

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
