# Protection Manager

**Purpose**: Fault detection and response

**Functions**:
- `PM_Init()`: Initialize thresholds
- `PM_Update()`: Check for faults
- `PM_SetOvercurrentThreshold(A)`: Configure threshold
- `PM_Reset()`: Clear fault, reconnect relay

**Protection Logic**:
- Overcurrent: I > threshold (3 consecutive readings)
- Overvoltage: V > threshold (3 consecutive readings)
- Action: Relay OFF, RGB RED, buzzer alert

See [State_Machines.md](../../../04_Detailed_Design/State_Machines/State_Machines.md)

**Document Version**: 1.0
