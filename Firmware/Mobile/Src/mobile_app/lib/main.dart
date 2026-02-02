import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:provider/provider.dart';
import 'config/theme.dart';
import 'providers/energy_provider.dart';
import 'providers/theme_provider.dart';
import 'screens/splash_screen.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  GoogleFonts.config.allowRuntimeFetching = false;

  // Set preferred orientations
  await SystemChrome.setPreferredOrientations([
    DeviceOrientation.portraitUp,
    DeviceOrientation.portraitDown,
  ]);

  runApp(const SmartEnergyApp());
}

class SmartEnergyApp extends StatelessWidget {
  const SmartEnergyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => ThemeProvider()),
        ChangeNotifierProvider(create: (_) => EnergyProvider()),
      ],
      child: Consumer<ThemeProvider>(
        builder: (context, themeProvider, _) {
          final overlayStyle = SystemUiOverlayStyle(
            statusBarColor: Colors.transparent,
            statusBarIconBrightness:
                themeProvider.isDarkMode ? Brightness.light : Brightness.dark,
            statusBarBrightness:
                themeProvider.isDarkMode ? Brightness.dark : Brightness.light,
          );

          return MaterialApp(
            title: 'Smart Energy',
            debugShowCheckedModeBanner: false,

            themeAnimationDuration: Duration.zero,

            // Theme
            theme: AppTheme.lightTheme,
            darkTheme: AppTheme.darkTheme,
            themeMode: themeProvider.themeMode,

            // Home
            home: const SplashScreen(),

            // Builder for consistent theming
            builder: (context, child) {
              return AnnotatedRegion<SystemUiOverlayStyle>(
                value: overlayStyle,
                child: MediaQuery(
                  data: MediaQuery.of(
                    context,
                  ).copyWith(textScaler: const TextScaler.linear(1.0)),
                  child: child!,
                ),
              );
            },
          );
        },
      ),
    );
  }
}
