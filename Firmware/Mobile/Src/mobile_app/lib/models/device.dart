class BluetoothDevice {
  final String name;
  final String address;
  final int rssi; // Signal strength
  final bool isConnected;
  
  BluetoothDevice({
    required this.name,
    required this.address,
    this.rssi = 0,
    this.isConnected = false,
  });
  
  // Convert to JSON
  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'address': address,
      'rssi': rssi,
      'isConnected': isConnected,
    };
  }
  
  // Parse from JSON
  factory BluetoothDevice.fromJson(Map<String, dynamic> json) {
    return BluetoothDevice(
      name: json['name'] as String,
      address: json['address'] as String,
      rssi: json['rssi'] as int? ?? 0,
      isConnected: json['isConnected'] as bool? ?? false,
    );
  }
  
  // Create a copy with updated values
  BluetoothDevice copyWith({
    String? name,
    String? address,
    int? rssi,
    bool? isConnected,
  }) {
    return BluetoothDevice(
      name: name ?? this.name,
      address: address ?? this.address,
      rssi: rssi ?? this.rssi,
      isConnected: isConnected ?? this.isConnected,
    );
  }
  
  @override
  String toString() {
    return 'BluetoothDevice(name: $name, address: $address, rssi: $rssi, connected: $isConnected)';
  }
  
  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;
    return other is BluetoothDevice && other.address == address;
  }
  
  @override
  int get hashCode => address.hashCode;
}
