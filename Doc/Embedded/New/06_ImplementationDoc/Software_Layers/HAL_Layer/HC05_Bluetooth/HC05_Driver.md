# 🔵 HC-05 Bluetooth Module Driver

<div align="center">

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-HAL_Layer-blue)
![License](https://img.shields.io/badge/License-Gestell-orange)
![Type](https://img.shields.io/badge/Type-Hardware_Driver-brightgreen)

**HC-05 Driver**

**Smart Energy Management System - Serial Wireless Bridge**

_Developed by Gestell Company - Professional Embedded Solutions_

</div>

---

## 📋 Table of Contents

- [Module Overview](#-1-module-overview)
- [Hardware Architecture](#-2-hardware-architecture)
- [Operating Modes](#-3-operating-modes)
- [AT Command Set](#-4-at-command-reference)
- [Packet Protocol](#-5-packet-protocol-structure)
- [State Machine](#-6-state-machine)
- [Sequence Diagrams](#-7-sequence-diagrams)
- [Configuration](#-8-configuration-parameters)
- [Dependencies](#-9-module-dependencies)

---

## 🔗 Related Documentation

| Document                                                                                               | Description    | Status       |
| ------------------------------------------------------------------------------------------------------ | -------------- | ------------ |
| **[UART_Driver.md](../../MCAL_Layer/UART/UART_Driver.md)**                                             | Serial Bus     | ✅ Available |
| **[Communication_Manager.md](../../Application_Layer/Communication_Manager/Communication_Manager.md)** | Protocol Logic | ✅ Available |

---

## 📋 1. Module Overview

### Purpose and Role

The HC-05 Driver facilitates wireless serial communication between the Energy Meter and an Android/iOS mobile application. It acts as a transparent bridge, converting UART serial frames into Bluetooth 2.0 (Classic) radio packets.

This module is essential for the "Technician Mode", allowing field engineers to configure thresholds, read error logs, and calibrate sensors without physical access to the device (up to 10 meters).

### Key Responsibilities

- **Transparent Bridge**: Passing bytes from UART Rx to Bluetooth Air, and Air to UART Tx.
- **Configuration**: Entering AT Mode to set Name, PIN, and Baud Rate.
- **State Monitoring**: Detecting connection status via the STATE pin.
- **Security**: Managing PIN pairing (default "1234").

---

## 2. Hardware Architecture

### 2.1 Interface Schematic

```mermaid
graph LR
    subgraph "Microcontroller (5V)"
        TX[UART TX]
        RX[UART RX]
        KEY[Key/EN Pin]
        STATE[State Pin]
    end

    subgraph "Level Shifter"
        R1[1 kΩ]
        R2[2.2 kΩ]
    end

    subgraph "HC-05 (3.3V Logic)"
        BT_RX[RXD Pin]
        BT_TX[TXD Pin]
        BT_KEY[Key Pin]
        BT_STATE[State Pin]
    end

    TX --> R1
    R1 --> BT_RX
    R1 --> R2
    R2 --> GND

    BT_TX --> RX

    KEY --> BT_KEY
    BT_STATE --> STATE

    style BT_RX fill:#E67E22,color:#fff
```

### 2.2 Pin Descriptions

1.  **RXD (HC-05)**: Receiving Data. **3.3V Logic Limit!** Requires divider.
2.  **TXD (HC-05)**: Transmitting Data. 3.3V Logic, but safe for 5V MCU input ($V_{IH} > 2.0V$).
3.  **KEY**: Pull HIGH during power-on to enter AT Command Mode.
4.  **STATE**: LOW = Disconnected. HIGH = Connected. Used by driver to pause transmission if link is lost.

---

## 3. Operating Modes

The HC-05 has two distinct operational states.

| Mode          | Entry Condition | LED Pattern                                     | Baud Rate         | Function         |
| ------------- | --------------- | ----------------------------------------------- | ----------------- | ---------------- |
| **Data Mode** | Standard Boot   | Fast Blink (Pairing) / Double Blink (Connected) | Configured (9600) | Transparent Data |
| **AT Mode**   | KEY High @ Boot | Slow Blink (2s period)                          | Fixed (38400)     | Configuration    |

> **Driver Strategy**: The driver operates primarily in Data Mode. AT Mode is only used during "Factory Reset" or generic setup, requiring a specific boot sequence.

---

## 4. AT Command Reference

Although rarely used in runtime, these commands are essential for setup.

| Command    | Description     | Parameter Example      | Response |
| ---------- | --------------- | ---------------------- | -------- |
| `AT`       | Test            | -                      | `OK`     |
| `AT+NAME`  | Set Device Name | `AT+NAME=GestellMeter` | `OK`     |
| `AT+PSWD`  | Set Pairing PIN | `AT+PSWD=8888`         | `OK`     |
| `AT+UART`  | Set Baud Rate   | `AT+UART=9600,0,0`     | `OK`     |
| `AT+ROLE`  | Master/Slave    | `0`=Slave, `1`=Master  | `OK`     |
| `AT+RESET` | Soft Reboot     | -                      | `OK`     |
| `AT+ORGL`  | Restore Factory | -                      | `OK`     |

---

## 5. Packet Protocol Structure

To ensure data integrity over the noisy wireless link, we use a simple frame structure.

### 5.1 JSON Frame (ASCII)

User readable, easy to debug.

```json
{
  "T": "DATA",
  "V": 220.5,
  "I": 5.12,
  "E": 150
}
```

- **Overhead**: High.
- **Parsing**: Requires `scanf` or parser.
- **Usage**: Technician Dashboard.

### 5.2 Binary Frame (Hex)

Compact, efficient.

| Start Byte | Cmd ID | Payload Len | Payload [0..N] | Checksum (XOR) | End Byte |
| ---------- | ------ | ----------- | -------------- | -------------- | -------- |
| `0xAA`     | `0x01` | `0x04`      | `00 00 00 00`  | `0x55`         | `0xFF`   |

- **Overhead**: Low (4 bytes).
- **Usage**: Firmware Upload, Raw Logging.

---

## 6. State Machine

```mermaid
stateDiagram-v2
    [*] --> Disconnected

    Disconnected --> Pairing: Power On

    state Pairing {
        [*] --> Scanning
    }

    Pairing --> Connected: Mobile Device Links

    state Connected {
        Idle --> Transmitting: Send Data
        Transmitting --> Idle: Complete

        Idle --> Receiving: RX Interrupt
        Receiving --> Parsing: End of Frame
        Parsing --> Idle: Valid/Invalid
    }

    Connected --> Disconnected: Link Lost (State Pin Low)
    Connected --> AT_Config: Mode Switch Request
```

---

## 7. Sequence Diagrams

### 7.1 Connection & Data Exchange

```mermaid
sequenceDiagram
    participant MOBILE as Android App
    participant HC05 as HC-05 Module
    participant DRV as Driver
    participant APP as Main App

    Note over HC05: Blinking Fast (Pairing)

    MOBILE->>HC05: Connect (PIN 1234)
    HC05->>HC05: Verify PIN
    HC05-->>MOBILE: Connected

    HC05->>DRV: Set STATE Pin HIGH
    DRV->>APP: Event: BT_CONNECTED

    Note over HC05: Blinking Slowly (Connected)

    APP->>DRV: Send_Telemetry(220V)
    DRV->>HC05: UART TX "220.0"
    HC05->>MOBILE: RF Data "220.0"

    MOBILE->>HC05: CMD "RELAY_OFF"
    HC05->>DRV: UART RX "RELAY_OFF"
    DRV->>APP: Callback("RELAY_OFF")
```

---

## 8. Configuration Parameters

Configured in `HC05_Cfg.h`.

| Parameter        | Default  | Description                        |
| ---------------- | -------- | ---------------------------------- |
| `BT_UART_BAUD`   | `9600`   | Must match module setting.         |
| `STATE_PIN`      | `PIN_D4` | Connection monitor input.          |
| `KEY_PIN`        | `PIN_D5` | AT Mode control output.            |
| `RX_BUFFER_SIZE` | `64`     | RAM buffer for incoming commands.  |
| `TX_BUFFER_SIZE` | `64`     | RAM buffer for outgoing telemetry. |

---

## 9. Module Dependencies

```mermaid
graph TD
    HC05[HC-05 Driver] --> UART[UART Driver]
    HC05 --> DIO[DIO Driver]

    APP[Comm Manager] --> HC05

    style HC05 fill:#F39C12,color:#000
    style UART fill:#4A90E2,color:#fff
```

### 9.1 UART Conflict

**Issue**: The ATmega32 has only one UART.
**Conflict**: ESP-01 and HC-05 both need UART.
**Solution**:

1.  **Multiplexing**: Use logic gates to switch TX/RX lines based on a `SELECT` pin.
2.  **Software UART**: Use a Bit-Banging driver for the slower device (HC-05 @ 9600).
3.  **Exclusive Mode**: User selects "WiFi Mode" OR "Bluetooth Mode" via button.

> **Current Implementation**: **Exclusive Mode**. The system boots in WiFi mode by default. Holding a button switches to Bluetooth mode (swapping RX/TX ISR focus).

---

<div align="center">

**Built with ❤️ by Gestell Team**

_Professional Embedded Systems Engineering_

**Copyright © 2025-2026 Gestell Company - All Rights Reserved**

</div>
