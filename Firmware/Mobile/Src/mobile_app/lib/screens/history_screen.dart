import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:intl/intl.dart';
import '../providers/energy_provider.dart';
import '../config/theme.dart';

class HistoryScreen extends StatefulWidget {
  const HistoryScreen({super.key});

  @override
  State<HistoryScreen> createState() => _HistoryScreenState();
}

class _HistoryScreenState extends State<HistoryScreen> {
  int _selectedPeriod = 0; // 0: Day, 1: Week, 2: Month

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Energy History'),
        centerTitle: true,
      ),
      body: Consumer<EnergyProvider>(
        builder: (context, provider, _) {
          final history = _getFilteredHistory(provider);

          return SingleChildScrollView(
            padding: const EdgeInsets.all(16),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                // Period Selector
                _buildPeriodSelector(),
                const SizedBox(height: 24),

                // Statistics Cards
                _buildStatisticsCards(history),
                const SizedBox(height: 24),

                // Power Chart
                Text(
                  'Power Consumption',
                  style: Theme.of(context).textTheme.titleLarge,
                ),
                const SizedBox(height: 16),
                _buildPowerChart(history),
                const SizedBox(height: 32),

                // Energy Chart
                Text(
                  'Energy Usage',
                  style: Theme.of(context).textTheme.titleLarge,
                ),
                const SizedBox(height: 16),
                _buildEnergyChart(history),
                const SizedBox(height: 24),
              ],
            ),
          );
        },
      ),
    );
  }

  List<dynamic> _getFilteredHistory(EnergyProvider provider) {
    final now = DateTime.now();
    DateTime start;

    switch (_selectedPeriod) {
      case 0: // Today
        start = DateTime(now.year, now.month, now.day);
        break;
      case 1: // Week
        start = now.subtract(const Duration(days: 7));
        break;
      case 2: // Month
        start = now.subtract(const Duration(days: 30));
        break;
      default:
        start = DateTime(now.year, now.month, now.day);
    }

    return provider.getHistoryByDateRange(start, now);
  }

  Widget _buildPeriodSelector() {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(8),
        child: Row(
          children: [
            Expanded(
              child: _buildPeriodButton('Today', 0),
            ),
            const SizedBox(width: 8),
            Expanded(
              child: _buildPeriodButton('Week', 1),
            ),
            const SizedBox(width: 8),
            Expanded(
              child: _buildPeriodButton('Month', 2),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildPeriodButton(String label, int index) {
    final isSelected = _selectedPeriod == index;

    return InkWell(
      onTap: () {
        setState(() {
          _selectedPeriod = index;
        });
      },
      child: Container(
        padding: const EdgeInsets.symmetric(vertical: 12),
        decoration: BoxDecoration(
          color: isSelected ? AppTheme.primaryColor : Colors.transparent,
          borderRadius: BorderRadius.circular(8),
        ),
        child: Text(
          label,
          textAlign: TextAlign.center,
          style: TextStyle(
            fontWeight: FontWeight.w600,
            color: isSelected ? Colors.white : Colors.grey[600],
          ),
        ),
      ),
    );
  }

  Widget _buildStatisticsCards(List<dynamic> history) {
    double avgPower = 0;
    double maxPower = 0;
    double totalEnergy = 0;

    if (history.isNotEmpty) {
      avgPower =
          history.map((e) => e.power).reduce((a, b) => a + b) / history.length;
      maxPower = history.map((e) => e.power).reduce((a, b) => a > b ? a : b);
      totalEnergy = history.isNotEmpty ? history.last.energy : 0;
    }

    return Row(
      children: [
        Expanded(
          child: _buildStatCard(
            'Average',
            '${avgPower.toStringAsFixed(0)} W',
            Icons.analytics,
            AppTheme.primaryColor,
          ),
        ),
        const SizedBox(width: 12),
        Expanded(
          child: _buildStatCard(
            'Peak',
            '${maxPower.toStringAsFixed(0)} W',
            Icons.trending_up,
            AppTheme.warningColor,
          ),
        ),
        const SizedBox(width: 12),
        Expanded(
          child: _buildStatCard(
            'Total',
            '${totalEnergy.toStringAsFixed(2)} kWh',
            Icons.energy_savings_leaf,
            AppTheme.successColor,
          ),
        ),
      ],
    );
  }

  Widget _buildStatCard(
      String label, String value, IconData icon, Color color) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          children: [
            Icon(icon, color: color, size: 28),
            const SizedBox(height: 8),
            Text(
              label,
              style: TextStyle(
                fontSize: 12,
                color: Colors.grey[600],
              ),
            ),
            const SizedBox(height: 4),
            Text(
              value,
              style: const TextStyle(
                fontSize: 14,
                fontWeight: FontWeight.bold,
              ),
              textAlign: TextAlign.center,
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildPowerChart(List<dynamic> history) {
    if (history.isEmpty) {
      return _buildNoDataCard();
    }

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: SizedBox(
          height: 250,
          child: LineChart(
            LineChartData(
              gridData: const FlGridData(show: true),
              titlesData: FlTitlesData(
                leftTitles: AxisTitles(
                  sideTitles: SideTitles(
                    showTitles: true,
                    reservedSize: 40,
                    getTitlesWidget: (value, meta) {
                      return Text(
                        '${value.toInt()}W',
                        style: const TextStyle(fontSize: 10),
                      );
                    },
                  ),
                ),
                bottomTitles: AxisTitles(
                  sideTitles: SideTitles(
                    showTitles: true,
                    reservedSize: 30,
                    getTitlesWidget: (value, meta) {
                      if (value.toInt() >= 0 &&
                          value.toInt() < history.length) {
                        final time = history[value.toInt()].timestamp;
                        return Text(
                          DateFormat('HH:mm').format(time),
                          style: const TextStyle(fontSize: 10),
                        );
                      }
                      return const Text('');
                    },
                  ),
                ),
                rightTitles: const AxisTitles(
                  sideTitles: SideTitles(showTitles: false),
                ),
                topTitles: const AxisTitles(
                  sideTitles: SideTitles(showTitles: false),
                ),
              ),
              borderData: FlBorderData(show: false),
              lineBarsData: [
                LineChartBarData(
                  spots: history.asMap().entries.map((entry) {
                    return FlSpot(
                      entry.key.toDouble(),
                      entry.value.power,
                    );
                  }).toList(),
                  isCurved: true,
                  color: AppTheme.primaryColor,
                  barWidth: 3,
                  dotData: const FlDotData(show: false),
                  belowBarData: BarAreaData(
                    show: true,
                    color: const Color(0x1A6366F1),
                  ),
                ),
              ],
              minY: 0,
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildEnergyChart(List<dynamic> history) {
    if (history.isEmpty) {
      return _buildNoDataCard();
    }

    // Group data by hour or day depending on period
    final groupedData = _groupDataForBarChart(history);

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: SizedBox(
          height: 250,
          child: BarChart(
            BarChartData(
              alignment: BarChartAlignment.spaceAround,
              gridData: const FlGridData(show: true),
              titlesData: FlTitlesData(
                leftTitles: AxisTitles(
                  sideTitles: SideTitles(
                    showTitles: true,
                    reservedSize: 40,
                    getTitlesWidget: (value, meta) {
                      return Text(
                        value.toStringAsFixed(1),
                        style: const TextStyle(fontSize: 10),
                      );
                    },
                  ),
                ),
                bottomTitles: AxisTitles(
                  sideTitles: SideTitles(
                    showTitles: true,
                    getTitlesWidget: (value, meta) {
                      final index = value.toInt();
                      if (index >= 0 && index < groupedData.length) {
                        return Text(
                          groupedData[index]['label'],
                          style: const TextStyle(fontSize: 10),
                        );
                      }
                      return const Text('');
                    },
                  ),
                ),
                rightTitles: const AxisTitles(
                  sideTitles: SideTitles(showTitles: false),
                ),
                topTitles: const AxisTitles(
                  sideTitles: SideTitles(showTitles: false),
                ),
              ),
              borderData: FlBorderData(show: false),
              barGroups: groupedData.asMap().entries.map((entry) {
                return BarChartGroupData(
                  x: entry.key,
                  barRods: [
                    BarChartRodData(
                      toY: entry.value['value'],
                      color: AppTheme.successColor,
                      width: 20,
                      borderRadius: const BorderRadius.vertical(
                        top: Radius.circular(4),
                      ),
                    ),
                  ],
                );
              }).toList(),
            ),
          ),
        ),
      ),
    );
  }

  List<Map<String, dynamic>> _groupDataForBarChart(List<dynamic> history) {
    if (history.isEmpty) return [];

    // For simplicity, show last 10 data points
    final step = history.length > 10 ? history.length ~/ 10 : 1;
    final result = <Map<String, dynamic>>[];

    for (var i = 0; i < history.length; i += step) {
      final data = history[i];
      result.add({
        'label': DateFormat('HH:mm').format(data.timestamp),
        'value': data.energy,
      });
    }

    return result.take(10).toList();
  }

  Widget _buildNoDataCard() {
    return Card(
      child: Container(
        height: 250,
        alignment: Alignment.center,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(
              Icons.bar_chart,
              size: 64,
              color: Colors.grey[400],
            ),
            const SizedBox(height: 16),
            Text(
              'No data available',
              style: TextStyle(
                fontSize: 16,
                color: Colors.grey[600],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
