#include "Display.h"

Display::Display(uint16_t width, uint16_t height) : screenWidth(width), screenHeight(height), tft(), 
        chartSprite(&tft), captionSprite(&tft) {
    chartHeight = screenHeight - CAPTION_AREA_HEIGHT;
    chartSprite.createSprite(screenWidth, chartHeight);
    captionSprite.createSprite(screenWidth, CAPTION_AREA_HEIGHT);
}

void Display::init() {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    tft.init();
    tft.setRotation(1);
    Serial.println("Display initialized");
}

void Display::draw(AdsrEnvelope* adsr) {
    drawChart(adsr);
    drawCaption(adsr);

    chartSprite.pushSprite(0, 0);
    captionSprite.pushSprite(0, screenHeight - CAPTION_AREA_HEIGHT);
}

void Display::drawChart(AdsrEnvelope* adsr) {
    chartSprite.fillSprite(TFT_BLACK);

    int xPrev = 0;
    int yPrev = -1;

    for (int x = 0; x < screenWidth; x++) {
        double time = adsr->getEnvelopeStartTime() + x * adsr->getEnvelopeDurationMs() / screenWidth;
        double envelopeValue = adsr->getEnvelopeValue(time);

        int y = (int)((chartHeight + 1) - (envelopeValue * chartHeight / adsr->getEnvelopeMax()));

        if (yPrev > 0) {
          chartSprite.drawLine(xPrev, yPrev, x, y, TFT_WHITE);
        }

        xPrev = x;
        yPrev = y;
    }

    captionSprite.fillSprite(TFT_BLACK);
    captionSprite.setTextFont(1);
    captionSprite.setTextSize(2);
    captionSprite.setTextColor(TFT_WHITE, TFT_BLACK);
}

void Display::drawCaption(AdsrEnvelope* adsr) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%s: %.1fs", "Attack", adsr->getAttackDurationMs() / 1000.0);
    String leftString = String(buffer);

    snprintf(buffer, sizeof(buffer), "%.1fs", adsr->getEnvelopeDurationMs() / 1000.0);
    String rightString = String(buffer);

    captionSprite.drawString(leftString, 6, 6);
    int16_t rightX = captionSprite.width() - captionSprite.textWidth(rightString) - 6;
    captionSprite.drawString(rightString, rightX, 6);
}


