#pragma once

#include "global.h"
#include "applications.h"
#include "SnakeGame.h"

#define BEGIN_LIST 11

class Menu
{
public:
    Menu(GyverOLED<SSD1306_128x64, OLED_BUFFER>* oled) : selected(0), oled(oled)
    {}
    ~Menu()
    {
        for (Application* app : apps){
            if (app) delete app;
        }
    }

    void set_apps(std::vector<Application*> in_apps) { apps = in_apps; }
    void show();
    void select_inc() { selected = ((selected + 1) >= apps.size() ? selected : selected + 1); }
    void select_dec() { selected = ((selected - 1) < 0 ? selected : selected - 1); }
    void execute_selected();

private:
    GyverOLED<SSD1306_128x64, OLED_BUFFER>* oled;
    std::vector<Application*> apps; 
    short selected;
};