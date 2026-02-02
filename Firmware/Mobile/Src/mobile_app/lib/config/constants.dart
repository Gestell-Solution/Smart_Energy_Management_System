// App Configuration Constants
class AppConstants {
  // App Info
  static const String appName = 'Smart Energy';
  static const String appVersion = '1.0.0';
  static const String buildNumber = '1';
  static const String buildDate = '29 January 2026';
  static const String companyName = 'Gestell Company';

  // Device Configuration
  static const String defaultDeviceId = '01';
  static const String bluetoothDeviceName = 'HC-05';

  // Energy Limits (from hardware specs) - max design values
  static const double maxVoltage = 260.0; // VAC (spec ceiling)
  static const double maxCurrent = 30.0; // Amperes (spec ceiling)
  static const double maxPower = 2000.0; // Watts (spec ceiling)

  // Default Thresholds (mutable so we can refresh from device info frame)
  static double defaultOvervoltageLimit = 250.0;
  static double defaultOvercurrentLimit = 10.0;
  static double defaultOverpowerLimit = 2000.0;

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
  static const String keyEnergyHistoryMigrated = 'energy_history_migrated';
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
  static const int cmdCuttOFF =
      0x09; // Relay Control / Protection Cutoff (embedded: CuttOFF)
  static const int cmdProtectionSafe = 0x0A;
  static const int cmdControlRelay =
      0x09; // Same as CuttOFF - relay control command
  static const int cmdGetDeviceInfo = 0x0C; // Device meta (limits + ID)
}
