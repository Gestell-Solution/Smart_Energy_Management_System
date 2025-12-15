# 🎉 Gestell Energy Dashboard - Enhanced Features

## ✨ New Features Added

### Dashboard has been enhanced with the following professional features:

---

## 📊 1. Real-time Monitoring (Enhanced)

### Main Metrics Cards
- **Voltage (V)** - Real-time voltage measurement with status indicator
- **Current (A)** - Real-time current measurement with status indicator
- **Power (W)** - Real-time power calculation with status indicator
- **Energy (kWh)** - Cumulative energy consumption with monitoring status

Each card now includes:
- ✅ Large value display
- ✅ Unit labels
- ✅ Status indicators (Normal/Warning/Alert)
- ✅ Mini trend charts (concept)
- ✅ Color-coded icons

---

## 📈 2. Statistics Dashboard

### New Statistics Cards
1. **Uptime** - System running time (hours & minutes)
2. **Estimated Cost** - Energy cost calculation ($0.15/kWh)
3. **Average Power** - Mean power consumption
4. **Peak Power** - Maximum power recorded

```
┌─────────────┬─────────────┬─────────────┬─────────────┐
│   Uptime    │  Est. Cost  │  Avg Power  │ Peak Power  │
│    0h 0m    │   $0.00     │    0.0 W    │    0.0 W    │
└─────────────┴─────────────┴─────────────┴─────────────┘
```

---

## 🔌 3. Four Relay Control System

### Individual Relay Controls
Each relay has:
- **Large ON/OFF Toggle Switch** - Professional switch design
- **Status Indicator** - ON (green) / OFF (red)
- **Load Tracking** - Real-time power consumption per relay
- **Runtime Counter** - Total operating time per relay

```
╔══════════════════════════════════╗
║  Relay 1                  [OFF]  ║
║                                  ║
║         ┌────────┐               ║
║         │   ○────│  OFF          ║
║         └────────┘               ║
║                                  ║
║  Load: 0.0 W    Runtime: 0h 0m   ║
╚══════════════════════════════════╝
```

### Relay Features:
- **Relay 1-4** - Individual control
- **Status Display** - Real-time ON/OFF state
- **Load Monitoring** - Power consumption per relay (concept)
- **Runtime Tracking** - Operation time tracking

### Master Control
- **Turn All ON** - Activate all 4 relays at once
- **Turn All OFF** - Deactivate all 4 relays at once

---

## 🎮 4. Enhanced Navigation

### Sidebar Tabs:
1. **Dashboard** - Main monitoring view
2. **Controls** - Calibration & WiFi provisioning
3. **Relays** - ⭐ NEW: 4 Relay control panel
4. **Logs** - System event logging
5. **Settings** - Thresholds & preferences

---

## ⚙️ 5. Professional Controls

### System Control
- **Protection Auto-Reset** - Automatic fault recovery
- **Data Logging** - Enable/disable data recording
- **Alerts Enabled** - System notifications

### Calibration
- **Voltage Factor** - Correction multiplier (0.5 - 2.0)
- **Current Factor** - Correction multiplier (0.5 - 2.0)
- **Apply Button** - Send calibration to device

### WiFi Provisioning
- **SSID Input** - Network name
- **Password Input** - Network password
- **Save via Bluetooth** - Store in device EEPROM

---

## 📝 6. System Logs

Enhanced logging system with:
- **Timestamps** - Precise time for each event
- **Color Coding:**
  - 🔵 Info (blue)
  - 🟢 Success (green)
  - 🟡 Warning (yellow)
  - 🔴 Error (red)
- **Clear Logs** - One-click log clearing
- **Auto-scroll** - Latest logs at top

---

## 🔧 7. Settings Panel

### Thresholds Configuration
- **Max Voltage** - Upper limit (default: 250V)
- **Min Voltage** - Lower limit (default: 200V)
- **Max Current** - Upper limit (default: 10A)

### Display Settings
- **Dark Mode Toggle** - Light/Dark theme (default: Dark)
- **Auto-refresh Toggle** - Real-time updates
- **Refresh Rate** - Update interval (1-10 seconds)

### About Section
- Company information
- Version number
- Copyright notice

---

## 🌐 8. Dual Connectivity Modes

### Local Mode (Bluetooth/Serial)
- Direct connection via HC-05
- Web Serial API
- Low latency (<50ms)
- No internet required

### Remote IoT Mode  
- WebSocket (Socket.IO)
- Internet-based monitoring
- Multi-user support
- Cloud connectivity

**Toggle between modes** with one click!

---

## 🎨 9. Professional UI Design

### Design Features:
- **Dark Theme** - Easy on eyes, energy efficient
- **Glassmorphism** - Modern translucent cards
- **Smooth Animations** - Fade-in effects
- **Responsive Layout** - Works on all screen sizes
- **Color-coded Elements** - Visual status indicators
- **Professional Icons** - Font Awesome 6.4.0

### Color Scheme:
- Primary: Blue (#3b82f6)
- Success: Green (#10b981)
- Warning: Orange (#f59e0b)
- Danger: Red (#ef4444)

---

## 📱 10. Responsive Design

Dashboard adapts to:
- **Desktop** (1920x1080)
- **Tablet** (768x1024)
- **Mobile** (320x568)

Relay cards arrange in:
- Desktop: 4 columns
- Tablet: 2 columns
- Mobile: 1 column

---

## 🔐 11. Protocol & Communication

### Frame Protocol:
```
[HEADER:0xAA][LENGTH][COMMAND][DATA...]
```

### Commands:
- `0x05` - Get RMS measurements (V, I, P, E)
- `0x09` - Relay control (relay_num, state)
- `0x0B` - Calibration
- `0x0C` - WiFi provisioning

### Relay Command Format:
```c
uint8_t payload[2] = {relay_num, state};
// relay_num: 1-4
// state: 0=OFF, 1=ON
```

---

## 🎯 12. Demo Mode

Test dashboard without hardware:
- Simulated voltage (220V ± 5V)
- Simulated current (0.5A ± 0.1A)
- Calculated power
- Incremental energy
- Updates every second

**Perfect for:**
- Testing UI
- Presentations
- Development
- Training

---

## 📊 13. Statistics Tracking

### Auto-calculated metrics:
- **Total Energy** - Cumulative kWh
- **Average Power** - Mean over session
- **Peak Power** - Maximum recorded
- **Estimated Cost** - Based on $0.15/kWh rate
- **Runtime per Relay** - Individual tracking
- **System Uptime** - Total connection time

---

## 🚀 How to Use

### 1. Start Dashboard
```bash
./start.sh
```

### 2. Select Mode
- Click **"Local Mode"** for Bluetooth
- Or click to switch to **"Remote IoT"**

### 3. Connect
- Local: Click **"Connect Device"** → Select port
- Remote: Click **"Connect Server"**

### 4. Control Relays
- Navigate to **"Relays"** tab
- Toggle individual relays ON/OFF
- Or use **"Turn All ON/OFF"** buttons

### 5. Monitor
- View real-time metrics in **Dashboard** tab
- Check statistics cards
- Watch live power graph

---

## 📸 Dashboard Preview

```
╔═══════════════════════════════════════════════════════════╗
║  ⚡ Gestell Energy          Dashboard                     ║
║  Dashboard                Real-time monitoring            ║
║  Controls                                                 ║
║  Relays  ← NEW!          ┌──────┬──────┬──────┬──────┐   ║
║  Logs                    │220.3V│1.25A │275.4W│12.5kWh│   ║
║  Settings                └──────┴──────┴──────┴──────┘   ║
║                                                           ║
║  🟢 Connected            ┌──────┬──────┬──────┬──────┐   ║
║                          │ 1h2m │$1.88 │240.3W│320.5W│   ║
║                          └──────┴──────┴──────┴──────┘   ║
║                                                           ║
║                          [Power Consumption Chart]        ║
╚═══════════════════════════════════════════════════════════╝
```

---

## 🎊 Summary

### What's New:
✅ **4 Relay Controls** - Individual + Master control  
✅ **Statistics Cards** - Uptime, Cost, Avg/Peak Power  
✅ **Enhanced Metrics** - Status indicators on all cards  
✅ **Professional UI** - Modern, responsive design  
✅ **Settings Panel** - Thresholds & display options  
✅ **Runtime Tracking** - Per-relay operation time  
✅ **Cost Estimation** - Automatic calculation  
✅ **Demo Mode** - Full simulation capability

### Preserved Features:
✅ Voltage, Current, Power, Energy monitoring  
✅ WiFi Provisioning via Bluetooth  
✅ Calibration system  
✅ Local & Remote modes  
✅ Real-time charting  
✅ Event logging  

---

## 📞 Quick Reference

| Feature | Location | Action |
|---------|----------|--------|
| View Metrics | Dashboard tab | Auto-updates |
| Control Relays | Relays tab | Toggle switches |
| Master Control | Relays tab → Bottom | Turn All ON/OFF |
| Calibrate | Controls tab | Enter factors → Apply |
| WiFi Setup | Controls tab | Enter SSID/Pass → Save |
| View Stats | Dashboard tab → Stats row | Auto-calculated |
| Change Thresholds | Settings tab | Set limits → Save |
| Demo Mode | Top → Demo Mode button | Start/Stop |

---

<div align="center">

**Gestell Energy Dashboard**  
Professional Energy Management System v1.0.0

© 2025 Gestell. All rights reserved.

</div>
