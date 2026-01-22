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

| File                 | Purpose              | Contains                                                           |
| -------------------- | -------------------- | ------------------------------------------------------------------ |
| `Module_Interface.h` | Public API           | Header guard, function declarations (Init, GetValue), public types |
| `Module_Private.h`   | Internal definitions | Private types, internal macros, static function declarations       |
| `Module.c`           | Implementation       | Includes both headers, function implementations                    |

**Typical Module Components**:

- **Init function**: Initializes module state and hardware
- **Get/Set functions**: Public interface for data access
- **Private functions**: Internal helper functions (static)
- **Header guards**: Prevent multiple inclusion

---

**Document Version**: 1.0  
**Last Updated**: January 2026
