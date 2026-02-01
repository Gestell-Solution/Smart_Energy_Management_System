# Code vs Documentation Comparison and Fix Plan

**Date:** January 31, 2026  
**Project:** Smart Energy Management System  
**Sources:** SRS, HLD, LLD, Memory_Map, MobileRequirementfromEmbedded, MOBILE_APP_COMM_PROTOCOL, FixTasks

---

## 1. Gap Analysis Summary

### 1.1 Critical

| # | Issue | Documentation | Current Code | Impact |
|---|-------|---------------|--------------|--------|
| C1 | **GET_RMS_DATA: Energy size** | SRS REQ-COMM-006, MobileRequirement: Energy = **4 bytes** (uint32) ×100 | Code sends **2 bytes** (uint16) | Mobile app parses incorrect data (Parse Error) |
| C2 | **GET_RMS_DATA: Payload length** | Docs: 10 bytes (V:2 + I:2 + P:2 + E:**4**) | Code: 8 bytes (E:2) | Mobile expects 10 bytes |
| C3 | **Current Sensor: ADC divisor** | Task08: Use 1024 instead of 1023 | `hCurrent_Config.h`: `ADC_MAX 1023.0` | ~0.1% error in current reading |

### 1.2 High

| # | Issue | Documentation | Current Code | Impact |
|---|-------|---------------|--------------|--------|
| H1 | **Save g_SystemData to EEPROM** | SRS REQ-EEPROM-003: Energy saved **every minute** or on change >0.1 kWh | Saved only on Overload or Store_In__EEPROM | Energy data loss on power outage |
| H2 | **EEPROM: Timeout + Verification** | SRS REQ-EEPROM-005: Timeout 5ms, Read-back, Retry 3 times | Not implemented | System may hang on EEPROM failure |
| H3 | **Energy Logger: State recovery** | Previous analysis: Save EEPROM_head and EEPROM_count | Init resets them to 0 | Log ordering lost after Power Cycle |
| H4 | **Dashboard Protocol (0x15)** | SRS REQ-COMM-019, Task05: Float32 Little-Endian for Dashboard | Not implemented | Dashboard will not work if present |

### 1.3 Medium

| # | Issue | Documentation | Current Code | Impact |
|---|-------|---------------|--------------|--------|
| M1 | **EEPROM Memory Map** | SRS/LLD: Energy float @ 0x00, separate fields | Code: g_SystemData (full struct) @ 0x00 | Layout differs from docs |
| M2 | **Relay Default State** | SRS REQ-PROT-011: Relay **OFF** on startup | PM_Init: relays **ON** (Fix-007) | Violates safety spec |
| M3 | **Command ID: RESET_ENERGY** | SRS REQ-COMM-015: 0x0E | Code: 0x04 Store_In__EEPROM | Mobile uses 0x04 - compatible |
| M4 | **Timer1 Duplicate ISR** | Task01 | Not verified | May cause sample rate error |
| M5 | **Watchdog Timer** | SRS REQ-SAFE-007 (optional) | Not implemented | No hang protection |

### 1.4 Low

| # | Issue | Documentation | Current Code |
|---|-------|---------------|--------------|
| L1 | Wear Leveling for Energy | Max 1 write/minute | Energy Logger writes every ~10 samples |
| L2 | Power Factor from EEPROM | SRS: PF stored in EEPROM, configurable | Fixed in MeasurementEngine_Config |
| L3 | Buzzer Mute Option | SRS | Not implemented |

---

## 2. Fix Plan

### Phase 1: Critical (Week 1)

#### 1.1 Fix GET_RMS_DATA – Energy 4 bytes

**File:** `Firmware/Embedded/Src/App/CommunicationManager/App_CommManager.c`

**Changes:**
- `GET_RMS_DATA_PAYLOAD_LEN` from 8 to **10**
- Energy: from `uint16_t e` to `uint32_t e`
- Add 4 bytes for Energy: `payload[6..9]` big-endian

```c
#define GET_RMS_DATA_PAYLOAD_LEN  10  /* was 8 */

case GET_RMS_DATA:
    uint8_t payload[10];
    uint16_t v = (uint16_t)(ME_GetVoltageRMS() * 10.0f);
    uint16_t i = (uint16_t)(ME_GetCurrentRMS() * 100.0f);
    uint16_t p = (uint16_t)(ME_GetActivePower() * 10.0f);
    float e_kwh = ME_GetEnergy() / 3600000.0f;
    uint32_t e = (uint32_t)(e_kwh * 100.0f);  /* uint32 instead of uint16 */
    payload[0] = (uint8_t)(v >> 8); payload[1] = (uint8_t)(v & 0xFF);
    payload[2] = (uint8_t)(i >> 8); payload[3] = (uint8_t)(i & 0xFF);
    payload[4] = (uint8_t)(p >> 8); payload[5] = (uint8_t)(p & 0xFF);
    payload[6] = (uint8_t)(e >> 24); payload[7] = (uint8_t)(e >> 16);
    payload[8] = (uint8_t)(e >> 8);  payload[9] = (uint8_t)(e & 0xFF);
```

**Verification:** Send GET_RMS_DATA request from mobile and confirm energy displays correctly.

---

#### 1.2 Fix Current ADC Divisor

**File:** `Firmware/Embedded/Src/Hal/ACS712CurntSnsr/hCurrent_Config.h` and `hCurrent_Private.h`

**Change:** `ADC_MAX 1023.0` → `ADC_MAX 1024.0`

---

### Phase 2: High (Week 2)

#### 2.1 Periodic save of g_SystemData

**File:** `Firmware/Embedded/Src/App/System_Controller/System_Controller_Program.c`

**Changes:**
- Add minute counter (e.g. static)
- Every 60 cycles (at 100ms) or on energy change >0.1 kWh: call `SystemData_SaveToEEPROM()`
- Limit: max one write per minute (wear leveling)

---

#### 2.2 EEPROM Timeout + Verification (Task06)

**File:** `Firmware/Embedded/Src/Mcal/EEPROM/EEPROM_Program.c`

**Changes:**
- Add ~5ms timeout per write operation
- Read-back after write and compare
- Retry up to 3 times on failure

---

#### 2.3 Energy Logger: Persist EEPROM_head and EEPROM_count

**File:** `EnergyLogger_Program.c`

**Changes:**
- Use a fixed address (e.g. 0x15) to store head and count
- In Init: read values from EEPROM if present
- After each StoreToEEPROM: update and save head/count

---

### Phase 3: Medium (Week 3)

#### 3.1 Relay Default OFF (optional)

**File:** `ProtectionManager__Program.c` in `PM_Init`

**Change:** Replace `hRelay_On` with `hRelay_Off` at startup, or add config option.

---

#### 3.2 Dashboard Protocol (0x15)

**File:** `App_CommManager.c`

**Change:** Add `case 0x15` that sends V, I, P, E as float32 Little-Endian (16 bytes).

---

#### 3.3 Verify Timer1 ISR (Task01)

**Files:** Timer1, ADC, ProtectionManager

**Action:** Search for duplicate ISR definition or call and fix.

---

### Phase 4: Documentation and cleanup (Week 4)

- Update Memory_Map.md to match actual code
- Document current EEPROM layout (SystemData @ 0x00, Logs @ 0x100)
- Update SRS if needed (e.g. RESET_ENERGY 0x04 instead of 0x0E)

---

## 3. Execution Priorities

| Priority | Task | Estimated Effort |
|----------|------|------------------|
| 1 | C1+C2: GET_RMS_DATA Energy 4 bytes | 2 hours |
| 2 | C3: Current ADC 1024 | 0.5 hours |
| 3 | H1: Periodic g_SystemData save | 4 hours |
| 4 | H2: EEPROM Timeout | 4 hours |
| 5 | H3: Energy Logger state persistence | 4 hours |
| 6 | H4: Dashboard 0x15 | 3 hours |
| 7 | M1-M5 | As needed |

---

## 4. Current Compliance Summary

| Document | Compliance | Notes |
|----------|------------|-------|
| SRS | ~75% | Differences in EEPROM, Comm, Relay default |
| HLD | ~90% | Layer architecture aligned |
| LLD | ~80% | Measurement algorithms aligned, EEPROM differs |
| Memory_Map | ~60% | Layout differs from docs |
| MobileRequirement | ~85% | Energy 4 bytes not implemented |
| MOBILE_APP_COMM_PROTOCOL | ~90% | Same Energy note |
| FixTasks | ~40% | Some tasks done, some not |

---

**This document was prepared by analyzing the code and documentation.**  
**Last updated:** January 31, 2026
