# Smart Energy Dashboard - Professional Setup

## 🚀 Quick Start

### Prerequisites
- Node.js 16+ installed
- npm or yarn package manager

### Installation

1. Install dependencies:
```bash
npm install
```

2. Start the dashboard server:
```bash
npm start
```

3. Open your browser and navigate to:
```
http://localhost:3000
```

## 📋 Available Scripts

- `npm start` - Start the production server
- `npm run dev` - Start development server with auto-reload (requires nodemon)
- `npm run build` - Build for production (static files)
- `npm test` - Run tests

## 🔧 Configuration

### Port Configuration
By default, the server runs on port `3000`. You can change this by setting the `PORT` environment variable:

```bash
PORT=8080 npm start
```

### Host Configuration
To bind to a specific network interface:

```bash
HOST=192.168.1.100 npm start
```

## 🌐 Connection Modes

The dashboard supports two connection modes:

### 1. Local Mode (Bluetooth/Serial)
- Uses Web Serial API
- Direct connection to embedded device via HC-05 Bluetooth or USB-Serial
- Requires HTTPS or localhost
- Click "Connect Device" button

### 2. Remote IoT Mode
- Uses WebSocket (Socket.IO)
- Connects to Node.js server
- Server communicates with embedded device
- Allows remote monitoring
- Click "Remote IoT" button to switch modes

## 📱 Features

### Dashboard View
- Real-time voltage, current, power, and energy monitoring
- Live charts with historical data
- Metric cards with mini visualizations
- Auto-updating values

### Controls View
- Relay master switch
- Protection auto-reset toggle
- Voltage and current calibration
- WiFi configuration (SSID/Password provisioning)

### Logs View
- System event logging
- Timestamped entries
- Color-coded by severity (info, warning, error, success)

### Demo Mode
- Test dashboard without hardware
- Simulated sensor data
- Useful for development and presentation

## 🔌 API Integration

### WebSocket Events (Server ↔ Dashboard)

**From Dashboard to Server:**
- `command` - Send control commands to device
  ```javascript
  socket.emit('command', { cmd: 0x05, frame: [0xAA, 0x00, 0x05] });
  ```

**From Server to Dashboard:**
- `data-packet` - Receive sensor data from device
  ```javascript
  socket.on('data-packet', (dataArray) => {
      // Process data
  });
  ```

- `server-status` - Server connection status
- `command-ack` - Command acknowledgment

### Serial Protocol

The dashboard uses a simple frame-based protocol:

```
[HEADER][LENGTH][COMMAND][DATA...]
```

- HEADER: 0xAA (1 byte)
- LENGTH: Data length (1 byte)
- COMMAND: Command code (1 byte)
- DATA: Variable length payload

**Command Codes:**
- `0x05` - Get RMS measurements (V, I, P, E)
- `0x09` - Relay control
- `0x0B` - Calibration
- `0x0C` - WiFi configuration

## 🛠️ Development

### File Structure
```
Dashboard/
├── index.html          # Main dashboard UI
├── style.css           # Styling and themes
├── script.js           # Client-side logic
├── server.js           # Node.js backend server
├── package.json        # Dependencies and scripts
└── README.md           # This file
```

### Browser Compatibility
- Chrome/Edge 89+ (Web Serial API support)
- Firefox/Safari (Remote mode only)
- Requires HTTPS for Web Serial API (except localhost)

### Debugging
- Open browser Developer Tools (F12)
- Check Console for logs
- Monitor Network tab for WebSocket connections

## 🔒 Security Notes

- Web Serial API only works on HTTPS or localhost
- WiFi credentials are sent via Bluetooth to device
- No credentials stored in browser
- Use HTTPS in production environments

## 📊 Performance

- Real-time updates every 1 second (configurable)
- Chart maintains last 20 data points
- Efficient WebSocket communication
- Low latency (<100ms typical)

## 🐛 Troubleshooting

### "Web Serial API not supported"
- Use Chrome/Edge browser
- Ensure localhost or HTTPS

### Connection fails
- Check Bluetooth pairing (Local mode)
- Verify server is running (Remote mode)
- Check firewall settings
- Verify device is powered on

### No data displayed
- Enable Demo Mode to test UI
- Check browser console for errors
- Verify serial port permissions
- Check device firmware communication protocol

## 📞 Support

For issues or questions:
- Check embedded system documentation
- Review communication protocol specs
- Enable demo mode for UI testing

---

**Gestell - Smart Energy Management System**  
Version 1.0.0
