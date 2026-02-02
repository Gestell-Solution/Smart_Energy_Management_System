import 'dart:async';
import 'package:flutter/foundation.dart';
import '../models/energy_data.dart';
import '../models/alert.dart';
import '../models/device.dart' as app_models;
import '../models/device_info.dart';
import '../services/bluetooth_service.dart';
import '../services/storage_service.dart';
import '../config/constants.dart';

class EnergyProvider with ChangeNotifier {
  final BluetoothService _bluetoothService = BluetoothService();
  final StorageService _storageService = StorageService();

  // State
  EnergyData? _currentData;
  DeviceInfo? _deviceInfo;
  List<EnergyData> _history = [];
  List<Alert> _alerts = [];
  List<app_models.BluetoothDevice> _availableDevices = [];
  app_models.BluetoothDevice? _connectedDevice;
  bool _isConnected = false;
  bool _isScanning = false;
  double _energyCostRate = AppConstants.defaultEnergyCostRate;

  // Subscriptions
  StreamSubscription? _dataSubscription;
  StreamSubscription? _deviceInfoSubscription;
  StreamSubscription? _connectionSubscription;
  StreamSubscription? _devicesSubscription;
  StreamSubscription? _scanErrorSubscription;
  StreamSubscription? _connectionErrorSubscription;
  Timer? _dataRequestTimer;
  
  // Scan error state
  String _scanError = '';

  // Connection error state
  String _connectionError = '';
  double _powerFactor = 1.0;

  // Getters
  EnergyData? get currentData => _currentData;
  DeviceInfo? get deviceInfo => _deviceInfo;
  List<EnergyData> get history => _history;
  List<Alert> get alerts => _alerts;
  List<app_models.BluetoothDevice> get availableDevices => _availableDevices;
  app_models.BluetoothDevice? get connectedDevice => _connectedDevice;
  bool get isConnected => _isConnected;
  bool get isScanning => _isScanning;
  double get energyCostRate => _energyCostRate;
  String get scanError => _scanError;
  String get connectionError => _connectionError;
  double get powerFactor => _powerFactor;

  // Computed values
  double get currentCost => _currentData?.calculateCost(_energyCostRate) ?? 0.0;
  int get unreadAlertsCount => _alerts.where((a) => !a.isRead).length;
  double get activePowerLimit {
    final pf = _powerFactor > 0 ? _powerFactor : 1.0;
    final apparentLimit =
        _deviceInfo?.maxPower ?? AppConstants.defaultOverpowerLimit;
    return apparentLimit * pf;
  }

  EnergyProvider() {
    _initialize();
  }

  Future<void> _initialize() async {
    // Initialize services
    await _storageService.initialize();
    await _bluetoothService.initialize();

    // Load saved data
    _energyCostRate = _storageService.getEnergyCostRate();
    _history = await _storageService.getEnergyHistory();
    _alerts = _storageService.getAlertHistory();

    // Load paired device
    _connectedDevice = _storageService.getPairedDevice();

    if (_connectedDevice != null) {
      try {
        final success =
            await _bluetoothService.connect(_connectedDevice!.address);
        if (success) {
          _connectedDevice =
              _connectedDevice!.copyWith(isConnected: true);
          _isConnected = true;
          await _bluetoothService.requestDeviceInfo();
        }
      } catch (e) {
        _connectionError = 'Auto-connect failed: $e';
      }
    }

    // Subscribe to streams
    _dataSubscription =
        _bluetoothService.dataStream.listen(_handleIncomingData);
    _deviceInfoSubscription =
        _bluetoothService.deviceInfoStream.listen(_handleDeviceInfo);
    _connectionSubscription =
        _bluetoothService.connectionState.listen(_handleConnectionChange);
    _devicesSubscription =
        _bluetoothService.devicesStream.listen(_handleDevicesUpdate);
    _scanErrorSubscription =
        _bluetoothService.scanErrorStream.listen(_handleScanError);
    _connectionErrorSubscription =
        _bluetoothService.connectionErrorStream.listen(_handleConnectionError);

    notifyListeners();
  }

  // Handle incoming energy data
  void _handleIncomingData(EnergyData data) {
    _currentData = data;

    // Track last power factor (clamped 0..1) based on incoming snapshot
    if (data.voltage > 0 && data.current > 0) {
      final pf = data.power / (data.voltage * data.current);
      _powerFactor = pf.clamp(0.0, 1.0);
    }

    for (var i = 0; i < _relayStates.length; i++) {
      _relayStates[i] = (data.relayStatesMask & (1 << i)) != 0;
    }

    // Save to history every 5 seconds
    if (_history.isEmpty ||
        DateTime.now().difference(_history.last.timestamp).inSeconds >= 5) {
      _history.add(data);

      // Keep only last 1000 records to avoid memory issues
      if (_history.length > 1000) {
        _history.removeAt(0);
      }

      unawaited(_storageService.saveEnergyData(data));
    }

    // Check for alerts
    _checkForAlerts(data);

    notifyListeners();
  }

  void _handleDeviceInfo(DeviceInfo info) {
    _deviceInfo = info;
    notifyListeners();
  }

  // Check for alert conditions
  void _checkForAlerts(EnergyData data) {
    final voltageLimit =
        _deviceInfo?.maxVoltage ?? AppConstants.defaultOvervoltageLimit;
    final currentLimit =
        _deviceInfo?.maxCurrent ?? AppConstants.defaultOvercurrentLimit;
    final powerLimit = activePowerLimit;

    // Check overload
    if (data.power > powerLimit) {
      _addAlert(Alert.overloadAlert(data.power));
    }

    // Check overcurrent
    if (data.current > currentLimit) {
      _addAlert(Alert.overcurrentAlert(data.current));
    }

    // Check overvoltage
    if (data.voltage > voltageLimit) {
      _addAlert(Alert.overvoltageAlert(data.voltage));
    }
  }

  // Add alert
  void _addAlert(Alert alert) {
    // Check if similar alert exists within last minute
    final recentAlerts = _alerts.where((a) =>
        a.type == alert.type &&
        DateTime.now().difference(a.timestamp).inMinutes < 1);

    if (recentAlerts.isEmpty) {
      _alerts.insert(0, alert);
      _storageService.saveAlert(alert);
      notifyListeners();
    }
  }

  // Handle connection state change
  void _handleConnectionChange(bool connected) {
    _isConnected = connected;
    print('[Provider] Connection state changed: connected=$connected');

    if (connected) {
      // Start requesting data periodically
      _startDataRequests();
      // Reset transient errors and cache
      _connectionError = '';
      _scanError = '';
      _connectedDevice ??= _bluetoothService.connectedDevice;
      _deviceInfo = null;
      // Refresh device limits if not yet received
      if (_deviceInfo == null) {
        _bluetoothService.requestDeviceInfo();
      }
    } else {
      // Stop data requests
      _stopDataRequests();
      _connectedDevice = null;
      _deviceInfo = null;
      _addAlert(Alert.connectionLost());
    }

    notifyListeners();
  }

  // Handle devices update
  void _handleDevicesUpdate(List<app_models.BluetoothDevice> devices) {
    _availableDevices = devices;
    notifyListeners();
  }

  // Handle scan errors
  void _handleScanError(String error) {
    _scanError = error;
    notifyListeners();
  }

  // Handle connection errors
  void _handleConnectionError(String error) {
    _connectionError = error;
    if (error.isNotEmpty) {
      _addAlert(Alert.systemError(error));
    }
    notifyListeners();
  }

  // Start periodic data requests
  void _startDataRequests() {
    _dataRequestTimer?.cancel();
  }

  // Stop periodic data requests
  void _stopDataRequests() {
    _dataRequestTimer?.cancel();
  }

  // Scan for devices
  Future<void> scanForDevices() async {
    _isScanning = true;
    notifyListeners();

    await _bluetoothService.startScan();

    _isScanning = false;
    notifyListeners();
  }

  // Connect to device
  Future<bool> connectToDevice(app_models.BluetoothDevice device) async {
    _connectionError = '';
    notifyListeners();

    // If a scan is ongoing, stop showing scanning state before connecting.
    if (_isScanning) {
      _isScanning = false;
      notifyListeners();
    }

    final success = await _bluetoothService.connect(device.address);

    if (success) {
      _connectedDevice = device.copyWith(isConnected: true);
      _isConnected = true;
      _deviceInfo = null;
      _scanError = '';
      notifyListeners();
      await _storageService.setPairedDevice(_connectedDevice!);
      // Fetch static device limits right after connection
      await _bluetoothService.requestDeviceInfo();
    }

    return success;
  }

  // Disconnect
  Future<void> disconnect() async {
    await _bluetoothService.disconnect();
    _connectedDevice = null;
    _isConnected = false;
    await _storageService.clearPairedDevice();
    notifyListeners();
  }

  // Reset energy counter
  Future<void> resetEnergyCounter() async {
    if (!_isConnected) {
      _connectionError = 'Not connected';
      notifyListeners();
      return;
    }

    final success = await _bluetoothService.resetEnergyCounter();
    if (!success) {
      _connectionError = 'Failed to reset energy counter';
      _addAlert(Alert.systemError(_connectionError));
      notifyListeners();
      return;
    }

    _connectionError = '';
    notifyListeners();
  }

  // Set energy cost rate
  Future<void> setEnergyCostRate(double rate) async {
    _energyCostRate = rate;
    await _storageService.setEnergyCostRate(rate);
    notifyListeners();
  }

  // Mark alert as read
  Future<void> markAlertAsRead(String alertId) async {
    final index = _alerts.indexWhere((a) => a.id == alertId);
    if (index != -1) {
      _alerts[index] = _alerts[index].copyWith(isRead: true);
      await _storageService.markAlertAsRead(alertId);
      notifyListeners();
    }
  }

  // Clear all alerts
  Future<void> clearAllAlerts() async {
    _alerts.clear();
    await _storageService.clearAlertHistory();
    notifyListeners();
  }

  Future<void> removeAlert(String alertId) async {
    final index = _alerts.indexWhere((a) => a.id == alertId);
    if (index == -1) return;
    _alerts.removeAt(index);
    await _storageService.removeAlert(alertId);
    notifyListeners();
  }

  Future<void> restoreAlert(Alert alert, {int index = 0}) async {
    final safeIndex = index.clamp(0, _alerts.length).toInt();
    _alerts.insert(safeIndex, alert);
    await _storageService.setAlertHistory(_alerts);
    notifyListeners();
  }

  // Get history for date range
  List<EnergyData> getHistoryByDateRange(DateTime start, DateTime end) {
    final filtered = _history.where((data) {
      return !data.timestamp.isBefore(start) && !data.timestamp.isAfter(end);
    }).toList();

    filtered.sort((a, b) => a.timestamp.compareTo(b.timestamp));
    return filtered;
  }

  // Get today's data
  List<EnergyData> getTodayData() {
    final now = DateTime.now();
    final start = DateTime(now.year, now.month, now.day);
    final end = start.add(const Duration(days: 1));
    return getHistoryByDateRange(start, end);
  }

  // Relay Control
  final List<bool> _relayStates = [false, false, false, false];
  List<bool> get relayStates => _relayStates;

  Future<void> toggleRelay(int index) async {
    if (index < 0 || index >= _relayStates.length) return;

    if (!_isConnected) {
      _connectionError = 'Not connected';
      notifyListeners();
      return;
    }

    final newState = !_relayStates[index];
    final success = await _bluetoothService.controlRelay(index, newState);
    if (!success) {
      _connectionError = 'Failed to control relay';
      _addAlert(Alert.systemError(_connectionError));
      notifyListeners();
      return;
    }

    _relayStates[index] = newState;
    _connectionError = '';
    notifyListeners();
  }

  @override
  void dispose() {
    _dataSubscription?.cancel();
    _deviceInfoSubscription?.cancel();
    _connectionSubscription?.cancel();
    _devicesSubscription?.cancel();
    _scanErrorSubscription?.cancel();
    _connectionErrorSubscription?.cancel();
    _dataRequestTimer?.cancel();
    _bluetoothService.dispose();
    super.dispose();
  }
}
