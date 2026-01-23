# 🛡️ Protection Manager

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Application_Layer-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Safety_Module-brightgreen)

**Protection Manager**

**Smart Energy Management System - System Safety & Fault Handling**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Module Overview](#-1-module-overview)
- [Architecture](#-2-architecture-diagram)
- [Protection Logic](#-3-protection-logic-algorithms)
- [State Machine](#-4-state-machine)
- [Sequence Diagrams](#-5-sequence-diagrams)
- [Threshold Configuration](#-6-threshold-configuration)
- [Error Classification](#-7-error-classification)
- [Performance Characteristics](#-8-performance-characteristics)
- [Dependencies](#-9-module-dependencies)
- [Recovery Procedures](#-10-recovery-procedures)

---

## 🔗 Related Documentation

| Document                                                                 | Description      | Status       |
| ------------------------------------------------------------------------ | ---------------- | ------------ |
| **[Relay_Driver.md](../../HAL_Layer/Relay/Relay_Driver.md)**             | Actuator Control | ✅ Available |
| **[Measurement_Engine.md](../Measurement_Engine/Measurement_Engine.md)** | Data Source      | ✅ Available |
| **[Buzzer_Driver.md](../../HAL_Layer/Buzzer/Buzzer_Driver.md)**          | Alarm System     | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The Protection Manager is the safety-critical core of the Smart Energy Management System. Its primary mission is to protect both the connected electrical load and the smart device itself from hazardous electrical conditions. It continuously monitors voltage, current, and system health status to detect anomalies and execute rapid protective actions (tripping the relay) when necessary.

### Key Responsibilities

- **Overcurrent Protection (OCP)**: Detects excessive current draw (short circuit or overload) and disconnects the load to prevent fire or damage.
- **Overvoltage Protection (OVP)**: Monitors mains voltage and isolates the load if voltage surges exceeding safe limits (e.g., >260V).
- **Undervoltage Protection (UVP)**: Disconnects load if voltage drops too low (e.g., <180V), protecting motors and compressors from overheating.
- **Fault Management**: Logs fault types, alerts the user via buzzer/display, and manages the recovery process (auto-recovery or manual reset).
- **Self-Diagnostics**: Monitors sensor health (e.g., detecting broken sensor wires).

### Requirements Traceability

| Requirement ID   | Description           | Implementation                           |
| :--------------- | :-------------------- | :--------------------------------------- |
| **REQ-PROT-001** | Overcurrent Detection | Implemented in `PM_CheckOvercurrent`     |
| **REQ-PROT-005** | Overvoltage Detection | Implemented in `PM_CheckOvervoltage`     |
| **REQ-SAFE-002** | Reaction Time <500ms  | 20ms Task Period ensures <100ms response |

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Protection System Architecture"
        ME[Measurement Engine] -->|Live Data<br/>V, I, P| PROT[Protection Manager]

        subgraph "Decision Core"
            THRESH[Threshold<br/>Comparator]
            HYST[Hysteresis<br/>Logic]
            DEBOUNCE[Time Delay<br/>Filter]
        end

        PROT --> THRESH
        THRESH --> HYST
        HYST --> DEBOUNCE

        DEBOUNCE -->|Action Req| EXEC[Execution Unit]

        EXEC -->|Trip| RELAY[Relay Driver]
        EXEC -->|Alarm| BUZZ[Buzzer Driver]
        EXEC -->|Indicator| LED[RGB LED Driver]

        PROT -->|Fault Status| DISP[Display Manager]
    end

    style PROT fill:#E24A4A,color:#fff
    style THRESH fill:#F39C12,color:#000
    style EXEC fill:#4A90E2,color:#fff
```

### Protection Layers

1.  **Fast Hardware Layer** (Future): If hardware comparators were used, they would bypass software. Currently, we rely on software.
2.  **Fast Software Layer**: Checks raw ADC values for massive spikes (Short Circuit) -> Immediate Trip.
3.  **RMS Layer**: Checks processed RMS values for standard Overload/Overvoltage -> Time-delayed Trip.

---

## 3. Protection Logic Algorithms

### Decision Flowchart

```mermaid
flowchart TD
    Start[New Measurement] --> CheckSC{Current > ShortCircuit Limit?}
    CheckSC -- Yes --> TRIP_FAST[Immediate Trip<br/>< 10ms]
    CheckSC -- No --> CheckOL{Current > Overload Limit?}

    CheckOL -- Yes --> IncTimer[Increment Timer]
    IncTimer --> CheckTime{Time > TripDelay?}
    CheckTime -- Yes --> TRIP_SLOW[Overload Trip<br/>after delay]
    CheckTime -- No --> Continue

    CheckOL -- No --> ResetTimer[Reset Timer]
    ResetTimer --> CheckOV{Voltage > Max Limit?}

    CheckOV -- Yes --> TRIP_OV[Overvoltage Trip]
    CheckOV -- No --> CheckUV{Voltage < Min Limit?}

    CheckUV -- Yes --> TRIP_UV[Undervoltage Trip]
    CheckUV -- No --> Normal[System Normal]

    style TRIP_FAST fill:#E74C3C,color:#fff
    style TRIP_SLOW fill:#E67E22,color:#fff
    style TRIP_OV fill:#F1C40F,color:#000
    style TRIP_UV fill:#F39C12,color:#000
    style Normal fill:#2ECC71,color:#fff
```

### Logic Description

1.  **Short Circuit Check**: Highest priority. Checks instantaneous current. If > 20A, trip immediately (ignoring debounce) to prevent damage.
2.  **Overload Check**: If current between Rated (10A) and Short (20A), start a timer. If condition persists for `OVERLOAD_DELAY` (e.g., 5s), then trip. This allows for inrush currents (e.g., motor starting).
3.  **Voltage Check**: Immediate check for OVP/UVP. Voltage faults typically require faster disconnection than minor overloads to protect electronics.

---

## 4. State Machine

```mermaid
stateDiagram-v2
    [*] --> Init

    Init --> Check_Sensors
    Check_Sensors --> Normal: Sensors OK
    Check_Sensors --> Fault_Sensor: Sensor Fail

    Normal --> Warning: Limit Exceeded<br/>(Wait Time)
    Warning --> Normal: Value Returned<br/>to Range
    Warning --> Fault_Trip: Time Limit Exceeded

    Normal --> Fault_Trip: Critical Value<br/>(Immediate)

    Fault_Trip --> Auto_Recover_Wait: If Auto-Reset Enabled
    Fault_Trip --> Manual_Reset_Wait: If Critical/Manual

    Auto_Recover_Wait --> Normal: Wait Time Over<br/>& Conditions Safe

    Manual_Reset_Wait --> Normal: Button Pressed<br/>& Conditions Safe

    state Fault_Trip {
        [*] --> Relay_OFF
        Relay_OFF --> Buzzer_Alarm
        Buzzer_Alarm --> Log_Error
    }
```

### State Descriptions

- **Normal**: All parameters within safe limits. Relay is potentially ON.
- **Warning**: Parameter exceeded threshold but within delay time (e.g., 12A for 1 second). LED might flash yellow.
- **Fault_Trip**: Protection activated. Relay forced OFF. Buzzer sounding.
- **Auto_Recover_Wait**: System waiting for cool-down period before attempting to reconnect load (useful for grid instability).
- **Manual_Reset_Wait**: System locked out until user interaction (required for Short Circuit events).

---

## 5. Sequence Diagrams

### 5.1 Overcurrent Trip Sequence

```mermaid
sequenceDiagram
    participant ADC as Hardware ADC
    participant ME as Measure Engine
    participant PROT as Protection Manager
    participant RELAY as Relay Driver
    participant UI as User Interface

    ADC->>ME: Raw Current Samples
    ME->>PROT: Update I_RMS = 15.5A

    PROT->>PROT: Check Limits (Max 10A)
    Note over PROT: Overload detected!<br/>Start 3s Timer

    PROT->>UI: Status = WARNING

    loop 3 Seconds
        ME->>PROT: I_RMS = 15.6A
    end

    PROT->>PROT: Timer Expired

    PROT->>RELAY: Relay_TurnOFF(EMERGENCY)
    relay-->>PROT: Success

    PROT->>UI: Status = FAULT_OVERCURRENT
    PROT->>PROT: Activate Buzzer
```

### 5.2 Auto-Recovery Sequence

```mermaid
sequenceDiagram
    participant PROT as Protection Manager
    participant RELAY as Relay Driver
    participant TIMER as System Timer

    Note over PROT: In FAULT state<br/>Relay is OFF

    loop Every 1 sec
        PROT->>PROT: Check Line Voltage
    end

    Note over PROT: Voltage returned to<br/>Normal range

    PROT->>TIMER: Start Recovery Delay (30s)

    loop 30 Seconds
        TIMER-->>PROT: Waiting...
    end

    TIMER->>PROT: Delay Complete

    PROT->>PROT: Final Safety Check
    PROT->>RELAY: Relay_TurnON()
    Note over PROT: System Restored
```

---

## 6. Threshold Configuration

Limits configured in `Protection_Config.h`.

| Parameter     | Default | Range     | Description                            |
| ------------- | ------- | --------- | -------------------------------------- |
| `OVP_LIMIT`   | 260 V   | 240-280 V | Overvoltage cutoff point               |
| `UVP_LIMIT`   | 180 V   | 150-200 V | Undervoltage cutoff point              |
| `OCP_LIMIT`   | 10.0 A  | 1-20 A    | Maximum continuous load (Overload)     |
| `SHORT_LIMIT` | 20.0 A  | Fixed     | Short circuit immediate trip threshold |
| `OCP_DELAY`   | 3000 ms | 1s-10s    | Time tolerance for overload            |
| `REC_TIME`    | 30 s    | 10s-300s  | Auto-recovery wait time                |
| `MAX_RETRIES` | 3       | 1-5       | Max auto-retries before lockout        |

---

## 7. Error Classification

Errors are classified by severity to determine the response strategy.

```mermaid
graph TD
    Faults[System Faults]

    Faults --> Level1[Critical Faults]
    Faults --> Level2[Major Faults]
    Faults --> Level3[Minor Faults]

    Level1 --> SC[Short Circuit]
    Level1 --> SENSOR[Sensor Failure]
    Level1 --> MEM[EEPROM Fail]

    Level2 --> OV[Over Voltage]
    Level2 --> UV[Under Voltage]

    Level3 --> OL["Overload (Mild)"]
    Level3 --> TEMP[High Temp]

    style Level1 fill:#E74C3C,color:#fff
    style Level2 fill:#F39C12,color:#fff
    style Level3 fill:#F1C40F,color:#000
```

### Response Matrix

| Severity     | Fault Type         | Relay Action | Buzzer       | Recovery               |
| ------------ | ------------------ | ------------ | ------------ | ---------------------- |
| **Critical** | Short Circuit      | Instant OFF  | Continuous   | Manual Only            |
| **Major**    | Over/Under Voltage | Instant OFF  | Beep Pattern | Auto-Recover           |
| **Minor**    | Overload           | Delayed OFF  | Intermittent | Auto-Recover (Limited) |

---

## 8. Performance Characteristics

### Reaction Time

The protection system speed depends on the sampling rate and processing pipeline.

| Event Type        | Detection Time   | Actuation Time | Total Trip Time   |
| ----------------- | ---------------- | -------------- | ----------------- |
| **Short Circuit** | < 20 ms          | < 10 ms        | **< 30 ms**       |
| **Voltage Surge** | < 100 ms         | < 10 ms        | **< 110 ms**      |
| **Overload**      | Configured Delay | < 10 ms        | **Delay + 10 ms** |

> **Note**: Relay mechanical switching time (~10ms) is the physical limit for actuation.

### Reliability

- **False Positive Rejection**: Filters short duration spikes (<10ms) unless they exceed short-circuit thresholds.
- **Failsafe**: If MCU watchdog resets, the system reboots in SAFE mode (Relay OFF).

---

## 9. Module Dependencies

```mermaid
graph LR
    PROT[Protection Manager] -->|Reads| ME[Measurement Engine]
    PROT -->|Controls| RELAY[Relay Driver]
    PROT -->|Alerts| BUZZ[Buzzer Driver]
    PROT -->|Logs| LOG[Energy Logger]

    ME -->|Raw/RMS| PROT

    style PROT fill:#E24A4A,color:#fff
```

- **Input**: Measurement Engine (V_RMS, I_RMS).
- **Output**: Relay Driver (ON/OFF), Buzzer (Alerts), Logger (Fault History).

---

## 10. Recovery Procedures

### Auto-Recovery Logic

To improve user experience, the system attempts to restore power automatically for grid-related faults (Voltage).

1.  **Fault Cleared**: Line voltage returns to safe range.
2.  **Wait**: System enters `Auto_Recover_Wait` state. Timer counts down (e.g., 30s).
3.  **Monitor**: If voltage fluctuates again during wait, timer resets.
4.  **Restore**: Once timer expires cleanly, Relay is engaged.

### Manual Reset Logic

Required for load-related faults (Short Circuit) to prevent re-energizing a dangerous fault.

1.  **Fault Latch**: System stays in `Manual_Reset_Wait` indefinitely.
2.  **User Action**: User must press the physical button or send a "RESET" command via app.
3.  **Verification**: System checks conditions one last time.
4.  **Restore**: Relay enabled, counters reset.

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
