// App Configuration Constants
class AppConstants {
  // App Info
  static const String appName = 'Smart Energy';
  static const String appVersion = '1.0.0';
  static const String companyName = 'Gestell Company';

  // Device Configuration
  static const String defaultDeviceId = '01';
  static const String bluetoothDeviceName = 'HC-05';

  // Energy Limits (from hardware specs)
  static const double maxVoltage = 260.0; // VAC
  static const double maxCurrent = 30.0; // Amperes
  static const double maxPower = 2000.0; // Watts

  // Default Thresholds
  static const double defaultOvervoltageLimit = 250.0;
  static const double defaultOvercurrentLimit = 10.0;
  static const double defaultOverpowerLimit = 2000.0;

  // Cost Configuration (EGP per kWh)
  static const double defaultEnergyCostRate = 0.90; // Adjustable in settings

  // Data Update Intervals
  static const Duration dataUpdateInterval = Duration(seconds: 1);
  static const Duration chartUpdateInterval = Duration(seconds: 5);

  // Storage Keys
  static const String keyThemeMode = 'theme_mode';
  static const String keyEnergyCostRate = 'energy_cost_rate';
  static const String keyPairedDevice = 'paired_device';
  static const String keyEnergyHistory = 'energy_history';
  static const String keyAlertHistory = 'alert_history';

  // ========== Embedded Protocol Constants ==========

  // Frame Protocol
  static const int frameHeader = 0xAA; // Frame start marker

  // Command IDs (matching embedded system)
  static const int cmdCalibrateSensors = 0x0B;
  static const int cmdReadEeprom = 0x03;
  static const int cmdWriteEeprom = 0x04;
  static const int cmdGetRmsData = 0x05;
  static const int cmdGetLoggedData = 0x06;
  static const int cmdNotification = 0x07;
  static const int cmdUpdateEeprom = 0x08;
  static const int cmdProtectionDanger = 0x09;
  static const int cmdProtectionSafe = 0x0A;
  static const int cmdControlRelay = 0x0D;
}
