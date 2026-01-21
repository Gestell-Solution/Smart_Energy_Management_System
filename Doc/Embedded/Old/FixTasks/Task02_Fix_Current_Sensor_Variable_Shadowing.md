# Task 02: Fix Current Sensor Variable Shadowing

## Task Information

- **Priority**: 🔴 CRITICAL
- **Assigned to**: Ahmed Twap
- **Estimated Time**: 10 hours
- **Category**: HAL Layer - ACS712 Current Sensor Driver
- **Blocker**: Yes - current readings completely wrong

---

## Problem Description

### Current Issue

The `hCurrent_Calibrate()` function has a **variable shadowing bug** where local variables inside if/else blocks hide the outer scope variable, preventing the calculated calibration offset from being saved.

### Impact

- ❌ **Zero offset = 0V instead of ~2.5V**
- ❌ **Current readings offset by ~25A**
- ❌ **Negative current values displayed**
- ❌ **Power calculations completely wrong**
- ❌ **Energy billing incorrect**

### Root Cause

```c
float VoltageConversion = 0;  // Outer scope

if (condition)
{
    float VoltageConversion = calculated_value;  // ❌ NEW local variable!
    // Dies at end of block
}

ACS712_ZERO_OFFSET = VoltageConversion;  // ❌ Uses outer variable (still 0!)
```

### Additional Issues in Same File

1. Wrong variable reset in `hCurrent_ReadRMS()`
2. Missing `sqrt()` safety checks

---

## Technical Background

### ACS712 Current Sensor

- **Type**: Hall-effect based current sensor
- **Model**: ACS712-20A (bidirectional)
- **Zero Current Output**: 2.5V (VCC/2)
- **Sensitivity**: 100 mV/A
- **Output Range**: 0.5V (-20A) to 4.5V (+20A)

### Calibration Process

The sensor outputs 2.5V when current = 0A. This offset must be measured during calibration and subtracted from all readings:

```
Measured Voltage = ADC reading → converted to volts
Current = (Measured Voltage - Zero Offset) / Sensitivity
```

If Zero Offset = 0 instead of 2.5:

```
Example: ADC reads 2.5V (actually 0A)
Wrong: Current = (2.5 - 0) / 0.1 = 25A ❌
Right: Current = (2.5 - 2.5) / 0.1 = 0A ✅
```

---

## Files to Modify

### File 1: `Hal/ACS712CurntSnsr/hCurrent_Program.c`

- Lines 134-150: `hCurrent_Calibrate()` function
- Lines 170-195: `hCurrent_ReadRMS()` function
- Additional safety checks for `sqrt()` calls

---

## Implementation Steps

### Step 1: Analyze Current Bug (2 hours)

**Actions:**

1. Open `Hal/ACS712CurntSnsr/hCurrent_Program.c`
2. Review lines 134-150 in detail
3. Trace variable scope using debugger or code analysis
4. Add debug print to verify current `ACS712_ZERO_OFFSET` value
5. Test current readings with NO load connected

**Debug Code to Add Temporarily:**

```c
void hCurrent_Calibrate(void)
{
    float VoltageConversion = 0;

    // ... (buggy code) ...

    ACS712_ZERO_OFFSET = VoltageConversion;

    // ✅ Add debug output
    #ifdef DEBUG_MODE
        char buffer[50];
        sprintf(buffer, "Zero Offset: %.3fV\r\n", ACS712_ZERO_OFFSET);
        UART_SendString(buffer);
    #endif
}
```

**Expected Finding:**

- Zero offset prints as `0.000V` instead of `~2.500V`

---

### Step 2: Fix Variable Shadowing in Calibrate Function (3 hours)

**File**: `Hal/ACS712CurntSnsr/hCurrent_Program.c`

**Function**: `hCurrent_Calibrate()`

**Before (BUGGY CODE):**

```c
void hCurrent_Calibrate(void)
{
    float VoltageConversion = 0;  // ← Outer scope variable

    if (Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num)
    {
        // ❌ Declares NEW local variable (shadows outer)
        float VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value / ADC_MAX) * Vref;
        // This local variable is destroyed at end of if-block
    }
    else
    {
        // ❌ Another NEW local variable (shadows outer)
        float VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value / ADC_MAX) * Vref;
        // This local variable is also destroyed at end of else-block
    }

    // ❌ Outer variable is still 0 - never assigned!
    ACS712_ZERO_OFFSET = VoltageConversion;
}
```

**After (FIXED CODE):**

```c
/**
 * @brief Calibrate ACS712 zero offset
 * @details Measures the sensor output voltage when current = 0A
 *          This offset (typically 2.5V) is subtracted from all readings
 *          Must be called with NO load connected
 * @note Should be called at system startup before measurements begin
 */
void hCurrent_Calibrate(void)
{
    float VoltageConversion;  // ✅ Declare without initialization

    if (Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num)
    {
        // ✅ ASSIGN to outer variable (no 'float' keyword)
        VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value / ADC_MAX) * Vref;
    }
    else
    {
        // ✅ ASSIGN to outer variable (no 'float' keyword)
        VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value / ADC_MAX) * Vref;
    }

    // ✅ Now uses the correctly calculated value
    ACS712_ZERO_OFFSET = VoltageConversion;

    #ifdef DEBUG_MODE
        // Debug output to verify calibration
        char debug_buffer[60];
        sprintf(debug_buffer, "[CAL] Zero Offset: %.3fV\r\n", ACS712_ZERO_OFFSET);
        UART_SendString(debug_buffer);
    #endif
}
```

**Key Changes:**

1. ✅ Removed `= 0` initialization from outer variable
2. ✅ Removed `float` keyword from assignments inside if/else
3. ✅ Added comprehensive Doxygen comments
4. ✅ Added optional debug output

---

### Step 3: Fix Wrong Variable Reset in ReadRMS (2 hours)

**File**: `Hal/ACS712CurntSnsr/hCurrent_Program.c`

**Function**: `hCurrent_ReadRMS()`

**Before (BUGGY CODE):**

```c
float hCurrent_ReadRMS(void)
{
    float meanSquare;
    float Current_RMS;

    // Calculate mean square
    meanSquare = Current_RMS_Calibrated_Actions.Current_ADC_Sum / RMS_Nominal_Samples_Num;
    Current_RMS = sqrt(meanSquare);

    // Apply calibration and convert to Amperes
    // ... conversion code ...

    // Reset for next cycle
    Current_RMS_Calibrated_Actions.Current_ADC_Sum = 0;  // ✅ Correct
    Current_RMS_Calibrated_Actions.Current_ADC_Avrg_Value = 0;  // ❌ WRONG!
    // Should reset Previous_ADC_Avrg_Value instead

    return Current_RMS;
}
```

**After (FIXED CODE):**

```c
/**
 * @brief Read calibrated RMS current value
 * @return RMS current in Amperes
 * @details Calculates RMS from accumulated squared samples,
 *          applies zero offset calibration, and converts to Amperes
 */
float hCurrent_ReadRMS(void)
{
    float meanSquare;
    float Current_RMS;
    float Current_in_Amperes;

    // Calculate mean square from accumulated samples
    meanSquare = Current_RMS_Calibrated_Actions.Current_ADC_Sum / RMS_Nominal_Samples_Num;

    // ✅ Safety: Prevent sqrt of negative (due to floating-point errors)
    if (meanSquare < 0.0f)
    {
        meanSquare = 0.0f;
    }

    // Calculate RMS
    Current_RMS = sqrt(meanSquare);

    // Convert to voltage
    float Voltage_RMS = (Current_RMS / ADC_MAX) * Vref;

    // Apply zero offset calibration and convert to Amperes
    // Sensitivity = 0.1V/A (100mV per Ampere)
    Current_in_Amperes = (Voltage_RMS - ACS712_ZERO_OFFSET) / ACS712_SENSITIVITY;

    // ✅ Clamp negative values to zero (no backward current in AC)
    if (Current_in_Amperes < 0.0f)
    {
        Current_in_Amperes = 0.0f;
    }

    // ✅ Reset BOTH accumulators correctly
    Current_RMS_Calibrated_Actions.Current_ADC_Sum = 0;
    Current_RMS_Calibrated_Actions.Previous_ADC_Avrg_Value = 0;  // ✅ CORRECT!

    return Current_in_Amperes;
}
```

---

### Step 4: Add sqrt() Safety Checks (1 hour)

Add safety checks before ALL `sqrt()` calls in the file:

```c
// Example pattern to apply:
float value_to_sqrt = /* some calculation */;

// ✅ Safety check
if (value_to_sqrt < 0.0f)
{
    value_to_sqrt = 0.0f;
}

float result = sqrt(value_to_sqrt);
```

**Functions to check:**

- `hCurrent_ReadRMS()` - ✅ Already added above
- `hCurrent_Calibrate()` - Check if any sqrt() calls exist
- Any other calculation functions

---

### Step 5: Testing and Verification (2 hours)

**Test Setup:**

1. Disconnect ALL loads from current sensor
2. Connect sensor to ATmega32 ADC input
3. Connect UART for debug output
4. Have known resistive loads ready (lightbulb, heater)

**Test 5.1: Zero Offset Calibration**

```
Procedure:
1. Flash updated firmware
2. Ensure NO load connected (current = 0A)
3. Call hCurrent_Calibrate()
4. Read debug output or check variable via debugger

Expected Result:
✅ ACS712_ZERO_OFFSET ≈ 2.5V (range: 2.4V - 2.6V acceptable)
❌ If reads 0.000V → fix didn't work

Tolerance: ±0.1V is normal (depends on sensor and VCC stability)
```

**Test 5.2: Zero Current Reading**

```
Procedure:
1. After calibration, with NO load
2. Call hCurrent_ReadRMS() 10 times
3. Record all readings

Expected Result:
✅ All readings ≈ 0.0A (range: -0.2A to +0.2A acceptable)
❌ If reads ~25A → calibration not applied correctly
```

**Test 5.3: Known Load Test**

```
Test Load 1: 60W incandescent bulb
Voltage: 220V
Expected Current: 60W / 220V = 0.27A

Procedure:
1. Connect 60W bulb
2. Measure with reference clamp meter: record actual current
3. Read from system: hCurrent_ReadRMS()
4. Compare

Expected Result:
✅ Reading within ±10% of reference meter
   (e.g., reference = 0.27A, system reads 0.24A - 0.30A)

Test Load 2: 1000W heater
Expected Current: 1000W / 220V = 4.55A

Expected Result:
✅ Reading ≈ 4.5A ± 0.5A

Test Load 3: 2000W heater
Expected Current: 2000W / 220V = 9.09A

Expected Result:
✅ Reading ≈ 9.0A ± 0.9A
```

**Test 5.4: Linearity Test**

```
Test multiple loads and plot:
X-axis: Reference meter reading
Y-axis: System reading

Expected Result:
✅ Linear relationship (R² > 0.95)
✅ Slope ≈ 1.0 (system reads same as reference)
✅ Intercept ≈ 0.0 (no offset error)
```

**Test 5.5: Negative Value Test**

```
Procedure:
1. Introduce artificial negative values (modify code temporarily)
   OR disconnect sensor (ADC reads 0V)
2. Check that Current_in_Amperes is clamped to 0.0

Expected Result:
✅ Never displays negative current
```

---

## Success Criteria

✅ **Zero offset calibrates to 2.5V ± 0.1V**  
✅ **Zero current reads as 0.0A ± 0.2A**  
✅ **Current readings accurate within ±10% vs reference meter**  
✅ **No negative current readings displayed**  
✅ **sqrt() safety checks prevent crashes**  
✅ **Code properly documented**  
✅ **Passes all 5 test procedures**

---

## Deliverables

1. ✅ Fixed `hCurrent_Program.c` with corrected variable scoping
2. ✅ Test report with calibration values
3. ✅ Accuracy measurements vs reference meter (Excel/CSV table)
4. ✅ Linearity plot (optional but recommended)
5. ✅ Updated calibration procedure document
6. ✅ Git commit with clear message

---

## Dependencies

**Blocked By**:

- Task 01 (Timer1 fix) - Needs correct sampling rate

**Blocks**:

- Task 03 (Power Calculation) - Needs accurate current readings
- Task 04 (Protection Manager) - Needs accurate current for overcurrent protection

---

## Configuration Constants

Verify these are defined correctly in `hCurrent_Config.h`:

```c
// ACS712-20A sensor constants
#define ACS712_SENSITIVITY    0.100f  // 100mV/A for 20A model
#define ACS712_ZERO_CURRENT_VOLTAGE  2.5f  // Nominal (before calibration)

// ADC configuration
#define ADC_MAX               1024.0f  // 10-bit ADC
#define Vref                  5.0f     // Reference voltage

// RMS sampling
#define RMS_Nominal_Samples_Num  200   // Number of samples per RMS calculation
```

---

## Notes and Considerations

### Why Variable Shadowing Happens

This is a common C programming mistake. The compiler allows it (only gives a warning if `-Wshadow` is enabled).

**Prevention:**

- Enable `-Wshadow` compiler warning
- Use different variable names for local scope
- Or use the pattern shown in the fix: declare once, assign in blocks

### ACS712 Accuracy Limitations

- ±1.5% error at 25°C (datasheet spec)
- Additional error from ADC quantization
- Temperature drift: ±4.5mV/°C
- Total expected error: ±5-10% is realistic

### Calibration Best Practices

- Calibrate at operating temperature
- Recalibrate if VCC changes
- Store offset in EEPROM for persistence across resets

---

## Related Documentation

- `Doc/Embedded/Issue_Solutions.md` - Section "HAL Layer Issues #5, #6"
- `Doc/Embedded/Embedded_Fix_Implementation_Plan.md` - Phase 1, Task 1.2
- ACS712 Datasheet (Allegro Microsystems)
- ATmega32 Datasheet - ADC Section

---

**Document Version**: 1.0  
**Created**: 2026-01-16  
**Last Updated**: 2026-01-16
