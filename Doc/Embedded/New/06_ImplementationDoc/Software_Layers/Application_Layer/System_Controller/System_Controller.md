# System Controller - Main System Orchestration

**Purpose**: Overall system coordination and initialization  
**Layer**: Application  
**Role**: Main entry point and system lifecycle management

---

## 1. Module Overview

### Purpose and Role

The System Controller serves as the main orchestrator of the Smart Energy Management System, managing initialization of all subsystems, coordinating high-level tasks, and supervising system state transitions.

### Key Responsibilities

- Initialize all hardware and software modules in correct order
- Coordinate periodic tasks scheduling
- Manage system states and mode transitions
- Watchdog management (future)
- Power mode control (future)
- Error recovery orchestration

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "System Controller Architecture"
        MAIN[Main Entry Point] --> SYSCTRL[System Controller]

        SYSCTRL --> INIT[Initialization<br/>Sequence]
        SYSCTRL --> TASK[Task Scheduler]
        SYSCTRL --> STATE[State Manager]

        INIT --> MCAL_INIT[MCAL Init<br/>ADC, Timer1, UART, DIO]
        INIT --> HAL_INIT[HAL Init<br/>Sensors, LCD, Relay]
        INIT --> APP_INIT[Application Init<br/>ME, PROT, DISP]

        TASK --> ME_TASK[Measurement Engine<br/>Real-time processing]
        TASK --> PROT_TASK[Protection Manager<br/>Safety monitoring]
        TASK --> COMM_TASK[Communication Manager<br/>Data transmission]
        TASK --> DISP_TASK[Display Manager<br/>UI updates]

        STATE --> NORMAL[Normal Operation]
        STATE --> FAULT[Fault State]
        STATE --> CALIB[Calibration Mode]

        SYSCTRL -.->|Future| WDT[Watchdog Timer]
        SYSCTRL -.->|Future| PWR[Power Management]
    end

    style SYSCTRL fill:#4A90E2,color:#fff
    style INIT fill:#50C878,color:#fff
    style TASK fill:#F39C12,color:#000
```

---

## 3. Initialization Sequence

```mermaid
sequenceDiagram
    participant MAIN as main()
    participant SYSCTL as System Controller
    participant MCAL as MCAL Layer
    participant HAL as HAL Layer
    participant APP as Application Layer

    MAIN->>SYSCTL: SystemController_Init()
    activate SYSCTL

    Note over SYSCTL: Phase 1: MCAL Initialization

    SYSCTL->>MCAL: DIO_Init()
    SYSCTL->>MCAL: ADC_Init()
    SYSCTL->>MCAL: Timer1_Init_CTC()
    SYSCTL->>MCAL: UART_Init(9600)
    SYSCTL->>MCAL: EEPROM_Init()
    SYSCTL->>MCAL: GIE_Enable()

    Note over SYSCTL: Phase 2: HAL Initialization

    SYSCTL->>HAL: VoltageSensor_Init()
    SYSCTL->>HAL: CurrentSensor_Init()
    SYSCTL->>HAL: LCD_Init()
    SYSCTL->>HAL: Relay_Init()
    SYSCTL->>HAL: Buzzer_Init()
    SYSCTL->>HAL: RGB_LED_Init()

    Note over SYSCTL: Phase 3: Application Initialization

    SYSCTL->>APP: MeasurementEngine_Init()
    SYSCTL->>APP: ProtectionManager_Init()
    SYSCTL->>APP: DisplayManager_Init()
    SYSCTL->>APP: CommunicationManager_Init()

    SYSCTL->>SYSCTL: Set state = NORMAL_OPERATION

    deactivate SYSCTL

    Note over MAIN: Enter main loop:<br/>while(1) SystemController_Task()
```

---

## 4. Task Scheduling

```mermaid
flowchart TB
    MAINLOOP[Main Loop<br/>while 1] --> TASK_CALL[SystemController_Task]

    TASK_CALL --> ME{Measurement<br/>Data Ready?}
    ME -->|Yes| PROC_ME[Process Measurements]
    ME -->|No| PROT

    PROC_ME --> PROT{Protection<br/>Check Due?}
    PROT -->|Yes| CHECK_PROT[Check Thresholds]
    PROT -->|No| COMM

    CHECK_PROT --> COMM{Communication<br/>Scheduled?}
    COMM -->|Yes| SEND_DATA[Transmit Data]
    COMM -->|No| DISP

    SEND_DATA --> DISP{Display<br/>Update Due?}
    DISP -->|Yes| UPDATE_LCD[Update LCD]
    DISP -->|No| IDLE

    UPDATE_LCD --> IDLE[Idle/Sleep<br/>Low Power]

    IDLE --> MAINLOOP

    style PROC_ME fill:#50C878,color:#fff
    style CHECK_PROT fill:#E24A4A,color:#fff
    style UPDATE_LCD fill:#F39C12,color:#000
```

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> PowerOn

    PowerOn --> Initializing: System Boot
    Initializing --> SelfTest: Init Complete

    SelfTest --> Normal: All Tests Pass
    SelfTest --> Fault: Test Failed

    Normal --> Calibration: User Request
    Calibration --> Normal: Calibration Complete

    Normal --> Fault: Protection Triggered
    Fault --> Normal: Fault Cleared

    Normal --> Standby: Low Power Request
    Standby --> Normal: Wake Event

    note right of Initializing
        Initialize all modules
        Load EEPROM settings
        Self-test sensors
    end note

    note right of Normal
        Active monitoring
        Periodic measurements
        Data transmission
        Display updates
    end note

    note right of Fault
        Relay OFF
        Alert user (buzzer + LED)
        Wait for fault clearance
    end note
```

---

## 6. Module Dependencies

```mermaid
graph TB
    subgraph "System Controller Dependencies"
        SYSCTRL[System Controller]

        SYSCTRL -->|Initializes| ME[Measurement Engine]
        SYSCTRL -->|Initializes| PROT[Protection Manager]
        SYSCTRL -->|Initializes| DISP[Display Manager]
        SYSCTRL -->|Initializes| COMM[Communication Manager]
        SYSCTRL -->|Initializes| CAL[Calibration Manager]

        ME -->|Uses| VSENS[Voltage Sensor]
        ME -->|Uses| CSENS[Current Sensor]

        PROT -->|Controls| RELAY[Relay]
        PROT -->|Alerts| BUZZER[Buzzer]

        DISP -->|Updates| LCD[LCD]
        COMM -->|Uses| ESP[ESP-01]
        COMM -->|Uses| BT[HC-05]
    end

    style SYSCTRL fill:#4A90E2,color:#fff
```

---

## 7. Configuration Parameters

| Parameter          | Value    | Description                  |
| ------------------ | -------- | ---------------------------- |
| Task rate          | ~10 Hz   | Main loop iteration rate     |
| Watchdog timeout   | 1 second | Future WDT implementation    |
| Startup delay      | 100 ms   | Allow hardware stabilization |
| Self-test duration | 500 ms   | Sensor validation time       |

---

## 8. Error Handling

**Initialization Failures:**

- If MCAL init fails: Halt and blink error code
- If HAL init fails: Continue with degraded functionality
- If calibration data corrupted: Use defaults, flag for recalibration

**Runtime Errors:**

- Module fault: Disable affected module, log error
- Communication failure: Retry with backoff
- Critical fault: Enter safe state (relay OFF)

---

## 9. Performance Characteristics

**Boot Time:**

- Hardware init: ~50 ms
- Software init: ~100 ms
- Total boot: ~150 ms

**CPU Utilization:**

- Idle: ~5% (mainly measurements)
- Active: ~20% (with communication)
- Peak: ~40% (during complex processing)

**Memory Usage:**

- RAM: ~100 bytes (state variables)
- Flash: ~1 kB (initialization code)

---

## Implementation Notes

### Initialization Order

Critical order to prevent issues:

1. **DIO first**: Other peripherals need pins configured
2. **Timers before ADC**: ADC needs Timer1 trigger
3. **GIE last in MCAL**: Enable interrupts after all setup
4. **Sensors before engines**: ME needs sensor drivers ready

### Task Scheduling Strategy

- **No RTOS**: Simple cooperative multitasking
- **Periodic tasks**: Triggered by flags from ISRs
- **Priority**: Protection > Measurement > Display > Communication

---

**Document Version**: 2.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
