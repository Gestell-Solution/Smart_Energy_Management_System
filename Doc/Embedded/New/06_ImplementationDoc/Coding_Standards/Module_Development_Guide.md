# Module Development Guide

---

## File Structure

Each module consists of 4 files:

1. **`Module_Interface.h`**: Public API declarations
2. **`Module_Config.h`**: Configuration parameters
3. **`Module_Private.h`**: Private definitions (types, macros)
4. **`Module.c`**: Implementation

---

## Naming Conventions

**Public Functions**: `MODULE_FunctionName()`  
Example: `DIO_SetPin()`, `ADC_Read()`

**Private Functions**: `module_functionName()`  
Example: `dio_configurePort()`, `adc_startConversion()`

**Global Variables**: `g_module_variableName`  
Example: `g_adc_result`, `g_uart_rxBuffer`

**Types**: `TypeName_t` (typedef with `_t` suffix)  
Example: `AdcChannel_t`, `UartConfig_t`

---

## Example Module Structure

```c
// Module_Interface.h
#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

void MODULE_Init(void);
uint8_t MODULE_GetValue(void);

#endif

// Module.c
#include "Module_Interface.h"
#include "Module_Private.h"

void MODULE_Init(void) {
    // Implementation
}
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026
