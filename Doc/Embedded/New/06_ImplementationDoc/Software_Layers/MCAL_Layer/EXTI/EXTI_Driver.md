# ⚡ EXTI Driver - External Interrupts

<div align="center">

![Status](https://img.shields.io/badge/Status-Reserved-yellow)
![Platform](https://img.shields.io/badge/Platform-ATmega32_EXTI-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-MCAL_Driver-brightgreen)

**EXTI Driver**

**Smart Energy Management System - Event Handling**

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

| Document                                  | Description | Status       |
| ----------------------------------------- | ----------- | ------------ |
| **[GIE_Driver.md](../GIE/GIE_Driver.md)** | Interrupts  | ✅ Available |
| **[DIO_Driver.md](../DIO/DIO_Driver.md)** | Pin Config  | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The EXTI (External Interrupt) driver provides hardware-triggered interrupt capability from external signals. Unlike polling-based input reading, external interrupts allow the system to respond immediately to critical events without continuous CPU monitoring. This is essential for time-sensitive inputs like emergency buttons, zero-crossing detection, or pulse counting.

### Key Responsibilities

- Configure external interrupt pins (INT0, INT1, INT2)
- Select trigger condition (rising edge, falling edge, low level, any edge)
- Register user callback functions for interrupt events
- Manage interrupt priority and enabling
- Provide debouncing support (software-based)

### Requirements Traceability (Reserved)

| Requirement ID   | Description         | Implementation                 |
| :--------------- | :------------------ | :----------------------------- |
| **REQ-ARCH-007** | External Interrupts | Support for INT0/INT1/INT2     |
| **REQ-IO-005**   | Button Inputs       | Optional interrupt-driven mode |

### Hardware Peripheral

Utilizes ATmega32's three external interrupt sources:

- **INT0** (PD2): Configurable trigger, highest priority
- **INT1** (PD3): Configurable trigger, medium priority
- **INT2** (PB2): Rising/falling edge only, lower priority
- All can wake MCU from sleep modes

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "External Interrupt Architecture"
        direction TB

        APP[Application<br/>Button/Sensor Handler] -->|Configure| EXTI_API[EXTI Driver API]

        EXTI_API --> CONFIG[Interrupt Configuration]
        EXTI_API --> CALLBACK[Callback Management]

        CONFIG --> MCUCR[MCUCR Register<br/>INT0/INT1 Sense Control]
        CONFIG --> MCUCSR[MCUCSR Register<br/>INT2 Sense Control]
        CONFIG --> GICR[GICR Register<br/>Interrupt Enables]

        EXT_PIN0[External Signal<br/>INT0 Pin PD2] --> EDGE_DET0[Edge/Level<br/>Detector INT0]
        EXT_PIN1[External Signal<br/>INT1 Pin PD3] --> EDGE_DET1[Edge/Level<br/>Detector INT1]
        EXT_PIN2[External Signal<br/>INT2 Pin PB2] --> EDGE_DET2[Edge/Level<br/>Detector INT2]

        EDGE_DET0 --> FLAG0[INTF0 Flag]
        EDGE_DET1 --> FLAG1[INTF1 Flag]
        EDGE_DET2 --> FLAG2[INTF2 Flag]

        FLAG0 -->|If INT0 enabled| ISR0[INT0_vect ISR]
        FLAG1 -->|If INT1 enabled| ISR1[INT1_vect ISR]
        FLAG2 -->|If INT2 enabled| ISR2[INT2_vect ISR]

        ISR0 --> CB0[User Callback]
        ISR1 --> CB1[User Callback]
        ISR2 --> CB2[User Callback]

        CALLBACK -.-> CB0
        CALLBACK -.-> CB1
        CALLBACK -.-> CB2
    end

    style EXTI_API fill:#4A90E2,color:#fff
    style EDGE_DET0 fill:#E24A4A,color:#fff
    style ISR0 fill:#50C878,color:#fff
```

---

## 3. Hardware Interface

### Pin Configuration

| Interrupt | Pin | Alternate Function | Trigger Modes                                  | Priority |
| --------- | --- | ------------------ | ---------------------------------------------- | -------- |
| INT0      | PD2 | GPIO               | Low level, Any edge, Rising edge, Falling edge | Highest  |
| INT1      | PD3 | GPIO               | Low level, Any edge, Rising edge, Falling edge | Medium   |
| INT2      | PB2 | AIN0               | Rising edge, Falling edge                      | Lower    |

### Register Overview

**MCUCR (MCU Control Register) - INT0/INT1 Configuration:**

- **ISC11:ISC10**: INT1 Sense Control
  - 00 = Low level triggers
  - 01 = Any edge triggers
  - 10 = Falling edge triggers
  - 11 = Rising edge triggers
- **ISC01:ISC00**: INT0 Sense Control (same encoding)

**MCUCSR (MCU Control and Status Register) - INT2 Configuration:**

- **ISC2**: INT2 Sense Control
  - 0 = Falling edge triggers
  - 1 = Rising edge triggers

**GICR (General Interrupt Control Register) - Enable Bits:**

- **INT2**: INT2 Enable
- **INT0**: INT0 Enable
- **INT1**: INT1 Enable

**GIFR (General Interrupt Flag Register) - Status Flags:**

- **INTF2**: INT2 Flag (set on trigger)
- **INTF0**: INT0 Flag (set on trigger)
- **INTF1**: INT1 Flag (set on trigger)
- Flags automatically cleared when ISR executes

---

## 4. Data Flow Diagram

```mermaid
flowchart TB
    subgraph "Button Press Example (INT0)"
        direction LR
        BTN[Button Press<br/>Connect PD2 to GND] --> EDGE[Falling Edge<br/>Detected]
        EDGE --> SYNC[Synchronize<br/>to System Clock]
        SYNC --> FLAG[Set INTF0<br/>Flag]
        FLAG --> CHECK{INT0 Enabled?<br/>Global Int Enabled?}
        CHECK -->|Yes| ISR[INT0 ISR<br/>Execute]
        CHECK -->|No| PEND[Pending<br/>Flag remains set]
        ISR --> CB[User Callback<br/>Handle Button]
        CB --> APP[Application<br/>Action]
    end

    subgraph "Configuration Flow"
        direction TB
        INIT[Application Init] --> SET_MODE[Set Trigger Mode<br/>Falling Edge]
        SET_MODE --> REG_CB[Register Callback]
        REG_CB --> EN_INT[Enable INT0]
        EN_INT --> RDY[Ready for Events]
    end

    style EDGE fill:#E24A4A,color:#fff
    style ISR fill:#50C878,color:#fff
    style CB fill:#F39C12,color:#000
```

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> Disabled: Power On / Reset

    Disabled --> Configured: Set Trigger Mode<br/>Register Callback

    Configured --> Enabled: Enable Interrupt<br/>(Set INT0/1/2 bit)

    Enabled --> WaitingTrigger: Monitoring Pin

    WaitingTrigger --> EventDetected: Trigger Condition Met

    EventDetected --> ISR_Pending: Set INTF Flag

    ISR_Pending --> ISR_Executing: Global Int Enabled

    ISR_Executing --> CallbackExecute: Jump to ISR

    CallbackExecute --> WaitingTrigger: RETI, Clear Flag

    Enabled --> Disabled: Disable Interrupt

    note right of Configured
        - Trigger mode selected
        - Callback registered
        - Not yet enabled
    end note

    note right of EventDetected
        Hardware detects edge/level
        Synchronized to clock
        Flag set immediately
    end note

    note right of ISR_Executing
        Flag auto-cleared on ISR entry
        User callback invoked
        Fast execution recommended
    end note
```

---

## 6. Sequence Diagrams

### Initialization and Configuration

```mermaid
sequenceDiagram
    participant APP as Application
    participant EXTI as EXTI Driver
    participant HW as Hardware

    APP->>EXTI: EXTI_Init(INT0, FALLING_EDGE)
    activate EXTI

    EXTI->>HW: Set ISC01:ISC00 = 10 (Falling edge)
    Note over HW: INT0 configured<br/>for falling edge

    EXTI->>APP: Init complete
    deactivate EXTI

    APP->>EXTI: EXTI_SetCallback(INT0, button_handler)
    EXTI->>EXTI: Store callback pointer
    EXTI->>APP: Callback registered

    APP->>EXTI: EXTI_Enable(INT0)
    activate EXTI

    EXTI->>HW: Clear INTF0 (clear any pending)
    EXTI->>HW: Set INT0 bit in GICR

    Note over HW: INT0 now active

    EXTI->>APP: Interrupt enabled
    deactivate EXTI

    Note over APP,HW: System ready for button input
```

### Button Press Handling (INT0 Example)

```mermaid
sequenceDiagram
    participant BTN as Physical Button
    participant PIN as PD2 Pin
    participant HW as Hardware
    participant ISR as INT0 ISR
    participant CB as Button Handler

    Note over BTN: Button idle (released)
    Note over PIN: High (pull-up active)

    BTN->>PIN: Press button (connect to GND)
    PIN->>PIN: Voltage falls HIGH→LOW

    Note over HW: Falling edge detected<br/>Synchronized to clock

    HW->>HW: Set INTF0 flag

    alt Global interrupts enabled
        HW->>ISR: Jump to INT0_vect
        activate ISR

        Note over HW: INTF0 auto-cleared

        ISR->>CB: Call button_handler()
        activate CB

        CB->>CB: Debounce check
        CB->>CB: Process button action

        CB->>ISR: Return
        deactivate CB

        ISR->>ISR: RETI instruction
        deactivate ISR

        Note over HW: Resume main program
    else Global interrupts disabled
        Note over HW: INTF0 remains set<br/>Pending until enabled
    end
```

---

## 7. Module Dependencies

### Dependency Diagram

```mermaid
graph TB
    subgraph "Application Layer"
        BTN_HAL[Button HAL Driver<br/>Future]
        ZC_DET[Zero Crossing Detector<br/>Future]
    end

    subgraph "MCAL Layer"
        EXTI[EXTI Driver]
        GIE[GIE Driver]
        DIO[DIO Driver]
    end

    BTN_HAL -->|Uses| EXTI
    ZC_DET -->|Uses| EXTI

    EXTI -->|Requires| GIE
    EXTI -->|Pin Config| DIO

    style EXTI fill:#4A90E2,color:#fff
```

### Dependencies

**GIE Driver:**

- Global interrupts must be enabled
- Required for ISR execution

**DIO Driver:**

- Pins must be configured as inputs
- Pull-up resistors may be enabled via DIO

### Dependent Modules (Future)

**Button Driver:**

- Emergency stop button
- User input buttons
- Mode selection switches

**Zero-Crossing Detector:**

- AC phase synchronization
- Power measurement accuracy
- Dimmer control (future)

---

## 8. Configuration Parameters

### Trigger Modes

| Mode         | ISC bits    | Description            | Use Case                    |
| ------------ | ----------- | ---------------------- | --------------------------- |
| Low Level    | 00          | Triggers while pin LOW | Level-sensitive detection   |
| Any Edge     | 01          | Triggers on both edges | Toggle detection            |
| Falling Edge | 10          | Triggers on HIGH→LOW   | Button press (active-LOW)   |
| Rising Edge  | 11 / ISC2=1 | Triggers on LOW→HIGH   | Button release, pulse start |

### Priority

| Interrupt | Vector Address | Priority | Notes                              |
| --------- | -------------- | -------- | ---------------------------------- |
| INT0      | 0x0002         | Highest  | Executes first if multiple pending |
| INT1      | 0x0004         | Medium   |                                    |
| INT2      | 0x0006         | Lower    | Limited trigger modes              |

---

## 9. Error Handling Strategy

### Bounce Handling

**Problem:** Mechanical switches bounce (multiple edges during single press)

- Duration: 5-20 ms typically
- Causes multiple interrupts from one press

**Solutions:**

1. Software debouncing in ISR
2. Ignore interrupts within debounce window
3. Hardware RC filter (100Ω + 100nF typical)

### Missed Interrupts

**Problem:** Event occurs while interrupts disabled

- Flag set but ISR doesn't execute

**Solution:** Check flag manually after re-enabling

---

## 10. Performance Characteristics

### Timing

| Metric              | Value      | Notes                |
| ------------------- | ---------- | -------------------- |
| ISR Entry Latency   | 4-5 cycles | ~250-312 ns @ 16 MHz |
| Minimum Pulse Width | ~3 cycles  | For edge detection   |
| Debounce Time (SW)  | 10-50 ms   | Configurable         |

### Resource Usage

- RAM: ~12 bytes (3 callback pointers)
- Flash: ~200 bytes
- Pins: PD2, PD3, PB2

---

## Implementation Notes

### Typical Usage (Future)

**Emergency Stop Button:**

- INT0 on PD2
- Falling edge trigger (button press)
- Highest priority
- Immediately cuts relay power

**Mode Button:**

- INT1 on PD3
- Falling edge trigger
- Debounced in ISR
- Changes display mode

---

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

---

## 📄 Document Control

| Attribute            | Value                     |
| -------------------- | ------------------------- |
| **Document Type**    | EXTI Driver Documentation |
| **Document Status**  | Reserved                  |
| **Document Version** | 2.0                       |
| **Last Updated**     | January 2026              |
| **Prepared By**      | Gestell Engineering Team  |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
