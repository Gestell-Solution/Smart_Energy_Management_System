# System Block Diagram

**Project**: Smart Energy Management System  
**Version**: 1.0

---

## 1. System Overview Diagram

```mermaid
flowchart TB
    subgraph External["External World"]
        AC[AC Mains<br/>220V 50Hz]
        Load[Electrical<br/>Load]
        User[User<br/>Mobile/Dashboard]
    end

    subgraph Sensors["Sensors"]
        VDiv[Voltage Divider<br/>470kΩ/4.7kΩ]
        ACS[ACS712<br/>Current Sensor]
    end

    subgraph MCU["ATmega32 Microcontroller"]
        ADC[ADC<br/>10-bit]
        CPU[CPU Core<br/>16 MHz]
        EEPROM_MCU[EEPROM<br/>1 KB]
        UART_MCU[UART<br/>9600 bps]
        TIMER[Timers<br/>0,1,2]
    end

    subgraph Display["Display & Indicators"]
        LCD[LCD 16x2<br/>HD44780]
        RGB[RGB LED]
        Buzzer[Buzzer]
    end

    subgraph Actuators["Actuators"]
        Relay[Relay<br/>10A SPDT]
    end

    subgraph Communication["Communication"]
        HC05[HC-05<br/>Bluetooth]
        ESP01[ESP-01<br/>WiFi]
    end

    AC --> VDiv
    AC --> ACS
    ACS --> Load

    VDiv --> ADC
    ACS --> ADC

    ADC --> CPU
    CPU --> EEPROM_MCU
    CPU --> UART_MCU
    CPU --> TIMER

    CPU --> LCD
    CPU --> RGB
    CPU --> Buzzer
    CPU --> Relay

    Relay --> Load

    UART_MCU --> HC05
    UART_MCU --> ESP01

    HC05 -.Bluetooth.-> User
    ESP01 -.WiFi.-> User
```

---

## 2. Hardware Architecture

### 2.1 Power Supply

- **Input**: 5V DC (from wall adapter or USB)
- **Regulators**:
  - 5V rail → ATmega32, LCD, relay, sensors
  - 3.3V rail (AMS1117) → HC-05/ESP-01
- **Protection**: Reverse polarity (Schottky diode), overcurrent (fuse)

### 2.2 Measurement Path

**Voltage Measurement**:

```
AC Mains → Voltage Divider (100:1) → Rectifier → Filter → ADC PA0
```

**Current Measurement**:

```
AC Load Line → ACS712 → RC Filter → ADC PA1
```

### 2.3 Control Path

**Protection Logic**:

```
ADC → CPU (Protection Manager) → Relay Driver (ULN2003) → Relay → Load
```

### 2.4 Communication Path

```
CPU (UART) ↔ Level Shifter ↔ HC-05/ESP-01 ↔ Mobile/Dashboard
```

---

## 3. Software Block Diagram

```mermaid
flowchart TB
    subgraph App["Application Layer"]
        ME[Measurement<br/>Engine]
        PM[Protection<br/>Manager]
        EL[Energy<br/>Logger]
        DM[Display<br/>Manager]
        CM[Communication<br/>Manager]
        CAL[Calibration<br/>Manager]
    end

    subgraph HAL["HAL Layer"]
        V_Sensor[Voltage<br/>Sensor]
        I_Sensor[Current<br/>Sensor]
        LCD_HAL[LCD]
        RGB_HAL[RGB LED]
        Buzzer_HAL[Buzzer]
        Relay_HAL[Relay]
        HC05_HAL[HC-05]
    end

    subgraph MCAL["MCAL Layer"]
        ADC_MCAL[ADC]
        DIO_MCAL[DIO]
        UART_MCAL[UART]
        TIMER_MCAL[Timers]
        EEPROM_MCAL[EEPROM]
        GIE_MCAL[GIE]
    end

    ME --> V_Sensor
    ME --> I_Sensor
    ME --> EL
    ME --> DM
    PM --> ME
    PM --> Relay_HAL
    PM --> DM
    EL --> EEPROM_MCAL
    DM --> LCD_HAL
    DM --> RGB_HAL
    DM --> Buzzer_HAL
    CM --> UART_MCAL
    CM --> ME
    CM --> PM

    V_Sensor --> ADC_MCAL
    I_Sensor --> ADC_MCAL
    LCD_HAL --> DIO_MCAL
    RGB_HAL --> DIO_MCAL
    Buzzer_HAL --> DIO_MCAL
    Relay_HAL --> DIO_MCAL
    HC05_HAL --> UART_MCAL

    ADC_MCAL --> TIMER_MCAL
    GIE_MCAL -.enables.-> TIMER_MCAL
    GIE_MCAL -.enables.-> UART_MCAL
```

---

## 4. Data Flow

### 4.1 Measurement Data Flow

1. Timer1 triggers ADC conversion (100 Hz)
2. ADC ISR stores sample in buffer
3. Measurement Engine processes samples (RMS calculation)
4. Results flow to:
   - Protection Manager (fault checking)
   - Display Manager (LCD update)
   - Energy Logger (EEPROM storage)
   - Communication Manager (transmit to user)

### 4.2 Command Data Flow

1. UART RX interrupt receives data
2. Communication Manager parses command
3. Command routed to appropriate module:
   - GET_DATA → Measurement Engine
   - RELAY_CONTROL → Protection Manager
   - CALIBRATE → Calibration Manager
   - RESET_ENERGY → Energy Logger
4. Response generated and transmitted via UART

---

## 5. Communication Variants

### Version 1: Bluetooth (HC-05)

```
Mobile App ←→ Bluetooth SPP ←→ HC-05 ←→ UART ←→ ATmega32
```

### Version 2: WiFi (ESP-01)

```
Dashboard ←→ WiFi/WebSocket ←→ ESP-01 ←→ UART ←→ ATmega32
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026
