import 'dart:async';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import '../models/device.dart' as app_models;
import '../models/energy_data.dart';
import '../config/constants.dart';

class BluetoothService {
  static final BluetoothService _instance = BluetoothService._internal();
  factory BluetoothService() => _instance;
  BluetoothService._internal();

  // Stream controllers
  final _connectionStateController = StreamController<bool>.broadcast();
  final _dataController = StreamController<EnergyData>.broadcast();
  final _devicesController =
      StreamController<List<app_models.BluetoothDevice>>.broadcast();

  // State
  BluetoothDevice? _connectedDevice;
  BluetoothCharacteristic? _dataCharacteristic;
  bool _isScanning = false;
  bool _isConnected = false;

  // Frame buffer for protocol
  final List<int> _frameBuffer = [];

  // Getters
  Stream<bool> get connectionState => _connectionStateController.stream;
  Stream<EnergyData> get dataStream => _dataController.stream;
  Stream<List<app_models.BluetoothDevice>> get devicesStream =>
      _devicesController.stream;
  bool get isConnected => _isConnected;
  bool get isScanning => _isScanning;

  // Initialize Bluetooth
  Future<void> initialize() async {
    try {
      if (await FlutterBluePlus.isSupported == false) {
        throw Exception('Bluetooth not supported on this device');
      }

      FlutterBluePlus.adapterState.listen((state) {
        if (state != BluetoothAdapterState.on) {
          _isConnected = false;
          _connectionStateController.add(false);
        }
      });
    } catch (e) {
      print('Bluetooth initialization error: $e');
    }
  }

  // Scan for devices
  Future<void> startScan({
    Duration timeout = const Duration(seconds: 10),
  }) async {
    if (_isScanning) return;

    try {
      _isScanning = true;
      final devices = <app_models.BluetoothDevice>[];

      await FlutterBluePlus.startScan(timeout: timeout);

      FlutterBluePlus.scanResults.listen((results) {
        devices.clear();
        for (var result in results) {
          if (result.device.platformName.isNotEmpty) {
            devices.add(
              app_models.BluetoothDevice(
                name: result.device.platformName,
                address: result.device.remoteId.toString(),
                rssi: result.rssi,
                isConnected: false,
              ),
            );
          }
        }
        _devicesController.add(devices);
      });

      await Future.delayed(timeout);
      await stopScan();
    } catch (e) {
      print('Scan error: $e');
      _isScanning = false;
    }
  }

  // Stop scanning
  Future<void> stopScan() async {
    try {
      await FlutterBluePlus.stopScan();
      _isScanning = false;
    } catch (e) {
      print('Stop scan error: $e');
    }
  }

  // Connect to device
  Future<bool> connect(String deviceAddress) async {
    try {
      final scanResults = await FlutterBluePlus.lastScanResults;
      final deviceResult = scanResults.firstWhere(
        (result) => result.device.remoteId.toString() == deviceAddress,
        orElse: () => throw Exception('Device not found'),
      );

      final device = deviceResult.device;

      await device.connect(timeout: const Duration(seconds: 15));
      _connectedDevice = device;

      final services = await device.discoverServices();

      // Find UART service characteristic for data
      for (var service in services) {
        for (var characteristic in service.characteristics) {
          if (characteristic.properties.notify ||
              characteristic.properties.read) {
            _dataCharacteristic = characteristic;

            if (characteristic.properties.notify) {
              await characteristic.setNotifyValue(true);

              characteristic.lastValueStream.listen((value) {
                _handleIncomingData(value);
              });
            }
            break;
          }
        }
        if (_dataCharacteristic != null) break;
      }

      _isConnected = true;
      _connectionStateController.add(true);

      // Start periodic data requests
      _startPeriodicDataRequest();

      return true;
    } catch (e) {
      print('Connection error: $e');
      _isConnected = false;
      _connectionStateController.add(false);
      return false;
    }
  }

  // Disconnect from device
  Future<void> disconnect() async {
    try {
      _stopPeriodicDataRequest();
      if (_connectedDevice != null) {
        await _connectedDevice!.disconnect();
        _connectedDevice = null;
        _dataCharacteristic = null;
        _isConnected = false;
        _connectionStateController.add(false);
      }
    } catch (e) {
      print('Disconnect error: $e');
    }
  }

  // Send framed command to device
  Future<void> sendCommand(int commandId, [List<int>? data]) async {
    try {
      if (_dataCharacteristic != null && _isConnected) {
        final frame = _buildFrame(commandId, data ?? []);
        await _dataCharacteristic!.write(frame, withoutResponse: false);
      }
    } catch (e) {
      print('Send command error: $e');
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
  void _handleIncomingData(List<int> value) {
    try {
      _frameBuffer.addAll(value);
      _processFrameBuffer();
    } catch (e) {
      print('Data handling error: $e');
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
        // Assuming data format: [V_high, V_low, I_high, I_low, P_high, P_low, E_high, E_low, E_mid_high, E_mid_low]

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
    } catch (e) {
      print('Command processing error: $e');
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

  // Reset energy counter
  Future<void> resetEnergyCounter() async {
    // Send write EEPROM command to reset energy counter
    // This would need the proper data format for your EEPROM structure
    await sendCommand(AppConstants.cmdWriteEeprom, [0x00, 0x00, 0x00, 0x00]);
  }

  // Control Relay
  Future<void> controlRelay(int relayIndex, bool state) async {
    // Command: [RelayIndex, State]
    // State: 1 for ON, 0 for OFF
    await sendCommand(
        AppConstants.cmdControlRelay, [relayIndex, state ? 1 : 0]);
  }

  // Dispose
  void dispose() {
    _stopPeriodicDataRequest();
    _connectionStateController.close();
    _dataController.close();
    _devicesController.close();
    disconnect();
  }
}
