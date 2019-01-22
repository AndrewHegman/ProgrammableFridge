#include <Arduino.h>
#include <TemperatureWatcher.h>
#include <LiquidCrystal.h>
#include <LCDMenu.h>
#include <DallasTemperature.h>
#include <Timer.h>

typedef enum{
  MainMenu,
  ChangeTargetMenu,
  ErrorMenu
}MenuState;

const int lcdRS  =  7;
const int lcdE   = 6;
const int lcdD7  = 11;
const int lcdD6  = 10;
const int lcdD5  = 9;
const int lcdD4  = 8;
 
const uint8_t selectButtonPin = 2;
const uint8_t rightButtonPin = 3;
const uint8_t leftButtonPin = 4;
const uint8_t temperatureSensorPin = 5;
const uint8_t relayControlPin = A0;

const unsigned long minDebounce = 200;

float GetTemperatureBySerial(void);
float GetTemperatureBySensor(void);
float GetTemperatureTest(void);

MenuState currentMenu = MenuState::MainMenu;
TemperatureWatcher tempWatcher(GetTemperatureBySensor, millis);
Timer relayTimer;

// float GetTemperature(void);
void WriteToDisplay(const TemperatureWatcher * const tempWatcher, const MenuState * const currentMenu, float localTargetTemperature);
void CheckButtonPresses(TemperatureWatcher * const tempWatcher, MenuState * currentMenu);


// TemperatureWatcher tempWatcher(GetTemperature, millis);
LiquidCrystal lcd(lcdRS, lcdE, lcdD4, lcdD5, lcdD6, lcdD7);
LCDMenu lcdMenu;
OneWire oneWire(temperatureSensorPin);
DallasTemperature temperatureSensor(&oneWire);

uint8_t deviceAddress[8];

byte degreeSymbol[8] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  
  // Set up relay output
  pinMode(relayControlPin, OUTPUT);

  pinMode(selectButtonPin, INPUT);
  digitalWrite(selectButtonPin, HIGH);
  
  pinMode(rightButtonPin, INPUT);
  digitalWrite(rightButtonPin, HIGH);

  pinMode(leftButtonPin, INPUT);
  digitalWrite(leftButtonPin, HIGH);

  // Set up temperature sensor

  delay(1000);

  // Set up LCD
  lcd.begin(LCD_NUMBER_OF_ROWS, LCD_NUMBER_OF_LINES);
  lcd.createChar(0, degreeSymbol);
  // currentTemperature = tempWatcher.GetCurrentTemperatureInstance();
  // targetTemperature = tempWatcher.GetTargetTemperatureInstance();

  relayTimer.SetTimeout(180000);
  relayTimer.Start();

  temperatureSensor.getAddress(deviceAddress, 0);

  tempWatcher.SetTargetTemperature(50.0f);
  tempWatcher.SetUpdateDelayMilliseconds(1000);

  WriteToDisplay(&tempWatcher, &currentMenu, 0.0f);
}

void loop() {
  tempWatcher.Update();
  // Serial.println(tempWatcher.Alarm());
  switch(tempWatcher.Alarm()){
    
    case TemperatureWatcher::TemperatureAlarm_t::alarmHigh:
      Serial.print("High: ");
      Serial.print(relayTimer.Expired() ? "1" : "0");
      Serial.print(" : ");
      Serial.println(relayTimer.Running()  ? "1" : "0");
      if(relayTimer.Expired()){
        digitalWrite(relayControlPin, HIGH);
        relayTimer.Stop();
      }
      break;
    
    case TemperatureWatcher::TemperatureAlarm_t::alarmLow:
      Serial.print("Low: ");
      Serial.println(relayTimer.Running()  ? "1" : "0");
      digitalWrite(relayControlPin, LOW);
      if(!relayTimer.Running()){
        Serial.print("Starting timer!!: ");
        relayTimer.Start();
        Serial.println(relayTimer.Running() ? "1" : "0");
      }
      break;

    case TemperatureWatcher::TemperatureAlarm_t::alarmNone:
      Serial.println("None");
      // No change needed
      break;

    default:
      Serial.println("Error");
      Serial.println(tempWatcher.Alarm());
      // Should never get into this block
      digitalWrite(relayControlPin, LOW);
      break;
  }

  CheckButtonPresses(&tempWatcher, &currentMenu);
}

void CheckButtonPresses(TemperatureWatcher * const tempWatcher, MenuState * currentMenu){

  static float localTargetTemperature = 50.0f;
  static unsigned long lastRightButtonPress = 0;
  static unsigned long lastLeftButtonPress = 0;
  static unsigned long lastSelectButtonPress = 0;
  static float lastTemperature = 0;

  bool updateScreen = false; // Only update screen when necessary

  if(!digitalRead(selectButtonPin)){

    if(millis() - lastSelectButtonPress > minDebounce){
      
      // Update target temperature if necessary
      if(*currentMenu == MenuState::ChangeTargetMenu){
        tempWatcher->SetTargetTemperature(localTargetTemperature);
      }

      // Change to new screen
      *currentMenu = *currentMenu == MenuState::ErrorMenu ? MenuState::ErrorMenu : 
                *currentMenu == MenuState::MainMenu ? MenuState::ChangeTargetMenu : MenuState::MainMenu;

      lastSelectButtonPress = millis();
      updateScreen = true;
    }
  }
  
  if(*currentMenu == MenuState::ChangeTargetMenu){

    if(!digitalRead(rightButtonPin)){

      if(millis() - lastRightButtonPress > minDebounce){

        localTargetTemperature += 0.5f;
        lastRightButtonPress = millis();
        updateScreen = true;
      }
    }

    if(!digitalRead(leftButtonPin)){

      if(millis() - lastLeftButtonPress > minDebounce){

        localTargetTemperature -= 0.5f;
        lastLeftButtonPress = millis();
        updateScreen = true;
      }
    }
  }
  // Serial.println(localTargetTemperature);
  // if(updateScreen || tempWatcher->TemperatureChanged()) WriteToDisplay(tempWatcher, currentMenu, localTargetTemperature);
  
  if(updateScreen || abs(tempWatcher->GetCurrentTemperature() - lastTemperature) > 0.1f){
  //   Serial.println(tempWatcher->GetCurrentTemperature());
  //   Serial.println(lastTemperature);
  //   Serial.println(tempWatcher->TemperatureChanged());
  //   Serial.println("\n\n\n");
    WriteToDisplay(tempWatcher, currentMenu, localTargetTemperature);
    lastTemperature = tempWatcher->GetCurrentTemperature();
  }
}

void WriteToDisplay(const TemperatureWatcher * const tempWatcher, const MenuState * const currentMenu, float localTargetTemperature){
  switch(*currentMenu){
    char buffer[3];
    char menuBuffer[16];

    case MenuState::MainMenu:

      lcd.setCursor(0, 0);
      dtostrf(tempWatcher->GetCurrentTemperature(), 3, 1, buffer);
      sprintf(menuBuffer, "%-10s%s F%4s", "Current:", buffer, " ");
      lcd.print(menuBuffer);

      lcd.setCursor(snprintf(NULL, 0, "%-10s%s", "Current:", buffer), 0);
      lcd.write(byte(0));

      memset(buffer, 0, 3);
      memset(menuBuffer, 0, 16);

      lcd.setCursor(0, 1);

      dtostrf(tempWatcher->GetTargetTemperature(), 3, 1, buffer);
      sprintf(menuBuffer, "%-10s%s F%4s", "Target:", buffer, " ");
      lcd.print(menuBuffer);

      lcd.setCursor(snprintf(NULL, 0, "%-10s%s", "Target:", buffer), 1);
      lcd.write(byte(0));

      break;

    case MenuState::ChangeTargetMenu:

      lcd.setCursor(0, 0);
      dtostrf(localTargetTemperature, 3, 1, buffer);
      sprintf(menuBuffer, "%-10s%s F%4s", "Target:", buffer, " ");
      lcd.print(menuBuffer);

      lcd.setCursor(snprintf(NULL, 0, "%-10s%s", "Target:", buffer), 0);
      lcd.write(byte(0));

      lcd.setCursor(0, 1);
      lcd.print("-     SET      +");
      break;

    case MenuState::ErrorMenu:
      // Fall through to default

    default:

      lcd.setCursor(0, 0);
      lcd.print("An error");

      lcd.setCursor(0, 1);
      lcd.print("has occured");
      break;
  }
}

float GetTemperatureTest(void){
  static float temp = 0.0f;
  return temp += 0.1f;
}

float GetTemperatureBySerial(void){
  static float temp;
  while(Serial.available()){
    temp = (float)Serial.readString().toFloat();
    Serial.println("Temperature set to " + String(temp));
  }
  return temp;
}

float GetTemperatureBySensor(void){
  temperatureSensor.requestTemperatures();
  return temperatureSensor.getTempFByIndex(0);
}
// inline void ChangeMenu(MenuState* currentMenu){
//   /*
//     currentMenu == 1 -> 0
//     currentMenu == 0 -> 1
//     currentMenu == 2 -> 2
//   */

//   *currentMenu = *currentMenu == MenuState::ErrorMenu ? MenuState::ErrorMenu : 
//                 *currentMenu == MenuState::MainMenu ? MenuState::ChangeTargetMenu : MenuState::MainMenu;

// }