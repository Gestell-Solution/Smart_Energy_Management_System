# Testing Guide - Smart Energy Management System
## Gestell Company

**Date:** December 12, 2025  
**Version:** 1.0.0  
**Author:** Hisham Ahmed <Hisham.ah.hamed@gmail.com>  
**Status:** Ready for Testing

---

## 🎯 Overview

This guide provides comprehensive testing instructions for the Smart Energy Management System, covering both mobile application and embedded firmware testing.

---

## 📋 Prerequisites

### Software Requirements
- Flutter SDK (3.38.4 or later)
- AVR-GCC Compiler (7.3.0 or later)
- Android Studio / VS Code (for mobile development)
- Proteus Design Suite (optional, for simulation)
- Serial Terminal Software (PuTTY, CoolTerm, etc.)

### Hardware Requirements (for actual hardware testing)
- ATmega32 Microcontroller
- HC-05 Bluetooth Module
- USB to Serial Programmer
- Android smartphone or emulator
- Power supply (5V)

---

## 🧪 Phase 1: Mobile Application Testing

### Step 1: Setup and Dependencies

```bash
cd mobile_app
flutter pub get
```

**Expected output:**
```
Resolving dependencies...
Got dependencies!
```

### Step 2: Code Analysis

```bash
flutter analyze
```

**Expected output:**
```
Analyzing mobile_app...
No issues found!
```

### Step 3: Run the Application

#### Option A: Chrome (Quick testing)
```bash
flutter run -d chrome
```

#### Option B: Android Emulator
```bash
# List available devices
flutter devices

# Run on specific device
flutter run -d <device-id>
```

#### Option C: Physical Android Device
```bash
# Enable USB debugging on your device
# Connect via USB
flutter run
```

### Step 4: Verify Branding ✨

Once the app launches, verify:

#### 4.1 Application Title
- **Check:** Window title or task switcher
- **Expected:** "Gestell - Smart Energy"
- **Status:** [ ] Verified

#### 4.2 Splash Screen
- **Check:** Initial loading screen
- **Expected visible:**
  - Energy icon/logo
  - "Smart Energy" as title
  - "Management System" as subtitle
  - "by Gestell Company" at bottom ✅
- **Status:** [ ] Verified

#### 4.3 Settings Screen
- **Check:** Navigate to Settings tab
- **Expected:** Under "About" section
  - "Developed by" shows "Gestell Company" ✅
  - "App Version" shows "1.0.0"
- **Status:** [ ] Verified

#### 4.4 General UI/UX
- **Check:** All screens and navigation
- **Expected:**
  - Smooth transitions
  - No UI glitches
  - All buttons responsive
  - Proper theme switching (Light/Dark)
- **Status:** [ ] Verified

### Step 5: Functionality Testing

#### 5.1 Navigation
- [ ] Dashboard screen loads
- [ ] History screen accessible
- [ ] Alerts screen accessible
- [ ] Settings screen accessible
- [ ] Bottom navigation works smoothly

#### 5.2 Theme Switching
- [ ] Light theme displays correctly
- [ ] Dark theme displays correctly
- [ ] Theme persists after app restart

#### 5.3 Bluetooth (Mock testing without hardware)
- [ ] "Scan for Devices" button works
- [ ] Shows "No devices found" appropriately
- [ ] Connection status indicator visible

**✅ Mobile App Testing Complete**

---

## 🔧 Phase 2: Embedded System Testing

### Step 1: Code Compilation

Navigate to project root:
```bash
cd /path/to/Smart_Energy_Management_System
```

#### Compile Single File (Quick test)
```bash
cd App/CommunicationManager
avr-gcc -mmcu=atmega32 -Wall -Os -c App_CommManager.c -o App_CommManager.o
```

**Expected:**
- No errors
- No warnings
- `.o` file created

#### Check Object File Size
```bash
avr-size App_CommManager.o
```

**Expected output:**
```
   text    data     bss     dec     hex filename
   XXXX     XXX     XXX    XXXX    XXXX App_CommManager.o
```

#### Full Project Compilation
```bash
# Using Makefile (if available)
make clean
make all

# Or manual compilation
avr-gcc -mmcu=atmega32 -Wall -Os -o main.elf \
    main.c \
    App/CommunicationManager/App_CommManager.c \
    Hal/HC05/HC05_Program.c \
    Mcal/UART/UART_Init.c \
    Mcal/UART/UART_Tx.c \
    Mcal/UART/UART_Rx.c \
    Mcal/DIO/DIO_Program.c \
    Mcal/Timer0/TIMER0_Program.c \
    # ... add all other required files

# Generate HEX file
avr-objcopy -O ihex main.elf main.hex
```

**Expected:**
- ✅ Compilation successful
- ✅ No errors or warnings
- ✅ `main.hex` file created

---

### Step 2: Proteus Simulation Testing

#### 2.1 Load Project in Proteus
1. Open Proteus Design Suite
2. Load your `.pdsprj` file
3. Verify components:
   - [ ] ATmega32 present
   - [ ] UART connections correct
   - [ ] Virtual Terminal or COMPIM present
   - [ ] Power supply connected
   - [ ] LED on PORTD.3 (for debugging)

#### 2.2 Configure Microcontroller
1. Double-click ATmega32
2. Set Program File: Browse to `main.hex`
3. Set Clock Frequency: 8000000 Hz (or as per your design)
4. Click OK

#### 2.3 Configure Virtual Terminal/COMPIM
**If using Virtual Terminal:**
- Double-click Virtual Terminal
- Baud Rate: 9600
- Click OK

**If using COMPIM:**
- Double-click COMPIM  
- Port Number: COM3 (or any available)
- Baud Rate: 9600
- Click OK

#### 2.4 Run Simulation
Click ▶ (Play button) in Proteus

**Expected:**
- ✅ Simulation starts without errors
- ✅ Microcontroller runs
- ✅ No red error markers

---

### Step 3: Protocol Testing (CRITICAL)

This tests the **dynamic frame length fix** which was the main bug fix.

#### Test 3.1: Small Frame (Regression Test)

**Purpose:** Verify old format still works

**Send via Virtual Terminal:**
```
AA 05 03 01 02
```

**Frame breakdown:**
- `AA` = Header (0xAA)
- `05` = Length (5 bytes total)
- `03` = Command (Read EEPROM = 0x03)
- `01 02` = Data (2 bytes)

**Expected Results:**
- ✅ LED on PORTD.3 blinks
- ✅ Frame is received and processed
- ✅ System responds (echo or command response)

**Status:** [ ] PASS / [ ] FAIL

---

#### Test 3.2: Medium Frame (Validates Fix) 🆕

**Purpose:** Verify dynamic length handling works

**Send via Virtual Terminal:**
```
AA 0A 06 11 22 33 44 55 66 77
```

**Frame breakdown:**
- `AA` = Header
- `0A` = Length (10 bytes = 0x0A)
- `06` = Command (Get Logged Data = 0x06)
- `11 22 33 44 55 66 77` = 7 bytes of data

**Expected Results:**
- ✅ All 10 bytes received correctly
- ✅ LED blinks
- ✅ System processes 7 data bytes
- ✅ Proper response sent back
- 🎉 **This proves the fix works!**

**Status:** [ ] PASS / [ ] FAIL

**If FAIL:** The dynamic length fix was not applied correctly. Verify `App_CommManager.c` line ~108 has `FrameLen = value;` not `FrameLen = 5;`

---

#### Test 3.3: Large Frame (Advanced) 🆕

**Purpose:** Test capability with larger data

**Send via Virtual Terminal:**
```
AA 32 05 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
```

**Frame breakdown:**
- `AA` = Header
- `32` = Length (50 bytes = 0x32)
- `05` = Command (Get RMS Data = 0x05)
- `01..2F` = 47 bytes of data

**Expected Results:**
- ✅ All 50 bytes received
- ✅ No buffer overflow
- ✅ System remains stable
- ✅ Correct processing

**Status:** [ ] PASS / [ ] FAIL

---

#### Test 3.4: Maximum Frame

**Purpose:** Test absolute limit

**Send via Virtual Terminal:**
```
AA FF 05 [252 bytes of data in HEX format]
```

**Expected Results:**
- ✅ 255 bytes received successfully
- ✅ No crashes or buffer overflow
- ✅ System handles maximum frame size

**Status:** [ ] PASS / [ ] FAIL

---

### Step 4: Stress Testing

#### 4.1 Rapid Frame Transmission

**Send 100 frames rapidly:**
- Mix of sizes: 5, 10, 20, 50 bytes
- Send as fast as possible

**Expected Results:**
- ✅ All frames processed
- ✅ No dropped frames
- ✅ System remains stable
- ✅ No memory leaks

**Status:** [ ] PASS / [ ] FAIL

#### 4.2 Malformed Frame Handling

**Test error recovery:**

**Send invalid header:**
```
BB 05 03 01 02  (wrong header)
```
**Expected:** Ignored, waits for 0xAA

**Send incomplete frame:**
```
AA 10 05 01 02  (length says 16, only 5 sent)
```
**Expected:** Timeout or waits for rest

**Status:** [ ] PASS / [ ] FAIL

---

## 🔗 Phase 3: Integration Testing

### Prerequisites
- Mobile app running
- Embedded system programmed and powered
- HC-05 Bluetooth module configured and connected

### Step 1: Bluetooth Pairing

1. Power on embedded system
2. HC-05 should be discoverable
3. In mobile app: Settings → Scan for Devices
4. Select "HC-05" from list
5. Wait for connection

**Expected Results:**
- ✅ Device appears in scan list
- ✅ Connection successful
- ✅ "Connected" indicator shows green
- ✅ Status message confirms connection

**Status:** [ ] Connected / [ ] Failed

---

### Step 2: Real-Time Data Display

1. Navigate to Dashboard
2. Observe gauges and values

**Verify:**
- [ ] Voltage reading updates (every 1 second)
- [ ] Current reading updates
- [ ] Power calculation correct
- [ ] Energy accumulation working
- [ ] Cost calculation displayed

---

### Step 3: Command Execution

Test each command:

#### 3.1 Refresh Data
- Click "Refresh" button
- **Expected:** New data request sent, values update

#### 3.2 Reset Energy Counter
- Click "Reset" button
- Confirm dialog
- **Expected:** Energy counter resets to 0

#### 3.3 Calibrate Sensors
- Navigate to Settings
- Click "Calibrate"
- **Expected:** Calibration command sent

**Status:** [ ] All commands work

---

### Step 4: Alert System

Trigger an overvoltage condition (if possible in simulation):

**Expected:**
- ✅ Alert notification appears
- ✅ Alert logged in history
- ✅ Buzzer/LED activates (on hardware)

**Status:** [ ] PASS / [ ] FAIL

---

## 📊 Test Results Summary

### Mobile Application
| Test | Status | Notes |
|------|--------|-------|
| Branding verification | [ ] | |
| UI/UX functionality | [ ] | |
| Navigation | [ ] | |
| Theme switching | [ ] | |
| Bluetooth scanning | [ ] | |

### Embedded System
| Test | Status | Notes |
|------|--------|-------|
| Compilation | [ ] | |
| 5-byte frame (old) | [ ] | |
| 10-byte frame (new) | [ ] | Critical fix verification |
| 50-byte frame | [ ] | |
| 255-byte frame | [ ] | |
| Stress test | [ ] | |
| Error handling | [ ] | |

### Integration
| Test | Status | Notes |
|------|--------|-------|
| Bluetooth connection | [ ] | |
| Data transmission | [ ] | |
| Command execution | [ ] | |
| Alert system | [ ] | |
| Stability (1+ hour) | [ ] | |

---

## 🐛 Troubleshooting

### Mobile App Issues

**Issue:** App doesn't run
```bash
# Solution:
flutter clean
flutter pub get
flutter run
```

**Issue:** Branding not visible
- Check `main.dart` line 42
- Check `pubspec.yaml` line 2
- Rebuild app

### Embedded Issues

**Issue:** Compilation errors
- Verify all `.h` files are accessible
- Check include paths
- Ensure correct MCU type: `-mmcu=atmega32`

**Issue:** Large frames not received
- **Critical:** Check `App_CommManager.c` line ~108
- Must be: `FrameLen = value;`
- NOT: `FrameLen = 5;`
- Recompile and test

**Issue:** Proteus simulation won't start
- Verify HEX file loaded
- Check clock frequency (8000000 Hz)
- Ensure power supply connected

### Integration Issues

**Issue:** Bluetooth won't connect
- Check HC-05 power and configuration
- Verify baud rate matches (9600)
- Check mobile app Bluetooth permissions

**Issue:** No data received
- Verify UART TX/RX connections
- Check baud rate on both sides
- Test with loopback first

---

## ✅ Success Criteria

### Minimum Requirements
- [ ] Mobile app displays Gestell branding
- [ ] Code compiles without errors
- [ ] Small frames (5 bytes) work
- [ ] Large frames (10+ bytes) work ← **Critical**
- [ ] Basic Bluetooth connection works

### Full Success
- [ ] All above minimum requirements
- [ ] All frame sizes up to 255 bytes work
- [ ] Stress test passes
- [ ] Full integration successful
- [ ] System stable for extended period

---

## 📝 Test Report Template

```
=================================================
TEST REPORT - Smart Energy Management System
=================================================
Date: _______________
Tester: Hisham Ahmed <Hisham.ah.hamed@gmail.com>
Company: Gestell Company
Version: 1.0.0

MOBILE APPLICATION:
[  ] PASS  [  ] FAIL  - Branding
[  ] PASS  [  ] FAIL  - Functionality  
[  ] PASS  [  ] FAIL  - UI/UX

EMBEDDED SYSTEM:
[  ] PASS  [  ] FAIL  - Compilation
[  ] PASS  [  ] FAIL  - Small frames
[  ] PASS  [  ] FAIL  - Large frames (CRITICAL)
[  ] PASS  [  ] FAIL  - Stress test

INTEGRATION:
[  ] PASS  [  ] FAIL  - Bluetooth connection
[  ] PASS  [  ] FAIL  - Data exchange
[  ] PASS  [  ] FAIL  - Commands

OVERALL STATUS: [  ] APPROVED  [  ] NEEDS WORK

Notes:
_________________________________________________
_________________________________________________
_________________________________________________
=================================================
```

---

## 🚀 Next Steps After Testing

### If All Tests Pass:
1. Document results
2. Create deployment package
3. Prepare for production
4. User acceptance testing

### If Tests Fail:
1. Document failures in detail
2. Review code changes
3. Fix issues
4. Retest

---

**Author:** Hisham Ahmed  
**Email:** Hisham.ah.hamed@gmail.com  
**Company:** Gestell Company  
**Document Version:** 1.0  
**Last Updated:** December 12, 2025

**© 2025 Gestell Company. All rights reserved.**
