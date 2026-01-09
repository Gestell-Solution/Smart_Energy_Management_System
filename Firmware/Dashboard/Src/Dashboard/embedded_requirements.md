# Embedded System Requirements for Dashboard Integration

This document outlines the missing components and necessary improvements in the embedded firmware to fully support the Smart Energy Management System Dashboard.

## 1. Communication Infrastructure

### 1.1. WiFi Support (ESP-01) - **CRITICAL MISSING COMPONENT**
*   **Current Status**: The `App_CommManager_Init` function comments out `hEsp01_init()`. There is no active driver or initialization for the ESP-01 WiFi module.
*   **Requirement**:
    *   Implement a full HAL driver for the ESP-01 module (AT commands or passthrough).
    *   Integrate ESP-01 into the `CommunicationManager`.
    *   Enable switching between or concurrent use of Bluetooth (HC-05) and WiFi.
    *   **Goal**: Allow remote monitoring via WiFi/Internet, not just local Bluetooth.

### 1.2. Bluetooth (HC-05) - *Improvements Needed*
*   **Current Status**: Basic support exists.
*   **Requirement**: Ensure the baud rate is reliable (e.g., 9600 or 115200) and documented.

## 2. Protocol Implementation

### 2.1. Frame parsing - **CRITICAL BUG**
*   **Current Status**: In `App_CommManager.c`, the `WaitLen` state hardcodes the length:
    ```c
    // FrameLen=value;
    FrameLen = 5; // <--- HARDCODED
    ```
    This prevents receiving variable-length frames or frames longer/shorter than 5 bytes.
*   **Requirement**:
    *   Remove hardcoded lengths.
    *   Trust the `Len` byte in the protocol header (`0xAA`, `Len`, `Cmd`, `Data...`).
    *   Implement proper bounds checking (`Max_Buffer_size`).

### 2.2. Command Handling - **INCOMPLETE**
*   **Current Status**: The logic inside `App_CommManager_ProcessCommand` is largely commented out or replaced with debug code:
    ```c
    void App_CommManager_ProcessCommand(uint8_t *frame)
    {
        App_CommManager_SendFrame(&frame[1],frame[0],5); // Echo back
        mDIO_TogglePin(GroupD,PIN3); // Toggle LED
    }
    ```
*   **Requirement**: Uncomment and implement the `switch` case for all Command IDs defined in `App_CommManager.h`:
    *   `Calibrate_Sensors (0x0B)`
    *   `Read_EEPROM (0x03)`
    *   `GET_RMS_DATA (0x05)`: Must return V, I, P, E struct.
    *   `Protection_Manager_danger/Safe`: Needs to trigger/reset relays.

## 3. Data Acquisition & Response

### 3.1. Real-time Data (`GET_RMS_DATA`)
*   **Requirement**: The firmware must serialize the RMS data (Voltage, Current, Power, Energy) into a byte array and send it back when this command is received.
*   **Format Suggestion**:
    *   Voltage (4 bytes float or 2 bytes fixed-point)
    *   Current (4 bytes float or 2 bytes fixed-point)
    *   Power (4 bytes float)
    *   Energy (4 bytes float)

### 3.2. Calibration (`Calibrate_Sensors`)
*   **Requirement**: Accept calibration factors from the dashboard and save them to EEPROM.

## 4. Summary of Necessary Actions
1.  **Fix `App_CommManager.c`**: Remove hardcoded length.
2.  **Implement `App_CommManager_ProcessCommand`**: Add actual logic for reading sensors and controlling relays.
3.  **Develop ESP-01 Driver**: Enable WiFi connectivity.
4.  **Define Payload Structure**: document exact byte order (Little Endian vs Big Endian) for multi-byte sensor values.
