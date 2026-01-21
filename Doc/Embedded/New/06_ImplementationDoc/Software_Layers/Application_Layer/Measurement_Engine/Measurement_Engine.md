# Measurement Engine

**Purpose**: Real-time energy monitoring

**Functions**:
- `ME_Init()`: Initialize sensors and Timer1
- `ME_Update()`: Calculate RMS values
- `ME_GetVoltageRMS()`: Return voltage (V)
- `ME_GetCurrentRMS()`: Return current (A)
- `ME_GetPower()`: Return power (W) = V × I × PF
- `ME_GetEnergy()`: Return energy (kWh)

**Algorithm**:
- Sample ADC at 100 Hz (Timer1 ISR)
- Calculate RMS: √(Σ V²/N)
- Integrate power for energy: E += P × Δt

See [LLD.md](../../../04_Detailed_Design/LLD/LLD.md)

**Document Version**: 1.0
