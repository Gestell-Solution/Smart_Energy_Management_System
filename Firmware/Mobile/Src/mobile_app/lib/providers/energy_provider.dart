import 'dart:async';
import 'package:flutter/foundation.dart';
import '../models/energy_data.dart';
import '../models/alert.dart';
import '../models/device.dart' as app_models;
import '../services/bluetooth_service.dart';
import '../services/storage_service.dart';
import '../config/constants.dart';

class EnergyProvider with ChangeNotifier {
  final BluetoothService _bluetoothService = BluetoothService();
  final StorageService _storageService = StorageService();

  // State
  EnergyData? _currentData;
  List<EnergyData> _history = [];
  List<Alert> _alerts = [];
  List<app_models.BluetoothDevice> _availableDevices = [];
  app_models.BluetoothDevice? _connectedDevice;
  bool _isConnected = false;
  bool _isScanning = false;
  double _energyCostRate = AppConstants.defaultEnergyCostRate;

  // Subscriptions
  StreamSubscription? _dataSubscription;
  StreamSubscription? _connectionSubscription;
  StreamSubscription? _devicesSubscription;
  Timer? _dataRequestTimer;

  // Getters
  EnergyData? get currentData => _currentData;
  List<EnergyData> get history => _history;
  List<Alert> get alerts => _alerts;
  List<app_models.BluetoothDevice> get availableDevices => _availableDevices;
  app_models.BluetoothDevice? get connectedDevice => _connectedDevice;
  bool get isConnected => _isConnected;
  bool get isScanning => _isScanning;
  double get energyCostRate => _energyCostRate;

  // Computed values
  double get currentCost => _currentData?.calculateCost(_energyCostRate) ?? 0.0;
  int get unreadAlertsCount => _alerts.where((a) => !a.isRead).length;

  EnergyProvider() {
    _initialize();
  }

  Future<void> _initialize() async {
    // Initialize services
    await _storageService.initialize();
    await _bluetoothService.initialize();

    // Load saved data
    _energyCostRate = _storageService.getEnergyCostRate();
    _history = _storageService.getEnergyHistory();
    _alerts = _storageService.getAlertHistory();

    // Load paired device
    _connectedDevice = _storageService.getPairedDevice();

    // Subscribe to streams
    _dataSubscription =
        _bluetoothService.dataStream.listen(_handleIncomingData);
    _connectionSubscription =
        _bluetoothService.connectionState.listen(_handleConnectionChange);
    _devicesSubscription =
        _bluetoothService.devicesStream.listen(_handleDevicesUpdate);

    notifyListeners();
  }

  // Handle incoming energy data
  void _handleIncomingData(EnergyData data) {
    _currentData = data;

    // Save to history every 5 seconds
    if (_history.isEmpty ||
        DateTime.now().difference(_history.last.timestamp).inSeconds >= 5) {
      _history.add(data);
      _storageService.saveEnergyData(data);
    }

    // Check for alerts
    _checkForAlerts(data);

    notifyListeners();
  }

  // Check for alert conditions
  void _checkForAlerts(EnergyData data) {
    // Check overload
    if (data.power > AppConstants.defaultOverpowerLimit) {
      _addAlert(Alert.overloadAlert(data.power));
    }

    // Check overcurrent
    if (data.current > AppConstants.defaultOvercurrentLimit) {
      _addAlert(Alert.overcurrentAlert(data.current));
    }

    // Check overvoltage
    if (data.voltage > AppConstants.defaultOvervoltageLimit) {
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

    if (connected) {
      // Start requesting data periodically
      _startDataRequests();
    } else {
      // Stop data requests
      _stopDataRequests();
      _addAlert(Alert.connectionLost());
    }

    notifyListeners();
  }

  // Handle devices update
  void _handleDevicesUpdate(List<app_models.BluetoothDevice> devices) {
    _availableDevices = devices;
    notifyListeners();
  }

  // Start periodic data requests
  void _startDataRequests() {
    _dataRequestTimer?.cancel();
    _dataRequestTimer = Timer.periodic(AppConstants.dataUpdateInterval, (_) {
      _bluetoothService.requestDataUpdate();
    });
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
    final success = await _bluetoothService.connect(device.address);

    if (success) {
      _connectedDevice = device.copyWith(isConnected: true);
      await _storageService.setPairedDevice(_connectedDevice!);
    }

    return success;
  }

  // Disconnect
  Future<void> disconnect() async {
    await _bluetoothService.disconnect();
    _connectedDevice = null;
    await _storageService.clearPairedDevice();
    notifyListeners();
  }

  // Reset energy counter
  Future<void> resetEnergyCounter() async {
    await _bluetoothService.resetEnergyCounter();
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

  // Get history for date range
  List<EnergyData> getHistoryByDateRange(DateTime start, DateTime end) {
    return _history.where((data) {
      return data.timestamp.isAfter(start) && data.timestamp.isBefore(end);
    }).toList();
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

    final newState = !_relayStates[index];
    await _bluetoothService.controlRelay(index, newState);

    // Update local state (optimistic UI)
    _relayStates[index] = newState;
    notifyListeners();
  }

  @override
  void dispose() {
    _dataSubscription?.cancel();
    _connectionSubscription?.cancel();
    _devicesSubscription?.cancel();
    _dataRequestTimer?.cancel();
    _bluetoothService.dispose();
    super.dispose();
  }
}
