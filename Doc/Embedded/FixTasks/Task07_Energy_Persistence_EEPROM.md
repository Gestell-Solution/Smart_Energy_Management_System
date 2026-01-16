# Task 07: Implement Energy Persistence in EEPROM

## Task Information

- **Priority**: 🟡 HIGH
- **Assigned to**: Basma Khaled
- **Estimated Time**: 8 hours
- **Category**: Application Layer - Data Persistence
- **Blocker**: No - but important for user experience

---

## Problem Description

### Current Issue

Energy counter **resets to zero** every time the system powers off or resets, losing all accumulated energy data.

### Impact

- ❌ **User loses energy billing data** on power loss
- ❌ **Monthly consumption tracking impossible**
- ❌ **Poor user experience**
- ❌ **System not suitable for metering applications**

### Requirements

1. **Save energy counter to EEPROM** periodically
2. **Restore energy counter** on system startup
3. **Wear leveling** - prevent EEPROM wear-out from frequent writes
4. **Integrity check** - detect corrupted data

---

## Technical Background

### EEPROM Endurance (ATmega32)

- **Write cycles**: 100,000 guaranteed
- **Retention**: 20 years @ 25°C (after write cycles depleted)

### Wear Leveling Strategy

**Problem**: Writing every second would wear out EEPROM in:

```
100,000 cycles / (3600 seconds/hour × 24 hours/day) = 1.16 days ❌
```

**Solution**: Write infrequently + intelligent triggers

```
Write Triggers:
1. Every 0.1 kWh increment (significant change)
2. Every 10 minutes (time-based backup)
3. On system shutdown (if detectable)
4. User manual save request

Expected write frequency: ~6 writes/hour (worst case)
Lifespan: 100,000 / (6 × 24) = 694 days ≈ 2 years ✅
```

### Data Structure

```
EEPROM Layout:
Address 0-3:   Energy value (float32, 4 bytes)
Address 4:     Checksum (XOR of bytes 0-3)
Address 5:     Validity flag (0xAA = valid)

Alternative (redundant storage):
Address 0-5:   Primary copy
Address 6-11:  Backup copy
```

---

## Files to Create/Modify

### New File: `App/DataPersistence/DataPersistence_Program.c`

- Energy save/load functions
- Wear leveling logic
- Checksum verification

### New File: `App/DataPersistence/DataPersistence_Interface.h`

- Public API

### Modified: `main.c`

- Call restore on startup
- Periodic save in main loop

---

## Implementation Steps

### Step 1: Define Data Structure (1 hour)

**File**: `App/DataPersistence/DataPersistence_Config.h`

```c
#ifndef DATA_PERSISTENCE_CONFIG_H
#define DATA_PERSISTENCE_CONFIG_H

/******************************************************************************
 * EEPROM Address Map
 ******************************************************************************/

#define DP_EEPROM_ENERGY_ADDR        0      // Energy value (float32, 4 bytes)
#define DP_EEPROM_CHECKSUM_ADDR      4      // Checksum (1 byte)
#define DP_EEPROM_VALIDITY_ADDR      5      // Validity flag (1 byte)

#define DP_VALIDITY_MAGIC            0xAA   // Valid data marker

/******************************************************************************
 * Wear Leveling Configuration
 ******************************************************************************/

/**
 * @brief Minimum energy increment to trigger save (kWh)
 * @note Prevents excessive writes for small changes
 */
#define DP_MIN_SAVE_INCREMENT_KWH    0.1f   // Save every 0.1 kWh

/**
 * @brief Periodic save interval (seconds)
 * @note Backup save even if energy hasn't changed much
 *       600 seconds = 10 minutes
 */
#define DP_PERIODIC_SAVE_INTERVAL_S  600

/**
 * @brief Enable redundant backup copy
 */
#define DP_USE_BACKUP_COPY           1

#if DP_USE_BACKUP_COPY
    #define DP_EEPROM_BACKUP_ADDR    10     // Backup starts at address 10
#endif

#endif
```

---

### Step 2: Implement Save Function (2 hours)

**File**: `App/DataPersistence/DataPersistence_Program.c`

```c
#include "DataPersistence_Interface.h"
#include "DataPersistence_Config.h"
#include "mEEPROM_Interface.h"
#include "MeasurementEngine_Interface.h"

/******************************************************************************
 * Private Functions
 ******************************************************************************/

/**
 * @brief Calculate checksum for data integrity
 * @param data Data bytes
 * @param length Number of bytes
 * @return XOR checksum
 */
static uint8_t DP_CalculateChecksum(uint8_t* data, uint8_t length)
{
    uint8_t checksum = 0;

    for (uint8_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

/**
 * @brief Convert float to byte array
 */
static void DP_FloatToBytes(float value, uint8_t* bytes)
{
    union
    {
        float f;
        uint8_t b[4];
    } converter;

    converter.f = value;

    for (uint8_t i = 0; i < 4; i++)
    {
        bytes[i] = converter.b[i];
    }
}

/**
 * @brief Convert byte array to float
 */
static float DP_BytesToFloat(uint8_t* bytes)
{
    union
    {
        float f;
        uint8_t b[4];
    } converter;

    for (uint8_t i = 0; i < 4; i++)
    {
        converter.b[i] = bytes[i];
    }

    return converter.f;
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/

/**
 * @brief Save energy value to EEPROM with integrity check
 * @return 1 if successful, 0 if error
 */
uint8_t DP_SaveEnergy(void)
{
    uint8_t energy_bytes[4];
    uint8_t checksum;
    EEPROM_Error_t result;

    // Get current energy
    float energy_kwh = ME_GetEnergy_kWh();

    // Convert to bytes
    DP_FloatToBytes(energy_kwh, energy_bytes);

    // Calculate checksum
    checksum = DP_CalculateChecksum(energy_bytes, 4);

    // Write energy value (4 bytes)
    result = mEEPROM_Write(DP_EEPROM_ENERGY_ADDR, energy_bytes, 4);
    if (result != EEPROM_OK)
    {
        return 0;  // Write failed
    }

    // Write checksum
    result = mEEPROM_WriteByte(DP_EEPROM_CHECKSUM_ADDR, checksum);
    if (result != EEPROM_OK)
    {
        return 0;
    }

    // Write validity flag
    result = mEEPROM_WriteByte(DP_EEPROM_VALIDITY_ADDR, DP_VALIDITY_MAGIC);
    if (result != EEPROM_OK)
    {
        return 0;
    }

    #if DP_USE_BACKUP_COPY
        // Write backup copy
        mEEPROM_Write(DP_EEPROM_BACKUP_ADDR, energy_bytes, 4);
        mEEPROM_WriteByte(DP_EEPROM_BACKUP_ADDR + 4, checksum);
        mEEPROM_WriteByte(DP_EEPROM_BACKUP_ADDR + 5, DP_VALIDITY_MAGIC);
    #endif

    #ifdef DEBUG_MODE
        char debug[60];
        sprintf(debug, "[DP] Saved energy: %.2f kWh\r\n", energy_kwh);
        UART_SendString(debug);
    #endif

    return 1;  // Success
}

/**
 * @brief Restore energy value from EEPROM
 * @return 1 if successful, 0 if no valid data found
 */
uint8_t DP_RestoreEnergy(void)
{
    uint8_t energy_bytes[4];
    uint8_t stored_checksum;
    uint8_t calculated_checksum;
    uint8_t validity_flag;
    uint8_t result_byte;

    // Read validity flag
    mEEPROM_ReadByte(DP_EEPROM_VALIDITY_ADDR, &validity_flag);

    if (validity_flag != DP_VALIDITY_MAGIC)
    {
        #ifdef DEBUG_MODE
            UART_SendString("[DP] No valid data found, starting fresh\r\n");
        #endif
        return 0;  // No valid data
    }

    // Read energy bytes
    for (uint8_t i = 0; i < 4; i++)
    {
        mEEPROM_ReadByte(DP_EEPROM_ENERGY_ADDR + i, &result_byte);
        energy_bytes[i] = result_byte;
    }

    // Read stored checksum
    mEEPROM_ReadByte(DP_EEPROM_CHECKSUM_ADDR, &stored_checksum);

    // Calculate checksum from read data
    calculated_checksum = DP_CalculateChecksum(energy_bytes, 4);

    // Verify integrity
    if (calculated_checksum != stored_checksum)
    {
        #ifdef DEBUG_MODE
            UART_SendString("[DP] Checksum mismatch, data corrupted\r\n");
        #endif

        #if DP_USE_BACKUP_COPY
            // Try backup copy
            return DP_RestoreFromBackup();
        #else
            return 0;  // Corrupted, no backup
        #endif
    }

    // Convert bytes to float
    float energy_kwh = DP_BytesToFloat(energy_bytes);

    // Sanity check (energy shouldn't be negative or impossibly large)
    if (energy_kwh < 0.0f || energy_kwh > 99999.0f)
    {
        #ifdef DEBUG_MODE
            UART_SendString("[DP] Energy value out of range\r\n");
        #endif
        return 0;
    }

    // Restore to Measurement Engine
    ME_SetEnergy_kWh(energy_kwh);

    #ifdef DEBUG_MODE
        char debug[60];
        sprintf(debug, "[DP] Restored energy: %.2f kWh\r\n", energy_kwh);
        UART_SendString(debug);
    #endif

    return 1;  // Success
}
```

---

### Step 3: Implement Wear Leveling Logic (2 hours)

**File**: `App/DataPersistence/DataPersistence_Program.c`

```c
/******************************************************************************
 * Wear Leveling
 ******************************************************************************/

static float DP_Last_Saved_Energy = 0.0f;
static uint32_t DP_Last_Save_Time = 0;  // Timestamp in seconds

/**
 * @brief Check if energy should be saved (wear leveling)
 * @param current_time System uptime in seconds
 * @return 1 if should save, 0 otherwise
 */
uint8_t DP_ShouldSave(uint32_t current_time)
{
    float current_energy = ME_GetEnergy_kWh();
    float energy_change = current_energy - DP_Last_Saved_Energy;

    // Check if significant increment
    if (energy_change >= DP_MIN_SAVE_INCREMENT_KWH)
    {
        return 1;  // Significant change
    }

    // Check if periodic interval elapsed
    uint32_t time_since_save = current_time - DP_Last_Save_Time;
    if (time_since_save >= DP_PERIODIC_SAVE_INTERVAL_S)
    {
        return 1;  // Periodic backup
    }

    return 0;  // No need to save yet
}

/**
 * @brief Periodic update function (call from main loop)
 * @param current_time System uptime in seconds
 */
void DP_Update(uint32_t current_time)
{
    if (DP_ShouldSave(current_time))
    {
        if (DP_SaveEnergy())
        {
            // Update tracking variables
            DP_Last_Saved_Energy = ME_GetEnergy_kWh();
            DP_Last_Save_Time = current_time;
        }
    }
}

/**
 * @brief Force immediate save (user request or shutdown)
 */
void DP_ForceSave(void)
{
    if (DP_SaveEnergy())
    {
        DP_Last_Saved_Energy = ME_GetEnergy_kWh();
        // Don't update time - allow periodic saves to continue
    }
}
```

---

### Step 4: Integration with main.c (1 hour)

**File**: `main.c`

```c
#include "DataPersistence_Interface.h"

int main(void)
{
    // Initialize peripherals
    System_Init();

    // ✅ Restore energy from EEPROM
    DP_RestoreEnergy();

    // Main loop
    while(1)
    {
        // Get current uptime (implement timer-based uptime counter)
        uint32_t uptime_seconds = GetSystemUptime();

        // ✅ Periodic EEPROM save check
        DP_Update(uptime_seconds);

        // Rest of main loop...
    }
}
```

---

### Step 5: Add ME_SetEnergy Function (1 hour)

**File**: `App/MeasurementEngine/MeasurementEngine_Program.c`

```c
/**
 * @brief Set energy counter value (for restoring from EEPROM)
 * @param energy_kwh Energy value in kWh
 */
void ME_SetEnergy_kWh(float energy_kwh)
{
    ME_Energy_kWh = energy_kwh;
}
```

**Add to Interface:**

```c
void ME_SetEnergy_kWh(float energy_kwh);
```

---

### Step 6: Testing (1 hour)

**Test 1: Save and Restore**

```
Procedure:
1. Accumulate energy (e.g., run load for 1 hour → 1 kWh)
2. Force save: DP_ForceSave()
3. Power cycle system (or reset)
4. Check if energy restored correctly

Expected Result:
✅ Energy counter starts at saved value (1.0 kWh)
```

**Test 2: Wear Leveling**

```
Procedure:
1. Monitor EEPROM writes over time
2. Verify writes occur only on:
   - 0.1 kWh increments
   - 10-minute intervals

Expected Result:
✅ Not writing every second
```

**Test 3: Corruption Handling**

```
Procedure:
1. Manually corrupt EEPROM data
2. Reset system

Expected Result:
✅ Detects corrupted data
✅ Starts fresh or uses backup
```

---

## Success Criteria

✅ **Energy persists across power cycles**  
✅ **Wear leveling prevents excessive writes**  
✅ **Checksum detects corrupted data**  
✅ **Restores correctly on startup**

---

## Deliverables

1. ✅ `DataPersistence_Program.c`
2. ✅ `DataPersistence_Interface.h`
3. ✅ `DataPersistence_Config.h`
4. ✅ Integration with main.c
5. ✅ Test report
6. ✅ Git commit

---

## Related Documentation

- Task 06 (EEPROM Timeout) - Prerequisite

---

**Document Version**: 1.0  
**Created**: 2026-01-16
