class EnergyData {
  final double voltage;      // Volts (V)
  final double current;      // Amperes (A)
  final double power;        // Watts (W)
  final double energy;       // Kilowatt-hours (kWh)
  final int relayStatesMask; // 4 LSBs used (bit0..bit3)
  final String status;       // System status
  final DateTime timestamp;
  
  EnergyData({
    required this.voltage,
    required this.current,
    required this.power,
    required this.energy,
    this.relayStatesMask = 0,
    required this.status,
    DateTime? timestamp,
  }) : timestamp = timestamp ?? DateTime.now();
  
  // Calculate cost based on energy and rate
  double calculateCost(double ratePerKwh) {
    return energy * ratePerKwh;
  }
  
  // Check if any parameter exceeds limits
  bool isOverload({
    double voltageLimit = 250.0,
    double currentLimit = 10.0,
    double powerLimit = 2000.0,
  }) {
    return voltage > voltageLimit || 
           current > currentLimit || 
           power > powerLimit;
  }
  
  // Parse from comma-separated string
  // Expected format: "V,I,P,E,Status"
  // Example: "220.5,5.2,1146.6,2.5,OK"
  factory EnergyData.fromString(String data) {
    try {
      final parts = data.split(',');
      if (parts.length < 5) {
        throw const FormatException('Invalid data format');
      }
      
      return EnergyData(
        voltage: double.parse(parts[0].trim()),
        current: double.parse(parts[1].trim()),
        power: double.parse(parts[2].trim()),
        energy: double.parse(parts[3].trim()),
        status: parts[4].trim(),
      );
    } catch (e) {
      throw FormatException('Failed to parse energy data: $e');
    }
  }
  
  // Convert to JSON
  Map<String, dynamic> toJson() {
    return {
      'voltage': voltage,
      'current': current,
      'power': power,
      'energy': energy,
      'relayStatesMask': relayStatesMask,
      'status': status,
      'timestamp': timestamp.toIso8601String(),
    };
  }
  
  // Parse from JSON
  factory EnergyData.fromJson(Map<String, dynamic> json) {
    return EnergyData(
      voltage: (json['voltage'] as num).toDouble(),
      current: (json['current'] as num).toDouble(),
      power: (json['power'] as num).toDouble(),
      energy: (json['energy'] as num).toDouble(),
      relayStatesMask: (json['relayStatesMask'] as int?) ?? 0,
      status: json['status'] as String,
      timestamp: DateTime.parse(json['timestamp'] as String),
    );
  }
  
  // Create a copy with updated values
  EnergyData copyWith({
    double? voltage,
    double? current,
    double? power,
    double? energy,
    int? relayStatesMask,
    String? status,
    DateTime? timestamp,
  }) {
    return EnergyData(
      voltage: voltage ?? this.voltage,
      current: current ?? this.current,
      power: power ?? this.power,
      energy: energy ?? this.energy,
      relayStatesMask: relayStatesMask ?? this.relayStatesMask,
      status: status ?? this.status,
      timestamp: timestamp ?? this.timestamp,
    );
  }
  
  @override
  String toString() {
    return 'EnergyData(V: ${voltage.toStringAsFixed(1)}V, '
           'I: ${current.toStringAsFixed(2)}A, '
           'P: ${power.toStringAsFixed(1)}W, '
           'E: ${energy.toStringAsFixed(2)}kWh, '
           'Relays: 0x${relayStatesMask.toRadixString(16).padLeft(2, '0')}, '
           'Status: $status)';
  }
}
