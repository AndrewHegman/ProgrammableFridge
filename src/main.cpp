#include <Arduino.h>
#include <TemperatureWatcher.h>

float GetTemperature(void){
  static float temp;
  while(Serial.available()){
    temp = (float)Serial.readString().toFloat();
    Serial.println("Temperature set to " + String(temp));
  }
  return temp;
}

TemperatureWatcher tempWatcher(GetTemperature, millis, 50.0f);

void setup() {
  Serial.begin(9600);
  tempWatcher.setUpdateDelayMilliseconds(1000);
}

void loop() {
  tempWatcher.update();
}

