# Task 03: Add Power Factor to Power Calculation

## Task Information

- **Priority**: 🔴 CRITICAL
- **Assigned to**: Ahmed Ashraf
- **Estimated Time**: 12 hours  
  **Category**: Application Layer - Measurement Engine
- **Blocker**: Yes - energy billing completely wrong

---

## Problem Description

### Current Issue

The system calculates **Apparent Power (S = V × I)** instead of **Real Power (P = S × cos φ)**, causing significant overestimation of energy consumption.

### Impact

- ❌ **15-40% error in power readings** (depends on load type)
- ❌ **Energy billing incorrect** - users pay more than actual consumption
- ❌ **Inaccurate energy monitoring**
- ❌ **Non-compliance with power metering standards**

### Root Cause

```c
// Current (WRONG) calculation:
ME_Power = ME_Vrms * ME_Irms;  // ❌ This is Apparent Power!

// AC physics:
// Real Power P = V × I × cos(φ)
// where φ = phase angle between voltage and current
```

---

## Technical Background

### AC Power Theory

**For DC Circuits:**

```
P = V × I  ✅ Simple
```

**For AC Circuits:**

```
Apparent Power:  S = Vrms × Irms  (Unit: VA - Volt-Ampere)
Real Power:      P = Vrms × Irms × cos(φ)  (Unit: W - Watt)
Reactive Power:  Q = Vrms × Irms × sin(φ)  (Unit: VAR)

Power Factor (PF) = cos(φ) = P / S
```

### Why It Matters

**Example: 1000W Motor Load**

```
Apparent Power S = 220V × 5.2A = 1144 VA
Power Factor = 0.85 (typical for motors)
Real Power P = 1144 VA × 0.85 = 972 W

Current Code Error:
System reads: 1144W  ❌
Actual: 972W  ✅
Error: +18% overestimated!

Over 1 month (720 hours):
Wrong: 1144W × 720h = 823 kWh
Right: 972W × 720h = 700 kWh
User overpays: 123 kWh!
```

### Typical Load Power Factors

| Load Type         | Power Factor | Example                   |
| ----------------- | ------------ | ------------------------- |
| Resistive         | 1.0          | Heater, incandescent bulb |
| Inductive (motor) | 0.7 - 0.8    | Drill, fan, refrigerator  |
| Mixed residential | 0.85 - 0.9   | Typical home              |
| LED lighting      | 0.5 - 0.7    | LED bulbs without PFC     |
| Computer PSU      | 0.6 - 0.95   | Depends on PSU quality    |

---

## Files to Modify

### File 1: `App/MeasurementEngine/MeasurementEngine_Config.h`

- Add power factor configuration

### File 2: `App/MeasurementEngine/MeasurementEngine_Program.c`

- Modify `ME_Update()` function
- Add power calculation functions
- Add getter/setter for power factor

### File 3: `App/MeasurementEngine/MeasurementEngine_Interface.h`

- Add new function prototypes

---

## Implementation Steps

### Step 1: Research and Document (2 hours)

**Actions:**

1. Review AC power theory
2. Study power factor measurement methods
3. Research typical residential power factors
4. Decide on implementation approach (fixed PF vs measured)
5. Document decision rationale

**Two Implementation Approaches:**

**Approach A: Fixed/Configurable Power Factor (SIMPLE)**

- Use typical PF value (0.85)
- Allow configuration via command
- Pros: Easy to implement, no hardware changes
- Cons: Less accurate for varying loads

**Approach B: Measure Actual Power Factor (ADVANCED)**

- Calculate P = average(v(t) × i(t)) from samples
- Derive actual PF from measurements
- Pros: More accurate, adapts to load
- Cons: Requires access to sample buffers, more complex

**Recommendation**: Start with Approach A, with option to upgrade to B later.

---

### Step 2: Add Configuration Defines (2 hours)

**File**: `App/MeasurementEngine/MeasurementEngine_Config.h`

**Add:**

```c
/******************************************************************************
 * Power Factor Configuration
 ******************************************************************************/

/**
 * @brief Enable power factor correction in power calculation
 * @details When enabled, real power P = S × PF is calculated
 *          When disabled, apparent power S = V × I is used
 * @note Set to 1 to enable, 0 to disable
 */
#define ME_POWER_FACTOR_ENABLED      1

/**
 * @brief Default power factor value
 * @details Typical residential power factor between 0.8 and 0.9
 *          Can be adjusted based on:
 *          - 0.85: General residential (recommended default)
 *          - 0.70: Heavy inductive loads (motors, compressors)
 *          - 0.95: Modern appliances with PFC
 *          - 1.00: Pure resistive loads (testing only)
 * @range 0.5 to 1.0
 */
#define ME_DEFAULT_POWER_FACTOR      0.85f

/**
 * @brief Allow runtime power factor adjustment
 * @details If enabled, PF can be changed via ME_SetPowerFactor()
 *          Useful for calibration or different load profiles
 */
#define ME_ALLOW_PF_RUNTIME_ADJUST   1

/******************************************************************************
 * Energy Integration Configuration
 ******************************************************************************/

/**
 * @brief Sample interval in seconds
 * @details Time between ME_Update() calls
 *          Used for energy integration: E = P × Δt
 * @note With 100Hz Timer1: interval = 0.01s (10ms)
 */
#define ME_SAMPLE_INTERVAL_S         0.01f

/**
 * @brief Energy conversion factor
 * @details Converts Watt-seconds to kilowatt-hours
 *          1 kWh = 3,600,000 Watt-seconds
 */
#define WS_TO_KWH                    (1.0f / 3600000.0f)

/**
 * @brief Power Factor valid range
 */
#define ME_PF_MIN                    0.5f
#define ME_PF_MAX                    1.0f
```

---

### Step 3: Update Measurement Engine Variables (1 hour)

**File**: `App/MeasurementEngine/MeasurementEngine_Program.c`

**Add to global variables section:**

```c
/******************************************************************************
 * Private Variables
 ******************************************************************************/

// Sensor readings
static float ME_Vrms = 0.0f;
static float ME_Irms = 0.0f;

// Power calculations
static float ME_ApparentPower = 0.0f;  // ✅ NEW: S = V × I (VA)
static float ME_RealPower = 0.0f;      // ✅ NEW: P = S × PF (W)
static float ME_PowerFactor = ME_DEFAULT_POWER_FACTOR;  // ✅ NEW

// Energy accumulation
static float ME_Energy_kWh = 0.0f;     // ✅ Energy in kWh
```

---

### Step 4: Implement Power Calculation - Simple Method (3 hours)

**File**: `App/MeasurementEngine/MeasurementEngine_Program.c`

**Function**: `ME_Update()`

**Before (BUGGY CODE):**

```c
void ME_Update(void)
{
    ME_Vrms = hVoltage_ReadRMS();
    ME_Irms = hCurrent_ReadRMS();

    ME_Power = ME_Vrms * ME_Irms;  // ❌ Apparent power!

    ME_Energy += ME_Power * ME_SAMPLE_INTERVAL;  // ❌ Wrong energy
}
```

**After (FIXED CODE - Simple Method):**

```c
/**
 * @brief Update power and energy measurements
 * @details Called every 10ms by Timer1 ISR callback
 *          - Reads RMS voltage and current
 *          - Calculates apparent power S = V × I
 *          - Calculates real power P = S × Power Factor
 *          - Integrates energy in kWh
 * @note This function should be called at consistent intervals
 *       defined by ME_SAMPLE_INTERVAL_S
 */
void ME_Update(void)
{
    // Read RMS values from sensors
    ME_Vrms = hVoltage_ReadRMS();
    ME_Irms = hCurrent_ReadRMS();

    // Calculate Apparent Power (VA)
    // S = Vrms × Irms
    ME_ApparentPower = ME_Vrms * ME_Irms;

    #if ME_POWER_FACTOR_ENABLED
        // ✅ Calculate Real Power with Power Factor correction
        // P = S × cos(φ) = S × PF
        ME_RealPower = ME_ApparentPower * ME_PowerFactor;
    #else
        // No PF correction - use apparent power
        // (Only accurate for resistive loads)
        ME_RealPower = ME_ApparentPower;
    #endif

    // Calculate energy increment in Watt-seconds
    // ΔE = P × Δt
    float energy_ws = ME_RealPower * ME_SAMPLE_INTERVAL_S;

    // Convert to kilowatt-hours and accumulate
    // 1 kWh = 3,600,000 Ws
    float energy_kwh_increment = energy_ws * WS_TO_KWH;
    ME_Energy_kWh += energy_kwh_increment;
}
```

---

### Step 5: Add Getter/Setter Functions (2 hours)

**File**: `App/MeasurementEngine/MeasurementEngine_Program.c`

**Add functions:**

```c
/**
 * @brief Get calculated real power
 * @return Real power in Watts
 */
float ME_GetPower(void)
{
    return ME_RealPower;  // ✅ Return real power, not apparent
}

/**
 * @brief Get apparent power
 * @return Apparent power in VA
 * @note Useful for diagnostics and PF calculation
 */
float ME_GetApparentPower(void)
{
    return ME_ApparentPower;
}

/**
 * @brief Get accumulated energy
 * @return Energy in kilowatt-hours
 */
float ME_GetEnergy_kWh(void)
{
    return ME_Energy_kWh;
}

/**
 * @brief Get current power factor setting
 * @return Power factor (0.5 to 1.0)
 */
float ME_GetPowerFactor(void)
{
    return ME_PowerFactor;
}

#if ME_ALLOW_PF_RUNTIME_ADJUST
/**
 * @brief Set power factor for calculations
 * @param pf Power factor value
 * @return 1 if successful, 0 if out of valid range
 * @note Valid range: 0.5 to 1.0
 */
uint8_t ME_SetPowerFactor(float pf)
{
    // Validate range
    if (pf >= ME_PF_MIN && pf <= ME_PF_MAX)
    {
        ME_PowerFactor = pf;
        return 1;  // Success
    }
    return 0;  // Out of range
}
#endif

/**
 * @brief Reset energy counter
 * @note Should be called when user resets billing cycle
 */
void ME_ResetEnergy(void)
{
    ME_Energy_kWh = 0.0f;
}
```

**File**: `App/MeasurementEngine/MeasurementEngine_Interface.h`

**Add prototypes:**

```c
// Power and energy getters
float ME_GetPower(void);
float ME_GetApparentPower(void);
float ME_GetEnergy_kWh(void);
float ME_GetPowerFactor(void);

// Power factor configuration
#if ME_ALLOW_PF_RUNTIME_ADJUST
uint8_t ME_SetPowerFactor(float pf);
#endif

// Energy reset
void ME_ResetEnergy(void);
```

---

### Step 6: Testing and Validation (2 hours)

**Test Setup:**

1. Calibrated reference power meter (e.g., Kill-A-Watt)
2. Various known loads
3. UART for debug output

**Test 6.1: Resistive Load (PF = 1.0)**

```
Load: 100W incandescent bulb or heater
Expected PF: 1.0

Test Procedure:
1. Set ME_PowerFactor = 1.0
2. Connect 100W load
3. Measure with reference meter: Record P_ref
4. Read system: ME_GetPower()
5. Compare

Expected Result:
✅ System reading ≈ Reference meter ± 10%
✅ If PF = 1.0, Real Power = Apparent Power

Example:
Reference: 100W
System (PF=1.0): ~100W ✅
System (PF=0.85): ~85W ❌ (would be wrong for resistive)
```

**Test 6.2: Inductive Load (PF = 0.7-0.8)**

```
Load: Electric drill or fan motor
Expected PF: 0.7 - 0.8

Test Procedure:
1. Measure with ref meter: P_real, V, I
2. Calculate PF_actual = P_real / (V × I)
3. Set system PF to calculated value
4. Compare readings

Example:
Reference meter:
- Voltage = 220V
- Current = 2.0A
- Real Power = 308W
- PF_actual = 308 / (220×2) = 0.70

System test:
- ME_Vrms = 220V ✅
- ME_Irms = 2.0A ✅
- ME_ApparentPower = 440VA ✅
- ME_SetPowerFactor(0.70)
- ME_RealPower = 440 × 0.70 = 308W ✅

Expected Result:
✅ With correct PF, matches reference meter
```

**Test 6.3: Mixed Load**

```
Load: Typical household mix (lights + appliances)
Expected PF: 0.85 - 0.9

Test Procedure:
1. Use default PF = 0.85
2. Compare with reference meter
3. Adjust PF until readings match

This calibrates PF for this specific installation
```

**Test 6.4: Energy Accumulation**

```
Test: Long-term accuracy

Procedure:
1. Reset energy: ME_ResetEnergy()
2. Connect known 1000W load (verified with ref meter)
3. Run for exactly 1 hour
4. Read both meters

Expected Result:
Reference meter: 1.00 kWh
System: ~1.00 kWh ± 0.05 kWh (±5%)

If error > 10%:
- Check Timer1 frequency (Task 01)
- Check power calculation
- Check energy integration formula
```

---

## Success Criteria

✅ **Power readings match reference meter within ±10%** (with correct PF setting)  
✅ **Energy accumulation accurate over 1-hour test** (±5%)  
✅ **Power Factor configurable and applied correctly**  
✅ **Works with different load types** (resistive, inductive, mixed)  
✅ **Code properly documented**  
✅ **Backward compatible** (can disable PF correction if needed)

---

## Deliverables

1. ✅ Updated `MeasurementEngine_Program.c` with PF calculation
2. ✅ Updated `MeasurementEngine_Config.h` with PF settings
3. ✅ Updated `MeasurementEngine_Interface.h` with new functions
4. ✅ Power measurement test report (comparison with reference meter)
5. ✅ Energy accuracy report (1-hour accumulation test)
6. ✅ Recommended PF values documentation
7. ✅ Git commit with clear message

---

## Dependencies

**Blocked By**:

- Task 01 (Timer1 fix) - Needs correct sampling rate for energy integration
- Task 02 (Current sensor fix) - Needs accurate current readings

**Blocks**:

- Communication protocols - Need to send corrected power/energy values
- Mobile/Dashboard integration

---

## Advanced Implementation (Optional Future Enhancement)

### True Power Factor Measurement

If sample buffers are accessible, implement real PF measurement:

```c
/**
 * @brief Calculate real power from instantaneous samples
 * @details True real power: P = average(v(t) × i(t))
 *          This accounts for phase shift automatically
 */
static float ME_CalculateRealPower(float* v_samples, float* i_samples, uint16_t count)
{
    float sum = 0.0f;

    // Sum instantaneous power for each sample
    for (uint16_t i = 0; i < count; i++)
    {
        sum += (v_samples[i] * i_samples[i]);
    }

    // Average = real power
    return sum / count;
}

void ME_Update_Advanced(void)
{
    // Read RMS for display
    ME_Vrms = hVoltage_ReadRMS();
    ME_Irms = hCurrent_ReadRMS();

    // Get sample buffers
    float* v_buf = hVoltage_GetSampleBuffer();
    float* i_buf = hCurrent_GetSampleBuffer();

    // ✅ Calculate REAL power from samples
    ME_RealPower = ME_CalculateRealPower(v_buf, i_buf, RMS_Nominal_Samples_Num);

    // Calculate apparent power
    ME_ApparentPower = ME_Vrms * ME_Irms;

    // Derive actual power factor
    if (ME_ApparentPower > 0.1f)
    {
        ME_PowerFactor = ME_RealPower / ME_ApparentPower;

        // Clamp to valid range (handle noise)
        if (ME_PowerFactor > 1.0f) ME_PowerFactor = 1.0f;
        if (ME_PowerFactor < 0.0f) ME_PowerFactor = 0.0f;
    }

    // Integrate energy
    float energy_ws = ME_RealPower * ME_SAMPLE_INTERVAL_S;
    ME_Energy_kWh += energy_ws * WS_TO_KWH;
}
```

**Pros**: Most accurate, adapts to any load  
**Cons**: Requires sensor drivers to expose sample arrays

---

## Notes and Considerations

### Why Not Just Use Current × Voltage?

Because AC voltage and current waveforms can be out of phase. The product V×I gives **apparent power**, not **real power**. Only real power does actual work and should be billed.

### Choosing Power Factor Value

- **0.85**: Safe default for mixed residential
- **0.9-0.95**: Modern appliances with power factor correction
- **0.7-0.8**: Industrial, heavy motors
- **1.0**: Lab testing with resistive loads ONLY

### Storage in EEPROM

Consider storing calibrated PF value in EEPROM so it persists across resets.

---

## Related Documentation

- `Doc/Embedded/Issue_Solutions.md` - Section "Application Layer Issues #9"
- `Doc/Embedded/Embedded_Fix_Implementation_Plan.md` - Phase 1, Task 1.3
- IEEE 1459-2010: Standard for Power Definitions

---

**Document Version**: 1.0  
**Created**: 2026-01-16  
**Last Updated**: 2026-01-16
