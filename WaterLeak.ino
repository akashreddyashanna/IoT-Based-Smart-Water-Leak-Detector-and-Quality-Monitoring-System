#define BLYNK_TEMPLATE_ID "TMPL3ZIDrPdLf"
#define BLYNK_TEMPLATE_NAME "Smart Water Leak Detectorr"
#define BLYNK_AUTH_TOKEN "Gk6SM1vI_FlFqN6HysDt3-XgtHN-GSAI"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if needed

// -------------------- WiFi Credentials --------------------
char ssid[] = "No Internet";
char pass[] = "123456789";

// -------------------- Pin Definitions --------------------
#define WATER_SENSOR A0   // Analog water sensor
#define BUZZER D5         // Buzzer pin
#define LED D6            // LED pin

// -------------------- Leak Detection Variables --------------------
int leakThreshold = 600;          // Below this = leak detected
int sensorValue = 0;
bool leakStatus = false;

// -------------------- TDS Sensor Variables --------------------
float aref = 3.3;
float ec = 0;
float offset = 0.14;
unsigned int tds = 0;
float ecCalibration = 1.0;

// -------------------- Timers --------------------
unsigned long lastRead = 0;
const unsigned long interval = 2000; // 2 seconds

// -------------------- Global Flag --------------------
bool leakDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  Wire.begin(D2, D1); // SDA = D2, SCL = D1
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Water Leak");
  lcd.setCursor(0, 1);
  lcd.print("System Ready!");

  Serial.println("========================================");
  Serial.println("   SMART WATER LEAK & QUALITY MONITOR   ");
  Serial.println("========================================");
  Serial.println("Connected to WiFi and Blynk successfully!");
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - lastRead >= interval) {
    lastRead = currentMillis;

    readWaterLeakSensor();
    readTDSSensor();
  }
}

// -------------------- Water Leak Sensor --------------------
void readWaterLeakSensor() {
  sensorValue = analogRead(WATER_SENSOR);
  bool currentLeak = (sensorValue < leakThreshold);

  leakStatus = currentLeak; // always reflect current leak status

  if (leakStatus) {
    if (!leakDetected) {
      Serial.println("💧 Water Leakage Detected!");
      Blynk.virtualWrite(V1, 1);
      Blynk.logEvent("water_leak", "Water Leakage Detected!");
    }

    leakDetected = true;
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Leak: DETECTED   ");
  } else {
    if (leakDetected) {
      Serial.println("✅ No Leakage in Water");
      Blynk.virtualWrite(V1, 0);
    }

    leakDetected = false;
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Leak: SAFE       ");
  }

  Serial.print("Water Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("  → ");
  Serial.println(leakStatus ? "💧 Leak" : "✅ Safe");
}

// -------------------- TDS Sensor --------------------
void readTDSSensor() {
  float rawValue = analogRead(WATER_SENSOR) * aref / 1024.0;
  Serial.print("Raw Analog Voltage (TDS): ");
  Serial.println(rawValue);

  ec = (rawValue * ecCalibration) - offset ; //---EC VALUE ---
  if (ec < 0) ec = 0;

  tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5 ; // ---TDS VALUE ---
  Serial.print("EC: ");
  Serial.print(ec, 2);
  Serial.print(" | TDS: ");
  Serial.println(tds);

  Blynk.virtualWrite(V0, tds);
  Blynk.virtualWrite(V2, ec);

  if (!leakDetected) { // Only update when no leak
    lcd.setCursor(0, 0);
    if (tds < 1200) { //---TDS VALUE LIMIT SET ----
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      lcd.print("Quality: GOOD   ");
      Serial.println("💧 Water Quality: Good");
    } else {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED, HIGH);
      lcd.print("Quality: BAD    ");
      Serial.println("🚨 ALERT: Poor Water Quality!");
    }
  } else {
    Serial.println("⚠️ Buzzer active due to water leakage!");
  }
}
#define BLYNK_TEMPLATE_ID "TMPL3ZIDrPdLf"
#define BLYNK_TEMPLATE_NAME "Smart Water Leak Detectorr"
#define BLYNK_AUTH_TOKEN "Gk6SM1vI_FlFqN6HysDt3-XgtHN-GSAI"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if needed

// -------------------- WiFi Credentials --------------------
char ssid[] = "No Internet";
char pass[] = "123456789";

// -------------------- Pin Definitions --------------------
#define WATER_SENSOR A0   // Analog water sensor
#define BUZZER D5         // Buzzer pin
#define LED D6            // LED pin

// -------------------- Leak Detection Variables --------------------
int leakThreshold = 600;          // Below this = leak detected
int sensorValue = 0;
bool leakStatus = false;

// -------------------- TDS Sensor Variables --------------------
float aref = 3.3;
float ec = 0;
float offset = 0.14;
unsigned int tds = 0;
float ecCalibration = 1.0;

// -------------------- Timers --------------------
unsigned long lastRead = 0;
const unsigned long interval = 2000; // 2 seconds

// -------------------- Global Flag --------------------
bool leakDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  Wire.begin(D2, D1); // SDA = D2, SCL = D1
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Water Leak");
  lcd.setCursor(0, 1);
  lcd.print("System Ready!");

  Serial.println("========================================");
  Serial.println("   SMART WATER LEAK & QUALITY MONITOR   ");
  Serial.println("========================================");
  Serial.println("Connected to WiFi and Blynk successfully!");
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - lastRead >= interval) {
    lastRead = currentMillis;

    readWaterLeakSensor();
    readTDSSensor();
  }
}

// -------------------- Water Leak Sensor --------------------
void readWaterLeakSensor() {
  sensorValue = analogRead(WATER_SENSOR);
  bool currentLeak = (sensorValue < leakThreshold);

  leakStatus = currentLeak; // always reflect current leak status

  if (leakStatus) {
    if (!leakDetected) {
      Serial.println("💧 Water Leakage Detected!");
      Blynk.virtualWrite(V1, 1);
      Blynk.logEvent("water_leak", "Water Leakage Detected!");
    }

    leakDetected = true;
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Leak: DETECTED   ");
  } else {
    if (leakDetected) {
      Serial.println("✅ No Leakage in Water");
      Blynk.virtualWrite(V1, 0);
    }

    leakDetected = false;
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Leak: SAFE       ");
  }

  Serial.print("Water Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("  → ");
  Serial.println(leakStatus ? "💧 Leak" : "✅ Safe");
}

// -------------------- TDS Sensor --------------------
void readTDSSensor() {
  float rawValue = analogRead(WATER_SENSOR) * aref / 1024.0;
  Serial.print("Raw Analog Voltage (TDS): ");
  Serial.println(rawValue);

  ec = (rawValue * ecCalibration) - offset ; //---EC VALUE ---
  if (ec < 0) ec = 0;

  tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5 ; // ---TDS VALUE ---
  Serial.print("EC: ");
  Serial.print(ec, 2);
  Serial.print(" | TDS: ");
  Serial.println(tds);

  Blynk.virtualWrite(V0, tds);
  Blynk.virtualWrite(V2, ec);

  if (!leakDetected) { // Only update when no leak
    lcd.setCursor(0, 0);
    if (tds < 1200) { //---TDS VALUE LIMIT SET ----
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      lcd.print("Quality: GOOD   ");
      Serial.println("💧 Water Quality: Good");
    } else {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED, HIGH);
      lcd.print("Quality: BAD    ");
      Serial.println("🚨 ALERT: Poor Water Quality!");
    }
  } else {
    Serial.println("⚠️ Buzzer active due to water leakage!");
  }
}
