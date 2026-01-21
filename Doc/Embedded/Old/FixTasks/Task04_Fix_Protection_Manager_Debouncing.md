# Task 04: Fix Protection Manager Debouncing

## Task Information

- **Priority**: 🔴 CRITICAL
- **Assigned to**: Mohamed Abdelgaber
- **Estimated Time**: 10 hours
- **Category**: Application Layer - Protection Manager
- **Blocker**: Yes - causes frequent false trips

---

## Problem Description

### Current Issue

The Protection Manager **immediately cuts power** on the first high reading, causing **false trips** during normal inrush currents (motor startup, transformer energization).

### Impact

- ❌ **False trips during motor startup** (inrush current)
- ❌ **Nuisance tripping on ADC noise**
- ❌ **Poor user experience** - appliances randomly shut off
- ❌ **System unusable** with motors, compressors, transformers
- ❌ **No hysteresis** - causes oscillation at threshold boundaries

### Root Cause

```c
// Current code:
if (current > threshold)  // ❌ Single sample!
{
    CUT_POWER();  // Immediate trip
}
```

**Real-World Example:**

```
Motor startup inrush:
t=0ms:   I=1A    (normal)
t=10ms:  I=30A   ❌ INSTANT TRIP! (but this is normal inrush!)
t=50ms:  I=5A    (would be normal running current - never reached)
```

---

## Technical Background

### Motor Inrush Current

When motors start, they draw 5-10× normal current for 50-200ms:

```
Type                Normal    Inrush    Duration
Small fan           0.5A      3A        50ms
Refrigerator comp.  1.5A      12A       100ms
Drill/power tool    2A        15A       80ms
```

**Problem**: Without debouncing, protection trips immediately on inrush, preventing motor from starting.

### Debouncing Concept

Like a button debounce, but for analog signals:

- Require **sustained** fault (e.g., 10 consecutive readings > threshold)
- Use **hysteresis** (different trip/reset thresholds)
- **Auto-reset** after sustained normal operation

### Timing at 100Hz Sampling

```
Sample Count    Time
1               10ms
5               50ms
10              100ms   ← Good for trip detection
20              200ms   ← Good for reset confirmation
50              500ms
100             1000ms (1 second)
```

---

## Files to Modify

### File 1: `App/ProtectionManager/ProtectionManager_Config.h`

- Add debounce configuration

### File 2: `App/ProtectionManager/ProtectionManager_Program.c`

- Lines 41-56: `PM_Update()` function
- Add debounce state variables
- Implement debounced logic with hysteresis

---

## Implementation Steps

### Step 1: Design Debouncing Algorithm (2 hours)

**Actions:**

1. Research typical inrush durations for common loads
2. Define debounce counter thresholds
3. Design state machine with hysteresis
4. Document timing calculations

**Debounce Strategy:**

```
TRIP Logic:
- Counter increments when value > threshold
- Trips when counter >= TRIP_DEBOUNCE_COUNT
- Prevents false trips on transient spikes

RESET Logic:
- Counter decrements when value < (threshold - hysteresis)
- Resets when counter == 0
- Prevents oscillation at threshold boundary

HYSTERESIS Zone:
- Between (threshold - hysteresis) and (threshold)
- Maintains current state
- Provides stability
```

**Example Timing:**

```
TRIP_DEBOUNCE_COUNT = 10 → 100ms sustained fault required
RESET_DEBOUNCE_COUNT = 20 → 200ms normal before auto-reset
```

---

### Step 2: Update Protection Config (1 hour)

**File**: `App/ProtectionManager/ProtectionManager_Config.h`

**Add:**

```c
/******************************************************************************
 * Protection Debouncing Configuration
 ******************************************************************************/

/**
 * @brief Trip debounce count
 * @details Number of consecutive over-threshold readings required
 *          before triggering protection trip
 * @note At 100Hz sampling:
 *       - 5 counts = 50ms (too fast, may trip on inrush)
 *       - 10 counts = 100ms (recommended - allows inrush)
 *       - 20 counts = 200ms (very safe, but slower protection)
 */
#define PM_TRIP_DEBOUNCE_COUNT       10

/**
 * @brief Reset debounce count
 * @details Number of consecutive normal readings required
 *          before auto-resetting protection
 * @note Should be >= TRIP_DEBOUNCE_COUNT to prevent oscillation
 *       20 counts = 200ms confirmation period
 */
#define PM_RESET_DEBOUNCE_COUNT      20

/******************************************************************************
 * Hysteresis Configuration
 ******************************************************************************/

/**
 * @brief Voltage hysteresis
 * @details Gap between trip and reset thresholds
 *          Prevents oscillation when voltage hovers near threshold
 * @example If threshold = 250V, hysteresis = 10V:
 *          - Trips at 250V
 *          - Resets at 240V
 */
#define PM_VOLTAGE_HYSTERESIS        10.0f  // 10V

/**
 * @brief Current hysteresis
 */
#define PM_CURRENT_HYSTERESIS        2.0f   // 2A

/**
 * @brief Power hysteresis
 */
#define PM_POWER_HYSTERESIS          200.0f // 200W

/******************************************************************************
 * Protection States
 ******************************************************************************/

/**
 * @brief Protection state definitions
 */
typedef enum
{
    PM_STATE_SAFE = 0,      // Normal operation
    PM_STATE_DANGER = 1,    // Protection tripped
    PM_STATE_WARNING = 2    // Optional: approaching limits (future)
} PM_State_t;
```

---

### Step 3: Add Debounce State Variables (1 hour)

**File**: `App/ProtectionManager/ProtectionManager_Program.c`

**Add to private variables section:**

```c
/******************************************************************************
 * Private Variables
 ******************************************************************************/

// Protection thresholds (existing)
static float Vrms_Threshold = 250.0f;
static float Irms_Threshold = 16.0f;
static float P_Threshold = 3500.0f;

// Debounce counters ✅ NEW
static uint8_t PM_Overvoltage_Counter = 0;
static uint8_t PM_Overcurrent_Counter = 0;
static uint8_t PM_Overpower_Counter = 0;

// Trip status flags ✅ NEW
static uint8_t PM_Voltage_Tripped = 0;
static uint8_t PM_Current_Tripped = 0;
static uint8_t PM_Power_Tripped = 0;

// Protection state
static PM_State_t Protection_State = PM_STATE_SAFE;
```

---

### Step 4: Implement Debounced Protection Logic (4 hours)

**File**: `App/ProtectionManager/ProtectionManager_Program.c`

**Function**: `PM_Update()`

**Before (BUGGY CODE):**

```c
void PM_Update()
{
    ME_Update();

    // ❌ Trips immediately on single reading!
    if (ME_GetVoltageRMS() > Vrms_Threshold ||
        ME_GetCurrentRMS() > Irms_Threshold ||
        ME_GetPower() > P_Threshold)
    {
        Protection_State = Danger;

        // Cut ALL relays
        for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
        {
            hRelay_Off(Relay_id);
        }

        hRGB_SetState(RGB_RED);
        Buzzer_On();
    }
}
```

**After (FIXED CODE):**

```c
/**
 * @brief Update protection monitoring with debouncing
 * @details Monitors voltage, current, and power with:
 *          - Sustained fault detection (debouncing)
 *          - Hysteresis to prevent oscillation
 *          - Auto-reset after sustained normal operation
 * @note Called every 10ms by main loop
 */
void PM_Update(void)
{
    float voltage, current, power;
    uint8_t fault_detected = 0;

    // Update measurements
    ME_Update();

    // Read current values
    voltage = ME_GetVoltageRMS();
    current = ME_GetCurrentRMS();
    power = ME_GetPower();

    /**************************************************************************
     * OVERVOLTAGE PROTECTION with Debouncing and Hysteresis
     **************************************************************************/
    if (voltage > Vrms_Threshold)
    {
        // Voltage ABOVE threshold → increment counter
        PM_Overvoltage_Counter++;

        // Trip after sustained overvoltage
        if (PM_Overvoltage_Counter >= PM_TRIP_DEBOUNCE_COUNT)
        {
            PM_Voltage_Tripped = 1;
            PM_Overvoltage_Counter = PM_TRIP_DEBOUNCE_COUNT;  // Cap counter
        }
    }
    else if (voltage < (Vrms_Threshold - PM_VOLTAGE_HYSTERESIS))
    {
        // Voltage well BELOW threshold (hysteresis zone) → reset
        PM_Overvoltage_Counter = 0;
        PM_Voltage_Tripped = 0;
    }
    else
    {
        // In HYSTERESIS zone → gradually decrement
        if (PM_Overvoltage_Counter > 0)
        {
            PM_Overvoltage_Counter--;
        }
    }

    /**************************************************************************
     * OVERCURRENT PROTECTION with Debouncing and Hysteresis
     **************************************************************************/
    if (current > Irms_Threshold)
    {
        // Current ABOVE threshold → increment counter
        PM_Overcurrent_Counter++;

        // Trip after sustained overcurrent
        if (PM_Overcurrent_Counter >= PM_TRIP_DEBOUNCE_COUNT)
        {
            PM_Current_Tripped = 1;
            PM_Overcurrent_Counter = PM_TRIP_DEBOUNCE_COUNT;  // Cap counter
        }
    }
    else if (current < (Irms_Threshold - PM_CURRENT_HYSTERESIS))
    {
        // Current well BELOW threshold → reset
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

    /**************************************************************************
     * OVERPOWER PROTECTION with Debouncing and Hysteresis
     **************************************************************************/
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

    /**************************************************************************
     * TRIP DECISION LOGIC
     **************************************************************************/
    // Fault if ANY protection tripped
    fault_detected = PM_Voltage_Tripped || PM_Current_Tripped || PM_Power_Tripped;

    if (fault_detected && (Protection_State != PM_STATE_DANGER))
    {
        // ✅ TRIP: Transition to danger state
        Protection_State = PM_STATE_DANGER;

        // Cut all relays immediately
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
        // ✅ AUTO-RESET: All faults cleared for sustained period
        // Only reset if ALL counters are zero (confirmed normal)
        if (PM_Overvoltage_Counter == 0 &&
            PM_Overcurrent_Counter == 0 &&
            PM_Overpower_Counter == 0)
        {
            Protection_State = PM_STATE_SAFE;
            hRGB_SetState(RGB_GREEN);
            hBuzzer_Off();

            // Optional: Restore relays automatically
            // (or require manual reset via button/app)
        }
    }
}
```

---

### Step 5: Add Logging Function (1 hour)

**File**: `App/ProtectionManager/ProtectionManager_Program.c`

**Add function:**

```c
/**
 * @brief Log the reason for protection trip
 * @details Can be displayed on LCD or sent via Bluetooth
 */
static void PM_LogTripReason(void)
{
    #ifdef DEBUG_MODE
        char msg[80];

        if (PM_Voltage_Tripped)
        {
            sprintf(msg, "[TRIP] Overvoltage: %.1fV > %.1fV\r\n",
                    ME_GetVoltageRMS(), Vrms_Threshold);
            UART_SendString(msg);
        }

        if (PM_Current_Tripped)
        {
            sprintf(msg, "[TRIP] Overcurrent: %.2fA > %.2fA\r\n",
                    ME_GetCurrentRMS(), Irms_Threshold);
            UART_SendString(msg);
        }

        if (PM_Power_Tripped)
        {
            sprintf(msg, "[TRIP] Overpower: %.1fW > %.1fW\r\n",
                    ME_GetPower(), P_Threshold);
            UART_SendString(msg);
        }
    #endif
}
```

---

### Step 6: Testing and Verification (2 hours)

**Test Setup:**

1. Motor or drill (for inrush current testing)
2. Variable power supply (for controlled overvoltage)
3. Adjustable load (power resistors or dimmers)
4. Oscilloscope or logic analyzer
5. UART terminal for debug output

**Test 6.1: Motor Startup (Inrush Test)**

```
Load: Electric drill or fan
Normal Current: ~2A
Inrush Current: ~15A for 80ms

Procedure:
1. Set PM_TRIP_DEBOUNCE_COUNT = 10 (100ms)
2. Set Irms_Threshold = 10A (should NOT trip on inrush)
3. Turn on motor, observe behavior

Expected Result:
✅ Motor starts successfully
✅ Relay stays ON
✅ Debug shows: "Overcurrent counter: 1, 2, 3, ..., 8" but not 10
✅ Motor reaches normal current, counter resets to 0

Wrong Behavior (if not fixed):
❌ Relay trips immediately
❌ Motor never starts
```

**Test 6.2: Sustained Overcurrent (Real Fault)**

```
Procedure:
1. Gradually increase load until current > threshold
2. Hold current above threshold continuously
3. Monitor counter and trip

Expected Result:
✅ Counter increments: 1, 2, 3, ..., 10
✅ At count=10: TRIP occurs
✅ Relays cut, alarm activates
✅ Total time to trip: ~100ms

Debug Output:
[PM] Current: 11.5A, Counter: 1
[PM] Current: 11.6A, Counter: 2
...
[PM] Current: 11.8A, Counter: 10
[TRIP] Overcurrent: 11.8A > 10.0A
```

**Test 6.3: Hysteresis Test**

```
Purpose: Verify no oscillation at threshold boundary

Procedure:
1. Set Irms_Threshold = 10A
2. Set PM_CURRENT_HYSTERESIS = 2A
3. Adjust load to hover around 9.5A - 10.5A

Expected Behavior:
- If current = 10.5A (above threshold): Counter increments
- If current drops to 9.5A (in hysteresis): Counter decrements slowly
- If current drops to 7.5A (below hysteresis): Counter resets to 0
- No rapid ON/OFF cycling of relays

```

**Test 6.4: Auto-Reset Test**

```
Procedure:
1. Cause a real overcurrent trip
2. Reduce load below (threshold - hysteresis)
3. Wait for auto-reset

Expected Result:
✅ After trip, Protection_State = DANGER
✅ Counter starts decrementing
✅ When counter = 0: Auto-reset occurs
✅ Protection_State = SAFE
✅ RGB turns GREEN, buzzer turns OFF
✅ Total reset time: ~200ms (PM_RESET_DEBOUNCE_COUNT * 10ms)
```

---

## Success Criteria

✅ **Motor startup does NOT cause false trips**  
✅ **Real sustained faults trip after 100ms**  
✅ **Auto-reset works after fault clears**  
✅ **No oscillation at threshold boundaries** (hysteresis working)  
✅ **Relays protected properly**  
✅ **Debug logging shows counter behavior**  
✅ **Code properly documented**

---

## Deliverables

1. ✅ Updated `ProtectionManager_Program.c` with debouncing
2. ✅ Updated `ProtectionManager_Config.h` with debounce settings
3. ✅ Test report with motor startup test results
4. ✅ Oscilloscope screenshots showing inrush current handling
5. ✅ Debug log file showing counter behavior
6. ✅ Git commit with clear message

---

## Dependencies

**Blocked By**:

- Task 01 (Timer1 fix) - Needs correct timing
- Task 02 (Current sensor fix) - Needs accurate current readings

**Blocks**:

- System usability with motors and inductive loads

---

## Configuration Tuning Guide

### Choosing Debounce Counts

**For faster protection (industrial, high-value equipment):**

```c
#define PM_TRIP_DEBOUNCE_COUNT       5   // 50ms
#define PM_RESET_DEBOUNCE_COUNT      10  // 100ms
```

**For slower protection (allows longer inrush):**

```c
#define PM_TRIP_DEBOUNCE_COUNT       20  // 200ms
#define PM_RESET_DEBOUNCE_COUNT      40  // 400ms
```

**Residential default (balanced):**

```c
#define PM_TRIP_DEBOUNCE_COUNT       10  // 100ms
#define PM_RESET_DEBOUNCE_COUNT      20  // 200ms
```

### Choosing Hysteresis

**Rule of Thumb:**

- Voltage: 5-10V (2-4%)
- Current: 10-15% of threshold
- Power: 5-10% of threshold

---

## Related Documentation

- `Doc/Embedded/Issue_Solutions.md` - Section "Application Layer Issues #10"
- `Doc/Embedded/Embedded_Fix_Implementation_Plan.md` - Phase 1, Task 1.4
- IEC 61000-4-11: Voltage Dips and Interruptions Immunity

---

**Document Version**: 1.0  
**Created**: 2026-01-16  
**Last Updated**: 2026-01-16
