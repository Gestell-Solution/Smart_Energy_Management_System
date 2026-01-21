# Mobile App Requirements from Embedded System

## Document Purpose
This document describes what the **Mobile App expects to receive** from the Embedded System to function properly. This is NOT a guide for fixing embedded code - that information exists in the Code Analysis Report.

---

## 1. Communication Protocol Expected by Mobile App

### 1.1 Frame Structure
The Mobile App expects frames in this format:
```
[HEADER][LENGTH][COMMAND][DATA...]
```

- **HEADER**: Fixed byte `0xAA` marking frame start
- **LENGTH**: Number of data bytes (Command + Data bytes)
- **COMMAND**: Single byte identifying the message type
- **DATA**: Variable length payload

### 1.2 Connection Method

**Bluetooth Connection**:
- Uses Flutter Blue Plus package
- Connects via HC-05 Bluetooth Classic (SPP profile)
- Scans for devices with "HC-05" in name
- Expected baud rate: 9600
- Uses UART characteristics for data exchange

---

## 2. Required Commands and Expected Responses

### 2.1 GET_RMS_DATA (Command 0x05)

**When Mobile App Sends**:
```
[0xAA][0x00][0x05]
```

**What Mobile App Expects to Receive**:
- Command ID: 0x05
- Payload length: 10 bytes
- Data format: **Scaled Integer Values in Big-Endian byte order**

**Expected Data Structure**:
1. **Voltage** (bytes 0-1): 16-bit unsigned integer, Big-Endian, **scaled by 10**
   - Example: 220.5V → stored as 2205 → bytes: [0x08, 0x9D]
   
2. **Current** (bytes 2-3): 16-bit unsigned integer, Big-Endian, **scaled by 100**
   - Example: 5.20A → stored as 520 → bytes: [0x02, 0x08]
   
3. **Power** (bytes 4-5): 16-bit unsigned integer, Big-Endian, **scaled by 10**
   - Example: 1146.6W → stored as 11466 → bytes: [0x2C, 0xCA]
   
4. **Energy** (bytes 6-9): 32-bit unsigned integer, Big-Endian, **scaled by 100**
   - Example: 2.50kWh → stored as 250 → bytes: [0x00, 0x00, 0x00, 0xFA]

**How Mobile App Parses the Response**:
```
// Mobile expects Big-Endian format (MSB first)
voltage_raw = (data[0] << 8) | data[1]
voltage_actual = voltage_raw / 10.0               → 2205 / 10.0 = 220.5V

current_raw = (data[2] << 8) | data[3]
current_actual = current_raw / 100.0              → 520 / 100.0 = 5.20A

power_raw = (data[4] << 8) | data[5]
power_actual = power_raw / 10.0                   → 11466 / 10.0 = 1146.6W

energy_raw = (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | data[9]
energy_actual = energy_raw / 100.0                → 250 / 100.0 = 2.50kWh
```

**Request Frequency**: Mobile App sends this request every **1 second** when connected.

---

### 2.2 CONTROL_RELAY (Command ID - Verify from Mobile Constants)

**When Mobile App Sends**:
```
[0xAA][0x02][CMD_ID][RelayIndex][State]
```

Where:
- RelayIndex: 0-3 or 1-4 (verify indexing in mobile app code)
- State: 0 = OFF, 1 = ON

**What Mobile App Expects**:
- The embedded system should turn the specified relay ON or OFF
- Optional: Send acknowledgment back to confirm action

**Mobile App UI**: 
- Has relay control buttons in Settings screen
- Shows relay state visually

---

### 2.3 WRITE_EEPROM / Reset Energy (Command ID - Verify from Mobile Constants)

**When Mobile App Sends**:
```
[0xAA][0x04][CMD_ID][0x00][0x00][0x00][0x00]
```

**What Mobile App Expects**:
- Embedded system should reset the energy counter to zero
- Energy counter should be saved to EEPROM for persistence
- Next GET_RMS_DATA response should show energy = 0.0

**Mobile App UI**:
- Settings screen has "Reset Energy" button
- Shows confirmation dialog before reset

---

## 3. Data Format Specifications

### 3.1 Byte Order: Big-Endian (Network Byte Order)

**CRITICAL**: Mobile App expects **Big-Endian** byte order (Most Significant Byte first)

**Example for 16-bit value 2205**:
```
Binary: 0000 1000 1001 1101
Big-Endian bytes: [0x08, 0x9D]
             High byte ↑    ↑ Low byte
```

**Example for 32-bit value 250**:
```
Binary: 0x000000FA
Big-Endian bytes: [0x00, 0x00, 0x00, 0xFA]
           MSB ↑                      ↑ LSB
```

### 3.2 Scaling Factors

| Parameter | Storage Type | Scaling Factor | Example Storage | Actual Value |
|-----------|--------------|----------------|-----------------|--------------|
| Voltage   | uint16       | × 10           | 2205            | 220.5 V      |
| Current   | uint16       | × 100          | 520             | 5.20 A       |
| Power     | uint16       | × 10           | 11466           | 1146.6 W     |
| Energy    | uint32       | × 100          | 250             | 2.50 kWh     |

**Why scaled integers?**
- Avoids floating-point transmission complexities
- Maintains precision
- More compact than float representation

### 3.3 Expected Value Ranges

| Parameter | Raw Range (Scaled) | Actual Range | Display Format |
|-----------|-------------------|--------------|----------------|
| Voltage   | 0 - 3000          | 0 - 300 V    | XXX.X V        |
| Current   | 0 - 2000          | 0 - 20.00 A  | XX.XX A        |
| Power     | 0 - 50000         | 0 - 5000 W   | XXXX.X W       |
| Energy    | 0 - 999900        | 0 - 9999 kWh | XXXX.X kWh     |

---

## 4. Mobile App Features and Expected Data Flow

### 4.1 Dashboard Screen

**Displays**:
- Voltage with animated gauge (arc display)
- Current with animated gauge
- Power with animated gauge  
- Energy counter (cumulative)
- System status (OK / OVERLOAD / OVERVOLT / HIGHPOWER)

**Alert Detection (Performed by Mobile)**:
Mobile App determines status based on thresholds:
- Current > 10A → Status: "OVERLOAD"
- Voltage > 250V → Status: "OVERVOLT"
- Power > 2000W → Status: "HIGHPOWER"
- Otherwise → Status: "OK"

**What Mobile Needs from Embedded**:
- Just the raw V, I, P, E values
- Mobile calculates status locally

### 4.2 History Screen

**Displays**:
- Power consumption chart over time
- Historical data from last session

**What Mobile Needs**:
- Continuous stream of power values via GET_RMS_DATA
- Mobile stores history locally using SharedPreferences

### 4.3 Alerts Screen

**Displays**:
- List of recent alerts (overload events)
- Timestamp and type of each alert

**What Mobile Needs**:
- Real-time data to detect threshold violations
- Mobile manages alert list locally

### 4.4 Settings Screen

**Displays**:
- Device connection status
- Relay controls
- Energy reset button
- Threshold configuration (stored locally in mobile)

**What Mobile Needs**:
- Ability to send relay control commands
- Ability to send energy reset command

---

## 5. Connection and Communication Behavior

### 5.1 Device Discovery

**Mobile App Behavior**:
1. User opens app
2. App scans for Bluetooth devices (10 second timeout)
3. Displays list of discovered devices
4. User selects device (typically named "HC-05")

**Expected from Embedded**:
- HC-05 module should be in discoverable mode
- Device name should be identifiable (e.g., "HC-05" or "Energy Monitor")

### 5.2 Connection Sequence

**Mobile App Behavior**:
1. User taps device from list
2. App attempts to connect (15 second timeout)
3. App discovers UART services and characteristics
4. App enables notifications on RX characteristic
5. App starts periodic GET_RMS_DATA requests

**Expected from Embedded**:
- HC-05 should accept pairing (PIN: typically "1234" or "0000")
- UART service should be available
- Should respond to data requests

### 5.3 Data Request Loop

**Mobile App Behavior**:
- Every 1 second, sends GET_RMS_DATA request
- Waits for response
- Updates UI with received data
- Detects alerts based on thresholds
- Logs data to history

**Expected from Embedded**:
- Respond to each request within 100ms
- Send fresh data (not cached old values)
- Maintain stable Bluetooth connection

---

## 6. Data Update and Timing Requirements

### 6.1 Update Frequency
- Mobile sends GET_RMS_DATA every **1 second**
- This provides smooth gauge animations and chart updates

### 6.2 Response Time
- Expected response time: **< 100ms**
- Mobile may show "timeout" if no response within 500ms

### 6.3 Data Consistency
- Energy counter should be **monotonically increasing** (never decrease unless reset)
- Energy counter should **persist across power cycles** (stored in EEPROM)
- Other values (V, I, P) should reflect **current measurements**

---

## 7. Key Differences from Dashboard Requirements

### 7.1 Data Format Comparison

| Aspect          | Mobile App                  | Dashboard                |
|-----------------|----------------------------|--------------------------|
| Data Type       | Scaled integers (uint16/32) | Float32                  |
| Byte Order      | **Big-Endian** (MSB first) | Little-Endian (LSB first) |
| V format        | uint16 ÷ 10                | float32                  |
| I format        | uint16 ÷ 100               | float32                  |
| P format        | uint16 ÷ 10                | float32                  |
| E format        | uint32 ÷ 100               | float32                  |
| Total bytes     | 10 bytes                   | 16 bytes                 |
| Precision       | Fixed precision            | Floating precision       |

### 7.2 Why Different Formats?

**Mobile Choice (Scaled Integers)**:
- ✅ More compact (10 vs 16 bytes)
- ✅ Easier to parse on resource-constrained embedded
- ✅ Avoids floating-point complications
- ✅ Sufficient precision for display

**Dashboard Choice (Float32)**:
- ✅ Native JavaScript number type
- ✅ Flexible precision
- ✅ Easier for web calculations

### 7.3 Recommendation for Embedded

Since mobile and dashboard expect different formats, embedded system could:

**Option A**: Use different command IDs
- Mobile uses: Command 0x05 → Returns 10-byte int16 format
- Dashboard uses: Command 0x15 (new) → Returns 16-byte float32 format

**Option B**: Auto-detect client type (more complex)

**Option C**: Choose one format and modify the other client (not recommended)

---

## 8. Error Handling

### 8.1 Connection Loss

**Mobile App Behavior**:
- Shows "Disconnected" status
- Stops sending requests
- Retains last known values on screen
- User can tap "Reconnect"

**Embedded Should**:
- Not crash if connection drops
- Be ready to accept new connection

### 8.2 Invalid Data Received

**Mobile App Behavior**:
- Ignores frames with wrong header
- Ignores frames with incorrect length
- Displays "---" for unparseable values
- Continues requesting data

**Embedded Should**:
- Always send valid frame structure
- Ensure data falls within expected ranges

### 8.3 Out-of-Range Values

**Mobile App Display**:
- Values > max range: Display in red/warning color
- Negative values (impossible for unsigned): Show as 0
- Very large values: May trigger alert

---

## 9. Summary of What Embedded Must Provide

### Essential Requirements:
1. ✅ HC-05 Bluetooth module at 9600 baud
2. ✅ Response to GET_RMS_DATA (0x05) with **10 bytes** of data
3. ✅ Data format: 4 scaled integers (V, I, P, E) in **Big-Endian** byte order
4. ✅ Voltage scaled ×10, Current scaled ×100, Power scaled ×10, Energy scaled ×100
5. ✅ Response time < 100ms

### Additional Features:
6. ⭕ Relay control command handler
7. ⭕ Energy reset (WRITE_EEPROM) handler
8. ⭕ Energy persistence in EEPROM

### Quality Requirements:
9. ⭕ Stable Bluetooth connection (no random disconnects)
10. ⭕ Accurate sensor readings (proper calibration)
11. ⭕ Energy counter accuracy (correct integration over time)

---

## 10. Protocol Example Session

**Mobile Connects and Monitors**:
```
Mobile    → [0xAA][0x00][0x05]                    (Request data)
Embedded  → [0xAA][0x0A][0x05][10 bytes int16]    (V=2205, I=520, P=11466, E=250)

... Mobile parses: 220.5V, 5.20A, 1146.6W, 2.50kWh ...
... Updates gauges and chart ...
... 1 second delay ...

Mobile    → [0xAA][0x00][0x05]                    (Request again)
Embedded  → [0xAA][0x0A][0x05][10 bytes int16]    (Updated values)

... User taps "Reset Energy" ...
... Mobile shows confirmation dialog, user confirms ...

Mobile    → [0xAA][0x04][CMD][0x00][0x00][0x00][0x00]  (Reset energy)
Embedded  → [Energy counter = 0, saved to EEPROM]

... Next request ...

Mobile    → [0xAA][0x00][0x05]
Embedded  → [0xAA][0x0A][0x05][V, I, P, E=0]      (Energy now 0)
```

---

## 11. Testing Mobile App Without Full Embedded

### Option 1: Implement Minimal Response
Just implement GET_RMS_DATA response with dummy values:
- Voltage: 2200 (220.0V)
- Current: 500 (5.00A)
- Power: 11000 (1100.0W)
- Energy: 100 (1.00kWh)

This allows:
- ✅ Testing connection
- ✅ Seeing UI update
- ✅ Verifying data parsing works

### Option 2: Use Embedded Echo
Have embedded echo back received commands:
- Helps verify mobile is sending correct format
- Can inspect what mobile sends

### Option 3: Bluetooth Serial Terminal
Use serial monitor on PC to simulate responses:
- Manually send response bytes
- Test mobile's parsing logic

---

## Notes

- This document describes **Mobile App's perspective only**
- For embedded code fixes and implementation details, refer to: `Code_Analysis_ReportV2.md`
- Mobile app code is complete - no changes needed on mobile side
- All required changes are on embedded system side
- Pay special attention to **Big-Endian byte order** - this is different from Dashboard!

---

**Document Version**: 1.0  
**Last Updated**: 2025-12-15  
**Contact**: Hesham Ahmed (Hisham4Ahmed@gmail.com)
