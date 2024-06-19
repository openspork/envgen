#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "AdsrEnvelope.h"

#define TFT_BL   4 // Display backlight control pin

class Display {
private:
    const uint16_t CAPTION_AREA_HEIGHT = 26;
    uint16_t screenWidth;
    uint16_t screenHeight;
    uint16_t chartHeight;
    TFT_eSPI tft;
    TFT_eSprite chartSprite;
    TFT_eSprite captionSprite;

    void drawChart(AdsrEnvelope* adsr);
    void drawCaption(AdsrEnvelope* adsr);

public:
    Display(uint16_t screenWidth, uint16_t screenHeight);
    void init();
    void draw(AdsrEnvelope* adsr);
};
#endif