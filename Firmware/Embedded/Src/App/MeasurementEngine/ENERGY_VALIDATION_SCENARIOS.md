# Energy Validation Scenarios (Post dt Fix)

This checklist validates the elapsed-time energy integration fix while keeping
the current wire contract unchanged (`kWh x100` in RMS payload).

## Bench Test A: Full Load + Light Commands

- Setup:
1. Turn all relays ON (initial commands only).
2. Target full-load power near `5610 W`.
3. Run for `10 minutes`.
- Expected:
1. Monotonic energy growth.
2. Slope close to `P / 3600000` kWh/s.
- Pass criteria:
1. First `0.1 kWh` appears in `64s ± 10%` (`57.6s` to `70.4s`).

## Bench Test B: Full Load + Heavy Commands

- Setup:
1. Same full-load condition as Test A.
2. Send aggressive/random command traffic during run.
3. Run for `10 minutes`.
- Expected:
1. No major drift in energy growth compared to Test A.
- Pass criteria:
1. Total kWh difference vs Test A after 10 minutes is `<= 5%`.

## Math Consistency Spot-Check

- For sampled points, validate:
1. `DeltaE_kWh ~= (P_W * DeltaT_ms) / 3600000000`.
2. Deviations remain within sensor-noise tolerance.

## Protocol Regression Check

- Validate RMS response frame:
1. Command: `GET_RMS_DATA (0x05)`
2. Payload length: `11`
3. Layout: `[VH][VL][IH][IL][PH][PL][E3][E2][E1][E0][RelayMask]`
4. Energy field interpretation: `kWh = uint32_value / 100.0`.
