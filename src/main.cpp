#include "global.h"
#include "menu.h"
#include <Adafruit_SSD1306.h>

/*
For adding new apps, you need to create a derived class of the Application class in a new header file,
include applications.h in it, and add two functions: execute() and create(). In the execute function,
you implement the program logic; in the create function, you implement a method that creates a new class
instance and returns pointer on it. Then you need to include your app's header file in menu.h and add the app pointer
to the menu's vector using the function menu.set_apps(vector<Application*>) in main.cpp.
*/

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
Menu menu(&oled);
bool is_changed = false;

void setup() {
  Serial.begin(9600);
  EEPROM.begin(256);
  menu.set_apps({new Snake(&oled), new Application}); //all applications and games
  oled.init(8, 9);
  oled.clear();
  oled.setScale(2); 
  oled.home();
  oled.setContrast(255);
  menu.show();
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
}

void loop() {
  switch (buttons_processing(CNT_OF_BUTTONS))
  {
  case BTN_DOWN:
    menu.select_inc(); 
    is_changed = true;
    break;
  case BTN_UP:
    menu.select_dec();
    is_changed = true;
    break;
  case BTN_SELECT:
    menu.execute_selected();
    is_changed = true;
    break;
  }

  if (is_changed){
    menu.show();
    is_changed = false;
  }

  oled.update();

}

// case BTN_BACK:{
//   int a = 15;
//   Serial.println(a);
//   EEPROM.put(0, a);
//   EEPROM.commit();
//   a = 10;
//   Serial.println(a);
//   EEPROM.get(0, a);
//   EEPROM.commit();
//   Serial.println(a);
//   return;
// }