# Dashboard Requirements from Embedded System

## Document Purpose
This document describes what the **Web Dashboard expects to receive** from the Embedded System to function properly. This is NOT a guide for fixing embedded code - that information exists in the Code Analysis Report.

---

## 1. Communication Protocol Expected by Dashboard

### 1.1 Frame Structure
The Dashboard expects frames in this format:
```
[HEADER][LENGTH][COMMAND][DATA...]
```

- **HEADER**: Fixed byte `0xAA` marking frame start
- **LENGTH**: Number of data bytes (excluding header, length, and command)
- **COMMAND**: Single byte identifying the message type
- **DATA**: Variable length payload

### 1.2 Connection Methods

**Local Mode (Bluetooth)**:
- Uses Web Serial API (Chrome/Edge browsers)
- Connects via HC-05 Bluetooth module
- Expected baud rate: 9600
- Direct serial communication

**Remote Mode (WiFi)**:
- Uses WebSocket connection to IoT server
- Server forwards data from ESP-01 module
- Server address: `http://localhost:3000` (configurable)
- Requires ESP-01 WiFi module on embedded side

---

## 2. Required Commands and Expected Responses

### 2.1 GET_RMS_DATA (Command 0x05)

**When Dashboard Sends**:
```
[0xAA][0x00][0x05]
```

**What Dashboard Expects to Receive**:
- Command ID: 0x05
- Payload length: 16 bytes
- Data format: **4 Float32 values in Little-Endian byte order**

**Expected Data Structure**:
1. **Voltage** (bytes 0-3): 32-bit float, Little-Endian, in Volts (e.g., 220.5)
2. **Current** (bytes 4-7): 32-bit float, Little-Endian, in Amperes (e.g., 5.20)
3. **Power** (bytes 8-11): 32-bit float, Little-Endian, in Watts (e.g., 1146.6)
4. **Energy** (bytes 12-15): 32-bit float, Little-Endian, in kWh (e.g., 2.50)

**How Dashboard Parses the Response**:
```javascript
// Dashboard uses JavaScript DataView to read floats
const voltage = view.getFloat32(0, true);   // true = little-endian
const current = view.getFloat32(4, true);
const power = view.getFloat32(8, true);
const energy = view.getFloat32(12, true);
```

**Request Frequency**: Dashboard sends this request every **1 second** when connected.

---

### 2.2 RELAY_CONTROL (Command 0x09)

**When Dashboard Sends**:
```
[0xAA][0x02][0x09][RelayNumber][State]
```

Where:
- RelayNumber: 1-4 (which relay to control)
- State: 0 = OFF, 1 = ON

**What Dashboard Expects**:
- The embedded system should turn the specified relay ON or OFF
- Optional: Send acknowledgment back to confirm action

**Dashboard UI**: 
- Has 4 toggle switches (Relay 1-4)
- Has "All ON" and "All OFF" master buttons
- Shows relay status visually

---

### 2.3 CALIBRATE_SENSORS (Command 0x02)

**When Dashboard Sends**:
```
[0xAA][0x01][0x02][0x01]
```

**What Dashboard Expects**:
- Embedded system should trigger calibration process
- Optional: Send acknowledgment or calibration status

**Dashboard UI**:
- Input fields for voltage factor (0.5 - 2.0)
- Input fields for current factor (0.5 - 2.0)
- "Apply Calibration" button

---

### 2.4 UPDATE_WIFI (Command 0x0C) - Optional for Remote Mode

**When Dashboard Sends**:
```
[0xAA][LEN][0x0C][SSID bytes...][,][Password bytes...][0x00]
```

Format:
- SSID characters
- Comma separator (0x2C)
- Password characters
- Null terminator (0x00)

**What Dashboard Expects**:
- Embedded system (ESP-01) should connect to specified WiFi network
- This enables Remote Mode operation

**Dashboard UI**:
- SSID input field
- Password input field
- "Save via Bluetooth" button (sends config over Bluetooth to ESP-01)

---

## 3. Data Format Specifications

### 3.1 Float32 Format (IEEE 754)

Dashboard expects **standard 32-bit floating point numbers** as defined by IEEE 754.

**Byte Order**: **Little-Endian** (least significant byte first)

Example: 220.5V represented as:
```
Float32: 0x435C8000
Little-Endian bytes: [0x00, 0x80, 0x5C, 0x43]
```

### 3.2 Expected Value Ranges

| Parameter | Unit | Typical Range | Display Precision |
|-----------|------|---------------|-------------------|
| Voltage   | V    | 0 - 300       | 1 decimal place   |
| Current   | A    | 0 - 20        | 2 decimal places  |
| Power     | W    | 0 - 5000      | 1 decimal place   |
| Energy    | kWh  | 0 - 9999      | 1 decimal place   |

---

## 4. Dashboard UI Features and Expected Data Flow

### 4.1 Live Dashboard View

**Displays**:
- Voltage gauge (0-300V range)
- Current gauge (0-20A range)
- Power gauge (0-5000W range)
- Energy counter (cumulative kWh)

**Statistics Calculated by Dashboard**:
- Uptime (calculated from connection time)
- Estimated cost (Energy × $0.15 per kWh)
- Average power (running average)
- Peak power (maximum observed)

### 4.2 Real-time Chart

**What Dashboard Shows**:
- Power consumption over time
- Rolling window of last 20 data points
- Updates every second with new data

**What Dashboard Needs**:
- Continuous stream of power values via GET_RMS_DATA

### 4.3 Relay Control View

**What Dashboard Shows**:
- 4 relay cards with ON/OFF toggles
- Per-relay load display (calculated as: total power when relay is ON)
- Per-relay runtime tracking

**What Dashboard Needs**:
- Ability to send RELAY_CONTROL commands
- Relays should respond to commands

---

## 5. Connection and Communication Behavior

### 5.1 Local Mode (Bluetooth) Operation

**Connection Sequence**:
1. User clicks "Connect Device" button
2. Browser shows serial port selection dialog
3. User selects COM port (HC-05)
4. Dashboard opens serial connection at 9600 baud
5. Dashboard starts sending GET_RMS_DATA every 1 second

**Expected Embedded Behavior**:
- HC-05 module should be discoverable
- Baud rate: 9600
- Should respond to each GET_RMS_DATA request within 100ms

### 5.2 Remote Mode (WiFi) Operation

**Connection Sequence**:
1. User clicks "Mode" button to switch to Remote
2. User clicks "Connect Server"
3. Dashboard connects to WebSocket server (localhost:3000)
4. Server relays data from ESP-01
5. Dashboard starts requesting data through server

**Expected Embedded Behavior**:
- ESP-01 should be connected to WiFi network
- ESP-01 should maintain TCP connection to server (port 3001)
- ESP-01 should forward UART data from ATmega32 to server

### 5.3 Demo Mode (No Embedded Required)

Dashboard has built-in demo mode with simulated data:
- Generates random voltage: 220V ± 5V
- Generates random current: 0.5A ± 0.1A
- Calculates power: V × I
- Accumulates energy over time

This allows testing Dashboard without embedded hardware.

---

## 6. Data Update and Timing Requirements

### 6.1 Requested Update Rate
- Dashboard sends GET_RMS_DATA every **1 second**
- This is the minimum required for smooth UI updates

### 6.2 Response Time Expectations
- Embedded should respond within **100ms** of receiving command
- Delays longer than 500ms may cause UI to show "timeout" warnings

### 6.3 Data Freshness
- Dashboard assumes received data represents **current moment** measurements
- Energy counter should be **cumulative** (not reset between requests)

---

## 7. Error Handling and Edge Cases

### 7.1 Missing or Invalid Data

**If Dashboard receives corrupted frame**:
- Ignores frame and waits for next valid frame
- No display update occurs
- Previous values remain shown

**If no data received for 5 seconds**:
- Dashboard may show "Connection Lost" warning
- Statistics freeze at last known values

### 7.2 Out-of-Range Values

**Dashboard Display Behavior**:
- Voltage > 300V: Shows in red, may trigger alert
- Current > 20A: Shows in red, may trigger alert  
- Negative values: Displays as 0.0
- NaN or Infinity: Displays as "---"

---

## 8. Summary of What Embedded Must Provide

### Minimum Requirements (Local Mode Only):
1. ✅ HC-05 Bluetooth module configured at 9600 baud
2. ✅ Response to GET_RMS_DATA (0x05) with 16 bytes of Float32 data
3. ✅ Data format: 4 floats (V, I, P, E) in Little-Endian byte order
4. ✅ Response time < 100ms per request
5. ✅ Relay control functionality responding to command 0x09

### Additional Requirements (Remote Mode):
6. ⭕ ESP-01 WiFi module initialized
7. ⭕ WiFi configuration handler (command 0x0C)
8. ⭕ TCP connection to IoT server (port 3001)

### Nice to Have:
9. ⭕ Calibration command handler (0x02)
10. ⭕ Command acknowledgments for better UX

---

## 9. Protocol Example Session

**Dashboard Connects and Starts Monitoring**:
```
Dashboard → [0xAA][0x00][0x05]                          (Request data)
Embedded  → [0xAA][0x10][0x05][16 bytes of floats]      (Send V,I,P,E)

... (1 second delay) ...

Dashboard → [0xAA][0x00][0x05]                          (Request data again)
Embedded  → [0xAA][0x10][0x05][16 bytes of floats]      (Send updated values)

... (user clicks Relay 1 ON) ...

Dashboard → [0xAA][0x02][0x09][0x01][0x01]              (Relay 1 ON)
Embedded  → [Relay 1 turns ON]
Embedded  → [0xAA][0x02][0x09][0x01][0x01]              (Optional ACK)
```

---

## 10. Testing Dashboard Without Full Embedded Implementation

### Option 1: Demo Mode
Click "Demo Mode" button - Dashboard generates simulated data

### Option 2: Partial Implementation
Implement only GET_RMS_DATA response:
- Dashboard will show live data
- Relay control will send commands (but nothing happens)
- This allows testing Dashboard UI and charts

### Option 3: Echo Test
Have embedded echo back any received command:
- Helps verify communication is working
- Can see if Dashboard is sending correct commands

---

## Notes

- This document describes **Dashboard's perspective only**
- For embedded code fixes and implementation details, refer to: `Code_Analysis_ReportV2.md`
- Dashboard code is complete and working - no changes needed on Dashboard side
- All required changes are on embedded system side

---

**Document Version**: 1.0  
**Last Updated**: 2025-12-15  
**Contact**: Hesham Ahmed (Hisham4Ahmed@gmail.com)
