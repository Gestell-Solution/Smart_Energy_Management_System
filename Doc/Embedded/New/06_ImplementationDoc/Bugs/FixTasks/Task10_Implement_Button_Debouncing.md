# Task 10: Implement Button Debouncing

<div align="center">

![Status](https://img.shields.io/badge/Status-Todo-grey)
![Priority](https://img.shields.io/badge/Priority-Medium-yellow)
![Assignee](https://img.shields.io/badge/Assigned-Mohamed_Abdelgaber-blue)
![Timeline](<https://img.shields.io/badge/Timeline-Day_5_(Jan_28)-orange>)

**User Input Reliability Optimization**

</div>

---

## 📋 Task Information

| Attribute        | Details                                                      |
| :--------------- | :----------------------------------------------------------- |
| **Priority**     | ⚠️ **MEDIUM**                                                |
| **Assignee**     | Mohamed Abdelgaber                                           |
| **Component**    | HAL Layer - Button Driver                                    |
| **Bug Type**     | Signal Noise / Mechanical Bounce                             |
| **Impact**       | Menu navigation double-clicks or skips screens unexpectedly. |
| **Dependencies** | 🏁 **None** (HAL Driver Issue)                               |
| **Blocker**      | ❌ **NO** - Annoyance, but functional.                       |
| **Deadline**     | **Wednesday, Jan 28, 2026**                                  |

---

## 🐛 Problem Description

### Current Issue

Mechanical buttons (Mode, Reset, Config) generate **noise** (bounce) when pressed. The microcontroller interprets these <5ms spikes as multiple presses, causing the menu to skip items or settings to toggle rapidly.

### Impact

- ❌ **Poor UX**: Menu navigation is jumpy.
- ❌ **Accidental Actions**: Double-clicks might trigger wrong functions.

---

## 🔍 Visual Analysis

```mermaid
graph TD
    A[Raw Button Press] --> B{Is Input High?}
    B -- Yes --> C[Wait 50ms (Debounce)]
    C --> D{Is Input STILL High?}
    D -- Yes --> E[CONFIRMED PRESS]
    D -- No --> F[Ignore (Noise)]
    B -- No --> G[No Action]

    style E fill:#9f9,stroke:#333
    style F fill:#f9f,stroke:#333
```

---

## 🛠️ Implementation Plan

### Update Button Driver

**File**: `Hal/Button/hButton_Program.c`

Use a **double-check method** with a delay or a **state counter** to verify stability of the input signal before returning `BTN_PRESSED`.

---

## ✅ Verification Plan

### Test Case 10.1: Menu Navigation

1. Enter Config Menu.
2. Press "Next" button 20 times rapidly.
3. Verify menu skips match physical clicks exactly.

---

<div align="center">
**Gestell Company - Internal Task Document**
</div>
