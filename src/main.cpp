#include <Arduino.h>
#include <TemperatureWatcher.h>
#include <LiquidCrystal.h>
#include <LCDMenu.h>

const int lcdRS  =  7;
const int lcdE   = 6;
const int lcdD7  = 11;
const int lcdD6  = 10;
const int lcdD5  = 9;
const int lcdD4  = 8;

float GetTemperature(void);
void WriteToDisplay(String, String);

TemperatureWatcher tempWatcher(GetTemperature, millis);
LiquidCrystal lcd(lcdRS, lcdE, lcdD4, lcdD5, lcdD6, lcdD7);
LCDMenu lcdMenu;

void setup() {
  Serial.begin(9600);
  
  // Set up relay output
  pinMode(LED_BUILTIN, OUTPUT);

  // Set up temperature watcher
  tempWatcher.setTargetTemperature(50.0f);
  tempWatcher.setUpdateDelayMilliseconds(1000);

  // Set up LCD
  lcd.begin(LCD_NUMBER_OF_ROWS, LCD_NUMBER_OF_LINES);
}

void loop() {
  tempWatcher.update();
  switch(tempWatcher.alarm()){
    case TemperatureWatcher::TemperatureAlarm_t::alarmHigh:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    
    case TemperatureWatcher::TemperatureAlarm_t::alarmLow:
      digitalWrite(LED_BUILTIN, LOW);
      break;

    case TemperatureWatcher::TemperatureAlarm_t::alarmNone:
      digitalWrite(LED_BUILTIN, LOW);
      break;
  }
  WriteToDisplay(String("Curr Temp: ") + String(tempWatcher.getCurrentTemperature()), 
                 "<-     +      ->");
  // WriteToDisplay(lcdMenu.current());
}

void WriteToDisplay(String line1, String line2){
  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}

float GetTemperature(void){
  static float temp;
  while(Serial.available()){
    temp = (float)Serial.readString().toFloat();
    Serial.println("Temperature set to " + String(temp));
  }
  return temp;
}