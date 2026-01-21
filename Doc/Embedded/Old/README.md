# Embedded System Documentation

## Overview
This directory contains complete analysis and plans for the Embedded System code of the project.

---

## 📄 Files in This Directory

### 1. **Code_Analysis_ReportV2.md**
**Size:** 94 KB  
**Type:** Comprehensive Analysis Report  
**Content:**
- Detailed analysis of all code files (29 files)
- Code quality assessment for each module
- Identification of critical issues and bugs
- Performance and memory usage reviews
- Team member contributions and reviews

**When to use:**
- When you need detailed understanding of issues
- For reviewing specific code modules
- To understand quality ratings for each module

---

### 2. **Issue_Solutions.md** ⭐ Main Solutions File
**Size:** 33 KB  
**Type:** Solutions Guide  
**Content:**
- Detailed explanation of each specific problem
- Old Code (showing the bug)
- Pseudocode for new implementation
- Impact analysis
- Missing Requirements with pseudocode

**Sections:**
1. **MCAL Layer Issues**
   - Timer1 Duplicate ISR (CRITICAL)
   - EEPROM Timeout Protection
   - EEPROM Block Write Overflow
   - ADC Timeout

2. **HAL Layer Issues**
   - Current Sensor Variable Shadowing (CRITICAL)
   - Current Sensor Wrong Reset
   - Voltage Sensor ADC Divisor
   - sqrt() Safety Checks

3. **Application Layer Issues**
   - Wrong Power Calculation (CRITICAL)
   - Protection Manager No Debouncing (CRITICAL)

4. **Communication Protocol Issues**
   - Different Data Formats (Mobile vs Dashboard)
   - Push Button Debouncing

5. **Missing Requirements**
   - WiFi Module Support (ESP-01)
   - Energy Reset Command
   - Calibration Command Handler
   - System Data Persistence

**When to use:**
- When implementing a fix for a specific problem
- To understand pseudocode for new implementation
- To know what requirements are missing

---

### 3. **Embedded_Fix_Implementation_Plan.md** ⭐ Implementation Plan
**Size:** 44 KB  
**Type:** Detailed Work Plan  
**Content:**
- Work divided into 4 Sprints (4 weeks)
- Task assignments for team members
- Detailed implementation steps for each task
- Time estimates for each task
- Testing strategy
- Risk management

**Phases:**
1. **Phase 1 (Week 1) - Critical Fixes:**
   - Task 1.1: Fix Timer1 Duplicate ISR (8h)
   - Task 1.2: Fix Current Sensor (10h)
   - Task 1.3: Fix Power Calculation (12h)
   - Task 1.4: Fix Protection Debouncing (10h)
   - Task 1.5: Communication Protocols (16h)

2. **Phase 2 (Week 2) - High Priority:**
   - EEPROM Timeout Protection (6h)
   - System Data Persistence (12h)
   - Energy Reset Command (6h)
   - Voltage Sensor Fix (4h)

3. **Phase 3 (Week 3) - Medium Priority:**
   - WiFi Module Support (16h)
   - Calibration Command (8h)
   - Button Debouncing (4h)
   - Code Cleanup (8h)

4. **Phase 4 (Week 4) - Testing:**
   - Integration Testing (16h)
   - Accuracy Validation (12h)
   - Mobile/Dashboard Testing (8h)

**When to use:**
- When planning a new sprint
- To assign tasks to team members
- To track work progress
- To ensure all testing is complete

---

### 4. **DashboardRequirementfromEmbedded.md**
**Size:** 10 KB  
**Type:** Requirements from Web Dashboard  
**Content:**
- Expected frame structure
- Data Format: Float32, Little-Endian
- Required commands (GET_RMS_DATA, RELAY_CONTROL, etc.)
- Request rate: 1 second
- Connection: Bluetooth (Local) or WiFi (Remote)

**Key differences from Mobile:**
- Data Type: **Float32** (16 bytes total)
- Byte Order: **Little-Endian**
- Command ID: 0x05 or 0x15

**When to use:**
- When developing Communication Protocol for Dashboard
- To verify correct data format
- When debugging Dashboard connection

---

### 5. **MobileRequirementfromEmbedded.md**
**Size:** 13 KB  
**Type:** Requirements from Mobile App  
**Content:**
- Expected frame structure
- Data Format: Scaled Integers, Big-Endian
- Required commands
- Request rate: 1 second
- Connection: Bluetooth only

**Key differences from Dashboard:**
- Data Type: **uint16/uint32** (10 bytes total)
- Byte Order: **Big-Endian**
- Scaling:
  - Voltage: ×10
  - Current: ×100
  - Power: ×10
  - Energy: ×100

**When to use:**
- When developing Communication Protocol for Mobile
- To verify correct data format
- When debugging Mobile App connection

---

### 6. **Training_Learning_Outcomes.md**
**Size:** 14 KB  
**Type:** Training Results  
**Content:**
- Overview of training program
- Learning outcomes for each sprint
- Team skills acquired

**When to use:**
- To review training outcomes
- To assess team development

---

## 🚀 Getting Started

### For Developer Fixing a Specific Bug:

1. **Read `Issue_Solutions.md`**
   - Find the specific problem
   - Understand the old code
   - Review the new pseudocode

2. **Refer to `Code_Analysis_ReportV2.md`**
   - Read detailed analysis of the module
   - Understand the full context

3. **Implement the solution**
   - Follow the pseudocode
   - Use implementation steps from `Embedded_Fix_Implementation_Plan.md`

4. **Test**
   - Use test procedures from the plan
   - Verify success criteria

---

### For Project Manager Planning:

1. **Start with `Embedded_Fix_Implementation_Plan.md`**
   - Review the 4 phases
   - Assign tasks to team members

2. **Use `Issue_Solutions.md`**
   - Understand priority of each issue
   - Estimate complexity of each task

3. **Refer to Requirements Files**
   - Understand Mobile/Dashboard requirements
   - Ensure compatibility

---

### For Tester:

1. **Use `Embedded_Fix_Implementation_Plan.md`**
   - Testing Strategy section
   - Test Procedures for each task

2. **Refer to `Code_Analysis_ReportV2.md`**
   - Understand corner cases
   - Know expected edge cases

3. **Use `Issue_Solutions.md`**
   - Testing Recommendations section
   - Success Criteria

---

## 📊 Summary of Critical Issues

### 🔴 CRITICAL (Must Fix Immediately):

| # | Issue | File | Impact |
|---|-------|------|--------|
| 1 | Timer1 Duplicate ISR | TIMER1_Program.c | Double sampling rate and energy |
| 2 | Current Sensor Shadowing | hCurrent_Program.c | Completely wrong current readings |
| 3 | Wrong Power Calculation | MeasurementEngine | 15-40% error in energy |
| 4 | No Protection Debouncing | ProtectionManager | Constant false trips |
| 5 | Protocol Format Mismatch | CommManager | Mobile/Dashboard don't work |

### ⚠️ HIGH (Should Fix Soon):

| # | Issue | File | Impact |
|---|-------|------|--------|
| 6 | EEPROM No Timeout | EEPROM_Program.c | System can freeze |
| 7 | Current Sensor Wrong Reset | hCurrent_Program.c | Inconsistent data |
| 8 | Missing Data Persistence | - | Data loss on power cycle |
| 9 | Energy Reset Missing | CommManager | Mobile App can't reset |

### ✅ MEDIUM (Improvements):

| # | Issue | File | Impact |
|---|-------|------|--------|
| 10 | Voltage ADC Divisor | Voltage_Program.c | ±0.22V accuracy |
| 11 | WiFi Support Missing | - | Dashboard Remote doesn't work |
| 12 | Calibration Command | CommManager | Can't calibrate remotely |
| 13 | Button Debouncing | Button driver | Multiple presses |

---

## 🎯 Recommended Priorities

### Week 1 (Critical Fixes):
```
Day 1-2: Timer1 + Current Sensor fixes
Day 3-4: Power Calculation + Protection Debouncing
Day 5:   Communication Protocol
```

### Week 2 (High Priority):
```
Day 1-2: EEPROM Protection + Data Persistence
Day 3-4: Energy Reset + Testing
Day 5:   Voltage Sensor fix
```

### Week 3 (Medium Priority):
```
Day 1-3: WiFi Module (optional)
Day 4:   Calibration + Debouncing
Day 5:   Code Cleanup
```

### Week 4 (Testing):
```
Day 1-2: Integration Testing
Day 3-4: Accuracy Validation
Day 5:   Mobile/Dashboard Testing
```

---

## 📞 Contact Information

**Lead Engineer:** Eng. Hesham Ahmed  
**Email:** Hisham4Ahmed@gmail.com  
**Company:** Gestell  

**Development Team:**
1. Mohamed Diaa - mohammediaato@gmail.com
2. Ahmed Ashraf - ahmedashraf2022222@gmail.com
3. Mohamed Abdelgaber - mohamedabdelgaber247@gmail.com
4. Basma Khaled - basmak55@gmail.com
5. Ahmed Twap - ahmedtwap2@gmail.com

---

## 📝 Notes

- All files use English for code and technical terms
- Pseudocode format for easy understanding
- Time estimates include development + testing

---

**Last Updated:** December 15, 2025  
**Version:** 1.0
