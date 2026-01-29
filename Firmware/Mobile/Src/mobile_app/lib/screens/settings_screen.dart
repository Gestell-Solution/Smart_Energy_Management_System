// ignore_for_file: deprecated_member_use

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/energy_provider.dart';
import '../providers/theme_provider.dart';
import '../config/theme.dart';
import '../config/constants.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
        centerTitle: true,
      ),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          // Bluetooth Connection Section
          _buildSectionHeader(context, 'Bluetooth Connection'),
          const SizedBox(height: 12),
          _buildBluetoothCard(context),
          const SizedBox(height: 24),

          // Preferences Section
          _buildSectionHeader(context, 'Preferences'),
          const SizedBox(height: 12),
          _buildThemeCard(context),
          const SizedBox(height: 12),
          _buildCostRateCard(context),
          const SizedBox(height: 24),

          // Device Info Section
          _buildSectionHeader(context, 'Device Information'),
          const SizedBox(height: 12),
          _buildDeviceInfoCard(context),
          const SizedBox(height: 24),

          // About Section
          _buildSectionHeader(context, 'About'),
          const SizedBox(height: 12),
          _buildAboutCard(context),
        ],
      ),
    );
  }

  Widget _buildSectionHeader(BuildContext context, String title) {
    return Padding(
      padding: const EdgeInsets.only(left: 4),
      child: Text(
        title,
        style: Theme.of(context).textTheme.titleMedium?.copyWith(
              fontWeight: FontWeight.bold,
            ),
      ),
    );
  }

  Widget _buildBluetoothCard(BuildContext context) {
    return Consumer<EnergyProvider>(
      builder: (context, provider, _) {
        return Card(
          child: Column(
            children: [
              // Connection Status
              ListTile(
                leading: Container(
                  padding: const EdgeInsets.all(10),
                  decoration: BoxDecoration(
                    color: provider.isConnected
                        ? AppTheme.primaryColor.withOpacity(0.15)
                        : AppTheme.error.withOpacity(0.15),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  child: Icon(
                    provider.isConnected
                        ? Icons.bluetooth_connected
                        : Icons.bluetooth_disabled,
                    color: provider.isConnected
                        ? AppTheme.success
                        : AppTheme.error,
                  ),
                ),
                title: Text(
                  provider.isConnected ? 'Connected' : 'Disconnected',
                  style: const TextStyle(fontWeight: FontWeight.w600),
                ),
                subtitle: Text(
                  provider.connectedDevice?.name ?? 'No device connected',
                ),
                trailing: provider.isConnected
                    ? ElevatedButton(
                        onPressed: () async {
                          await provider.disconnect();
                          if (context.mounted) {
                            ScaffoldMessenger.of(context).showSnackBar(
                              const SnackBar(
                                  content: Text('Disconnected from device')),
                            );
                          }
                        },
                        style: ElevatedButton.styleFrom(
                          backgroundColor: AppTheme.error,
                        ),
                        child: const Text('Disconnect'),
                      )
                    : null,
              ),

              // Scan/Connect Button
              if (!provider.isConnected)
                Padding(
                  padding: const EdgeInsets.all(16),
                  child: SizedBox(
                    width: double.infinity,
                    child: ElevatedButton.icon(
                      onPressed: provider.isScanning
                          ? null
                          : () => _showDeviceScanDialog(context),
                      icon: Icon(
                        provider.isScanning
                            ? Icons.hourglass_empty
                            : Icons.bluetooth_searching,
                      ),
                      label: Text(
                        provider.isScanning
                            ? 'Scanning...'
                            : 'Scan for Devices',
                      ),
                    ),
                  ),
                ),
            ],
          ),
        );
      },
    );
  }

  Widget _buildThemeCard(BuildContext context) {
    return Consumer<ThemeProvider>(
      builder: (context, themeProvider, _) {
        return Card(
          child: SwitchListTile(
            secondary: Container(
              padding: const EdgeInsets.all(10),
              decoration: BoxDecoration(
                color: AppTheme.primaryColor.withOpacity(0.15),
                borderRadius: BorderRadius.circular(12),
              ),
              child: Icon(
                themeProvider.isDarkMode ? Icons.dark_mode : Icons.light_mode,
                color: AppTheme.primaryColor,
              ),
            ),
            title: const Text(
              'Dark Mode',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle: const Text('Switch between light and dark theme'),
            value: themeProvider.isDarkMode,
            onChanged: (value) {
              themeProvider.toggleTheme();
            },
          ),
        );
      },
    );
  }

  Widget _buildCostRateCard(BuildContext context) {
    return Consumer<EnergyProvider>(
      builder: (context, provider, _) {
        return Card(
          child: ListTile(
            leading: Container(
              padding: const EdgeInsets.all(10),
              decoration: BoxDecoration(
                color: AppTheme.warning.withOpacity(0.1),
                borderRadius: BorderRadius.circular(12),
              ),
              child: const Icon(
                Icons.attach_money,
                color: AppTheme.warning,
              ),
            ),
            title: const Text(
              'Energy Cost Rate',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle:
                Text('${provider.energyCostRate.toStringAsFixed(2)} EGP/kWh'),
            trailing: IconButton(
              icon: const Icon(Icons.edit),
              onPressed: () => _showCostRateDialog(context, provider),
            ),
          ),
        );
      },
    );
  }

  Widget _buildDeviceInfoCard(BuildContext context) {
    return Card(
      child: Column(
        children: [
          _buildInfoTile(
            'Device ID',
            AppConstants.defaultDeviceId,
            Icons.fingerprint,
          ),
          const Divider(height: 1),
          _buildInfoTile(
            'Max Voltage',
            '${AppConstants.maxVoltage.toStringAsFixed(0)} V',
            Icons.electric_bolt,
          ),
          const Divider(height: 1),
          _buildInfoTile(
            'Max Current',
            '${AppConstants.maxCurrent.toStringAsFixed(0)} A',
            Icons.electrical_services,
          ),
          const Divider(height: 1),
          _buildInfoTile(
            'Max Power',
            '${AppConstants.maxPower.toStringAsFixed(0)} W',
            Icons.power,
          ),
        ],
      ),
    );
  }

  Widget _buildInfoTile(String title, String value, IconData icon) {
    return ListTile(
      leading: Icon(icon, color: AppTheme.primaryColor),
      title: Text(title),
      trailing: Text(
        value,
        style: const TextStyle(
          fontWeight: FontWeight.bold,
        ),
      ),
    );
  }

  Widget _buildAboutCard(BuildContext context) {
    return Card(
      child: Column(
        children: [
          // App Info Header
          ListTile(
            leading: Container(
              padding: const EdgeInsets.all(10),
              decoration: BoxDecoration(
                color: AppTheme.primaryColor.withOpacity(0.1),
                borderRadius: BorderRadius.circular(12),
              ),
              child: const Icon(
                Icons.info,
                color: AppTheme.primaryColor,
              ),
            ),
            title: const Text(
              'Gestell Smart Energy',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle: const Text('Real-time Energy Monitoring System'),
          ),
          const Divider(height: 1),

          // Version
          const ListTile(
            leading: Icon(Icons.update, color: AppTheme.primaryColor),
            title: Text(
              'Version',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle: Text(
                '${AppConstants.appVersion} (Build ${AppConstants.buildNumber})\n${AppConstants.buildDate}'),
          ),
          const Divider(height: 1),

          // Company
          const ListTile(
            leading: Icon(Icons.business, color: AppTheme.primaryColor),
            title: Text(
              'Developed by',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle: Text(AppConstants.companyName),
          ),
          const Divider(height: 1),

          // Development Team - Expandable
          const ExpansionTile(
            leading: Icon(Icons.group, color: AppTheme.primaryColor),
            title: Text(
              'Development Team',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            children: [
              Padding(
                padding:
                    EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    // Managers Section
                    Row(
                      children: [
                        Icon(Icons.manage_accounts,
                            size: 20, color: AppTheme.primaryColor),
                        SizedBox(width: 8),
                        Text(
                          'Managers',
                          style: TextStyle(
                            fontWeight: FontWeight.bold,
                            color: AppTheme.primaryColor,
                            fontSize: 14,
                          ),
                        ),
                      ],
                    ),
                    SizedBox(height: 8),
                    Padding(
                      padding: EdgeInsets.only(left: 28),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text('• Hesham Ahmed',
                              style: TextStyle(
                                  fontSize: 13, fontWeight: FontWeight.bold)),
                          Text('  (Project Manager)',
                              style:
                                  TextStyle(fontSize: 11, color: Colors.grey)),
                          SizedBox(height: 8),
                        ],
                      ),
                    ),

                    SizedBox(height: 12),

                    // Embedded Team Section
                    Row(
                      children: [
                        Icon(Icons.memory,
                            size: 20, color: AppTheme.primaryColor),
                        SizedBox(width: 8),
                        Text(
                          'Embedded',
                          style: TextStyle(
                            fontWeight: FontWeight.bold,
                            color: AppTheme.primaryColor,
                            fontSize: 14,
                          ),
                        ),
                      ],
                    ),
                    SizedBox(height: 8),
                    Padding(
                      padding: EdgeInsets.only(left: 28),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text('• Mohamed Abdelgaber',
                              style: TextStyle(fontSize: 13)),
                          SizedBox(height: 6),
                          Text('• Ahmed Twap', style: TextStyle(fontSize: 13)),
                          SizedBox(height: 6),
                          Text('• Ahmed Ashraf',
                              style: TextStyle(fontSize: 13)),
                          SizedBox(height: 6),
                          Text('• Basma Khaled',
                              style: TextStyle(fontSize: 13)),
                          SizedBox(height: 6),
                          Text('• Mohammed Diaa',
                              style: TextStyle(fontSize: 13)),
                          SizedBox(height: 8),
                        ],
                      ),
                    ),

                    SizedBox(height: 12),

                    // Mobile Team Section
                    Row(
                      children: [
                        Icon(Icons.phone_android,
                            size: 20, color: AppTheme.primaryColor),
                        SizedBox(width: 8),
                        Text(
                          'Mobile',
                          style: TextStyle(
                            fontWeight: FontWeight.bold,
                            color: AppTheme.primaryColor,
                            fontSize: 14,
                          ),
                        ),
                      ],
                    ),
                    SizedBox(height: 8),
                    Padding(
                      padding: EdgeInsets.only(left: 28),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text('• Aya Mohamed', style: TextStyle(fontSize: 13)),
                          SizedBox(height: 6),
                          Text('• Salma Tarek', style: TextStyle(fontSize: 13)),
                          SizedBox(height: 8),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),

          const Divider(height: 1),

          // Project Description
          const ExpansionTile(
            leading:
                Icon(Icons.description, color: AppTheme.primaryColor),
            title: Text(
              'About Project',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            children: [
              Padding(
                padding: EdgeInsets.all(16),
                child: Text(
                  'The Gestell Smart Energy Management System is an advanced IoT solution '
                  'for real-time monitoring and control of electrical energy consumption. '
                  '\n\n🎯 Project Goals:\n'
                  '• Reduce energy waste through intelligent monitoring\n'
                  '• Protect electrical appliances from damage\n'
                  '• Enable data-driven energy decisions\n'
                  '• Provide cost analysis and savings tracking'
                  '\n\n✨ Key Features:\n'
                  '• Real-time voltage, current, and power monitoring\n'
                  '• Energy consumption tracking and history\n'
                  '• Intelligent relay control and scheduling\n'
                  '• Alert system for electrical anomalies\n'
                  '• Bluetooth connectivity (HC-05 module)\n'
                  '• Cost calculation and analysis\n'
                  '• Historical data logging and export'
                  '\n\n🔧 Technical Stack:\n'
                  '• Embedded: AVR ATmega32, ACS712, ZMPT101B\n'
                  '• Mobile: Flutter Framework (Dart)\n'
                  '• Communication: Bluetooth Classic (SPP)\n'
                  '• Database: SQLite local storage',
                  style: TextStyle(fontSize: 13, height: 1.5),
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }

  void _showDeviceScanDialog(BuildContext context) {
    final provider = context.read<EnergyProvider>();

    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Scan for Devices'),
        content: SizedBox(
          width: double.maxFinite,
          height: 320,
          child: Consumer<EnergyProvider>(
            builder: (context, provider, _) {
              if (provider.isScanning) {
                if (provider.availableDevices.isEmpty) {
                  return const Center(
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        CircularProgressIndicator(),
                        SizedBox(height: 16),
                        Text('Scanning for devices...'),
                        SizedBox(height: 8),
                        Text(
                          'This may take up to 15 seconds',
                          style: TextStyle(
                            fontSize: 12,
                            color: Colors.grey,
                          ),
                        ),
                      ],
                    ),
                  );
                }

                return Column(
                  children: [
                    const LinearProgressIndicator(),
                    const SizedBox(height: 12),
                    Expanded(
                      child: ListView.builder(
                        itemCount: provider.availableDevices.length,
                        itemBuilder: (context, index) {
                          final device = provider.availableDevices[index];
                          return ListTile(
                            leading: const Icon(Icons.bluetooth),
                            title: Text(device.name),
                            subtitle: Text(device.address),
                            trailing: Text('${device.rssi} dBm'),
                            onTap: () async {
                              Navigator.pop(context);
                              final success =
                                  await provider.connectToDevice(device);

                              if (context.mounted) {
                                final errorMessage =
                                    provider.connectionError.isNotEmpty
                                        ? provider.connectionError
                                        : 'Failed to connect';
                                ScaffoldMessenger.of(context).showSnackBar(
                                  SnackBar(
                                    content: Text(
                                      success
                                          ? 'Connected to ${device.name}'
                                          : errorMessage,
                                    ),
                                  ),
                                );
                              }
                            },
                          );
                        },
                      ),
                    ),
                  ],
                );
              }

              // Show scan error if any
              if (provider.scanError.isNotEmpty) {
                return Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Icon(
                        Icons.error_outline,
                        size: 48,
                        color: AppTheme.error,
                      ),
                      const SizedBox(height: 16),
                      Text(
                        'Scan Error',
                        style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                          color: AppTheme.error,
                        ),
                      ),
                      const SizedBox(height: 8),
                      Padding(
                        padding: const EdgeInsets.symmetric(horizontal: 16),
                        child: Text(
                          provider.scanError,
                          textAlign: TextAlign.center,
                          style: const TextStyle(fontSize: 14),
                        ),
                      ),
                      const SizedBox(height: 16),
                      Text(
                        'Troubleshooting tips:',
                        style: TextStyle(
                          fontSize: 14,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                      const SizedBox(height: 8),
                      const Padding(
                        padding: EdgeInsets.symmetric(horizontal: 16),
                        child: Text(
                          '• Enable Bluetooth & Location\n'
                          '• Grant Bluetooth permissions\n'
                          '• Ensure devices are discoverable\n'
                          '• Move closer to devices',
                          textAlign: TextAlign.center,
                          style: TextStyle(fontSize: 12),
                        ),
                      ),
                    ],
                  ),
                );
              }

              if (provider.availableDevices.isEmpty) {
                return const Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Icon(
                        Icons.bluetooth_disabled,
                        size: 48,
                        color: Colors.grey,
                      ),
                      SizedBox(height: 16),
                      Text(
                        'No devices found',
                        style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                          color: Colors.grey,
                        ),
                      ),
                      SizedBox(height: 8),
                      Text(
                        'Try scanning again or check device settings',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 14),
                      ),
                    ],
                  ),
                );
              }

              return ListView.builder(
                itemCount: provider.availableDevices.length,
                itemBuilder: (context, index) {
                  final device = provider.availableDevices[index];
                  return ListTile(
                    leading: const Icon(Icons.bluetooth),
                    title: Text(device.name),
                    subtitle: Text(device.address),
                    trailing: Text('${device.rssi} dBm'),
                    onTap: () async {
                      Navigator.pop(context);
                      final success = await provider.connectToDevice(device);

                      if (context.mounted) {
                        final errorMessage = provider.connectionError.isNotEmpty
                            ? provider.connectionError
                            : 'Failed to connect';
                        ScaffoldMessenger.of(context).showSnackBar(
                          SnackBar(
                            content: Text(
                              success
                                  ? 'Connected to ${device.name}'
                                  : errorMessage,
                            ),
                          ),
                        );
                      }
                    },
                  );
                },
              );
            },
          ),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('Close'),
          ),
          if (!provider.isScanning)
            ElevatedButton(
              onPressed: () {
                provider.scanForDevices();
              },
              child: const Text('Scan Again'),
            ),
        ],
      ),
    );

    // Start scanning automatically
    provider.scanForDevices();
  }

  void _showCostRateDialog(BuildContext context, EnergyProvider provider) {
    final controller = TextEditingController(
      text: provider.energyCostRate.toStringAsFixed(2),
    );

    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Set Energy Cost Rate'),
        content: TextField(
          controller: controller,
          keyboardType: const TextInputType.numberWithOptions(decimal: true),
          decoration: const InputDecoration(
            labelText: 'Cost per kWh (EGP)',
            hintText: '0.90',
            prefixIcon: Icon(Icons.attach_money),
          ),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () {
              final value = double.tryParse(controller.text);
              if (value != null && value > 0) {
                provider.setEnergyCostRate(value);
                Navigator.pop(context);
                ScaffoldMessenger.of(context).showSnackBar(
                  const SnackBar(content: Text('Cost rate updated')),
                );
              }
            },
            child: const Text('Save'),
          ),
        ],
      ),
    );
  }
}
