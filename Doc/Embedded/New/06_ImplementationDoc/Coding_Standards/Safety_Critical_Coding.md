# 🛡️ Safety-Critical Coding Guidelines

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-Safety_Systems-red)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Critical_Standard-brightgreen)

**Safety-Critical Coding Guidelines**

**Smart Energy Management System - Protection Logic Rules**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Core Safety Principles](#-core-safety-principles-for-energy-management)
- [Undefined Behavior](#-2-avoid-undefined-behavior)
- [Defensive Programming](#-3-defensive-programming-for-protection)
- [Type Safety](#-5-explicit-type-safety)
- [Critical Sections](#-14-critical-section-protection)

---

## 🔗 Related Documentation

| Document                                                        | Description      | Status       |
| --------------------------------------------------------------- | ---------------- | ------------ |
| **[Coding_Standards.md](Coding_Standards.md)**                  | Coding Standards | ✅ Available |
| **[Code_Review_Checklist.md](Code_Review_Checklist.md)**        | Review Checklist | ✅ Available |
| **[Risk_Management.md](../../01_Managment/Risk_Management.md)** | Risk Management  | ✅ Available |

---

## 📋 Core Safety Principles for Energy Management

This document outlines safety-critical coding practices specifically adapted for the Smart Energy Management System, where failures can result in:

- Electrical hazards (overcurrent, overvoltage)
- Equipment damage
- Fire risk
- Data loss

###1. Protective Systems Must Never Fail

**Protection Priority**: Overcurrent and overvoltage protection are the highest priority functions in the system. These must execute reliably under all conditions.

**Fail-Safe Design Philosophy**:

- Default state is SAFE (relay OFF, load disconnected)
- Protection triggers on ANY doubt (better false alarm than missed fault)
- Multiple redundant checks before allowing power to load
- Hardware watchdog as final safety net

**Rule**: Protection code paths must be simple, testable, and free of complex logic that could fail.

---

## 2. Avoid Undefined Behavior

### Division by Zero Protection

**Energy Management Context**: Power and energy calculations involve division.

**Unsafe Pattern**:

```
Description: Calculate power without checking current validity
Operation: power = voltage × current
         energy_per_hour = power / 3600
Risk: If power calculation invalid, division operation undefined
```

**Safe Pattern**:

```
Validation Steps:
1. Check if current measurement is above minimum threshold (e.g., > 0.01A)
2. Check if voltage measurement is within valid range (180V - 260V)
3. Verify calculation result is within physically possible range
4. Only then perform division operation
5. If any check fails, use default safe value (zero power)
```

### Array Bounds Checking

**Energy Management Context**: Sample buffers for RMS calculations.

**Unsafe Pattern**:

```
Description: ADC sample buffer access without validation
Buffer: voltage_samples[128]
Access: voltage_samples[sample_index] = new_value
Risk: If sample_index ≥ 128, memory corruption occurs
```

**Safe Pattern**:

```
Validation: Before array access
Step 1: Verify sample_index < BUFFER_SIZE (128)
Step 2: If invalid, reset index to zero
Step 3: Log error for debugging
Step 4: Then proceed with validated index
Result: Array access guaranteed safe
```

### Variable Initialization

**Energy Management Context**: Measurement variables must start in known state.

**Rule**: ALL variables must be explicitly initialized, especially:

- Calibration factors (default to 1.0, not random value)
- Energy counter (load from EEPROM, default to 0.0 if corrupted)
- Protection thresholds (safe defaults, never uninitialized)
- Relay state flag (always start as OFF/false)

---

## 3. Defensive Programming for Protection

### Input Validation for Safety Functions

**Overcurrent Protection Example**:

```
Function Purpose: Set new overcurrent threshold
Parameter: threshold_amps (desired overcurrent limit)

Validation Sequence:
1. Check threshold is not zero (would disable protection - unsafe!)
2. Check threshold ≥ minimum_rated_current (e.g., 0.5A)
3. Check threshold ≤ maximum_sensor_rating (30A for ACS712-30A)
4. Check threshold ≤ relay_rating (10A absolute maximum)
5. If ALL checks pass: Accept new threshold
6. If ANY check fails: Reject, keep previous safe value, log error
```

**Voltage Threshold Example**:

```
Similar validation for overvoltage protection:
- Minimum: 200V (below this, grid fault likely)
- Maximum: 280V (hardware damage risk above this)
- Reject values outside this range
- Never allow protection to be fully disabled
```

### Pointer Safety in Critical Paths

**Energy Management Context**: Passing measurement data structures.

**Rule**: Every pointer parameter in protection-related functions MUST be validated before dereference.

**Safe Pattern**:

```
Function: Process measurement data for protection check
Parameter: pointer to MeasurementData structure

Step 1: Check if pointer is NULL
Step 2: If NULL, trigger immediate protection (assume fault)
Step 3: If valid, check structure magic number/CRC
Step 4: Validate timestamp is recent (< 1 second old)
Step 5: ONLY after all checks, dereference and use data
```

---

## 4. Eliminate Magic Numbers

### Configuration with Named Constants

**Energy Management Context**: Thresholds, timing values, scaling factors.

**Unsafe**:

```
Description: Hardcoded values in protection logic
Example: if (current_reading > 2000) { trigger_protection(); }
Problem: What is 2000? What units? Easy to misunderstand.
```

**Safe**:

```
Named Constants with Units and Descriptions:

OVERCURRENT_THRESHOLD_SCALED = 2000
  Description: Overcurrent threshold
  Units: Scaled integer (current_amps × 100)
  Physical meaning: 20.00 Amperes
  Derivation: Sensor max = 30A, safe limit = 20A

MAX_SAMPLE_AGE_MS = 500
  Description: Maximum age of measurement before considered stale
  Units: Milliseconds
  Purpose: Detect sensor/ADC failure

RELAY_DEBOUNCE_COUNT = 3
  Description: Number of consecutive fault readings before relay trip
  Purpose: Prevent nuisance trips from transients
```

**Benefit**: Code is self-documenting, values easy to tune, units explicit.

---

## 5. Explicit Type Safety

### Fixed-Point Arithmetic for Measurements

**Energy Management Context**: Voltage and current transmitted as scaled integers to mobile app.

**Unsafe**:

```
Description: Implicit float-to-integer conversion
Operation: scaled_value = voltage_float * 10
Risk: Truncation, loss of precision, unexpected results
```

**Safe**:

```
Explicit Conversion with Rounding:

Step 1: Multiply float by scale factor (10 for voltage)
Step 2: Add 0.5 for proper rounding
Step 3: Cast to integer explicitly
Step 4: Verify result is within range (0 - 3000 for voltage)
Step 5: If out of range, clip to limits

Example: voltage_float = 235.47V
  Step 1: 235.47 × 10 = 2354.7
  Step 2: 2354.7 + 0.5 = 2355.2
  Step 3: (integer)2355.2 = 2355
  Result: Transmitted value = 2355 (represents 235.5V)
```

### Protection of 32-bit Arithmetic

**Energy Management Context**: Energy accumulation can overflow.

**Safe Pattern**:

```
Energy Counter Protection:

Type: 32-bit float (maximum ~3.4 × 10^38)
Physical limit: Energy in kWh unlikely to exceed 10^6 in product lifetime
Overflow check: If energy_kwh > 1,000,000 kWh
Action: Saturate at maximum, log warning
Reasoning: Better to freeze counter than wrap to zero
```

---

## 6. Switch Statements with Default

### Protection State Machine

**Energy Management Context**: Protection manager FSM must handle all states.

**Required Pattern**:

```
Protection State Switch:

States Defined:
- PROT_NORMAL: System operating safely
- PROT_DEBOUNCING: Potential fault detected, confirming
- PROT_FAULT_OVERCURRENT: Confirmed overcurrent, relay OFF
- PROT_FAULT_OVERVOLTAGE: Confirmed overvoltage, relay OFF

Switch Statement Structure:
  case PROT_NORMAL:
    Action: Monitor measurements, no protection action

  case PROT_DEBOUNCING:
    Action: Count consecutive faults, trigger if threshold met

  case PROT_FAULT_OVERCURRENT:
    Action: Keep relay OFF, wait for manual reset

  case PROT_FAULT_OVERVOLTAGE:
    Action: Keep relay OFF, wait for manual reset

  default: ← MANDATORY for safety
    Action: Assume unknown state = FAULT
    Force relay OFF
    Log critical error
    Reset system (watchdog timeout)
```

**Reasoning**: If FSM enters undefined state (due to memory corruption, cosmic ray, etc.), default case provides fail-safe behavior.

---

## 7. Minimize Side Effects in Expressions

### Protection Logic Must Be Explicit

**Unsafe** (Multiple side effects):

```
Description: Complex expression with hidden state changes
Expression: if ((++fault_count > 3) && (relay_on = false)) { ... }
Problems:
  - Post-increment and assignment in condition
  - Difficult to trace execution order
  - Risk of logic errors
```

**Safe** (Explicit sequence):

```
Clear Step-by-Step Protection Logic:

Step 1: fault_count = fault_count + 1
Step 2: relay_state = RELAY_OFF
Step 3: if (fault_count > DEBOUNCE_THRESHOLD) {
          trigger_protection();
        }

Benefits:
  - Each operation on separate line
  - Easy to set breakpoints during debugging
  - Clear order of operations
  - No ambiguity in logic
```

---

## 8. Global Variable Discipline

### Protection Data Encapsulation

**Rule**: Protection state must be encapsulated, not exposed as raw globals.

**Unsafe**:

```
Description: Protection state as global variables visible everywhere
Variables:
  g_overcurrent_fault (accessible from any module)
  g_relay_state (can be modified anywhere)
Risk: Any module can corrupt protection state
```

**Safe**:

```
Module-Private State with Controlled Access:

File: ProtectionManager.c

Static (file-scope) variables:
  static bool s_protection_triggered;
  static uint8_t s_debounce_counter;
  static ProtectionState_t s_current_state;

Public API functions:
  bool PM_IsProtectionActive(void)
    Returns: Current protection status
    Access: Read-only, cannot modify state

  void PM_Reset(void)
    Purpose: Clear protection fault (manual reset)
    Validation: Only allowed if fault condition cleared

  void PM_ForceTrip(void)
    Purpose: Emergency protection trigger
    Use: Software-commanded emergency stop

Benefits:
  - Protection state cannot be accidentally corrupted
  - All state changes go through validated functions
  - Easy to add logging/diagnostics to state changes
```

---

## 9. Stack Usage Management

### Avoid Large Buffers on Stack

**Energy Management Context**: ADC sample buffers, communication buffers.

**Unsafe**:

```
Description: Large temporary array in function
Function: Process_RMS_Calculation()
  Local variable: uint16_t large_buffer[256]
  Stack usage: 512 bytes
  Total SRAM: 2048 bytes
  Risk: 25% of SRAM on single function stack!
```

**Safe**:

```
Static Allocation for Large Buffers:

File: Measurement_Engine.c

Module-private buffers:
  static uint16_t s_voltage_samples[128];  // 256 bytes
  static uint16_t s_current_samples[128];  // 256 bytes
  Total: 512 bytes in .bss section (not stack)

Stack usage per function:
  Maximum local variables: < 50 bytes per function
  Safe for nested calls: 8-10 levels deep
  Stack headroom: ~1 KB remaining for RTOS tasks
```

---

## 10. Watchdog Refresh Strategy

### Energy Management Watchdog Implementation

**Purpose**: Detect software failures (infinite loops, stack corruption, deadlocks) and automatically recover.

**Timeout Selection**: 500ms (provides 5× safety margin for 100ms main loop)

**Refresh Location**: Single, well-defined location only.

**Safe Pattern**:

```
Main System Loop:

Initialize:
  Configure watchdog: 500ms timeout
  Enable watchdog

Main Loop (forever):
  Step 1: Execute Measurement Engine (max 30ms)
  Step 2: Execute Protection Manager (max 10ms)
  Step 3: Execute Display Manager (max 20ms)
  Step 4: Execute Communication Manager (max 20ms)
  Step 5: Execute Energy Logger (max 10ms)

  Total worst-case: ~90ms

  Step 6: Refresh Watchdog ← ONLY HERE

  Step 7: Delay until 100ms cycle complete

Loop repeats
```

**What Watchdog Detects**:

- Infinite loop in any module (loop doesn't reach watchdog refresh)
- Function hang (blocking I/O, EEPROM write stuck)
- Stack overflow leading to crash
- ISR infinite loop

**Recovery**: Automatic system reset, protection state forced to SAFE (relay OFF).

---

## 11. Fail-Safe Default States

### Power-Up and Error Defaults

**Energy Management Fail-Safe Philosophy**:

**Relay Control**:

```
Default State: OFF (load disconnected)
Power-Up: Relay remains OFF until:
  1. Measurements validated as normal
  2. No protection faults present
  3. Explicit user enable command received
Reason: Fail-safe is NO POWER to load
```

**Protection Thresholds**:

```
Default Values (if EEPROM corrupted):
  Overcurrent threshold: 5.0A (conservative, safe limit)
  Overvoltage threshold: 250V (protecting for 220V nominal)
Reason: Better to trip unnecessarily than miss real fault
```

**Calibration Factors**:

```
Default: 1.0 (no scaling)
Recovery: If EEPROM invalid:
  - Use factory defaults
  - Flag "uncalibrated" status
  - Measurements still functional, just inaccurate
  - User prompted to recalibrate
Reason: Degraded operation better than no operation
```

**Energy Counter**:

```
Default: Read from EEPROM
If EEPROM corrupted: Start from 0.0 kWh
Logging: Record that counter was reset
Reason: Loss of energy data acceptable, system continues operating
```

---

## 12. Protection Response Time Guarantees

### Timing-Critical Safety Requirements

**Maximum Protection Response Time**: 300ms from fault condition to relay disconnection.

**Timing Budget Breakdown**:

```
ADC Conversion: 10ms (next sample in Timer1 cycle)
Deb ounce Period: 30ms (3 consecutive samples @ 10ms each)
Protection Decision: < 1ms (FSM transition)
Relay Driver: < 10ms (ULN2003 + relay coil energization)

Total: ~50ms typical, 100ms worst-case
Safety Margin: 3× under requirement ✓
```

**Timing Protection Mechanisms**:

- Timer1 ISR priority: Highest (ensures ADC sampling never delayed)
- Protection Manager: Runs immediately after measurements available
- No blocking calls in protection path (no EEPROM writes, no delays)
- Relay control: Direct GPIO write (no queue, immediate)

---

## 13. Error Logging for Safety Analysis

### Fault Recording Strategy

**Purpose**: Enable post-fault analysis, identify recurring issues, improve safety.

**What to Log**:

```
Protection Event Record:
  - Timestamp (milliseconds since power-on)
  - Fault type (overcurrent, overvoltage)
  - Measured voltage at fault
  - Measured current at fault
  - Calculated power at fault
  - Number of times protection triggered (lifetime counter)
  - Relay state before/after

Storage: EEPROM (dedicated 256-byte circular buffer)
Retrieval: Via UART command for maintenance analysis
```

**Benefits**:

- Identify borderline faults vs clear violations
- Detect nuisance trips requiring threshold adjustment
- Track degradation of electrical loads over time

---

## 14. Critical Section Protection

### EEPROM Write Atomicity

**Energy Management Context**: Energy counter must be written atomically to prevent corruption.

**Critical Requirement**: EEMWE→EEWE sequence must complete within 4 CPU cycles uninterrupted.

**Safe Pattern**:

```
EEPROM Write Sequence:

Step 1: Disable Global Interrupts
  Purpose: Prevent ISR from breaking critical timing

Step 2: Set EEMWE bit (Master Write Enable)
  Time-critical window: 4 cycles only

Step 3: Set EEWE bit (Write Enable) within 4 cycles
  Atomic operation initiated

Step 4: Re-enable Global Interrupts
  Critical section complete

Step 5: Wait for write completion (non-critical)
  Can be interrupted, doesn't affect write

Maximum Interrupt Disable Time: < 1 microsecond
Impact on Real-Time Performance: Negligible
```

---

## 15. Measurement Validation

### Sanity Checks on Sensor Data

**Protect Against Sensor Failures**:

**Voltage Range Validation**:

```
Physical Reality: AC mains voltage in Egypt = 220V ± 10%
Valid Range: 180V to 260V

If measured voltage:
  < 50V: Sensor disconnected or failed → Trigger sensor fault
  50V - 180V: Unusual but possible brownout → Allow, log warning
  180V - 260V: Normal operating range → Accept
  > 260V: Definitely fault (overvoltage) → Trigger protection
  > 400V: Sensor malfunction (impossible value) → Trigger sensor fault
```

**Current Range Validation**:

```
Physical Reality: ACS712-30A sensor range = 0 to 30A

If measured current:
  < 0A: Impossible (negative current) → Sensor fault
  0 - 0.01A: Likely zero, within noise → Clamp to zero
  0.01A - 20A: Normal range → Accept
  > 20A: Approaching sensor maximum → Trigger overcurrent protection
  > 30A: Sensor saturation → Assume maximum fault condition
```

**Derivative Check** (detect spikes):

```
Rapid Change Detection:
  If |current[n] - current[n-1]| > 10A in single sample:
    Physical impossibility: Current cannot change that fast
    Likely: Noise spike, EMI, sensor glitch
    Action: Discard sample, use previous value
    Log: Noise event for analysis
```

---

## 📞 Support & Contact

**Project Information**:

- **Project Name**: Smart Energy Management System
- **Development Company**: Gestell - Professional Embedded Solutions

**Technical Support**: Hisham4Ahmed@gmail.com

---

## 📄 Document Control

| Attribute                  | Value                      |
| -------------------------- | -------------------------- |
| **Document Type**          | Safety Critical Guidelines |
| **Document Status**        | Active                     |
| **Document Version**       | 1.0                        |
| **Last Updated**           | January 2026               |
| **Prepared By**            | Gestell Engineering Team   |
| **Safety Integrity Level** | Critical Protection        |

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
