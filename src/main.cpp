#include <Arduino.h>
#include <TemperatureWatcher.h>
#include <LiquidCrystal.h>
#include <LCDMenu.h>

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
const unsigned long minDebounce = 200;

// float GetTemperature(void);
void WriteToDisplay(const TemperatureWatcher * const tempWatcher, const MenuState * const currentMenu, float localTargetTemperature);
void CheckButtonPresses(TemperatureWatcher * const tempWatcher, MenuState * currentMenu);
float GetTemperature(void);

// TemperatureWatcher tempWatcher(GetTemperature, millis);
LiquidCrystal lcd(lcdRS, lcdE, lcdD4, lcdD5, lcdD6, lcdD7);
LCDMenu lcdMenu;

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
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(selectButtonPin, INPUT);
  digitalWrite(selectButtonPin, HIGH);
  
  pinMode(rightButtonPin, INPUT);
  digitalWrite(rightButtonPin, HIGH);

  pinMode(leftButtonPin, INPUT);
  digitalWrite(leftButtonPin, HIGH);

  // Set up temperature watcher


  // Set up LCD
  lcd.begin(LCD_NUMBER_OF_ROWS, LCD_NUMBER_OF_LINES);
  lcd.createChar(0, degreeSymbol);
  // currentTemperature = tempWatcher.GetCurrentTemperatureInstance();
  // targetTemperature = tempWatcher.GetTargetTemperatureInstance();
}

void loop() {
  static MenuState currentMenu = MenuState::MainMenu;
  static TemperatureWatcher tempWatcher(GetTemperature, millis);
  static bool tempWatcherInitialized = false;
  
  if(!tempWatcherInitialized){

    tempWatcher.SetTargetTemperature(50.0f);
    tempWatcher.SetUpdateDelayMilliseconds(1000);
    tempWatcherInitialized = true;

    WriteToDisplay(&tempWatcher, &currentMenu, 0.0f);

    Serial.println(tempWatcherInitialized);
  }

  tempWatcher.Update();
  switch(tempWatcher.Alarm()){
    case TemperatureWatcher::TemperatureAlarm_t::alarmHigh:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    
    case TemperatureWatcher::TemperatureAlarm_t::alarmLow:
      digitalWrite(LED_BUILTIN, LOW);
      break;

    case TemperatureWatcher::TemperatureAlarm_t::alarmNone:
      digitalWrite(LED_BUILTIN, LOW);
      break;

    default:
      digitalWrite(LED_BUILTIN, LOW);
      break;
  }

  CheckButtonPresses(&tempWatcher, &currentMenu);
}

void CheckButtonPresses(TemperatureWatcher * const tempWatcher, MenuState * currentMenu){

  static float localTargetTemperature = tempWatcher->GetTargetTemperature();

  bool updateScreen = false; // Only update screen when necessary
  if(!digitalRead(selectButtonPin)){

    static unsigned long lastSelectButtonPress = 0;
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

      static unsigned long lastRightButtonPress = 0;
      if(millis() - lastRightButtonPress > minDebounce){

        localTargetTemperature += 0.5f;
        lastRightButtonPress = millis();
        updateScreen = true;
      }
    }

    if(!digitalRead(leftButtonPin)){

      static unsigned long lastLeftButtonPress = 0;
      if(millis() - lastLeftButtonPress > minDebounce){

        localTargetTemperature -= 0.5f;
        lastLeftButtonPress = millis();
        updateScreen = true;
      }
    }
  }
  if(updateScreen) WriteToDisplay(tempWatcher, currentMenu, localTargetTemperature);
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
      lcd.print("+     SET      -");
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

float GetTemperature(void){
  static float temp;
  while(Serial.available()){
    temp = (float)Serial.readString().toFloat();
    Serial.println("Temperature set to " + String(temp));
  }
  return temp;
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