# Memory Management

--- 

## Flash Memory (Program)
**Total**: 32 KB  
**Usage**: 60-75% (~20-24 KB)  
**Remaining**: ~8-12 KB for future features

## SRAM (Data)
**Total**: 2 KB  
**Usage**: ~50% (~1 KB)
- Stack: ~512 bytes
- Global variables: ~200 bytes
- ADC buffers: ~256 bytes  
- UART buffers: ~64 bytes

## EEPROM (Non-Volatile)
**Total**: 1 KB  
**Usage**: ~20 bytes (configuration + energy counter)  
**Endurance**: 100,000 write cycles

**Memory Map**: See [Memory_Map.md](../../04_Detailed_Design/Memory_Map/Memory_Map.md)

---

**Allocation Strategy**: Static allocation only (no heap/malloc)

**Document Version**: 1.0
