# Task 05: Implement Different Data Formats for Mobile and Dashboard

## Task Information

- **Priority**: 🔴 CRITICAL
- **Assigned to**: Mohamed Diaa
- **Estimated Time**: 10 hours
- **Category**: Communication Layer - Protocol Handler
- **Blocker**: Yes - Dashboard and Mobile expect different formats

---

## Problem Description

### Current Issue

The **Mobile App** and **Dashboard** expect **different data formats** for the same measurement data, but the embedded system currently has NO code to handle these different formats.

### Impact

- ❌ **Mobile App cannot parse data correctly**
- ❌ **Dashboard receives wrong format**
- ❌ **One platform or both will display incorrect values**
- ❌ **No communication protocol implementation exists**

### Requirements

**Mobile App Expects (Command 0x05):**

- Data Type: **Scaled unsigned integers**
- Byte Order: **Big-Endian** (MSB first)
- Total Size: **10 bytes**
- Format:
  - Voltage: uint16, scaled ×10
  - Current: uint16, scaled ×100
  - Power: uint16, scaled ×10
  - Energy: uint32, scaled ×100

**Dashboard Expects (Command 0x05 or 0x15):**

- Data Type: **IEEE 754 Float32**
- Byte Order: **Little-Endian** (LSB first)
- Total Size: **16 bytes**
- Format:
  - Voltage: float32
  - Current: float32
  - Power: float32
  - Energy: float32

---

## Technical Background

### Data Format Comparison

**Example: Voltage = 220.5V**

**Mobile Format (Big-Endian uint16 ×10):**

```
Scaled value: 220.5 × 10 = 2205
Binary: 0000 1000 1001 1101
Big-Endian bytes: [0x08, 0x9D]
           MSB ↑      ↑ LSB

Mobile parsing:
voltage_raw = (bytes[0] << 8) | bytes[1]  // Big-Endian
voltage_actual = voltage_raw / 10.0       // 2205 / 10 = 220.5
```

**Dashboard Format (Little-Endian float32):**

```
Float32 representation: 220.5f
IEEE 754 hex: 0x435C8000
Little-Endian bytes: [0x00, 0x80, 0x5C, 0x43]
              LSB ↑                      ↑ MSB

Dashboard parsing:
voltage = DataView.getFloat32(0, true)  // true = little-endian
```

### Command Strategy

**Option A: Different Command IDs** ✅ RECOMMENDED

- Mobile: Command 0x05 → Returns 10-byte integer format
- Dashboard: Command 0x15 → Returns 16-byte float format
- Pros: Clear separation, easy to implement
- Cons: Need two command handlers

**Option B: Auto-detect** (not recommended)

- Use same command, detect client type
- Complex and error-prone

---

## Files to Create/Modify

### New File 1: `App/CommunicationManager/CM_Config.h`

- Communication protocol configuration

### New File 2: `App/CommunicationManager/CM_Program.c`

- Protocol frame handling
- Command parsing
- Response formatting

### New File 3: `App/CommunicationManager/CM_Interface.h`

- Public API

### Modified File: `main.c`

- Integrate communication manager

---

## Implementation Steps

### Step 1: Define Protocol Configuration (1 hour)

**Create File**: `App/CommunicationManager/CM_Config.h`

```c
#ifndef CM_CONFIG_H
#define CM_CONFIG_H

/******************************************************************************
 * Protocol Frame Format
 ******************************************************************************/

/**
 * Frame Structure: [HEADER][LENGTH][COMMAND][DATA...]
 */
#define CM_FRAME_HEADER              0xAA    // Frame start marker

/******************************************************************************
 * Command Definitions
 ******************************************************************************/

// Mobile App Commands (Integer format)
#define CMD_GET_RMS_DATA_MOBILE      0x05    // Returns 10 bytes (uint16/uint32)
#define CMD_RELAY_CONTROL            0x09    // Control relays
#define CMD_CALIBRATE                0x02    // Calibrate sensors
#define CMD_WRITE_EEPROM             0x10    // Write to EEPROM (energy reset)

// Dashboard Commands (Float format)
#define CMD_GET_RMS_DATA_DASHBOARD   0x15    // Returns 16 bytes (float32)
#define CMD_UPDATE_WIFI              0x0C    // WiFi configuration

/******************************************************************************
 * Response Sizes
 ******************************************************************************/

#define CM_MOBILE_RMS_DATA_SIZE      10      // 2+2+2+4 = 10 bytes
#define CM_DASHBOARD_RMS_DATA_SIZE   16      // 4+4+4+4 = 16 bytes

/******************************************************************************
 * Buffer Sizes
 ******************************************************************************/

#define CM_RX_BUFFER_SIZE            64      // Receive buffer
#define CM_TX_BUFFER_SIZE            32      // Transmit buffer

#endif // CM_CONFIG_H
```

---

### Step 2: Implement Byte Conversion Utilities (2 hours)

**Create File**: `App/CommunicationManager/CM_Program.c`

```c
#include "CM_Interface.h"
#include "CM_Config.h"
#include <string.h>

/******************************************************************************
 * Byte Conversion Utilities
 ******************************************************************************/

/**
 * @brief Convert uint16 to Big-Endian bytes
 * @param value Value to convert
 * @param buffer Output buffer (2 bytes)
 */
static void CM_Uint16_ToBigEndian(uint16_t value, uint8_t* buffer)
{
    buffer[0] = (value >> 8) & 0xFF;   // MSB first
    buffer[1] = value & 0xFF;          // LSB last
}

/**
 * @brief Convert uint32 to Big-Endian bytes
 * @param value Value to convert
 * @param buffer Output buffer (4 bytes)
 */
static void CM_Uint32_ToBigEndian(uint32_t value, uint8_t* buffer)
{
    buffer[0] = (value >> 24) & 0xFF;  // MSB first
    buffer[1] = (value >> 16) & 0xFF;
    buffer[2] = (value >> 8) & 0xFF;
    buffer[3] = value & 0xFF;          // LSB last
}

/**
 * @brief Convert float32 to Little-Endian bytes
 * @param value Float value to convert
 * @param buffer Output buffer (4 bytes)
 * @note Uses union type punning for conversion
 */
static void CM_Float32_ToLittleEndian(float value, uint8_t* buffer)
{
    // Union for type conversion
    union
    {
        float float_value;
        uint8_t bytes[4];
    } converter;

    converter.float_value = value;

    // AVR is little-endian, so bytes are already in correct order
    buffer[0] = converter.bytes[0];    // LSB first
    buffer[1] = converter.bytes[1];
    buffer[2] = converter.bytes[2];
    buffer[3] = converter.bytes[3];    // MSB last
}
```

---

### Step 3: Implement Mobile Data Response (2 hours)

**Add to**: `App/CommunicationManager/CM_Program.c`

```c
/**
 * @brief Send RMS data in Mobile format (Big-Endian integers)
 * @details Command: 0x05
 *          Response: 10 bytes
 *          Format: [V_uint16][I_uint16][P_uint16][E_uint32]
 *          All values: Big-Endian
 */
static void CM_SendRMSData_Mobile(void)
{
    uint8_t buffer[3 + CM_MOBILE_RMS_DATA_SIZE];  // Header + Length + CMD + Data
    uint8_t index = 0;

    // Get measurements
    float voltage = ME_GetVoltageRMS();
    float current = ME_GetCurrentRMS();
    float power = ME_GetPower();
    float energy = ME_GetEnergy_kWh();

    // Scale and convert to integers
    uint16_t voltage_scaled = (uint16_t)(voltage * 10.0f);     // ×10
    uint16_t current_scaled = (uint16_t)(current * 100.0f);    // ×100
    uint16_t power_scaled = (uint16_t)(power * 10.0f);         // ×10
    uint32_t energy_scaled = (uint32_t)(energy * 100.0f);      // ×100

    // Build frame
    buffer[index++] = CM_FRAME_HEADER;                // 0xAA
    buffer[index++] = CM_MOBILE_RMS_DATA_SIZE + 1;    // Length (data + command)
    buffer[index++] = CMD_GET_RMS_DATA_MOBILE;        // 0x05

    // Voltage (Big-Endian uint16)
    CM_Uint16_ToBigEndian(voltage_scaled, &buffer[index]);
    index += 2;

    // Current (Big-Endian uint16)
    CM_Uint16_ToBigEndian(current_scaled, &buffer[index]);
    index += 2;

    // Power (Big-Endian uint16)
    CM_Uint16_ToBigEndian(power_scaled, &buffer[index]);
    index += 2;

    // Energy (Big-Endian uint32)
    CM_Uint32_ToBigEndian(energy_scaled, &buffer[index]);
    index += 4;

    // Send via UART (Bluetooth)
    UART_SendBuffer(buffer, index);

    #ifdef DEBUG_MODE
        char debug[100];
        sprintf(debug, "[CM] Mobile: V=%d, I=%d, P=%d, E=%lu\r\n",
                voltage_scaled, current_scaled, power_scaled, energy_scaled);
        UART_SendString(debug);
    #endif
}
```

---

### Step 4: Implement Dashboard Data Response (2 hours)

**Add to**: `App/CommunicationManager/CM_Program.c`

```c
/**
 * @brief Send RMS data in Dashboard format (Little-Endian floats)
 * @details Command: 0x15
 *          Response: 16 bytes
 *          Format: [V_float32][I_float32][P_float32][E_float32]
 *          All values: Little-Endian
 */
static void CM_SendRMSData_Dashboard(void)
{
    uint8_t buffer[3 + CM_DASHBOARD_RMS_DATA_SIZE];
    uint8_t index = 0;

    // Get measurements (already in float)
    float voltage = ME_GetVoltageRMS();
    float current = ME_GetCurrentRMS();
    float power = ME_GetPower();
    float energy = ME_GetEnergy_kWh();

    // Build frame
    buffer[index++] = CM_FRAME_HEADER;                   // 0xAA
    buffer[index++] = CM_DASHBOARD_RMS_DATA_SIZE + 1;    // Length
    buffer[index++] = CMD_GET_RMS_DATA_DASHBOARD;        // 0x15

    // Voltage (Little-Endian float32)
    CM_Float32_ToLittleEndian(voltage, &buffer[index]);
    index += 4;

    // Current (Little-Endian float32)
    CM_Float32_ToLittleEndian(current, &buffer[index]);
    index += 4;

    // Power (Little-Endian float32)
    CM_Float32_ToLittleEndian(power, &buffer[index]);
    index += 4;

    // Energy (Little-Endian float32)
    CM_Float32_ToLittleEndian(energy, &buffer[index]);
    index += 4;

    // Send via UART
    UART_SendBuffer(buffer, index);

    #ifdef DEBUG_MODE
        char debug[100];
        sprintf(debug, "[CM] Dashboard: V=%.1f, I=%.2f, P=%.1f, E=%.2f\r\n",
                voltage, current, power, energy);
        UART_SendString(debug);
    #endif
}
```

---

### Step 5: Implement Command Handler (2 hours)

**Add to**: `App/CommunicationManager/CM_Program.c`

```c
/**
 * @brief Process received command
 * @param command_id Command byte received
 */
void CM_HandleCommand(uint8_t command_id)
{
    switch(command_id)
    {
        case CMD_GET_RMS_DATA_MOBILE:
            // Mobile app request (integer format)
            CM_SendRMSData_Mobile();
            break;

        case CMD_GET_RMS_DATA_DASHBOARD:
            // Dashboard request (float format)
            CM_SendRMSData_Dashboard();
            break;

        case CMD_RELAY_CONTROL:
            // Handle relay control (both platforms)
            CM_HandleRelayControl();
            break;

        case CMD_CALIBRATE:
            // Trigger sensor calibration
            hVoltage_Calibrate();
            hCurrent_Calibrate();
            break;

        case CMD_WRITE_EEPROM:
            // Reset energy counter
            ME_ResetEnergy();
            // Save to EEPROM (Task 06)
            break;

        default:
            // Unknown command - ignore
            break;
    }
}

/**
 * @brief Handle relay control command
 * @details Expected data: [RelayNumber][State]
 *          RelayNumber: 1-4
 *          State: 0=OFF, 1=ON
 */
static void CM_HandleRelayControl(void)
{
    // Read relay data from RX buffer
    uint8_t relay_number = CM_GetReceivedData(0);
    uint8_t state = CM_GetReceivedData(1);

    // Validate relay number (1-4)
    if (relay_number >= 1 && relay_number <= 4)
    {
        // Convert to 0-indexed
        uint8_t relay_id = relay_number - 1;

        if (state == 1)
        {
            hRelay_On(relay_id);
        }
        else
        {
            hRelay_Off(relay_id);
        }

        // Optional: Send acknowledgment
        // CM_SendACK(CMD_RELAY_CONTROL, relay_number, state);
    }
}
```

---

### Step 6: Testing and Verification (1 hour)

**Test Setup:**

1. Bluetooth connection to PC/phone
2. Serial terminal for monitoring
3. Mobile app and Dashboard ready

**Test 6.1: Mobile App Communication**

```
Procedure:
1. Mobile sends: [0xAA][0x00][0x05]
2. Monitor embedded response

Expected Response:
[0xAA][0x0B][0x05][10 bytes of data]

Verify with known values:
V = 220.5V → scaled = 2205 → bytes [0x08, 0x9D]
I = 5.20A  → scaled = 520  → bytes [0x02, 0x08]
P = 1146.6W → scaled = 11466 → bytes [0x2C, 0xCA]
E = 2.50kWh → scaled = 250 → bytes [0x00, 0x00, 0x00, 0xFA]

Full response:
[0xAA][0x0B][0 x05][0x08][0x9D][0x02][0x08][0x2C][0xCA][0x00][0x00][0x00][0xFA]
```

**Test 6.2: Dashboard Communication**

```
Procedure:
1. Dashboard sends: [0xAA][0x00][0x15]
2. Monitor embedded response

Expected Response:
[0xAA][0x11][0x15][16 bytes of floats]

Verify with float converter tool:
220.5 → 0x435C8000 (LE: 00 80 5C 43)
5.20  → 0x40A66666 (LE: 66 66 A6 40)
etc.
```

**Test 6.3: Relay Control**

```
Mobile/Dashboard sends: [0xAA][0x02][0x09][0x01][0x01]
                        Relay 1 ON

Expected:
✅ Relay 1 turns ON
✅ Can verify with multimeter on relay output
```

---

## Success Criteria

✅ **Mobile app receives correct integer format (Big-Endian)**  
✅ **Dashboard receives correct float format (Little-Endian)**  
✅ **Both platforms display same values**  
✅ **Relay control works from both platforms**  
✅ **No data corruption or parsing errors**  
✅ **Response time < 100ms**

---

## Deliverables

1. ✅ `CM_Config.h` - Protocol configuration
2. ✅ `CM_Program.c` - Communication manager implementation
3. ✅ `CM_Interface.h` - Public API
4. ✅ Test report with hex dumps of responses
5. ✅ Screenshots from Mobile app showing correct values
6. ✅ Screenshots from Dashboard showing correct values
7. ✅ Git commit with clear message

---

## Dependencies

**Blocked By**:

- Task 01, 02, 03 - Needs accurate measurements

**Blocks**:

- Mobile app integration
- Dashboard integration

---

## Related Documentation

- `Doc/Embedded/Issue_Solutions.md` - Section "Communication Protocol Issues #11"
- `Doc/Embedded/DashboardRequirementfromEmbedded.md`
- `Doc/Embedded/MobileRequirementfromEmbedded.md`

---

**Document Version**: 1.0  
**Created**: 2026-01-16  
**Last Updated**: 2026-01-16
