# Task 08: Fix Voltage Sensor ADC Divisor

## Task Information

- **Priority**: ⚠️ MEDIUM
- **Assigned to**: Ahmed Ashraf
- **Estimated Time**: 2 hours
- **Category**: HAL Layer - Voltage Sensor Driver
- **Blocker**: No - minor accuracy improvement

---

## Problem Description

### Current Issue

Using **1023** instead of **1024** as ADC divisor in voltage calculation.

### Impact

- ❌ **0.1% error in voltage readings** (~0.22V @ 220V)
- Minor but easily fixable accuracy improvement

### Root Cause

10-bit ADC has **1024 levels** (0-1023), not 1023.

---

## Technical Background

### 10-bit ADC

```
Resolution: 10 bits
Levels: 2^10 = 1024 (0 to 1023)
Step size: Vref / 1024

Correct formula: Voltage = (ADC_Value / 1024) × Vref
Wrong formula:   Voltage = (ADC_Value / 1023) × Vref
```

**Error calculation:**

```
At ADC_MAX (1023):
Wrong: 1023/1023 × 5V = 5.000V
Right: 1023/1024 × 5V = 4.995V

Error: ~0.1% (neglig ible but wrong)
```

---

## Implementation

**File**: `Hal/VoltageSensor/hVoltage_Program.c`

**Before:**

```c
float instant_voltage = Voltage_Value * (1.0f / 1023.0f) * Vref * Voltage_Divider_Ratio;
```

**After:**

```c
float instant_voltage = Voltage_Value * (1.0f / 1024.0f) * Vref * Voltage_Divider_Ratio;
```

**Add config constant:**

```c
#define ADC_MAX_VALUE    1024.0f  // 10-bit ADC levels
```

**Use in code:**

```c
float instant_voltage = Voltage_Value * (1.0f / ADC_MAX_VALUE) * Vref * Voltage_Divider_Ratio;
```

---

## Testing

```
Test: Compare before/after readings
Expected: ~0.1% difference (minimal)

Example @ 220V:
Before: 220.2V
After:  220.0V
```

---

## Deliverables

✅ Fixed voltage calculation  
✅ Test report  
✅ Git commit

---

**Document Version**: 1.0  
**Created**: 2026-01-16
