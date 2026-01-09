# 📱 Smart Energy Management Mobile App

A beautiful and intuitive Flutter mobile application for monitoring and controlling the Smart Energy Management System via Bluetooth.

## ✨ Features

### 📊 Real-time Monitoring
- **Live Data Display**: Real-time voltage, current, power, and energy consumption
- **Visual Gauges**: Beautiful animated gauges for voltage and current
- **Cost Calculation**: Automatic energy cost calculation based on configurable rates
- **Connection Status**: Live Bluetooth connection status indicator

### 📈 History & Analytics
- **Data Charts**: Interactive line and bar charts for energy consumption trends
- **Multiple Time Periods**: View data for today, last week, or last month
- **Statistics**: Average, peak, and total energy consumption metrics
- **Data Export**: Export energy data for analysis (coming soon)

### 🔔 Alerts System
- **Smart Alerts**: Automatic detection of overload, overcurrent, and overvoltage conditions
- **Alert History**: Track all system alerts with timestamps
- **Priority Levels**: Color-coded alerts (Critical, Warning, Info)
- **Dismissible Alerts**: Swipe to dismiss individual alerts

### ⚙️ Settings & Configuration
- **Bluetooth Management**: Scan, pair, and connect to HC-05 devices
- **Theme Switching**: Beautiful light and dark themes
- **Cost Rate Configuration**: Set energy cost per kWh
- **Device Information**: View system limits and device details

### 🎨 Beautiful UI/UX
- **Modern Design**: Clean, professional interface with gradients and glassmorphism
- **Dark Mode**: Eye-friendly dark theme
- **Smooth Animations**: Fluid transitions and micro-animations
- **Responsive Layout**: Optimized for all screen sizes

## 🛠️ Tech Stack

- **Framework**: Flutter 3.0+
- **State Management**: Provider
- **Charts**: FL Chart & Syncfusion Gauges
- **Bluetooth**: Flutter Blue Plus
- **Storage**: Shared Preferences
- **Typography**: Google Fonts (Inter)

## 📦 Installation

### Prerequisites
- Flutter SDK (3.0.0 or higher)
- Android Studio / Xcode
- Android device with Bluetooth (Android 5.0+) or iOS device

### Steps

1. **Install Flutter**
   ```bash
   # Follow official Flutter installation guide
   # https://docs.flutter.dev/get-started/install
   ```

2. **Clone and Setup**
   ```bash
   cd mobile_app
   flutter pub get
   ```

3. **Run the App**
   ```bash
   # For Android
   flutter run

   # For iOS
   flutter run -d ios

   # For release build
   flutter build apk --release
   ```

## 🔧 Configuration

### Bluetooth Data Protocol

The app expects data from the HC-05 module in the following CSV format:

```
V,I,P,E,Status
```

**Example:**
```
220.5,5.2,1146.6,2.5,OK
```

Where:
- **V**: Voltage in Volts (e.g., 220.5)
- **I**: Current in Amperes (e.g., 5.2)  
- **P**: Power in Watts (e.g., 1146.6)
- **E**: Energy in kWh (e.g., 2.5)
- **Status**: System status string (e.g., "OK", "OVERLOAD")

### Energy Cost Rate

Default rate is set to **0.90 EGP/kWh**. You can change this in the Settings screen.

### System Limits

Default thresholds (can be modified in `lib/config/constants.dart`):
- **Max Voltage**: 260V
- **Max Current**: 30A
- **Max Power**: 2000W
- **Overvoltage Limit**: 250V
- **Overcurrent Limit**: 10A
- **Overpower Limit**: 2000W

## 📱 Permissions

The app requires the following Android permissions:
- `BLUETOOTH` - For Bluetooth communication
- `BLUETOOTH_ADMIN` - For managing Bluetooth connections
- `BLUETOOTH_SCAN` - For scanning Bluetooth devices
- `BLUETOOTH_CONNECT` - For connecting to devices
- `ACCESS_FINE_LOCATION` - Required for Bluetooth scanning on Android

These are automatically requested when needed.

## 🏗️ Project Structure

```
lib/
├── config/
│   ├── theme.dart          # App themes (light/dark)
│   └── constants.dart      # App constants
├── models/
│   ├── energy_data.dart    # Energy data model
│   ├── alert.dart          # Alert model
│   └── device.dart         # Bluetooth device model
├── services/
│   ├── bluetooth_service.dart  # Bluetooth communication
│   └── storage_service.dart    # Local data storage
├── providers/
│   ├── energy_provider.dart    # Energy data state management
│   └── theme_provider.dart     # Theme state management
├── screens/
│   ├── splash_screen.dart      # Splash screen
│   ├── dashboard_screen.dart   # Main dashboard
│   ├── history_screen.dart     # History & charts
│   ├── alerts_screen.dart      # Alerts & notifications
│   └── settings_screen.dart    # Settings
└── main.dart               # App entry point
```

## 🚀 Usage

1. **First Launch**
   - App opens with splash screen
   - Automatically navigates to dashboard

2. **Connect to Device**
   - Go to Settings tab
   - Tap "Scan for Devices"
   - Select your HC-05 device from the list
   - Wait for connection confirmation

3. **Monitor Energy**
   - View real-time data on Dashboard
   - Check gauges for voltage and current
   - Monitor power and energy consumption
   - Track total cost

4. **View History**
   - Switch to History tab
   - Select time period (Today/Week/Month)
   - View charts and statistics

5. **Check Alerts**
   - Red badge shows unread alert count
   - Review system warnings and errors
   - Swipe to dismiss alerts

## 🎨 Screenshots

<!-- Add screenshots here when available -->

## 🔄 Data Flow

1. ATmega32 → HC-05 (UART): Sends energy data
2. HC-05 → Mobile App (Bluetooth): Transmits data
3. BluetoothService: Parses incoming data
4. EnergyProvider: Updates app state
5. UI: Displays data with real-time updates
6. StorageService: Saves history locally

## 🐛 Troubleshooting

### Bluetooth Connection Issues
- Ensure Bluetooth is enabled on your phone
- Make sure HC-05 is powered and in range
- Check that device is not paired with another phone
- Grant all required permissions

### No Data Displaying
- Verify Bluetooth connection is active
- Check data format from ATmega32
- Ensure UART baud rate matches (typically 9600)

### App Crashes
- Check Flutter version compatibility
- Verify all dependencies are installed (`flutter pub get`)
- Clear app data and reinstall

## 🔮 Future Enhancements

- [ ] Cloud data synchronization
- [ ] Data export to CSV/PDF
- [ ] Custom alert thresholds
- [ ] Energy usage predictions
- [ ] Multi-device support
- [ ] Widget support for home screen
- [ ] Notification push alerts

## 👨‍💻 Development

Built with ❤️ by **Gestell Company**

### Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## 📄 License

Copyright © 2025 Gestell Company. All rights reserved.

## 📞 Support

For support and questions, please contact:
- Email: Hisham4Ahmed@gmail.com
- Project: Smart Energy Management System

---

**Version**: 1.0.0  
**Last Updated**: December 2025
