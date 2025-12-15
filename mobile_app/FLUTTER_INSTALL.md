# Flutter Installation Guide for Linux

## Problem

You're seeing: **`Command 'flutter' not found`**

This means Flutter SDK is not installed on your system yet.

## Solution: Install Flutter

### Method 1: Using Snap (Easiest)

```bash
sudo snap install flutter --classic
```

Then verify:

```bash
flutter doctor
```

---

### Method 2: Manual Installation (Recommended)

#### Step 1: Download Flutter SDK

```bash
cd 
mkdir development
cd development

# Download Flutter (latest stable)
wget https://storage.googleapis.com/flutter_infra_release/releases/stable/linux/flutter_linux-3.16.5-stable.tar.xz

# Extract
tar xf flutter_linux-3.16.5-stable.tar.xz
```

#### Step 2: Add Flutter to PATH

```bash
echo 'export PATH="$PATH:$HOME/development/flutter/bin"' >> ~/.bashrc
source ~/.bashrc
```

#### Step 3: Verify Installation

```bash
flutter doctor
```

You'll see something like:

```text
[✓] Flutter (Channel stable, 3.16.5, on Linux)
[!] Android toolchain - install Android SDK
[!] Chrome - not installed
```

---

### Method 3: Install Android SDK (Required for APK)

Flutter needs Android SDK to build APK files.

#### Option A: Install Android Studio (Full)

```bash
# Download from: https://developer.android.com/studio
# Or use snap:
sudo snap install android-studio --classic
```

Then in Android Studio:

1. Open Settings → Appearance & Behavior → System Settings → Android SDK
2. Install SDK Platforms (Android 13.0 - API 33)
3. Install SDK Tools (Android SDK Build-Tools, Platform-Tools)

#### Option B: Install Command Line Tools Only

```bash
# Install Java (required)
sudo apt install openjdk-11-jdk

# Download Android command line tools
mkdir -p ~/Android/Sdk/cmdline-tools
cd ~/Android/Sdk/cmdline-tools
wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip
unzip commandlinetools-linux-9477386_latest.zip
mv cmdline-tools latest

# Add to PATH
echo 'export ANDROID_HOME=$HOME/Android/Sdk' >> ~/.bashrc
echo 'export PATH=$PATH:$ANDROID_HOME/cmdline-tools/latest/bin' >> ~/.bashrc
echo 'export PATH=$PATH:$ANDROID_HOME/platform-tools' >> ~/.bashrc
source ~/.bashrc

# Accept licenses
yes | sdkmanager --licenses

# Install required packages
sdkmanager "platform-tools" "platforms;android-33" "build-tools;33.0.0"
```

---

## Final Setup

After installing Flutter and Android SDK:

```bash
# Run doctor to check everything
flutter doctor

# You should see:
# [✓] Flutter
# [✓] Android toolchain
```

If there are any issues, follow the suggestions from `flutter doctor`.

---

## Build Your APK

Once Flutter and Android SDK are installed:

```bash
cd mobile_app

# Get dependencies
flutter pub get

# Build APK
flutter build apk --release

# APK will be at:
# build/app/outputs/flutter-apk/app-release.apk
```

---

## Quick Install Script

Save this as `install_flutter.sh`:

```bash
#!/bin/bash

echo "Installing Flutter..."

# Download Flutter
cd ~
mkdir -p development
cd development
wget https://storage.googleapis.com/flutter_infra_release/releases/stable/linux/flutter_linux-3.16.5-stable.tar.xz
tar xf flutter_linux-3.16.5-stable.tar.xz

# Add to PATH
echo 'export PATH="$PATH:$HOME/development/flutter/bin"' >> ~/.bashrc
source ~/.bashrc

# Verify
flutter doctor

echo "Flutter installed! Run 'flutter doctor' to check status."
```

Run with:

```bash
chmod +x install_flutter.sh
./install_flutter.sh
```

---

## Summary

**To fix your error:**

1. Install Flutter SDK (Method 1 or 2)
2. Install Android SDK (Method 3)
3. Run `flutter doctor` to verify
4. Then run `flutter pub get` in your mobile_app directory

**Estimated time:** 10-15 minutes

Your mobile app code is ready - you just need to install the tools to build it! 🚀
