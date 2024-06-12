#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>

const char* ssid = "ZTE_2.4G_UnYeTA";
const char* password = "LaxRFCrP";
const char* serverAddress = "http://192.168.1.10:5000/sensor/data";
int sensorPin = 34;

int getAirQualityData() {
  int analog_data = analogRead(sensorPin);
  Serial.print("Air Quality: ");
  Serial.println(analog_data);
  delay(300);  // Allow sensor to stabilize
  return analog_data;
}

void postSensorData(){
  WiFiClient client;
  HTTPClient http;

  int airQualityData = getAirQualityData();
  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"gas\": " + String(airQualityData) + "}";

  int httpResponseCode = http.POST(payload);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  Serial.println(payload);

  http.end();

  delay(5000);
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Cek status koneksi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Tampilkan pesan sukses
  Serial.println("");
  Serial.println("WiFi connected!");
}

void loop() {
  postSensorData();
}
