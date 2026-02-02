// ignore_for_file: deprecated_member_use

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/energy_provider.dart';
import '../providers/theme_provider.dart';
import '../config/theme.dart';
import '../config/constants.dart';
import 'about_screen.dart';
import 'alerts_screen.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
        centerTitle: true,
        actions: [
          IconButton(
            icon: const Icon(Icons.notifications_outlined),
            onPressed: () {
              Navigator.of(context).push(
                MaterialPageRoute(builder: (_) => const AlertsScreen()),
              );
            },
          ),
          const SizedBox(width: 8),
        ],
      ),
      body: SafeArea(
        top: false,
        child: ListView(
          padding: const EdgeInsets.fromLTRB(16, 16, 16, 24),
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
            Consumer<EnergyProvider>(
              builder: (context, provider, _) =>
                  _buildDeviceInfoCard(context, provider),
            ),
            const SizedBox(height: 24),

            // About Section
            _buildSectionHeader(context, 'About'),
            const SizedBox(height: 12),
            Card(
              child: ListTile(
                leading: const Icon(Icons.info_outline, color: AppTheme.primaryColor),
                title: const Text(
                  'About Smart Energy',
                  style: TextStyle(fontWeight: FontWeight.w600),
                ),
                subtitle: const Text('Company info, team, and contact links'),
                trailing: const Icon(Icons.chevron_right),
                onTap: () {
                  Navigator.of(context).push(
                    MaterialPageRoute(builder: (_) => const AboutScreen()),
                  );
                },
              ),
            ),
          ],
        ),
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
            onChanged: (!themeProvider.isInitialized || themeProvider.isToggling)
                ? null
                : (value) async {
                    await themeProvider.setThemeMode(value);
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

  Widget _buildDeviceInfoCard(
      BuildContext context, EnergyProvider provider) {
    final info = provider.deviceInfo;
    final deviceId =
        info != null ? info.deviceId.toString().padLeft(2, '0') : AppConstants.defaultDeviceId;
    final maxVoltage = info?.maxVoltage ?? AppConstants.defaultOvervoltageLimit;
    final maxCurrent = info?.maxCurrent ?? AppConstants.defaultOvercurrentLimit;
    final pf = provider.powerFactor;
    final apparentPower =
        info?.maxPower ?? AppConstants.defaultOverpowerLimit;
    // Active limit is what triggers the overload alert
    final maxPower = provider.activePowerLimit;

    return Card(
      child: Column(
        children: [
          _buildInfoTile(
            'Device ID',
            deviceId,
            Icons.fingerprint,
          ),
          const Divider(height: 1),
          _buildInfoTile(
            'Max Voltage',
            '${maxVoltage.toStringAsFixed(0)} V',
            Icons.electric_bolt,
          ),
          const Divider(height: 1),
          _buildInfoTile(
            'Max Current',
            '${maxCurrent.toStringAsFixed(0)} A',
            Icons.electrical_services,
          ),
          const Divider(height: 1),
          _buildInfoTile(
            'Max Power',
            '${maxPower.toStringAsFixed(0)} W (PF ${pf.toStringAsFixed(2)})',
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

  void _showDeviceScanDialog(BuildContext context) {
    final provider = context.read<EnergyProvider>();
    final scaffoldContext = context;

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

                              if (scaffoldContext.mounted) {
                                final errorMessage =
                                    provider.connectionError.isNotEmpty
                                        ? provider.connectionError
                                        : 'Failed to connect';
                                ScaffoldMessenger.of(scaffoldContext)
                                    .showSnackBar(
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

                      if (scaffoldContext.mounted) {
                        final errorMessage = provider.connectionError.isNotEmpty
                            ? provider.connectionError
                            : 'Failed to connect';
                        ScaffoldMessenger.of(scaffoldContext).showSnackBar(
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
