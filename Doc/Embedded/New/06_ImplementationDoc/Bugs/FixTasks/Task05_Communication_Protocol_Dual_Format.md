# Task 05: Implement Different Data Formats for Mobile and Dashboard

<div align="center">

![Status](https://img.shields.io/badge/Status-Planned-lightgrey)
![Priority](https://img.shields.io/badge/Priority-Critical-red)
![Assignee](https://img.shields.io/badge/Assignee-Mohamed_Diaa-blue)
![Timeline](<https://img.shields.io/badge/Timeline-Day_3_4_(Jan_26_27)-yellow>)

**Communication Layer Enhancement**

</div>

---

## 📋 Task Information

| Attribute        | Details                                                         |
| :--------------- | :-------------------------------------------------------------- |
| **Priority**     | 🔴 **CRITICAL**                                                 |
| **Assignee**     | Mohamed Diaa                                                    |
| **Component**    | Application Layer - Communication Manager                       |
| **Bug Type**     | Protocol Mismatch (Endpoints expect different Endianness)       |
| **Impact**       | Mobile and Dashboard display garbage or wrong values.           |
| **Dependencies** | 🔗 **Task 03** (Real Power/Energy Data Validity)                |
| **Blocker**      | 🚀 **YES** - Prevents correct integration with Frontend/Mobile. |
| **Deadline**     | **Tuesday, Jan 27, 2026**                                       |

---

## 🐛 Problem Description

### Current Issue

The **Mobile App** and **Dashboard** expect **different data formats** for the same measurement data, but the embedded system currently has NO code to handle these different formats.

- **Mobile App**: Expects **Integer** (Big Endian)
- **Dashboard**: Expects **Float32** (Little Endian)

### Impact

- ❌ **Mobile App cannot parse data correctly**
- ❌ **Dashboard receives wrong format**
- ❌ **One platform or both will display incorrect values**

---

## 🛠️ Implementation Plan

### Step 1: Define Protocol Configuration

**File**: `App/CommunicationManager/CM_Config.h`

```c
#define CMD_GET_RMS_DATA_MOBILE      0x05    // Returns 10 bytes (uint16/uint32)
#define CMD_GET_RMS_DATA_DASHBOARD   0x15    // Returns 16 bytes (float32)
```

### Step 2: Implement Byte Conversion

Implement `CM_Uint16_ToBigEndian` and `CM_Float32_ToLittleEndian`.

### Step 3: Implement Command Handler

Add switch case in `CM_HandleCommand` to route 0x05 and 0x15 commands to respective formatter functions.

---

## 🧪 Verification Plan

| Test Case         | Procedure              | Expected Result                                   |
| :---------------- | :--------------------- | :------------------------------------------------ |
| **Mobile Req**    | Send `0xAA 0x00 0x05`. | Recv `[0xAA 0x0B 0x05 ...BIG_ENDIAN_INT...]`      |
| **Dashboard Req** | Send `0xAA 0x00 0x15`. | Recv `[0xAA 0x11 0x15 ...LITTLE_ENDIAN_FLOAT...]` |

---

<div align="center">
**Gestell Company - Internal Task Document**
</div>
