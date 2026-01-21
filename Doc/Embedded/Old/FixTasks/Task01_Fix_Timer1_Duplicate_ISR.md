# Task 01: Fix Timer1 Duplicate ISR

## Task Information

- **Priority**: 🔴 CRITICAL
- **Assigned to**: Mohamed Diaa
- **Estimated Time**: 8 hours
- **Category**: MCAL Layer - Timer Driver
- **Blocker**: Yes - affects all measurements

---

## Problem Description

### Current Issue

The Timer1 driver has **two ISR handlers** (`__vector_7` and `__vector_8`) that both invoke the same callback function, causing the callback to be called **twice per timer cycle** instead of once.

### Impact

- ❌ **Sampling rate doubled**: 200 Hz instead of intended 100 Hz
- ❌ **Energy calculation wrong**: Double the actual value
- ❌ **RMS calculations incorrect**: Wrong time window
- ❌ **All measurements affected**: Voltage, Current, Power, Energy

### Root Cause

Both Compare Match A and Compare Match B interrupts are enabled with the same OCR values (1250), and both call the same callback function.

---

## Technical Background

### Timer1 Configuration

```
Operating Mode: CTC (Clear Timer on Compare)
Clock Source: System clock with prescaler 64
Frequency: 16MHz / 64 = 250kHz
TOP Value: ICR1 = 1250
Target Frequency: 250kHz / 1250 = 200Hz (per channel)
Desired Frequency: 100Hz (single channel)

Current Setup:
- OCR1A = 1250 → triggers __vector_7 @ 100Hz ✅
- OCR1B = 1250 → triggers __vector_8 @ 100Hz ❌ (DUPLICATE!)
- Both call same callback → Effective rate = 200Hz ❌
```

---

## Files to Modify

### File 1: `Mcal/Timer1/TIMER1_Program.c`

- Lines 93-115: ISR handlers
- Lines 40-50: Initialization function

### File 2: `Mcal/Timer1/TIMER1_Config.h`

- Configuration macros (documentation only)

---

## Implementation Steps

### Step 1: Understand Current Behavior (1 hour)

**Actions:**

1. Open `Mcal/Timer1/TIMER1_Program.c`
2. Review lines 93-115 (ISR handlers)
3. Document current callback invocation flow
4. Measure actual sampling rate with oscilloscope or logic analyzer
5. Verify the double-call issue

**Expected Finding:**

- Callback invoked 200 times per second
- Energy accumulates twice as fast as it should

---

### Step 2: Update Timer1 Initialization (2 hours)

**File**: `Mcal/Timer1/TIMER1_Program.c`

**Function**: `mTIMER1_Init()`

**Changes Required:**

**Before:**

```c
void mTIMER1_Init(void)
{
    // ... other config ...

    // Enable both interrupts ❌
    SetBit(TIMSK_Reg, OCIE1A_Bit);
    SetBit(TIMSK_Reg, OCIE1B_Bit);  // ❌ THIS CAUSES DUPLICATE
}
```

**After:**

```c
void mTIMER1_Init(void)
{
    // Configure Timer1 in CTC mode
    // WGM13:0 = 0b1100 (CTC mode with ICR1 as TOP)
    SetBit(TCCR1B_Reg, WGM13_Bit);
    SetBit(TCCR1B_Reg, WGM12_Bit);

    // Set TOP value
    ICR1_Reg = 1250;

    // Set Compare Match A value (used for interrupt)
    OCR1A_Reg = 1250;

    // Enable ONLY Compare Match A interrupt
    SetBit(TIMSK_Reg, OCIE1A_Bit);  // ✅ Enable

    // Explicitly disable Compare Match B interrupt
    ClearBit(TIMSK_Reg, OCIE1B_Bit);  // ✅ Disable to prevent duplicate calls

    // Set prescaler to 64: CS12=0, CS11=1, CS10=1
    ClearBit(TCCR1B_Reg, CS12_Bit);
    SetBit(TCCR1B_Reg, CS11_Bit);
    SetBit(TCCR1B_Reg, CS10_Bit);

}
```

---

### Step 3: Remove Duplicate ISR Handler (2 hours)

**File**: `Mcal/Timer1/TIMER1_Program.c`

**Lines**: 93-115

**Before:**

```c
// Compare Match A ISR
void __vector_7(void) __attribute__((signal));
void __vector_7()
{
    if(Timer1_Global_Callback != Null)
    {
        Timer1_Global_Callback();  // ✅ First call
    }
}

// Compare Match B ISR
void __vector_8(void) __attribute__((signal));  // ❌ REMOVE
void __vector_8()                               // ❌ REMOVE
{                                               // ❌ REMOVE
    if(Timer1_Global_Callback != Null)          // ❌ REMOVE
    {                                           // ❌ REMOVE
        Timer1_Global_Callback();               // ❌ DUPLICATE CALL!
    }                                           // ❌ REMOVE
}                                               // ❌ REMOVE
```

**After:**

```c
/**
 * @brief Timer1 Compare Match A ISR
 * @details Called at 100Hz (every 10ms) to trigger ADC sampling
 *          This is the ONLY interrupt enabled for Timer1
 *          Compare Match B is disabled to prevent duplicate callbacks
 * @note ISR vector number 7 corresponds to TIMER1_COMPA_vect
 */
void __vector_7(void) __attribute__((signal));
void __vector_7()
{
    if(Timer1_Global_Callback != Null)
    {
        Timer1_Global_Callback();
    }
}

// __vector_8 (Timer1 Compare Match B) is NOT used in this application
// It has been removed to prevent duplicate callback invocations
```

---

### Step 4: Update Documentation Comments (1 hour)

**Add to file header:**

```c
/**
 * @file TIMER1_Program.c
 * @brief Timer1 driver implementation for ATmega32
 *
 * @details
 * Timer1 Configuration:
 * - Mode: CTC (Clear Timer on Compare) with ICR1 as TOP
 * - Clock: System clock (16MHz) with prescaler 64
 * - Frequency: 100Hz (10ms period)
 * - Interrupt: Compare Match A only (TIMER1_COMPA_vect)
 * - Compare Match B is DISABLED to prevent duplicate callbacks
 *
 * @note Only __vector_7 (COMPA) ISR is implemented
 *       __vector_8 (COMPB) is intentionally not used
 */
```

---

### Step 5: Testing and Verification (2 hours)

**Test Setup:**

1. Connect oscilloscope or logic analyzer to a test GPIO pin
2. Modify callback to toggle test pin
3. Flash modified firmware to ATmega32

**Test Procedure:**

**Test 5.1: Frequency Verification**

```c
// Add to callback (temporary test code):
void Test_Callback(void)
{
    static uint8_t toggle = 0;
    toggle = !toggle;

    if(toggle)
        SetBit(PORTD_Reg, 0);  // Set PD0 high
    else
        ClearBit(PORTD_Reg, 0);  // Set PD0 low

    // Original callback functionality
    ME_Update();
}
```

**Expected Result:**

- Pin toggles at 50Hz (100Hz / 2)
- Period = 20ms between rising edges
- If sees 10ms → still has duplicate issue ❌

**Test 5.2: Energy Accumulation Rate**

```
1. Reset energy counter to zero
2. Connect known 1000W load (measured with reference power meter)
3. Run for exactly 1 hour
4. Check energy reading

Expected: ~1.0 kWh
If reads ~2.0 kWh → still has duplicate issue ❌
```

**Test 5.3: RMS Sampling Count**

```c
// Add debug counter in ME_Update():
void ME_Update(void)
{
    static uint16_t sample_count = 0;
    sample_count++;

    // Every 1 second should have 100 samples
    if((sample_count % 100) == 0)
    {
        // Log via UART: "100 samples in 1 second"
    }

    // ... rest of ME_Update ...
}
```

**Expected Result:**

- 100 samples per second
- If sees 200 → still has duplicate issue ❌

---

## Success Criteria

✅ **Callback invoked exactly 100 times per second** (not 200)  
✅ **GPIO toggle test shows 50Hz frequency**  
✅ **Energy accumulation rate corrected** (1000W load → 1.0 kWh/hour)  
✅ **No compilation errors or warnings**  
✅ **Code properly documented**

---

## Deliverables

1. ✅ Modified `Mcal/Timer1/TIMER1_Program.c` with single ISR
2. ✅ Updated Doxygen comments
3. ✅ Test report with oscilloscope screenshot
4. ✅ Energy accumulation test results
5. ✅ Git commit with clear message

---

## Dependencies

**Blocked By**: None

**Blocks**:

- Task 03: Fix Power Calculation (needs correct sampling rate)
- All measurement accuracy improvements

---

## Notes and Considerations

### Alternative Solution (If Both Channels Needed in Future)

If future requirements need both Timer1 channels:

```c
// Separate callbacks for each channel
static void (*Timer1_CallbackA)(void) = Null;
static void (*Timer1_CallbackB)(void) = Null;

void mTIMER1_RegisterCallback_ChannelA(void (*callback)(void))
{
    Timer1_CallbackA = callback;
}

void mTIMER1_RegisterCallback_ChannelB(void (*callback)(void))
{
    Timer1_CallbackB = callback;
}

void __vector_7(void) { if(Timer1_CallbackA) Timer1_CallbackA(); }
void __vector_8(void) { if(Timer1_CallbackB) Timer1_CallbackB(); }
```

### Safety Note

⚠️ **CRITICAL**: Do not re-enable OCIE1B without assigning a different callback or setting different OCR1B value!

---

## Related Documentation

- `Doc/Embedded/Issue_Solutions.md` - Section "MCAL Layer Issues #1"
- `Doc/Embedded/Embedded_Fix_Implementation_Plan.md` - Phase 1, Task 1.1
- ATmega32 Datasheet - Section 15: 16-bit Timer/Counter1

---

**Document Version**: 1.0  
**Created**: 2026-01-16  
**Last Updated**: 2026-01-16
