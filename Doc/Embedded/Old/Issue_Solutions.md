# Issue Solutions - Embedded System Code Fixes

## Document Information
**Date:** December 15, 2025  
**Project:** Smart Energy Management System  
**Platform:** ATmega32 Microcontroller  
**Lead Engineer:** Eng. Hesham Ahmed  
**Company:** Gestell  

---

## Table of Contents

1. [MCAL Layer Issues](#mcal-layer-issues)
2. [HAL Layer Issues](#hal-layer-issues)
3. [Application Layer Issues](#application-layer-issues)
4. [Communication Protocol Issues](#communication-protocol-issues)
5. [Missing Requirements](#missing-requirements)

---

## MCAL Layer Issues

### 1. 🔴 CRITICAL: Timer1 Duplicate ISR Handlers

#### Problem Description
The same callback is invoked twice per Timer1 cycle, doubling the sampling rate and energy accumulation, resulting in incorrect measurements.

#### Old Code
```c
// In TIMER1_Program.c
static void (*Timer1_Global_Callback)(void) = Null;

// ISR for Compare Match A
void __vector_7(void) __attribute__((signal));
void __vector_7()
{
    if(Timer1_Global_Callback != Null)
    {
        Timer1_Global_Callback();  // ✅ First call
    }
}

// ISR for Compare Match B  
void __vector_8(void) __attribute__((signal));
void __vector_8()
{
    if(Timer1_Global_Callback != Null)
    {
        Timer1_Global_Callback();  // ❌ SAME CALLBACK - Second call!
    }
}

// In TIMER1_Config.h:
// OCR1A = 1250
// OCR1B = 1250  // Same value!
```

**Impact:** 
- Sampling rate: 200 Hz instead of 100 Hz
- Energy calculation: Double the actual value
- RMS calculations: Wrong time window

#### New Code Pseudocode
```
// Solution 1: Disable OCR1B interrupt (Recommended)
FUNCTION mTIMER1_Init():
    // Configure Timer1 in CTC mode
    SET WGM12 bit in TCCR1B  // CTC mode
    SET ICR1 = 1250          // TOP value
    SET OCR1A = 1250         // Compare value
    
    // Enable ONLY Compare Match A interrupt
    ENABLE OCIE1A interrupt
    // DO NOT enable OCIE1B
    
    // Set prescaler to 64
    SET CS11 and CS10 bits
    
    RETURN

// Keep only __vector_7 ISR
ISR __vector_7():
    IF Timer1_Global_Callback != Null:
        CALL Timer1_Global_Callback()
    ENDIF
    RETURN

// Remove or comment out __vector_8 entirely
```

#### Alternative Solution (if both channels needed)
```
// Solution 2: Separate callbacks for each channel
DECLARE Timer1_CallbackA as function pointer
DECLARE Timer1_CallbackB as function pointer

FUNCTION mTIMER1_RegisterCallback(channel, callback):
    IF channel == TIMER1_CHANNEL_A:
        Timer1_CallbackA = callback
    ELSE IF channel == TIMER1_CHANNEL_B:
        Timer1_CallbackB = callback
    ENDIF
    RETURN

ISR __vector_7():
    IF Timer1_CallbackA != Null:
        CALL Timer1_CallbackA()
    ENDIF
    RETURN

ISR __vector_8():
    IF Timer1_CallbackB != Null:
        CALL Timer1_CallbackB()
    ENDIF
    RETURN
```

---

### 2. ⚠️ MEDIUM: EEPROM No Timeout Protection

#### Problem Description
The EEPROM driver can hang indefinitely if hardware fails, blocking the entire system.

#### Old Code
```c
void mEEPROM_WriteByte(uint16_t Address, uint8_t Data)
{ 
    if (Address > AVR_EEPROM_MAXAddress)
    {
        return;
    }
  
    // ❌ Can hang forever!
    while (GetBit(EECR_Reg, EEWE_Bit) == 1)
    {
        // Wait forever - NO TIMEOUT!
    }

    EEAR_Reg = Address;
    EEDR_Reg = Data;
    SetBit(EECR_Reg, EEMWE_Bit);
    SetBit(EECR_Reg, EEWE_Bit);
}
```

**Impact:**
- If EEPROM fails, system freezes completely
- No error recovery possible

#### New Code Pseudocode
```
DEFINE EEPROM_TIMEOUT_MS = 10
DEFINE EEPROM_POLL_DELAY_US = 100

FUNCTION mEEPROM_WriteByte(Address, Data) RETURNS error_code:
    IF Address > AVR_EEPROM_MAXAddress:
        RETURN EEPROM_INVALID_ADDRESS
    ENDIF
  
    // ✅ Wait with timeout
    SET timeout_counter = 0
    SET max_iterations = (EEPROM_TIMEOUT_MS * 1000) / EEPROM_POLL_DELAY_US
  
    WHILE GetBit(EECR_Reg, EEWE_Bit) == 1:
        DELAY_US(EEPROM_POLL_DELAY_US)
        INCREMENT timeout_counter
    
        IF timeout_counter >= max_iterations:
            // Timeout! EEPROM not responding
            RETURN EEPROM_TIMEOUT_ERROR
        ENDIF
    END WHILE
  
    // Proceed with write
    SET EEAR_Reg = Address
    SET EEDR_Reg = Data
    SET EECR_Reg EEMWE bit
    SET EECR_Reg EEWE bit
  
    RETURN EEPROM_OK
END FUNCTION
```

---

### 3. ⚠️ LOW: EEPROM Block Write Overflow

#### Problem Description
Block write function doesn't check for memory overflow, potentially writing beyond EEPROM bounds.

#### Old Code
```c
void mEEPROM_Write(uint16_t Address, const uint8_t* data, uint16_t length)
{
    if (data != Null)
    {
        for (uint16_t index = 0; index < length; index++)
        {
            // ❌ No overflow check!
            mEEPROM_WriteByte(Address + index, data[index]);
        }
    }
}
```

#### New Code Pseudocode
```
FUNCTION mEEPROM_Write(Address, data, length) RETURNS error_code:
    IF data == Null:
        RETURN EEPROM_NULL_POINTER
    ENDIF
  
    // ✅ Check for overflow
    IF (Address + length) > (AVR_EEPROM_MAXAddress + 1):
        RETURN EEPROM_OVERFLOW_ERROR
    ENDIF
  
    FOR index FROM 0 TO length-1:
        result = mEEPROM_WriteByte(Address + index, data[index])
        IF result != EEPROM_OK:
            RETURN result  // Propagate error
        ENDIF
    END FOR
  
    RETURN EEPROM_OK
END FUNCTION
```

---

### 4. ⚠️ LOW: ADC No Timeout in Synchronous Read

#### Problem Description
Synchronous ADC read can hang if hardware fails during conversion.

#### Old Code
```c
uint16_t mADC_Read(uint8_t channel)
{
    ADMUX_Reg = (ADMUX_Reg & ADC_Channel_UpperNibble_Mask) | channel;
    SetBit(ADCSRA_Reg, ADSC_bit);
  
    // ❌ Can hang forever
    while (GetBit(ADCSRA_Reg, ADSC_bit))
    {
        // Busy wait - no timeout
    }
  
    return ADCData_Reg;
}
```

#### New Code Pseudocode
```
FUNCTION mADC_Read_Safe(channel) RETURNS (value, error):
    SET ADMUX_Reg channel bits
    START_CONVERSION()
    
    SET timeout = 0
    SET MAX_TIMEOUT = 1000  // 10ms @ 10us per loop
  
    WHILE conversion_not_complete:
        DELAY_US(10)
        INCREMENT timeout
        
        IF timeout > MAX_TIMEOUT:
            RETURN (0, ADC_TIMEOUT_ERROR)
        ENDIF
    END WHILE
    
    RETURN (ADCData_Reg, ADC_NO_ERROR)
END FUNCTION
```

---

## HAL Layer Issues

### 5. 🔴 CRITICAL: Current Sensor Variable Shadowing

#### Problem Description
Bug in calibration function prevents saving zero offset due to variable shadowing. Local variables inside if/else blocks hide the outer variable.

#### Old Code
```c
void hCurrent_Calibrate(void)
{
    float VoltageConversion = 0;  // ← Outer scope
  
    if (Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num)
    {
        // ❌ NEW local variable shadows outer one!
        float VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value / ADC_MAX) * Vref;
        // Dies at end of block
    }
    else
    {
        // ❌ ANOTHER new local variable!
        float VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value / ADC_MAX) * Vref;
    }
  
    // ❌ Uses OUTER variable which is still 0!
    ACS712_ZERO_OFFSET = VoltageConversion;  // Always 0!
}
```

**Impact:**
- Zero offset = 0 instead of 2.5V
- Current measurements offset by ~25A!

#### New Code Pseudocode
```
FUNCTION hCurrent_Calibrate():
    DECLARE VoltageConversion as float  // ✅ No initialization
  
    IF Calibration_Actions.Callibration_Samples_Num < RMS_Nominal_Samples_Num:
        // ✅ ASSIGN, don't declare
        VoltageConversion = (Calibration_Actions.Previous_ADC_Avrg_Value / ADC_MAX) * Vref
    ELSE:
        // ✅ ASSIGN, don't declare
        VoltageConversion = (Calibration_Actions.Current_ADC_Avrg_Value / ADC_MAX) * Vref
    ENDIF
  
    // ✅ Now uses correct value
    ACS712_ZERO_OFFSET = VoltageConversion
    
    RETURN
END FUNCTION
```

---

### 6. 🔴 CRITICAL: Current Sensor Wrong Variable Reset

#### Problem Description
Reset of wrong variable after RMS reading, causing data inconsistency.

#### Old Code
```c
float hCurrent_ReadRMS(void)
{
    // ... calculations ...
    
    Current_RMS_Calibrated_Actions.Current_ADC_Sum = 0;  // ✅ Correct
    
    // ❌ WRONG! Should reset Previous_ADC_Avrg_Value
    Current_RMS_Calibrated_Actions.Current_ADC_Avrg_Value = 0;
    
    return Current_RMS;
}
```

#### New Code Pseudocode
```
FUNCTION hCurrent_ReadRMS() RETURNS float:
    // Calculate RMS from accumulated samples
    meanSquare = Current_RMS_Calibrated_Actions.Current_ADC_Sum / RMS_Nominal_Samples_Num
    
    // ✅ Safety check before sqrt
    IF meanSquare < 0:
        meanSquare = 0
    ENDIF
    
    Current_RMS = SQRT(meanSquare)
    
    // ✅ Reset BOTH sum and Previous average
    Current_RMS_Calibrated_Actions.Current_ADC_Sum = 0
    Current_RMS_Calibrated_Actions.Previous_ADC_Avrg_Value = 0  // ✅ CORRECT variable
    
    RETURN Current_RMS
END FUNCTION
```

---

### 7. ⚠️ MEDIUM: Voltage Sensor Wrong ADC Divisor

#### Problem Description
Using 1023 instead of 1024 in voltage calculation formula.

#### Old Code
```c
float instant_voltage = Voltage_Value * (1.0f / 1023.0f) * Vref * Voltage_Divider_Ratio;
```

**Impact:**
- 0.1% error in readings (~0.22V @ 220V)

#### New Code Pseudocode
```
FUNCTION CalculateVoltage(ADC_Value) RETURNS float:
    // ✅ 10-bit ADC has 1024 levels (0-1023)
    voltage = ADC_Value * (1.0 / 1024.0) * Vref * Voltage_Divider_Ratio
    RETURN voltage
END FUNCTION
```

---

### 8. ⚠️ LOW: Current/Voltage Sensor Missing sqrt() Safety

#### Problem Description
No check before calling sqrt() for potential negative values due to floating-point rounding errors.

#### Old Code
```c
float meanSquare = sum_of_squares / NUM_SAMPLES;
Current_RMS = sqrt(meanSquare);  // ⚠️ What if meanSquare < 0?
```

#### New Code Pseudocode
```
FUNCTION CalculateRMS(sum_of_squares, num_samples) RETURNS float:
    meanSquare = sum_of_squares / num_samples
    
    // ✅ Clamp to prevent negative values
    IF meanSquare < 0.0:
        meanSquare = 0.0
    ENDIF
    
    rms_value = SQRT(meanSquare)
    RETURN rms_value
END FUNCTION
```

---

## Application Layer Issues

### 9. 🔴 CRITICAL: Wrong Power Calculation (No Power Factor)

#### Problem Description
Code calculates Apparent Power (S) instead of Real Power (P), leading to significant energy calculation errors.

#### Old Code
```c
void ME_Update(void)
{
    ME_Vrms = hVoltage_ReadRMS();
    ME_Irms = hCurrent_ReadRMS();

    // ❌ This is Apparent Power, not Real Power!
    ME_Power = ME_Vrms * ME_Irms;

    ME_Energy += ME_Power * ME_SAMPLE_INTERVAL;
}
```

**Impact:**
- 15-40% error in energy calculation depending on load type
- User pays more than actual consumption

**Physics Background:**
```
DC Circuits:     P = V × I  ✅
AC Circuits:     S = Vrms × Irms  (Apparent Power)
                 P = Vrms × Irms × cos(φ)  (Real Power)
                 
Where φ = phase angle between V and I
Power Factor (PF) = cos(φ)

Common loads:
- Resistive (heater): PF = 1.0
- Inductive (motor): PF = 0.7
- Capacitive (PSU): PF = 0.6
```

#### New Code Pseudocode - Solution 1 (Using Typical PF)
```
DEFINE TYPICAL_POWER_FACTOR = 0.85  // Residential average

FUNCTION ME_Update():
    ME_Vrms = hVoltage_ReadRMS()
    ME_Irms = hCurrent_ReadRMS()

    // ✅ Calculate real power with typical PF
    ME_Power = ME_Vrms * ME_Irms * TYPICAL_POWER_FACTOR
    
    // ✅ Calculate energy in kWh
    energy_ws = ME_Power * ME_SAMPLE_INTERVAL_S  // Watt-seconds
    energy_kwh = energy_ws / 3600000.0  // Convert to kWh
    
    ME_Energy_kWh = ME_Energy_kWh + energy_kwh

    RETURN
END FUNCTION
```

#### New Code Pseudocode - Solution 2 (Measuring Real PF)
```
FUNCTION CalculateRealPower(voltage_samples[], current_samples[], count) RETURNS float:
    sum = 0
    
    FOR i FROM 0 TO count-1:
        // Instantaneous power = v(t) × i(t)
        sum = sum + (voltage_samples[i] * current_samples[i])
    END FOR
    
    // Average of instantaneous power = Real Power
    real_power = sum / count
    RETURN real_power
END FUNCTION

FUNCTION ME_Update():
    ME_Vrms = hVoltage_ReadRMS()
    ME_Irms = hCurrent_ReadRMS()
  
    // ✅ Calculate real power from samples
    ME_Power = CalculateRealPower(voltage_buffer, current_buffer, NUM_SAMPLES)
  
    // Calculate PF for display
    apparent_power = ME_Vrms * ME_Irms
    IF apparent_power > 0:
        power_factor = ME_Power / apparent_power
    ELSE:
        power_factor = 1.0
    ENDIF
  
    // Accumulate energy
    ME_Energy += ME_Power * ME_SAMPLE_INTERVAL
    
    RETURN
END FUNCTION
```

---

### 10. 🔴 CRITICAL: Protection Manager No Debouncing

#### Problem Description
Protection system immediately cuts power on first high reading, causing false trips during motor startup (inrush current).

#### Old Code
```c
void PM_Update()
{
    ME_Update();
  
    // ❌ Trips immediately on single reading!
    if (ME_GetVoltageRMS() > Vrms_Threshold || 
        ME_GetCurrentRMS() > Irms_Threshold || 
        ME_GetPower() > P_Threshold)
    {
        Protection_State = Danger;
    
        // Cut ALL relays immediately
        for (uint8_t Relay_id = hRELAY_0; Relay_id <= hRELAY_3; Relay_id++)
        {
            hRelay_Off(Relay_id);
        }
    
        hRGB_SetState(RGB_RED);
        Buzzer_On();
    }
}
```

**Impact:**
- False trips on motor startup (inrush current)
- False trips on ADC noise
- Poor user experience

**Real-World Example:**
```
Motor startup:
t=0ms:   I=1A
t=10ms:  I=30A  ← Inrush current (normal!)
          ❌ System trips immediately
t=50ms:  I=5A   ← Normal running current
```

#### New Code Pseudocode
```
// State variables
DECLARE Overvoltage_Counter = 0
DECLARE Overcurrent_Counter = 0
DECLARE Overpower_Counter = 0

DEFINE TRIP_DEBOUNCE_COUNT = 10    // 100ms @ 10ms sampling
DEFINE RESET_DEBOUNCE_COUNT = 20   // 200ms
DEFINE CURRENT_HYSTERESIS = 2.0    // 2A margin

FUNCTION PM_Update():
    ME_Update()
  
    V = ME_GetVoltageRMS()
    I = ME_GetCurrentRMS()
    P = ME_GetPower()
  
    fault = FALSE
  
    // ✅ Overcurrent with debouncing and hysteresis
    IF I > Irms_Threshold:
        INCREMENT Overcurrent_Counter
        
        IF Overcurrent_Counter >= TRIP_DEBOUNCE_COUNT:
            fault = TRUE
            Overcurrent_Counter = TRIP_DEBOUNCE_COUNT  // Cap
        ENDIF
        
    ELSE IF I < (Irms_Threshold - CURRENT_HYSTERESIS):
        // ✅ Reset only when well below threshold
        Overcurrent_Counter = 0
    ENDIF
  
    // Similar logic for Overvoltage_Counter and Overpower_Counter
    // ...
  
    // ✅ Trip only after sustained fault
    IF fault AND Protection_State != Danger:
        Protection_State = Danger
        
        // Cut relays
        FOR Relay_id FROM hRELAY_0 TO hRELAY_3:
            hRelay_Off(Relay_id)
        END FOR
        
        hRGB_SetState(RGB_RED)
        Buzzer_On()
    ENDIF
    
    // ✅ Auto-reset after sustained normal operation
    IF NOT fault AND Protection_State == Danger:
        IF Overcurrent_Counter == 0 AND Overvoltage_Counter == 0:
            Protection_State = Safe
            hRGB_SetState(RGB_GREEN)
            Buzzer_Off()
        ENDIF
    ENDIF
    
    RETURN
END FUNCTION
```

**Benefits:**
```
Motor startup (with debouncing):
t=0ms:   I=1A,  Counter=0
t=10ms:  I=30A, Counter=1  ✅ No trip yet
t=20ms:  I=25A, Counter=2  ✅ No trip yet
t=30ms:  I=20A, Counter=3  ✅ No trip yet
t=40ms:  I=8A,  Counter=0  ✅ Reset, no trip
t=50ms:  I=5A,  Counter=0  ✅ Normal operation

Real overload:
t=0ms:   I=25A, Counter=1
t=10ms:  I=26A, Counter=2
t=20ms:  I=27A, Counter=3
t=30ms:  I=28A, Counter=4
t=40ms:  I=29A, Counter=5  ❌ TRIP! (Correct behavior)
```

---

## Communication Protocol Issues

### 11. 🔴 CRITICAL: Different Data Formats for Mobile vs Dashboard

#### Problem Description
Mobile App and Dashboard expect different data formats:
- Mobile: Scaled integers, Big-Endian
- Dashboard: Float32, Little-Endian

#### Current Requirement Missing
No existing code handles this difference.

#### Requirement from Mobile App
```
Command: GET_RMS_DATA (0x05)
Expected Response: 10 bytes

Data Format:
- Voltage:  uint16, Big-Endian, scaled ×10
  Example: 220.5V → 2205 → [0x08, 0x9D]
  
- Current:  uint16, Big-Endian, scaled ×100
  Example: 5.20A → 520 → [0x02, 0x08]
  
- Power:    uint16, Big-Endian, scaled ×10
  Example: 1146.6W → 11466 → [0x2C, 0xCA]
  
- Energy:   uint32, Big-Endian, scaled ×100
  Example: 2.50kWh → 250 → [0x00, 0x00, 0x00, 0xFA]

Total: 10 bytes
```

#### Requirement from Dashboard
```
Command: GET_RMS_DATA (0x05 or 0x15)
Expected Response: 16 bytes

Data Format:
- Voltage:  float32, Little-Endian
  Example: 220.5V → 0x435C8000 → [0x00, 0x80, 0x5C, 0x43]
  
- Current:  float32, Little-Endian
  Example: 5.20A → 0x40A66666 → [0x66, 0x66, 0xA6, 0x40]
  
- Power:    float32, Little-Endian
  
- Energy:   float32, Little-Endian

Total: 16 bytes
```

#### Solution Pseudocode - Multiple Commands
```
// Option: Use different command IDs

FUNCTION CM_HandleCommand(command_id):
    IF command_id == CMD_GET_RMS_MOBILE (0x05):
        SendDataToMobile()
        
    ELSE IF command_id == CMD_GET_RMS_DASHBOARD (0x15):
        SendDataToDashboard()
        
    ENDIF
    RETURN
END FUNCTION

FUNCTION SendDataToMobile():
    // Pack data in Mobile format (Big-Endian integers)
    voltage_scaled = (uint16)(ME_Vrms * 10.0)
    current_scaled = (uint16)(ME_Irms * 100.0)
    power_scaled = (uint16)(ME_Power * 10.0)
    energy_scaled = (uint32)(ME_Energy * 100.0)
    
    // Convert to Big-Endian bytes
    buffer[0] = 0xAA  // Header
    buffer[1] = 0x0A  // Length (10 bytes)
    buffer[2] = 0x05  // Command
    
    // Voltage (Big-Endian)
    buffer[3] = (voltage_scaled >> 8) & 0xFF   // MSB
    buffer[4] = voltage_scaled & 0xFF          // LSB
    
    // Current (Big-Endian)
    buffer[5] = (current_scaled >> 8) & 0xFF
    buffer[6] = current_scaled & 0xFF
    
    // Power (Big-Endian)
    buffer[7] = (power_scaled >> 8) & 0xFF
    buffer[8] = power_scaled & 0xFF
    
    // Energy (Big-Endian, 32-bit)
    buffer[9] = (energy_scaled >> 24) & 0xFF   // MSB
    buffer[10] = (energy_scaled >> 16) & 0xFF
    buffer[11] = (energy_scaled >> 8) & 0xFF
    buffer[12] = energy_scaled & 0xFF          // LSB
    
    SendUART(buffer, 13)
    RETURN
END FUNCTION

FUNCTION SendDataToDashboard():
    // Pack data in Dashboard format (Little-Endian float32)
    buffer[0] = 0xAA  // Header
    buffer[1] = 0x10  // Length (16 bytes)
    buffer[2] = 0x15  // Command (different from mobile)
    
    // Convert float to bytes (Little-Endian)
    FloatToBytes_LE(ME_Vrms, buffer[3:6])
    FloatToBytes_LE(ME_Irms, buffer[7:10])
    FloatToBytes_LE(ME_Power, buffer[11:14])
    FloatToBytes_LE(ME_Energy, buffer[15:18])
    
    SendUART(buffer, 19)
    RETURN
END FUNCTION

FUNCTION FloatToBytes_LE(value, output_buffer[]):
    // Convert 32-bit float to 4 bytes (Little-Endian)
    union_converter = (union of float and uint8[4])
    union_converter.float_value = value
    
    output_buffer[0] = union_converter.bytes[0]  // LSB
    output_buffer[1] = union_converter.bytes[1]
    output_buffer[2] = union_converter.bytes[2]
    output_buffer[3] = union_converter.bytes[3]  // MSB
    
    RETURN
END FUNCTION
```

---

### 12. ⚠️ MEDIUM: Push Button No Debouncing

#### Problem Description
Button can register multiple presses from single physical press due to mechanical bouncing.

#### Old Code (Requirement Missing)
```c
// Current: Direct interrupt on button press
void __vector_INT0()
{
    // ❌ Can trigger multiple times from single press
    Button_Callback();
}
```

#### New Code Pseudocode
```
DECLARE last_button_time = 0
DEFINE DEBOUNCE_DELAY_MS = 50

ISR INT0_vect():
    current_time = GetSystemTimeMS()
    
    // ✅ Ignore if too soon after last press
    IF (current_time - last_button_time) < DEBOUNCE_DELAY_MS:
        RETURN  // Ignore bouncing
    ENDIF
    
    last_button_time = current_time
    
    IF Button_Callback != NULL:
        Button_Callback()
    ENDIF
    
    RETURN
END ISR
```

---

## Missing Requirements

### 13. 🔴 MISSING: WiFi Module Support (ESP-01)

#### Description
Dashboard requires Remote Mode via WiFi, but current code doesn't support ESP-01.

#### Requirements from Dashboard
```
- ESP-01 WiFi module initialized
- WiFi configuration handler (command 0x0C)
- TCP connection to IoT server (port 3001)
- Forward UART data from ATmega32 to server
```

#### Pseudocode for Implementation
```
// ESP-01 Communication (via UART)

FUNCTION ESP01_Init():
    // Reset ESP-01
    ESP_RESET_PIN = LOW
    DELAY_MS(100)
    ESP_RESET_PIN = HIGH
    DELAY_MS(1000)
    
    // Test AT command
    SendAT("AT")
    IF NOT WaitForResponse("OK", 1000):
        RETURN ERROR
    ENDIF
    
    // Set mode to Station
    SendAT("AT+CWMODE=1")
    WaitForResponse("OK", 1000)
    
    RETURN SUCCESS
END FUNCTION

FUNCTION ESP01_ConnectWiFi(ssid, password):
    command = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\""
    SendAT(command)
    
    IF WaitForResponse("OK", 10000):
        RETURN SUCCESS
    ELSE:
        RETURN ERROR
    ENDIF
END FUNCTION

FUNCTION ESP01_ConnectServer(server_ip, port):
    command = "AT+CIPSTART=\"TCP\",\"" + server_ip + "\"," + port
    SendAT(command)
    
    IF WaitForResponse("CONNECT", 5000):
        RETURN SUCCESS
    ELSE:
        RETURN ERROR
    ENDIF
END FUNCTION

FUNCTION ESP01_SendData(data[], length):
    // Send data length
    command = "AT+CIPSEND=" + length
    SendAT(command)
    
    WaitForResponse(">", 1000)
    
    // Send actual data
    FOR i FROM 0 TO length-1:
        UART_SendByte(data[i])
    END FOR
    
    WaitForResponse("SEND OK", 1000)
    RETURN
END FUNCTION

// Command Handler for WiFi Config (0x0C)
FUNCTION HandleWiFiConfig(data[], length):
    // Parse SSID and Password from data
    // Format: "SSID,Password\0"
    
    ssid = ExtractSSID(data)
    password = ExtractPassword(data)
    
    // Connect to WiFi
    IF ESP01_ConnectWiFi(ssid, password) == SUCCESS:
        // Save credentials to EEPROM
        SaveWiFiCredentials(ssid, password)
        
        // Send ACK
        SendAcknowledgment(CMD_UPDATE_WIFI, STATUS_SUCCESS)
    ELSE:
        SendAcknowledgment(CMD_UPDATE_WIFI, STATUS_FAILED)
    ENDIF
    
    RETURN
END FUNCTION
```

#### Integration with Communication Manager
```
FUNCTION CM_HandleCommand(command_id, data[], length):
    SWITCH command_id:
        CASE CMD_GET_RMS_DATA:
            SendRMSData()
            
        CASE CMD_RELAY_CONTROL:
            HandleRelayControl(data)
            
        CASE CMD_UPDATE_WIFI:
            HandleWiFiConfig(data, length)  // ✅ New handler
            
        CASE CMD_CALIBRATE:
            HandleCalibration(data)
            
        DEFAULT:
            // Unknown command
            BREAK
    END SWITCH
    
    RETURN
END FUNCTION
```

---

### 14. ⚠️ MISSING: Energy Reset Command

#### Description
Mobile App requires capability to reset accumulated energy counter.

#### Requirements from Mobile App
```
Command: WRITE_EEPROM / Reset Energy
Payload: [0x00, 0x00, 0x00, 0x00]

Expected behavior:
- Reset energy counter to 0
- Save to EEPROM for persistence
- Next response should show energy = 0.0
```

#### Pseudocode for Implementation
```
FUNCTION HandleEnergyReset():
    // Reset energy counter
    ME_Energy_kWh = 0.0
    
    // Save to EEPROM for persistence
    result = SystemDataManager_SaveEnergy()
    
    IF result == SUCCESS:
        // Send acknowledgment
        SendAcknowledgment(CMD_RESET_ENERGY, STATUS_SUCCESS)
    ELSE:
        SendAcknowledgment(CMD_RESET_ENERGY, STATUS_FAILED)
    ENDIF
    
    RETURN
END FUNCTION

// Add to Communication Manager
FUNCTION CM_HandleCommand(command_id, data[], length):
    // ... existing commands ...
    
    CASE CMD_RESET_ENERGY:  // ✅ New command
        HandleEnergyReset()
        BREAK
        
    // ...
END FUNCTION

// SystemDataManager enhancement
FUNCTION SystemDataManager_SaveEnergy() RETURNS error_code:
    // Convert float to bytes
    energy_bytes[4]
    FloatToBytes(ME_Energy_kWh, energy_bytes)
    
    // Write to EEPROM
    result = mEEPROM_Write(EEPROM_ENERGY_ADDRESS, energy_bytes, 4)
    
    RETURN result
END FUNCTION

FUNCTION SystemDataManager_LoadEnergy() RETURNS float:
    energy_bytes[4]
    
    // Read from EEPROM
    mEEPROM_Read(EEPROM_ENERGY_ADDRESS, energy_bytes, 4)
    
    // Convert bytes to float
    energy_value = BytesToFloat(energy_bytes)
    
    // Sanity check
    IF energy_value < 0 OR energy_value > 99999:
        energy_value = 0.0  // Invalid, reset
    ENDIF
    
    RETURN energy_value
END FUNCTION
```

---

### 15. ⚠️ MISSING: Calibration Command Handler

#### Description
Dashboard requires capability to send calibration factors for voltage and current.

#### Requirements from Dashboard
```
Command: CALIBRATE_SENSORS (0x02)
Payload: [0x01]  // Trigger calibration

Dashboard UI has:
- Voltage factor: 0.5 - 2.0
- Current factor: 0.5 - 2.0
- "Apply Calibration" button
```

#### Pseudocode for Implementation
```
FUNCTION HandleCalibrationCommand(data[], length):
    IF data[0] == 0x01:  // Trigger calibration
        // Start calibration process
        hCurrent_StartCalibration()
        hVoltage_StartCalibration()
        
        // Send acknowledgment
        SendAcknowledgment(CMD_CALIBRATE, STATUS_IN_PROGRESS)
        
    ELSE IF data[0] == 0x02:  // Apply custom factors
        // Extract calibration factors from data
        voltage_factor = ExtractFloat(data[1:4])
        current_factor = ExtractFloat(data[5:8])
        
        // Apply factors
        hVoltage_SetCalibrationFactor(voltage_factor)
        hCurrent_SetCalibrationFactor(current_factor)
        
        // Save to EEPROM
        SystemDataManager_SaveCalibration()
        
        // Send acknowledgment
        SendAcknowledgment(CMD_CALIBRATE, STATUS_SUCCESS)
    ENDIF
    
    RETURN
END FUNCTION

// Add calibration factor to sensor modules
GLOBAL voltage_calibration_factor = 1.0
GLOBAL current_calibration_factor = 1.0

FUNCTION hVoltage_SetCalibrationFactor(factor):
    IF factor >= 0.5 AND factor <= 2.0:
        voltage_calibration_factor = factor
        RETURN SUCCESS
    ELSE:
        RETURN ERROR_OUT_OF_RANGE
    ENDIF
END FUNCTION

FUNCTION hVoltage_ReadRMS() RETURNS float:
    // ... existing RMS calculation ...
    
    // ✅ Apply calibration factor
    Voltage_RMS = Voltage_RMS * voltage_calibration_factor
    
    RETURN Voltage_RMS
END FUNCTION

// Similar for current sensor
```

---

### 16. ⚠️ MISSING: System Data Persistence

#### Description
Currently no complete system for saving and restoring critical data.

#### Requirements
```
Data to persist in EEPROM:
1. Energy counter (kWh)
2. Calibration factors (Voltage, Current)
3. Protection thresholds
4. WiFi credentials (if ESP-01 used)
5. Total runtime hours
```

#### Pseudocode for Implementation
```
// EEPROM Memory Map
DEFINE EEPROM_MAGIC_NUMBER_ADDR = 0x000
DEFINE EEPROM_ENERGY_ADDR = 0x004        // 4 bytes (float)
DEFINE EEPROM_VOLTAGE_CAL_ADDR = 0x008   // 4 bytes (float)
DEFINE EEPROM_CURRENT_CAL_ADDR = 0x00C   // 4 bytes (float)
DEFINE EEPROM_V_THRESHOLD_ADDR = 0x010   // 4 bytes (float)
DEFINE EEPROM_I_THRESHOLD_ADDR = 0x014   // 4 bytes (float)
DEFINE EEPROM_P_THRESHOLD_ADDR = 0x018   // 4 bytes (float)
DEFINE EEPROM_RUNTIME_ADDR = 0x01C       // 4 bytes (uint32)
DEFINE EEPROM_WIFI_SSID_ADDR = 0x020     // 32 bytes (string)
DEFINE EEPROM_WIFI_PASS_ADDR = 0x040     // 32 bytes (string)

DEFINE EEPROM_MAGIC_NUMBER = 0xABCD1234  // Indicates valid data

STRUCTURE SystemData:
    magic_number as uint32
    energy_kwh as float
    voltage_cal_factor as float
    current_cal_factor as float
    voltage_threshold as float
    current_threshold as float
    power_threshold as float
    runtime_hours as uint32
    wifi_ssid as string[32]
    wifi_password as string[32]
END STRUCTURE

FUNCTION SystemDataManager_Init():
    // Check if EEPROM has valid data
    magic = mEEPROM_ReadUint32(EEPROM_MAGIC_NUMBER_ADDR)
    
    IF magic == EEPROM_MAGIC_NUMBER:
        // Valid data exists, load it
        SystemDataManager_Load()
    ELSE:
        // First boot, initialize with defaults
        SystemDataManager_SetDefaults()
        SystemDataManager_Save()
    ENDIF
    
    RETURN
END FUNCTION

FUNCTION SystemDataManager_Load():
    ME_Energy_kWh = mEEPROM_ReadFloat(EEPROM_ENERGY_ADDR)
    voltage_calibration_factor = mEEPROM_ReadFloat(EEPROM_VOLTAGE_CAL_ADDR)
    current_calibration_factor = mEEPROM_ReadFloat(EEPROM_CURRENT_CAL_ADDR)
    Vrms_Threshold = mEEPROM_ReadFloat(EEPROM_V_THRESHOLD_ADDR)
    Irms_Threshold = mEEPROM_ReadFloat(EEPROM_I_THRESHOLD_ADDR)
    P_Threshold = mEEPROM_ReadFloat(EEPROM_P_THRESHOLD_ADDR)
    system_runtime_hours = mEEPROM_ReadUint32(EEPROM_RUNTIME_ADDR)
    
    // Sanity checks
    IF ME_Energy_kWh < 0 OR ME_Energy_kWh > 99999:
        ME_Energy_kWh = 0.0
    ENDIF
    
    IF voltage_calibration_factor < 0.5 OR voltage_calibration_factor > 2.0:
        voltage_calibration_factor = 1.0
    ENDIF
    
    // ... similar checks for other values ...
    
    RETURN
END FUNCTION

FUNCTION SystemDataManager_Save():
    mEEPROM_WriteUint32(EEPROM_MAGIC_NUMBER_ADDR, EEPROM_MAGIC_NUMBER)
    mEEPROM_WriteFloat(EEPROM_ENERGY_ADDR, ME_Energy_kWh)
    mEEPROM_WriteFloat(EEPROM_VOLTAGE_CAL_ADDR, voltage_calibration_factor)
    mEEPROM_WriteFloat(EEPROM_CURRENT_CAL_ADDR, current_calibration_factor)
    mEEPROM_WriteFloat(EEPROM_V_THRESHOLD_ADDR, Vrms_Threshold)
    mEEPROM_WriteFloat(EEPROM_I_THRESHOLD_ADDR, Irms_Threshold)
    mEEPROM_WriteFloat(EEPROM_P_THRESHOLD_ADDR, P_Threshold)
    mEEPROM_WriteUint32(EEPROM_RUNTIME_ADDR, system_runtime_hours)
    
    RETURN
END FUNCTION

FUNCTION SystemDataManager_SetDefaults():
    ME_Energy_kWh = 0.0
    voltage_calibration_factor = 1.0
    current_calibration_factor = 1.0
    Vrms_Threshold = 250.0  // Volts
    Irms_Threshold = 20.0   // Amperes
    P_Threshold = 4000.0    // Watts
    system_runtime_hours = 0
    
    RETURN
END FUNCTION

// Periodic save (every 10 minutes or on significant energy change)
FUNCTION SystemDataManager_PeriodicSave():
    STATIC last_save_time = 0
    STATIC last_saved_energy = 0.0
    
    current_time = GetSystemTimeMS()
    energy_delta = ABS(ME_Energy_kWh - last_saved_energy)
    
    // Save if 10 minutes passed OR energy changed by 0.1 kWh
    IF (current_time - last_save_time) > 600000 OR energy_delta > 0.1:
        SystemDataManager_Save()
        last_save_time = current_time
        last_saved_energy = ME_Energy_kWh
    ENDIF
    
    RETURN
END FUNCTION
```

---

## Summary of Priorities

### 🔴 CRITICAL (Must Fix Immediately)
1. Timer1 Duplicate ISR - fixes energy measurement
2. Current Sensor Variable Shadowing - fixes current readings
3. Wrong Power Calculation - adds Power Factor
4. Protection Manager No Debouncing - prevents false trips
5. Communication Protocol - supports Mobile/Dashboard formats

### ⚠️ HIGH (Should Fix Soon)
6. EEPROM Timeout Protection - prevents system hang
7. Current Sensor Wrong Reset - fixes data consistency
8. Energy Reset Command - required by Mobile App
9. System Data Persistence - prevents data loss

### ✅ MEDIUM (Nice to Have)
10. Voltage Sensor ADC Divisor - improves accuracy
11. Push Button Debouncing - better user experience
12. WiFi Module Support - enables remote monitoring
13. Calibration Command - allows fine-tuning

### 💡 LOW (Optional Improvements)
14. ADC Timeout - additional safety
15. EEPROM Overflow Check - edge case protection
16. sqrt() Safety Checks - robustness

---

## Testing Recommendations

After applying fixes, test in this order:

1. **Timer1 Fix Test:**
   - Check callback frequency with oscilloscope
   - Verify energy accumulation rate

2. **Current Sensor Fix Test:**
   - Measure zero current, verify ~0A reading
   - Test with known loads

3. **Power Calculation Test:**
   - Compare with calibrated power meter
   - Test with different load types

4. **Protection Test:**
   - Start motor, verify no false trip
   - Create real overload, verify proper trip

5. **Communication Test:**
   - Connect Mobile App, verify data format
   - Connect Dashboard, verify data format

---

**Document End**
