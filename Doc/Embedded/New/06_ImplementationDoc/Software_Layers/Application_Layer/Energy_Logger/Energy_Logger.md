# Energy Logger

**Purpose**: Persist energy data to EEPROM

**Functions**:
- `App_EnergyLogger_Init()`: Load energy from EEPROM
- `App_EnergyLogger_Update(log)`: Update log buffer
- `App_EnergyLogger_Task()`: Periodic save

**Save Strategy**:
- Every 60 seconds
- On change >0.1 kWh
- With retry (3 attempts)

**Document Version**: 1.0
