# Task 07: Implement Energy Persistence in EEPROM

<div align="center">

![Status](https://img.shields.io/badge/Status-Planned-lightgrey)
![Priority](https://img.shields.io/badge/Priority-High-yellow)
![Assignee](https://img.shields.io/badge/Assignee-Basma_Khaled-blue)
![Timeline](<https://img.shields.io/badge/Timeline-Day_5_(Jan_28)-orange>)

**Application Layer Feature**

</div>

---

## 📋 Task Information

| Attribute        | Details                                                       |
| :--------------- | :------------------------------------------------------------ |
| **Priority**     | 🟡 **HIGH**                                                   |
| **Assignee**     | Basma Khaled                                                  |
| **Component**    | Application Layer - Data Persistence Manager                  |
| **Bug Type**     | Missing Feature / Volatile Memory Data Loss                   |
| **Impact**       | Total energy consumption is lost on every power cycle.        |
| **Dependencies** | 🔗 **Task 06** (EEPROM Driver), **Task 03** (Energy Accuracy) |
| **Blocker**      | ❌ **NO** - Only affects billing feature continuity.          |
| **Deadline**     | **Wednesday, Jan 28, 2026**                                   |

---

## 🐛 Problem Description

### Current Issue

Energy counter **resets to zero** every time the system powers off or resets, losing all accumulated energy data.

### Impact

- ❌ **User loses energy billing data** on power loss.
- ❌ **Monitoring impossible** over long periods.

---

## 🛠️ Implementation Plan

### Strategy

1.  **Wear Leveling**: Only save when Energy increases by **0.1 kWh** OR every **10 minutes**.
2.  **Integrity**: Use a **CRC/Checksum** to verify data validity on startup.
3.  **Backup**: Store two copies (Address 0 and Address 10) for redundancy.

### New Module

Create `App/DataPersistence/DataPersistence_Program.c` to handle the logic, calling the MCAL EEPROM driver.

---

## 🧪 Verification Plan

| Test Case         | Procedure                                   | Expected Result              |
| :---------------- | :------------------------------------------ | :--------------------------- |
| **Power Cycle**   | Accumulate 1.5kWh -> Power OFF -> Power ON. | Reads 1.5kWh (not 0).        |
| **Wear Leveling** | Monitor Writes for 1 hour.                  | Writes <= 6 times.           |
| **Corruption**    | Corrupt EEPROM Address 0.                   | Detects error, loads Backup. |

---

<div align="center">
**Gestell Company - Internal Task Document**
</div>
