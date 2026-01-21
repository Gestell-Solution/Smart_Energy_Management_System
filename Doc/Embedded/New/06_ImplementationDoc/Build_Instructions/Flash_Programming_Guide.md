# Flash Programming Guide

---

## Hardware Connection

Connect USBasp programmer to 6-pin ISP header:
1. VCC (red)
2. MISO
3. SCK
4. MOSI
5. RESET
6. GND (black)

**Power**: Board must be powered separately (5V).

---

## Programming Command

```bash
avrdude -c usbasp -p m32 -U flash:w:firmware.hex
```

**Options**:
- `-c usbasp`: Programmer type
- `-p m32`: Target MCU (ATmega32)
- `-U flash:w:firmware.hex`: Write HEX to flash

---

## Fuse Programming

**Set fuses** (only once):
```bash
avrdude -c usbasp -p m32 -U lfuse:w:0xFF:m -U hfuse:w:0xC9:m
```

**Verify fuses**:
```bash
avrdude -c usbasp -p m32 -U lfuse:r:-:h -U hfuse:r:-:h
```

**Expected Output**:
- LFUSE: 0xFF
- HFUSE: 0xC9

---

## Troubleshooting

**Error: "Device not found"**  
→ Check connections, verify power

**Error: "Verification failed"**  
→ Check crystal, verify fuse bits

---

**Document Version**: 1.0  
**Last Updated**: January 2026
