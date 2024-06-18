#include "Display.h"

Display::Display(int width, int height) : screenWidth(width), screenHeight(height), tft() {
    buffer = (uint16_t*)malloc(screenWidth * screenHeight * sizeof(uint16_t));
}

void Display::init() {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    tft.init();
    tft.setRotation(3);
    Serial.println("Display initialized");
}

void Display::drawChart(AdsrEnvelope* adsr) {
    memset (buffer, 0, screenWidth * screenHeight * sizeof(uint16_t));

    for (int i = 0; i < screenWidth; i++) {
        double time = adsr->getEnvelopeStartTime() + i * adsr->getEnvelopeDurationMs() / screenWidth;
        double envelopeValue = adsr->getEnvelopeValue(time);

        int x = screenWidth - 1 - i;
        int y = (int)(envelopeValue * screenHeight / adsr->getEnvelopeMax());
        buffer[x + y * screenWidth] = TFT_WHITE;
        // Serial.print("[");
        // Serial.print(x);
        // Serial.print(", ");
        // Serial.print(y);
        // Serial.print("] ");
    }

    tft.pushImage(0, 0, screenWidth, screenHeight, buffer);
    Serial.println("Chart drawn");
}


