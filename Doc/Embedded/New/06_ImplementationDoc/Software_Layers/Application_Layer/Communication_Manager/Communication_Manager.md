# Communication Manager

**Purpose**: UART protocol handling

**Functions**:
- `App_CommManager_Init()`: Initialize UART
- `App_CommManager_Task()`: Process commands
- `App_CommManager_SendData()`: Transmit response

**Dual Protocol**:
- Mobile App: Scaled integers, Big-Endian
- Dashboard: Float32, Little-Endian

**Commands**: GET_RMS_DATA, RELAY_CONTROL, CALIBRATE, RESET_ENERGY

See [ICD.md](../../../04_Detailed_Design/Interface_Control_Document_ICD/Interface_Control_Document_ICD.md)

**Document Version**: 1.0
