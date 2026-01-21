class DeviceConfig {
  final int? id;
  final String deviceName;
  final String deviceAddress;
  final bool autoReconnect;
  final DateTime lastConnected;

  DeviceConfig({
    this.id,
    required this.deviceName,
    required this.deviceAddress,
    this.autoReconnect = false,
    DateTime? lastConnected,
  }) : lastConnected = lastConnected ?? DateTime.now();

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'deviceName': deviceName,
      'deviceAddress': deviceAddress,
      'autoReconnect': autoReconnect ? 1 : 0,
      'lastConnected': lastConnected.toIso8601String(),
    };
  }

  factory DeviceConfig.fromMap(Map<String, dynamic> map) {
    return DeviceConfig(
      id: map['id'] as int?,
      deviceName: map['deviceName'] as String,
      deviceAddress: map['deviceAddress'] as String,
      autoReconnect: (map['autoReconnect'] as int) == 1,
      lastConnected: DateTime.parse(map['lastConnected'] as String),
    );
  }

  DeviceConfig copyWith({
    int? id,
    String? deviceName,
    String? deviceAddress,
    bool? autoReconnect,
    DateTime? lastConnected,
  }) {
    return DeviceConfig(
      id: id ?? this.id,
      deviceName: deviceName ?? this.deviceName,
      deviceAddress: deviceAddress ?? this.deviceAddress,
      autoReconnect: autoReconnect ?? this.autoReconnect,
      lastConnected: lastConnected ?? this.lastConnected,
    );
  }
}
