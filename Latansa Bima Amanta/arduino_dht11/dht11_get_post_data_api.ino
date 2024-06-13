#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Konfigurasi WiFi
const char* ssid = "memel";
const char* password = "admin123";

// Konfigurasi server REST API
const char* serverName = "http://192.168.0.113:5000/api/dht";

// Konfigurasi DHT
#define DHTPIN 32 // Pin GPIO yang terhubung ke Data pin DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { // Jika terhubung ke WiFi

    // Membaca data dari sensor DHT11
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Periksa apakah pembacaan berhasil
    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      return;
    }

    // Menampilkan data di Serial Monitor
    Serial.print("Kelembaban: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Suhu: ");
    Serial.print(t);
    Serial.println(" *C");

    // Mengirim data menggunakan metode POST
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Mengatur URL dan header
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      // Membuat payload JSON
      String payload = "{\"temperature\": " + String(t) + ", \"humidity\": " + String(h) + "}";

      // Mengirim request POST
      int httpResponseCode = http.POST(payload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      // Mengakhiri koneksi
      http.end();
    }
  }

  // Delay 10 detik
  delay(10000);
}
