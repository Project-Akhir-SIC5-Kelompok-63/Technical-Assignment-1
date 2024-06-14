#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define DHT11PIN 21
#define DHTTYPE DHT11

DHT dht(DHT11PIN, DHTTYPE);

const char* ssid = "inissid";
const char* password = "";
const char* serverPath = "http://192.168.0.100:5000/api/ta";

unsigned long lastMsg = 0;
float temp = 0;

void setup_wifi() { 
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  dht.begin();
}

void loop() {
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    float temp = dht.readTemperature();

    if (isnan(temp)) {
      Serial.println("Failed to read DHT!");
      return;
    }

    String temp_string = String(temp, 2);
    
    Serial.print("Temperature: ");
    Serial.println(temp);

    HTTPClient http;
    http.begin(serverPath);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "temperature=" + temp_string;
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}
