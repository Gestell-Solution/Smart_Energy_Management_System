import 'package:flutter/material.dart';
import '../services/storage_service.dart';

class ThemeProvider with ChangeNotifier {
  final StorageService _storageService = StorageService();

  bool _isDarkMode = false;

  bool get isDarkMode => _isDarkMode;
  ThemeMode get themeMode => _isDarkMode ? ThemeMode.dark : ThemeMode.light;

  ThemeProvider() {
    _loadThemeMode();
  }

  Future<void> _loadThemeMode() async {
    await _storageService.initialize();
    _isDarkMode = _storageService.getThemeMode();
    notifyListeners();
  }

  Future<void> toggleTheme() async {
    _isDarkMode = !_isDarkMode;
    await _storageService.setThemeMode(_isDarkMode);
    notifyListeners();
  }

  Future<void> setThemeMode(bool isDark) async {
    _isDarkMode = isDark;
    await _storageService.setThemeMode(isDark);
    notifyListeners();
  }
}
