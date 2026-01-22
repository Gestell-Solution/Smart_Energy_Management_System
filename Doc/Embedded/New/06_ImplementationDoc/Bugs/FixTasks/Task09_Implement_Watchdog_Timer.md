# Task 09: Implement Watchdog Timer (WDT)

<div align="center">

![Status](https://img.shields.io/badge/Status-Todo-grey)
![Priority](https://img.shields.io/badge/Priority-High-yellow)
![Assignee](https://img.shields.io/badge/Assigned-Hesham_Ahmed-blue)
![Timeline](<https://img.shields.io/badge/Timeline-Day_1_to_2_(Jan_24_25)-orange>)

**Safety Feature Implementation**

</div>

---

## 📋 Task Information

| Attribute        | Details                                                                   |
| :--------------- | :------------------------------------------------------------------------ |
| **Priority**     | 🟡 **HIGH**                                                               |
| **Assignee**     | Hesham Ahmed                                                              |
| **Component**    | MCAL Layer - Watchdog Timer (WDT)                                         |
| **Bug Type**     | Missing Feature / System Resilience                                       |
| **Impact**       | System cannot recover from latch-ups or infinite loops (requires reboot). |
| **Dependencies** | 🏁 **None** (Independent Hardware Feature)                                |
| **Blocker**      | ❌ **NO** - But essential for 24/7 reliability.                           |
| **Deadline**     | **Sunday, Jan 25, 2026**                                                  |

---

## 🐛 Problem Description

### Current Issue

The system currently **lacks a mechanism to recover from hangs**. If the software enters an infinite loop or if EMI triggers a latch-up, the device remains unresponsive until manually power-cycled.

### Impact

- ❌ **System Freeze**: User must physically unplug device.
- ❌ **Reliability**: Not suitable for 24/7 Energy Management.

---

## 🔍 Visual Analysis

```mermaid
sequenceDiagram
    participant MainLoop
    participant WDT_Hardware

    Note over MainLoop, WDT_Hardware: Normal Operation
    MainLoop->>WDT_Hardware: Enable WDT (2.1s)
    loop Every Cycle
        MainLoop->>MainLoop: Execute Tasks
        MainLoop->>WDT_Hardware: Refresh (Kick)
    end

    Note over MainLoop, WDT_Hardware: System Hang Scenario
    MainLoop->>MainLoop: Enters Infinite Loop (Hang)
    MainLoop-xWDT_Hardware: NO Refresh!
    Note right of WDT_Hardware: Timer Counts Up...
    WDT_Hardware->>WDT_Hardware: Timeout Reached (2.1s)
    WDT_Hardware->>MainLoop: SYSTEM RESET
    Note over MainLoop: System Reboots & Recovers
```

---

## 🛠️ Implementation Plan

### Step 1: Create WDT Driver (MCAL)

**File**: `Mcal/WDT/WDT_Program.c`

Implement `WDT_Enable()`, `WDT_Disable()`, and `WDT_Refresh()` using `WDTCR` register.

### Step 2: Integrate into System Controller

**File**: `App/SystemController/SystemController.c`

Enable WDT at startup. Call `WDT_Refresh()` at end of `System_Loop()`.

---

## ✅ Verification Plan

### Test Case 9.1: Reset Verification

1. Comment out `WDT_Refresh()`.
2. Confirm system reboots every 2.1 seconds.

---

<div align="center">
**Gestell Company - Internal Task Document**
</div>
