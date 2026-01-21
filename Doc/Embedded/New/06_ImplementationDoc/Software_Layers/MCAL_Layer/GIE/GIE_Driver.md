# GIE Driver - Global Interrupt Enable

**MCU**: ATmega32  
**Purpose**: Master interrupt system control  
**Scope**: System-wide interrupt enable/disable mechanism

---

## 1. Module Overview

### Purpose and Role

The GIE (Global Interrupt Enable) driver provides centralized control over the ATmega32's interrupt system. It acts as a master switch that can enable or disable ALL interrupts simultaneously, regardless of individual peripheral interrupt settings. This is critical for implementing atomic operations and protecting critical code sections.

### Key Responsibilities

- Enable global interrupt system (allow ISR execution)
- Disable global interrupt system (prevent ISR execution)
- Protect critical code sections from interruption
- Coordinate with peripheral interrupt enables
- Provide atomic operation support

### Hardware Peripheral

Utilizes the Status Register (SREG) I-bit (Interrupt Enable Flag):

- Single bit control (bit 7 of SREG)
- Master enable for all interrupts
- Set by SEI (Set Interrupt Enable) instruction
- Cleared by CLI (Clear Interrupt) instruction
- Automatically cleared on interrupt entry, restored on RETI

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "Global Interrupt System"
        direction TB

        APP[Application/<br/>System Init] -->|Control| GIE_API[GIE Driver<br/>mGIE_Enable / mGIE_Disable]

        GIE_API -->|SEI Instruction| SREG_I[SREG I-bit<br/>Interrupt Flag]
        GIE_API -->|CLI Instruction| SREG_I

        SREG_I -->|Gate| INT_CTRL[Interrupt<br/>Controller]

        subgraph "Peripheral Interrupts"
            ADC_INT[ADC<br/>ADIE]
            UART_INT[UART<br/>RXCIE]
            TIMER_INT[Timer1<br/>OCIE1A]
            EXTI_INT[External<br/>INT0/INT1]
        end

        ADC_INT -->|Request| INT_CTRL
        UART_INT -->|Request| INT_CTRL
        TIMER_INT -->|Request| INT_CTRL
        EXTI_INT -->|Request| INT_CTRL

        INT_CTRL -->|Vectored| ISR[ISR Execution<br/>CPU]

        ISR -.->|Auto Clear| SREG_I
        ISR -.->|RETI Auto Restore| SREG_I
    end

    style GIE_API fill:#4A90E2,color:#fff
    style SREG_I fill:#E24A4A,color:#fff
    style INT_CTRL fill:#50C878,color:#fff
```

---

## 3. Hardware Interface

### SREG (Status Register)

**Bit 7: I (Global Interrupt Enable)**

- **0**: All interrupts disabled (masked)
- **1**: Interrupts enabled (individual peripheral flags control actual execution)
- Default after reset: 0 (interrupts disabled)

**Related Instructions:**

- **SEI**: Set Interrupt Enable (sets I-bit to 1)
- **CLI**: Clear Interrupt (clears I-bit to 0)
- **RETI**: Return from Interrupt (automatically restores I-bit from stack)

### Interaction with Peripheral Interrupts

```mermaid
graph LR
    I_BIT[SREG I-bit<br/>Global Enable] --> AND1{AND}
    ADIE[ADC ADIE<br/>Peripheral Enable] --> AND1
    ADIF[ADC ADIF<br/>Interrupt Flag] --> AND1
    AND1 -->|All 1| ADC_ISR[ADC ISR<br/>Executes]

    I_BIT --> AND2{AND}
    RXCIE[UART RXCIE] --> AND2
    RXC[UART RXC] --> AND2
    AND2 --> UART_ISR[UART ISR<br/>Executes]
```

**Interrupt Execution Conditions:**
All three must be true for ISR to execute:

1. **Global enable**: SREG I-bit = 1
2. **Peripheral enable**: Individual interrupt enable bit set (e.g., ADIE, RXCIE)
3. **Interrupt flag**: Event occurred, flag set (e.g., ADIF, RXC)

---

## 4. Data Flow Diagram

```mermaid
flowchart TB
    subgraph "Interrupt Enable Flow"
        INIT[System Initialization] --> PERIPH[Initialize Peripherals<br/>Set individual enables]
        PERIPH --> GIE_EN[mGIE_Enable()<br/>Execute SEI instruction]
        GIE_EN --> I_SET[SREG I-bit = 1]
        I_SET --> SYS_RDY[System Ready<br/>Interrupts Active]
    end

    subgraph "Interrupt Disable Flow (Critical Section)"
        CRIT_START[Enter Critical Section] --> GIE_DIS[mGIE_Disable()<br/>Execute CLI instruction]
        GIE_DIS --> I_CLR[SREG I-bit = 0]
        I_CLR --> ATOMIC[Atomic Operation<br/>No ISR can run]
        ATOMIC --> GIE_EN2[mGIE_Enable()<br/>Execute SEI instruction]
        GIE_EN2 --> CRIT_END[Exit Critical Section]
    end

    subgraph "ISR Execution Flow"
        EVENT[Peripheral Event] --> FLAG[Set Interrupt Flag]
        FLAG --> CHECK{I-bit = 1?<br/>Peripheral EN = 1?}
        CHECK -->|Yes| SAVE_I[Auto Clear I-bit<br/>Save SREG to stack]
        CHECK -->|No| IGNORE[Ignore Interrupt]
        SAVE_I --> EXEC_ISR[Execute ISR]
        EXEC_ISR --> RETI[RETI Instruction]
        RETI --> RESTORE_I[Restore I-bit from stack]
        RESTORE_I --> CONT[Continue Main Program]
    end

    style I_SET fill:#50C878,color:#fff
    style I_CLR fill:#E24A4A,color:#fff
    style ATOMIC fill:#F39C12,color:#000
```

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> Disabled: Power On / Reset

    Disabled --> Enabled: SEI Instruction<br/>(mGIE_Enable)

    Enabled --> Disabled: CLI Instruction<br/>(mGIE_Disable)

    Enabled --> ISR_Executing: Interrupt Event<br/>(Auto I-bit clear)

    ISR_Executing --> Enabled: RETI Instruction<br/>(Auto I-bit restore)

    note right of Disabled
        SREG I-bit = 0
        No ISRs can execute
        Peripheral flags still set
        Used for critical sections
    end note

    note right of Enabled
        SREG I-bit = 1
        ISRs can execute if:
        - Peripheral enable bit set
        - Interrupt flag set
    end note

    note right of ISR_Executing
        CPU executing ISR
        I-bit temporarily cleared
        Nested interrupts disabled
        RETI restores previous state
    end note
```

### State Descriptions

**Disabled:**

- SREG I-bit = 0
- All interrupts masked globally
- Peripheral interrupt flags still set (pending)
- ISRs cannot execute
- Used for atomic operations

**Enabled:**

- SREG I-bit = 1
- Interrupts allowed based on peripheral settings
- ISRs execute when conditions met
- Normal operation mode

**ISR_Executing:**

- CPU executing interrupt service routine
- I-bit automatically cleared on ISR entry
- Prevents nested interrupts (default behavior)
- I-bit automatically restored by RETI

---

## 6. Sequence Diagrams

### System Initialization Sequence

```mermaid
sequenceDiagram
    participant MAIN as Main Program
    participant PERIPH as Peripherals
    participant GIE as GIE Driver
    participant HW as Hardware

    Note over MAIN: System startup

    MAIN->>PERIPH: Initialize ADC
    PERIPH->>PERIPH: Set ADIE = 1 (ADC interrupt enable)

    MAIN->>PERIPH: Initialize UART
    PERIPH->>PERIPH: Set RXCIE = 1 (UART RX interrupt enable)

    MAIN->>PERIPH: Initialize Timer1
    PERIPH->>PERIPH: Set OCIE1A = 1 (Timer1 interrupt enable)

    Note over MAIN: All peripherals configured<br/>but interrupts not active yet

    MAIN->>GIE: mGIE_Enable()
    GIE->>HW: Execute SEI instruction
    HW->>HW: Set SREG I-bit = 1

    Note over HW: Interrupts now active<br/>ISRs can execute

    GIE->>MAIN: Return

    Note over MAIN: System operational<br/>with interrupts enabled
```

### Critical Section Protection Sequence

```mermaid
sequenceDiagram
    participant MAIN as Main Program
    participant GIE as GIE Driver
    participant HW as Hardware
    participant INT as Interrupt System

    Note over MAIN: Need to perform<br/>atomic operation

    MAIN->>GIE: mGIE_Disable()
    GIE->>HW: Execute CLI instruction
    HW->>HW: Clear SREG I-bit = 0
    GIE->>MAIN: Return

    Note over INT: Interrupts blocked<br/>  Flags still set (pending)

    MAIN->>MAIN: Critical operation<br/>(e.g., 16-bit read/write)

    Note over MAIN: Operation complete

    MAIN->>GIE: mGIE_Enable()
    GIE->>HW: Execute SEI instruction
    HW->>HW: Set SREG I-bit = 1
    GIE->>MAIN: Return

    Note over INT: Pending interrupts<br/>now execute if flags set

    alt Interrupt pending
        INT->>INT: Execute pending ISR
    end
```

### ISR Execution with Automatic I-bit Control

```mermaid
sequenceDiagram
    participant MAIN as Main Program
    participant HW as Hardware (SREG)
    participant INT as Interrupt Controller
    participant ISR as ISR Code

    Note over MAIN: Normal execution<br/>SREG I-bit = 1

    INT->>INT: Interrupt event detected

    INT->>HW: Save PC to stack
    INT->>HW: Save SREG to stack (I-bit = 1)
    INT->>HW: Clear SREG I-bit = 0 (auto)

    Note over HW: Nested interrupts<br/>now disabled

    INT->>ISR: Jump to interrupt vector

    activate ISR
    ISR->>ISR: Execute ISR code
    ISR->>ISR: Process interrupt
    deactivate ISR

    ISR->>HW: RETI instruction

    HW->>HW: Restore SREG from stack (I-bit = 1)
    HW->>HW: Restore PC from stack

    Note over HW: I-bit automatically<br/>restored to enabled

    HW->>MAIN: Resume execution

    Note over MAIN: Normal execution resumes
```

---

## 7. Module Dependencies

### Dependency Diagram

```mermaid
graph TB
    subgraph "All System Modules"
        MAIN[Main Program<br/>Initialization]
        ALL_ISR[All ISR-based<br/>Modules]
    end

    subgraph "MCAL Drivers with Interrupts"
        ADC[ADC Driver<br/>ISR]
        UART[UART Driver<br/>ISR]
        TIMER[Timer1 Driver<br/>ISR]
        EEPROM[EEPROM Driver<br/>Critical Sections]
    end

    subgraph "GIE Driver"
        GIE[GIE Control<br/>SEI / CLI]
    end

    MAIN -->|Initialize & Enable| GIE

    ADC -->|Requires| GIE
    UART -->|Requires| GIE
    TIMER -->|Requires| GIE
    EEPROM -->|Atomic Operations| GIE

    ALL_ISR -.->|Implicit Dependency| GIE

    style GIE fill:#4A90E2,color:#fff
```

### Dependencies On Other Modules

**None** - GIE is a foundational module with no dependencies. It directly controls hardware (SREG) via CPU instructions.

### Modules That Depend On This Module

**All Interrupt-Based Modules:**

- ADC Driver (interrupt-driven sampling)
- UART Driver (RX interrupt)
- Timer1 Driver (compare match interrupt, optional)
- EEPROM Driver (atomic write operations)
- External Interrupts (if used)

**System Initialization:**

- Must call `mGIE_Enable()` after all peripheral setup
- Last step in initialization sequence
- Enables interrupt-driven operation

**Critical Section Protection:**

- Any code requiring atomic operations
- 16-bit register access (TCNT1, OCR1A, etc.)
- EEPROM write timing-critical sequence
- Shared variable access between main loop and ISR

---

## 8. Configuration Parameters

### Constants and Macros

| Macro          | Implementation                | Purpose                   |
| -------------- | ----------------------------- | ------------------------- |
| mGIE_Enable()  | sei() or asm volatile ("sei") | Enable global interrupts  |
| mGIE_Disable() | cli() or asm volatile ("cli") | Disable global interrupts |

### Usage Guidelines

**When to Enable:**

- After all peripheral initialization
- At end of system startup sequence
- After exiting critical section

**When to Disable:**

- Before critical sections (atomic operations)
- During 16-bit register access
- During timing-sensitive sequences
- Temporarily for quick operations (< 10 µs recommended)

**Duration of Disable:**

- Keep disabled time minimal
- Long disables cause interrupt latency
- Maximum recommended: < 100 µs
- ADC/UART may miss events if disabled too long

---

## 9. Error Handling Strategy

### Potential Issues

**Forgot to Re-enable:**

- Disabling interrupts without re-enabling
- System hangs if interrupt-driven
- Detection: Watchdog timeout, no ISR activity

**Nested Critical Sections:**

- Calling mGIE_Disable() twice
- Inner enable prematurely restores interrupts
- Detection: Logic error, testing required

**Long Critical Sections:**

- Holding interrupts disabled too long
- Missed interrupts, lost data
- Detection: Monitoring interrupt latency

### Solutions

**Save/Restore Pattern:**
Instead of blind enable/disable, save and restore previous state.

**Timeout Monitoring:**

- Watchdog detect if interrupts never enabled
- System reset as recovery

**Code Review:**

- Ensure every disable has matching enable
- Verify critical section duration minimal

---

## 10. Performance Characteristics

### Execution Time

| Operation          | Cycles | Time @ 16 MHz | Notes                      |
| ------------------ | ------ | ------------- | -------------------------- |
| SEI instruction    | 1      | 62.5 ns       | Enable interrupts          |
| CLI instruction    | 1      | 62.5 ns       | Disable interrupts         |
| ISR entry overhead | 4-5    | 250-312 ns    | Save PC, SREG, clear I-bit |
| RETI overhead      | 4-5    | 250-312 ns    | Restore SREG, PC           |

### Resource Usage

**Memory:**

- No RAM usage
- No global variables
- Program memory: ~10 bytes (inline macros)

**Hardware:**

- Uses SREG I-bit only
- No dedicated peripheral

---

## Implementation Notes

### Typical Usage Pattern

**System Initialization:**

1. Initialize all peripherals (ADC, UART, etc.)
2. Configure peripheral interrupt enables
3. Call mGIE_Enable() as **last step**
4. System now interrupt-driven

**Critical Section:**

1. Call mGIE_Disable()
2. Perform atomic operation (< 10 µs)
3. Call mGIE_Enable()
4. Interrupts restored

**Best Practice:**

- Keep critical sections short
- Document why interrupts disabled
- Use save/restore for nested contexts

---

**Document Version**: 2.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team  
**Related Documents**: All interrupt-based drivers
