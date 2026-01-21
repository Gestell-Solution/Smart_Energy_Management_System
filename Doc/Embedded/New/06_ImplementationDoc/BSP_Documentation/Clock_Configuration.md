# Clock Configuration

**Project**: Smart Energy Management System  
**Version**: 1.0

---

## Crystal Oscillator Configuration

**Type**: External crystal oscillator  
**Frequency**: 16 MHz  
**Load Capacitors**: 22pF each (to ground)

**Connections**:
- Crystal between XTAL1 (Pin 13) and XTAL2 (Pin 12)
- 22pF capacitor from XTAL1 to GND
- 22pF capacitor from XTAL2 to GND

---

## CPU Clock

**F_CPU**: 16,000,000 Hz (16 MHz)  
**Prescaler**: None (CLKDIV8 fuse unprogrammed)

---

## Peripheral Clocks

All peripherals derive from CPU clock:

| Peripheral | Clock Source | Prescaler | Effective Frequency |
|------------|--------------|-----------|---------------------|
| **CPU** | Crystal | 1:1 | 16 MHz |
| **ADC** | CPU | /128 | 125 kHz |
| **Timer0** | CPU | Configurable | Variable |
| **Timer1** | CPU | Configurable | Variable |
| **UART** | CPU | Baud rate divider | - |

---

## Fuse Configuration for Clock

**CKSEL[3:0]** = 1111: External crystal oscillator, >8 MHz  
**SUT[1:0]** = 10: Slowly rising power  
**CKOPT** = 0: Enable full swing for high frequencies

**Result**: Stable 16 MHz operation with optimal startup time.

---

**Document Version**: 1.0  
**Last Updated**: January 2026
