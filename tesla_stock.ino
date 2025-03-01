#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "URB 2.4";
const char* password = "11242002";
const String apiKey = "0E3ZSE78BN821L9L";
const String symbol = "SPY"; // Simbolul pentru acțiunile Tesla

const String url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + apiKey;

void setup() {
  Serial.begin(115200);
  delay(250);
  display.begin(i2c_Address, true);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("Connected to WiFi");
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0); 
  display.println("Connected to WiFi...");
  display.display();
  delay(1000);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      String price = doc["Global Quote"]["05. price"]; // Obține prețul acțiunilor Tesla

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("S&P500 Price:");
      display.println("$" + price); // Afișează prețul acțiunilor Tesla
      display.display();
    } else {
      Serial.println("Error getting data from API");
    }

    http.end();
  }

  delay(60000); // Actualizează datele o dată pe minut
}
