enum AlertType {
  overload,
  overcurrent,
  overvoltage,
  systemError,
  connectionLost,
  info,
}

enum AlertSeverity {
  critical,
  warning,
  info,
}

class Alert {
  final String id;
  final AlertType type;
  final AlertSeverity severity;
  final String message;
  final DateTime timestamp;
  final bool isRead;
  
  Alert({
    String? id,
    required this.type,
    required this.severity,
    required this.message,
    DateTime? timestamp,
    this.isRead = false,
  }) : id = id ?? DateTime.now().millisecondsSinceEpoch.toString(),
       timestamp = timestamp ?? DateTime.now();
  
  // Get icon based on alert type
  String get icon {
    switch (type) {
      case AlertType.overload:
        return '⚡';
      case AlertType.overcurrent:
        return '🔌';
      case AlertType.overvoltage:
        return '⚠️';
      case AlertType.systemError:
        return '❌';
      case AlertType.connectionLost:
        return '📡';
      case AlertType.info:
        return 'ℹ️';
    }
  }
  
  // Get color based on severity
  int get color {
    switch (severity) {
      case AlertSeverity.critical:
        return 0xFFEF4444; // Red
      case AlertSeverity.warning:
        return 0xFFF59E0B; // Amber
      case AlertSeverity.info:
        return 0xFF3B82F6; // Blue
    }
  }
  
  // Convert to JSON
  Map<String, dynamic> toJson() {
    return {
      'id': id,
      'type': type.toString(),
      'severity': severity.toString(),
      'message': message,
      'timestamp': timestamp.toIso8601String(),
      'isRead': isRead,
    };
  }
  
  // Parse from JSON
  factory Alert.fromJson(Map<String, dynamic> json) {
    return Alert(
      id: json['id'] as String,
      type: AlertType.values.firstWhere(
        (e) => e.toString() == json['type'],
        orElse: () => AlertType.info,
      ),
      severity: AlertSeverity.values.firstWhere(
        (e) => e.toString() == json['severity'],
        orElse: () => AlertSeverity.info,
      ),
      message: json['message'] as String,
      timestamp: DateTime.parse(json['timestamp'] as String),
      isRead: json['isRead'] as bool? ?? false,
    );
  }
  
  // Create a copy with updated values
  Alert copyWith({
    String? id,
    AlertType? type,
    AlertSeverity? severity,
    String? message,
    DateTime? timestamp,
    bool? isRead,
  }) {
    return Alert(
      id: id ?? this.id,
      type: type ?? this.type,
      severity: severity ?? this.severity,
      message: message ?? this.message,
      timestamp: timestamp ?? this.timestamp,
      isRead: isRead ?? this.isRead,
    );
  }
  
  // Factory methods for common alerts
  factory Alert.overloadAlert(double power) {
    return Alert(
      type: AlertType.overload,
      severity: AlertSeverity.critical,
      message: 'Power overload detected! Current power: ${power.toStringAsFixed(0)}W',
    );
  }
  
  factory Alert.overcurrentAlert(double current) {
    return Alert(
      type: AlertType.overcurrent,
      severity: AlertSeverity.critical,
      message: 'Overcurrent detected! Current: ${current.toStringAsFixed(1)}A',
    );
  }
  
  factory Alert.overvoltageAlert(double voltage) {
    return Alert(
      type: AlertType.overvoltage,
      severity: AlertSeverity.warning,
      message: 'High voltage detected! Voltage: ${voltage.toStringAsFixed(1)}V',
    );
  }
  
  factory Alert.connectionLost() {
    return Alert(
      type: AlertType.connectionLost,
      severity: AlertSeverity.warning,
      message: 'Bluetooth connection lost',
    );
  }
  
  factory Alert.systemError(String error) {
    return Alert(
      type: AlertType.systemError,
      severity: AlertSeverity.critical,
      message: 'System error: $error',
    );
  }
}
