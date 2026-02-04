## Plan: Embedded Code Cleanup and Persistency Fixes

TL;DR: Fix critical parsing and persistence bugs, make buffer and EEPROM handling safe, and decide whether to integrate or remove the `System_Controller`. Apply P0 fixes first (communication parsing, buffer safety, string overflows), then P1 (EEPROM/Logger, controller integration), then P2 cleanup.

### Steps
1. Fix Communication Manager parsing and buffer safety: make `Datareceived`, `Comm_Front`, `Comm_Rear`, `CurrentSizeofBuffer`, and `Ishandling` module-static, add atomic access to the circular buffer, validate `FrameLen`, and replace `stringtoNumber()` with robust parsing. File: [App/CommunicationManager/App_CommManager.c](App/CommunicationManager/App_CommManager.c)
2. Prevent memory/string overflows: replace `strcat()` chains with `snprintf()` in `Update_Rms_Data()` and ensure `FloatNumber_to_string()` uses bounded buffers. File: [App/System_Controller/System_Controller_Program.c](App/System_Controller/System_Controller_Program.c)
3. Ensure SystemData persistence: mark dirty on changes (already present), call `SystemData_SaveToEEPROM()` for critical changes or reduce periodic save interval; add an explicit `SystemData_FlushIfDirty()` hook on reset/trip. Files: [App/CommunicationManager/App_CommManager.c](App/CommunicationManager/App_CommManager.c), [Common/SystemDataManager/SystemDataManager.c](Common/SystemDataManager/SystemDataManager.c), [main.c](main.c)
4. Make EnergyLogger non-blocking and durable: restore `EEPROM_head`/`EEPROM_count` from EEPROM at init, and change `App_EnergyLogger_Task()` to write 1 entry per call (or a small chunk) instead of a blocking loop. File: [App/EnergyLogger/EnergyLogger_Program.c](App/EnergyLogger/EnergyLogger_Program.c)
5. Decide on `System_Controller`: either integrate (call `App_SystemController_Init()` and `App_SystemController_Update()` from `main`) and remove duplicate logic in `main`, or delete the module and keep `main`-superloop. Files: [App/System_Controller/System_Controller_Program.c](App/System_Controller/System_Controller_Program.c), [main.c](main.c)
6. Safety and style cleanups: fix float→int scaling with explicit scaling factors, correct filename typos (`MeasurementEngine_Progarm.c` → `MeasurementEngine_Program.c`), remove commented/debug code. Various files.

### Further Considerations
1. Which approach for `System_Controller` do you prefer? Integrate as main orchestrator (recommended) or remove it and keep the superloop? Option A: Integrate → fewer duplicated updates, single source of truth. Option B: Remove → simpler but lose central state machine.
2. For persistence, do you want critical config saves to be immediate on change (higher EEPROM writes) or batched (lower wear)? Recommend batched with explicit flush on critical events (Reset/Trip).


This file is an untitled plan draft for iterative refinement. Next: implement P0 fixes in `App/CommunicationManager` if you approve.
