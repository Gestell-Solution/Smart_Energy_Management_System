import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:intl/intl.dart';
import '../providers/energy_provider.dart';
import '../models/alert.dart';
import '../config/theme.dart';

class AlertsScreen extends StatelessWidget {
  const AlertsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Alerts & Notifications'),
        centerTitle: true,
        actions: [
          Consumer<EnergyProvider>(
            builder: (context, provider, _) {
              if (provider.alerts.isEmpty) return const SizedBox();

              return IconButton(
                icon: const Icon(Icons.clear_all),
                tooltip: 'Clear all alerts',
                onPressed: () {
                  _showClearDialog(context);
                },
              );
            },
          ),
        ],
      ),
      body: Consumer<EnergyProvider>(
        builder: (context, provider, _) {
          if (provider.alerts.isEmpty) {
            return _buildEmptyState();
          }

          return Column(
            children: [
              // Unread count banner
              if (provider.unreadAlertsCount > 0)
                Container(
                  width: double.infinity,
                  padding: const EdgeInsets.all(12),
                  color: const Color(0x1A6366F1),
                  child: Text(
                    '${provider.unreadAlertsCount} unread alert${provider.unreadAlertsCount > 1 ? 's' : ''}',
                    textAlign: TextAlign.center,
                    style: const TextStyle(
                      fontWeight: FontWeight.w600,
                      color: AppTheme.primaryColor,
                    ),
                  ),
                ),

              // Alerts list
              Expanded(
                child: ListView.builder(
                  padding: const EdgeInsets.all(16),
                  itemCount: provider.alerts.length,
                  itemBuilder: (context, index) {
                    final alert = provider.alerts[index];
                    return _buildAlertCard(context, alert, provider);
                  },
                ),
              ),
            ],
          );
        },
      ),
    );
  }

  Widget _buildEmptyState() {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(
            Icons.notifications_off,
            size: 80,
            color: Colors.grey[400],
          ),
          const SizedBox(height: 24),
          Text(
            'No Alerts',
            style: TextStyle(
              fontSize: 24,
              fontWeight: FontWeight.bold,
              color: Colors.grey[600],
            ),
          ),
          const SizedBox(height: 8),
          Text(
            'You\'re all caught up!',
            style: TextStyle(
              fontSize: 16,
              color: Colors.grey[500],
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildAlertCard(
      BuildContext context, Alert alert, EnergyProvider provider) {
    return Dismissible(
      key: Key(alert.id),
      direction: DismissDirection.endToStart,
      background: Container(
        alignment: Alignment.centerRight,
        padding: const EdgeInsets.only(right: 20),
        margin: const EdgeInsets.only(bottom: 12),
        decoration: BoxDecoration(
          color: AppTheme.errorColor,
          borderRadius: BorderRadius.circular(12),
        ),
        child: const Icon(
          Icons.delete,
          color: Colors.white,
        ),
      ),
      onDismissed: (direction) {
        // Remove alert (this would need implementation in provider)
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: const Text('Alert dismissed'),
            action: SnackBarAction(
              label: 'Undo',
              onPressed: () {
                // Undo functionality
              },
            ),
          ),
        );
      },
      child: Card(
        margin: const EdgeInsets.only(bottom: 12),
        elevation: alert.isRead ? 0 : 2,
        child: InkWell(
          onTap: () {
            if (!alert.isRead) {
              provider.markAlertAsRead(alert.id);
            }
          },
          borderRadius: BorderRadius.circular(12),
          child: Container(
            padding: const EdgeInsets.all(16),
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(12),
              border: Border.all(
                color: alert.isRead
                    ? Colors.transparent
                    : Color(alert.color).withValues(alpha: 0.3),
                width: 2,
              ),
            ),
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                // Alert Icon
                Container(
                  width: 48,
                  height: 48,
                  decoration: BoxDecoration(
                    color: Color(alert.color).withValues(alpha: 0.1),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  child: Center(
                    child: _buildAlertIcon(alert),
                  ),
                ),
                const SizedBox(width: 16),

                // Alert Content
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Row(
                        children: [
                          Expanded(
                            child: Text(
                              _getAlertTitle(alert.type),
                              style: TextStyle(
                                fontSize: 16,
                                fontWeight: FontWeight.bold,
                                color: Color(alert.color),
                              ),
                            ),
                          ),
                          if (!alert.isRead)
                            Container(
                              width: 8,
                              height: 8,
                              decoration: const BoxDecoration(
                                color: AppTheme.primaryColor,
                                shape: BoxShape.circle,
                              ),
                            ),
                        ],
                      ),
                      const SizedBox(height: 8),
                      Text(
                        alert.message,
                        style: Theme.of(context).textTheme.bodyMedium,
                      ),
                      const SizedBox(height: 8),
                      Text(
                        _formatTimestamp(alert.timestamp),
                        style: TextStyle(
                          fontSize: 12,
                          color: Colors.grey[600],
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildAlertIcon(Alert alert) {
    IconData icon;

    switch (alert.type) {
      case AlertType.overload:
        icon = Icons.flash_on;
        break;
      case AlertType.overcurrent:
        icon = Icons.electrical_services;
        break;
      case AlertType.overvoltage:
        icon = Icons.warning;
        break;
      case AlertType.systemError:
        icon = Icons.error;
        break;
      case AlertType.connectionLost:
        icon = Icons.bluetooth_disabled;
        break;
      case AlertType.info:
        icon = Icons.info;
        break;
    }

    return Icon(
      icon,
      color: Color(alert.color),
      size: 24,
    );
  }

  String _getAlertTitle(AlertType type) {
    switch (type) {
      case AlertType.overload:
        return 'Power Overload';
      case AlertType.overcurrent:
        return 'Overcurrent';
      case AlertType.overvoltage:
        return 'High Voltage';
      case AlertType.systemError:
        return 'System Error';
      case AlertType.connectionLost:
        return 'Connection Lost';
      case AlertType.info:
        return 'Information';
    }
  }

  String _formatTimestamp(DateTime timestamp) {
    final now = DateTime.now();
    final difference = now.difference(timestamp);

    if (difference.inMinutes < 1) {
      return 'Just now';
    } else if (difference.inHours < 1) {
      return '${difference.inMinutes} min ago';
    } else if (difference.inDays < 1) {
      return '${difference.inHours} hour${difference.inHours > 1 ? 's' : ''} ago';
    } else if (difference.inDays < 7) {
      return '${difference.inDays} day${difference.inDays > 1 ? 's' : ''} ago';
    } else {
      return DateFormat('MMM dd, yyyy').format(timestamp);
    }
  }

  void _showClearDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Clear All Alerts'),
        content: const Text(
            'Are you sure you want to clear all alerts? This action cannot be undone.'),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () {
              context.read<EnergyProvider>().clearAllAlerts();
              Navigator.pop(context);
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(content: Text('All alerts cleared')),
              );
            },
            style: ElevatedButton.styleFrom(
              backgroundColor: AppTheme.errorColor,
            ),
            child: const Text('Clear All'),
          ),
        ],
      ),
    );
  }
}
