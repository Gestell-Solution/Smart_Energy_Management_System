# Gestell Smart Energy Management System

## 🏢 Professional Flutter Project Structure

**Version:** 1.0.0  
**Platforms:** Android & Web (Chrome)  
**Company:** Gestell Company  
**Developer:** Hisham Ahmed <Hisham.ah.hamed@gmail.com>

---

## 📱 Platform Support

### ✅ **Supported Platforms**
- **Android** - Full Bluetooth support for HC-05/HC-06 modules
- **Web (Chrome)** - Demo mode with simulated devices

### ❌ **Unsupported Platforms**
- **iOS** - Completely removed from project
- **Linux Desktop** - Not configured

---

## 🏗️ Project Structure

```
mobile_app/
├── lib/                          # Main application code
│   ├── config/                   # Configuration files
│   │   ├── constants.dart        # App constants
│   │   └── theme.dart           # App theming
│   ├── database/                 # Local database
│   │   └── database_helper.dart  # SQLite helper
│   ├── models/                   # Data models
│   │   ├── device.dart          # Device models
│   │   ├── energy_data.dart     # Energy data model
│   │   └── ...                  # Other models
│   ├── providers/               # State management
│   │   ├── energy_provider.dart # Energy data provider
│   │   └── theme_provider.dart  # Theme provider
│   ├── screens/                  # UI screens
│   │   ├── splash_screen.dart   # App splash screen
│   │   ├── home_screen.dart     # Main dashboard
│   │   └── ...                  # Other screens
│   ├── services/                 # Business logic
│   │   ├── bluetooth_service.dart # Platform-aware BT service
│   │   └── data_service.dart    # Data management
│   ├── widgets/                  # Reusable UI components
│   └── main.dart                # App entry point
├── android/                     # Android-specific code
├── web/                         # Web-specific code
│   ├── index.html              # Web entry point
│   └── manifest.json           # PWA manifest
├── assets/                      # Static assets
│   └── images/                 # Image assets
├── test/                       # Unit tests
├── pubspec.yaml               # Dependencies
└── README.md                  # This file
```

---

## 🚀 Getting Started

### **Prerequisites**
- Flutter SDK >= 3.0.0
- Android SDK (for Android development)
- Chrome browser (for web development)

### **Installation**
```bash
# Clone the project
cd mobile_app

# Get dependencies
flutter pub get

# Run on Android
flutter run

# Run on Web (Chrome)
flutter run -d chrome
```

---

## 📦 Dependencies

### **Core Dependencies**
- `flutter` - Flutter framework
- `provider` - State management
- `google_fonts` - Typography
- `fl_chart` - Charts and graphs
- `syncfusion_flutter_gauges` - Energy gauges

### **Platform-Specific**
- `flutter_bluetooth_serial` - Android Bluetooth (HC-05/HC-06)
- `universal_html` - Web HTML support
- `js` - JavaScript interop for web

### **Data & Storage**
- `sqflite` - Local SQLite database
- `shared_preferences` - Simple key-value storage
- `csv` - Data export functionality

---

## 🔧 Platform Features

### **Android Features**
- ✅ Full Bluetooth Classic support
- ✅ HC-05/HC-06 module compatibility
- ✅ Real-time energy monitoring
- ✅ Local data storage
- ✅ CSV data export

### **Web Features**
- ✅ Responsive web interface
- ✅ Demo mode with simulated devices
- ✅ Real-time data visualization
- ✅ Modern UI/UX design
- ⚠️ Bluetooth not supported (Web limitation)

---

## 📱 Application Features

### **Core Functionality**
- **Real-time Energy Monitoring** - Voltage, Current, Power, Energy
- **Bluetooth Communication** - Connect to energy monitoring devices
- **Data Visualization** - Charts, gauges, and graphs
- **Historical Data** - Store and view energy consumption history
- **Export Capabilities** - CSV export for analysis

### **User Interface**
- **Material Design 3** - Modern, clean interface
- **Dark/Light Themes** - User preference support
- **Responsive Layout** - Works on all screen sizes
- **Professional Branding** - Gestell company identity

---

## 🛠️ Development Commands

### **Build Commands**
```bash
# Android APK (Debug)
flutter build apk --debug

# Android APK (Release)
flutter build apk --release

# Web Build
flutter build web

# Web Build with WASM
flutter build web --wasm
```

### **Testing Commands**
```bash
# Run unit tests
flutter test

# Run integration tests
flutter test integration_test/

# Analyze code
flutter analyze
```

---

## 📊 Performance Metrics

### **Build Performance**
- **Android APK Size:** ~15MB (debug)
- **Web Build Size:** ~2MB (compressed)
- **Build Time:** ~3-5 minutes

### **Runtime Performance**
- **Startup Time:** <2 seconds
- **Data Update Rate:** 1Hz (configurable)
- **Memory Usage:** <50MB typical

---

## 🔒 Security & Permissions

### **Android Permissions**
- `BLUETOOTH_SCAN` - Scan for Bluetooth devices
- `BLUETOOTH_CONNECT` - Connect to Bluetooth devices
- `BLUETOOTH` - Legacy Bluetooth support
- `LOCATION` - Required for Bluetooth scanning

### **Web Security**
- HTTPS required for production
- No special permissions needed
- Sandbox environment compliance

---

## 📈 Future Enhancements

### **Planned Features**
- [ ] Web Bluetooth API support (when widely available)
- [ ] Real-time data synchronization
- [ ] Cloud data backup
- [ ] Advanced analytics dashboard
- [ ] Push notifications for alerts

### **Technical Improvements**
- [ ] WASM optimization for web
- [ ] Background Bluetooth scanning
- [ ] Offline mode enhancements
- [ ] Performance optimizations

---

## 🤝 Support

**Developer:** Hisham Ahmed  
**Email:** Hisham.ah.hamed@gmail.com  
**Company:** Gestell Company

---

## 📄 License

© 2025 Gestell Company. All rights reserved.

---

*Last updated: January 2025*
