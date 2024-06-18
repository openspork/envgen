#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "AdsrEnvelope.h"

#define TFT_BL   4 // Display backlight control pin

class Display {
private:
    int screenWidth;
    int screenHeight;
    uint16_t* buffer;
    TFT_eSPI tft;

public:
    Display(int screenWidth, int screenHeight);
    void init();
    void drawChart(AdsrEnvelope* adsr);
};
#endif