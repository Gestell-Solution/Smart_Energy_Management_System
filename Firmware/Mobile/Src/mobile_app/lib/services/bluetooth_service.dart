import 'dart:async';
import 'dart:typed_data';
import 'dart:io' show Platform;
import 'package:flutter/foundation.dart' show kIsWeb;
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:location/location.dart';
import '../models/device.dart' as app_models;
import '../models/energy_data.dart';
import '../models/device_info.dart';
import '../config/constants.dart';

/// Bluetooth service for managing device connections and data communication.
///
/// Uses Bluetooth Classic (flutter_bluetooth_serial) to support:
/// - HC-05/HC-06 Bluetooth modules
/// - Traditional Bluetooth devices (laptops, PCs, smartphones)
/// - Any Bluetooth Classic SPP (Serial Port Profile) device
///
/// This service is designed for universal compatibility to enable
/// testing with various Bluetooth devices including Proteus simulation.
class BluetoothService {
  static final BluetoothService _instance = BluetoothService._internal();
  factory BluetoothService() => _instance;
  BluetoothService._internal();

  // Bluetooth instance
  final FlutterBluetoothSerial _bluetooth = FlutterBluetoothSerial.instance;

  // Stream controllers
  final _connectionStateController = StreamController<bool>.broadcast();
  final _dataController = StreamController<EnergyData>.broadcast();
  final _deviceInfoController = StreamController<DeviceInfo>.broadcast();
  final _devicesController =
      StreamController<List<app_models.BluetoothDevice>>.broadcast();
  final _scanErrorController = StreamController<String>.broadcast();
  final _connectionErrorController = StreamController<String>.broadcast();

  // State
  BluetoothConnection? _connection;
  bool _isScanning = false;
  bool _isConnected = false;
  app_models.BluetoothDevice? _connectedDevice;

  DateTime? _lastDataReceivedAt;
  int _receivedBytesCount = 0;

  // Frame buffer for protocol
  final List<int> _frameBuffer = [];

  // Getters
  Stream<bool> get connectionState => Stream<bool>.multi((controller) {
        controller.add(_isConnected);
        final sub = _connectionStateController.stream.listen(
          controller.add,
          onError: controller.addError,
          onDone: controller.close,
        );
        controller.onCancel = () => sub.cancel();
      });
  Stream<EnergyData> get dataStream => _dataController.stream;
  Stream<DeviceInfo> get deviceInfoStream => _deviceInfoController.stream;
  Stream<List<app_models.BluetoothDevice>> get devicesStream =>
      _devicesController.stream;
  Stream<String> get scanErrorStream => _scanErrorController.stream;
  Stream<String> get connectionErrorStream => _connectionErrorController.stream;
  bool get isConnected => _isConnected;
  bool get isScanning => _isScanning;
  app_models.BluetoothDevice? get connectedDevice => _connectedDevice;

  // Initialize Bluetooth
  Future<void> initialize() async {
    try {
      // Check if running on web
      if (kIsWeb) {
        print('[BT] Web platform detected - Bluetooth not supported');
        return;
      }

      // Request permissions based on Android version
      if (Platform.isAndroid) {
        await _requestBluetoothPermissions();
      }

      // Check if Bluetooth is available
      bool? isAvailable = await _bluetooth.isAvailable;
      if (isAvailable == null || !isAvailable) {
        // throw Exception('Bluetooth not available on this device');
        // Don't throw to allow app usage without BT
        print('[BT] Bluetooth not available');
        return;
      }

      // Request enable if needed
      bool? isEnabled = await _bluetooth.isEnabled;
      if (isEnabled == null || !isEnabled) {
        await _bluetooth.requestEnable();
      }

      // Listen to state changes
      _bluetooth.onStateChanged().listen((state) {
        print('[BT] Adapter state changed: $state');

        if (state == BluetoothState.STATE_OFF ||
            state == BluetoothState.STATE_TURNING_OFF) {
          if (_isConnected) {
            _isConnected = false;
            _connectedDevice = null;
            _connectionStateController.add(false);
          }
        }
      });
    } catch (e) {
      print('[BT] Bluetooth initialization error: $e');
    }
  }

  // Check if Bluetooth is supported on current platform
  bool get isBluetoothSupported => !kIsWeb;

  // Get platform info
  String get platformInfo {
    if (kIsWeb) return 'Web (Bluetooth not supported)';
    if (Platform.isAndroid) return 'Android';
    return 'Unknown';
  }

  // Request Bluetooth permissions based on Android version
  Future<bool> _requestBluetoothPermissions() async {
    try {
      print('[BT] Requesting Bluetooth permissions...');
      
      // Request permissions based on Android version
      final statuses = await [
        Permission.bluetoothScan,
        Permission.bluetoothConnect,
        Permission.bluetooth,
        Permission.location,
      ].request();

      final scanGranted = statuses[Permission.bluetoothScan]?.isGranted ?? false;
      final connectGranted = statuses[Permission.bluetoothConnect]?.isGranted ?? false;
      final bluetoothGranted = statuses[Permission.bluetooth]?.isGranted ?? false;
      final locationGranted = statuses[Permission.location]?.isGranted ?? false;

      print('[BT] Permission results:');
      print('[BT]  - Bluetooth Scan: ${scanGranted ? "GRANTED" : "DENIED"}');
      print('[BT]  - Bluetooth Connect: ${connectGranted ? "GRANTED" : "DENIED"}');
      print('[BT]  - Bluetooth (Legacy): ${bluetoothGranted ? "GRANTED" : "DENIED"}');
      print('[BT]  - Location: ${locationGranted ? "GRANTED" : "DENIED"}');

      // Required permissions depend on Android version and OEM behavior.
      // Prefer modern permissions when available, but allow legacy/location combos as fallback.
      final hasModernPermissions = scanGranted && connectGranted;
      final hasLegacyPermissions = bluetoothGranted && locationGranted;
      final hasAnyLikelyWorkingPermission = hasModernPermissions || hasLegacyPermissions;

      if (!hasAnyLikelyWorkingPermission) {
        _scanErrorController.add(
            'Bluetooth permissions denied. Please enable Nearby Devices/Bluetooth permissions (and Location if prompted).');
        return false;
      }

      return true;
    } catch (e) {
      print('[BT] Error requesting permissions: $e');
      _scanErrorController.add('Failed to request permissions: $e');
      return false;
    }
  }

  // Check if location services are enabled
  Future<bool> _checkLocationServices() async {
    try {
      if (!Platform.isAndroid) return true; // Location not required on other platforms
      
      Location location = Location();
      bool serviceEnabled = await location.serviceEnabled();
      
      if (!serviceEnabled) {
        print('[BT] Location services disabled');
        _scanErrorController.add('Location services are required for Bluetooth scanning. Please enable location services.');
        
        // Try to request enabling location services
        bool serviceRequested = await location.requestService();
        if (!serviceRequested) {
          _scanErrorController.add('Location services must be enabled to scan for Bluetooth devices.');
          return false;
        }
      }
      
      print('[BT] Location services enabled');
      return true;
    } catch (e) {
      print('[BT] Error checking location services: $e');
      _scanErrorController.add('Failed to check location services: $e');
      return false;
    }
  }

  // Scan for devices - Universal compatibility with enhanced error handling
  Future<void> startScan({
    Duration timeout = const Duration(seconds: 15),
  }) async {
    if (_isScanning) {
      print('[BT] Scan already in progress');
      return;
    }

    _isScanning = true;
    final devices = <app_models.BluetoothDevice>[];
    _scanErrorController.add(''); // Clear previous errors

    try {

      // Handle web platform
      if (kIsWeb) {
        print('[BT] Web platform - Simulating device scan for demo');
        // Add demo devices for web testing
        devices.add(
          app_models.BluetoothDevice(
            name: 'Demo Energy Monitor',
            address: '00:00:00:00:00:01',
            rssi: -60,
            isConnected: false,
          ),
        );
        _devicesController.add(devices);
        _isScanning = false;
        return;
      }

      print('[BT] Starting enhanced device scan...');
      
      // Check permissions first
      final hasPermissions = await _requestBluetoothPermissions();
      if (!hasPermissions) {
        return;
      }

      final scanPermissionStatus = await Permission.bluetoothScan.status;
      final shouldRequireLocationServices =
          Platform.isAndroid && !scanPermissionStatus.isGranted;

      if (shouldRequireLocationServices) {
        final locationEnabled = await _checkLocationServices();
        if (!locationEnabled) {
          return;
        }
      }

      // Check if Bluetooth is available and enabled
      bool? isAvailable = await _bluetooth.isAvailable;
      if (isAvailable == null || !isAvailable) {
        _scanErrorController.add('Bluetooth not available on this device');
        return;
      }

      bool? isEnabled = await _bluetooth.isEnabled;
      if (isEnabled == null || !isEnabled) {
        print('[BT] Bluetooth not enabled, requesting enable...');
        bool? enabled = await _bluetooth.requestEnable();
        if (enabled == null || !enabled) {
          _scanErrorController.add('Bluetooth must be enabled to scan for devices');
          return;
        }
      }

      print('[BT] Starting device scan (Universal Mode - All Devices)');

      try {
        await _bluetooth.cancelDiscovery();
      } catch (e) {
        print('[BT] Cancel previous discovery failed: $e');
      }

      // Get bonded devices first
      try {
        List<BluetoothDevice> bondedDevices = await _bluetooth.getBondedDevices();
        print('[BT] Found ${bondedDevices.length} bonded devices');

        for (var device in bondedDevices) {
          final deviceName = device.name ?? 'Unknown Device';
          print('[BT] Bonded: $deviceName [${device.address}]');

          devices.add(
            app_models.BluetoothDevice(
              name: deviceName,
              address: device.address,
              rssi: 0,
              isConnected: device.isConnected,
            ),
          );
        }

        // Emit bonded devices immediately
        _devicesController.add(devices);
        print('[BT] Emitted ${devices.length} bonded devices');
      } catch (e) {
        print('[BT] Error getting bonded devices: $e');
        _scanErrorController.add('Error getting paired devices: $e');
      }

      // Start discovery for nearby devices
      print('[BT] Starting discovery for nearby devices...');
      int discoveryCount = 0;
      late final StreamSubscription discoverySubscription;
      
      try {
        discoverySubscription = _bluetooth.startDiscovery().listen(
          (result) {
            discoveryCount++;
            print('[BT] Discovery result received: ${result.device.name} [${result.device.address}]');
            
            // Check if device already in list
            bool exists = devices.any((d) => d.address == result.device.address);

            if (!exists) {
              final deviceName = result.device.name ??
                  'BT Device (${result.device.address.substring(0, 8)}...)';

              print(
                  '[BT] Discovered: $deviceName [${result.device.address}] RSSI: ${result.rssi}');

              devices.add(
                app_models.BluetoothDevice(
                  name: deviceName,
                  address: result.device.address,
                  rssi: result.rssi,
                  isConnected: result.device.isConnected,
                ),
              );
              _devicesController.add(List.from(devices));
              print('[BT] Added device to list. Total: ${devices.length}');
            }
          },
          onError: (error) {
            print('[BT] Discovery error: $error');
            _scanErrorController.add('Discovery failed: $error');
          },
          onDone: () {
            print('[BT] Discovery completed. Found $discoveryCount discovery results');
          },
        );

        // Wait for discovery to complete with timeout
        print('[BT] Discovery running, will timeout in ${timeout.inSeconds} seconds');
        await Future.delayed(timeout);
        
        // Cancel discovery if still running
        await discoverySubscription.cancel();
        print('[BT] Discovery subscription cancelled');

        try {
          await _bluetooth.cancelDiscovery();
          print('[BT] Discovery cancelled');
        } catch (e) {
          print('[BT] cancelDiscovery() failed: $e');
        }
        
      } catch (e) {
        print('[BT] Error starting discovery: $e');
        _scanErrorController.add('Failed to start device discovery: $e');
      }
      
      print('[BT] Scan completed. Total devices found: ${devices.length}');
      if (devices.isEmpty) {
        print('[BT] No devices found with main scan, trying alternative method...');
        final alternativeDevices = await _startAlternativeScan();
        if (alternativeDevices.isNotEmpty) {
          devices.addAll(alternativeDevices);
          _devicesController.add(devices);
          print('[BT] Alternative scan found ${alternativeDevices.length} devices');
        }
        
        if (devices.isEmpty) {
          final scanStatus = await Permission.bluetoothScan.status;
          final connectStatus = await Permission.bluetoothConnect.status;
          final locationPermissionStatus = await Permission.location.status;
          bool locationServiceEnabled = false;

          try {
            if (Platform.isAndroid) {
              locationServiceEnabled = await Location().serviceEnabled();
            } else {
              locationServiceEnabled = true;
            }
          } catch (_) {
            locationServiceEnabled = false;
          }

          final isAvailable = await _bluetooth.isAvailable;
          final isEnabled = await _bluetooth.isEnabled;

          _scanErrorController.add(
              'No devices found.\n\nStatus:\n- Bluetooth available: ${isAvailable == true}\n- Bluetooth enabled: ${isEnabled == true}\n- Permission bluetoothScan: ${scanStatus.isGranted}\n- Permission bluetoothConnect: ${connectStatus.isGranted}\n- Permission location: ${locationPermissionStatus.isGranted}\n- Location service enabled: $locationServiceEnabled\n\nNotes:\n- For laptops/phones: enable Discoverable/Visible while scanning\n- For HC-05: ensure pairing is done first (PIN 1234/0000) and device is powered');
        } else {
          _scanErrorController.add(''); // Clear any errors if devices found
        }
      } else {
        _scanErrorController.add(''); // Clear any errors if devices found
      }
    } catch (e) {
      print('[BT] Scan error: $e');
      _scanErrorController.add('Scan failed: $e');
    } finally {
      _isScanning = false;
    }
  }

  // Alternative scan method for better compatibility
  Future<List<app_models.BluetoothDevice>> _startAlternativeScan() async {
    try {
      print('[BT] Starting alternative scan method...');
      
      // Try to get all available devices using different approaches
      final devices = <app_models.BluetoothDevice>[];
      
      // Method 1: Get bonded devices
      try {
        List<BluetoothDevice> bondedDevices = await _bluetooth.getBondedDevices();
        for (var device in bondedDevices) {
          devices.add(
            app_models.BluetoothDevice(
              name: device.name ?? 'Unknown Device',
              address: device.address,
              rssi: 0,
              isConnected: device.isConnected,
            ),
          );
        }
        print('[BT] Alternative scan found ${bondedDevices.length} bonded devices');
      } catch (e) {
        print('[BT] Error getting bonded devices in alternative scan: $e');
      }
      
      // Method 2: Try discovery with shorter timeout
      try {
        await for (var result in _bluetooth.startDiscovery()) {
          bool exists = devices.any((d) => d.address == result.device.address);
          if (!exists) {
            devices.add(
              app_models.BluetoothDevice(
                name: result.device.name ?? 'BT Device',
                address: result.device.address,
                rssi: result.rssi,
                isConnected: result.device.isConnected,
              ),
            );
            print('[BT] Alternative scan discovered: ${result.device.name}');
          }
        }
      } catch (e) {
        print('[BT] Alternative discovery error: $e');
      }
      
      print('[BT] Alternative scan completed. Total devices: ${devices.length}');
      return devices;
      
    } catch (e) {
      print('[BT] Alternative scan failed: $e');
      _scanErrorController.add('Alternative scan failed: $e');
      return [];
    }
  }
  Future<void> stopScan() async {
    try {
      await _bluetooth.cancelDiscovery();
      _isScanning = false;
    } catch (e) {
      print('[BT] Stop scan error: $e');
    }
  }

  // Connect to device
  Future<bool> connect(String deviceAddress) async {
    try {
      print('[BT] Attempting to connect to: $deviceAddress');

      _connectionErrorController.add('');

      if (kIsWeb) {
        _connectionErrorController.add('Bluetooth not supported on web');
        return false;
      }

      if (Platform.isAndroid) {
        final statuses = await [
          Permission.bluetoothConnect,
          Permission.bluetooth,
        ].request();

        final connectGranted =
            statuses[Permission.bluetoothConnect]?.isGranted ?? false;
        final bluetoothGranted =
            statuses[Permission.bluetooth]?.isGranted ?? false;

        if (!connectGranted && !bluetoothGranted) {
          _connectionErrorController.add(
              'Bluetooth connect permission denied. Please allow Nearby devices/Bluetooth permission.');
          return false;
        }
      }

      try {
        await _bluetooth.cancelDiscovery();
      } catch (e) {
        print('[BT] cancelDiscovery() before connect failed: $e');
      }

      await Future.delayed(const Duration(milliseconds: 350));

      // Disconnect if already connected
      if (_connection != null) {
        print('[BT] Disconnecting from current device...');
        await disconnect();
        await Future.delayed(const Duration(milliseconds: 500));
      }

      bool isBonded = false;
      try {
        final bondedDevices = await _bluetooth.getBondedDevices();
        isBonded = bondedDevices.any((d) => d.address == deviceAddress);
      } catch (e) {
        print('[BT] getBondedDevices() before connect failed: $e');
      }

      if (!isBonded) {
        print('[BT] Device not bonded. Attempting to pair...');
        try {
          final bonded = await _bluetooth.bondDeviceAtAddress(deviceAddress);
          if (bonded != true) {
            print('[BT] bondDeviceAtAddress returned false. Will still try SPP connect.');
          }
        } catch (e) {
          // Some devices (especially laptops/PCs) can already appear "connected" or paired at the OS level,
          // yet bondDeviceAtAddress may fail. Bonding is not always required to attempt an RFCOMM/SPP socket.
          print('[BT] Pairing attempt failed (non-fatal): $e');
        }

        await Future.delayed(const Duration(milliseconds: 900));

        final bondedConfirmed = await _waitUntilBonded(deviceAddress);
        if (!bondedConfirmed) {
          // Non-fatal: still attempt SPP connect.
          print('[BT] Device still not listed as bonded. Will still try SPP connect.');
        }
      }

      await Future.delayed(const Duration(milliseconds: 250));

      BluetoothConnection? connection;
      Object? lastConnectError;

      for (int attempt = 1; attempt <= 3; attempt++) {
        try {
          print('[BT] SPP connect attempt $attempt/3 to $deviceAddress');
          connection = await BluetoothConnection.toAddress(deviceAddress)
              .timeout(const Duration(seconds: 20));
          if (connection.isConnected) {
            break;
          }
        } catch (e) {
          lastConnectError = e;
          print('[BT] SPP connect attempt $attempt failed: $e');
          await Future.delayed(Duration(milliseconds: 500 * attempt));
        }
      }

      _connection = connection;

      if (_connection == null || !_connection!.isConnected) {
        throw Exception(lastConnectError ?? 'Failed to establish connection');
      }

      _isConnected = true;
      _lastDataReceivedAt = null;
      _receivedBytesCount = 0;
      try {
        final bondedDevices = await _bluetooth.getBondedDevices();
        final matched = bondedDevices.where((d) => d.address == deviceAddress);
        final btDevice = matched.isNotEmpty ? matched.first : null;
        _connectedDevice = app_models.BluetoothDevice(
          name: btDevice?.name ?? 'Unknown Device',
          address: deviceAddress,
          rssi: 0,
          isConnected: true,
        );
      } catch (e) {
        _connectedDevice = app_models.BluetoothDevice(
          name: 'Unknown Device',
          address: deviceAddress,
          rssi: 0,
          isConnected: true,
        );
        print('[BT] getBondedDevices() after connect failed: $e');
      }
      _connectionStateController.add(true);
      print('[BT] ✓ Connected successfully to $deviceAddress');
      print('[BT] Connection object state: isConnected=${_connection?.isConnected}');
      print('[BT] Internal _isConnected flag: $_isConnected');

      // Listen to incoming data
      _connection!.input!.listen(
        (Uint8List data) {
          print('[BT] Received ${data.length} bytes');
          _receivedBytesCount += data.length;
          _lastDataReceivedAt = DateTime.now();
          _handleIncomingData(data);
        },
        onDone: () {
          print('[BT] Connection closed');
          final hadAnyData = _receivedBytesCount > 0;
          if (!hadAnyData) {
            _connectionErrorController.add(
                'Connection closed immediately by remote device. Ensure the device supports Bluetooth Classic SPP (HC-05/HC-06) and no other app is connected.');
          }
          _stopPeriodicDataRequest();
          _isConnected = false;
          _connectedDevice = null;
          _connectionStateController.add(false);
          print('[BT] onDone: set _isConnected=false and emitted false');
          _connection = null; // Important: Clear connection object
        },
        onError: (error) {
          print('[BT] Connection error: $error');
          _connectionErrorController.add('Connection error: $error');
          _stopPeriodicDataRequest();
          disconnect();
        },
      );

      // Start periodic data requests
      _startPeriodicDataRequest();
      print('[BT] Periodic data requests started');

      return true;
    } catch (e) {
      print('[BT] ✗ Connection failed: $e');
      final msg = e.toString();
      final isSocketConnectError =
          msg.contains('connect_error') || msg.contains('BluetoothSocket');
      if (isSocketConnectError) {
        _connectionErrorController.add(
            'Connection failed: could not open Bluetooth Classic SPP (Serial Port) socket.\n\nIf you are connecting to a Laptop/PC:\n- Linux/Windows must expose an SPP/RFCOMM Serial Port service (e.g. Incoming COM port on Windows, rfcomm/sdptool on Linux).\n- "Connected" in Bluetooth settings may be audio (e.g. aptX) or generic pairing, not Serial/SPP.\n\nIf you are connecting to HC-05/HC-06:\n- Ensure the module is Classic SPP (not BLE)\n- Disconnect it from any other phone/app (SPP usually allows 1 connection)\n- Forget device then pair again (PIN 1234/0000)\n- Power-cycle the module and try again');
      } else {
        _connectionErrorController.add('Connection failed: $e');
      }
      _stopPeriodicDataRequest();
      _isConnected = false;
      _connectedDevice = null;
      _connectionStateController.add(false);
      return false;
    }
  }

  Future<bool> _waitUntilBonded(
    String deviceAddress, {
    Duration timeout = const Duration(seconds: 10),
    Duration pollInterval = const Duration(milliseconds: 300),
  }) async {
    final endAt = DateTime.now().add(timeout);
    while (DateTime.now().isBefore(endAt)) {
      try {
        final bondedDevices = await _bluetooth.getBondedDevices();
        final isBonded = bondedDevices.any((d) => d.address == deviceAddress);
        if (isBonded) return true;
      } catch (e) {
        print('[BT] _waitUntilBonded getBondedDevices() failed: $e');
      }
      await Future.delayed(pollInterval);
    }
    return false;
  }

  // Disconnect from device
  Future<void> disconnect() async {
    try {
      _stopPeriodicDataRequest();

      if (_connection != null) {
        await _connection!
            .close(); // Changed from disconnect() to close() for BluetoothConnection
        _connection = null;
        _connectedDevice = null;
        _isConnected = false;
        _connectionStateController.add(false);
        print('[BT] Disconnected successfully');
      }
    } catch (e) {
      print('[BT] Disconnect error: $e');
    }
  }

  // Send framed command to device
  Future<bool> sendCommand(int commandId, [List<int>? data]) async {
    try {
      if (_connection == null || !_isConnected || !_connection!.isConnected) {
        _connectionErrorController.add('Not connected');
        return false;
      }

      final frame = _buildFrame(commandId, data ?? []);
      print(
          '[BT] Sending command 0x${commandId.toRadixString(16).padLeft(2, '0')}: ${frame.map((b) => '0x${b.toRadixString(16).padLeft(2, '0')}').join(' ')}');
      _connection!.output.add(Uint8List.fromList(frame));
      await _connection!.output.allSent;
      return true;
    } catch (e) {
      print('[BT] Send command error: $e');
      _connectionErrorController.add('Send command error: $e');
      return false;
    }
  }

  // Build frame: [Header][Length][Command][Data...]
  List<int> _buildFrame(int commandId, List<int> data) {
    final frame = <int>[];
    frame.add(AppConstants.frameHeader); // 0xAA
    frame.add(data.length); // Length
    frame.add(commandId); // Command ID
    frame.addAll(data); // Data payload
    return frame;
  }

  // Handle incoming data with framed protocol
  void _handleIncomingData(Uint8List value) {
    try {
      _frameBuffer.addAll(value);
      _processFrameBuffer();
    } catch (e) {
      print('[BT] Data handling error: $e');
    }
  }

  // Process accumulated frame buffer
  void _processFrameBuffer() {
    while (_frameBuffer.length >= 3) {
      // Look for frame header
      final headerIndex = _frameBuffer.indexOf(AppConstants.frameHeader);

      if (headerIndex == -1) {
        _frameBuffer.clear();
        return;
      }

      // Remove data before header
      if (headerIndex > 0) {
        _frameBuffer.removeRange(0, headerIndex);
      }

      // Check if we have enough data for length and command
      if (_frameBuffer.length < 3) return;

      final length = _frameBuffer[1];
      final commandId = _frameBuffer[2];

      // Check if we have complete frame
      final totalFrameLength = 3 + length;
      if (_frameBuffer.length < totalFrameLength) return;

      // Extract data
      final frameData = _frameBuffer.sublist(3, totalFrameLength);

      // Remove processed frame from buffer
      _frameBuffer.removeRange(0, totalFrameLength);

      // Process command
      _processCommand(commandId, frameData);
    }
  }

  // Process received command/response
  void _processCommand(int commandId, List<int> data) {
    try {
      if (commandId == AppConstants.cmdGetRmsData && data.length >= 10) {
        // Parse RMS data from frame
        final voltage = ((data[0] << 8) | data[1]) / 10.0;
        final current = ((data[2] << 8) | data[3]) / 100.0;
        final power = ((data[4] << 8) | data[5]) / 10.0;

        // Energy is 32-bit
        final energy =
            ((data[6] << 24) | (data[7] << 16) | (data[8] << 8) | data[9]) /
                100.0;

        final energyData = EnergyData(
          voltage: voltage,
          current: current,
          power: power,
          energy: energy,
          status: _getStatus(voltage, current, power),
        );

        _dataController.add(energyData);
      }

      if (commandId == AppConstants.cmdGetDeviceInfo) {
        final info = DeviceInfo.fromPayload(data);
        _deviceInfoController.add(info);
      }
    } catch (e) {
      print('[BT] Command processing error: $e');
    }
  }

  // Determine status based on values
  String _getStatus(double voltage, double current, double power) {
    if (current > AppConstants.defaultOvercurrentLimit) {
      return 'OVERLOAD';
    }
    if (voltage > AppConstants.defaultOvervoltageLimit) {
      return 'OVERVOLT';
    }
    if (power > AppConstants.defaultOverpowerLimit) {
      return 'HIGHPOWER';
    }
    return 'OK';
  }

  // Periodic data request timer
  Timer? _dataRequestTimer;

  void _startPeriodicDataRequest() {
    _dataRequestTimer?.cancel();
    _dataRequestTimer = Timer.periodic(AppConstants.dataUpdateInterval, (_) {
      requestDataUpdate();
    });
  }

  void _stopPeriodicDataRequest() {
    _dataRequestTimer?.cancel();
  }

  // Request data update
  Future<void> requestDataUpdate() async {
    await sendCommand(AppConstants.cmdGetRmsData);
  }

  Future<bool> requestDeviceInfo() async {
    return await sendCommand(AppConstants.cmdGetDeviceInfo);
  }

  // Reset energy counter
  Future<bool> resetEnergyCounter() async {
    return await sendCommand(
        AppConstants.cmdWriteEeprom, [0x00, 0x00, 0x00, 0x00]);
  }

  // Control Relay
  Future<bool> controlRelay(int relayIndex, bool state) async {
    return await sendCommand(
        AppConstants.cmdControlRelay, [relayIndex, state ? 1 : 0]);
  }

  // Dispose
  void dispose() {
    stopScan();
    disconnect();
    _connection?.close();
    _connectionStateController.close();
    _dataController.close();
    _deviceInfoController.close();
    _devicesController.close();
    _scanErrorController.close();
    _connectionErrorController.close();
  }
}
