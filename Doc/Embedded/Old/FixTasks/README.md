# FixTasks - Embedded System Fixes

## Overview

This folder contains detailed task documentation for fixing all embedded system issues in the Smart Energy Management System.

Each task file includes:

- ✅ Problem description and impact
- ✅ Technical background
- ✅ Step-by-step implementation guide
- ✅ Code examples (before/after)
- ✅ Testing procedures
- ✅ Success criteria
- ✅ Deliverables

---

## Task Priority Legend

- 🔴 **CRITICAL**: Blocks system functionality, must fix immediately
- 🟡 **HIGH**: Important for user experience
- ⚠️ **MEDIUM**: Should fix for quality
- ℹ️ **LOW**: Nice to have

---

## Task List

### Phase 1: Critical Fixes

| #  | Task Name                                                                                | Priority    | Assignee           | Est. Time | Status           |
| -- | -----------------------------------------------------------------------------------------| ----------- | ------------------ | --------- | ---------------- |
| 01 | [Fix Timer1 Duplicate ISR](Task01_Fix_Timer1_Duplicate_ISR.md)                           | 🔴 CRITICAL | Mohamed Diaa       | 8 hrs     | ⏸️ Not Started   |
| 02 | [Fix Current Sensor Variable Shadowing](Task02_Fix_Current_Sensor_Variable_Shadowing.md) | 🔴 CRITICAL | Ahmed Twap         | 10 hrs    | ⏸️ Not Started   |
| 03 | [Add Power Factor to Power Calculation](Task03_Add_Power_Factor.md)                      | 🔴 CRITICAL | Ahmed Ashraf       | 12 hrs    | ⏸️ Not Started   |
| 04 | [Fix Protection Manager Debouncing](Task04_Fix_Protection_Manager_Debouncing.md)         | 🔴 CRITICAL | Mohamed Abdelgaber | 10 hrs    | ⏸️ Not Started   |

### Phase 2: High Priority

| #  | Task Name                                                                          | Priority    | Assignee     | Est. Time | Status           |
| -- | -----------------------------------------------------------------------------------| ----------- | ------------ | --------- | ---------------- |
| 05 | [Communication Protocol Dual Format](Task05_Communication_Protocol_Dual_Format.md) | 🔴 CRITICAL | Mohamed Diaa | 10 hrs    | ⏸️ Not Started   |
| 06 | [Add EEPROM Timeout Protection](Task06_Add_EEPROM_Timeout.md)                      | ⚠️ MEDIUM   | Ahmed Ashraf | 6 hrs     | ⏸️ Not Started   |
| 07 | [Energy Persistence in EEPROM](Task07_Energy_Persistence_EEPROM.md)                | 🟡 HIGH     | Basma Khaled | 8 hrs     | ⏸️ Not Started   |
| 08 | [Fix Voltage ADC Divisor](Task08_Fix_Voltage_ADC_Divisor.md)                       | ⚠️ MEDIUM   | Ahmed Ashraf | 2 hrs     | ⏸️ Not Started   |

---

## Dependencies

**Task Dependency Graph:**

```text
Task 01 (Timer1) → Task 02 (Current) → Task 03 (Power Factor) → Task 04 (Protection)
                                           ↓
                                        Task 05 (Communication)

Task 06 (EEPROM Timeout) → Task 07 (Data Persistence)

Task 08 (Voltage) - Independent
```

---

## Team Assignments

### Mohamed Diaa

- Task 01: Timer1 ISR (8 hrs)
- Task 05: Communication Protocol (10 hrs)
- **Total**: 18 hours

### Ahmed Twap

- Task 02: Current Sensor (10 hrs)
- **Total**: 10 hours

### Ahmed Ashraf

- Task 03: Power Factor (12 hrs)
- Task 06: EEPROM Timeout (6 hrs)
- Task 08: Voltage ADC (2 hrs)
- **Total**: 20 hours

### Mohamed Abdelgaber

- Task 04: Protection Manager (10 hrs)
- **Total**: 10 hours

### Basma Khaled

- Task 07: Data Persistence (8 hrs)
- **Total**: 8 hours

---

## Total Effort

**Estimated Total**: 66 hours (~2 weeks with 5 developers)

---

## Getting Started

### For Each Task

1. **Read the task file completely** before starting
2. **Understand the problem** and why it occurs
3. **Follow implementation steps** in order
4. **Test thoroughly** using provided test procedures
5. **Submit deliverables** as listed
6. **Commit with clear message** referencing task number

### Code Review Process

1. Self-review against success criteria
2. Peer review (another team member)
3. Technical review (Hesham Ahmed)
4. Integration testing

---

## Success Metrics

After completing all tasks:

- ✅ **Sampling rate**: 100 Hz (not 200 Hz)
- ✅ **Current readings**: Accurate within ±5%
- ✅ **Energy billing**: Accurate within ±5%
- ✅ **No false protection trips** on motor startup
- ✅ **Mobile app** displays correct values
- ✅ **Dashboard** displays correct values
- ✅ **Energy persists** across power cycles
- ✅ **No system hangs** (EEPROM timeout protection)

---

## Testing Requirements

Each task must include:

1. **Unit tests** - Test individual functions
2. **Integration tests** - Test with other modules
3. **Hardware tests** - Test with actual sensors and loads
4. **Documentation** - Test reports and screenshots

---

## Documentation References

- **Implementation Plan**: `../Embedded_Fix_Implementation_Plan.md`
- **Issue Solutions**: `../Issue_Solutions.md`
- **Dashboard Requirements**: `../DashboardRequirementfromEmbedded.md`
- **Mobile Requirements**: `../MobileRequirementfromEmbedded.md`

---

## Communication

### Questions

- Tag Hesham Ahmed in commit messages
- Use clear task references (e.g., "Question on Task 01")

### Status Updates

Update this README with task status:

- ⏸️ Not Started
- 🔄 In Progress
- ✅ Complete
- ⚠️ Blocked (explain reason)

---

## Notes

- **Code quality matters** - Follow coding standards
- **Test thoroughly** - Don't skip testing steps
- **Document changes** - Update Doxygen comments
- **Ask early** - If stuck, ask for help quickly

---

**Created**: 2026-01-16
**Lead**: Eng. Hesham Ahmed
**Company**: Gestell
**Project**: Smart Energy Management System
