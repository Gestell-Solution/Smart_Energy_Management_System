# Smart Energy Management System – Hardware Specifications

## 1. Microcontroller Unit (MCU)

- **Model:** ATmega32  
- **Clock Frequency:** 8 MHz (external crystal)  
- **ADC Resolution:** 10-bit  
- **Supply Voltage:** 5V  
- **Key Features Used:**  
  - ADC channels (for current & voltage sensing)  
  - DIO (for relay, buzzer, LCD, Bluetooth, WiFi)  
  - UART (for HC-05 & ESP8266 communication)  

---

## 2. Sensors & Measurement

- **Current Sensor:** ACS712 (30A version)  
  - Measurement Range: ±30A  
  - Sensitivity: 66 mV/A  
  - Output: Analog voltage centered at Vcc/2 (2.5V at 0A)  
- **Voltage Measurement:** Resistive Voltage Divider  
  - Input: 220VAC (scaled down to <5V for ADC input)  
  - Divider resistors rated for high voltage and power dissipation  
  - Isolation and filtering recommended (RC low-pass filter)  

---

## 3. Output & Actuators

- **Relay Module:**  
  - Rated: 250VAC, 10A  
  - Control: 5V coil driven by MCU via transistor driver  
- **Buzzer:**  
  - 5V active buzzer for overload alert  
- **LCD Display:**  
  - 16x4 character LCD (HD44780 compatible)  
  - Interface: 4-bit parallel mode  

---

## 4. Communication Modules

- **Bluetooth:** HC-05  
  - Interface: UART (9600 bps default)  
  - Range: ~10m  
- **WiFi:** ESP8266 (ESP-01 or NodeMCU)  
  - Interface: UART (to ATmega32)  
  - WiFi Standard: IEEE 802.11 b/g/n  

---

## 5. Power Supply

- **Input:** 220VAC  
- **Conversion:**  
  - Step-down transformer + rectifier + regulator, or SMPS module (5V DC output)  
  - Power rating: ≥ 1A to supply MCU + sensors + modules  

---

## 6. Pin Mapping

| Component        | MCU Pin (ATmega32) | Notes                  |
|------------------|---------------------|------------------------|
| ACS712 Current   | PA0 (ADC0)          | Analog input           |
| Voltage Divider  | PA1 (ADC1)          | Analog input           |
| LCD Data D4–D7   | PC0–PC3             | 4-bit data lines       |
| LCD RS           | PC4                 | Register Select        |
| LCD EN           | PC5                 | Enable pin             |
| Relay Control    | PD4                 | Output via transistor  |
| Buzzer           | PD5                 | Output                 |
| LED Indicator    | PD6                 | Output (status alert)  |
| Push Button      | PD2                 | Input (user control)   |
| HC-05 Bluetooth  | PD0 (RX), PD1 (TX)  | UART0 communication    |
| ESP8266 WiFi     | UART1 / Soft UART   | Serial communication   |

---

## 7. Safety Considerations

- Use proper isolation for high-voltage AC measurement.  
- Add fuses/circuit breakers to protect hardware.  
- Ensure relay contacts are rated for expected load.  
- Proper grounding for user and device safety.  
