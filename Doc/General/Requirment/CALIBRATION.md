# Smart Energy Management System – Calibration Guide

This document explains how to calibrate the **ACS712 current sensor** and the **voltage divider circuit** to ensure accurate measurements.

---

## 1. ACS712 Current Sensor Calibration

### 1.1 Zero-Current Offset Calibration

- The ACS712 output at **0A** is ideally **Vcc/2** (≈ 2.5V at 5V supply).  
- Due to manufacturing tolerances, the actual zero-current voltage may vary.  
- **Procedure:**
  1. Power up the system with **no load connected**.
  2. Read multiple ADC samples from the ACS712 input (e.g., 100 samples).
  3. Compute the average value → this is the **offset**.
  4. Store this offset in firmware (`offset_adc_value`) and subtract it from every measurement.

### 1.2 Sensitivity Calibration

- Datasheet sensitivity for ACS712-30A = **66 mV/A**.  
- Actual sensitivity may vary.  
- **Procedure:**
  1. Connect a known resistive load (e.g., a heater or incandescent bulb) with a **known current** (measured by a calibrated multimeter).
  2. Record the ADC readings and calculate the measured current.
  3. Compare with the multimeter value.
  4. Adjust the **scaling factor** in firmware until the readings match.

---

## 2. Voltage Divider Calibration

### 2.1 Divider Design

- Voltage divider reduces **220VAC RMS (≈311V peak)** to a safe ADC input range (<5V).  
- Use **high-voltage resistors** with proper power rating.  
- Example:  
  - R1 = 330kΩ, R2 = 4.7kΩ → Scale factor ≈ 70:1.  
  - Output voltage at 220VAC RMS ≈ 3.14V peak (safe for 5V ADC).

### 2.2 Calibration Procedure

1. Apply a known AC mains voltage (e.g., 220VAC measured with a multimeter).  
2. Capture multiple ADC samples from the voltage divider.  
3. Calculate RMS voltage using firmware.  
4. Compare firmware value with multimeter reading.  
5. Adjust the **scaling factor** in firmware (`V_SCALE`) until the two values match.

---

## 3. Recommended Calibration Steps for the Team

1. Perform **offset calibration** for ACS712 on every power-up (auto-calibration).  
2. Perform **sensitivity calibration** once and store the factor in code or EEPROM.  
3. Perform **voltage calibration** once and confirm with at least two different AC levels (e.g., 200VAC and 240VAC).  
4. Document the final scaling factors in the code for reproducibility.

---

## 4. Notes

- Always use isolation equipment when dealing with mains voltage.  
- Use a calibrated multimeter as the reference for both current and voltage calibration.  
- Repeat calibration if the hardware is changed (different sensor, new resistors, etc.).  
