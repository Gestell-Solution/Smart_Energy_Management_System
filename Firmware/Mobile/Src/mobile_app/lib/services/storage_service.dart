import 'dart:convert';
import 'package:shared_preferences/shared_preferences.dart';
import '../models/energy_data.dart';
import '../models/alert.dart';
import '../models/device.dart';
import '../database/energy_database.dart';
import '../config/constants.dart';

class StorageService {
  static final StorageService _instance = StorageService._internal();
  factory StorageService() => _instance;
  StorageService._internal();
  
  SharedPreferences? _prefs;
  final EnergyDatabase _db = EnergyDatabase();
  
  // Initialize
  Future<void> initialize() async {
    _prefs = await SharedPreferences.getInstance();

    await _migrateEnergyHistoryToDbIfNeeded();
  }

  Future<void> _migrateEnergyHistoryToDbIfNeeded() async {
    final alreadyMigrated =
        _prefs?.getBool(AppConstants.keyEnergyHistoryMigrated) ?? false;
    if (alreadyMigrated) return;

    final json = _prefs?.getString(AppConstants.keyEnergyHistory);
    if (json == null || json.isEmpty) {
      await _prefs?.setBool(AppConstants.keyEnergyHistoryMigrated, true);
      return;
    }

    try {
      final List<dynamic> jsonList = jsonDecode(json);
      final items = jsonList
          .map((e) => EnergyData.fromJson(e as Map<String, dynamic>))
          .toList();

      await _db.insertEnergyDataBatch(items);
      await _db.trimEnergyHistory(maxRecords: 1000);

      await _prefs?.remove(AppConstants.keyEnergyHistory);
      await _prefs?.setBool(AppConstants.keyEnergyHistoryMigrated, true);
    } catch (e) {
      print('Energy history migration error: $e');
      // Don't set migrated flag so we can retry on next launch.
    }
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
      await _db.insertEnergyData(data);
      await _db.trimEnergyHistory(maxRecords: 1000);
      return true;
    } catch (e) {
      print('Save energy data error: $e');
      return false;
    }
  }
  
  Future<List<EnergyData>> getEnergyHistory({int limit = 1000}) async {
    try {
      return await _db.getEnergyHistoryData(limit: limit);
    } catch (e) {
      print('Get energy history error: $e');
      return [];
    }
  }
  
  Future<bool> clearEnergyHistory() async {
    try {
      await _db.clearEnergyHistory();
      return true;
    } catch (e) {
      print('Clear energy history error: $e');
      return false;
    }
  }
  
  // Get history for specific time range
  Future<List<EnergyData>> getEnergyHistoryByDateRange(
      DateTime start, DateTime end,
      {int limit = 1000}) async {
    try {
      return await _db.getEnergyHistoryData(
        limit: limit,
        startDate: start,
        endDate: end,
      );
    } catch (e) {
      print('Get energy history by date range error: $e');
      return [];
    }
  }
  
  // Get today's history
  Future<List<EnergyData>> getTodayHistory({int limit = 1000}) async {
    final now = DateTime.now();
    final start = DateTime(now.year, now.month, now.day);
    final end = start.add(const Duration(days: 1));
    return getEnergyHistoryByDateRange(start, end, limit: limit);
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

  Future<bool> setAlertHistory(List<Alert> alerts) async {
    try {
      final jsonList = alerts.map((e) => e.toJson()).toList();
      final json = jsonEncode(jsonList);
      return await _prefs?.setString(AppConstants.keyAlertHistory, json) ??
          false;
    } catch (e) {
      print('Set alert history error: $e');
      return false;
    }
  }

  Future<bool> removeAlert(String alertId) async {
    try {
      final history = getAlertHistory();
      history.removeWhere((a) => a.id == alertId);
      return await setAlertHistory(history);
    } catch (e) {
      print('Remove alert error: $e');
      return false;
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

        return await setAlertHistory(history);
      }
      return false;
    } catch (e) {
      print('Mark alert as read error: $e');
      return false;
    }
  }
  
  // Clear all data
  Future<bool> clearAllData() async {
    try {
      await _db.deleteDatabase();
      return await _prefs?.clear() ?? false;
    } catch (e) {
      print('Clear all data error: $e');
      return false;
    }
  }
}
