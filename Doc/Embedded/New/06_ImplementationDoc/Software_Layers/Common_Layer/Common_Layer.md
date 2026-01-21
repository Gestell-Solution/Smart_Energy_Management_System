# Common Layer

**Purpose**: Shared utilities and data structures

---

## Responsibilities

- Common data types (typedefs, enums, structs)
- Shared macros and constants
- Project configuration (`ProjectCfg.h`)
- Utility functions

---

## Key Files

**`Config.h`**: System-wide configuration  
**`Macros.h`**: Utility macros (BIT manipulation, MIN/MAX, etc.)  
**`ProjectCfg.h`**: F_CPU and project-level defines

---

## Common Data Structures

```c
// Measurement data structure
typedef struct {
    float voltage_rms;
    float current_rms;
    float power_watts;
    float energy_kwh;
    uint32_t timestamp;
} MeasurementData_t;

// Protection status enumeration
typedef enum {
    PROT_NORMAL,
    PROT_OVERCURRENT,
    PROT_OVERVOLTAGE,
    PROT_SENSOR_FAULT
} ProtectionStatus_t;

// Energy log entry
typedef struct {
    uint32_t timestamp;
    float voltage;
    float current;
    float power;
    float energy_kwh;
} EnergyLog_t;
```

---

## Utility Macros

```c
// Bit manipulation
#define SET_BIT(REG, BIT)    ((REG) |= (1 << (BIT)))
#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(1 << (BIT)))
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))
#define READ_BIT(REG, BIT)   (((REG) >> (BIT)) & 1)

// Math utilities
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(x)    (((x) < 0) ? -(x) : (x))
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
