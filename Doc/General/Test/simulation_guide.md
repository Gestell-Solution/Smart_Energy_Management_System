# Simulation Guide (Linux & Windows)

This guide explains how to simulate the **Smart Energy Management System** and connect it to the **Web Dashboard** without physical hardware.

## The Concept
We create a **"Virtual Serial Cable"** with two ends (Port A and Port B).
1.  **Simulator (Proteus / Simulide)** connects to **Port A** (TX/RX).
2.  **Web Dashboard (Chrome)** connects to **Port B**.

---

## 🏗️ Option 1: Linux Setup

### 1. Create Virtual Ports
We use the `socat` tool.
1.  Install socat:
    ```bash
    sudo apt install socat
    ```
2.  Run this command (keep the terminal open!):
    ```bash
    socat -d -d pty,raw,echo=0 pty,raw,echo=0
    ```
3.  Note the output ports (e.g., `/dev/pts/2` and `/dev/pts/3`).

### 2. Configure Simulator (Simulide)
1.  Add a **Uart** component.
2.  Connect **TX** to MCU **RXD**, and **RX** to MCU **TXD**.
3.  Right-click Uart -> Properties.
4.  Set Port to the **first** virtual port (e.g., `/dev/pts/2`).
5.  Set Baud Rate to **9600**.

### 3. Connect Dashboard
1.  Open `Dashboard/index.html` in **Chrome**.
2.  Click **Connect Device**.
3.  Select the **second** virtual port (e.g., `/dev/pts/3`).
    *   *Note: You might need `sudo chmod 666 /dev/pts/3` if permission is denied.*

---

## 🪟 Option 2: Windows Setup

### 1. Create Virtual Ports
You need a 3rd party tool. We recommend **com0com** (Open Source) or **VSPE**.

#### Using com0com (Recommended):
1.  Download and install **com0com**.
2.  Open "Setup Command Prompt" for com0com.
3.  Create a pair (e.g., `CNCA0` and `CNCB0`) or rename them to `COM8` and `COM9`.
    *   Example: Pair `COM8` <-> `COM9`.

### 2. Configure Simulator (Proteus)
1.  Add the **COMPIM** component from the library.
2.  Connect:
    *   **COMPIM TXD** -> **ATmega32 TXD** (Pin 15).
    *   **COMPIM RXD** -> **ATmega32 RXD** (Pin 14).
3.  Double-click COMPIM:
    *   **Physical Port**: `COM8`.
    *   **Physical Baud Rate**: `9600`.
    *   **Virtual Baud Rate**: `9600`.

### 3. Connect Dashboard
1.  Open `Dashboard/index.html` in **Chrome** or **Edge**.
2.  Click **Connect Device**.
3.  Select the other port pair: `COM9`.
4.  You should see the data flowing immediately!

---

## 🌐 Simulating WiFi (IoT Mode) on Localhost

To test the **IoT Dashboard** (Remote Mode):

1.  **Start the Server**:
    ```bash
    cd Server
    node index.js
    ```
2.  **Configure ESP-01 (Harder to simulate)**:
    *   Simulating the ESP-01 AT commands fully is difficult in Proteus/Simulide.
    *   **Workaround**: Use the **Serial/Bluetooth** mode for simulation verification. It uses the exact same `App_CommManager` logic.
