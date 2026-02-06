#pragma once

#include "applications.h"

#define SCALE 2
#define SCALED_WIDTH_DISPLAY WIDTH_DISPLAY / SCALE
#define SCALED_HEIGHT_DISPLAY HEIGHT_DISPLAY / SCALE

#define RAND_STRN 1, 5

#define FIELD_X1 0
#define FIELD_Y1 10
#define FIELD_X2 127
#define FIELD_Y2 63

#define DIR_UP {0, -SCALE}
#define DIR_DOWN {0, SCALE}
#define DIR_LEFT {-SCALE, 0}
#define DIR_RIGHT {SCALE, 0}

#define ADDRESS_SAVE 0
#define ADDRESS_ISFIRSTWRITE 12


struct Point
{

    short x, y;

    bool operator!=(Point& p){
        return !(x == p.x && y == p.y);
    }
    bool operator==(Point& p){
        return x == p.x && y == p.y;
    }
};

struct Save
{
    Point head_pos, end_pos;
    int tick, len;
    void init_at_start(){
        head_pos = {WIDTH_DISPLAY / 2, HEIGHT_DISPLAY / 2 - SCALE};
        end_pos = {WIDTH_DISPLAY / 2, HEIGHT_DISPLAY / 2};
        tick = 0;
        len = 5;
    }
    
    void print(){
        Serial.printf("saved: {{%d, %d}, {%d, %d}, %d, %d}\n", head_pos.x, head_pos.y, end_pos.x, end_pos.y, tick, len);
    }
};

class Apple 
{
public:
    Apple(short x, short y, int saturation) : coord({x, y}), saturation(saturation)
    {}
    ~Apple() 
    {}

    bool operator<< (Point& p){
        return coord == p;
    }

    Point coord;
    int saturation;
};

class Border
{
public:
    Border(short x1, short y1, short x2, short y2) : coordLU({x1, y1}), coordRD({x2, y2})
    {}
    ~Border() {}

    bool operator<< (Point& p){
        //Serial.printf("p.x: %d, p.y: %d | LU.x: %d, LU.y: %d, RD.x: %d, RD.y: %d\n", p.x, p.y, coordLU.x, coordLU.y, coordRD.x, coordRD.y);
        return !(p.x >= coordLU.x && p.x <= coordRD.x && p.y >= coordLU.y && p.y <= coordRD.y);
    }
    void show(GyverOLED<SSD1306_128x64, OLED_BUFFER>* oled){
        oled->rect(coordLU.x, coordLU.y, coordRD.x, coordRD.y, OLED_STROKE);
    }

private:
    Point coordLU, coordRD;
};


class Snake : public Application
{
public:

    Snake(GyverOLED<SSD1306_128x64, OLED_BUFFER>* oled) :
        Application("Snake"),
        oled(oled),
        field(new Point[SCALED_HEIGHT_DISPLAY * SCALED_WIDTH_DISPLAY]),
        head(field),
        end(field),
        direction(DIR_UP),
        border(FIELD_X1, FIELD_Y1, FIELD_X2, FIELD_Y2),
        apple(random(FIELD_X1 + 3, FIELD_X2 - 3) & (~1), random(FIELD_Y1 + 3, FIELD_Y2 - 3) & (~1), random(RAND_STRN))
    {
        // if (EEPROM.read(ADDRESS_ISFIRSTWRITE) != 4){
        //     EEPROM.write(ADDRESS_ISFIRSTWRITE, 4);
        //     EEPROM.commit();
        //     save.init_at_start();
        //     EEPROM.put(ADDRESS_SAVE, save);
        //     EEPROM.commit();
        // }
        save.print();
        EEPROM.get(ADDRESS_SAVE, save);
        tick = save.tick;
        len = save.len;
        EEPROM.commit();
        save.print();
        Serial.printf("head before constructor: %d, %d\n", head->x, head->y);
        head = field + save.len;
        *head = save.head_pos;
        *end = save.end_pos;
        Serial.printf("head after constructor: %d, %d\n", head->x, head->y);
    }
    ~Snake()
    {
        delete[] field;
    }

    int execute();
    Application* create(){
        Snake* app = new Snake(oled);
        return app;
    }


private:
    GyverOLED<SSD1306_128x64, OLED_BUFFER>* oled;
    Point* field;
    Point *head, *end;
    Point direction;
    Border border;
    Apple apple;
    Save save;
    int tick, len;

    bool is_crash_in_oneself();
    bool in_body(Point* p);
    void set_pixel(short x, short y, bool is_fill = true);
};