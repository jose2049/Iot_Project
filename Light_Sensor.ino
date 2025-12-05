#include <Wire.h>     //I2C communication protocol
#include <BH1750.h>   //libray to read from sensor

BH1750 lightMeter;

void setup(){     //I2C communication protocol
  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin(); //Initializing object "lightMeter"

  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  float lux = lightMeter.readLightLevel();    //varaible to save luminance values
  //Displaying the measurement on the Serial Monitor
  Serial.print("Light: ");    
  Serial.print(lux);    
  Serial.println(" lx");
  delay(1000);
}