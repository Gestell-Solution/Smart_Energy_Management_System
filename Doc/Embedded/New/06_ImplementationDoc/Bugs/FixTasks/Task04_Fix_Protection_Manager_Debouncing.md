# Task 04: Fix Protection Manager Debouncing

<div align="center">

![Status](https://img.shields.io/badge/Status-Planned-lightgrey)
![Priority](https://img.shields.io/badge/Priority-Critical-red)
![Assignee](https://img.shields.io/badge/Assignee-Mohamed_Abdelgaber-blue)
![Timeline](<https://img.shields.io/badge/Timeline-Day_5_(Jan_28)-orange>)

**Application Layer Critical Fix**

</div>

---

## 📋 Task Information

| Attribute        | Details                                                           |
| :--------------- | :---------------------------------------------------------------- |
| **Priority**     | 🔴 **CRITICAL**                                                   |
| **Assignee**     | Mohamed Abdelgaber                                                |
| **Component**    | Application Layer - Protection Manager                            |
| **Bug Type**     | Signal Noise / Inrush Current Handling                            |
| **Impact**       | System trips falsely on motor startup (Inrush Current).           |
| **Dependencies** | 🔗 **Task 01** (Timer1), **Task 02** (Current Readings)           |
| **Blocker**      | 🚀 **YES** - Prevents operation of motors/pumps (False Positive). |
| **Deadline**     | **Wednesday, Jan 28, 2026**                                       |

---

## 🐛 Problem Description

### Current Issue

The Protection Manager **immediately cuts power** on the first high reading, causing **false trips** during normal inrush currents (motor startup, transformer energization).

### Impact

- ❌ **False trips during motor startup** (inrush current)
- ❌ **Nuisance tripping on ADC noise**
- ❌ **Poor user experience** - appliances randomly shut off

### Root Cause

```c
// Current code:
if (current > threshold)  // ❌ Single sample!
{
    CUT_POWER();  // Immediate trip
}
```

---

## 🛠️ Implementation Plan

### Step 1: Design Debouncing Algorithm

**Debounce Strategy:**

```
TRIP Logic:
- Counter increments when value > threshold
- Trips when counter >= TRIP_DEBOUNCE_COUNT (e.g., 100ms)

RESET Logic:
- Counter decrements when value < (threshold - hysteresis)
- Resets when counter == 0 (e.g., 200ms normal)
```

### Step 2: Update Protection Config

**File**: `App/ProtectionManager/ProtectionManager_Config.h`

```c
#define PM_TRIP_DEBOUNCE_COUNT       10  // 100ms
#define PM_RESET_DEBOUNCE_COUNT      20  // 200ms
#define PM_CURRENT_HYSTERESIS        2.0f   // 2A
```

---

## 🧪 Verification Plan

| Test Case         | Procedure                               | Expected Result               |
| :---------------- | :-------------------------------------- | :---------------------------- |
| **Motor Startup** | Start 1HP Motor (Inrush ~15A for 50ms). | System **IGNORES** spike.     |
| **Real Overload** | Sustained 20A Load (>100ms).            | System **TRIPS** after 0.1s.  |
| **Auto-Reset**    | Reduce load below threshold-hysteresis. | System **RESETS** after 0.2s. |

---

<div align="center">
**Gestell Company - Internal Task Document**
</div>
