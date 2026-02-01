
# تقرير مشاكل كود الموبايل (Flutter) + قائمة Tasks

## 1) نطاق التقرير (Mobile-only)

- **الهدف:** استخراج مشاكل كود الموبايل نفسه (Flutter) وتحويلها إلى Tasks قابلة للتنفيذ.
- **داخل المشروع:** `Firmware/Mobile/Src/mobile_app`
- **ملاحظة:** هذا التقرير **لا يغطي مشاكل Embedded/Integration** إلا لو أثرت على Bug في الموبايل نفسه.

## 2) ملخص سريع (Executive Summary)

- **حالة التطبيق على Android:** قابل للتشغيل في العموم، مع وجود نقاط كانت تسبب UX سيء أو Bugs وتم إصلاح جزء كبير منها.
- **أهم ما كان ظاهر للمستخدم وتم إصلاحه:** feedback بتاع الـ connect، وglitches في theme toggle، وDismissing alerts بدون حذف فعلي.
- **أهم Tech Debt متبقي:** تخزين Energy History في SharedPreferences (غير مناسب للأحجام الكبيرة) مع وجود SQLite جاهز لكنه غير مستخدم.

## 3) إصلاحات تم تنفيذها بالفعل (Applied Fixes)

### 3.1 Bluetooth connect feedback

- إصلاح عدم ظهور SnackBar بعد `connect` بسبب استخدام `context` الخاص بالـ dialog بعد `Navigator.pop`.
- إغلاق حالة الـ scan قبل بدء الـ connect لتفادي UI متداخل.

**Files:**

- `lib/screens/settings_screen.dart`
- `lib/providers/energy_provider.dart`

### 3.2 Dark/Light theme toggle

- تعطيل الـ switch أثناء `initialize` وأثناء عملية toggling.
- الاعتماد على `setThemeMode(value)` بدل `toggleTheme()`.
- جعل System UI overlay style (status bar) يتغير حسب الثيم.

**Files:**

- `lib/screens/settings_screen.dart`
- `lib/providers/theme_provider.dart`
- `lib/main.dart`

### 3.3 Alerts (dismiss/undo) + Storage

- الـ Dismissible بقى يحذف الـ alert فعليًا من provider + storage.
- Undo بقى يرجّع الـ alert.
- تقوية `Alert.fromJson` لو `isRead` مفقود.

**Files:**

- `lib/screens/alerts_screen.dart`
- `lib/providers/energy_provider.dart`
- `lib/services/storage_service.dart`
- `lib/models/alert.dart`

### 3.4 Relay toggle reliability

- منع optimistic UI: لا يتم تغيير حالة الريلاي إلا لو `BluetoothService.controlRelay` رجع success.
- error handling + alert عند الفشل.

**Files:**

- `lib/providers/energy_provider.dart`
- `lib/services/bluetooth_service.dart`

### 3.5 History stability + correctness

- تحويل `HistoryScreen` من `List<dynamic>` إلى `List<EnergyData>`.
- فلترة التاريخ بقيت inclusive + ترتيب النتائج بالـ timestamp.
- وضع cap للـ history في الذاكرة (1000 record) لتفادي memory growth.

**Files:**

- `lib/screens/history_screen.dart`
- `lib/providers/energy_provider.dart`
- `lib/services/storage_service.dart`

### 3.6 Misc UX stability fixes

- زر Reset في Dashboard بقى يستنى النتيجة ويعرض success/error حقيقي بدل رسالة نجاح ثابتة.
- إصلاح Flutter compatibility في `alerts_screen.dart` (`withValues` -> `withOpacity`).
- إلغاء Timer في `SplashScreen` داخل `dispose` لتفادي callback بعد dispose.

**Files:**

- `lib/screens/dashboard_screen.dart`
- `lib/screens/alerts_screen.dart`
- `lib/screens/splash_screen.dart`

## 4) مشاكل/Tech Debt متبقية (محتاجة Tasks)

### 4.1 Data storage (High priority)

- SharedPreferences غير مناسب لحفظ History كبيرة (حتى لو capped)، والأفضل استخدام SQLite الموجود بالفعل.

### 4.2 Navigation consistency

- خلي navigation موحّد (إما routes في `MaterialApp` أو `MaterialPageRoute` في كل مكان).

### 4.3 Dark mode consistency

- في شاشات مختلفة مازال فيه `Colors.white` و`Colors.grey[...]` بشكل hardcoded مما يقلل وضوح الدارك مود.

### 4.4 Provider/Service lifecycle

- `BluetoothService` singleton و`EnergyProvider` بيعمل `dispose()` للـ controllers. ده مقبول طالما provider واحد طول عمر التطبيق، لكنه حساس لو حصل إعادة إنشاء.

### 4.5 Testing

- مفيش اختبارات (unit/widget) لتغطية parsing, storage migrations, provider logic.

## 5) جدول Tasks (Actionable Backlog)

| ID | Priority | Area | Task | Files (examples) |
| --- | --- | --- | --- | --- |
| M-001 | P0 | Storage | نقل Energy History من SharedPreferences إلى SQLite (EnergyDatabase) + provider APIs للقراءة بالـ ranges | `lib/database/energy_database.dart`, `lib/providers/energy_provider.dart`, `lib/services/storage_service.dart` |
| M-002 | P0 | Storage | عمل migration آمن: أول تشغيل بعد التحديث ينقل البيانات القديمة (لو موجودة) ثم يمسح key القديم | `lib/services/storage_service.dart` |
| M-003 | P1 | UI/Dark mode | إزالة hardcoded colors/text في الشاشات واستبدالها بـ `Theme.of(context)` | `lib/screens/*` |
| M-004 | P1 | Navigation | توحيد navigation routing عبر app (routes أو MaterialPageRoute) | `lib/main.dart`, `lib/screens/*` |
| M-005 | P1 | UX | عرض errors للمستخدم بشكل موحّد (SnackBar/banner) عند scan/connect/relay failure | `lib/screens/settings_screen.dart`, `lib/providers/energy_provider.dart` |
| M-006 | P2 | Architecture | حسم ownership للـ periodic data requests (Service vs Provider) وإزالة الـ dead code في provider | `lib/providers/energy_provider.dart`, `lib/services/bluetooth_service.dart` |
| M-007 | P2 | Testing | إضافة unit tests لـ JSON parsing + alerts persistence + date range filters | `test/*` |
| M-008 | P2 | Platform | لو Web مطلوب فعليًا: فصل `BluetoothService` بمنطق conditional imports بدل `dart:io` | `lib/services/bluetooth_service.dart` |
