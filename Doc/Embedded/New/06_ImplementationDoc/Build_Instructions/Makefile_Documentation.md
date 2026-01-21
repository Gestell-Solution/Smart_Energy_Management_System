# Makefile Documentation

---

## Makefile Targets

| Target | Description |
|--------|-------------|
| `all` | Build firmware.hex (default) |
| `flash` | Upload firmware to MCU via USBasp |
| `clean` | Remove all build files |
| `size` | Display memory usage |
| `fuses` | Program fuse bits |

---

## Key Variables

```makefile
MCU = atmega32
F_CPU = 16000000UL
PROGRAMMER = usbasp
BAUDRATE = 9600
```

---

## Example Makefile Snippet

```makefile
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -Os -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU)

firmware.hex: firmware.elf
$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: firmware.hex
$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$<
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026
