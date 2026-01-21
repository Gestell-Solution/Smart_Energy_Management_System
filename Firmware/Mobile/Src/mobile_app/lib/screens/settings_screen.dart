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
                        ? const Color(0x1A10B981)
                        : const Color(0x1AEF4444),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  child: Icon(
                    provider.isConnected
                        ? Icons.bluetooth_connected
                        : Icons.bluetooth_disabled,
                    color: provider.isConnected
                        ? AppTheme.successColor
                        : AppTheme.errorColor,
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
                          backgroundColor: AppTheme.errorColor,
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
                color: const Color(0x1A6366F1),
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
                color: const Color(0x1AF59E0B),
                borderRadius: BorderRadius.circular(12),
              ),
              child: const Icon(
                Icons.attach_money,
                color: AppTheme.warningColor,
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
          ListTile(
            leading: Container(
              padding: const EdgeInsets.all(10),
              decoration: BoxDecoration(
                color: const Color(0x1A3B82F6),
                borderRadius: BorderRadius.circular(12),
              ),
              child: const Icon(
                Icons.info,
                color: AppTheme.infoColor,
              ),
            ),
            title: const Text(
              'App Version',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle: const Text(AppConstants.appVersion),
          ),
          const Divider(height: 1),
          const ListTile(
            leading: Icon(Icons.business, color: AppTheme.primaryColor),
            title: Text(
              'Developed by',
              style: TextStyle(fontWeight: FontWeight.w600),
            ),
            subtitle: Text(AppConstants.companyName),
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
          height: 300,
          child: Consumer<EnergyProvider>(
            builder: (context, provider, _) {
              if (provider.isScanning) {
                return const Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      CircularProgressIndicator(),
                      SizedBox(height: 16),
                      Text('Scanning for devices...'),
                    ],
                  ),
                );
              }

              if (provider.availableDevices.isEmpty) {
                return const Center(
                  child: Text('No devices found'),
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
                        ScaffoldMessenger.of(context).showSnackBar(
                          SnackBar(
                            content: Text(
                              success
                                  ? 'Connected to ${device.name}'
                                  : 'Failed to connect',
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
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () {
              provider.scanForDevices();
            },
            child: const Text('Scan'),
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
