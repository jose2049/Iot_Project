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

//M5
#include <M5Stack.h>
#include "Adafruit_TCS34725.h"
#define commonAnode \
    true  // set to false if using a common cathode LED.

byte gammatable[256];  // our RGB -> eye-recognized gamma color

static uint16_t color16(uint16_t r, uint16_t g, uint16_t b)
{
    uint16_t _color;
    _color = (uint16_t)(r & 0xF8) << 8;
    _color |= (uint16_t)(g & 0xFC) << 3;
    _color |= (uint16_t)(b & 0xF8) >> 3;
    return _color;
}

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const char* ssid = "JoseiPhone";
const char* password = "Jm290905=??";

WebServer server(80);

// GLOBAL sensor variables
float t = 0, h = 0, tvoc = 0, eco2 = 0;
unsigned long lastRead = 0;
uint16_t clear, red, green, blue;
float BPI = 0;
uint flag = 0;

void readSensors() {
  // Read DHT11
  DHT.read(DHT11_PIN);
  t = DHT.temperature;
  h = DHT.humidity;
 
  // Read SGP30
  if (sgp.IAQmeasure()) {
    tvoc = sgp.TVOC;
    eco2 = sgp.eCO2;
  }
  
  M5.lcd.setTextSize(2);  // Set the text size to 2. 
  M5.Lcd.setTextColor(WHITE, BLACK);
  Serial.println("Color View Test!");
}

void handleRoot() {
  String message = homePagePart1 + homePagePart2;

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

  // Init SGP30
  if (!sgp.begin()) {
    Serial.println("SGP30 init failed!");
  }

  server.on("/", handleRoot);
  
  Serial.println("HTTP server started");

  M5.begin();             // Init M5Stack. 
  M5.Power.begin();       // Init power  
  while (!tcs.begin()) {  //color unit
      Serial.println("No TCS34725 found ... check your connections");
      M5.Lcd.drawString("No Found sensor.", 50, 100, 4);
      delay(1000);
  }

  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);  // Sets the integration time for the TC34725.


    server.on("/temperature", [](){ server.send(200, "text/plain", String(t)); });
    server.on("/humidity",    [](){ server.send(200, "text/plain", String(h)); });
    server.on("/bpi", [](){
    server.send(200, "text/plain", String(BPI, 2)); // sends 0.00, 0.45, etc
});
    server.on("/tvoc",        [](){ server.send(200, "text/plain", String(tvoc)); });
    server.on("/eco2",        [](){ server.send(200, "text/plain", String(eco2)); });
    //Starting the server
    server.begin();
}

void loop() {
  server.handleClient();

  // Read sensors every 1 second (non-blocking)
  if (millis() - lastRead >= 1000) {
    readSensors();
    lastRead = millis();
  }
  
  tcs.getRawData(&red, &green, &blue, &clear);  // Reads the raw red, green, blue and clear channel values
  // Figure out some basic hex code for visualization.  
  uint32_t sum = clear;
  float r, g, b;
  r = red;
  r /= sum;
  g = green;
  g /= sum;
  b = blue;
  b /= sum;
  r *= 256;
  g *= 256;
  b *= 256;
  uint16_t _color = color16((int)r, (int)g, (int)b);

  M5.lcd.setCursor(0, 20);                 // Place the cursor at (0,20).  
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 20);  // Fill the screen with a black rectangle. 

  Serial.print("R: "); Serial.print(red);
  Serial.print(" G: "); Serial.print(green);
  Serial.print(" B: "); Serial.println(blue);
  M5.Lcd.print("0x");
  M5.Lcd.print((int)r, HEX);
  M5.Lcd.print((int)g, HEX);
  M5.Lcd.print((int)b, HEX);

  BPI = blue / (float)clear;

  if (BPI > 0.65) {
    Serial.println("High blue light pollution");
    flag = 1;
  }
  else if (BPI > 0.45) {
    Serial.println("Moderate blue light");
    flag = 2;
  }
  else {
    Serial.println("Low blue light");
    flag = 3;
  }

  if (g > r && g > b) {
    M5.Lcd.setTextColor(GREEN);
     M5.Lcd.println("Green Dominant");
     Serial.println("Green Dominant");
  }
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.printf("\nBPI: %.2f\n", BPI);
  //delay(1000);
}