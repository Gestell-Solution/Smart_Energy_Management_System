# Interrupt Management

---

## Enabled Interrupts

1. **Timer1 COMPA**: ADC sampling trigger (100 Hz)
2. **UART RX**: Command reception (asynchronous)

---

## ISR Rules

- Keep **short** (<100 µs execution)
- **No blocking** operations  
- Set flags only, process in main loop
- **No floating-point** in ISR (use integer math)

---

## Priority

Hardware fixed priority (lowest vector number = highest priority):
1. RESET
2. INT0/INT1  
3. Timer1 COMPA ← **Used (ADC sampling)**
4. ...
5. UART RX ← **Used (commands)**

---

**Document Version**: 1.0
