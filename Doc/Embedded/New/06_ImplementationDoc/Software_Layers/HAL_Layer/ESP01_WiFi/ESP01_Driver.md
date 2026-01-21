# ESP-01 WiFi Module Driver

**Module**: ESP-01 (ESP8266)  
**Interface**: UART (AT Commands)  
**Purpose**: WiFi connectivity for cloud integration and remote monitoring

---

## 1. Module Overview

### Purpose and Role

The ESP-01 driver provides WiFi connectivity through the ESP8266 module, enabling the Smart Energy Management System to transmit data to cloud platforms, mobile apps, and web dashboards.

### Key Responsibilities

- WiFi network connection management
- AT command protocol handling
- TCP/IP data transmission
- Connection status monitoring
- Error recovery and reconnection

### Hardware Component

- SoC: ESP8266 WiFi chip
- WiFi: 802.11 b/g/n (2.4 GHz)
- Modes: Station (client) or Access Point
- Interface: AT commands via UART
- Power: 3.3V (requires level shifter from 5V MCU)

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "ESP-01 Driver Architecture"
        APP[Application/<br/>Communication Manager] -->|Send Data| ESP_API[ESP-01 Driver]

        ESP_API --> AT_CMD[AT Command<br/>Generator]
        ESP_API --> STATE[Connection<br/>State Machine]
        ESP_API --> PARSER[Response<br/>Parser]

        AT_CMD --> UART[UART Driver<br/>9600/115200 baud]

        UART -->|TX| LEVEL[Level Shifter<br/>5V→3.3V]
        LEVEL --> ESP_TX[ESP-01 RX Pin]

        ESP_RX[ESP-01 TX Pin] -->|3.3V| UART_RX[UART RX<br/>Direct Connection]

        ESP_TX --> ESP_MODULE[ESP8266<br/>WiFi Module]
        ESP_RX --> ESP_MODULE

        ESP_MODULE --> WIFI[WiFi Network<br/>802.11 b/g/n]

        WIFI --> CLOUD[Cloud Server<br/>Mobile App]
    end

    style ESP_API fill:#4A90E2,color:#fff
    style ESP_MODULE fill:#E24A4A,color:#fff
    style CLOUD fill:#50C878,color:#fff
```

---

## 3. AT Command Protocol

### Common Commands

| Command     | Purpose              | Response     | Example                         |
| ----------- | -------------------- | ------------ | ------------------------------- |
| AT          | Test communication   | OK           | Basic connectivity check        |
| AT+GMR      | Get firmware version | Version info | Firmware verification           |
| AT+CWJAP    | Connect to WiFi      | OK or ERROR  | AT+CWJAP="SSID","password"      |
| AT+CIFSR    | Get IP address       | IP address   | Check connection status         |
| AT+CIPSTART | Start TCP connection | OK/CONNECT   | AT+CIPSTART="TCP","server",port |
| AT+CIPSEND  | Send data            | SEND OK      | AT+CIPSEND=length, then data    |
| AT+CIPCLOSE | Close connection     | CLOSED       | Disconnect from server          |

---

## 4. Connection Sequence

```mermaid
sequenceDiagram
    participant APP as Application
    participant ESP as ESP-01 Driver
    participant MOD as ESP8266 Module
    participant WIFI as WiFi Network
    participant SRV as Server

    APP->>ESP: Initialize WiFi
    ESP->>MOD: AT (test)
    MOD->>ESP: OK

    ESP->>MOD: AT+CWJAP="SSID","password"
    MOD->>WIFI: Connect request
    WIFI->>MOD: Connected
    MOD->>ESP: OK

    ESP->>MOD: AT+CIFSR (get IP)
    MOD->>ESP: IP: 192.168.1.100

    APP->>ESP: Send data to server
    ESP->>MOD: AT+CIPSTART="TCP","server.com",8080
    MOD->>SRV: TCP connection
    SRV->>MOD: ACK
    MOD->>ESP: CONNECT

    ESP->>MOD: AT+CIPSEND=50
    MOD->>ESP: >
    ESP->>MOD: {JSON data payload}
    MOD->>SRV: Transmit data
    SRV->>MOD: ACK
    MOD->>ESP: SEND OK

    ESP->>MOD: AT+CIPCLOSE
    MOD->>SRV: Close connection
    MOD->>ESP: CLOSED
```

---

## 5. State Machine

```mermaid
stateDiagram-v2
    [*] --> Disconnected

    Disconnected --> Initializing: Power ON
    Initializing --> Ready: AT OK

    Ready --> Connecting: CWJAP command
    Connecting --> Connected: WiFi joined
    Connecting --> Disconnected: Timeout/Error

    Connected --> TCPOpen: CIPSTART
    TCPOpen --> DataTransmit: CIPSEND
    DataTransmit --> TCPOpen: SEND OK
    TCPOpen --> Connected: CIPCLOSE

    Connected --> Disconnected: Connection lost

    note right of Initializing
        AT command test
        Firmware version check
        Set mode
    end note

    note right of Connecting
        Connect to AP
        Wait for IP
        Timeout: 10 seconds
    end note
```

---

## 6. Configuration Parameters

| Parameter      | Value         | Description                     |
| -------------- | ------------- | ------------------------------- |
| UART Baud Rate | 9600 / 115200 | Configurable (9600 more stable) |
| WiFi Mode      | Station       | Connect to existing network     |
| IP Assignment  | DHCP          | Automatic IP from router        |
| TCP Timeout    | 30 seconds    | Connection timeout              |
| Max Retry      | 3 attempts    | Reconnection attempts           |
| Keep-alive     | 60 seconds    | Periodic ping                   |

---

## 7. Power Management

**Power Requirements:**

- Operating voltage: 3.3V ± 0.3V (strict)
- Peak current: 200-300 mA (transmission)
- Average current: 80 mA (connected)
- Minimum regulator capacity: 500 mA

**Level Shifting:**

- TX (MCU→ESP): Voltage divider (5V→3.3V)
- RX (ESP→MCU): Direct connection (3.3V recognized as HIGH)

---

## 8. Error Handling

**Connection Failures:**

- Wrong password: Retry with correct credentials
- Network unavailable: Wait and retry
- Weak signal: Move closer or change antenna

**Communication Errors:**

- AT timeout: Module reset required
- Garbled response: Baud rate mismatch
- No response: Power supply issue

**Recovery Procedure:**

1. Detect error condition
2. Log error type
3. Close connections
4. Reset module (toggle power or RST pin)
5. Re-initialize
6. Retry connection

---

## 9. Module Dependencies

```mermaid
graph TB
    COMM[Communication Manager] -->|WiFi Data| ESP[ESP-01 Driver]
    CLOUD[Cloud Integration] -->|MQTT/HTTP| COMM

    ESP -->|AT Commands| UART[UART Driver]
    ESP -.->|Reset Control| DIO[DIO Driver]

    style ESP fill:#4A90E2,color:#fff
```

---

## 10. Performance Characteristics

**Data Throughput:**

- Maximum: ~1 Mbps (802.11b)
- Typical: 100-300 kbps (depends on signal)
- Latency: 50-200 ms (to internet)

**Resource Usage:**

- RAM: ~100 bytes (buffers, state)
- Flash: ~800 bytes
- UART: Shared with HC-05 (software mux required)

**Reliability:**

- Connection uptime: 95%+ (good signal)
- Packet loss: < 1% (typical)
- Reconnect time: 5-10 seconds

---

## Implementation Notes

### Stability Considerations

**Power Supply:**

- Low-ESR capacitors: 100µF + 10µF close to module
- Stable 3.3V regulator essential
- Poor power = frequent resets

**AT Command Timing:**

- Wait for response before next command
- Timeout: 1-5 seconds per command
- Buffer responses (may arrive in chunks)

**Baud Rate Selection:**

- 9600: More stable, lower speed
- 115200: Faster, may have errors
- Default often 115200, change to 9600 for reliability

---

**Document Version**: 2.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
