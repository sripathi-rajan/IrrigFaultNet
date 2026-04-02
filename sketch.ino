#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define SM1_PIN 36   // 10cm
#define SM2_PIN 39   // 30cm
#define SM3_PIN 34   // 60cm

DHT dht(DHTPIN, DHTTYPE);

// Simulated field capacity & thresholds
const float FIELD_CAP = 100.0;
const float TARGET_MOISTURE = 65.0;  // ideal root zone %

float readMoisture(int pin) {
  int raw = analogRead(pin);
  return (raw / 4095.0) * 100.0;  // returns float properly
}

String classifyFault(float m10, float m30, float m60, float target) {
  float avg = (m10 + m30 + m60) / 3.0;
  float deltaM = avg - target;

  if (avg < target * 0.6) return "UNDER_IRRIGATION";
  if (avg > FIELD_CAP * 0.95) return "OVER_IRRIGATION";
  if (m10 > target * 0.85 && avg < target * 0.5) return "TIMING_FAULT";
  if (m10 > m60 * 2.0) return "DISTRIBUTION_FAULT";
  return "NORMAL";
}

float computeSeverity(float m10, float m30, float m60, float target) {
  float avg = (m10 + m30 + m60) / 3.0;
  float severity = abs(avg - target) / target;
  if (severity > 1.0) severity = 1.0;
  return severity;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("timestamp,moisture_10cm,moisture_30cm,moisture_60cm,temp,humidity,fault_type,severity");
}

void loop() {
  float m10 = readMoisture(SM1_PIN);
  float m30 = readMoisture(SM2_PIN);
  float m60 = readMoisture(SM3_PIN);
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    temp = 28.0; hum = 60.0;  // fallback
  }

  String fault = classifyFault(m10, m30, m60, TARGET_MOISTURE);
  float severity = computeSeverity(m10, m30, m60, TARGET_MOISTURE);

  // CSV output
  Serial.print(millis() / 1000);
  Serial.print(",");
  Serial.print(m10, 1);
  Serial.print(",");
  Serial.print(m30, 1);
  Serial.print(",");
  Serial.print(m60, 1);
  Serial.print(",");
  Serial.print(temp, 1);
  Serial.print(",");
  Serial.print(hum, 1);
  Serial.print(",");
  Serial.print(fault);
  Serial.print(",");
  Serial.println(severity, 3);

  delay(3000);  // every 3 seconds in sim = 15 min real
}