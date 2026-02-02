class DeviceInfo {
  final int deviceId;
  final int maxVoltage;
  final int maxCurrent;
  final int maxPower;

  const DeviceInfo({
    required this.deviceId,
    required this.maxVoltage,
    required this.maxCurrent,
    required this.maxPower,
  });

  factory DeviceInfo.fromPayload(List<int> data) {
    if (data.length != 7) {
      throw FormatException(
          'Invalid device info payload length: ${data.length}');
    }

    final deviceId = data[0];
    final maxVoltage = (data[1] << 8) | data[2];
    final maxCurrent = (data[3] << 8) | data[4];
    final maxPower = (data[5] << 8) | data[6];

    return DeviceInfo(
      deviceId: deviceId,
      maxVoltage: maxVoltage,
      maxCurrent: maxCurrent,
      maxPower: maxPower,
    );
  }
}
