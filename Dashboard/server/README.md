# Smart Energy IoT Server

## Overview
This Node.js server acts as a bridge between the ESP-01 WiFi module (embedded system) and the Web Dashboard for remote monitoring.

## Architecture
```
ESP-01 (WiFi) ←→ TCP Server (Port 3001) ←→ WebSocket Server (Port 3000) ←→ Dashboard (Browser)
```

## Features
- **TCP Server**: Receives data from ESP-01 on port 3001
- **WebSocket Server**: Sends data to Dashboard on port 3000
- **Bidirectional**: Commands from Dashboard are forwarded to ESP-01
- **CORS Enabled**: Accepts connections from any origin (configurable)

## Prerequisites
- Node.js 14 or higher
- npm or yarn

## Installation

```bash
cd server
npm install
```

## Usage

### Start Server
```bash
npm start
```

### Development Mode (with auto-reload)
```bash
npm run dev
```

## Configuration

### Ports
- **TCP Port**: 3001 (ESP-01 connection)
- **HTTP/WebSocket Port**: 3000 (Dashboard connection)

To change ports, edit `index.js`:
```javascript
const TCP_PORT = 3001;
const HTTP_PORT = 3000;
```

## Testing

### Test with Dashboard
1. Start server: `npm start`
2. Open Dashboard in browser
3. Click "Mode" button → Switch to "Remote Mode"
4. Click "Connect Server"
5. Dashboard should show "Connected (Remote)"

### Test ESP-01 Connection
Use TCP client to simulate ESP-01:
```bash
nc localhost 3001
```

Then send hex data:
```
AA 10 05 [16 bytes of float data]
```

## Troubleshooting

### Dashboard Can't Connect
- Check server is running: `npm start`
- Verify port 3000 is not in use: `netstat -an | grep 3000`
- Check firewall settings

### ESP-01 Can't Connect
- Verify ESP-01 WiFi connected
- Check ESP-01 configured to connect to server IP:port
- Ensure port 3001 is open

## Protocol

### ESP-01 → Server (TCP)
Raw binary frames:
```
[0xAA][LENGTH][COMMAND][DATA...]
```

### Server → Dashboard (WebSocket)
JSON event `data-packet`:
```json
{
  "event": "data-packet",
  "data": [0xAA, 0x10, 0x05, ...]
}
```

### Dashboard → Server (WebSocket)
JSON event `command`:
```json
{
  "cmd": 0x05,
  "frame": [0xAA, 0x00, 0x05]
}
```

### Server → ESP-01 (TCP)
Raw binary frame forwarded as-is.

## Development

### Project Structure
```
server/
├── index.js       # Main server file
├── package.json   # Dependencies
└── README.md      # This file
```

### Dependencies
- `express`: HTTP server
- `socket.io`: WebSocket library
- `cors`: Cross-origin resource sharing
- `nodemon`: Development auto-reload (dev only)

## Production Deployment

### Using PM2 (Recommended)
```bash
npm install -g pm2
pm2 start index.js --name energy-server
pm2 save
pm2 startup
```

### Using systemd
Create `/etc/systemd/system/energy-server.service`:
```ini
[Unit]
Description=Smart Energy IoT Server
After=network.target

[Service]
Type=simple
User=yourusername
WorkingDirectory=/path/to/Dashboard/server
ExecStart=/usr/bin/node index.js
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

Then:
```bash
sudo systemctl enable energy-server
sudo systemctl start energy-server
```

## Security Considerations

⚠️ **Production Security**:
- Change CORS origin from `*` to specific Dashboard URL
- Add authentication for WebSocket connections
- Use HTTPS/WSS in production
- Implement rate limiting
- Add input validation

Example secure configuration:
```javascript
const io = new Server(server, {
    cors: {
        origin: "https://yourdomain.com",
        methods: ["GET", "POST"],
        credentials: true
    }
});
```

## License
ISC - Gestell Company

## Support
For issues or questions, contact: Hisham4Ahmed@gmail.com
