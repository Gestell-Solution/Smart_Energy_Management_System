# Embedded System Fix Implementation Plan

## Document Information
**Date:** December 15, 2025  
**Project:** Smart Energy Management System  
**Platform:** ATmega32 Microcontroller  
**Lead Engineer:** Eng. Hesham Ahmed  
**Company:** Gestell  
**Total Estimated Time:** 3-4 weeks (with testing)

---

## Table of Contents

1. [Sprint Planning Overview](#sprint-planning-overview)
2. [Phase 1: Critical Fixes (Week 1)](#phase-1-critical-fixes-week-1)
3. [Phase 2: High Priority (Week 2)](#phase-2-high-priority-week-2)
4. [Phase 3: Medium Priority (Week 3)](#phase-3-medium-priority-week-3)
5. [Phase 4: Testing & Validation (Week 4)](#phase-4-testing--validation-week-4)
6. [Detailed Implementation Steps](#detailed-implementation-steps)
7. [Testing Strategy](#testing-strategy)
8. [Risk Management](#risk-management)

---

## Sprint Planning Overview

### Team Assignments

**Development Team:**
1. **Mohamed Diaa** - Timer1, Communication Protocol
2. **Ahmed Ashraf** - Power Calculation, EEPROM fixes
3. **Mohamed Abdelgaber** - Protection Manager, Debouncing
4. **Basma Khaled** - Data Persistence, Testing
5. **Ahmed Twap** - Current Sensor fixes, Validation

**Reviewers:**
- **Hesham Ahmed** - Code review, technical decisions
- **Alaa Emad** - Testing coordination, integration

### Effort Estimation

| Priority | Tasks | Estimated Hours | Developers |
|----------|-------|----------------|------------|
| Critical | 5 tasks | 60 hours | 3 developers |
| High | 4 tasks | 40 hours | 2 developers |
| Medium | 4 tasks | 30 hours | 2 developers |
| Testing | Integration | 40 hours | All team |
| **TOTAL** | **13 tasks** | **170 hours** | **~4 weeks** |

---

## Phase 1: Critical Fixes (Week 1)

### Sprint Goal
Fix all critical bugs that prevent accurate measurements and cause system instability.

### Task 1.1: Fix Timer1 Duplicate ISR ⏱️ 8 hours

**Assigned to:** Mohamed Diaa  
**Priority:** 🔴 CRITICAL  
**Blocker:** Yes - affects all measurements

#### Implementation Steps

**Step 1: Understand Current Behavior (1 hour)**
```
1. Review TIMER1_Program.c lines 93-115
2. Document current ISR behavior
3. Trace callback invocations
4. Measure actual sampling rate with oscilloscope
```

**Step 2: Modify Timer1 configuration (2 hours)**
```
File: Mcal/Timer1/TIMER1_Config.h

Changes:
1. Keep ICR1 = 1250 (TOP value for CTC mode)
2. Keep OCR1A = 1250 (Compare Match A)
3. Document: OCR1B not used in this application
```

**Step 3: Update Timer1 Init Function (2 hours)**
```
File: Mcal/Timer1/TIMER1_Program.c

Function: mTIMER1_Init()

Changes:
1. Enable ONLY OCIE1A interrupt
2. Comment out or remove OCIE1B enable
3. Add clear comment explaining single interrupt usage

Code modification:
    // Enable Timer1 Compare Match A Interrupt
    SetBit(TIMSK_Reg, OCIE1A_Bit);  // ✅ Keep this
    
    // DO NOT enable Compare Match B (not used)
    // ClearBit(TIMSK_Reg, OCIE1B_Bit);  // ✅ Explicitly disable
```

**Step 4: Keep Only __vector_7 ISR (2 hours)**
```
File: Mcal/Timer1/TIMER1_Program.c

Changes:
1. Keep __vector_7() ISR (Compare Match A)
2. Remove or comment out __vector_8() ISR (Compare Match B)
3. Update Doxygen comments

Before (lines 93-115):
    void __vector_7(void) __attribute__((signal));
    void __vector_7()
    {
        if(Timer1_Global_Callback != Null)
        {
            Timer1_Global_Callback();
        }
    }
    
    void __vector_8(void) __attribute__((signal));  // ❌ REMOVE THIS
    void __vector_8()                               // ❌ REMOVE THIS
    {                                               // ❌ REMOVE THIS
        if(Timer1_Global_Callback != Null)          // ❌ REMOVE THIS
        {                                           // ❌ REMOVE THIS
            Timer1_Global_Callback();               // ❌ REMOVE THIS
        }                                           // ❌ REMOVE THIS
    }                                               // ❌ REMOVE THIS

After:
    /**
     * @brief Timer1 Compare Match A ISR
     * @details Called every 10ms (100Hz) to trigger ADC sampling
     *          Only this ISR is used; Compare Match B is disabled
     */
    void __vector_7(void) __attribute__((signal));
    void __vector_7()
    {
        if(Timer1_Global_Callback != Null)
        {
            Timer1_Global_Callback();
        }
    }
    
    // __vector_8 (Compare Match B) is not used in this application
```

**Step 5: Verification (1 hour)**
```
1. Compile and verify no compilation errors
2. Flash to ATmega32
3. Attach oscilloscope to test GPIO (toggle in callback)
4. Verify frequency = 100 Hz exactly
5. Verify energy accumulation rate (should be half of before)
```

**Deliverables:**
- ✅ Modified TIMER1_Program.c
- ✅ Updated Doxygen comments
- ✅ Test report showing 100Hz operation

**Success Criteria:**
- Callback invoked exactly 100 times per second
- Energy calculation rate corrected

---

### Task 1.2: Fix Current Sensor Variable Shadowing ⏱️ 10 hours

**Assigned to:** Ahmed Twap  
**Priority:** 🔴 CRITICAL  
**Blocker:** Yes - current readings completely wrong

#### Implementation Steps

**Step 1: Analyze Current Bug (2 hours)**
```
1. Review hCurrent_Program.c lines 134-150
2. Trace variable scope using debugger
3. Document ACS712_ZERO_OFFSET current value (should be 0)
4. Test current readings with known loads
```

**Step 2: Fix hCurrent_Calibrate() Function (3 hours)**
```
File: Hal/ACS712CurntSnsr/hCurrent_Program.c

Function: hCurrent_Calibrate()

Before (lines 134-150):
    void hCurrent_Calibrate(void)
    {
        float VoltageConversion = 0;  // ← Outer scope
      
        if (Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num)
        {
            float VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value / ADC_MAX) * Vref;
            // ❌ Local variable shadows outer
        }
        else
        {
            float VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value / ADC_MAX) * Vref;
            // ❌ Another local variable shadows outer
        }
      
        ACS712_ZERO_OFFSET = VoltageConversion;  // ❌ Uses outer (still 0)
    }

After:
    /**
     * @brief Calibrate ACS712 zero offset
     * @details Calculates zero-current voltage offset (typically 2.5V)
     */
    void hCurrent_Calibrate(void)
    {
        float VoltageConversion;  // ✅ Declare without initialization
      
        if (Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num)
        {
            // ✅ ASSIGN, don't declare
            VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value / ADC_MAX) * Vref;
        }
        else
        {
            // ✅ ASSIGN, don't declare
            VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value / ADC_MAX) * Vref;
        }
      
        // ✅ Now uses correct calculated value
        ACS712_ZERO_OFFSET = VoltageConversion;
    }
```

**Step 3: Fix hCurrent_ReadRMS() Wrong Reset (2 hours)**
```
File: Hal/ACS712CurntSnsr/hCurrent_Program.c

Function: hCurrent_ReadRMS()

Before (line 146):
    float hCurrent_ReadRMS(void)
    {
        // ... RMS calculation ...
        
        Current_RMS_Calibrated_Actions.Current_ADC_Sum = 0;  // ✅ Correct
        Current_RMS_Calibrated_Actions.Current_ADC_Avrg_Value = 0;  // ❌ WRONG!
        
        return Current_RMS;
    }

After:
    /**
     * @brief Read calibrated RMS current
     * @return RMS current in Amperes
     */
    float hCurrent_ReadRMS(void)
    {
        float meanSquare;
        float Current_RMS;
        
        // Calculate RMS
        meanSquare = Current_RMS_Calibrated_Actions.Current_ADC_Sum / RMS_Nominal_Samples_Num;
        
        // ✅ Safety: prevent sqrt of negative
        if (meanSquare < 0.0f)
        {
            meanSquare = 0.0f;
        }
        
        Current_RMS = sqrt(meanSquare);
        
        // Apply calibration and convert to Amperes
        // ... existing conversion code ...
        
        // ✅ Reset BOTH accumulator and Previous average
        Current_RMS_Calibrated_Actions.Current_ADC_Sum = 0;
        Current_RMS_Calibrated_Actions.Previous_ADC_Avrg_Value = 0;  // ✅ CORRECT
        
        return Current_RMS;
    }
```

**Step 4: Add sqrt() Safety Check (1 hour)**
```
Function: hCurrent_ReadRMS() and hCurrent_Calibrate()

Add before every sqrt() call:
    if (meanSquare < 0.0f)
    {
        meanSquare = 0.0f;
    }
    Current_RMS = sqrt(meanSquare);
```

**Step 5: Testing (2 hours)**
```
Test Procedure:
1. Connect sensor with NO load (0A)
2. Run calibration
3. Verify ACS712_ZERO_OFFSET ≈ 2.5V (not 0!)
4. Test with known loads:
   - 1A load → verify ≈ 1A reading
   - 5A load → verify ≈ 5A reading
   - 10A load → verify ≈ 10A reading
5. Document accuracy (should be ±5%)
```

**Deliverables:**
- ✅ Fixed hCurrent_Program.c
- ✅ Test report with accuracy measurements
- ✅ Updated calibration procedure document

**Success Criteria:**
- Zero offset = 2.5V ± 0.1V
- Current readings accurate within 5%
- No negative current readings

---

### Task 1.3: Fix Power Calculation (Add Power Factor) ⏱️ 12 hours

**Assigned to:** Ahmed Ashraf  
**Priority:** 🔴 CRITICAL  
**Blocker:** Yes - energy billing completely wrong

#### Implementation Steps

**Step 1: Research and Document Power Factor (2 hours)**
```
1. Study AC power theory (Real vs Apparent power)
2. Document common residential power factors
3. Research measurement methods
4. Choose implementation approach
```

**Step 2: Define Configuration (2 hours)**
```
File: App/MeasurementEngine/MeasurementEngine_Config.h

Add new defines:
    /**
     * Power Factor Configuration
     * 
     * For accurate power measurement in AC circuits:
     * Real Power (P) = Apparent Power (S) × Power Factor
     * 
     * Typical residential PF: 0.85
     * Can be adjusted based on load type
     */
    #define ME_POWER_FACTOR_ENABLED      1
    #define ME_DEFAULT_POWER_FACTOR      0.85f
    
    /**
     * Sample interval in seconds
     * Used for energy integration: E = P × t
     */
    #define ME_SAMPLE_INTERVAL_S         0.01f  // 10ms = 100Hz
    
    /**
     * Energy unit conversion
     * From Watt-seconds to kilowatt-hours
     */
    #define WS_TO_KWH                    (1.0f / 3600000.0f)
```

**Step 3: Update Measurement Engine Variables (1 hour)**
```
File: App/MeasurementEngine/MeasurementEngine_Program.c

Add global variables:
    // Power measurements
    static float ME_Vrms = 0.0f;
    static float ME_Irms = 0.0f;
    static float ME_ApparentPower = 0.0f;  // ✅ New: S = V × I
    static float ME_RealPower = 0.0f;      // ✅ New: P = S × PF
    static float ME_PowerFactor = ME_DEFAULT_POWER_FACTOR;
    
    // Energy accumulation
    static float ME_Energy_kWh = 0.0f;
```

**Step 4: Implement Power Calculation - Simple Method (3 hours)**
```
File: App/MeasurementEngine/MeasurementEngine_Program.c

Function: ME_Update()

Before:
    void ME_Update(void)
    {
        ME_Vrms = hVoltage_ReadRMS();
        ME_Irms = hCurrent_ReadRMS();
        
        ME_Power = ME_Vrms * ME_Irms;  // ❌ Apparent power!
        
        ME_Energy += ME_Power * ME_SAMPLE_INTERVAL;
    }

After (Simple method with fixed PF):
    /**
     * @brief Update power and energy measurements
     * @details Called every 10ms by Timer1 interrupt
     *          Calculates Real Power using Power Factor
     *          Integrates energy in kWh
     */
    void ME_Update(void)
    {
        // Read RMS values
        ME_Vrms = hVoltage_ReadRMS();
        ME_Irms = hCurrent_ReadRMS();
        
        // Calculate Apparent Power (VA)
        ME_ApparentPower = ME_Vrms * ME_Irms;
        
        #if ME_POWER_FACTOR_ENABLED
            // ✅ Calculate Real Power (W) with Power Factor
            ME_RealPower = ME_ApparentPower * ME_PowerFactor;
        #else
            // Use apparent power (for resistive loads only)
            ME_RealPower = ME_ApparentPower;
        #endif
        
        // Calculate energy increment in Watt-seconds
        float energy_ws = ME_RealPower * ME_SAMPLE_INTERVAL_S;
        
        // Convert to kWh and accumulate
        float energy_kwh_increment = energy_ws * WS_TO_KWH;
        ME_Energy_kWh += energy_kwh_increment;
    }
    
    /**
     * @brief Get calculated real power
     * @return Real power in Watts
     */
    float ME_GetPower(void)
    {
        return ME_RealPower;  // ✅ Return real power, not apparent
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
     * @brief Set power factor for calculations
     * @param pf Power factor (0.5 to 1.0)
     * @return 1 if successful, 0 if out of range
     */
    uint8_t ME_SetPowerFactor(float pf)
    {
        if (pf >= 0.5f && pf <= 1.0f)
        {
            ME_PowerFactor = pf;
            return 1;
        }
        return 0;
    }
```

**Step 5: Alternative - Measure Real Power from Samples (Optional, 4 hours)**
```
File: App/MeasurementEngine/MeasurementEngine_Program.c

Add new function for true power measurement:

    /**
     * @brief Calculate real power from instantaneous samples
     * @details P = average(v(t) × i(t))
     *          This is the TRUE real power, accounting for phase shift
     * @param v_samples Array of voltage samples
     * @param i_samples Array of current samples
     * @param count Number of samples
     * @return Real power in Watts
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
    
    /**
     * @brief Enhanced ME_Update with true power measurement
     */
    void ME_Update_Advanced(void)
    {
        // Read RMS values for display
        ME_Vrms = hVoltage_ReadRMS();
        ME_Irms = hCurrent_ReadRMS();
        
        // Get instantaneous sample buffers from sensors
        float* voltage_samples = hVoltage_GetSampleBuffer();
        float* current_samples = hCurrent_GetSampleBuffer();
        uint16_t num_samples = RMS_Nominal_Samples_Num;
        
        // ✅ Calculate REAL power from samples
        ME_RealPower = ME_CalculateRealPower(voltage_samples, current_samples, num_samples);
        
        // Calculate apparent power for comparison
        ME_ApparentPower = ME_Vrms * ME_Irms;
        
        // Calculate and store actual power factor
        if (ME_ApparentPower > 0.1f)  // Avoid division by zero
        {
            ME_PowerFactor = ME_RealPower / ME_ApparentPower;
            
            // Clamp to valid range
            if (ME_PowerFactor > 1.0f) ME_PowerFactor = 1.0f;
            if (ME_PowerFactor < 0.0f) ME_PowerFactor = 0.0f;
        }
        
        // Integrate energy
        float energy_ws = ME_RealPower * ME_SAMPLE_INTERVAL_S;
        ME_Energy_kWh += energy_ws * WS_TO_KWH;
    }

Note: This advanced method requires sensor drivers to expose sample buffers.
```

**Step 6: Testing (2 hours)**
```
Test Setup:
1. Use calibrated power meter as reference
2. Test with different loads:
   - Resistive (heater): Expected PF = 1.0
   - Inductive (drill): Expected PF = 0.7-0.8
   - Mixed residential: Expected PF = 0.85

Test Procedure:
1. Connect known 100W resistive load
   - Measure with reference meter: 100W
   - Measure with system (PF=1.0): Should read ~100W
   - Measure with system (PF=0.85): Should read ~85W
   - Adjust PF until readings match

2. Run for 1 hour, compare energy:
   - Reference meter: X kWh
   - System: Should match within 5%

Documentation:
- Record all measurements
- Calculate accuracy percentage
- Document recommended PF for different scenarios
```

**Deliverables:**
- ✅ Updated MeasurementEngine_Program.c
- ✅ Updated MeasurementEngine_Config.h
- ✅ Power measurement test report
- ✅ Energy accuracy report

**Success Criteria:**
- Power readings match reference meter within 10%
- Energy accumulation accurate over 1-hour test
- Power Factor configurable and applied correctly

---

### Task 1.4: Fix Protection Manager Debouncing ⏱️ 10 hours

**Assigned to:** Mohamed Abdelgaber  
**Priority:** 🔴 CRITICAL  
**Blocker:** Yes - causes frequent false trips

#### Implementation Steps

**Step 1: Design Debouncing Algorithm (2 hours)**
```
1. Research typical inrush current durations
2. Define debounce counters and thresholds
3. Design state machine
4. Document hysteresis values
```

**Step 2: Update Protection Manager Config (1 hour)**
```
File: App/ProtectionManager/ProtectionManager_Config.h

Add debouncing configuration:
    /**
     * Protection Debouncing Configuration
     * 
     * TRIP_DEBOUNCE_COUNT: Number of consecutive over-threshold readings
     *                      before triggering protection
     * RESET_DEBOUNCE_COUNT: Number of consecutive normal readings
     *                       before resetting protection
     * 
     * @ 100Hz sampling:
     * - 5 counts = 50ms
     * - 10 counts = 100ms
     * - 20 counts = 200ms
     */
    #define PM_TRIP_DEBOUNCE_COUNT       10    // 100ms sustained fault
    #define PM_RESET_DEBOUNCE_COUNT      20    // 200ms normal to reset
    
    /**
     * Hysteresis values
     * Gap between trip and reset thresholds to prevent oscillation
     */
    #define PM_VOLTAGE_HYSTERESIS        10.0f  // 10V
    #define PM_CURRENT_HYSTERESIS        2.0f   // 2A
    #define PM_POWER_HYSTERESIS          200.0f // 200W
```

**Step 3: Add Debounce State Variables (1 hour)**
```
File: App/ProtectionManager/ProtectionManager_Program.c

Add static variables:
    // Debounce counters
    static uint8_t PM_Overvoltage_Counter = 0;
    static uint8_t PM_Overcurrent_Counter = 0;
    static uint8_t PM_Overpower_Counter = 0;
    
    // Trip flags
    static uint8_t PM_Voltage_Tripped = 0;
    static uint8_t PM_Current_Tripped = 0;
    static uint8_t PM_Power_Tripped = 0;
```

**Step 4: Implement Debounced Protection Logic (4 hours)**
```
File: App/ProtectionManager/ProtectionManager_Program.c

Function: PM_Update()

Before (lines 41-56):
    void PM_Update()
    {
        ME_Update();
      
        if (ME_GetVoltageRMS() > Vrms_Threshold || 
            ME_GetCurrentRMS() > Irms_Threshold || 
            ME_GetPower() > P_Threshold)
        {
            Protection_State = Danger;
            
            for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
            {
                hRelay_Off(Relay_id);
            }
            
            hRGB_SetState(RGB_RED);
            Buzzer_On();
        }
    }

After:
    /**
     * @brief Update protection monitoring with debouncing
     * @details Monitors V, I, P with sustained fault detection
     *          Includes hysteresis to prevent oscillation
     *          Auto-resets after sustained normal operation
     */
    void PM_Update(void)
    {
        // Update measurements
        ME_Update();
        
        // Read current values
        float voltage = ME_GetVoltageRMS();
        float current = ME_GetCurrentRMS();
        float power = ME_GetPower();
        
        uint8_t fault_detected = 0;
        
        // ========== OVERVOLTAGE PROTECTION ==========
        if (voltage > Vrms_Threshold)
        {
            // Voltage above threshold
            PM_Overvoltage_Counter++;
            
            if (PM_Overvoltage_Counter >= PM_TRIP_DEBOUNCE_COUNT)
            {
                // Sustained overvoltage
                PM_Voltage_Tripped = 1;
                PM_Overvoltage_Counter = PM_TRIP_DEBOUNCE_COUNT;  // Cap
            }
        }
        else if (voltage < (Vrms_Threshold - PM_VOLTAGE_HYSTERESIS))
        {
            // Voltage well below threshold (hysteresis)
            PM_Overvoltage_Counter = 0;
            PM_Voltage_Tripped = 0;
        }
        else
        {
            // In hysteresis zone - maintain current state
            if (PM_Overvoltage_Counter > 0)
            {
                PM_Overvoltage_Counter--;
            }
        }
        
        // ========== OVERCURRENT PROTECTION ==========
        if (current > Irms_Threshold)
        {
            // Current above threshold
            PM_Overcurrent_Counter++;
            
            if (PM_Overcurrent_Counter >= PM_TRIP_DEBOUNCE_COUNT)
            {
                // Sustained overcurrent
                PM_Current_Tripped = 1;
                PM_Overcurrent_Counter = PM_TRIP_DEBOUNCE_COUNT;  // Cap
            }
        }
        else if (current < (Irms_Threshold - PM_CURRENT_HYSTERESIS))
        {
            // Current well below threshold
            PM_Overcurrent_Counter = 0;
            PM_Current_Tripped = 0;
        }
        else
        {
            // In hysteresis zone
            if (PM_Overcurrent_Counter > 0)
            {
                PM_Overcurrent_Counter--;
            }
        }
        
        // ========== OVERPOWER PROTECTION ==========
        if (power > P_Threshold)
        {
            PM_Overpower_Counter++;
            
            if (PM_Overpower_Counter >= PM_TRIP_DEBOUNCE_COUNT)
            {
                PM_Power_Tripped = 1;
                PM_Overpower_Counter = PM_TRIP_DEBOUNCE_COUNT;
            }
        }
        else if (power < (P_Threshold - PM_POWER_HYSTERESIS))
        {
            PM_Overpower_Counter = 0;
            PM_Power_Tripped = 0;
        }
        else
        {
            if (PM_Overpower_Counter > 0)
            {
                PM_Overpower_Counter--;
            }
        }
        
        // ========== TRIP LOGIC ==========
        // Fault if ANY protection tripped
        fault_detected = PM_Voltage_Tripped || PM_Current_Tripped || PM_Power_Tripped;
        
        if (fault_detected && (Protection_State != PM_STATE_DANGER))
        {
            // Transition to danger state
            Protection_State = PM_STATE_DANGER;
            
            // Cut all relays
            for (uint8_t relay_id = hRELAY_0; relay_id <= hRELAY_3; relay_id++)
            {
                hRelay_Off(relay_id);
            }
            
            // Activate alarms
            hRGB_SetState(RGB_RED);
            hBuzzer_On();
            
            // Log trip reason (optional)
            PM_LogTripReason();
        }
        else if (!fault_detected && (Protection_State == PM_STATE_DANGER))
        {
            // All faults cleared, auto-reset
            // Only if ALL counters at zero for sustained period
            if (PM_Overvoltage_Counter == 0 && 
                PM_Overcurrent_Counter == 0 && 
                PM_Overpower_Counter == 0)
            {
                Protection_State = PM_STATE_SAFE;
                hRGB_SetState(RGB_GREEN);
                hBuzzer_Off();
            }
        }
    }
    
    /**
     * @brief Log the reason for protection trip
     * @details Can be displayed on LCD or sent via Bluetooth
     */
    static void PM_LogTripReason(void)
    {
        // Determine which protection triggered
        if (PM_Voltage_Tripped)
        {
            // Store: "OVERVOLT"
        }
        if (PM_Current_Tripped)
        {
            // Store: "OVERCURRENT"
        }
        if (PM_Power_Tripped)
        {
            // Store: "OVERPOWER"
        }
    }
```

**Step 5: Testing (2 hours)**
```
Test 1: Motor Inrush Current
Setup:
- Connect electric drill or motor
- Set current threshold to 10A
- Motor startup current: ~30A for 1 second

Expected Behavior:
1. t=0ms: Motor OFF, I=1A, Counter=0
2. t=10ms: Switch ON, I=30A, Counter=1
3. t=20ms: Inrush, I=28A, Counter=2
4. t=100ms: Inrush, I=25A, Counter=10 (would trip if instant)
5. t=150ms: Settled, I=8A, Counter decreases
6. t=200ms: Normal, I=7A, Counter=0
7. Result: NO FALSE TRIP ✅

Test 2: Real Overload
Setup:
- Connect 15A sustained load
- Threshold: 10A

Expected Behavior:
1. t=0ms: I=15A, Counter=1
2. t=10ms: I=15A, Counter=2
...
10. t=100ms: I=15A, Counter=10 → TRIP ✅
11. Relays cut, buzzer ON, LED RED

Documentation:
- Record waveforms with oscilloscope
- Document counter values over time
- Verify no false trips on 10 motor startup cycles
```

**Deliverables:**
- ✅ Updated ProtectionManager_Program.c
- ✅ Updated ProtectionManager_Config.h
- ✅ Protection testing report
- ✅ Inrush current test video/oscilloscope capture

**Success Criteria:**
- No false trips during motor startup
- Real faults detected within 100ms
- Hysteresis prevents oscillation
- Auto-reset works reliably

---

### Task 1.5: Implement Communication Protocol Formats ⏱️ 16 hours

**Assigned to:** Mohamed Diaa  
**Priority:** 🔴 CRITICAL  
**Blocker:** Yes - Mobile and Dashboard can't get data

**Implementation Steps:** See Issue_Solutions.md for detailed pseudocode

**Summary:**
1. Define protocol constants (2h)
2. Implement data conversion helpers (3h)
3. Implement Mobile response format (4h)
4. Implement Dashboard response format (4h)
5. Update command handler (2h)
6. Testing (3h)

**Deliverables:**
- ✅ Updated CommManager_Program.c
- ✅ Updated CommManager_Config.h
- ✅ Protocol testing report
- ✅ Compatible with Mobile App
- ✅ Compatible with Dashboard

---

## Phase 2: High Priority (Week 2)

### Task 2.1: Add EEPROM Timeout Protection ⏱️ 6 hours

**Assigned to:** Ahmed Ashraf  
**Priority:** ⚠️ HIGH

**Steps:**
1. Modify mEEPROM_WriteByte() to add timeout (2h)
2. Modify mEEPROM_ReadByte() to add timeout (1h)
3. Add error code return values (1h)
4. Update all callers to check errors (1h)
5. Testing (1h)

---

### Task 2.2: Implement System Data Persistence ⏱️ 12 hours

**Assigned to:** Basma Khaled  
**Priority:** ⚠️ HIGH

**Steps:**
1. Design EEPROM memory map (2h)
2. Implement SystemDataManager module (4h)
3. Integrate with main application (2h)
4. Add periodic save mechanism (2h)
5. Testing (2h)

**Features to persist:**
- Energy counter
- Calibration factors
- Protection thresholds
- Runtime hours

---

### Task 2.3: Implement Energy Reset Command ⏱️ 6 hours

**Assigned to:** Ahmed Ashraf  
**Priority:** ⚠️ HIGH

**Steps:**
1. Add CMD_RESET_ENERGY to protocol (1h)
2. Implement HandleEnergyReset() (2h)
3. Integrate with CommunicationManager (1h)
4. Add to Mobile/Dashboard commands (1h)
5. Testing (1h)

---

### Task 2.4: Fix Voltage Sensor ADC Divisor ⏱️ 4 hours

**Assigned to:** Ahmed Twap  
**Priority:** ⚠️ HIGH

**Steps:**
1. Locate voltage calculation code (0.5h)
2. Change 1023.0 to 1024.0 (0.5h)
3. Test accuracy with known voltages (2h)
4. Document improvement (1h)

**Impact:** Improves accuracy from ±0.22V to ±0.05V @ 220V

---

## Phase 3: Medium Priority (Week 3)

### Task 3.1: Implement WiFi Module Support (ESP-01) ⏱️ 16 hours

**Assigned to:** Mohamed Diaa + Ahmed Twap  
**Priority:** ⚠️ MEDIUM  
**Note:** Required for Dashboard Remote Mode

**Steps:**
1. ESP-01 hardware integration (2h)
2. AT command interface implementation (4h)
3. WiFi connection management (3h)
4. TCP server communication (4h)
5. Integration with Communication Manager (2h)
6. Testing (2h)

---

### Task 3.2: Implement Calibration Command Handler ⏱️ 8 hours

**Assigned to:** Mohamed Abdelgaber  
**Priority:** ⚠️ MEDIUM

**Steps:**
1. Add calibration command handler (2h)
2. Implement factor adjustment (2h)
3. Save/load calibration to EEPROM (2h)
4. Testing (2h)

---

### Task 3.3: Add Push Button Debouncing ⏱️ 4 hours

**Assigned to:** Basma Khaled  
**Priority:** ⚠️ MEDIUM

**Steps:**
1. Implement software debouncing in ISR (1h)
2. Add timestamp tracking (1h)
3. Testing (2h)

---

### Task 3.4: Code Cleanup and Documentation ⏱️ 8 hours

**Assigned to:** All team  
**Priority:** ⚠️ MEDIUM

**Steps:**
1. Update all Doxygen comments (3h)
2. Add usage examples (2h)
3. Generate documentation (1h)
4. Code review (2h)

---

## Phase 4: Testing & Validation (Week 4)

### Task 4.1: Integration Testing ⏱️ 16 hours

**Assigned to:** All team  

**Testing Phases:**

**Phase 4.1.1: Unit Testing (4h)**
```
Test each fixed module independently:
1. Timer1: Verify 100Hz operation
2. Current Sensor: Verify calibration and readings
3. Measurement Engine: Verify power calculation
4. Protection Manager: Verify debouncing
5. Communication: Verify both protocols
```

**Phase 4.1.2: Integration Testing (4h)**
```
Test module interactions:
1. Sensor → Measurement Engine → Communication
2. Measurement Engine → Protection Manager
3. EEPROM → System Data Manager → Application
4. Mobile/Dashboard → Communication → Application
```

**Phase 4.1.3: System Testing (4h)**
```
Test complete system:
1. Power on from cold start
2. Load calibration from EEPROM
3. Start measurements
4. Connect Mobile App
5. Connect Dashboard
6. Trigger protection
7. Reset energy
8. Save to EEPROM
9. Power cycle
10. Verify persistence
```

**Phase 4.1.4: Stress Testing (4h)**
```
1. Run for 24 hours continuous
2. Multiple connect/disconnect cycles
3. Rapid relay switching
4. Extreme load variations
5. EMI/noise injection
```

---

### Task 4.2: Accuracy Validation ⏱️ 12 hours

**Test Setup:**
- Reference power meter (calibrated)
- Oscilloscope
- Known loads (resistive, inductive, capacitive)
- Data logger

**Test Procedure:**
```
1. Voltage Accuracy Test (3h)
   - Range: 100V - 250V
   - Expected: ±2% accuracy

2. Current Accuracy Test (3h)
   - Range: 0.5A - 20A
   - Expected: ±5% accuracy

3. Power Accuracy Test (3h)
   - Different load types
   - Expected: ±10% accuracy

4. Energy Integration Test (3h)
   - 1-hour, 6-hour, 24-hour runs
   - Expected: ±5% accuracy
```

---

### Task 4.3: Mobile/Dashboard Compatibility Testing ⏱️ 8 hours

**Mobile App Testing (4h):**
1. Connection establishment
2. Data reception and parsing
3. Relay control
4. Energy reset
5. Disconnection handling

**Dashboard Testing (4h):**
1. Local mode (Bluetooth)
2. Data reception and parsing
3. Real-time charts
4. Relay control
5. (Optional) Remote mode via WiFi

---

## Testing Strategy

### Test Environment Setup

**Hardware:**
- ATmega32 development board
- ACS712 current sensor
- Voltage divider circuit
- HC-05 Bluetooth module
- 16x4 LCD display
- 4-channel relay module
- Various loads (100W, 500W, 1000W)
- Calibrated power meter

**Software:**
- AVR Studio / Atmel Studio
- AVRDUDE programmer
- Serial terminal (PuTTY, TeraTerm)
- Logic analyzer software
- Oscilloscope

---

## Risk Management

### Risk Matrix

| Risk | Probability | Impact | Mitigation |
|------|------------|--------|------------|
| Timer1 fix breaks sampling | Low | High | Extensive testing, oscilloscope verification |
| EEPROM timeout causes issues | Low | Medium | Thorough timeout value testing |
| Power factor accuracy | Medium | High | Compare with calibrated meter |
| Protocol compatibility | Medium | High | Test with actual Mobile/Dashboard |
| Team member unavailability | Medium | Medium | Cross-training, documentation |

---

## Success Metrics

### Phase 1 Success Criteria
- ✅ All critical bugs fixed
- ✅ Energy measurement accurate within 10%
- ✅ No false protection trips
- ✅ Both Mobile and Dashboard can receive data

### Phase 2 Success Criteria
- ✅ Data persists across power cycles
- ✅ System never hangs (timeout protection)
- ✅ Calibration can be adjusted

### Phase 3 Success Criteria
- ✅ WiFi connectivity working (if implemented)
- ✅ User experience smooth
- ✅ Code well-documented

### Phase 4 Success Criteria
- ✅ 24-hour stability test passed
- ✅ Accuracy within specifications
- ✅ Mobile and Dashboard fully compatible

---

## Deployment Plan

### Pre-Deployment Checklist
1. [ ] All tests passed
2. [ ] Code reviewed
3. [ ] Documentation complete
4. [ ] Backup of old firmware
5. [ ] Rollback plan ready

### Deployment Steps
1. Flash new firmware
2. Erase EEPROM (force re-initialization)
3. Run calibration sequence
4. Test basic functionality
5. Connect Mobile/Dashboard
6. Monitor for 1 hour
7. If stable, mark as deployed

---

## Appendix

### Useful Commands

**Compile:**
```bash
avr-gcc -mmcu=atmega32 -Os -o main.elf main.c
```

**Flash:**
```bash
avrdude -p m32 -c usbasp -U flash:w:main.hex
```

**Test EEPROM:**
```bash
avrdude -p m32 -c usbasp -U eeprom:r:eeprom_backup.hex:i
```

---

## Document Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-12-15 | Hesham Ahmed | Initial plan created |

---

**Plan End**
