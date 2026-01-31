# Smart Energy Management System - Communication Protocol Test Cases (Mobile App ↔ Embedded)

**Document Version:** 1.0  
**Last Updated:** 31 January 2026  
**Scope:** Communication-only validation for the binary framed protocol used by the mobile app.  
**Related Spec:** `MOBILE_APP_COMM_PROTOCOL.md`  

---

## 1) Purpose

This document provides a comprehensive set of **communication test cases** to determine:

- Which **commands** are correctly implemented on the embedded side.
- Whether the mobile app can **send**, **receive**, and **parse** frames correctly.
- Whether failures are caused by **transport**, **framing**, **endianness/scaling**, or **command logic**.

Each test case includes:

- Exact message format (hex)
- Preconditions
- Steps
- Expected results on both sides
- Failure diagnosis hints (who is likely wrong)

---

## 2) System Under Test (SUT)

### 2.1 Transport

- Bluetooth Classic SPP (HC-05 / HC-06 style)

### 2.2 Frame Format (binary)

```text
[0xAA][LEN][CMD][PAYLOAD...]
```

- `0xAA` is the frame header.
- `LEN` counts **payload bytes only**.
- No checksum/CRC is used by the current mobile implementation.

### 2.3 Endianness & Scaling (critical)

- Multi-byte values are **Big-endian**.
- `GET_RMS_DATA (0x05)` expected scaling in mobile:
  - Voltage int = `V * 10`
  - Current int = `I * 100`
  - Power int = `P * 10`
  - Energy int = `Energy * 100`

---

## 3) Test Environment & Tools

### 3.1 Required

- Android phone running the Flutter app
- Embedded target + HC-05/HC-06 connected to MCU UART
- Stable power supply

### 3.2 Recommended (for faster triage)

- UART debug logging on embedded (prints when receiving CMD and when sending response)
- A way to inject frames (choose one):
  - A second Bluetooth SPP terminal app on another phone/PC
  - A PC with RFCOMM serial port connected to the SPP link
- Optional: logic analyzer on UART TX/RX (MCU ↔ HC-05)

---

## 4) Pass/Fail Rules

- A test is **PASS** only if:
  - The correct frame is sent (format correct), and
  - The expected response is received (if applicable), and
  - The mobile UI/state matches decoded values (if applicable), and
  - No unexpected disconnects or parser lock-ups occur.

---

## 5) Global Communication Test Cases (Framing & Robustness)

These cases are command-agnostic and validate the stream/frame parser behavior.

### CP-TC-001: Basic frame parsing (single complete frame in one chunk)

- **Type:** Framing
- **Direction:** Embedded → App
- **Goal:** Confirm the mobile parser can parse a normal frame.
- **Preconditions:** App connected over SPP.
- **Steps:**
  1. Embedded sends one complete frame:

     ```text
     AA 00 05
     ```

     (This is a deliberately short frame, may be ignored depending on command parser. The main target here is framing.)
- **Expected (App):**
  - No crash.
  - Frame buffer should not get stuck.
- **Expected (Embedded):** Frame transmitted.
- **Failure hints:**
  - If app disconnects/crashes: app-side Bluetooth handling issue.

### CP-TC-002: Split frame across multiple packets (chunking)

- **Type:** Framing
- **Direction:** Embedded → App
- **Goal:** Ensure buffering works when frame arrives in parts.
- **Steps:**
  1. Embedded sends the following bytes with small delays (e.g., 20–100 ms):

     Part 1:

     ```text
     AA 0A
     ```

     Part 2:

     ```text
     05 08 CA 02 BC
     ```

     Part 3:

     ```text
     05 DC 00 00 4E 20
     ```

- **Expected (App):**
  - App correctly reconstructs the full frame and decodes `GET_RMS_DATA` if command handling is enabled.
- **Failure hints:**
  - If partial parsing happens: bug in app frame buffering.

### CP-TC-003: Multiple frames in one packet (back-to-back)

- **Type:** Framing
- **Direction:** Embedded → App
- **Goal:** Ensure the parser can process multiple frames in one read.
- **Steps:**
  1. Embedded sends two full frames concatenated:

     ```text
     AA 00 05 AA 00 0C
     ```

- **Expected (App):**
  - No crash.
  - Both frames are consumed from the buffer.
- **Failure hints:**
  - If only first is processed: parser loop issue.

### CP-TC-004: Garbage bytes before header

- **Type:** Framing
- **Direction:** Embedded → App
- **Goal:** Ensure parser discards bytes until it finds `0xAA`.
- **Steps:**
  1. Embedded sends:

     ```text
     55 FF 13 00 AA 00 05
     ```

- **Expected (App):**
  - Discard `55 FF 13 00`, then recognize frame starting at `AA`.
- **Failure hints:**
  - If app never recovers: header search/trim issue.

### CP-TC-005: Header missing (no 0xAA)

- **Type:** Framing
- **Direction:** Embedded → App
- **Goal:** Confirm the buffer clears safely when no header exists.
- **Steps:**
  1. Embedded sends random bytes without `0xAA`:

     ```text
     10 11 12 13 14 15
     ```

- **Expected (App):**
  - Buffer cleared and waits for valid data; no crash.

### CP-TC-006: Invalid length (LEN too large for actual data)

- **Type:** Framing
- **Direction:** Embedded → App
- **Goal:** Confirm app waits for full frame and does not mis-parse.
- **Steps:**
  1. Embedded sends:

     ```text
     AA 20 05 01 02 03
     ```

- **Expected (App):**
  - App should wait for the remaining bytes (should not process partial frame).
- **Failure hints:**
  - If app processes early: length validation bug.

### CP-TC-007: Unknown command ID

- **Type:** Compatibility
- **Direction:** Embedded → App
- **Goal:** Ensure unknown `CMD` does not crash the app.
- **Steps:**
  1. Embedded sends:

     ```text
     AA 01 FE 00
     ```

- **Expected (App):**
  - No crash.
  - Frame ignored (since no handler for `0xFE`).

---

## 6) Command-Specific Test Cases

### 6.1 GET_RMS_DATA (0x05)

#### CP-TC-101: GET_RMS_DATA request is sent by App

- **Type:** Command / Request generation
- **Direction:** App → Embedded
- **Goal:** Ensure app sends correct request frame periodically.
- **Preconditions:** App connected; stay on dashboard/normal operation.
- **Steps:**
  1. Observe embedded UART RX (or embedded logs).
  2. Wait 3–5 seconds.
- **Expected (Embedded):**
  - Receive repeated requests every ~1s:

    ```text
    AA 00 05
    ```

- **Expected (App):**
  - Connection stays stable.
- **Failure hints:**
  - If embedded does not see this frame: issue is app-side timer/BT output or the SPP link.

#### CP-TC-102: GET_RMS_DATA valid response (happy path)

- **Type:** Command / Parsing
- **Direction:** Embedded → App
- **Goal:** Confirm correct parsing and scaling.
- **Steps:**
  1. After receiving `AA 00 05`, embedded replies with:

     ```text
     AA 0A 05  08 CA  02 BC  05 DC  00 00 4E 20
     ```

- **Expected (App):**
  - Voltage = 225.0 V
  - Current = 7.00 A
  - Power = 150.0 W
  - Energy = 200.00 (scaled by /100)
- **Expected (Embedded):**
  - Response frame sent each time it receives request.
- **Failure hints:**
  - Wrong numbers but stable parsing: likely **endianness or scaling** mismatch (embedded-side packing).
  - No update at all: embedded is not responding, or app is not parsing.

#### CP-TC-103: GET_RMS_DATA wrong LEN (embedded bug simulation)

- **Type:** Negative
- **Direction:** Embedded → App
- **Goal:** Ensure app does not decode wrong-length payload.
- **Steps:**
  1. Embedded sends:

     ```text
     AA 09 05  08 CA 02 BC 05 DC 00 00 4E
     ```

- **Expected (App):**
  - Should ignore or fail silently (no crash).
- **Failure hints:**
  - Crash: app-side robustness issue.

#### CP-TC-104: GET_RMS_DATA little-endian vs big-endian detection

- **Type:** Diagnostic
- **Direction:** Embedded → App
- **Goal:** Identify if embedded is sending swapped byte order.
- **Steps:**
  1. Embedded sends a test pattern where endianness is obvious:

     ```text
     AA 0A 05  00 01  00 02  00 03  00 00 00 04
     ```

- **Expected (App) if big-endian:**
  - V = 0.1
  - I = 0.02
  - P = 0.3
  - E = 0.04
- **If app shows huge values:** embedded likely sending little-endian.

#### CP-TC-105: GET_RMS_DATA response sent without being requested

- **Type:** Behavioral
- **Direction:** Embedded → App
- **Goal:** Confirm app can handle unsolicited `0x05` frames.
- **Steps:**
  1. Embedded pushes `AA 0A 05 ...` without waiting for request.
- **Expected (App):**
  - Should still parse and update values.
- **Failure hints:**
  - If app only works when requested: app-side assumption issue.

---

### 6.2 GET_DEVICE_INFO (0x0C)

#### CP-TC-201: GET_DEVICE_INFO request is sent by App

- **Type:** Command / Request generation
- **Direction:** App → Embedded
- **Goal:** Ensure app sends correct request frame.
- **Steps:**
  1. Trigger “device info” in the app (Settings/device info UI flow).
  2. Observe embedded RX.
- **Expected (Embedded):**

  ```text
  AA 00 0C
  ```

#### CP-TC-202: GET_DEVICE_INFO valid response (happy path)

- **Type:** Command / Parsing
- **Direction:** Embedded → App
- **Goal:** Confirm correct parsing.
- **Steps:**
  1. Embedded replies with `LEN=7`:

     Example (deviceId=1, maxV=260, maxI=30, maxP=2000):

     ```text
     AA 07 0C  01  01 04  00 1E  07 D0
     ```

- **Expected (App):**
  - Device ID = 1
  - Max Voltage = 260
  - Max Current = 30
  - Max Power = 2000

#### CP-TC-203: GET_DEVICE_INFO wrong length

- **Type:** Negative
- **Direction:** Embedded → App
- **Steps:**
  1. Embedded sends `LEN != 7` (e.g., `LEN=6`):

     ```text
     AA 06 0C 01 01 04 00 1E 07
     ```

- **Expected (App):**
  - Ignore or fail silently (no crash).

---

### 6.3 CONTROL_RELAY (0x09)

#### CP-TC-301: CONTROL_RELAY ON

- **Type:** Command
- **Direction:** App → Embedded
- **Goal:** Verify correct payload and embedded action.
- **Steps:**
  1. In the app, turn ON relay index 0.
  2. Embedded should see:

     ```text
     AA 02 09 00 01
     ```

- **Expected (Embedded):**
  - Relay 0 turns ON.
- **Expected (App):**
  - No response required.

#### CP-TC-302: CONTROL_RELAY OFF

- **Steps:**
  1. In the app, turn OFF relay index 0.
  2. Embedded should see:

     ```text
     AA 02 09 00 00
     ```

- **Expected (Embedded):**
  - Relay 0 turns OFF.

#### CP-TC-303: CONTROL_RELAY invalid relay index

- **Type:** Negative
- **Direction:** App → Embedded
- **Goal:** Ensure embedded handles unexpected indices safely.
- **Steps:**
  1. Inject frame (from a tester) with invalid index:

     ```text
     AA 02 09 FF 01
     ```

- **Expected (Embedded):**
  - Do not crash.
  - Ignore or clamp index based on firmware policy.

---

### 6.4 WRITE_EEPROM / Reset Energy (0x04)

#### CP-TC-401: Reset energy command is sent by App

- **Type:** Command
- **Direction:** App → Embedded
- **Goal:** Verify app frame content.
- **Steps:**
  1. Trigger “reset energy counter” in app.
  2. Embedded should receive:

     ```text
     AA 04 04 00 00 00 00
     ```

- **Expected (Embedded):**
  - Energy counter becomes 0.
  - EEPROM is updated (if implemented).

#### CP-TC-402: Reset energy persistence check

- **Type:** Functional + Communication
- **Direction:** App → Embedded + Embedded → App
- **Goal:** Confirm the reset is reflected in subsequent RMS readings.
- **Steps:**
  1. Execute CP-TC-401.
  2. After reset, perform CP-TC-102 responses with energy counter low/zero.
- **Expected (App):**
  - Energy displayed drops to near zero.
- **Failure hints:**
  - If relay works but energy never resets: embedded command handler issue.

---

### 6.5 NOTIFICATION (0x07) (Embedded → App)

> Important: the current mobile parser does not implement handling for `0x07`.

#### CP-TC-501: NOTIFICATION is ignored safely by App

- **Type:** Compatibility
- **Direction:** Embedded → App
- **Goal:** Ensure app does not crash on unsolicited `0x07`.
- **Steps:**
  1. Embedded sends:

     ```text
     AA 02 07 01 00
     ```

- **Expected (App):**
  - No crash.
  - No UI change (current behavior).

---

## 7) Unimplemented / TBD Commands (0x03, 0x06, 0x08, 0x0A, 0x0B)

These command IDs exist in mobile constants, but mobile may not include UI/flows for them and/or does not parse their responses.

### CP-TC-601: Embedded should not break if it receives unknown/unused commands

- **Type:** Robustness
- **Direction:** App → Embedded (or injected)
- **Steps:**
  1. Inject one of the following frames:

     ```text
     AA 00 03
     AA 00 06
     AA 00 08
     AA 00 0A
     AA 00 0B
     ```

- **Expected (Embedded):**
  - Does not crash.
  - Either ignores, or sends a defined “not supported” response (if firmware implements one).

---

## 8) Quick Triage Guide (Who is failing?)

### 8.1 App → Embedded path failure indicators

- Embedded never sees `AA 00 05` periodically after connect:
  - Likely app timer/send path issue OR SPP output not working.

### 8.2 Embedded → App path failure indicators

- Embedded logs show it is sending valid frames, but app UI never updates:
  - Likely app parser not receiving bytes (BT input) or command handler mismatch.

### 8.3 Endianness/scaling mismatch indicators

- App updates, but values are nonsense (very large or too small):
  - Likely embedded packing order or scaling mismatch.

---

## 9) Master Summary Table (All Test Cases)

| Case ID | Area | Command | Direction | What it verifies | Expected outcome | Likely owner if fails |
| --- | --- | --- | --- | --- | --- | --- |
| CP-TC-001 | Framing | - | Emb → App | Basic buffering + consumption | No crash / no lock-up | App |
| CP-TC-002 | Framing | - | Emb → App | Split frame reassembly | Decodes after full data | App |
| CP-TC-003 | Framing | - | Emb → App | Multiple frames in one read | Both frames consumed | App |
| CP-TC-004 | Framing | - | Emb → App | Garbage before header | Parser resyncs at 0xAA | App |
| CP-TC-005 | Framing | - | Emb → App | No header at all | Buffer cleared safely | App |
| CP-TC-006 | Framing | - | Emb → App | LEN too large | Waits for remaining bytes | App |
| CP-TC-007 | Compatibility | - | Emb → App | Unknown CMD | Frame ignored safely | App |
| CP-TC-101 | Request | 0x05 | App → Emb | App sends GET_RMS_DATA | `AA 00 05` every ~1s | App/Transport |
| CP-TC-102 | Response | 0x05 | Emb → App | RMS response parsing + scaling | UI updates with correct values | Embedded (packing) / App (parsing) |
| CP-TC-103 | Negative | 0x05 | Emb → App | Wrong LEN handling | No crash, ignore | App |
| CP-TC-104 | Diagnostic | 0x05 | Emb → App | Endianness detection | Correct small values | Embedded |
| CP-TC-105 | Behavioral | 0x05 | Emb → App | Unsolicited RMS frames | Still updates | App |
| CP-TC-201 | Request | 0x0C | App → Emb | App sends GET_DEVICE_INFO | `AA 00 0C` | App |
| CP-TC-202 | Response | 0x0C | Emb → App | Device info parsing | Values decoded correctly | Embedded/App |
| CP-TC-203 | Negative | 0x0C | Emb → App | Wrong LEN handling | No crash, ignore | App |
| CP-TC-301 | Command | 0x09 | App → Emb | Relay ON frame + action | Relay turns ON | Embedded |
| CP-TC-302 | Command | 0x09 | App → Emb | Relay OFF frame + action | Relay turns OFF | Embedded |
| CP-TC-303 | Negative | 0x09 | App → Emb | Invalid relay index handling | No crash | Embedded |
| CP-TC-401 | Command | 0x04 | App → Emb | Reset energy command | Counter reset | Embedded |
| CP-TC-402 | Functional | 0x04/0x05 | Both | Reset reflected in readings | Energy drops | Embedded |
| CP-TC-501 | Compatibility | 0x07 | Emb → App | Notification ignored safely | No crash | App |
| CP-TC-601 | Robustness | 0x03/0x06/0x08/0x0A/0x0B | Injected → Emb | Unknown/unused command handling | No crash | Embedded |

---

## End of Document
