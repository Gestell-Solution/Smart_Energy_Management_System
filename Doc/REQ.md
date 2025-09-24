# Smart Energy Management System – Requirements

## 1. Objective

A smart system for monitoring household energy consumption in real-time, displaying values on an LCD, transmitting data to a mobile/server, and enforcing overload protection.

---

## 2. Functional Requirements

- **FR1 – Current Measurement**  
  - Use ACS712 sensor to measure current in the range of 0–30A.

- **FR2 – Voltage Measurement**  
  - Use a voltage divider to measure voltage in the range of 0–260VAC.

- **FR3 – Power & Energy Calculation**  
  - Calculate instantaneous power \( P = V \times I \).  
  - Calculate cumulative energy consumption in kWh.

- **FR4 – Display**  
  - Display measured values (V, I, P, Energy) on a 16x4 LCD with 1s refresh rate.  

- **FR5 – Protection**  
  - If current or power exceeds a defined threshold (e.g., 10A or 2000W) → trigger buzzer and disconnect load via relay.  

- **FR6 – Communication**  
  - Transmit values to mobile via HC-05 Bluetooth.  
  - Upload data to a cloud server via ESP8266 WiFi.

---

## 3. Non-Functional Requirements

- Accuracy: ±5% for current and voltage measurements after calibration.  
- Update rate: LCD every 1s, server update every 10s.  
- System power consumption ≤ 1W.  
- System must recover safely after unexpected reset or power loss.

## 4. Acceptance Criteria

- **AC1 (Current Measurement):** For a reference current of 2.0A, the reading must be within ±5%.  
- **AC2 (Voltage Measurement):** For 220VAC input, the reading must be between 209–231V.  
- **AC3 (Protection):** When current exceeds 10A, the relay must disconnect the load within ≤ 100ms.  
- **AC4 (Display):** LCD must update values correctly every 1 second.  
- **AC5 (Mobile Communication):** Mobile app must receive updated values via HC-05 on demand.  
- **AC6 (Server Communication):** New values must be uploaded to the server every 10 seconds without interruption.  

---

## 5. Constraints

- MCU: ATmega32 @ 8MHz.  
- Input Voltage: 220VAC @ 50Hz.  
- Sensors: ACS712 (current), Voltage Divider (voltage).  
- Display: 16x4 LCD.  
- Communication Modules: HC-05 Bluetooth, ESP8266 WiFi.
