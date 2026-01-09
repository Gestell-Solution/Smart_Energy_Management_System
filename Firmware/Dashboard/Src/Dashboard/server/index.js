const net = require('net');
const http = require('http');
const { Server } = require("socket.io");
const express = require('express');
const cors = require('cors');

const app = express();
app.use(cors());

const server = http.createServer(app);
const io = new Server(server, {
    cors: {
        origin: "*", // Allow all origins for simplicity in this demo
        methods: ["GET", "POST"]
    }
});

// Store connected clients
let espClient = null;

// --- TCP Server (For ESP-01) ---
const TCP_PORT = 3001;
const tcpServer = net.createServer((socket) => {
    console.log('ESP-01 Connected via TCP');
    espClient = socket;

    socket.on('data', (data) => {
        // data is a Buffer
        console.log('Received from ESP:', data.toString('hex'));

        // Forward to Dashboard via WebSocket
        // We broadcast to all connected dashboards
        io.emit('data-packet', Array.from(data)); // Convert buffer to array for JSON serialization
    });

    socket.on('end', () => {
        console.log('ESP-01 Disconnected');
        espClient = null;
    });

    socket.on('error', (err) => {
        console.error('TCP Error:', err);
    });
});

tcpServer.listen(TCP_PORT, '0.0.0.0', () => {
    console.log(`TCP Server listening on port ${TCP_PORT} (for ESP-01)`);
});

// --- WebSocket / HTTP Server (For Dashboard) ---
const HTTP_PORT = 3000;

io.on('connection', (socket) => {
    console.log('Dashboard Connected via WebSocket');

    socket.on('command', (command) => {
        // command should be object { cmd: 0xXX, data: [...] }
        console.log('Command from Dashboard:', command);

        if (espClient) {
            // Construct frame: Header(AA) + Len + Cmd + Data
            // Dashboard sends raw payload or formatted?
            // Let's assume dashboard sends just the payload { cmd, data }
            // and we construct the frame here OR dashboard sends fully constructed frame.
            // Let's rely on dashboard logic to be consistent with Bluetooth.
            // Dashboard (updated script.js) will send the raw bytes of the frame.

            const frame = Buffer.from(command.frame);
            espClient.write(frame);
            console.log('Forwarded command to ESP');
        } else {
            console.log('ESP not connected, cannot forward command');
        }
    });

    socket.on('disconnect', () => {
        console.log('Dashboard Disconnected');
    });
});

server.listen(HTTP_PORT, () => {
    console.log(`HTTP/WebSocket Server listening on port ${HTTP_PORT} (for Dashboard)`);
});
