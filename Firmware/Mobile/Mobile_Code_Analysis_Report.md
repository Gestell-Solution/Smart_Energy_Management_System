# تقرير تحليل كود الموبايل مقابل الـ Requirements

**Project:** Smart Energy Management System

**Scope:** `Firmware/Mobile` (Flutter Mobile App)

**Reference Requirements:** `Doc/General/Requirment/REQ.md` + ملاحظات الوثائق داخل `Firmware/Mobile/Src/doc/*`

---

## 1) ملخص تنفيذي

- **تطبيق الموبايل نفسه (Flutter) يبدو “قابل للبناء والتشغيل” على Android**، وفيه تنفيذ واضح لـ Bluetooth Classic (HC‑05) وبروتوكول Frames.
- **لكن النظام ككل غالبًا “لن يشتغل end-to-end” حاليًا** لأن وثيقة `INTEGRATION_GAPS.md` تشير لوجود **عطل/نقص Critical في الـ Embedded**: `App_CommManager_ProcessCommand()` يعمل echo ولا يرد بإطار `GET_RMS_DATA` بالشكل المطلوب.
- يوجد **تعارض/عدم اتساق في الوثائق** حول صيغة البيانات (CSV vs Binary frames) ووحدة الطاقة (Wh vs kWh) وقد يؤدي لقراءات/تكلفة خاطئة حتى بعد اكتمال الـ embedded.
- ادعاء دعم الـ Web في README غير مضمون حاليًا لأن `bluetooth_service.dart` يستورد `dart:io` والذي عادةً **يكسر build الويب** بدون conditional imports.

**Bottom line:**

- **Mobile App:** غالبًا جاهز للتشغيل على Android، لكن يحتاج توحيد البروتوكول/الوحدات والتأكد من إعدادات build.
- **Integration:** متوقف على إصلاحات Embedded (Critical) لتنفيذ `GET_RMS_DATA` وباقي الأوامر.

---

## 2) المتطلبات المطلوبة (حسب `REQ.md`) وما يتعلق بالموبايل

### 2.1 متطلبات تخص الموبايل مباشرة

- **FR6 / AC5 (Bluetooth HC‑05):** الموبايل يجب أن يستقبل القيم من النظام عبر Bluetooth “on demand”.

### 2.2 متطلبات ليست ضمن مسؤولية الموبايل مباشرة (لكن تؤثر على نجاح النظام)

- **FR1–FR5:** قياس/حساب/عرض/حماية: هذه على Embedded.
- **FR6 / AC6 (رفع بيانات للسيرفر كل 10 ثوانٍ عبر ESP8266):** Embedded/Server.

---

## 3) ما الموجود فعليًا في كود الموبايل

### 3.1 ستاك المشروع

- Flutter / Dart (SDK: `>=3.0.0 <4.0.0`)
- State management: `provider`
- Bluetooth Classic: `flutter_bluetooth_serial` (مناسب لـ HC‑05)
- Permissions: `permission_handler` + `location`

### 3.2 نقطة البداية

- `lib/main.dart` يقوم بتهيئة Providers (`ThemeProvider`, `EnergyProvider`) ويبدأ بـ `SplashScreen`.

### 3.3 خدمة البلوتوث (Core)

- `lib/services/bluetooth_service.dart`
- **يرسل frame بالشكل:** `[0xAA][Length][CommandId][Data...]`
- **يطلب بيانات كل 1 ثانية** (`AppConstants.dataUpdateInterval = 1s`) عبر إرسال `cmdGetRmsData = 0x05`.
- **يفكّ Frame Response** ويتوقع payload بطول **10 bytes** لـ `GET_RMS_DATA`:
  - Voltage: uint16 / 10
  - Current: uint16 / 100
  - Power: uint16 / 10
  - Energy: uint32 / 100

### 3.4 واجهات المستخدم

- يوجد شاشات: Dashboard / History / Alerts / Settings.
- Settings فيها scan/connect/disconnect، إعداد cost rate، ومعلومات الجهاز.

---

## 4) مطابقة المتطلبات (Gap Analysis)

### 4.1 AC5 / FR6 (Bluetooth – Mobile receives values on demand)

- **الحالة:** ✅ موجود في الكود.
- **تفاصيل:** التطبيق يرسل `GET_RMS_DATA (0x05)` دوريًا (كل ثانية)، وهذا يعتبر “on demand” لأن الجهاز لا يبعت بدون طلب.
- **المخاطر:** نجاح هذا البند يعتمد على الـ Embedded إنه يرد بالـ frame الصحيح.

### 4.2 متطلبات إضافية غير منصوص عليها للموبايل

- لا يوجد في `REQ.md` متطلبات Cloud/UI محددة للموبايل غير الاستقبال/العرض، وبالتالي من ناحية requirements العامة، التطبيق يغطي المطلوب.

---

## 5) مشاكل/نواقص محتملة في كود الموبايل (قد تمنع التشغيل أو تسبب نتائج خاطئة)

### 5.1 **تعارض وثائق البروتوكول (CSV vs Binary Frames)**

- **الملاحظة:** `Firmware/Mobile/Src/doc/README.md` يذكر أن البروتوكول CSV بالشكل `V,I,P,E,Status`.
- **لكن الكود الفعلي** + `PROTOCOL.md` يستخدمان **Binary frames** Header=0xAA.
- **التأثير:** الفريق ممكن يبني Embedded على CSV بالخطأ، أو يختبر باستخدام CSV فتظهر “لا بيانات” في التطبيق.
- **التوصية:** اعتماد وثيقة واحدة (يفضل `PROTOCOL.md`) وتحديث/حذف أي وصف قديم.

### 5.2 **وحدة الطاقة Energy: Wh vs kWh (Bug منطقي محتمل)**

- `EnergyData.energy` موصوف كـ **kWh**.
- `PROTOCOL.md` يصف الطاقة في response على أنها **Wh** (Accumulated Energy) ثم scaling `/100`.
- `bluetooth_service.dart` يحسب:
  - `energy = uint32 / 100.0`
  - ثم يضعها داخل `EnergyData.energy` (الذي يعتبرها kWh) ويستخدمها لحساب التكلفة.
- **التأثير:** لو الـ embedded يبعت Wh فعلاً، فالتكلفة والقراءات ستكون خاطئة (factor 1000 غالبًا).
- **التوصية:** توحيد وحدة الطاقة في البروتوكول والكود:
  - إما إرسال kWh من embedded.
  - أو تحويل Wh → kWh في التطبيق (`kWh = Wh / 1000`).

### 5.3 دعم Web غير مضمون

- `bluetooth_service.dart` يحتوي على `import 'dart:io' show Platform;`.
- في Flutter Web عادةً `dart:io` غير متاح وبالتالي قد يفشل build للويب.
- **التوصية:** إذا مطلوب Web فعليًا:
  - استخدام **conditional imports** أو فصل BluetoothService لمنصتين.
  - أو إزالة ادعاء Web support والاكتفاء بـ Android.

### 5.4 إعدادات Android build قد تسبب مشاكل بيئية

- `android/app/build.gradle.kts`:
  - `compileSdk = 36` (قد يحتاج Android SDK/AGP حديث جدًا).
  - `targetSdk = 34`.
- **التأثير:** على بيئات تطوير غير محدثة، build قد يفشل بسبب compileSdk.
- **التوصية:** تثبيت SDK المناسب، أو خفض compileSdk لقيمة مستقرة متوفرة في بيئة الفريق.

### 5.5 ملاحظة: مزج “Relay Control” واسم command

- في `AppConstants` يوجد:
  - `cmdCuttOFF = 0x09`
  - `cmdControlRelay = 0x09`
- **المخاطر:** ليس خطأ بحد ذاته، لكن يلزم توحيد المعنى على الـ embedded: هل 0x09 “قطع حماية” أم “تحكم relay”.

---

## 6) هل هيشتغل ولا لا؟

### 6.1 تشغيل تطبيق الموبايل وحده (Android)

- **متوقع يشتغل:** ✅ غالبًا نعم.
- **ملاحظات:** يحتاج Flutter SDK وAndroid SDK وصلاحيات Bluetooth/Location.

### 6.2 تشغيله مع الـ Embedded (السيناريو الحقيقي)

- **متوقع يشتغل:** ⚠️ غالبًا لا حاليًا.
- **سبب أساسي:** `INTEGRATION_GAPS.md` يحدد Gap Critical في Embedded: عدم تنفيذ رد `GET_RMS_DATA`.

---

## 7) أهم النواقص خارج الموبايل (Blockers للتكامل)

حسب `Firmware/Mobile/Src/doc/INTEGRATION_GAPS.md`:

- **Gap #1 (Critical):** `App_CommManager_ProcessCommand()` في embedded يعمل echo ولا يبني response frame الصحيح لـ `GET_RMS_DATA`.
- **Gap #6 (Medium):** `WRITE_EEPROM`/Reset energy غير منفذ.
- التحقق من تحديث `g_SystemData` وتشغيل MeasurementEngine + scheduler.

---

## 8) Checklist سريعة للاختبار/القبول

### Mobile (Android)

- **[Build]** `flutter pub get` بدون أخطاء
- **[Run]** `flutter run` على جهاز Android
- **[Permissions]** التطبيق يطلب Bluetooth permissions + Location عند الحاجة
- **[Scan]** يظهر HC‑05 في قائمة الأجهزة (يفضل بعد pairing من إعدادات Android)

### Integration (Mobile ↔ Embedded)

- **[Protocol]** عند إرسال `AA 00 05` من الموبايل، الـ embedded يرد `AA 0A 05 ...10 bytes...`
- **[Values]** القيم تظهر منطقية (V حوالي 220V، I حسب الحمل)
- **[Units]** التحقق من وحدة energy (Wh/kWh) والتكلفة

---

## 9) توصيات نهائية (أولوية عالية)

- **[P0]** إصلاح Embedded لتنفيذ `GET_RMS_DATA` كما في `PROTOCOL.md`.
- **[P0]** توحيد “وحدة الطاقة” (Wh vs kWh) بين embedded والموبايل.
- **[P1]** توحيد الوثائق: إزالة وصف CSV أو تحديثه ليطابق الـ Binary protocol.
- **[P2]** لو Web مطلوب: إصلاح `dart:io` عبر conditional imports؛ لو غير مطلوب: تحديث README لتجنب لبس.

---

**مكان الكود المُراجع:**

- Mobile app: `Firmware/Mobile/Src/mobile_app`
- Requirements: `Doc/General/Requirment/REQ.md`
- Protocol: `Firmware/Mobile/Src/doc/PROTOCOL.md`
- Integration gaps: `Firmware/Mobile/Src/doc/INTEGRATION_GAPS.md`
