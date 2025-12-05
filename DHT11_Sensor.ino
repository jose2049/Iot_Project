//Temperature & Humidity sensor
//Adding libraries
#include <DFRobot_DHT11.h>
//Creating a objec DHT
DFRobot_DHT11 DHT;
#define DHT11_PIN 4

void setup(){
  //Inilializing serial monitor
  Serial.begin(115200);
}

void loop(){
  //Reading form PIN 4 and printing out tem & humi
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  delay(1000);
}

