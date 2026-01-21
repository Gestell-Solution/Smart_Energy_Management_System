# Timing and Scheduling

---

## Execution Model

**Type**: Bare-metal superloop (no RTOS)

**Main Loop**: 100ms cycle time (10 Hz)

**Structure**:
```
while(1) {
    if (100ms_elapsed) {
        ME_Update();
        PM_Update();
        DM_Update();
        EL_Task();
        CM_Task();
    }
}
```

---

## Interrupt-Driven Tasks

**Timer1 ISR (100 Hz)**: ADC sampling  
**UART RX ISR**: Command reception

---

## Task Update Rates

- Measurement: 100ms
- Display rotation: 2000ms  
- EEPROM save: 60000ms
- Communication: On demand

---

**Document Version**: 1.0
