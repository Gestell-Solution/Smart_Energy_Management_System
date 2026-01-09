/**
 * Smart Energy Management System - Dashboard Server
 * Professional Web Server with WebSocket Support
 * 
 * Features:
 * - Serves static dashboard files
 * - WebSocket communication for real-time data
 * - Serial port integration for embedded device
 * - CORS support for development
 * 
 * @author Gestell Team
 * @version 1.0.0
 */

const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const cors = require('cors');
const path = require('path');

// Configuration
const PORT = process.env.PORT || 3000;
const HOST = process.env.HOST || '0.0.0.0';

// Initialize Express App
const app = express();
const server = http.createServer(app);
const io = socketIO(server, {
    cors: {
        origin: "*",
        methods: ["GET", "POST"]
    }
});

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname)));

// Routes
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/health', (req, res) => {
    res.json({
        status: 'ok',
        timestamp: new Date().toISOString(),
        clients: io.engine.clientsCount
    });
});

// WebSocket Connection Handling
io.on('connection', (socket) => {
    console.log(`✅ Client connected: ${socket.id}`);
    console.log(`📊 Total clients: ${io.engine.clientsCount}`);

    // Send welcome message
    socket.emit('server-status', {
        message: 'Connected to Smart Energy Server',
        timestamp: new Date().toISOString()
    });

    // Handle commands from dashboard
    socket.on('command', (data) => {
        console.log(`📨 Command received from ${socket.id}:`, data);

        // Here you would forward commands to the embedded device
        // For now, we'll echo back for testing
        socket.emit('command-ack', {
            cmd: data.cmd,
            status: 'received',
            timestamp: new Date().toISOString()
        });
    });

    // Handle data from embedded device (to be forwarded to dashboard)
    socket.on('device-data', (data) => {
        console.log('📡 Device data received:', data);
        // Broadcast to all connected dashboards
        io.emit('data-packet', data);
    });

    // Handle disconnection
    socket.on('disconnect', () => {
        console.log(`❌ Client disconnected: ${socket.id}`);
        console.log(`📊 Total clients: ${io.engine.clientsCount}`);
    });

    // Error handling
    socket.on('error', (error) => {
        console.error(`⚠️ Socket error for ${socket.id}:`, error);
    });
});

// Start Server
server.listen(PORT, HOST, () => {
    console.log('');
    console.log('═══════════════════════════════════════════════════════════');
    console.log('⚡ Smart Energy Management System - Dashboard Server');
    console.log('═══════════════════════════════════════════════════════════');
    console.log(`🌐 Server running at: http://${HOST}:${PORT}`);
    console.log(`🔗 Local access: http://localhost:${PORT}`);
    console.log(`📁 Serving from: ${__dirname}`);
    console.log(`⏰ Started at: ${new Date().toISOString()}`);
    console.log('═══════════════════════════════════════════════════════════');
    console.log('');
    console.log('📌 Available endpoints:');
    console.log(`   - Dashboard UI: http://localhost:${PORT}/`);
    console.log(`   - Health Check: http://localhost:${PORT}/health`);
    console.log('');
    console.log('🔌 WebSocket server ready for connections');
    console.log('💡 Press Ctrl+C to stop the server');
    console.log('');
});

// Graceful shutdown
process.on('SIGTERM', () => {
    console.log('\n🛑 SIGTERM received. Shutting down gracefully...');
    server.close(() => {
        console.log('✅ Server closed');
        process.exit(0);
    });
});

process.on('SIGINT', () => {
    console.log('\n\n🛑 SIGINT received. Shutting down gracefully...');
    server.close(() => {
        console.log('✅ Server closed');
        process.exit(0);
    });
});

// Error handling
process.on('uncaughtException', (error) => {
    console.error('❌ Uncaught Exception:', error);
    process.exit(1);
});

process.on('unhandledRejection', (reason, promise) => {
    console.error('❌ Unhandled Rejection at:', promise, 'reason:', reason);
    process.exit(1);
});
