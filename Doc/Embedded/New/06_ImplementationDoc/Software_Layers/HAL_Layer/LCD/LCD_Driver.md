# LCD Driver - HD44780 Character Display

**Display**: 16×2 Character LCD  
**Controller**: HD44780 Compatible  
**Interface**: 4-bit parallel mode  
**Purpose**: User interface display for real-time system information

---

## 1. Module Overview

### Purpose and Role

The LCD driver controls a 16×2 character display for presenting system information including voltage, current, power, energy consumption, and system status to users.

### Key Responsibilities

- Initialize LCD in 4-bit mode
- Display text strings on 2 lines
- Cursor positioning and control
- Special character support
- Screen clearing and updating

### Hardware Component

- Type: 16 characters × 2 lines alphanumeric
- Controller: Hitachi HD44780 or compatible
- Interface: 4-bit data bus (D4-D7)
- Supply: 5V DC
- Current: 1-2 mA (display) + 15-20 mA (backlight)

---

## 2. Architecture Diagram

```mermaid
graph TB
    subgraph "LCD Driver Architecture"
        APP[Application/<br/>Display Manager] -->|Display Commands| LCD_API[LCD Driver API]

        LCD_API --> INIT[Initialization<br/>Sequence]
        LCD_API --> CMD[Command Mode]
        LCD_API --> DATA[Data Mode]
        LCD_API --> CURSOR[Cursor Control]

        CMD --> PROTOCOL[4-Bit Protocol<br/>Handler]
        DATA --> PROTOCOL

        PROTOCOL --> DIO[DIO Driver]

        DIO --> RS[RS Pin<br/>PD2]
        DIO --> EN[EN Pin<br/>PD3]
        DIO --> D4_7[Data Pins<br/>PD4-PD7]

        RS --> LCD_HW[HD44780<br/>LCD Controller]
        EN --> LCD_HW
        D4_7 --> LCD_HW

        LCD_HW --> DISPLAY[16×2 Character<br/>Display]
    end

    style LCD_API fill:#4A90E2,color:#fff
    style PROTOCOL fill:#E24A4A,color:#fff
    style DISPLAY fill:#50C878,color:#fff
```

---

## 3. Pin Connections

| LCD Pin | Function        | MCU Pin       | Description                     |
| ------- | --------------- | ------------- | ------------------------------- |
| RS      | Register Select | PD2           | 0=Command, 1=Data               |
| E       | Enable          | PD3           | Falling edge triggers operation |
| D4-D7   | Data Bus        | PD4-PD7       | 4-bit parallel data             |
| RW      | Read/Write      | GND           | Write-only mode                 |
| VDD/VSS | Power           | +5V/GND       | Power supply                    |
| V0      | Contrast        | Potentiometer | Contrast adjustment             |
| A/K     | Backlight       | +5V/GND       | LED backlight                   |

---

## 4. 4-Bit Interface Protocol

```mermaid
sequenceDiagram
    participant CPU as MCU
    participant LCD as LCD Controller

    Note over CPU,LCD: Send Byte in 4-Bit Mode

    CPU->>LCD: Set RS (0=CMD / 1=DATA)
    CPU->>LCD: Put HIGH nibble on D4-D7
    CPU->>LCD: Pulse EN: HIGH→LOW
    Note over LCD: Latch upper nibble

    CPU->>LCD: Put LOW nibble on D4-D7
    CPU->>LCD: Pulse EN: HIGH→LOW
    Note over LCD: Latch lower nibble

    Note over LCD: Combine nibbles<br/>Execute command/display char

    CPU->>CPU: Wait 40µs (command exec)
```

---

## 5. Initialization Sequence

```mermaid
flowchart TB
    START[Power ON] --> WAIT1[Wait 40ms<br/>VDD stabilize]
    WAIT1 --> CMD1[Send 0x03<br/>8-bit mode attempt]
    CMD1 --> WAIT2[Wait 4.1ms]
    WAIT2 --> CMD2[Send 0x03 again]
    CMD2 --> WAIT3[Wait 100µs]
    WAIT3 --> CMD3[Send 0x03 third time]
    CMD3 --> CMD4[Send 0x02<br/>Switch to 4-bit]
    CMD4 --> CONF1[Send 0x28<br/>4-bit, 2 lines, 5×8]
    CONF1 --> CONF2[Send 0x0C<br/>Display ON, Cursor OFF]
    CONF2 --> CONF3[Send 0x06<br/>Increment, No shift]
    CONF3 --> CLEAR[Send 0x01<br/>Clear display]
    CLEAR --> WAIT4[Wait 2ms<br/>Clear complete]
    WAIT4 --> READY[LCD Ready]

    style READY fill:#50C878,color:#fff
```

---

## 6. Display Memory Layout

```mermaid
graph LR
    subgraph "DD RAM Addresses"
        L1[Line 1: 0x00-0x0F<br/>16 characters]
        L2[Line 2: 0x40-0x4F<br/>16 characters]
    end

    L1 -.->|Cursor wraps| L2
```

**Cursor Positioning:**

- Line 1, Column 0: Address 0x00, Command 0x80
- Line 2, Column 0: Address 0x40, Command 0xC0
- Line 2, Column 5: Address 0x45, Command 0xC5

---

## 7. Common Commands

| Operation       | Command     | Execution Time | Description               |
| --------------- | ----------- | -------------- | ------------------------- |
| Clear Display   | 0x01        | 1.64ms         | Clear screen, home cursor |
| Return Home     | 0x02        | 1.64ms         | Cursor to position 0      |
| Display ON      | 0x0C        | 40µs           | Enable display            |
| Display OFF     | 0x08        | 40µs           | Blank display             |
| Cursor Position | 0x80 + addr | 40µs           | Set DD RAM address        |

---

## 8. Timing Requirements

| Parameter            | Minimum | Typical | Notes              |
| -------------------- | ------- | ------- | ------------------ |
| Enable pulse width   | 230ns   | 500ns   | HIGH state         |
| Enable cycle time    | 500ns   | 1µs     | Total pulse        |
| Command execution    | 40µs    | 50µs    | Most commands      |
| Clear/Home execution | 1.64ms  | 2ms     | Special commands   |
| Inter-command delay  | 40µs    | 50µs    | Between operations |

**Safe Implementation**: Use 50µs delays between operations, 2ms after clear/home.

---

## 9. Module Dependencies

```mermaid
graph TB
    DISPLAY[Display Manager] -->|Format Text| LCD[LCD Driver]
    ME[Measurement Engine] -->|Values| DISPLAY

    LCD -->|Pin Control| DIO[DIO Driver]

    style LCD fill:#4A90E2,color:#fff
```

---

## 10. Performance Characteristics

**Display Update:**

- Single character: ~100µs
- Full line (16 chars): ~1.6ms
- Full screen (32 chars): ~3.2ms
- Refresh rate: Typically 2-5 Hz

**Resource Usage:**

- RAM: ~50 bytes (driver state)
- Flash: ~600 bytes
- Pins: 6 GPIO (RS, EN, D4-D7)

---

## Implementation Notes

### Character Write Procedure

1. Set RS=HIGH (data mode)
2. Send high nibble + pulse EN
3. Send low nibble + pulse EN
4. Wait 40µs
5. Cursor auto-advances

### String Display

- Loop through characters
- Position cursor first if needed
- Auto-wrap at position 15 to line 2

---

**Document Version**: 2.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
