import 'dart:convert';
import 'package:shared_preferences/shared_preferences.dart';
import '../models/energy_data.dart';
import '../models/alert.dart';
import '../models/device.dart';
import '../config/constants.dart';

class StorageService {
  static final StorageService _instance = StorageService._internal();
  factory StorageService() => _instance;
  StorageService._internal();
  
  SharedPreferences? _prefs;
  
  // Initialize
  Future<void> initialize() async {
    _prefs = await SharedPreferences.getInstance();
  }
  
  // Theme Mode
  Future<bool> setThemeMode(bool isDark) async {
    return await _prefs?.setBool(AppConstants.keyThemeMode, isDark) ?? false;
  }
  
  bool getThemeMode() {
    return _prefs?.getBool(AppConstants.keyThemeMode) ?? false;
  }
  
  // Energy Cost Rate
  Future<bool> setEnergyCostRate(double rate) async {
    return await _prefs?.setDouble(AppConstants.keyEnergyCostRate, rate) ?? false;
  }
  
  double getEnergyCostRate() {
    return _prefs?.getDouble(AppConstants.keyEnergyCostRate) ?? 
           AppConstants.defaultEnergyCostRate;
  }
  
  // Paired Device
  Future<bool> setPairedDevice(BluetoothDevice device) async {
    final json = jsonEncode(device.toJson());
    return await _prefs?.setString(AppConstants.keyPairedDevice, json) ?? false;
  }
  
  BluetoothDevice? getPairedDevice() {
    final json = _prefs?.getString(AppConstants.keyPairedDevice);
    if (json == null) return null;
    
    try {
      final map = jsonDecode(json) as Map<String, dynamic>;
      return BluetoothDevice.fromJson(map);
    } catch (e) {
      return null;
    }
  }
  
  Future<bool> clearPairedDevice() async {
    return await _prefs?.remove(AppConstants.keyPairedDevice) ?? false;
  }
  
  // Energy History
  Future<bool> saveEnergyData(EnergyData data) async {
    try {
      final history = getEnergyHistory();
      history.add(data);
      
      // Keep only last 1000 records to avoid memory issues
      if (history.length > 1000) {
        history.removeAt(0);
      }
      
      final jsonList = history.map((e) => e.toJson()).toList();
      final json = jsonEncode(jsonList);
      
      return await _prefs?.setString(AppConstants.keyEnergyHistory, json) ?? false;
    } catch (e) {
      print('Save energy data error: $e');
      return false;
    }
  }
  
  List<EnergyData> getEnergyHistory() {
    final json = _prefs?.getString(AppConstants.keyEnergyHistory);
    if (json == null) return [];
    
    try {
      final List<dynamic> jsonList = jsonDecode(json);
      return jsonList.map((e) => EnergyData.fromJson(e as Map<String, dynamic>)).toList();
    } catch (e) {
      print('Get energy history error: $e');
      return [];
    }
  }
  
  Future<bool> clearEnergyHistory() async {
    return await _prefs?.remove(AppConstants.keyEnergyHistory) ?? false;
  }
  
  // Get history for specific time range
  List<EnergyData> getEnergyHistoryByDateRange(DateTime start, DateTime end) {
    final history = getEnergyHistory();
    return history.where((data) {
      return data.timestamp.isAfter(start) && data.timestamp.isBefore(end);
    }).toList();
  }
  
  // Get today's history
  List<EnergyData> getTodayHistory() {
    final now = DateTime.now();
    final start = DateTime(now.year, now.month, now.day);
    final end = start.add(const Duration(days: 1));
    return getEnergyHistoryByDateRange(start, end);
  }
  
  // Alert History
  Future<bool> saveAlert(Alert alert) async {
    try {
      final history = getAlertHistory();
      history.insert(0, alert); // Add to beginning
      
      // Keep only last 100 alerts
      if (history.length > 100) {
        history.removeLast();
      }
      
      final jsonList = history.map((e) => e.toJson()).toList();
      final json = jsonEncode(jsonList);
      
      return await _prefs?.setString(AppConstants.keyAlertHistory, json) ?? false;
    } catch (e) {
      print('Save alert error: $e');
      return false;
    }
  }
  
  List<Alert> getAlertHistory() {
    final json = _prefs?.getString(AppConstants.keyAlertHistory);
    if (json == null) return [];
    
    try {
      final List<dynamic> jsonList = jsonDecode(json);
      return jsonList.map((e) => Alert.fromJson(e as Map<String, dynamic>)).toList();
    } catch (e) {
      print('Get alert history error: $e');
      return [];
    }
  }
  
  Future<bool> clearAlertHistory() async {
    return await _prefs?.remove(AppConstants.keyAlertHistory) ?? false;
  }
  
  Future<bool> markAlertAsRead(String alertId) async {
    try {
      final history = getAlertHistory();
      final index = history.indexWhere((a) => a.id == alertId);
      
      if (index != -1) {
        history[index] = history[index].copyWith(isRead: true);
        
        final jsonList = history.map((e) => e.toJson()).toList();
        final json = jsonEncode(jsonList);
        
        return await _prefs?.setString(AppConstants.keyAlertHistory, json) ?? false;
      }
      return false;
    } catch (e) {
      print('Mark alert as read error: $e');
      return false;
    }
  }
  
  // Clear all data
  Future<bool> clearAllData() async {
    return await _prefs?.clear() ?? false;
  }
}
