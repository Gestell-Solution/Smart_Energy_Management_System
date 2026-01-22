# Power Management

---

## Current Power Mode

**Active Mode Only**: No sleep/idle currently implemented

**Power Consumption**:

- HC-05 version: ~250mA
- ESP-01 version: ~370mA (peak during WiFi TX)

---

## Future Enhancements

**Idle Mode**: Sleep CPU between 100ms cycles (reduce ~30% power)

**Configuration**:

| Setting       | Value                   | Description                            |
| ------------- | ----------------------- | -------------------------------------- |
| Sleep Mode    | IDLE                    | CPU sleep, peripherals active          |
| Trigger Point | After main loop tasks   | Before waiting for next cycle          |
| Wake Sources  | Timer1, UART interrupts | Auto-wake on peripheral event          |
| Power Savings | ~30% reduction          | CPU clock stopped, peripherals running |

**Process**: Set sleep mode → Enter sleep → Wake on interrupt → Resume execution

---

**Document Version**: 1.0
