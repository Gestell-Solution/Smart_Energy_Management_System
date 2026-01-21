# Low-Level Design (LLD)

**Project**: Smart Energy Management System  
**Component**: Firmware Low-Level Design  
**Version**: 1.0

---

## 1. Measurement Algorithms

### 1.1 RMS Calculation Algorithm

**Purpose**: Calculate RMS (Root Mean Square) for AC voltage and current

**Algorithm**:

```
Input: samples[] (ADC readings buffer), N (number of samples)
Output: RMS value (float)

1. sum_of_squares = 0
2. For i = 0 to N-1:
     sample_value = samples[i] - offset  // Remove DC bias
     sum_of_squares += sample_value²
3. mean_square = sum_of_squares / N
4. RMS = √(mean_square)
5. RMS_scaled = RMS × calibration_factor
6. Return RMS_scaled
```

**Optimization**:

- Use integer arithmetic until final step (avoid floating-point in ISR)
- Keep buffer size power of 2 for efficient division (N=128)
- Use fixed-point math where possible

### 1.2 Power Calculation

**Formula**: `P = V_RMS × I_RMS × Power_Factor`

**Steps**:

1. Get V_RMS from voltage sensor
2. Get I_RMS from current sensor
3. Multiply by power factor (from EEPROM, default 0.95)
4. Return power in Watts

### 1.3 Energy Integration

**Formula**: `E(t) = E(t-1) + P × Δt`

**Implementation**:

```c
static float energy_joules = 0.0f;  // Cumulative energy in Joules
static uint32_t last_time_ms = 0;

void UpdateEnergy(float power_watts) {
    uint32_t current_time_ms = GetSystemTime();
    float delta_time_sec = (current_time_ms - last_time_ms) / 1000.0f;

    energy_joules += power_watts * delta_time_sec;
    last_time_ms = current_time_ms;
}

float GetEnergyKWh(void) {
    return energy_joules / 3600000.0f;  // Convert J to kWh
}
```

---

## 2. Protection Logic

### 2.1 Overcurrent Detection FSM

**States**:

- NORMAL: Current within limits
- DEBOUNCING: Current exceeded, counting samples
- FAULT: Protection triggered

**Transitions**:

```
NORMAL → DEBOUNCING: I > threshold
DEBOUNCING → FAULT: debounce_count >= 3
DEBOUNCING → NORMAL: I < threshold
FAULT → NORMAL: Manual reset command
```

**Pseudocode**:

```c
static enum {NORMAL, DEBOUNCING, FAULT} oc_state = NORMAL;
static uint8_t debounce_count = 0;

void CheckOvercurrent(float current) {
    switch(oc_state) {
        case NORMAL:
            if (current > threshold) {
                oc_state = DEBOUNCING;
                debounce_count = 1;
            }
            break;

        case DEBOUNCING:
            if (current > threshold) {
                debounce_count++;
                if (debounce_count >= 3) {
                    oc_state = FAULT;
                    TriggerProtection();
                }
            } else {
                oc_state = NORMAL;
                debounce_count = 0;
            }
            break;

        case FAULT:
            // Stay in fault until manual reset
            break;
    }
}

void ResetProtection(void) {
    oc_state = NORMAL;
    debounce_count = 0;
    ClearFaultFlags();
}
```

### 2.2 Protection Actions

**Sequence** (executed in <100ms):

1. Set relay pin LOW (relay OFF)
2. Set fault flag in memory
3. Update display ("OVERLOAD!" or "OVERVOLT!")
4. Set RGB LED to RED
5. Start buzzer pattern (3 or 5 beeps)
6. Queue communication alert

---

## 3. Communication Protocol Implementation

### 3.1 Frame Parser

**State Machine**:

- IDLE: Waiting for header (0xAA)
- LENGTH: Reading length byte
- COMMAND: Reading command byte
- DATA: Reading data bytes
- PROCESS: Processing complete frame

**Pseudocode**:

```c
typedef enum {IDLE, LENGTH, COMMAND, DATA, PROCESS} RxState_t;

static RxState_t rx_state = IDLE;
static uint8_t rx_buffer[32];
static uint8_t rx_index = 0;
static uint8_t expected_length = 0;
static uint8_t command_id = 0;

void UART_RxISR(void) {
    uint8_t byte = UART_ReadByte();

    switch(rx_state) {
        case IDLE:
            if (byte == 0xAA) {  // Header
                rx_state = LENGTH;
                rx_index = 0;
            }
            break;

        case LENGTH:
            expected_length = byte;
            rx_state = COMMAND;
            break;

        case COMMAND:
            command_id = byte;
            if (expected_length == 0) {
                rx_state = PROCESS;
            } else {
                rx_state = DATA;
            }
            break;

        case DATA:
            rx_buffer[rx_index++] = byte;
            if (rx_index >= expected_length) {
                rx_state = PROCESS;
            }
            break;

        case PROCESS:
            ProcessCommand(command_id, rx_buffer, expected_length);
            rx_state = IDLE;
            break;
    }
}
```

### 3.2 Data Format Conversion

**Mobile App (Scaled Integer, Big-Endian)**:

```c
void SendMobileData(float v, float i, float p, float e) {
    uint16_t v_scaled = (uint16_t)(v * 10);      // 220.5V → 2205
    uint16_t i_scaled = (uint16_t)(i * 100);     // 5.20A → 520
    uint16_t p_scaled = (uint16_t)(p * 10);      // 1146.6W → 11466
    uint32_t e_scaled = (uint32_t)(e * 100);     // 2.50kWh → 250

    // Big-Endian (MSB first)
    UART_SendByte(0xAA);  // Header
    UART_SendByte(10);    // Length
    UART_SendByte(0x05);  // Command
    UART_SendByte((v_scaled >> 8) & 0xFF);  // V high byte
    UART_SendByte(v_scaled & 0xFF);         // V low byte
    UART_SendByte((i_scaled >> 8) & 0xFF);
    UART_SendByte(i_scaled & 0xFF);
    UART_SendByte((p_scaled >> 8) & 0xFF);
    UART_SendByte(p_scaled & 0xFF);
    UART_SendByte((e_scaled >> 24) & 0xFF);
    UART_SendByte((e_scaled >> 16) & 0xFF);
    UART_SendByte((e_scaled >> 8) & 0xFF);
    UART_SendByte(e_scaled & 0xFF);
}
```

**Dashboard (Float32, Little-Endian)**:

```c
void SendDashboardData(float v, float i, float p, float e) {
    union {
        float f;
        uint8_t bytes[4];
    } converter;

    UART_SendByte(0xAA);  // Header
    UART_SendByte(16);    // Length
    UART_SendByte(0x05);  // Command

    // Voltage (Little-Endian)
    converter.f = v;
    for (int i = 0; i < 4; i++) {
        UART_SendByte(converter.bytes[i]);
    }

    // Current
    converter.f = i;
    for (int i = 0; i < 4; i++) {
        UART_SendByte(converter.bytes[i]);
    }

    // Power
    converter.f = p;
    for (int i = 0; i < 4; i++) {
        UART_SendByte(converter.bytes[i]);
    }

    // Energy
    converter.f = e;
    for (int i = 0; i < 4; i++) {
        UART_SendByte(converter.bytes[i]);
    }
}
```

---

## 4. EEPROM Management

### 4.1 Write Strategy with Timeout

```c
#define EEPROM_TIMEOUT_MS 5

bool EEPROM_WriteWithTimeout(uint16_t address, uint8_t data) {
    uint32_t start_time = GetSystemTime();

    // Write data
    EEPROM_Write_Direct(address, data);

    // Wait for completion with timeout
    while (EEPROM_IsBusy()) {
        if ((GetSystemTime() - start_time) > EEPROM_TIMEOUT_MS) {
            return false;  // Timeout
        }
    }

    // Verify
    uint8_t readback = EEPROM_Read(address);
    return (readback == data);
}
```

### 4.2 Retry Mechanism

```c
bool EEPROM_WriteWithRetry(uint16_t address, uint8_t data) {
    for (uint8_t attempt = 0; attempt < 3; attempt++) {
        if (EEPROM_WriteWithTimeout(address, data)) {
            return true;  // Success
        }
        _delay_ms(1);  // Brief delay before retry
    }
    return false;  // Failed after 3 attempts
}
```

---

## 5. Display Manager Logic

### 5.1 Screen Rotation

```c
typedef enum {SCREEN_V_I, SCREEN_P_E, SCREEN_STATUS} DisplayScreen_t;

static DisplayScreen_t current_screen = SCREEN_V_I;
static uint32_t last_rotation_time = 0;

void DM_Update(void) {
    uint32_t current_time = GetSystemTime();

    if ((current_time - last_rotation_time) >= ROTATION_INTERVAL_MS) {
        // Rotate to next screen
        current_screen = (current_screen + 1) % 3;
        last_rotation_time = current_time;

        LCD_Clear();
        switch(current_screen) {
            case SCREEN_V_I:
                DisplayVoltageAndCurrent();
                break;
            case SCREEN_P_E:
                DisplayPowerAndEnergy();
                break;
            case SCREEN_STATUS:
                DisplayStatus();
                break;
        }
    }
}
```

### 5.2 LCD Content Formatting

```c
void DisplayVoltageAndCurrent(void) {
    char line1[17], line2[17];

    float v = ME_GetVoltageRMS();
    float i = ME_GetCurrentRMS();

    snprintf(line1, 17, "V: %.1f V      ", v);
    snprintf(line2, 17, "I: %5.2f A      ", i);

    LCD_SetCursor(0, 0);
    LCD_WriteString(line1);
    LCD_SetCursor(1, 0);
    LCD_WriteString(line2);
}
```

---

## 6. RGB LED Color Control

### 6.1 PWM Color Mixing

```c
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGBColor_t;

void RGB_SetColor(RGBColor_t color) {
    // Assuming Timer1 PWM on PB1, PB2, PB3
    OCR1A = color.red;    // Red intensity (0-255)
    OCR1B = color.green;  // Green intensity (0-255)
    // Blue on PB3 (software PWM or Timer0)
    // Simplified: just ON/OFF for blue
    if (color.blue > 128) {
        DIO_SetPin(PORTB, PIN3, HIGH);
    } else {
        DIO_SetPin(PORTB, PIN3, LOW);
    }
}

// Predefined colors
const RGBColor_t COLOR_GREEN = {0, 255, 0};
const RGBColor_t COLOR_YELLOW = {255, 255, 0};
const RGBColor_t COLOR_RED = {255, 0, 0};
const RGBColor_t COLOR_BLUE = {0, 0, 255};
```

---

## 7. Timing and Scheduling

### 7.1 Main Loop Timing (100ms cycle)

```c
int main(void) {
    // Initialization
    SystemInit();

    uint32_t last_cycle_time = 0;

    while(1) {
        uint32_t current_time = GetSystemTime();

        if ((current_time - last_cycle_time) >= 100) {  // 100ms
            last_cycle_time = current_time;

            // Execute periodic tasks
            ME_Update();
            PM_Update();
            DM_Update();
            EL_Task();
            CM_Task();
        }

        // Optional: Low-power idle between cycles
        // _delay_ms(1);
    }
}
```

### 7.2 Timer1 ISR (ADC Sampling at 100 Hz)

```c
ISR(TIMER1_COMPA_vect) {
    static uint8_t channel = 0;

    // Start ADC conversion on current channel
    ADC_StartConversion(channel);

    // Wait for conversion complete (blocking in ISR - keep short!)
    uint16_t adc_value = ADC_Read();

    // Store in appropriate buffer
    if (channel == 0) {
        voltage_samples[sample_index] = adc_value;
    } else {
        current_samples[sample_index] = adc_value;
    }

    // Toggle channel (0 → 1 → 0)
    channel = !channel;

    if (channel == 0) {
        sample_index = (sample_index + 1) % SAMPLE_BUFFER_SIZE;
    }
}
```

---

**Document Version**: 1.0  
**Last Updated**: January 2026  
**Maintained By**: Gestell Engineering Team
