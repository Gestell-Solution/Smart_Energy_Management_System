import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';
import '../models/energy_data.dart';
import '../models/device_config.dart';
import '../models/relay_schedule.dart';

class EnergyDatabase {
  static final EnergyDatabase _instance = EnergyDatabase._internal();
  factory EnergyDatabase() => _instance;
  EnergyDatabase._internal();

  static Database? _database;

  Future<Database> get database async {
    if (_database != null) return _database!;
    _database = await _initDatabase();
    return _database!;
  }

  Future<Database> _initDatabase() async {
    final databasePath = await getDatabasesPath();
    final path = join(databasePath, 'energy_manager.db');

    return await openDatabase(
      path,
      version: 1,
      onCreate: _onCreate,
    );
  }

  Future<void> _onCreate(Database db, int version) async {
    // Historical Energy Data Table
    await db.execute('''
      CREATE TABLE energy_history (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp TEXT NOT NULL,
        voltage REAL NOT NULL,
        current REAL NOT NULL,
        power REAL NOT NULL,
        energy REAL NOT NULL,
        status TEXT NOT NULL
      )
    ''');

    // Device Configurations Table
    await db.execute('''
      CREATE TABLE device_configs (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        deviceName TEXT NOT NULL,
        deviceAddress TEXT NOT NULL UNIQUE,
        autoReconnect INTEGER NOT NULL DEFAULT 0,
        lastConnected TEXT NOT NULL
      )
    ''');

    // Relay Schedules Table
    await db.execute('''
      CREATE TABLE relay_schedules (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        relayIndex INTEGER NOT NULL,
        scheduleType TEXT NOT NULL,
        scheduledTime TEXT NOT NULL,
        turnOn INTEGER NOT NULL,
        enabled INTEGER NOT NULL DEFAULT 1,
        days TEXT
      )
    ''');

    // Settings Table
    await db.execute('''
      CREATE TABLE settings (
        key TEXT PRIMARY KEY,
        value TEXT NOT NULL
      )
    ''');

    // Insert default settings
    await db.insert('settings', {'key': 'electricity_rate', 'value': '1.5'});
  }

  // ==================== Energy History Operations ====================

  Future<int> insertEnergyData(EnergyData data) async {
    final db = await database;
    return await db.insert('energy_history', {
      'timestamp': DateTime.now().toIso8601String(),
      'voltage': data.voltage,
      'current': data.current,
      'power': data.power,
      'energy': data.energy,
      'status': data.status,
    });
  }

  Future<List<Map<String, dynamic>>> getEnergyHistory({
    int? limit,
    DateTime? startDate,
    DateTime? endDate,
  }) async {
    final db = await database;
    String query = 'SELECT * FROM energy_history';
    List<String> whereConditions = [];
    List<dynamic> whereArgs = [];

    if (startDate != null) {
      whereConditions.add('timestamp >= ?');
      whereArgs.add(startDate.toIso8601String());
    }

    if (endDate != null) {
      whereConditions.add('timestamp <= ?');
      whereArgs.add(endDate.toIso8601String());
    }

    if (whereConditions.isNotEmpty) {
      query += ' WHERE ${whereConditions.join(' AND ')}';
    }

    query += ' ORDER BY timestamp DESC';

    if (limit != null) {
      query += ' LIMIT $limit';
    }

    return await db.rawQuery(query, whereArgs);
  }

  Future<void> clearOldEnergyData(int daysToKeep) async {
    final db = await database;
    final cutoffDate = DateTime.now().subtract(Duration(days: daysToKeep));
    await db.delete(
      'energy_history',
      where: 'timestamp < ?',
      whereArgs: [cutoffDate.toIso8601String()],
    );
  }

  // ==================== Device Config Operations ====================

  Future<int> insertDeviceConfig(DeviceConfig config) async {
    final db = await database;
    return await db.insert(
      'device_configs',
      config.toMap(),
      conflictAlgorithm: ConflictAlgorithm.replace,
    );
  }

  Future<List<DeviceConfig>> getAllDeviceConfigs() async {
    final db = await database;
    final maps = await db.query(
      'device_configs',
      orderBy: 'lastConnected DESC',
    );
    return maps.map((map) => DeviceConfig.fromMap(map)).toList();
  }

  Future<DeviceConfig?> getDeviceConfig(String deviceAddress) async {
    final db = await database;
    final maps = await db.query(
      'device_configs',
      where: 'deviceAddress = ?',
      whereArgs: [deviceAddress],
      limit: 1,
    );

    if (maps.isEmpty) return null;
    return DeviceConfig.fromMap(maps.first);
  }

  Future<void> updateDeviceConfig(DeviceConfig config) async {
    final db = await database;
    await db.update(
      'device_configs',
      config.toMap(),
      where: 'id = ?',
      whereArgs: [config.id],
    );
  }

  Future<void> deleteDeviceConfig(int id) async {
    final db = await database;
    await db.delete(
      'device_configs',
      where: 'id = ?',
      whereArgs: [id],
    );
  }

  // ==================== Relay Schedule Operations ====================

  Future<int> insertRelaySchedule(RelaySchedule schedule) async {
    final db = await database;
    return await db.insert('relay_schedules', schedule.toMap());
  }

  Future<List<RelaySchedule>> getAllRelaySchedules() async {
    final db = await database;
    final maps = await db.query('relay_schedules');
    return maps.map((map) => RelaySchedule.fromMap(map)).toList();
  }

  Future<List<RelaySchedule>> getActiveRelaySchedules() async {
    final db = await database;
    final maps = await db.query(
      'relay_schedules',
      where: 'enabled = ?',
      whereArgs: [1],
    );
    return maps.map((map) => RelaySchedule.fromMap(map)).toList();
  }

  Future<void> updateRelaySchedule(RelaySchedule schedule) async {
    final db = await database;
    await db.update(
      'relay_schedules',
      schedule.toMap(),
      where: 'id = ?',
      whereArgs: [schedule.id],
    );
  }

  Future<void> deleteRelaySchedule(int id) async {
    final db = await database;
    await db.delete(
      'relay_schedules',
      where: 'id = ?',
      whereArgs: [id],
    );
  }

  // ==================== Settings Operations ====================

  Future<String?> getSetting(String key) async {
    final db = await database;
    final maps = await db.query(
      'settings',
      where: 'key = ?',
      whereArgs: [key],
      limit: 1,
    );

    if (maps.isEmpty) return null;
    return maps.first['value'] as String;
  }

  Future<void> setSetting(String key, String value) async {
    final db = await database;
    await db.insert(
      'settings',
      {'key': key, 'value': value},
      conflictAlgorithm: ConflictAlgorithm.replace,
    );
  }

  Future<double> getElectricityRate() async {
    final rate = await getSetting('electricity_rate');
    return rate != null ? double.parse(rate) : 1.5; // Default 1.5 EGP/kWh
  }

  Future<void> setElectricityRate(double rate) async {
    await setSetting('electricity_rate', rate.toString());
  }

  // ==================== Database Maintenance ====================

  Future<void> close() async {
    final db = await database;
    await db.close();
  }

  Future<void> deleteDatabase() async {
    final databasePath = await getDatabasesPath();
    final path = join(databasePath, 'energy_manager.db');
    await databaseFactory.deleteDatabase(path);
  }
}
