#include "Display.h"

Display::Display(int width, int height) : screenWidth(width), screenHeight(height), tft(), chartSprite(&tft) {
    chartSprite.createSprite(screenWidth, screenHeight);
}

void Display::init() {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    tft.init();
    tft.setRotation(3);
    Serial.println("Display initialized");
}

void Display::drawChart(AdsrEnvelope* adsr) {
    chartSprite.fillSprite(TFT_BLACK);

    int xPrev = 0;
    int yPrev = -1;

    for (int i = 0; i < screenWidth; i++) {
        double time = adsr->getEnvelopeStartTime() + i * adsr->getEnvelopeDurationMs() / screenWidth;
        double envelopeValue = adsr->getEnvelopeValue(time);

        int x = screenWidth - 1 - i;
        int y = (int)(envelopeValue * screenHeight / adsr->getEnvelopeMax());

        if (yPrev > 0) {
          chartSprite.drawLine(xPrev, yPrev, x, y, TFT_WHITE);
        }

        xPrev = x;
        yPrev = y;

        // Serial.print("[");
        // Serial.print(x);
        // Serial.print(", ");
        // Serial.print(y);
        // Serial.print("] ");
    }

    chartSprite.pushSprite(0, 0);

    Serial.println("Chart drawn");
}


