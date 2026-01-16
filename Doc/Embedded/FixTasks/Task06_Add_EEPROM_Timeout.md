# Task 06: Add EEPROM Timeout Protection

## Task Information

- **Priority**: ⚠️ MEDIUM
- **Assigned to**: Ahmed Ashraf
- **Estimated Time**: 6 hours
- **Category**: MCAL Layer - EEPROM Driver
- **Blocker**: No - but can cause system hangs

---

## Problem Description

### Current Issue

The EEPROM driver can **hang indefinitely** if hardware fails during write/read operations, causing complete system freeze.

### Impact

- ❌ **System freezes completely** if EEPROM fails
- ❌ **No error recovery possible**
- ❌ **Watchdog reset required** (if enabled)
- ❌ **User must power cycle** device

### Root Cause

```c
// Current code:
while (GetBit(EECR_Reg, EEWE_Bit) == 1)
{
    // ❌ Infinite loop - NO TIMEOUT!
}
```

If EEPROM hardware fails, this loop never exits.

---

## Technical Background

### EEPROM Write Timing (ATmega32)

- **Typical write time**: 3.3ms @ 25°C
- **Maximum write time**: 8.5ms (datasheet guaranteed) -**cycles before wear-out**: 100,000 writes

### Timeout Strategy

```
Timeout = 10ms (conservative, >8.5ms max)
Poll delay = 100µs (reasonable granularity)
Max iterations = 10ms / 100µs = 100
```

---

## Files to Modify

### File: `Mcal/EEPROM/mEEPROM_Program.c`

- Add timeout to `mEEPROM_WriteByte()`
- Add timeout to `mEEPROM_Write()` (block function)
- Add overflow check

---

## Implementation Steps

### Step 1: Add Configuration (1 hour)

**File**: `Mcal/EEPROM/mEEPROM_Config.h`

**Add:**

```c
/******************************************************************************
 * EEPROM Timeout Configuration
 ******************************************************************************/

/**
 * @brief EEPROM write timeout in milliseconds
 * @details Maximum time to wait for write completion
 *          Datasheet max: 8.5ms, we use 10ms for safety
 */
#define EEPROM_TIMEOUT_MS            10

/**
 * @brief Polling delay in microseconds
 * @details Time between busy-wait checks
 */
#define EEPROM_POLL_DELAY_US         100

/**
 * @brief Maximum polling iterations
 */
#define EEPROM_MAX_ITERATIONS        ((EEPROM_TIMEOUT_MS * 1000) / EEPROM_POLL_DELAY_US)

/******************************************************************************
 * Error Codes
 ******************************************************************************/

typedef enum
{
    EEPROM_OK = 0,              // Success
    EEPROM_TIMEOUT_ERROR,       // Timeout waiting for write
    EEPROM_INVALID_ADDRESS,     // Address out of range
    EEPROM_OVERFLOW_ERROR,      // Block write would overflow
    EEPROM_NULL_POINTER         // Null data pointer
} EEPROM_Error_t;
```

---

### Step 2: Fix WriteByte Function (2 hours)

**File**: `Mcal/EEPROM/mEEPROM_Program.c`

**Before (BUGGY CODE):**

```c
void mEEPROM_WriteByte(uint16_t Address, uint8_t Data)
{
    if (Address > AVR_EEPROM_MAXAddress)
    {
        return;
    }

    // ❌ Can hang forever!
    while (GetBit(EECR_Reg, EEWE_Bit) == 1)
    {
        // Wait forever - NO TIMEOUT!
    }

    EEAR_Reg = Address;
    EEDR_Reg = Data;
    SetBit(EECR_Reg, EEMWE_Bit);
    SetBit(EECR_Reg, EEWE_Bit);
}
```

**After (FIXED CODE):**

```c
/**
 * @brief Write single byte to EEPROM with timeout protection
 * @param Address EEPROM address (0 to 1023 for ATmega32)
 * @param Data Byte to write
 * @return EEPROM_Error_t error code
 */
EEPROM_Error_t mEEPROM_WriteByte(uint16_t Address, uint8_t Data)
{
    uint16_t timeout_counter = 0;

    // Validate address
    if (Address > AVR_EEPROM_MAXAddress)
    {
        return EEPROM_INVALID_ADDRESS;
    }

    // ✅ Wait with timeout protection
    while (GetBit(EECR_Reg, EEWE_Bit) == 1)
    {
        // Small delay
        _delay_us(EEPROM_POLL_DELAY_US);
        timeout_counter++;

        // ✅ Check timeout
        if (timeout_counter >= EEPROM_MAX_ITERATIONS)
        {
            // EEPROM not responding!
            return EEPROM_TIMEOUT_ERROR;
        }
    }

    // Proceed with write
    EEAR_Reg = Address;
    EEDR_Reg = Data;

    // Atomic write enable sequence
    SetBit(EECR_Reg, EEMWE_Bit);
    SetBit(EECR_Reg, EEWE_Bit);

    return EEPROM_OK;
}
```

---

### Step 3: Fix Block Write Function (2 hours)

**File**: `Mcal/EEPROM/mEEPROM_Program.c`

**Before (BUGGY CODE):**

```c
void mEEPROM_Write(uint16_t Address, const uint8_t* data, uint16_t length)
{
    if (data != Null)
    {
        for (uint16_t index = 0; index < length; index++)
        {
            // ❌ No overflow check!
            // ❌ No error handling from WriteByte!
            mEEPROM_WriteByte(Address + index, data[index]);
        }
    }
}
```

**After (FIXED CODE):**

```c
/**
 * @brief Write block of data to EEPROM with safety checks
 * @param Address Starting EEPROM address
 * @param data Pointer to data to write
 * @param length Number of bytes to write
 * @return EEPROM_Error_t error code
 */
EEPROM_Error_t mEEPROM_Write(uint16_t Address, const uint8_t* data, uint16_t length)
{
    EEPROM_Error_t result;

    // Validate pointer
    if (data == Null)
    {
        return EEPROM_NULL_POINTER;
    }

    // ✅ Check for overflow
    if ((Address + length) > (AVR_EEPROM_MAXAddress + 1))
    {
        return EEPROM_OVERFLOW_ERROR;
    }

    // Write each byte
    for (uint16_t index = 0; index < length; index++)
    {
        result = mEEPROM_WriteByte(Address + index, data[index]);

        // ✅ Propagate error if timeout occurs
        if (result != EEPROM_OK)
        {
            return result;
        }
    }

    return EEPROM_OK;
}
```

---

### Step 4: Update Interface Header (1 hour)

**File**: `Mcal/EEPROM/mEEPROM_Interface.h`

**Update function prototypes:**

```c
// Change return types from void to EEPROM_Error_t
EEPROM_Error_t mEEPROM_WriteByte(uint16_t Address, uint8_t Data);
EEPROM_Error_t mEEPROM_Write(uint16_t Address, const uint8_t* data, uint16_t length);

// ReadByte can also benefit from timeout (optional)
EEPROM_Error_t mEEPROM_ReadByte(uint16_t Address, uint8_t* Data);
```

---

### Step 5: Update All Callers (Optional but Recommended)

**Files that use EEPROM:**

- Energy persistence code
- Configuration storage

**Update pattern:**

```c
// Before:
mEEPROM_WriteByte(address, value);

// After:
EEPROM_Error_t result = mEEPROM_WriteByte(address, value);
if (result != EEPROM_OK)
{
    // Handle error (log, retry, use defaults, etc.)
    #ifdef DEBUG_MODE
        UART_SendString("[ERROR] EEPROM write failed\r\n");
    #endif
}
```

---

## Testing and Verification

### Test 1: Normal Operation

```
Procedure:
1. Write to EEPROM at various addresses
2. Read back and verify
3. Measure write time

Expected Result:
✅ All writes succeed (EEPROM_OK)
✅ Write time < 10ms (timeout not triggered)
✅ Data correctly stored
```

### Test 2: Timeout Simulation

```
Procedure:
1. Modify code to force EEWE bit stuck (or disconnect EEPROM hardware)
2. Attempt write
3. Measure timeout

Expected Result:
✅ Function returns EEPROM_TIMEOUT_ERROR
✅ Timeout occurs after ~10ms
✅ System does NOT freeze
```

### Test 3: Overflow Protection

```
Procedure:
1. Attempt to write beyond EEPROM bounds
2. Example: Write 10 bytes starting at address 1020

Expected Result:
✅ Returns EEPROM_OVERFLOW_ERROR
✅ No data written
✅ No system corruption
```

---

## Success Criteria

✅ **EEPROM writes have timeout protection**  
✅ **System never freezes on EEPROM failure**  
✅ **Error codes properly propagated**  
✅ **Block writes check for overflow**  
✅ **All tests pass**

---

## Deliverables

1. ✅ Updated `mEEPROM_Program.c` with timeout logic
2. ✅ Updated `mEEPROM_Config.h` with timeout settings
3. ✅ Updated `mEEPROM_Interface.h` with error codes
4. ✅ Test report showing timeout protection
5. ✅ Git commit

---

## Related Documentation

- `Doc/Embedded/Issue_Solutions.md` - MCAL Layer Issues #2, #3
- ATmega32 Datasheet - EEPROM section

---

**Document Version**: 1.0  
**Created**: 2026-01-16
