#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Definisikan pin dan tipe sensor
#define DHTPIN 4
#define DHTTYPE DHT11

// Informasi jaringan WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Alamat server
const char* serverName = "http://192.168.1.100:5000/data"; // Ganti dengan IP server lokal Anda

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Membaca suhu dan kelembaban
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Periksa apakah pembacaan berhasil
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000); // Tambahkan jeda untuk debugging
    return;
  }

  // Mengirim data jika WiFi terhubung
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String httpRequestData = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(10000); // Kirim data setiap 10 detik
}
