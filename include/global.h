#pragma once

#include <Arduino.h>
#include <vector>
#include <GyverOLED.h>
#include <EEPROM.h>
#include "boolean_array.h"

#define BTN_LEFT 4
#define BTN_RIGHT 0
#define BTN_DOWN 1 
#define BTN_UP 5
#define BTN_BACK 3
#define BTN_SELECT 2

#define FLAG_LEFT 0
#define FLAG_RIGHT 1
#define FLAG_DOWN 2
#define FLAG_UP 3 
#define FLAG_BACK 4
#define FLAG_SELECT 5

#define CNT_OF_BUTTONS 6
#define HEIGHT_DISPLAY 64
#define WIDTH_DISPLAY 128

#define CMD_NONE 0
#define CMD_RECREATEAPP 1

static boolean_array<byte> flags_btns_state;
static boolean_array<byte> flags_btns_pstate;

int buttons_processing(unsigned);