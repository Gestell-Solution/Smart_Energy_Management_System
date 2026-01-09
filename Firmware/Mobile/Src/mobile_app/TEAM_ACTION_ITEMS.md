# 📋 Team Action Items - Embedded vs Mobile

## 📱 **Mobile Team - COMPLETE ✅**

### **Status: 100% Done - No Action Required**

All work is complete:

- ✅ Flutter app fully implemented
- ✅ All UI screens created (Splash, Dashboard, History, Alerts, Settings)
- ✅ Bluetooth service with framed protocol support
- ✅ Binary data parser for 0xAA frames
- ✅ Command handling (GET_RMS_DATA, Write_EEPROM)
- ✅ Charts, gauges, alerts system
- ✅ Dark/Light theme
- ✅ Data persistence
- ✅ Ready to build APK

**Next Steps:**

1. ✅ Build APK: `flutter build apk --release`
2. ⏸️ **Wait for Embedded team** to implement GET_RMS_DATA response
3. 🧪 Test with real hardware once embedded is ready
4. 🔧 Minor adjustments if data format differs (5-10 min fix)

---

## 🔧 **Embedded Team - ACTION REQUIRED ⚠️**

### **Status: 1 Critical Fix Needed**

### **✅ What's Already Done:**

- ✅ Communication Manager structure exists
- ✅ Frame protocol defined (0xAA header, length, command, data)
- ✅ HC-05 Bluetooth module integrated
- ✅ UART communication working
- ✅ Timer and task scheduler set up
- ✅ SystemData_t structure with all measurements

---

### **❌ What Needs Implementation:**

## **Task 1: Implement GET_RMS_DATA Response** 🔴 **CRITICAL**

**File:** `App/CommunicationManager/App_CommManager.c`

**Current Code (Line 132-137):**

```c
void App_CommManager_ProcessCommand(uint8_t *frame)
{
    App_CommManager_SendFrame(&frame[1],frame[0],5);  // ❌ Just echoes
    mDIO_TogglePin(GroupD,PIN3);
}
```

**Required Change:**

Uncomment and implement the switch-case (lines 138-181):

```c
void App_CommManager_ProcessCommand(uint8_t *frame)
{
    switch (frame[0])  // Command ID
    {
        case GET_RMS_DATA:  // 0x05
            SendRMSData();
            break;
            
        case Write_EEPROM:  // 0x04
            HandleResetEnergy(frame);
            break;
            
        case Read_EEPROM:
            // Existing implementation
            break;
            
        case Calibrate_Sensors:
            // Existing implementation
            break;
            
        // Add other cases as needed
        
        default:
            // Unknown command
            break;
    }
}

// New function to add:
void SendRMSData(void)
{
    extern SystemData_t g_SystemData;
    uint8_t data[10];
    
    // Pack data in big-endian format (high byte first)
    // Voltage (uint16, scaled by 10)
    data[0] = (g_SystemData.Voltage_RMS >> 8) & 0xFF;
    data[1] = g_SystemData.Voltage_RMS & 0xFF;
    
    // Current (uint16, scaled by 100)
    data[2] = (g_SystemData.Current_RMS >> 8) & 0xFF;
    data[3] = g_SystemData.Current_RMS & 0xFF;
    
    // Power (uint16, scaled by 10)
    data[4] = (g_SystemData.Power >> 8) & 0xFF;
    data[5] = g_SystemData.Power & 0xFF;
    
    // Energy (uint32, scaled by 100)
    data[6] = (g_SystemData.EnergyCounter >> 24) & 0xFF;
    data[7] = (g_SystemData.EnergyCounter >> 16) & 0xFF;
    data[8] = (g_SystemData.EnergyCounter >> 8) & 0xFF;
    data[9] = g_SystemData.EnergyCounter & 0xFF;
    
    // Send frame: [0xAA][10][0x05][data...]
    App_CommManager_SendFrame(data, GET_RMS_DATA, 10);
}

// Optional: Reset energy command
void HandleResetEnergy(uint8_t *frame)
{
    extern SystemData_t g_SystemData;
    g_SystemData.EnergyCounter = 0;
    SystemData_SaveToEEPROM();
    
    // Send ACK
    uint8_t ack = 0x01;
    App_CommManager_SendFrame(&ack, Write_EEPROM, 1);
}
```

**Estimated Time:** 30-45 minutes

---

## **Task 2: Verify System Modules Running** 🟡 **IMPORTANT**

**File:** `main.c` (or wherever main loop is)

**Verify:**

```c
int main(void)
{
    // Initializations...
    SystemData_Init();
    App_CommManager_Init();
    App_MeasurementEngine_Init();   // ← Must be called
    App_ProtectionManager_Init();
    
    GIE_Enable();
    
    while(1)
    {
        App_MeasurementEngine_Update();  // ← Must run to update g_SystemData
        App_ProtectionManager_Update();
        App_CommManager_Task();          // ← Must run to process Bluetooth
        
        // Other tasks...
    }
}
```

**Action:**

- ✅ Confirm `App_MeasurementEngine_Update()` is running
- ✅ Confirm it updates `g_SystemData.Voltage_RMS`, `Current_RMS`, `Power`, `EnergyCounter`
- ✅ Confirm `App_CommManager_Task()` is called frequently (every 5-50ms)

**Estimated Time:** 10-15 minutes (verification only)

---

## **Task 3: Test Response Format** 🧪

**Testing:**

1. **Connect via Serial Terminal:**

   ```bash
   # Send manual frame
   echo -ne '\xAA\x00\x05' > /dev/ttyUSB0
   
   # Check response
   cat /dev/ttyUSB0 | hexdump -C
   ```

2. **Expected Response:**

```text
0000000  aa 0a 05 08 9d 02 08 2c ea 00 00 00 fa
```

- `aa` = Header
- `0a` = Length (10 bytes)
- `05` = Command (GET_RMS_DATA)
- `08 9d` = Voltage (2205 = 220.5V)
- `02 08` = Current (520 = 5.20A)
- `2c ea` = Power (11498 = 1149.8W)
- `00 00 00 fa` = Energy (250 = 2.50kWh)

**Estimated Time:** 15-20 minutes

---

## 📊 **Work Distribution Summary**

| **Team** | **Tasks** | **Estimated Time** | **Status** |
|----------|-----------|-------------------|------------|
| **Mobile** | None - Complete | N/A | ✅ Done |
| **Embedded** | Implement GET_RMS_DATA | 30-45 min | ⏳ Pending |
| **Embedded** | Verify modules running | 10-15 min | ⏳ Pending |
| **Embedded** | Test response format | 15-20 min | ⏳ Pending |
| **Both** | Integration testing | 1-2 hours | ⏸️ After embedded fixes |

**Total Embedded Work:** ~1-1.5 hours

---

## 🎯 **Integration Timeline**

```text
Day 1 (Embedded Team):
├─ Task 1: Implement SendRMSData()          [45 min]
├─ Task 2: Verify modules running           [15 min]
├─ Task 3: Test with serial terminal        [20 min]
└─ Build and flash firmware                 [15 min]

Day 2 (Both Teams):
├─ Mobile: Install APK on phone            [5 min]
├─ Connect HC-05 to phone                  [10 min]
├─ Test data reception                     [30 min]
├─ Debug if needed                         [30-60 min]
└─ Final validation                        [30 min]
```

---

## ✅ **Checklist for Embedded Team**

```text
Before Starting:
[ ] Backup current code
[ ] Open App/CommunicationManager/App_CommManager.c
[ ] Review SystemData_t structure

Implementation:
[ ] Uncomment switch-case in App_CommManager_ProcessCommand()
[ ] Add SendRMSData() function
[ ] Add HandleResetEnergy() function (optional)
[ ] Add function prototypes to .h file

Verification:
[ ] Check g_SystemData is being updated by Measurement Engine
[ ] Verify main loop calls App_CommManager_Task()
[ ] Compile without errors
[ ] Flash to ATmega32

Testing:
[ ] Test with serial terminal (send 0xAA 0x00 0x05)
[ ] Verify response format is correct
[ ] Check data values match LCD display
[ ] Verify multiple requests work

Ready for Integration:
[ ] Notify mobile team
[ ] Provide test device for mobile testing
```

---

## ✅ **Checklist for Mobile Team**

```text
Current Status:
[✓] All code complete
[✓] APK buildable
[✓] Bluetooth service ready
[✓] Protocol parser implemented

Waiting For:
[ ] Embedded team completes GET_RMS_DATA implementation

Once Embedded Ready:
[ ] Install APK on Android phone
[ ] Enable Bluetooth and location permissions
[ ] Scan for HC-05 device
[ ] Connect to device
[ ] Verify data displays on dashboard
[ ] Test all screens (Dashboard, History, Alerts, Settings)
[ ] Verify alerts trigger correctly
[ ] Test theme switching
[ ] Test data persistence

If Issues:
[ ] Check Bluetooth connection
[ ] Verify data format in bluetooth_service.dart
[ ] Adjust byte order if needed (big-endian vs little-endian)
[ ] Check scaling factors (÷10, ÷100)
```

---

## 🚨 **Critical Path**

**Blocking Item:** Embedded GET_RMS_DATA implementation

**Once Unblocked:** Full integration can proceed

**Risk:** Low - only 1 function needs implementation

**Mitigation:** Mobile app designed to be flexible with data format

---

## 📞 **Communication Protocol**

**Embedded Team Deliverables:**

1. Confirmation that `SendRMSData()` is implemented
2. Test results from serial terminal
3. Sample response frame in hex

**Mobile Team Deliverables:**

1. APK file ready for testing
2. Testing checklist results
3. Any data format adjustments needed

---

## 🎯 **Success Criteria**

Integration is successful when:

- ✅ Mobile app connects to HC-05
- ✅ Dashboard shows live voltage, current, power, energy
- ✅ Values update every second
- ✅ Alerts trigger when thresholds exceeded
- ✅ History charts populate with data
- ✅ Reset button clears energy counter

---

**Current Status:**

- **Mobile:** ✅ Ready
- **Embedded:** ⏳ 1 hour of work remaining
- **Integration:** ⏸️ Waiting on embedded

**Next Action:** Embedded team implements `SendRMSData()` function

---
