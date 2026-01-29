import 'package:flutter/material.dart';
import 'dart:async';
import '../config/theme.dart';
import 'package:google_fonts/google_fonts.dart';
import 'dashboard_screen.dart';

class SplashScreen extends StatefulWidget {
  const SplashScreen({super.key});

  @override
  State<SplashScreen> createState() => _SplashScreenState();
}

class _SplashScreenState extends State<SplashScreen>
    with SingleTickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<double> _fadeAnimation;
  late Animation<double> _scaleAnimation;
  late Animation<double> _slideAnimation;

  @override
  void initState() {
    super.initState();

    _controller = AnimationController(
      duration: const Duration(milliseconds: 1800),
      vsync: this,
    );

    _fadeAnimation = Tween<double>(begin: 0.0, end: 1.0).animate(
      CurvedAnimation(
          parent: _controller,
          curve: const Interval(0.0, 0.6, curve: Curves.easeOut)),
    );

    _scaleAnimation = Tween<double>(begin: 0.7, end: 1.0).animate(
      CurvedAnimation(
          parent: _controller,
          curve: const Interval(0.0, 0.6, curve: Curves.elasticOut)),
    );

    _slideAnimation = Tween<double>(begin: 30.0, end: 0.0).animate(
      CurvedAnimation(
          parent: _controller,
          curve: const Interval(0.3, 0.9, curve: Curves.easeOut)),
    );

    _controller.forward();

    // Navigate to dashboard after 3 seconds
    Timer(const Duration(seconds: 3), () {
      if (mounted) {
        Navigator.of(context).pushReplacement(
          PageRouteBuilder(
            pageBuilder: (context, animation, secondaryAnimation) =>
                const DashboardScreen(),
            transitionsBuilder:
                (context, animation, secondaryAnimation, child) {
              return FadeTransition(opacity: animation, child: child);
            },
            transitionDuration: const Duration(milliseconds: 600),
          ),
        );
      }
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        decoration: BoxDecoration(
          gradient: LinearGradient(
            begin: Alignment.topLeft,
            end: Alignment.bottomRight,
            colors: [
              AppTheme.secondaryColor, // Dark Blue
              AppTheme.secondaryColor.withOpacity(0.85),
              const Color(0xFF1A1A1A),
            ],
          ),
        ),
        child: SafeArea(
          child: Center(
            child: FadeTransition(
              opacity: _fadeAnimation,
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  const Spacer(flex: 1),

                  // Logo/Icon with Gestell Green
                  ScaleTransition(
                    scale: _scaleAnimation,
                    child: Container(
                      width: 120,
                      height: 120,
                      decoration: BoxDecoration(
                        gradient: LinearGradient(
                          colors: [
                            AppTheme.primaryColor,
                            AppTheme.primaryColor.withOpacity(0.8),
                          ],
                        ),
                        shape: BoxShape.circle,
                        boxShadow: [
                          BoxShadow(
                            color: AppTheme.primaryColor.withOpacity(0.4),
                            blurRadius: 30,
                            spreadRadius: 5,
                          ),
                        ],
                      ),
                      child: const Icon(
                        Icons.bolt_rounded,
                        size: 60,
                        color: AppTheme.secondaryColor,
                      ),
                    ),
                  ),
                  const SizedBox(height: 32),

                  // Brand Name - GESTELL
                  Transform.translate(
                    offset: Offset(0, _slideAnimation.value),
                    child: Text(
                      'GESTELL',
                      style: GoogleFonts.inter(
                        fontSize: 42,
                        fontWeight: FontWeight.w800,
                        color: Colors.white,
                        letterSpacing: 4,
                      ),
                    ),
                  ),
                  const SizedBox(height: 10),

                  // Subtitle
                  Transform.translate(
                    offset: Offset(0, _slideAnimation.value),
                    child: Text(
                      'Smart Energy Management',
                      style: GoogleFonts.inter(
                        fontSize: 14,
                        fontWeight: FontWeight.w500,
                        color: AppTheme.primaryColor,
                        letterSpacing: 1.2,
                      ),
                    ),
                  ),

                  const SizedBox(height: 50),

                  // Loading Indicator
                  const SizedBox(
                    width: 40,
                    height: 40,
                    child: CircularProgressIndicator(
                      valueColor: AlwaysStoppedAnimation<Color>(
                        AppTheme.primaryColor,
                      ),
                      strokeWidth: 3.5,
                    ),
                  ),

                  const Spacer(flex: 1),

                  // Brand Slogan
                  Padding(
                    padding: const EdgeInsets.symmetric(horizontal: 32),
                    child: Text(
                      'Stable by Design. Engineered to Last.',
                      textAlign: TextAlign.center,
                      style: GoogleFonts.inter(
                        fontSize: 12,
                        fontWeight: FontWeight.w500,
                        color: Colors.white.withOpacity(0.6),
                        letterSpacing: 0.8,
                        height: 1.4,
                      ),
                    ),
                  ),
                  const SizedBox(height: 6),

                  Text(
                    'Built to Trust.',
                    style: GoogleFonts.inter(
                      fontSize: 12,
                      fontWeight: FontWeight.w600,
                      color: AppTheme.primaryColor.withOpacity(0.8),
                      letterSpacing: 0.8,
                    ),
                  ),
                  const SizedBox(height: 30),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
