# Buzzer Driver - Audio Alert System

**Hardware**: Active 5V Buzzer  
**Control**: Digital ON/OFF via GPIO  
**Pin**: PB4  
**Purpose**: Audible feedback for system events and alerts

---

## 1. Module Overview

### Purpose and Role

The Buzzer driver provides audible feedback for system events, user actions, and protection alerts through an active buzzer that requires only a digital control signal.

### Key Responsibilities

- Generate single beeps and beep patterns
- Distinguish different alert types by pattern
- Non-blocking pattern execution
- Volume control (on/off only)
- Integration with protection and UI systems

### Hardware Component

- Type: Active buzzer with internal oscillator
- Operating voltage: 5V DC
- Frequency: 2.3 kHz ± 300 Hz (internal)
- Sound level: 85 dB @ 10 cm
- Current: ~30 mA when active

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Buzzer Driver Architecture"
        APP[Application/<br/>Protection/UI] -->|Beep Command| BUZZ_API[Buzzer Driver API]

        BUZZ_API --> SINGLE[Single Beep]
        BUZZ_API --> PATTERN[Pattern Generator]
        BUZZ_API --> STATE[State Machine]

        STATE --> DIO[DIO Driver<br/>PB4]

        DIO -->|HIGH/LOW| PIN[PB4 Pin]

        PIN -->|5V/0V| BUZZER[Active Buzzer<br/>Internal Oscillator]

        BUZZER -->|Sound| AUDIO[Audible Alert<br/>2.3 kHz Tone]

        subgraph "Beep Patterns"
            NORMAL[Normal Alert<br/>3 beeps]
            CRITICAL[Critical Alert<br/>5 beeps]
            WARNING[Warning<br/>2 short beeps]
        end

        PATTERN --> NORMAL
        PATTERN --> CRITICAL
        PATTERN --> WARNING
    end

    style BUZZ_API fill:#4A90E2,color:#fff
    style BUZZER fill:#E24A4A,color:#fff
    style AUDIO fill:#50C878,color:#fff
```

---

## 3. Data Flow

```mermaid
flowchart TB
    EVENT[System Event] --> SELECT{Event Type?}

    SELECT -->|Overcurrent| PATTERN_3[3 Beeps Pattern]
    SELECT -->|Overvoltage| PATTERN_5[5 Beeps Pattern]
    SELECT -->|Config Change| PATTERN_2[2 Short Beeps]
    SELECT -->|User Action| SINGLE[Single Beep]

    PATTERN_3 --> SM[Beep State Machine]
    PATTERN_5 --> SM
    PATTERN_2 --> SM
    SINGLE --> SM

    SM --> BEEP_ON[State: BEEP ON<br/>PB4 = HIGH]
    BEEP_ON --> WAIT_1[Wait Duration]
    WAIT_1 --> BEEP_OFF[State: BEEP OFF<br/>PB4 = LOW]
    BEEP_OFF --> WAIT_2[Wait Gap]
    WAIT_2 --> MORE{More Beeps?}
    MORE -->|Yes| BEEP_ON
    MORE -->|No| IDLE[State: IDLE]

    style BEEP_ON fill:#50C878,color:#fff
    style BEEP_OFF fill:#E24A4A,color:#fff
```

---

## 4. State Machine

```mermaid
stateDiagram-v2
    [*] --> IDLE

    IDLE --> BEEP_ON: Start Pattern

    BEEP_ON --> BEEP_OFF: Duration Elapsed

    BEEP_OFF --> BEEP_ON: More Beeps
    BEEP_OFF --> IDLE: Pattern Complete

    note right of BEEP_ON
        PB4 = HIGH
        Buzzer active (30 mA)
        Typical: 50-200ms
    end note

    note right of BEEP_OFF
        PB4 = LOW
        Buzzer silent
        Gap: 50-200ms
    end note
```

---

## 5. Beep Patterns

### Pattern Definitions

| Pattern        | Beeps | Duration | Gap   | Total Time | Use Case                 |
| -------------- | ----- | -------- | ----- | ---------- | ------------------------ |
| Single         | 1     | 150ms    | -     | 150ms      | User action confirmation |
| Warning        | 2     | 50ms     | 50ms  | 150ms      | Configuration change     |
| Normal Alert   | 3     | 100ms    | 100ms | 500ms      | Overcurrent protection   |
| Critical Alert | 5     | 100ms    | 100ms | 900ms      | Overvoltage protection   |

---

## 6. Module Dependencies

```mermaid
graph TB
    PROT[Protection Manager] -->|Alert| BUZZER[Buzzer Driver]
    UI[User Interface] -->|Feedback| BUZZER
    SYS[System Controller] -->|Events| BUZZER

    BUZZER -->|Control| DIO[DIO Driver]

    style BUZZER fill:#4A90E2,color:#fff
```

---

## 7. Configuration Parameters

| Parameter         | Value     | Description         |
| ----------------- | --------- | ------------------- |
| Min beep duration | 50ms      | Minimum audible     |
| Max beep duration | 500ms     | Maximum comfortable |
| Typical beep      | 100-150ms | Standard duration   |
| Gap duration      | 50-200ms  | Between beeps       |
| Max pattern time  | 2 seconds | User comfort limit  |

---

## 8. Performance Characteristics

**Power:**

- Active: 30 mA @ 5V
- Idle: < 1 µA
- Energy per beep: ~4.5 mJ (100ms @ 30mA)

**Timing:**

- Response time: < 1ms (GPIO switching)
- Pattern accuracy: ±5ms

---

## 9. Implementation Notes

### Non-Blocking Operation

Pattern execution via timer-based state machine allows main system to continue during beep sequences.

### Safety Considerations

- Limited pattern duration prevents user annoyance
- Distinct patterns for different alert types
- Silent failure acceptable (disconnected buzzer)

---

**Document Version**: 2.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
