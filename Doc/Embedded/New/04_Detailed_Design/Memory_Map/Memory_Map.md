# Memory Map - ATmega32

**Project**: Smart Energy Management System  
**Version**: 1.0

---

## 1. Flash Memory (Program Memory)

**Total**: 32 KB (0x0000 - 0x7FFF)

| Address Range | Size | Content |
|---------------|------|---------|
| 0x0000 - 0x0033 | 52 bytes | Interrupt Vector Table |
| 0x0034 - 0x5FFF | ~24 KB | Application Code |
| 0x6000 - 0x7FFF | ~8 KB | Reserved / Unused |

**Usage**: ~20-24 KB used (60-75%)

---

## 2. SRAM (Data Memory)

**Total**: 2 KB (0x0060 - 0x085F)

### 2.1 Register File
- 0x0000 - 0x001F: General Purpose Registers (R0-R31)
- 0x0020 - 0x005F: I/O Registers

### 2.2 Internal SRAM
- 0x0060 - 0x085F: 2048 bytes

**Allocation**:
| Section | Size | Usage |
|---------|------|-------|
| Stack | ~512 bytes | Function calls, local variables |
| Global Variables | ~200 bytes | Measurement data, flags |
| ADC Buffers | 256 bytes | Voltage/current samples (128×2) |
| UART Buffers | 64 bytes | TX/RX queues |
| Reserved | ~1 KB | Remaining for future use |

**Usage**: ~1 KB used (50%)

---

## 3. EEPROM (Non-Volatile)

**Total**: 1 KB (0x0000 - 0x03FF)

| Address | Size | Data | Description |
|---------|------|------|-------------|
| 0x0000 | 4 | Energy (kWh) | float32, cumulative |
| 0x0004 | 2 | Voltage Cal | uint16, ×1000 |
| 0x0006 | 2 | Current Cal | uint16, ×1000 |
| 0x0008 | 2 | Current Zero Offset | uint16, ADC counts |
| 0x000A | 2 | Power Factor | uint16, ×1000 (950=0.95) |
| 0x000C | 2 | Overcurrent Threshold | uint16, ×100 (2000=20A) |
| 0x000E | 2 | Overvoltage Threshold | uint16 (250V) |
| 0x0010 | 1 | Config Flags | bit0=buzzer_en |
| 0x0011 | 1 | Display Rotation | seconds (2) |
| 0x0012 | 1 | RGB Brightness | 0-255 |
| 0x0013 | 1 | Debounce Count | 3 |
| 0x0014 | 4 | UART Baud Rate | uint32 (9600) |
| 0x0018 | 1 | Protocol Select | 0=Mobile, 1=Dashboard |
| 0x0019 - 0x03FF | 998 | Reserved | Future use |

**Write Frequency**:
- Energy: Every 60 seconds
- Config: Only on user change

---

## 4. I/O Registers

### Key Peripheral Registers

**PORTA** (0x3B):
- PA0: ADC0 (Voltage sensor input)
- PA1: ADC1 (Current sensor input)

**PORTB** (0x38):
- PB0: Relay control output
- PB1: RGB Red (OC1A PWM)
- PB2: RGB Green (OC1B PWM)
- PB3: RGB Blue
- PB4: Buzzer output
- PB5-PB7: ISP (MOSI, MISO, SCK)

**PORTD** (0x32):
- PD0: UART RX
- PD1: UART TX
- PD2-PD7: LCD interface (RS, E, D4-D7)

**ADC Registers**:
- ADMUX (0x27): ADC mux and reference
- ADCSRA (0x26): ADC control and status
- ADCH/ADCL (0x24/0x25): ADC result

**Timer1 Registers**:
- TCCR1A/B (0x4F/0x4E): Timer1 control
- OCR1A/B (0x4A/0x48): Compare registers
- TCNT1 (0x4C): Counter value

**UART Registers**:
- UBRR (0x29/0x40): Baud rate
- UDR (0x2C): Data register
- UCSRA/B/C (0x2B/0x2A/0x40): Control/status

---

**Document Version**: 1.0  
**Last Updated**: January 2026
