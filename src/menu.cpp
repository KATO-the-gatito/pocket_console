#include "menu.h"

void Menu::show(){
    oled->clear();
    oled->setScale(1);
    oled->invertText(false);
    oled->home();
    oled->printf("GAMES & APPS\n\r");
    oled->line(0, BEGIN_LIST - 1, 200, BEGIN_LIST - 1);
    oled->setScale(2);
    oled->setCursor(0, 2);

    for (Application* app : apps){
        oled->invertText(apps[selected] == app ? true : false);
        oled->printf("%s\n\r", app->name);
    }

    oled->home();
}
void Menu::execute_selected() { 
    Application* sel_app = apps[selected];
    switch(sel_app->execute())
    {
    case CMD_RECREATEAPP: {
        Application* app = sel_app->create();
        delete sel_app;
        apps[selected] = app;
        break;
    }
    }
}