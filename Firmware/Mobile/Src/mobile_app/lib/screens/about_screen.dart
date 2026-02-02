import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';
import '../config/constants.dart';
import '../config/theme.dart';
import 'alerts_screen.dart';

class AboutScreen extends StatelessWidget {
  const AboutScreen({super.key});

  Future<void> _openUrl(BuildContext context, Uri uri) async {
    final ok = await launchUrl(uri, mode: LaunchMode.externalApplication);
    if (!ok && context.mounted) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Could not open link')),
      );
    }
  }

  Future<void> _openEmail(BuildContext context, String email) async {
    await _openUrl(context, Uri(scheme: 'mailto', path: email));
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    final projectHighlights = <String>[
      'Real-time monitoring of Voltage, Current, Power, and Energy',
      'Bluetooth Classic connection for local device communication',
      'Data refresh every 1 second when connected',
      'Alerts detection on mobile: OVERLOAD / OVERVOLT / HIGHPOWER',
      'Relay control and energy reset commands',
    ];

    final embeddedTeam = <String>[
      'Ahmed Ashraf',
      'Ahmed Twap',
      'Basma Khaled',
      'Mohamed Abdelgaber',
      'Mohammed Diaa',
    ]..sort();

    final mobileTeam = <String>[
      'Aya Mohamed',
      'Salma Tarek',
    ]..sort();

    return Scaffold(
      appBar: AppBar(
        title: const Text('About'),
        centerTitle: true,
        actions: [
          IconButton(
            icon: Icon(Icons.notifications_outlined, color: theme.iconTheme.color),
            onPressed: () {
              Navigator.of(context).push(
                MaterialPageRoute(builder: (_) => const AlertsScreen()),
              );
            },
          ),
          const SizedBox(width: 8),
        ],
      ),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          Card(
            child: Column(
              children: [
                ListTile(
                  leading: Container(
                    padding: const EdgeInsets.all(10),
                    decoration: BoxDecoration(
                      color: AppTheme.primaryColor.withOpacity(0.1),
                      borderRadius: BorderRadius.circular(12),
                    ),
                    child: const Icon(
                      Icons.energy_savings_leaf,
                      color: AppTheme.primaryColor,
                    ),
                  ),
                  title: Text(
                    AppConstants.appName,
                    style: const TextStyle(fontWeight: FontWeight.w600),
                  ),
                  subtitle: const Text('Real-time Energy Monitoring & Control'),
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
                const Divider(height: 1),
                const ListTile(
                  leading: Icon(Icons.update, color: AppTheme.primaryColor),
                  title: Text(
                    'Version',
                    style: TextStyle(fontWeight: FontWeight.w600),
                  ),
                  subtitle: Text(
                    '${AppConstants.appVersion} (Build ${AppConstants.buildNumber})\n${AppConstants.buildDate}',
                  ),
                ),
              ],
            ),
          ),
          const SizedBox(height: 16),
          Card(
            child: Padding(
              padding: const EdgeInsets.fromLTRB(16, 12, 16, 16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Row(
                    children: [
                      const Icon(Icons.description_outlined,
                          color: AppTheme.primaryColor),
                      const SizedBox(width: 10),
                      Expanded(
                        child: Text(
                          'Project Overview',
                          style: theme.textTheme.titleMedium?.copyWith(
                            fontWeight: FontWeight.w700,
                          ),
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(height: 12),
                  Text(
                    'Smart Energy Management System is an end-to-end solution for monitoring and controlling electrical loads. The mobile app connects to the embedded device and provides real-time metrics, history, and alerts.',
                    style: theme.textTheme.bodyMedium,
                    textAlign: TextAlign.start,
                  ),
                  const SizedBox(height: 16),
                  Text(
                    'Key Capabilities',
                    style: theme.textTheme.titleSmall?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: AppTheme.primaryColor,
                    ),
                  ),
                  const SizedBox(height: 10),
                  ...projectHighlights.map(
                    (t) => Padding(
                      padding: const EdgeInsets.only(bottom: 8),
                      child: Row(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: const EdgeInsets.only(top: 6),
                            child: Container(
                              width: 6,
                              height: 6,
                              decoration: const BoxDecoration(
                                color: AppTheme.primaryColor,
                                shape: BoxShape.circle,
                              ),
                            ),
                          ),
                          const SizedBox(width: 10),
                          Expanded(child: Text(t)),
                        ],
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ),
          const SizedBox(height: 16),
          Card(
            child: Column(
              children: [
                ListTile(
                  leading: const Icon(Icons.contact_mail, color: AppTheme.primaryColor),
                  title: const Text(
                    'Contact',
                    style: TextStyle(fontWeight: FontWeight.w600),
                  ),
                  subtitle: const Text('Reach Gestell for support and inquiries'),
                ),
                const Divider(height: 1),
                ListTile(
                  leading: const Icon(Icons.email_outlined, color: AppTheme.primaryColor),
                  title: const Text('Email'),
                  subtitle: const Text('info.gestell@gmail.com'),
                  onTap: () => _openEmail(context, 'info.gestell@gmail.com'),
                ),
                const Divider(height: 1),
                ListTile(
                  leading: const Icon(Icons.code, color: AppTheme.primaryColor),
                  title: const Text('GitHub'),
                  subtitle: const Text('github.com/Gestell-Solution'),
                  onTap: () => _openUrl(
                    context,
                    Uri.parse('https://github.com/Gestell-Solution'),
                  ),
                ),
                const Divider(height: 1),
                ListTile(
                  leading: const Icon(Icons.link, color: AppTheme.primaryColor),
                  title: const Text('LinkedIn'),
                  subtitle: const Text('linkedin.com/company/gestell-company'),
                  onTap: () => _openUrl(
                    context,
                    Uri.parse('https://www.linkedin.com/company/gestell-company/'),
                  ),
                ),
              ],
            ),
          ),
          const SizedBox(height: 16),
          Card(
            child: Padding(
              padding: const EdgeInsets.fromLTRB(16, 12, 16, 16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Row(
                    children: [
                      const Icon(Icons.group, color: AppTheme.primaryColor),
                      const SizedBox(width: 10),
                      Expanded(
                        child: Text(
                          'Development Team',
                          style: theme.textTheme.titleMedium?.copyWith(
                            fontWeight: FontWeight.w700,
                          ),
                          textAlign: TextAlign.start,
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(height: 14),
                  Text(
                    'Managers',
                    style: theme.textTheme.titleSmall?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: AppTheme.primaryColor,
                    ),
                    textAlign: TextAlign.start,
                  ),
                  const SizedBox(height: 10),
                  const Padding(
                    padding: EdgeInsets.only(left: 8),
                    child: Align(
                      alignment: Alignment.centerLeft,
                      child: Text('Hesham Ahmed (Project Manager)'),
                    ),
                  ),
                  const SizedBox(height: 18),
                  Text(
                    'Embedded',
                    style: theme.textTheme.titleSmall?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: AppTheme.primaryColor,
                    ),
                    textAlign: TextAlign.start,
                  ),
                  const SizedBox(height: 10),
                  Padding(
                    padding: const EdgeInsets.only(left: 8),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: embeddedTeam
                          .map(
                            (name) => Padding(
                              padding: const EdgeInsets.only(bottom: 8),
                              child: Align(
                                alignment: Alignment.centerLeft,
                                child: Text(name),
                              ),
                            ),
                          )
                          .toList(),
                    ),
                  ),
                  const SizedBox(height: 18),
                  Text(
                    'Mobile',
                    style: theme.textTheme.titleSmall?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: AppTheme.primaryColor,
                    ),
                    textAlign: TextAlign.start,
                  ),
                  const SizedBox(height: 10),
                  Padding(
                    padding: const EdgeInsets.only(left: 8),
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: mobileTeam
                          .map(
                            (name) => Padding(
                              padding: const EdgeInsets.only(bottom: 8),
                              child: Align(
                                alignment: Alignment.centerLeft,
                                child: Text(name),
                              ),
                            ),
                          )
                          .toList(),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }
}
