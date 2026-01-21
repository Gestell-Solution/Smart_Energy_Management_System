class RelaySchedule {
  final int? id;
  final int relayIndex; // 0-3
  final String scheduleType; // 'daily', 'weekly', 'once'
  final DateTime scheduledTime;
  final bool turnOn; // true = ON, false = OFF
  final bool enabled;
  final String? days; // For weekly: comma-separated days (Mon,Tue,etc)

  RelaySchedule({
    this.id,
    required this.relayIndex,
    required this.scheduleType,
    required this.scheduledTime,
    required this.turnOn,
    this.enabled = true,
    this.days,
  });

  Map<String, dynamic> toMap() {
    return {
      'id': id,
      'relayIndex': relayIndex,
      'scheduleType': scheduleType,
      'scheduledTime': scheduledTime.toIso8601String(),
      'turnOn': turnOn ? 1 : 0,
      'enabled': enabled ? 1 : 0,
      'days': days,
    };
  }

  factory RelaySchedule.fromMap(Map<String, dynamic> map) {
    return RelaySchedule(
      id: map['id'] as int?,
      relayIndex: map['relayIndex'] as int,
      scheduleType: map['scheduleType'] as String,
      scheduledTime: DateTime.parse(map['scheduledTime'] as String),
      turnOn: (map['turnOn'] as int) == 1,
      enabled: (map['enabled'] as int) == 1,
      days: map['days'] as String?,
    );
  }

  RelaySchedule copyWith({
    int? id,
    int? relayIndex,
    String? scheduleType,
    DateTime? scheduledTime,
    bool? turnOn,
    bool? enabled,
    String? days,
  }) {
    return RelaySchedule(
      id: id ?? this.id,
      relayIndex: relayIndex ?? this.relayIndex,
      scheduleType: scheduleType ?? this.scheduleType,
      scheduledTime: scheduledTime ?? this.scheduledTime,
      turnOn: turnOn ?? this.turnOn,
      enabled: enabled ?? this.enabled,
      days: days ?? this.days,
    );
  }

  bool shouldExecuteNow() {
    final now = DateTime.now();

    if (!enabled) return false;

    if (scheduleType == 'once') {
      return now.isAfter(scheduledTime) &&
          now.difference(scheduledTime).inMinutes < 1;
    } else if (scheduleType == 'daily') {
      return now.hour == scheduledTime.hour &&
          now.minute == scheduledTime.minute;
    } else if (scheduleType == 'weekly' && days != null) {
      final dayNames = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];
      final todayName = dayNames[now.weekday - 1];
      return days!.contains(todayName) &&
          now.hour == scheduledTime.hour &&
          now.minute == scheduledTime.minute;
    }

    return false;
  }
}
