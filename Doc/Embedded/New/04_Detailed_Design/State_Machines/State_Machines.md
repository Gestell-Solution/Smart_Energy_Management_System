# State Machines

**Project**: Smart Energy Management System  
**Version**: 1.0

---

## 1. Protection Manager FSM

### 1.1 Overcurrent Protection State Machine

```mermaid
stateDiagram-v2
    [*] --> NORMAL
    NORMAL --> DEBOUNCING: Current > Threshold
    DEB OUNCING --> FAULT: Count >= 3
    DEBOUNCING --> NORMAL: Current < Threshold
    FAULT --> NORMAL: Manual Reset
    FAULT --> FAULT: Fault Persists
```

**States**:

- **NORMAL**: Current within safe limits, relay ON (if enabled)
- **DEBOUNCING**: Current exceeded, counting consecutive samples
- **FAULT**: Protection triggered, relay OFF, alert active

**Transitions**:
| From | To | Condition | Action |
|------|-----|-----------|--------|
| NORMAL | DEBOUNCING | I > I_threshold | debounce_count = 1 |
| DEBOUNCING | FAULT | debounce_count >= 3 | Trigger protection |
| DEBOUNCING | NORMAL | I < I_threshold | debounce_count = 0 |
| FAULT | NORMAL | Reset command | Clear fault flags |

**Actions on FAULT Entry**:

1. Relay OFF
2. Display "OVERLOAD!"
3. RGB LED RED
4. Buzzer 3 beeps
5. Send alert via UART

---

### 1.2 Overvoltage Protection State Machine

```mermaid
stateDiagram-v2
    [*] --> NORMAL
    NORMAL --> DEBOUNCING: Voltage > Threshold
    DEBOUNCING --> FAULT: Count >= 3
    DEBOUNCING --> NORMAL: Voltage < Threshold
    FAULT --> NORMAL: Manual Reset
```

**States**: Same as overcurrent

**Transitions**: Similar logic, V instead of I

**Actions on FAULT Entry**:

1. Relay OFF
2. Display "OVERVOLT!"
3. RGB LED RED
4. Buzzer 5 beeps
5. Send alert via UART

---

## 2. Communication Manager FSM

### 2.1 UART Receive State Machine

```mermaid
stateDiagram-v2
    [*] --> IDLE
    IDLE --> LENGTH: Byte == 0xAA (Header)
    LENGTH --> COMMAND: Length received
    COMMAND --> DATA: Command received (if len > 0)
    COMMAND --> PROCESS: Command received (if len == 0)
    DATA --> DATA: bytes < expected_length
    DATA --> PROCESS: All data received
    PROCESS --> IDLE: Command processed
```

**States**:

- **IDLE**: Waiting for frame header (0xAA)
- **LENGTH**: Reading length byte
- **COMMAND**: Reading command ID
- **DATA**: Reading data payload
- **PROCESS**: Processing complete frame

**Variables**:

- `rx_state`: Current state
- `expected_length`: Number of data bytes
- `rx_index`: Current data byte index
- `command_id`: Received command

---

## 3. Display Manager FSM

### 3.1 Screen Rotation State Machine

```mermaid
stateDiagram-v2
    [*] --> SCREEN_V_I
    SCREEN_V_I --> SCREEN_P_E: 2 seconds elapsed
    SCREEN_P_E --> SCREEN_STATUS: 2 seconds elapsed
    SCREEN_STATUS --> SCREEN_V_I: 2 seconds elapsed
```

**States**:

- **SCREEN_V_I**: Display Voltage & Current
- **SCREEN_P_E**: Display Power & Energy
- **SCREEN_STATUS**: Display Status & Relay state

**Transition**: Every 2 seconds (configurable)

---

## 4. Energy Logger FSM

### 4.1 EEPROM Save State Machine

```mermaid
stateDiagram-v2
    [*] --> IDLE
    IDLE --> PENDING: Save condition met
    PENDING --> WRITING: Ready to write
    WRITING --> VERIFY: Write complete
    VERIFY --> IDLE: Verification OK
    VERIFY --> RETRY: Verification failed
    RETRY --> WRITING: Retry count < 3
    RETRY --> ERROR: Retry count >= 3
    ERROR --> IDLE: Error logged
```

**Save Conditions**:

- Timer elapsed (60 seconds)
- Energy change > threshold (0.1 kWh)
- Manual save command

**States**:

- **IDLE**: No pending save
- **PENDING**: Save requested, waiting for safe time
- **WRITING**: Writing to EEPROM
- **VERIFY**: Reading back to verify
- **RETRY**: Retrying failed write
- **ERROR**: Write failed after retries

---

## 5. Calibration Manager FSM

### 5.1 Calibration State Machine

```mermaid
stateDiagram-v2
    [*] --> NORMAL_OP
    NORMAL_OP --> CAL_VOLTAGE: Voltage cal command
    NORMAL_OP --> CAL_CURRENT: Current cal command
    NORMAL_OP --> CAL_ZERO: Zero offset cal command
    CAL_VOLTAGE --> SAVE_EEPROM: Calibration complete
    CAL_CURRENT --> SAVE_EEPROM: Calibration complete
    CAL_ZERO --> SAVE_EEPROM: Calibration complete
    SAVE_EEPROM --> NORMAL_OP: Save complete
```

**States**:

- **NORMAL_OP**: Normal operation, calibration ready
- **CAL_VOLTAGE**: Calibrating voltage sensor
- **CAL_CURRENT**: Calibrating current sensor
- **CAL_ZERO**: Calibrating current zero offset
- **SAVE_EEPROM**: Saving calibration to EEPROM

---

## 6. System Controller State Diagram (Overall)

```mermaid
stateDiagram-v2
    [*] --> INIT
    INIT --> SELF_TEST: Hardware init complete
    SELF_TEST --> NORMAL: Self-test passed
    SELF_TEST --> ERROR: Self-test failed
    NORMAL --> PROTECTION: Fault detected
    NORMAL --> CALIBRATION: Cal command received
    PROTECTION --> NORMAL: Reset command
    CALIBRATION --> NORMAL: Cal complete
    ERROR --> INIT: Hard reset
```

**System States**:

- **INIT**: Power-up initialization
- **SELF_TEST**: ADC, EEPROM, UART checks
- **NORMAL**: Normal operation (measurement, protection)
- **PROTECTION**: Protection triggered (relay OFF)
- **CALIBRATION**: Calibration in progress
- **ERROR**: Critical error (requires reset)

---

**Document Version**: 1.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
