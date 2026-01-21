# Build System Setup

---

## Linux/Ubuntu Installation

```bash
sudo apt-get update
sudo apt-get install avr-gcc avr-libc avrdude make
```

---

## Verify Installation

```bash
# Check compiler
avr-gcc --version

# Check programmer connection
avrdude -c usbasp -p m32
```

---

## Alternative: Windows

Download and install:
- **WinAVR** or **Atmel Studio**
- **USBasp drivers**

---

**Document Version**: 1.0  
**Last Updated**: January 2026
