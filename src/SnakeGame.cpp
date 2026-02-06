#include "SnakeGame.h"

bool Snake::is_crash_in_oneself(){
    int i = 0;
    while (1)
    {
        //Serial.println(head - end);
        if (*(end + i) == *head){
            if ((end + i) == head)
                break;
            else
                return true;
        }
        i++;
    }
    return false;
} 
bool Snake::in_body(Point* p){
    for (int i = 0; i < head - end; i++) {
        if (*(end + i) == *p)
            return true;
    }
    return false;
}
void Snake::set_pixel(short x, short y, bool is_fill){
    oled->rect(x, y, x + SCALE - 1, y + SCALE - 1, is_fill);
}

int Snake::execute(){
    Serial.printf("head in execute first: %d, %d\n", head->x, head->y);
    oled->clear();
    oled->home();
    oled->setScale(1);
    oled->printf("Length: %d\n\r", len);

    bool is_defeat = false;

    while (1) {
        oled->update();

        switch (buttons_processing(CNT_OF_BUTTONS))
        {
        case BTN_UP:
            direction = DIR_UP;
            break;
        case BTN_DOWN:
            direction = DIR_DOWN;
            break;
        case BTN_LEFT:
            direction = DIR_LEFT;
            break;
        case BTN_RIGHT:
            direction = DIR_RIGHT;
            break;
        case BTN_BACK:
            if (!is_defeat){
                save = {*head, *end, tick, len};
                EEPROM.put(ADDRESS_SAVE, save);
                EEPROM.commit();
                Serial.printf("saved: {{%d, %d}, {%d, %d}, %d, %d}\n", save.head_pos.x, save.head_pos.y, save.end_pos.x, save.end_pos.y, save.tick, save.len);
                //Serial.printf("saved_test: {{%d, %d}, {%d, %d}, %d}\n", test_save.head_pos.x, test_save.head_pos.y, test_save.end_pos.x, test_save.end_pos.y, test_save.tick);
                return CMD_NONE;
            }
            break;
        case BTN_SELECT:
            if (is_defeat) return CMD_RECREATEAPP;
            break;
        }
        
        if (!is_defeat){
            head++;
            *head = { static_cast<short>((head - 1)->x + direction.x), static_cast<short>((head - 1)->y + direction.y) };
            end++;
            if (head - field > SCALED_HEIGHT_DISPLAY * SCALED_WIDTH_DISPLAY) head = field;
            if (end - field > SCALED_HEIGHT_DISPLAY * SCALED_WIDTH_DISPLAY) end = field;

            set_pixel(head->x, head->y);
            set_pixel(end->x, end->y, false);
            set_pixel(apple.coord.x, apple.coord.y);
            border.show(oled);

            if (apple << *head){
                end -= apple.saturation;
                bool flag = false;
                short x, y, satur = static_cast<short>(random(RAND_STRN));
                Point p;
                while (!flag){
                    x = static_cast<short>(random(FIELD_X1 + 3, FIELD_X2 - 3) & (~1)),
                    y = static_cast<short>(random(FIELD_Y1 + 3, FIELD_Y2 - 3) & (~1)),

                    p = {x, y};
                    if (!in_body(&p)){
                        flag = true;
                    }
                }
                apple = Apple(x, y, satur);
                len += satur;

                oled->home();
                oled->setScale(1);
                oled->printf("Length: %d +%d\n\r", len, satur);
            }
            Serial.printf("head: %d, %d\n", head->x, head->y);

            tick++;

            if (is_crash_in_oneself() || border << *head){
                save.init_at_start();
                EEPROM.put(ADDRESS_SAVE, save);
                EEPROM.commit();
                oled->clear();
                oled->setCursor(10, 0);
                oled->setScale(2);
                oled->print("GAME OVER\n\r");
                oled->setScale(1);
                oled->print("press select for\n\rcontinue\n\r");
                is_defeat = true;
            }
        }
    }
    return CMD_NONE;
}