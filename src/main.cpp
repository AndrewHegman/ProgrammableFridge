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
float* currentTemperature = NULL;
float* targetTemperature = NULL;

float GetTemperature(void);
void WriteToDisplay(menuScreen_t menu);

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
  currentTemperature = tempWatcher.GetCurrentTemperatureInstance();
  targetTemperature = tempWatcher.GetTargetTemperatureInstance();

  menuScreen_t currentTemperatureScreen;
  
  String currTempStr = String("Curr Temp: ") + String(*currentTemperature);
  String currTargetStr = String("Target: ") + String(*targetTemperature);

  currentTemperatureScreen.text.add(&currTempStr, 0);
  currentTemperatureScreen.text.add(&currTargetStr, 1);
  lcdMenu.RegisterMenuScreen(currentTemperatureScreen);
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
  WriteToDisplay(lcdMenu.current());
}

void WriteToDisplay(menuScreen_t menu){
  lcd.setCursor(0, 0);
  lcd.print(String("Curr Temp: ") + String(*currentTemperature));

  lcd.setCursor(0, 1);
  lcd.print(String("Target: ") + String(*targetTemperature));
}

float GetTemperature(void){
  static float temp;
  while(Serial.available()){
    temp = (float)Serial.readString().toFloat();
    Serial.println("Temperature set to " + String(temp));
  }
  return temp;
}