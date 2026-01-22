# FixTasks Summary - Smart Energy Management System

## Document Information

**Date**: January 16, 2026  
**Project**: Smart Energy Management System  
**Platform**: ATmega32 Microcontroller  
**Lead**: Eng. Hesham Ahmed  
**Company**: Gestell

---

## Executive Summary

We have created **8 detailed tasks** to fix all critical issues in the embedded system. Each task contains:

✅ **Detailed Problem Description** - What the issue is and why it occurs  
✅ **Step-by-Step Instructions** - Implementation guide with code  
✅ **Before/After Code** - Clear examples  
✅ **Testing Procedures** - How to verify the fix works  
✅ **Success Criteria** - When to consider the task complete  
✅ **Required Deliverables** - What should be submitted

---

## Complete Task List

### Phase 1: Critical Fixes (Week 1)

#### Task 01: Fix Timer1 Duplicate ISR

- **Problem**: ISR called twice instead of once
- **Impact**: Sampling rate doubled, energy calculation wrong
- **Solution**: Remove second ISR, keep only one
- **Assignee**: Mohamed Diaa
- **Time**: 8 hours

#### Task 02: Fix Current Sensor Variable Shadowing

- **Problem**: Variable shadowing prevents saving calibration value
- **Impact**: Current readings offset by 25 Amperes
- **Solution**: Correct variable scope
- **Assignee**: Ahmed Twap
- **Time**: 10 hours

#### Task 03: Add Power Factor

- **Problem**: Calculates Apparent Power instead of Real Power
- **Impact**: 15-40% error in energy calculation
- **Solution**: Multiply power by Power Factor (0.85)
- **Assignee**: Ahmed Ashraf
- **Time**: 12 hours

#### Task 04: Fix Protection Manager Debouncing

- **Problem**: Immediate trip on first high reading
- **Impact**: False trips during motor startup
- **Solution**: Add debouncing and hysteresis
- **Assignee**: Mohamed Abdelgaber
- **Time**: 10 hours

### Phase 2: High Priority (Week 2)

#### Task 05: Communication Protocol Dual Format

- **Problem**: Mobile and Dashboard expect different formats
- **Impact**: One or both cannot read data correctly
- **Solution**: Separate command for each platform
- **Assignee**: Mohamed Diaa
- **Time**: 10 hours

#### Task 06: EEPROM Timeout Protection

- **Problem**: System hangs forever if EEPROM fails
- **Impact**: Complete system freeze
- **Solution**: Add 10ms timeout
- **Assignee**: Ahmed Ashraf
- **Time**: 6 hours

#### Task 07: Energy Persistence in EEPROM

- **Problem**: Energy counter resets on power cycle
- **Impact**: Loss of billing data
- **Solution**: Periodic save to EEPROM with wear leveling
- **Assignee**: Basma Khaled
- **Time**: 8 hours

#### Task 08: Fix Voltage ADC Divisor

- **Problem**: Using 1023 instead of 1024
- **Impact**: 0.1% error in voltage reading
- **Solution**: Correct the constant
- **Assignee**: Ahmed Ashraf
- **Time**: 2 hours

---

## Team Workload Distribution

| Developer              | Tasks           | Total Hours  |
| ---------------------- | --------------- | ------------ |
| **Mohamed Diaa**       | Task 01, 05     | 18 hours     |
| **Ahmed Twap**         | Task 02         | 10 hours     |
| **Ahmed Ashraf**       | Task 03, 06, 08 | 20 hours     |
| **Mohamed Abdelgaber** | Task 04         | 10 hours     |
| **Basma Khaled**       | Task 07         | 8 hours      |
| **TOTAL**              | **8 tasks**     | **66 hours** |

---

## Dependency Chart

```text
┌─────────────┐
│  Task 01    │  Timer1 Fix
│  Timer ISR  │  (Foundation)
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Task 02    │  Current Sensor
│  Current    │  (Needs correct timing)
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Task 03    │  Power Factor
│  Power Calc │  (Needs accurate I)
└──────┬──────┘
       │
       ├─────────────────────────┐
       │                         │
       ↓                         ↓
┌─────────────┐          ┌─────────────┐
│  Task 04    │          │  Task 05    │
│  Protection │          │  Comm Proto │
└─────────────┘          └─────────────┘

Independent chain:
┌─────────────┐
│  Task 06    │  EEPROM Timeout
│  EEPROM     │
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Task 07    │  Data Persistence
│  Persist    │
└─────────────┘

Independent:
┌─────────────┐
│  Task 08    │  Voltage ADC
│  Voltage    │
└─────────────┘
```

---

## Implementation Timeline (Suggested)

### Week 1

**Goal**: Fix core measurements

| Day     | Task                 | Developer          | Status |
| ------- | -------------------- | ------------------ | ------ |
| Day 1-2 | Task 01 (Timer)      | Mohamed Diaa       | ⏸️     |
| Day 2-3 | Task 02 (Current)    | Ahmed Twap         | ⏸️     |
| Day 3-5 | Task 03 (Power)      | Ahmed Ashraf       | ⏸️     |
| Day 4-5 | Task 04 (Protection) | Mohamed Abdelgaber | ⏸️     |

### Week 2

**Goal**: Integration and communication

| Day     | Task                     | Developer    | Status |
| ------- | ------------------------ | ------------ | ------ |
| Day 1-2 | Task 05 (Communication)  | Mohamed Diaa | ⏸️     |
| Day 1   | Task 08 (Voltage)        | Ahmed Ashraf | ⏸️     |
| Day 2-3 | Task 06 (EEPROM Timeout) | Ahmed Ashraf | ⏸️     |
| Day 3-4 | Task 07 (Persistence)    | Basma Khaled | ⏸️     |
| Day 5   | **Integration Testing**  | Everyone     | ⏸️     |

---

## Overall Success Criteria

After completing all tasks, the system must meet:

### Measurements

✅ **Sampling rate**: Exactly 100 Hz (not 200 Hz)  
✅ **Current accuracy**: ±5% of actual value  
✅ **Voltage accuracy**: ±2% of actual value  
✅ **Power accuracy**: ±10% with correct Power Factor  
✅ **Energy accumulation accuracy**: ±5% over 1 hour

### Protection System

✅ **No false trips** during motor startup  
✅ **Fast trip** (100ms) on real faults  
✅ **Auto-reset** after fault clears

### Communications

✅ **Mobile App** displays correct values  
✅ **Dashboard** displays correct values  
✅ **Relay Control** works from both platforms  
✅ **Response time** less than 100ms

### Data Storage

✅ **Energy persists** after power cycle  
✅ **No system hangs** from EEPROM  
✅ **Wear Leveling** prevents excessive writes

---

## Task Files

Each task has a detailed file in the `FixTasks/` folder:

1. `Task01_Fix_Timer1_Duplicate_ISR.md`
2. `Task02_Fix_Current_Sensor_Variable_Shadowing.md`
3. `Task03_Add_Power_Factor.md`
4. `Task04_Fix_Protection_Manager_Debouncing.md`
5. `Task05_Communication_Protocol_Dual_Format.md`
6. `Task06_Add_EEPROM_Timeout.md`
7. `Task07_Energy_Persistence_EEPROM.md`
8. `Task08_Fix_Voltage_ADC_Divisor.md`

**Index File**: `README.md` - Contains complete table and instructions

---

## Reference Documents

Tasks were created based on:

- `Embedded_Fix_Implementation_Plan.md` - Comprehensive implementation plan
- `Issue_Solutions.md` - Detailed issue solutions
- `DashboardRequirementfromEmbedded.md` - Dashboard requirements
- `MobileRequirementfromEmbedded.md` - Mobile App requirements

---

## Communication and Tracking

### To Update Status

Edit `README.md` and change task status:

- ⏸️ Not Started → Work hasn't begun
- 🔄 In Progress → Currently working
- ✅ Complete → Finished
- ⚠️ Blocked → Blocked (state reason)

### For Questions

- Contact Hesham Ahmed
- Mention task number in message
- Ask early if you encounter difficulties

---

## Important Notes

### Code Quality

- Follow coding standards
- Write clear Doxygen comments
- Use descriptive variable names
- Don't leave commented out code without reason

### Testing

- **Don't skip** testing steps
- Test on actual hardware
- Record results (Screenshots, Measurements)
- Compare with reference meter

### Git Commits

- Clear message: `[Task 01] Fixed Timer1 duplicate ISR`
- Commit after each completed stage
- Create separate branch for each task
- Request Code Review before merge

---

## Conclusion

A **complete and organized folder** has been created containing:

📁 **FixTasks/**

- ✅ 8 detailed task files
- ✅ Comprehensive README
- ✅ Clear team distribution
- ✅ Dependency chart
- ✅ Suggested timeline
- ✅ Clear success criteria

- **Everything is ready for the team to start work! 🚀**

---

**Prepared by**: Eng. Hesham Ahmed  
**Date**: January 16, 2026  
**Company**: Gestell Solution  
**Project**: Smart Energy Management System
