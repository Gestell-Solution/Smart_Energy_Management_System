# ⚡ Smart Energy Management System - Dashboard

<div align="center">

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![Node](https://img.shields.io/badge/node-%3E%3D16.0.0-green.svg)
![License](https://img.shields.io/badge/license-MIT-yellow.svg)

**Professional Web Dashboard for Real-time Energy Monitoring and Control**

[Quick Start](#-quick-start) • [Features](#-features) • [Documentation](#-documentation) • [Support](#-support)

</div>

---

## 📋 Table of Contents

- [Overview](#overview)
- [Quick Start](#-quick-start)
- [Features](#-features)
- [Connection Modes](#-connection-modes)
- [Documentation](#-documentation)
- [Project Structure](#-project-structure)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)

---

## Overview

A professional, real-time web dashboard for monitoring and controlling the Smart Energy Management System. Built with modern web technologies, featuring both local (Bluetooth/Serial) and remote (IoT/WebSocket) connectivity options.

### Key Highlights

✨ **Modern UI** - Clean, responsive interface with real-time updates  
🔌 **Dual Connectivity** - Local (Bluetooth) and Remote (IoT) modes  
📊 **Live Charts** - Real-time visualization of energy consumption  
⚙️ **Full Control** - Calibration, WiFi setup, and relay control  
🎯 **Demo Mode** - Test without hardware connection  
🏢 **Professional** - Gestell branded, production-ready

---

## 🚀 Quick Start

### Method 1: Automated (Recommended)

```bash
./start.sh
```

This script automatically:
- ✅ Checks Node.js and npm installation
- ✅ Installs dependencies
- ✅ Starts the server

### Method 2: Manual Setup

```bash
# Install dependencies
npm install

# Start the server
npm start

# Open browser
# Navigate to: http://localhost:3000
```

### First Time Setup

1. **Install Node.js** (if not already installed)
   - Download from [nodejs.org](https://nodejs.org/)
   - Recommended: v16.x or higher

2. **Clone/Navigate** to Dashboard directory

3. **Run** the automated script or manual commands above

4. **Access** the dashboard at `http://localhost:3000`

---

## ✨ Features

### 📊 Real-time Monitoring Dashboard
- **Live Metrics Display**
  - Voltage (V)
  - Current (A)
  - Power (W)
  - Energy (kWh)
- **Historical Charts** with configurable time ranges (1H, 6H, 24H)
- **Auto-refresh** every second
- **Visual Indicators** with color-coded metric cards

### ⚙️ Advanced Controls
- **Relay Control**
  - Master switch ON/OFF
  - Auto-reset protection
- **Calibration Settings**
  - Voltage correction factor
  - Current correction factor
- **WiFi Provisioning**
  - Configure SSID and Password
  - Save directly to device via Bluetooth
  - Persistent storage in device EEPROM

### 📝 Event Logging
- Timestamped system events
- Color-coded severity levels (info, warning, error, success)
- Real-time log updates
- Clearable log history

### 🎮 Demo Mode
- Test dashboard without hardware
- Simulated sensor data
- Perfect for development and presentations

---

## 🔌 Connection Modes

### Local Mode (Bluetooth/Serial)
- **Technology:** Web Serial API
- **Connection:** Direct via HC-05 Bluetooth or USB-Serial
- **Requirements:**
  - Chrome/Edge browser (89+)
  - HTTPS or localhost
- **Advantages:**
  - Low latency (<50ms)
  - No internet required
  - Direct device communication

**Usage:**
1. Click "Connect Device" button
2. Select serial port from browser dialog
3. Start monitoring

### Remote IoT Mode
- **Technology:** WebSocket (Socket.IO)
- **Connection:** Internet via Node.js server
- **Requirements:**
  - Running Node.js server
  - Internet connection
- **Advantages:**
  - Remote access from anywhere
  - Multi-user support
  - Centralized data logging

**Usage:**
1. Click "Local Mode" button to switch to "Remote IoT"
2. Click "Connect Server"
3. Server handles device communication

---

## 📚 Documentation

| Document | Language | Description |
|----------|----------|-------------|
| [SETUP_GUIDE.md](./SETUP_GUIDE.md) | 🇬🇧 English | Complete setup and usage guide |
| [START_HERE.md](./START_HERE.md) | 🇬🇧 English | Quick start guide for first-time users |
| [QUICK_REFERENCE.txt](./QUICK_REFERENCE.txt) | 🇬🇧 English | Quick reference card |
| [CODE_ANALYSIS.md](./CODE_ANALYSIS.md) | 🇬🇧 English | Comprehensive code analysis report |
| [embedded_requirements.md](./embedded_requirements.md) | 🇬🇧 English | Embedded system requirements |

### Quick Links

- 📖 [Complete Setup Guide](./SETUP_GUIDE.md) - Full documentation
- 🚀 [First-Time User Guide](./START_HERE.md) - Start here
- 🎯 [Quick Reference](./QUICK_REFERENCE.txt) - Cheat sheet
- 📊 [Code Analysis](./CODE_ANALYSIS.md) - Technical details
- 🔧 [Embedded Requirements](./embedded_requirements.md) - Hardware specs

---

## 📁 Project Structure

```
Dashboard/
├── index.html              # Main UI - Dashboard interface
├── style.css               # Styling - Modern dark theme
├── script.js               # Frontend logic - Client communication
├── server.js               # Backend server - Express + Socket.IO
├── package.json            # Dependencies and scripts
├── start.sh                # Automated launch script
├── .env.example            # Environment configuration template
├── .gitignore              # Git ignore rules
│
├── README.md               # This file
├── SETUP_GUIDE.md          # English setup guide
├── SETUP_GUIDE_AR.md       # Arabic setup guide
├── QUICK_REFERENCE.txt     # Quick reference card
│
└── embedded_*.md           # Embedded system documentation
```

---

## 🛠️ NPM Scripts

```bash
npm start           # Start production server
npm run dev         # Development mode with auto-reload
npm test            # Run tests
npm install         # Install dependencies
```

---

## 🐛 Troubleshooting

### Web Serial API Not Supported
**Problem:** Browser doesn't support Web Serial API  
**Solution:** Use Chrome or Edge browser (version 89+)

### Connection Failed
**Problem:** Cannot connect to device  
**Solutions:**
- **Local Mode:**
  - Check Bluetooth pairing
  - Verify device is powered on
  - Check serial port permissions
- **Remote Mode:**
  - Ensure server is running (`npm start`)
  - Check firewall settings
  - Verify correct server URL in code

### No Data Displayed
**Problem:** Dashboard shows no values  
**Solutions:**
1. Try **Demo Mode** to verify UI works
2. Open Developer Tools (F12) and check Console
3. Verify embedded device firmware protocol
4. Check serial port configuration

### Server Won't Start
**Problem:** `npm start` fails  
**Solution:**
```bash
# Reinstall dependencies
rm -rf node_modules
npm install

# Try starting again
npm start
```

### Port Already in Use
**Problem:** Port 3000 already occupied  
**Solution:**
```bash
# Use different port
PORT=8080 npm start
```

---

## 🔐 Security Notes

- Web Serial API requires HTTPS or localhost
- WiFi credentials sent via Bluetooth (not stored in browser)
- Use HTTPS in production environments
- Implement authentication for remote access

---

## 📊 Technical Specifications

| Feature | Specification |
|---------|--------------|
| **Update Rate** | 1 second |
| **Chart Points** | 20 data points |
| **Latency** | <100ms (typical) |
| **Browser Support** | Chrome 89+, Edge 89+ (full), Firefox/Safari (remote only) |
| **Protocol** | Frame-based binary (0xAA header) |
| **WebSocket** | Socket.IO v4.7.2 |
| **Node.js** | v16+ required |

---

## 🌟 Features at a Glance

| Feature | Status | Description |
|---------|--------|-------------|
| Real-time Monitoring | ✅ | Live V, I, P, E readings |
| Historical Charts | ✅ | Power consumption over time |
| Bluetooth Connection | ✅ | Web Serial API support |
| Remote IoT | ✅ | WebSocket connectivity |
| WiFi Provisioning | ✅ | Configure device WiFi |
| Calibration | ✅ | Adjust sensor readings |
| Demo Mode | ✅ | Simulated data for testing |
| Event Logging | ✅ | Timestamped system events |
| Responsive UI | ✅ | Works on all screen sizes |
| Dark Theme | ✅ | Modern, easy on eyes |

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

---

## 📄 License

This project is licensed under the MIT License.

---

## 📞 Support

For issues or questions:
- Check documentation files
- Review troubleshooting section
- Enable demo mode for UI testing
- Check browser console for errors

---

<div align="center">

**Gestell - Smart Energy Management System**  
Professional Dashboard v1.0.0

© 2025 Gestell. All rights reserved.

[Documentation](#-documentation) • [Quick Start](#-quick-start) • [Features](#-features)

</div>
