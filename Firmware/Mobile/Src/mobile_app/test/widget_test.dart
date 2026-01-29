import 'package:flutter_test/flutter_test.dart';

import 'package:smart_energy_app/main.dart';

void main() {
  testWidgets('App launches successfully', (WidgetTester tester) async {
    // Build our app and trigger a frame.
    await tester.pumpWidget(const SmartEnergyApp());

    // Verify that the app launches
    expect(find.byType(SmartEnergyApp), findsOneWidget);
  });
}
