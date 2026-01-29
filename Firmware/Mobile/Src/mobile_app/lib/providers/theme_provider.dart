import 'package:flutter/material.dart';
import '../services/storage_service.dart';

class ThemeProvider with ChangeNotifier {
  final StorageService _storageService = StorageService();

  bool _isDarkMode = false;
  bool _isInitialized = false;
  bool _isToggling = false; // Prevent concurrent toggles
  DateTime _lastToggleTime = DateTime.now();

  // Debounce duration to prevent rapid toggles
  static const Duration _debounceDuration = Duration(milliseconds: 300);

  bool get isDarkMode => _isDarkMode;
  ThemeMode get themeMode => _isDarkMode ? ThemeMode.dark : ThemeMode.light;
  bool get isInitialized => _isInitialized;

  ThemeProvider() {
    _loadThemeMode();
  }

  Future<void> _loadThemeMode() async {
    try {
      await _storageService.initialize();
      _isDarkMode = _storageService.getThemeMode();
      _isInitialized = true;
      notifyListeners();
      debugPrint(
          '[Theme] Loaded theme mode: ${_isDarkMode ? 'Dark' : 'Light'}');
    } catch (e) {
      // Silently fail with default light theme
      _isDarkMode = false;
      _isInitialized = true;
      notifyListeners();
      debugPrint(
          '[Theme] Error loading theme mode: $e - Using default light theme');
    }
  }

  Future<void> toggleTheme() async {
    // Debounce rapid toggles
    final now = DateTime.now();
    if (now.difference(_lastToggleTime) < _debounceDuration) {
      debugPrint('[Theme] Toggle debounced - too fast');
      return;
    }

    // Prevent concurrent toggles
    if (_isToggling) {
      debugPrint('[Theme] Toggle already in progress, skipping');
      return;
    }

    // Ensure initialized before toggling
    if (!_isInitialized) {
      debugPrint('[Theme] Not initialized yet, waiting...');
      return;
    }

    _isToggling = true;
    _lastToggleTime = now;
    final previousMode = _isDarkMode;

    try {
      // Update UI immediately for smooth transition
      _isDarkMode = !_isDarkMode;
      notifyListeners();
      debugPrint('[Theme] Theme toggled to: ${_isDarkMode ? 'Dark' : 'Light'}');

      // Save to storage
      await _storageService.setThemeMode(_isDarkMode);
      debugPrint('[Theme] Theme saved successfully');
    } catch (e) {
      // Revert on error to maintain consistency
      debugPrint('[Theme] Error toggling theme: $e - Reverting');
      _isDarkMode = previousMode;
      notifyListeners();
    } finally {
      _isToggling = false;
    }
  }

  Future<void> setThemeMode(bool isDark) async {
    // Prevent concurrent operations
    if (_isToggling) {
      debugPrint('[Theme] Operation in progress, skipping setThemeMode');
      return;
    }

    // Ensure initialized
    if (!_isInitialized) {
      debugPrint('[Theme] Not initialized yet, queuing theme mode');
      return;
    }

    // No change needed
    if (_isDarkMode == isDark) {
      debugPrint('[Theme] Theme already set to: ${isDark ? 'Dark' : 'Light'}');
      return;
    }

    _isToggling = true;
    final previousMode = _isDarkMode;

    try {
      _isDarkMode = isDark;
      notifyListeners(); // Update UI immediately
      await _storageService.setThemeMode(isDark);
      debugPrint('[Theme] Theme mode set to: ${isDark ? 'Dark' : 'Light'}');
    } catch (e) {
      debugPrint('[Theme] Error setting theme mode: $e - Reverting');
      _isDarkMode = previousMode;
      notifyListeners();
    } finally {
      _isToggling = false;
    }
  }
}
