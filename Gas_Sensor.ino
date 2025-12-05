//SPG30 Sensor 
// can I use the same library for the ESP30?
//Air SGP30 sensor
//Need to include library?

#include <Wire.h>
#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;


void setup() {
  Serial.begin(9600);

  if (! sgp.begin()) {
    Serial.println("Sensor not found :-(");
    while (1);
  }

  
}

void loop() {
  
  //Read air quality values
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }

  //Print values
  Serial.print("TVOC: ");
  Serial.print(sgp.TVOC);
  Serial.println(" ppb\n");

  Serial.print("eCO2: ");
  Serial.print(sgp.eCO2);
  Serial.println(" ppm\n");

  delay(1000);

}

