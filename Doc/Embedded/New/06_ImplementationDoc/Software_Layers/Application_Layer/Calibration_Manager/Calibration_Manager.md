# Calibration Manager

**Purpose**: Sensor calibration procedures

**Functions**:
- `CalibrationManager_Init()`: Load from EEPROM
- `CalibrationManager_CalibrateVoltage(ref_V)`: Voltage calibration
- `CalibrationManager_CalibrateCurrent(ref_A)`: Current calibration
- `CalibrationManager_CalibrateZeroOffset()`: Zero-current calibration
- `CalibrationManager_SaveToEEPROM()`: Persist calibration

**Procedure**:
1. Apply known reference (220V, 10A)
2. Read measured value
3. Calculate factor = reference / measured
4. Save to EEPROM

**Document Version**: 1.0
