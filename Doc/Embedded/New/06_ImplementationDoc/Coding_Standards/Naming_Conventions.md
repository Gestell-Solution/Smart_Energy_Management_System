# Naming Conventions

---

## Modules
**Format**: CamelCase  
**Example**: `MeasurementEngine`, `ProtectionManager`

---

## Functions
**Public**: `MODULE_FunctionName()`  
**Private**: `module_functionName()`  
**Example**: `ME_GetVoltageRMS()`, `me_calculateRMS()`

---

## Variables
**Local**: snake_case  
**Global**: `g_module_variableName`  
**Example**: `sample_index`, `g_adc_buffer`

---

## Constants
**Format**: UPPER_CASE  
**Example**: `MAX_VOLTAGE`, `ADC_CHANNEL_COUNT`

---

## Types
**Format**: TypeName_t  
**Example**: `MeasurementData_t`, `ProtectionStatus_t`

---

## Enums
**Format**: EnumName_t with PREFIX_VALUE  
**Example**:
```c
typedef enum {
    PROT_NORMAL,
    PROT_OVERCURRENT,
    PROT_OVERVOLTAGE
} ProtectionStatus_t;
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026
