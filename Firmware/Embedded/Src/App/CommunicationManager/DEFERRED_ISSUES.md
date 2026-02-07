# CommunicationManager Deferred Issues

This file tracks CommunicationManager issues that are still intentionally deferred
after the energy hardening patch.

## Resolved in Energy Hardening Patch

1. `LEN=0` commands are now accepted.
2. `App_CommManager_Task()` is now scheduled once from `App_CommManager_Init()`.

## Still Deferred

1. **No ACK/error response strategy for invalid frames or unsupported commands**
   - Impact: mobile side has limited visibility when a command is malformed or not handled.
   - Follow-up: define a compact error/ack frame policy and implement consistent responses.

2. **`Get_Logged_DATA` command path is still placeholder**
   - Impact: external clients cannot reliably fetch historical logger entries.
   - Follow-up: finalize payload format (indexing/pagination) and implement command handler.
