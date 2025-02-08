#define BLYNK_TEMPLATE_ID "TMPL37e7DeBMI"
#define BLYNK_TEMPLATE_NAME "iot"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char auth[] = "4pBegnC-QPEFI2cm-1EoQPdjj83DPrGv";  
char ssid[] = "Vivo v25";  
char pass[] = "12345678";  

#define DHTPIN 4        // DHT11 Data pin → GPIO 4
#define DHTTYPE DHT11
#define FLAME_SENSOR 5  // Flame Sensor connected to GPIO 5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;  // Timer to send data at intervals

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int flameDetected = digitalRead(FLAME_SENSOR);  // LOW = Fire detected, HIGH = No Fire

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("⚠ Failed to read from DHT sensor!");
    return;
  }

  Serial.println("------ SENSOR DATA ------");
  Serial.print("🌡 Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("💧 Humidity: "); Serial.print(humidity); Serial.println(" %");

  if (flameDetected == LOW) {
    Serial.println("🔥 Fire Detected!");
    Blynk.virtualWrite(V4, 255);  // Turn ON LED in Blynk (Fire detected)
    Blynk.logEvent("fire_alert", "🔥 Fire Detected! Take Action!");  // Send Blynk Notification
  } else {
    Serial.println("✅ No Fire Detected");
    Blynk.virtualWrite(V4, 0);  // Turn OFF LED in Blynk (No Fire)
  }

  // Temperature Alert
  if (temperature > 50) {  // Set your threshold
    Serial.println("⚠ High Temperature Warning!");
    Blynk.logEvent("high_temperature", "🔥 Warning: High Temperature Detected!");  // Send Blynk Notification
  }

  // Send Data to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  Serial.println("------------------------");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(FLAME_SENSOR, INPUT);

  Blynk.begin(auth, ssid, pass);  
  timer.setInterval(2000L, sendSensorData);  // Send data every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}