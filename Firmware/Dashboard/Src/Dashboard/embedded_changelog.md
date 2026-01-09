# Embedded System Code Changes Log

This document details the modifications made to the firmware to enable full integration with the Smart Energy Management Dashboard.

## 1. New Drivers Added

### 1.1. WiFi Driver (ESP-01)
**Reason**: To enable remote connectivity alongside Bluetooth.
**Files Created**:
- `Hal/ESP01/ESP01_Interface.h`: Defines the API (`hEsp01_init`, `hEsp01_SendBuffer`).
- `Hal/ESP01/ESP01_Program.c`: Implements the driver using UART.

## 2. Modified Files

### 2.1. `App/CommunicationManager/App_CommManager.c`

#### A. Protocol Logic Fix
- **Issue**: The original code had `FrameLen = 5;` hardcoded.
- **Fix**: Changed to `FrameLen = value;`.

#### B. Command Processing Implementation
- **`GET_RMS_DATA` (0x05)**: Retrieves and sends measurements (V, I, P, E).
- **`Calibrate_Sensors` (0x0B)**: Forwards calibration data.
- **[NEW] `CMD_UPDATE_WIFI` (0x0C)**:
    - Receives `SSID,PASSWORD` string via Bluetooth.
    - Saves them to **EEPROM** (Addr 100 & 150).

#### C. Dual-Channel Response
- Added `hEsp01_SendBuffer` to `App_CommManager_SendFrame`.

#### D. Initialization
- Added `hEsp01_init()` call.

### 2.2. `Hal/ESP01/ESP01_Program.c`

#### A. Dynamic Configuration
- **Old**: Credentials were Macros (`#define WIFI_SSID`).
- **New**: `hEsp01_init` reads the SSID and Password dynamically from **EEPROM** addresses 100/150 at startup.

## 3. Configuration & Integration
- Included necessary headers (`EEPROM_Interface.h`).

---
**How to Provision WiFi**:
1. Connect Dashboard via Bluetooth.
2. Go to **Settings -> WiFi Setup**.
3. Enter Name/Pass and click "Save".
4. Reset the Device. It will now connect to the new network.
