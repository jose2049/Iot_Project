// This code is derived from the HelloServer Example 
// in the (ESP32) WebServer library .
//
// It hosts a webpage which has one temperature reading to display.
// The webpage is always the same apart from the reading which would change.
// The getTemp() function simulates getting a temperature reading.
// homePage.h contains 2 constant string literals which is the two parts of the
// webpage that never change.
// handleRoot() builds up the webpage by adding as a C++ String:
// homePagePart1 + getTemp() +homePagePart2 
// It then serves the webpage with the command:  
// server.send(200, "text/html", message);
// Note the text is served as html.
//
// Replace the code in the homepage.h file with your own website HTML code.
// 
// This example requires only an ESP32 and download cable. No other hardware is reuired.
// A wifi SSID and password is required.
// Written by: Natasha Rohan  12/3/23
//
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 4

#include <Wire.h>
#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;

#include <BH1750.h>
BH1750 lightMeter;

const char* ssid = "JoseiPhone";
const char* password = "Jm290905=??";

WebServer server(80);

// GLOBAL sensor variables
float t = 0, h = 0, lux = 0, tvoc = 0, eco2 = 0;
unsigned long lastRead = 0;

void readSensors() {
  // Read DHT11
  DHT.read(DHT11_PIN);
  t = DHT.temperature;
  h = DHT.humidity;

  // Read BH1750
  lux = lightMeter.readLightLevel();

  // Read SGP30
  if (sgp.IAQmeasure()) {
    tvoc = sgp.TVOC;
    eco2 = sgp.eCO2;
  }
}

void handleRoot() {
  String message = homePagePart1;

  message += "<tr><td>Temperature</td><td>" + String(t) + "</td><td>°C</td></tr>";
  message += "<tr><td>Humidity</td><td>"    + String(h) + "</td><td>%</td></tr>";
  message += "<tr><td>Light</td><td>"       + String(lux) + "</td><td>Lux</td></tr>";
  message += "<tr><td>TVOC</td><td>"        + String(tvoc) + "</td><td>ppb</td></tr>";
  message += "<tr><td>eCO2</td><td>"        + String(eco2) + "</td><td>ppm</td></tr>";

  message += homePagePart2;

  server.send(200, "text/html", message);
}

void setup() {
  Serial.begin(115200);

  // Start WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // Init I2C and BH1750
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 OK");
  }

  // Init SGP30
  if (!sgp.begin()) {
    Serial.println("SGP30 init failed!");
  }

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Read sensors every 1 second (non-blocking)
  if (millis() - lastRead >= 1000) {
    readSensors();
    lastRead = millis();
  }
}

