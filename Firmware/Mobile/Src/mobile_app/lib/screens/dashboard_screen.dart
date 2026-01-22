import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';
import 'dart:io';
import 'dart:ui';
import 'package:csv/csv.dart';
import 'package:path_provider/path_provider.dart';
import 'package:share_plus/share_plus.dart';
import '../providers/energy_provider.dart';
import '../providers/theme_provider.dart';
import '../config/theme.dart';
import '../database/energy_database.dart';
import 'history_screen.dart';
import 'alerts_screen.dart';
import 'settings_screen.dart';

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  int _selectedIndex = 0;

  final List<Widget> _screens = [
    const _DashboardView(),
    const HistoryScreen(),
    const AlertsScreen(),
    const SettingsScreen(),
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      extendBody: true, // For transparency behind navbar
      body: _screens[_selectedIndex],
      bottomNavigationBar: Container(
        decoration: BoxDecoration(
          color: AppTheme.secondaryColor.withOpacity(0.95),
          border: Border(
            top: BorderSide(
              color: AppTheme.primaryColor.withOpacity(0.2),
              width: 1,
            ),
          ),
          boxShadow: [
            BoxShadow(
              color: Colors.black.withOpacity(0.3),
              blurRadius: 20,
              offset: const Offset(0, -5),
            ),
          ],
        ),
        child: ClipRRect(
          child: BackdropFilter(
            filter: ImageFilter.blur(sigmaX: 10, sigmaY: 10),
            child: BottomNavigationBar(
              currentIndex: _selectedIndex,
              onTap: (index) {
                setState(() {
                  _selectedIndex = index;
                });
              },
              type: BottomNavigationBarType.fixed,
              backgroundColor: Colors.transparent,
              elevation: 0,
              selectedItemColor: AppTheme.primaryColor,
              unselectedItemColor: AppTheme.textLight,
              selectedLabelStyle: const TextStyle(fontWeight: FontWeight.bold),
              items: const [
                BottomNavigationBarItem(
                  icon: Icon(Icons.dashboard_rounded),
                  label: 'Dashboard',
                ),
                BottomNavigationBarItem(
                  icon: Icon(Icons.history_rounded),
                  label: 'History',
                ),
                BottomNavigationBarItem(
                  icon: Icon(Icons.notifications_active_rounded),
                  label: 'Alerts',
                ),
                BottomNavigationBarItem(
                  icon: Icon(Icons.settings_rounded),
                  label: 'Settings',
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class _DashboardView extends StatelessWidget {
  const _DashboardView();

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final isDark = context.watch<ThemeProvider>().isDarkMode;

    return Container(
      decoration: BoxDecoration(
        color: isDark ? null : AppTheme.background,
      ),
      child: SafeArea(
        bottom: false,
        child: CustomScrollView(
          physics: const BouncingScrollPhysics(),
          slivers: [
            // App Bar
            SliverAppBar(
              expandedHeight: 140,
              floating: false,
              pinned: true,
              backgroundColor: Colors.transparent,
              elevation: 0,
              flexibleSpace: FlexibleSpaceBar(
                title: Text(
                  'Energy Dashboard',
                  style: Theme.of(context).appBarTheme.titleTextStyle,
                ),
                centerTitle: true,
                background: Container(
                  decoration: BoxDecoration(
                    gradient: LinearGradient(
                      colors: [
                        AppTheme.primaryColor.withOpacity(0.1),
                        Colors.transparent,
                      ],
                      begin: Alignment.topCenter,
                      end: Alignment.bottomCenter,
                    ),
                  ),
                ),
              ),
              actions: [
                _buildConnectionBadge(context),
                IconButton(
                  icon: Icon(Icons.notifications_outlined,
                      color: theme.iconTheme.color),
                  onPressed: () => Navigator.pushNamed(context, '/alerts'),
                ),
                const SizedBox(width: 8),
              ],
            ),

            SliverPadding(
              padding: const EdgeInsets.all(16),
              sliver: SliverList(
                delegate: SliverChildListDelegate([
                  // Welcome / Status Section
                  _buildGreetingSection(context),
                  const SizedBox(height: 24),

                  // Key Metrics Grid
                  const Text("Live Metrics",
                      style:
                          TextStyle(fontSize: 18, fontWeight: FontWeight.w600)),
                  const SizedBox(height: 12),

                  Consumer<EnergyProvider>(
                    builder: (context, provider, _) {
                      final data = provider.currentData;
                      // Calculate PF safely
                      double pf = 0.0;
                      if (data != null &&
                          data.voltage > 0 &&
                          data.current > 0) {
                        pf = data.power / (data.voltage * data.current);
                        if (pf > 1.0) pf = 1.0; // Cap at 1.0
                      }

                      return Column(
                        children: [
                          // Main Gauges Row
                          Row(
                            children: [
                              Expanded(
                                child: _buildMinimalGauge(
                                    context,
                                    'Voltage',
                                    data?.voltage ?? 0.0,
                                    'V',
                                    260,
                                    AppTheme.primaryColor),
                              ),
                              const SizedBox(width: 16),
                              Expanded(
                                child: _buildMinimalGauge(
                                    context,
                                    'Current',
                                    data?.current ?? 0.0,
                                    'A',
                                    30,
                                    const Color(0xFF00C853)),
                              ),
                            ],
                          ),
                          const SizedBox(height: 16),

                          // Secondary Metrics Grid
                          Row(
                            children: [
                              Expanded(
                                child: _buildInfoCard(
                                  context,
                                  'Power',
                                  '${(data?.power ?? 0.0).toStringAsFixed(1)} kW',
                                  Icons.bolt_rounded,
                                  AppTheme.accentColor,
                                ),
                              ),
                              const SizedBox(width: 16),
                              Expanded(
                                child: _buildInfoCard(
                                  context,
                                  'Energy',
                                  '${(data?.energy ?? 0.0).toStringAsFixed(1)} kWh',
                                  Icons.electric_meter_rounded,
                                  AppTheme.success,
                                ),
                              ),
                            ],
                          ),
                          const SizedBox(height: 16),
                          Row(
                            children: [
                              Expanded(
                                child: _buildInfoCard(
                                  context,
                                  'Power Factor',
                                  pf.toStringAsFixed(2),
                                  Icons.pie_chart_rounded,
                                  const Color(0xFF9C27B0),
                                ),
                              ),
                              const SizedBox(width: 16),
                              Expanded(
                                child: _buildInfoCard(
                                  context,
                                  'Status',
                                  data?.status ?? 'N/A',
                                  Icons.info_outline_rounded,
                                  AppTheme.primaryColor,
                                ),
                              ),
                            ],
                          ),
                        ],
                      );
                    },
                  ),

                  const SizedBox(height: 24),
                  const Text("Quick Controls",
                      style:
                          TextStyle(fontSize: 18, fontWeight: FontWeight.w600)),
                  const SizedBox(height: 12),
                  _buildRelaysSection(context),

                  const SizedBox(height: 24),
                  const Text("Estimated Cost",
                      style:
                          TextStyle(fontSize: 18, fontWeight: FontWeight.w600)),
                  const SizedBox(height: 12),
                  _buildCostCard(context),

                  const SizedBox(height: 80), // Bottom padding
                ]),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildGreetingSection(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(20),
      decoration: BoxDecoration(
        color: AppTheme.primaryColor,
        borderRadius: BorderRadius.circular(24),
        boxShadow: [
          BoxShadow(
            color: AppTheme.primaryColor.withOpacity(0.3),
            blurRadius: 15,
            offset: const Offset(0, 8),
          ),
        ],
      ),
      child: Row(
        children: [
          Container(
            padding: const EdgeInsets.all(12),
            decoration: BoxDecoration(
              color: Colors.white.withOpacity(0.2),
              shape: BoxShape.circle,
            ),
            child: const Icon(Icons.home_filled, color: Colors.white, size: 32),
          ),
          const SizedBox(width: 16),
          Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: const [
              Text(
                "System Online",
                style: TextStyle(
                  color: Colors.white,
                  fontSize: 18,
                  fontWeight: FontWeight.bold,
                ),
              ),
              Text(
                "Monitoring active",
                style: TextStyle(
                  color: Colors.white70,
                  fontSize: 14,
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }

  Widget _buildConnectionBadge(BuildContext context) {
    return Consumer<EnergyProvider>(
      builder: (context, provider, _) {
        final isConnected = provider.isConnected;
        return Container(
          margin: const EdgeInsets.symmetric(horizontal: 8, vertical: 12),
          padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
          decoration: BoxDecoration(
            color: isConnected
                ? AppTheme.success.withOpacity(0.1)
                : AppTheme.error.withOpacity(0.1),
            borderRadius: BorderRadius.circular(20),
            border: Border.all(
              color: isConnected ? AppTheme.success : AppTheme.error,
              width: 1,
            ),
          ),
          child: Row(
            mainAxisSize: MainAxisSize.min,
            children: [
              Container(
                width: 8,
                height: 8,
                decoration: BoxDecoration(
                  color: isConnected ? AppTheme.success : AppTheme.error,
                  shape: BoxShape.circle,
                ),
              ),
              const SizedBox(width: 8),
              Text(
                isConnected ? 'Online' : 'Offline',
                style: TextStyle(
                  color: isConnected ? AppTheme.success : AppTheme.error,
                  fontSize: 12,
                  fontWeight: FontWeight.bold,
                ),
              ),
            ],
          ),
        );
      },
    );
  }

  Widget _buildMinimalGauge(BuildContext context, String title, double value,
      String unit, double max, Color color) {
    final theme = Theme.of(context);
    return Container(
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: theme.cardTheme.color,
        borderRadius: BorderRadius.circular(24),
        boxShadow: [
          BoxShadow(
            color: Colors.black.withOpacity(0.05),
            blurRadius: 10,
            offset: const Offset(0, 4),
          ),
        ],
      ),
      child: Column(
        children: [
          SizedBox(
            height: 100,
            child: SfRadialGauge(
              axes: <RadialAxis>[
                RadialAxis(
                  minimum: 0,
                  maximum: max,
                  showLabels: false,
                  showTicks: false,
                  startAngle: 180,
                  endAngle: 0,
                  radiusFactor: 0.9,
                  canScaleToFit: true,
                  axisLineStyle: AxisLineStyle(
                    thickness: 12,
                    cornerStyle: CornerStyle.bothCurve,
                    color: theme.scaffoldBackgroundColor,
                  ),
                  pointers: <GaugePointer>[
                    RangePointer(
                      value: value,
                      width: 12,
                      color: color,
                      cornerStyle: CornerStyle.bothCurve,
                      enableAnimation: true,
                    ),
                  ],
                  annotations: <GaugeAnnotation>[
                    GaugeAnnotation(
                      angle: 90,
                      positionFactor: 0.1,
                      widget: Column(
                        mainAxisSize: MainAxisSize.min,
                        children: [
                          Text(
                            value.toStringAsFixed(1),
                            style: TextStyle(
                              fontSize: 24,
                              fontWeight: FontWeight.bold,
                              color: theme.textTheme.bodyLarge?.color,
                            ),
                          ),
                          Text(
                            unit,
                            style: TextStyle(
                              fontSize: 14,
                              color: theme.textTheme.bodyMedium?.color,
                            ),
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
          const SizedBox(height: 4),
          Text(title, style: theme.textTheme.bodyMedium),
        ],
      ),
    );
  }

  Widget _buildInfoCard(BuildContext context, String title, String value,
      IconData icon, Color iconColor) {
    final theme = Theme.of(context);
    return Container(
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: theme.cardTheme.color,
        borderRadius: BorderRadius.circular(24),
        boxShadow: [
          BoxShadow(
            color: Colors.black.withOpacity(0.05),
            blurRadius: 10,
            offset: const Offset(0, 4),
          ),
        ],
      ),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Container(
            padding: const EdgeInsets.all(8),
            decoration: BoxDecoration(
              color: iconColor.withOpacity(0.1),
              borderRadius: BorderRadius.circular(12),
            ),
            child: Icon(icon, color: iconColor, size: 24),
          ),
          const SizedBox(height: 12),
          Text(
            value,
            style: const TextStyle(
              fontSize: 20,
              fontWeight: FontWeight.bold,
            ),
          ),
          Text(
            title,
            style: theme.textTheme.bodyMedium,
          ),
        ],
      ),
    );
  }

  Widget _buildRelaysSection(BuildContext context) {
    return Consumer<EnergyProvider>(
      builder: (context, provider, _) {
        final relays = provider.relayStates;
        return Column(
          children: [
            // First Row
            Row(
              children: [
                Expanded(
                    child: _buildRelayToggle(context, 'Load 1', relays[0],
                        (v) => provider.toggleRelay(0))),
                const SizedBox(width: 16),
                Expanded(
                    child: _buildRelayToggle(context, 'Load 2', relays[1],
                        (v) => provider.toggleRelay(1))),
              ],
            ),
            const SizedBox(height: 16),
            // Second Row
            Row(
              children: [
                Expanded(
                    child: _buildRelayToggle(context, 'Load 3', relays[2],
                        (v) => provider.toggleRelay(2))),
                const SizedBox(width: 16),
                Expanded(
                    child: _buildRelayToggle(context, 'Load 4', relays[3],
                        (v) => provider.toggleRelay(3))),
              ],
            ),
          ],
        );
      },
    );
  }

  Widget _buildRelayToggle(BuildContext context, String label, bool isActive,
      Function(bool) onChanged) {
    return InkWell(
      onTap: () => onChanged(!isActive),
      borderRadius: BorderRadius.circular(24),
      child: AnimatedContainer(
        duration: const Duration(milliseconds: 300),
        padding: const EdgeInsets.all(16),
        decoration: BoxDecoration(
          color: isActive
              ? AppTheme.primaryColor
              : Theme.of(context).cardTheme.color,
          borderRadius: BorderRadius.circular(24),
          boxShadow: [
            BoxShadow(
              color: isActive
                  ? AppTheme.primaryColor.withOpacity(0.3)
                  : Colors.black.withOpacity(0.05),
              blurRadius: 10,
              offset: const Offset(0, 4),
            ),
          ],
        ),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Text(
              label,
              style: TextStyle(
                color: isActive
                    ? Colors.white
                    : Theme.of(context).textTheme.bodyLarge?.color,
                fontWeight: FontWeight.bold,
              ),
            ),
            Icon(
              isActive
                  ? Icons.power_settings_new_rounded
                  : Icons.circle_outlined,
              color: isActive ? Colors.white : Colors.grey,
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildCostCard(BuildContext context) {
    return Consumer<EnergyProvider>(
      builder: (context, provider, _) {
        final cost = provider.currentCost;
        return Container(
          padding: const EdgeInsets.all(20),
          decoration: BoxDecoration(
            color: Colors.white,
            borderRadius: BorderRadius.circular(24),
            border: Border.all(color: AppTheme.success.withOpacity(0.3)),
          ),
          child: Row(
            children: [
              Container(
                padding: const EdgeInsets.all(12),
                decoration: BoxDecoration(
                  color: AppTheme.success.withOpacity(0.1),
                  borderRadius: BorderRadius.circular(16),
                ),
                child: const Icon(Icons.attach_money_rounded,
                    color: AppTheme.success, size: 32),
              ),
              const SizedBox(width: 16),
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const Text(
                    "Total Cost",
                    style: TextStyle(
                      fontSize: 14,
                      color: Colors.grey,
                    ),
                  ),
                  Text(
                    "\$${cost.toStringAsFixed(2)}",
                    style: const TextStyle(
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                      color: AppTheme.success,
                    ),
                  ),
                ],
              ),
              const Spacer(),
              TextButton(
                onPressed: () => _showResetDialog(context),
                child: const Text("Reset"),
              )
            ],
          ),
        );
      },
    );
  }

  void _showResetDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (ctx) => AlertDialog(
        title: const Text('Reset Energy Data?'),
        content: const Text(
            'This will reset all accumulated energy and cost counters.'),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(24)),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(ctx).pop(),
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () {
              Provider.of<EnergyProvider>(context, listen: false)
                  .resetEnergyCounter();
              Navigator.of(ctx).pop();
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(content: Text('Energy data reset successfully')),
              );
            },
            child: const Text('Reset'),
          ),
        ],
      ),
    );
  }
}
