# Proteus Simulation Testing Guide
## Gestell Smart Energy Management System

**Date:** December 12, 2025  
**Platform:** Proteus Design Suite  
**Author:** Hisham Ahmed <Hisham.ah.hamed@gmail.com>  
**Status:** Ready for Simulation

---

## 🎯 Overview

This guide provides step-by-step instructions for testing the Smart Energy Management System using Proteus simulation software. Proteus is ideal for initial testing because it:

- ✅ Accurately simulates ATmega32 microcontroller
- ✅ Supports UART communication and Virtual Terminal
- ✅ Enables easy protocol testing
- ✅ Provides fast debugging capabilities
- ✅ **No physical hardware required!**

---

## 📋 Requirements

### Software
- [x] Proteus Design Suite (v8.0 or later)
- [x] AVR-GCC Compiler
- [x] Virtual Serial Port Software (optional for mobile app integration)

### Required Files
```
Project/
├── main.c
├── App/
│   └── CommunicationManager/
│       ├── App_CommManager.c  ← Updated file
│       └── App_CommManager.h
├── Hal/
├── Mcal/
└── Design/
    └── circuit.pdsprj  ← Proteus file
```

---

## 🛠️ Setup Instructions

### Step 1: Prepare Proteus Project

#### 1.1 Open Your Existing Design
```
File → Open Project → circuit.pdsprj
```

#### 1.2 Verify Essential Components
Required components in your schematic:
- [ ] ATmega32 Microcontroller
- [ ] COMPIM (Virtual COM Port) or VIRTUAL TERMINAL
- [ ] Crystal Oscillator (typically 8MHz or 16MHz)
- [ ] Power Supply (VCC and GND)
- [ ] LED for confirmation (on PORTD.3 as per code)

#### 1.3 Configure UART Connections
**For ATmega32:**
- RXD (Pin 14) → from COMPIM TX
- TXD (Pin 15) → to COMPIM RX
- GND → Ground

---

### Step 2: Compile the Code

#### 2.1 Using AVR-GCC:
```bash
# Navigate to project directory
cd /path/to/project

# Compile main file with all dependencies
avr-gcc -mmcu=atmega32 -Os -o main.elf main.c \
    App/CommunicationManager/App_CommManager.c \
    Hal/HC05/HC05_Program.c \
    Mcal/UART/*.c \
    Mcal/DIO/DIO_Program.c \
    Mcal/Timer0/TIMER0_Program.c
    
# Generate HEX file for Proteus
avr-objcopy -O ihex main.elf main.hex
```

**Alternative using Makefile:**
```bash
make clean
make all
```

#### 2.2 Verify HEX File:
```bash
# Confirm file was created
ls -lh main.hex

# Check size
avr-size main.elf
```

**Expected output:**
```
   text    data     bss     dec     hex filename
   XXXX     XXX     XXX    XXXX    XXXX main.elf
```

---

### Step 3: Load Code into Proteus

#### 3.1 Configure Microcontroller
1. Double-click on ATmega32 in Proteus
2. In "Program File" → Browse and select `main.hex`
3. Set Clock Frequency: 8000000 Hz (or as per your design)
4. Click OK

#### 3.2 Configure Virtual Terminal/COMPIM

**If using VIRTUAL TERMINAL:**
- Double-click on VIRTUAL TERMINAL
- Select Baud Rate = 9600 (or as per your code)
- Click OK

**If using COMPIM:**
- Double-click on COMPIM
- Port Number = COM3 (or any available port)
- Baud Rate = 9600
- Click OK

---

## 🧪 Protocol Testing

### Method 1: Using VIRTUAL TERMINAL (Easiest)

#### Step 1: Start Simulation
```
In Proteus: Click ▶ (Play button)
```

#### Step 2: Send Test Data

In the Virtual Terminal, type data in HEX format:

##### Test 1: Small Frame (5 bytes - Old Format)
```
AA 05 03 01 02
```

**Frame breakdown:**
- `AA` = Header
- `05` = Length (5 bytes)
- `03` = Command (Read EEPROM)
- `01 02` = Data

**Expected Results:**
- ✅ LED on PORTD.3 blinks
- ✅ Terminal receives response (echo back)
- ✅ No errors in simulation

---

##### Test 2: Medium Frame (10 bytes - Tests the Fix) 🆕
```
AA 0A 06 11 22 33 44 55 66 77
```

**Frame breakdown:**
- `AA` = Header
- `0A` = Length (10 bytes = 0x0A)
- `06` = Command (Get Logged Data)
- `11..77` = 7 bytes of data

**Expected Results:**
- ✅ System receives all 10 bytes
- ✅ LED blinks
- ✅ Processes all 7 data bytes correctly
- ✅ Sends proper response
- 🎉 **This proves the dynamic length fix works!**

**If this test fails:** The fix was not applied correctly. Check `App_CommManager.c` line ~108 should have `FrameLen = value;` NOT `FrameLen = 5;`

---

##### Test 3: Large Frame (50 bytes) 🆕
```
AA 32 05 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
```

**Frame breakdown:**
- `AA` = Header
- `32` = Length (50 bytes = 0x32)
- `05` = Command (Get RMS Data)
- `01..2F` = 47 bytes of data

**Expected Results:**
- ✅ Receives all 50 bytes
- ✅ **Before fix:** Would FAIL!
- ✅ **After fix:** SUCCESS! 🎉

---

### Method 2: Using COMPIM + Serial Terminal

#### Step 1: Setup Virtual Serial Port

**On Windows:**
Use **com0com** tool:
```
# Install com0com
# Create pair: COM3 <--> COM4
```

**On Linux:**
Use **socat**:
```bash
# Create virtual serial port pair
socat -d -d pty,raw,echo=0 pty,raw,echo=0
# Output example: /dev/pts/3 and /dev/pts/4
```

#### Step 2: Connect Proteus
- COMPIM in Proteus → COM3
- Serial Terminal program → COM4

#### Step 3: Use Serial Terminal

Recommended programs:
- **RealTerm** (Windows)
- **CoolTerm** (Cross-platform)
- **PuTTY** (Cross-platform)
- **minicom** (Linux)

**Settings:**
- Baud: 9600
- Data bits: 8
- Parity: None
- Stop bits: 1
- Flow Control: None

#### Step 4: Send Data
Use "Send" in HEX mode:
```
AA 0A 06 11 22 33 44 55 66 77
```

---

## 🔬 Advanced Testing: Mobile App Integration

### Method (Advanced):

#### Step 1: Virtual Bluetooth Setup
```bash
# On Linux
sudo bluetoothctl
# Setup virtual bluetooth device

# On Windows
# Use Bluetooth Virtual COM port feature
```

#### Step 2: Connection Chain
```
Mobile App → Virtual Bluetooth → Virtual COM → Proteus COMPIM
```

**Note:** This method is complex and may not work easily.  
**Better approach:**
- Test mobile app and embedded separately
- Use real hardware for full integration testing

---

## 📊 Test Scenarios

### Scenario 1: Basic Reception Test ✅

**Objective:** Verify system receives data

**Steps:**
1. Start simulation in Proteus
2. Send: `AA 05 03 01 02`
3. Observe LED on PORTD.3

**Success criteria:**
- ✅ LED blinks
- ✅ No simulation errors
- ✅ Terminal shows response

---

### Scenario 2: Dynamic Length Test 🆕

**Objective:** Verify bug fix works

**Steps:**
1. Send small frame: `AA 05 03 01 02` ✅ Should work
2. Send medium frame: `AA 0A 06 11 22 33 44 55 66 77` ✅ **Must work!**
3. Send large frame: `AA 32 05 [47 bytes]` ✅ **Must work!**

**Success criteria:**
- ✅ All frames received successfully
- ✅ LED blinks for each frame
- ✅ Correct responses sent

**Failure (indicates fix not applied):**
- ❌ Only small frames work
- ❌ Large frames ignored or cause errors

---

### Scenario 3: Stress Test 🔥

**Objective:** Test system stability

**Steps:**
1. Send 100 consecutive frames rapidly
2. Vary frame sizes (5, 10, 20, 50 bytes)
3. Monitor memory and buffer

**Success criteria:**
- ✅ System receives all frames
- ✅ No buffer overflow
- ✅ No memory leaks
- ✅ System remains stable

---

## 🐛 Troubleshooting

### Issue: Simulation Won't Start

**Possible causes:**
- ❌ HEX file not loaded
- ❌ Clock frequency incorrect
- ❌ Power supply not connected

**Solution:**
1. Verify HEX file path in ATmega32 properties
2. Set Clock = 8000000 Hz
3. Check VCC and GND connections

---

### Issue: UART Not Working

**Possible causes:**
- ❌ Baud rate mismatch
- ❌ TX/RX wires swapped
- ❌ UART not enabled in code

**Solution:**
Verify in code:
```c
#define F_CPU 8000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// In main():
UCSRB |= (1<<RXEN)|(1<<TXEN);  // Enable RX & TX
```

---

### Issue: LED Doesn't Blink

**Possible causes:**
- ❌ PIN configuration wrong
- ❌ Code doesn't reach DIO_TogglePin
- ❌ Frame not received

**Solution:**
1. Verify DIO initialization:
```c
mDIO_SetPinDirection(GroupD, PIN3, OUTPUT);
```

2. Use Proteus breakpoints:
   - Double-click on code line
   - Set breakpoint
   - Step through execution (F11)

---

### Issue: Large Frames Not Processed ⚠️

**This means:**
- ❌ The fix was NOT applied correctly!

**Verify:**
1. Open `App_CommManager.c`
2. Find line ~108:
```c
case WaitLen:
    FrameLen = value;  // ✅ Should be this
    // NOT:
    // FrameLen = 5;  // ❌ Error!
```

3. If you find `FrameLen = 5;`, change to `FrameLen = value;`
4. Re-compile and re-test

---

## 📈 Performance Measurement

### In Proteus you can measure:

#### 1. Execution Time
```
Use Stopwatch in Proteus:
- Start stopwatch
- Send data
- Stop when processed
- Note time
```

#### 2. Memory Usage
```bash
# After compilation
avr-size main.elf
```

**Output:**
```
   text    data     bss     dec     hex filename
   XXXX     XXX     XXX    XXXX    XXXX main.elf
```

- **text** = Program memory (Flash)
- **data** = Initialized variables (RAM)
- **bss** = Uninitialized variables (RAM)

#### 3. Power Consumption  (Approximate)
In Proteus:
```
Debug → Show Power Analysis
```

---

## ✅ Testing Checklist

### Initial Setup
- [ ] Proteus installed and working
- [ ] AVR-GCC compiler installed
- [ ] .pdsprj file available
- [ ] All components present in schematic

### Compilation
- [ ] Code compiles without errors
- [ ] HEX file generated successfully
- [ ] Size appropriate for ATmega32 (< 32KB)

### Simulation
- [ ] Design simulates without crashes
- [ ] UART transmits and receives
- [ ] LED blinks on data reception

### Protocol Testing
- [ ] 5-byte frame received ✅
- [ ] 10-byte frame received ✅ (proves fix)
- [ ] 50-byte frame received ✅
- [ ] 100-byte frame received ✅
- [ ] 255-byte frame received ✅

### Stability
- [ ] 100 consecutive frames processed
- [ ] No buffer overflow occurs
- [ ] No memory leaks detected
- [ ] System stable over time

---

## 🎯 Expected Results

### ✅ Full Success Means:

1. **Basic Reception:**
   - ✅ Receives small frames (5 bytes)
   
2. **Dynamic Length (The Fix):**
   - ✅ Receives medium frames (10-50 bytes)
   - ✅ Receives large frames (100-255 bytes)
   - 🎉 **This proves the fix is successful!**

3. **Stability:**
   - ✅ No crashes
   - ✅ Proper buffer management
   - ✅ Works for extended periods

---

## 📚 Additional Resources

### Proteus Help:
```
Help → Contents → Microcontroller Simulation
```

### AVR UART Documentation:
```
https://www.nongnu.org/avr-libc/user-manual/group__util__setbaud.html
```

### Virtual Serial Port Tools:
- **Windows:** com0com
- **Linux:** socat, tty0tty

---

## 🚀 Next Steps

After successful Proteus testing:

1. **Document Results**
   - Take screenshots
   - Record values
   - Note any issues

2. **Prepare for Hardware**
   - Order components
   - Prepare PCB
   - Setup programming tools

3. **Real Hardware Testing**
   - Flash firmware
   - Test on actual hardware
   - Compare with Proteus results
   - Note any differences

---

## 💡 Important Notes

### ✅ DO:
- Use Proteus for rapid development
- Test multiple scenarios
- Document everything
- Use breakpoints for debugging

### ❌ DON'T:
- Rely only on Proteus for final product
- Ignore timing differences
- Skip real hardware testing
- Assume 100% simulation accuracy

---

## 📝 Test Report Template

```
=================================================
PROTEUS SIMULATION TEST REPORT
=================================================
Date: _______________
Tester: Hisham Ahmed
Email: Hisham.ah.hamed@gmail.com
Company: Gestell Company

COMPILATION:
[  ] PASS  [  ] FAIL  - Code compiles
[  ] PASS  [  ] FAIL  - HEX generated

SIMULATION:
[  ] PASS  [  ] FAIL  - Design loads
[  ] PASS  [  ] FAIL  - UART works
[  ] PASS  [  ] FAIL  - LED blinks

PROTOCOL (CRITICAL):
[  ] PASS  [  ] FAIL  - 5-byte frame
[  ] PASS  [  ] FAIL  - 10-byte frame (FIX TEST)
[  ] PASS  [  ] FAIL  - 50-byte frame
[  ] PASS  [  ] FAIL  - 255-byte frame

STABILITY:
[  ] PASS  [  ] FAIL  - Stress test
[  ] PASS  [  ] FAIL  - No crashes

OVERALL: [  ] APPROVED  [  ] NEEDS WORK

Notes:
_________________________________________________
_________________________________________________
=================================================
```

---

**Author:** Hisham Ahmed  
**Email:** Hisham.ah.hamed@gmail.com  
**Company:** Gestell Company  
**Version:** 1.0  
**Last Updated:** December 12, 2025

**© 2025 Gestell Company. All rights reserved.**

---

**Note:** Proteus is excellent for testing, but **does not replace** real hardware completely.  
**Best practice:** Test on Proteus first, then validate on actual hardware.
