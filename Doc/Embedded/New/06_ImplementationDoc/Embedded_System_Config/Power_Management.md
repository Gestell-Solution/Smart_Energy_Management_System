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

**Implementation**:
```c
// After main loop tasks
set_sleep_mode(SLEEP_MODE_IDLE);
sleep_mode();
// Wake on Timer1 or UART interrupt
```

---

**Document Version**: 1.0
