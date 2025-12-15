/**
 * Gestell Energy Dashboard - Client Script
 * Professional Energy Management System
 * 
 * Features:
 * - Real-time monitoring (V, I, P, E)
 * - 4 Relay Controls
 * - Statistics tracking
 * - Local & Remote connectivity
 * - Demo mode for testing
 * 
 * @author Gestell Team
 * @version 1.0.0
 */

// ==================== DOM Elements ====================
const connectBtn = document.getElementById('connect-btn');
const demoBtn = document.getElementById('demo-btn');
const modeBtn = document.getElementById('mode-btn');
const statusDot = document.getElementById('status-dot');
const statusText = document.getElementById('status-text');
const navItems = document.querySelectorAll('.nav-links li');
const views = document.querySelectorAll('.view');
const logsList = document.getElementById('logs-list');

// Metric Elements
const elVoltage = document.getElementById('val-voltage');
const elCurrent = document.getElementById('val-current');
const elPower = document.getElementById('val-power');
const elEnergy = document.getElementById('val-energy');

// Statistics Elements
const elUptime = document.getElementById('stat-uptime');
const elCost = document.getElementById('stat-cost');
const elAvgPower = document.getElementById('stat-avg-power');
const elPeakPower = document.getElementById('stat-peak-power');

// Relay Elements
const relayInputs = {
    1: document.getElementById('relay-1'),
    2: document.getElementById('relay-2'),
    3: document.getElementById('relay-3'),
    4: document.getElementById('relay-4')
};

// Chart Context
const ctxMain = document.getElementById('mainChart').getContext('2d');

// ==================== State Variables ====================
let port;
let reader;
let inputStream;
let outputStream;
let isConnected = false;
let isDemoMode = false;
let demoInterval;
let socket; // Socket.io instance
let startTime = null;
let stats = {
    totalPower: 0,
    peakPower: 0,
    sampleCount: 0,
    relayRuntimes: { 1: 0, 2: 0, 3: 0, 4: 0 },
    relayStartTimes: { 1: null, 2: null, 3: null, 4: null }
};

// Constants
const FRAME_HEADER = 0xAA;
const CMD_GET_RMS = 0x05;
const CMD_CALIBRATE = 0x0B;
const CMD_RELAY = 0x09;
const CMD_UPDATE_WIFI = 0x0C;

// Connection Modes
const MODE_LOCAL = 'local';
const MODE_REMOTE = 'remote';
let currentMode = MODE_LOCAL;

// ==================== Chart Initialization ====================
const mainChart = new Chart(ctxMain, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Power (W)',
            data: [],
            borderColor: '#3b82f6',
            backgroundColor: 'rgba(59, 130, 246, 0.1)',
            fill: true,
            tension: 0.4
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            x: { grid: { color: 'rgba(255,255,255,0.1)' }, ticks: { color: '#94a3b8' } },
            y: { grid: { color: 'rgba(255,255,255,0.1)' }, ticks: { color: '#94a3b8' } }
        },
        plugins: {
            legend: { labels: { color: '#f8fafc' } }
        }
    }
});

// ==================== Event Listeners ====================
connectBtn.addEventListener('click', handleConnection);
demoBtn.addEventListener('click', toggleDemoMode);
modeBtn.addEventListener('click', toggleConnMode);

// Navigation
navItems.forEach(item => {
    item.addEventListener('click', () => {
        const tabName = item.dataset.tab;
        navItems.forEach(nav => nav.classList.remove('active'));
        item.classList.add('active');
        views.forEach(view => view.classList.remove('active'));
        document.getElementById(`view-${tabName}`).classList.add('active');
        document.getElementById('page-title').textContent = tabName.charAt(0).toUpperCase() + tabName.slice(1);
    });
});

// Relay Controls
Object.keys(relayInputs).forEach(relayNum => {
    if (relayInputs[relayNum]) {
        relayInputs[relayNum].addEventListener('change', (e) => {
            handleRelayToggle(parseInt(relayNum), e.target.checked);
        });
    }
});

// Master Relay Controls
document.getElementById('btn-all-on')?.addEventListener('click', () => {
    Object.keys(relayInputs).forEach(num => {
        if (relayInputs[num]) {
            relayInputs[num].checked = true;
            handleRelayToggle(parseInt(num), true);
        }
    });
});

document.getElementById('btn-all-off')?.addEventListener('click', () => {
    Object.keys(relayInputs).forEach(num => {
        if (relayInputs[num]) {
            relayInputs[num].checked = false;
            handleRelayToggle(parseInt(num), false);
        }
    });
});

// WiFi Provisioning
document.getElementById('btn-wifi-save')?.addEventListener('click', () => {
    const ssid = document.getElementById('wifi-ssid').value;
    const pass = document.getElementById('wifi-pass').value;

    if (!ssid || !pass) {
        log('Please enter both SSID and Password', 'warning');
        return;
    }

    log(`Sending WiFi Config: ${ssid}`, 'info');
    const payload = new Uint8Array(ssid.length + pass.length + 2);
    let idx = 0;
    for (let i = 0; i < ssid.length; i++) payload[idx++] = ssid.charCodeAt(i);
    payload[idx++] = 44; // ','
    for (let i = 0; i < pass.length; i++) payload[idx++] = pass.charCodeAt(i);
    payload[idx++] = 0; // '\0'

    sendFrame(CMD_UPDATE_WIFI, payload);
});

// Calibration
document.getElementById('btn-calibrate')?.addEventListener('click', () => {
    const vFactor = parseFloat(document.getElementById('cal-voltage').value);
    const cFactor = parseFloat(document.getElementById('cal-current').value);
    log(`Applying Calibration: V=${vFactor}, I=${cFactor}`, 'info');
    sendFrame(CMD_CALIBRATE, new Uint8Array([0x01]));
});

// Clear Logs
document.getElementById('btn-clear-logs')?.addEventListener('click', () => {
    logsList.innerHTML = '<div class="log-item info"><span class="timestamp">--:--:--</span><span class="message">Logs cleared.</span></div>';
});

// ==================== Connection Mode Toggle ====================
function toggleConnMode() {
    if (isConnected) disconnect();

    if (currentMode === MODE_LOCAL) {
        currentMode = MODE_REMOTE;
        modeBtn.innerHTML = '<i class="fa-solid fa-cloud"></i> Remote IoT';
        modeBtn.classList.replace('secondary', 'primary');
        connectBtn.innerHTML = '<i class="fa-solid fa-link"></i> Connect Server';
        log('Switched to Remote IoT Mode', 'info');
    } else {
        currentMode = MODE_LOCAL;
        modeBtn.innerHTML = '<i class="fa-solid fa-network-wired"></i> Local Mode';
        modeBtn.classList.replace('primary', 'secondary');
        connectBtn.innerHTML = '<i class="fa-brands fa-bluetooth-b"></i> Connect Device';
        log('Switched to Local Bluetooth Mode', 'info');
    }
}

// ==================== Connection Handling ====================
async function handleConnection() {
    if (isConnected) {
        disconnect();
    } else {
        if (currentMode === MODE_LOCAL) {
            await connectLocal();
        } else {
            connectRemote();
        }
    }
}

// Local Mode (Web Serial)
async function connectLocal() {
    if (!('serial' in navigator)) {
        log('Web Serial API not supported. Please use Chrome or Edge browser.', 'error');
        return;
    }
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        const textEncoder = new TextEncoderStream();
        outputStream = textEncoder.writable;

        isConnected = true;
        updateStatus(true, 'Local (HC-05)');
        log('Connected to device via Bluetooth/Serial', 'success');
        startTime = Date.now();
        readLoop();
    } catch (err) {
        log('Connection failed: ' + err.message, 'error');
    }
}

// Remote Mode (Socket.IO)
function connectRemote() {
    try {
        const socketUrl = 'http://localhost:3000';
        log(`Connecting to Server: ${socketUrl}...`, 'info');
        socket = io(socketUrl);

        socket.on('connect', () => {
            isConnected = true;
            updateStatus(true, 'Remote (IoT)');
            log('Connected to IoT Server', 'success');
            startTime = Date.now();
        });

        socket.on('disconnect', () => {
            isConnected = false;
            updateStatus(false);
            log('Disconnected from Server', 'warning');
        });

        socket.on('data-packet', (dataArray) => {
            const buffer = new Uint8Array(dataArray);
            processBuffer(buffer);
        });

        socket.on('connect_error', (err) => {
            log('Socket Error: ' + err.message, 'error');
        });

    } catch (err) {
        log('Remote setup failed: ' + err.message, 'error');
    }
}

async function disconnect() {
    if (currentMode === MODE_LOCAL) {
        if (port) {
            if (reader) {
                await reader.cancel();
                reader = null;
            }
            await port.close();
            port = null;
        }
    } else {
        if (socket) {
            socket.disconnect();
            socket = null;
        }
    }
    isConnected = false;
    updateStatus(false);
    log('Disconnected', 'info');
}

async function readLoop() {
    while (port && port.readable && isConnected) {
        try {
            reader = port.readable.getReader();
            let buffer = new Uint8Array();
            while (true) {
                const { value, done } = await reader.read();
                if (done) break;
                buffer = processStreamingBuffer(buffer, value);
            }
        } catch (error) {
            log('Read error: ' + error, 'error');
        } finally {
            if (reader) reader.releaseLock();
        }
    }
}

function processStreamingBuffer(oldBuffer, newBytes) {
    const newBuffer = new Uint8Array(oldBuffer.length + newBytes.length);
    newBuffer.set(oldBuffer);
    newBuffer.set(newBytes, oldBuffer.length);
    return processBuffer(newBuffer);
}

function processBuffer(buffer) {
    while (buffer.length >= 3) {
        let headerIndex = -1;
        for (let i = 0; i < buffer.length; i++) {
            if (buffer[i] === FRAME_HEADER) {
                headerIndex = i;
                break;
            }
        }

        if (headerIndex === -1) return new Uint8Array();
        if (headerIndex > 0) buffer = buffer.slice(headerIndex);
        if (buffer.length < 3) return buffer;

        const len = buffer[1];
        const totalFrameSize = 3 + len;

        if (buffer.length >= totalFrameSize) {
            const cmd = buffer[2];
            const data = buffer.slice(3, 3 + len);
            handleCommand(cmd, data);
            buffer = buffer.slice(totalFrameSize);
        } else {
            return buffer;
        }
    }
    return buffer;
}

function handleCommand(cmd, data) {
    if (cmd === CMD_GET_RMS) {
        if (data.length >= 16) {
            const view = new DataView(data.buffer);
            const v = view.getFloat32(0, true);
            const i = view.getFloat32(4, true);
            const p = view.getFloat32(8, true);
            const e = view.getFloat32(12, true);
            updateMetrics(v, i, p, e);
        }
    } else {
        log(`Received command: 0x${cmd.toString(16)}`, 'info');
    }
}

async function sendFrame(cmd, data) {
    const len = data ? data.length : 0;
    const frame = new Uint8Array(3 + len);

    frame[0] = FRAME_HEADER;
    frame[1] = len;
    frame[2] = cmd;
    if (len > 0) frame.set(data, 3);

    if (currentMode === MODE_LOCAL) {
        if (!port || !port.writable) return;
        const writer = port.writable.getWriter();
        await writer.write(frame);
        writer.releaseLock();
    } else {
        if (socket && isConnected) {
            socket.emit('command', { cmd: cmd, frame: Array.from(frame) });
        }
    }
}

// ==================== Relay Control ====================
function handleRelayToggle(relayNum, state) {
    log(`Relay ${relayNum}: ${state ? 'ON' : 'OFF'}`, 'info');

    // Update UI
    const statusEl = document.getElementById(`relay-${relayNum}-status`);
    if (statusEl) {
        statusEl.textContent = state ? 'ON' : 'OFF';
        statusEl.classList.toggle('on', state);
        statusEl.classList.toggle('off', !state);
    }

    // Track runtime
    if (state) {
        stats.relayStartTimes[relayNum] = Date.now();
    } else {
        if (stats.relayStartTimes[relayNum]) {
            const runtime = (Date.now() - stats.relayStartTimes[relayNum]) / 1000;
            stats.relayRuntimes[relayNum] += runtime;
            stats.relayStartTimes[relayNum] = null;
        }
    }

    // Send command to device
    const payload = new Uint8Array([relayNum, state ? 1 : 0]);
    sendFrame(CMD_RELAY, payload);
}

// ==================== UI Updates ====================
function updateStatus(connected, modeText = '') {
    if (connected) {
        statusText.textContent = `Connected ${modeText}`;
        statusDot.classList.add('connected');
        connectBtn.innerHTML = '<i class="fa-solid fa-plug-circle-xmark"></i> Disconnect';
        connectBtn.classList.replace('primary', 'secondary');
    } else {
        statusText.textContent = 'Disconnected';
        statusDot.classList.remove('connected');
        connectBtn.innerHTML = currentMode === MODE_LOCAL ?
            '<i class="fa-brands fa-bluetooth-b"></i> Connect Device' :
            '<i class="fa-solid fa-link"></i> Connect Server';
        connectBtn.classList.replace('secondary', 'primary');
    }
}

function updateMetrics(v, i, p, e) {
    elVoltage.innerText = v.toFixed(1);
    elCurrent.innerText = i.toFixed(2);
    elPower.innerText = p.toFixed(1);
    elEnergy.innerText = e.toFixed(1);

    // Update statistics
    stats.totalPower += p;
    stats.sampleCount++;
    if (p > stats.peakPower) stats.peakPower = p;

    const avgPower = stats.sampleCount > 0 ? stats.totalPower / stats.sampleCount : 0;
    elAvgPower.textContent = avgPower.toFixed(1) + ' W';
    elPeakPower.textContent = stats.peakPower.toFixed(1) + ' W';

    // Update cost (assuming $0.15 per kWh)
    const cost = (e * 0.15).toFixed(2);
    elCost.textContent = '$' + cost;

    // Update uptime
    if (startTime) {
        const uptime = Math.floor((Date.now() - startTime) / 1000);
        const hours = Math.floor(uptime / 3600);
        const minutes = Math.floor((uptime % 3600) / 60);
        elUptime.textContent = `${hours}h ${minutes}m`;
    }

    // Update chart
    const now = new Date().toLocaleTimeString();
    if (mainChart.data.labels.length > 20) {
        mainChart.data.labels.shift();
        mainChart.data.datasets[0].data.shift();
    }
    mainChart.data.labels.push(now);
    mainChart.data.datasets[0].data.push(p);
    mainChart.update();
}

function log(msg, type = 'info') {
    const div = document.createElement('div');
    div.className = `log-item ${type}`;
    div.innerHTML = `
        <span class="timestamp">${new Date().toLocaleTimeString()}</span>
        <span class="message">${msg}</span>
    `;
    logsList.prepend(div);
}

// ==================== Demo Mode ====================
function toggleDemoMode() {
    isDemoMode = !isDemoMode;
    if (isDemoMode) {
        demoBtn.classList.add('active');
        demoBtn.innerHTML = '<i class="fa-solid fa-stop"></i> Stop Demo';
        log('Demo Mode Started', 'success');
        startTime = Date.now();

        let v = 220;
        let i = 0.5;
        let e = 1000;
        demoInterval = setInterval(() => {
            v += (Math.random() - 0.5) * 5;
            i += (Math.random() - 0.5) * 0.1;
            if (i < 0) i = 0;
            const p = v * i;
            e += p / 3600000; // Convert to kWh
            updateMetrics(v, i, p, e);
        }, 1000);

    } else {
        demoBtn.classList.remove('active');
        demoBtn.innerHTML = '<i class="fa-solid fa-play"></i> Demo Mode';
        clearInterval(demoInterval);
        log('Demo Mode Stopped', 'info');
    }
}

// ==================== Initialization ====================
log('Gestell Energy Dashboard initialized', 'success');
log('Click "Demo Mode" to test, or "Connect Device" for real data', 'info');
