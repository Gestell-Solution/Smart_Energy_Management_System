# Task 06: Add EEPROM Timeout Protection

<div align="center">

![Status](https://img.shields.io/badge/Status-Planned-lightgrey)
![Priority](https://img.shields.io/badge/Priority-Medium-yellow)
![Assignee](https://img.shields.io/badge/Assignee-Ahmed_Ashraf-blue)
![Timeline](<https://img.shields.io/badge/Timeline-Day_5_(Jan_28)-orange>)

**MCAL Layer Safety Fix**

</div>

---

## 📋 Task Information

| Attribute        | Details                                                        |
| :--------------- | :------------------------------------------------------------- |
| **Priority**     | ⚠️ **MEDIUM**                                                  |
| **Assignee**     | Ahmed Ashraf                                                   |
| **Component**    | MCAL Layer - EEPROM Driver                                     |
| **Bug Type**     | Infinite Loop / Lack of Error Handling                         |
| **Impact**       | System hangs (Freezes) if EEPROM hardware fails to respond.    |
| **Dependencies** | 🏁 **None** (Driver Stability Issue)                           |
| **Blocker**      | ❌ **NO** - But highly recommended for industrial reliability. |
| **Deadline**     | **Wednesday, Jan 28, 2026**                                    |

---

## 🐛 Problem Description

### Current Issue

The EEPROM driver can **hang indefinitely** if hardware fails during write/read operations, causing complete system freeze.

### Root Cause

```c
// Current code:
while (GetBit(EECR_Reg, EEWE_Bit) == 1)
{
    // ❌ Infinite loop - NO TIMEOUT!
}
```

If EEPROM hardware fails to clear the EEWE bit, this loop never exits.

---

## 🛠️ Implementation Plan

### Step 1: Add Timeout Configuration

**File**: `Mcal/EEPROM/mEEPROM_Config.h`

```c
#define EEPROM_TIMEOUT_MS            10
#define EEPROM_POLL_DELAY_US         100
#define EEPROM_MAX_ITERATIONS        ((EEPROM_TIMEOUT_MS * 1000) / EEPROM_POLL_DELAY_US)
```

### Step 2: Fix WriteByte Function

Add a counter in the `while` loop that increments every 100us. If it exceeds 100 iterations (10ms), return `EEPROM_TIMEOUT_ERROR`.

---

## 🧪 Verification Plan

| Test Case        | Procedure                                | Expected Result           |
| :--------------- | :--------------------------------------- | :------------------------ |
| **Normal Write** | Write byte to EEPROM.                    | Success, time < 10ms.     |
| **Timeout Sim**  | Force `EEWE` bit stuck (or remove chip). | Returns Error after 10ms. |

---

<div align="center">
**Gestell Company - Internal Task Document**
</div>
