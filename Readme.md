
# 📘 Smart Energy Meter with Control

## ✅ Objective

Develop a smart energy monitoring and control system that measures household electricity consumption, displays data in real-time, and provides overload protection with wireless monitoring.

---

## 🧠 Features Covered

* Real-time Current & Voltage Measurement (via ACS712 + Voltage Divider).
* Power (W) and Energy (kWh) Calculation.
* LCD Display Interface.
* Overload Detection with Alerts (Buzzer + LED).
* Load Control via Relay (Automatic cut-off on overload).
* Data Logging with RTC (optional).
* Wireless Communication:

  * **HC-05 Bluetooth** → Local mobile monitoring.
  * **ESP8266 Wi-Fi** → Remote/cloud monitoring.

---

## 🧱 Hardware Components

* ATmega32 Microcontroller
* ACS712 Current Sensor
* Voltage Divider for AC Voltage Measurement
* 16x4 LCD
* Relay Module (5V, opto-isolated)
* Buzzer + LED (Overload Alert)
* Push Buttons (Mode / Reset)
* HC-05 Bluetooth Module
* ESP8266 Wi-Fi Module
* Power Supply (220V AC → 5V DC regulated)

---

## 📌 Pin Mapping

* **ACS712 Current Sensor** → ADC0 (PA0)
* **Voltage Divider** → ADC1 (PA1)
* **LCD Data** → PC0–PC3
* **LCD Control**: RS → PC4, EN → PC5
* **Relay Control** → PD4
* **Buzzer** → PD5
* **LED Alert** → PD6
* **Push Button 1 (Mode)** → PD2
* **HC-05 Bluetooth** → UART0 (PD0–PD1)
* **ESP8266 Wi-Fi** → UART1 (or Software UART)

---

## 🧭 System Workflow

1. MCU continuously reads analog signals from ACS712 and Voltage Divider.
2. Calculate Irms, Vrms, Power (P = V × I), and cumulative Energy (kWh).
3. Display values on LCD in real-time.
4. If power exceeds Threshold:

   * Activate Buzzer + LED.
   * Disconnect load via Relay.
5. User can reset system or change display modes via buttons.
6. Data can be monitored via Bluetooth (mobile app) or Wi-Fi (cloud).

---

## 📊 Threshold Table (Example)

| Parameter   | Limit       | Action                       |
| ----------- | ----------- | ---------------------------- |
| Current (I) | > 10A       | Trigger overload protection  |
| Power (P)   | > 2000W     | Buzzer + LED + Cut-off Relay |
| Energy (E)  | > 5 kWh/day | Display warning message      |

---

### 📱 Mobile App Mockup

* **Bluetooth Tab**:

  * Show Current (A), Power (W), Energy (kWh).
  * Button to Reset Energy.
* **Wi-Fi Tab**:

  * Upload data to **Web Server**.
  * **Dashboard Website** shows:
    * Current, Power, Energy in widgets.
    * Graphs of energy consumption over time.
    * Alerts panel for overload.
    * Option to download reports.

---

### This Project Made By Gestell Company
