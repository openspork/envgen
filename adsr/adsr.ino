#include "AdsrEnvelope.h"
#include "Display.h"
#include "EncoderHandler.h"

#if defined(ESP32)
#define DAC_BIT_WIDTH 8
#else
#define DAC_BIT_WIDTH 12
#endif

int envelopeValue = 0;
unsigned long startTime = 0;

uint16_t A_max = (1 << DAC_BIT_WIDTH) - 1;
AdsrEnvelope adsr(
    A_max, // 4095 on 12 bit DAC
    100, // T_attack
    75, // T_decay
    250, // T_sustain
    A_max * 0.75, //  A_sustain
    250 // T_release
    );
Display display(240, 135);
EncoderHandler encoderHandler(&adsr);

void setup() {
  Serial.begin(115200);

#if defined(ESP32)
  delay(1000);  // wait for Serial on ESP32
#else
  analogWriteResolution(DAC_BIT_WIDTH);  // Max out DAC resolution
#endif

  adsr.setEnvelopeStartTime(millis());

  Serial.println("Hello Envelope Generator");

  display.init();
  encoderHandler.registerOnEncoderChange(onEncoderChanged);
  encoderHandler.setup();
  display.draw(&adsr);
}

void loop() {
  unsigned long currentTime = millis();

  // For now, automatically trigger a new adsr envelope once the previous one has finished
  if(currentTime > adsr.getEnvelopeStartTime() + adsr.getEnvelopeDurationMs()) {
    adsr.setEnvelopeStartTime(currentTime);
  }

  envelopeValue = adsr.getEnvelopeValue(currentTime);

  encoderHandler.tick();

#if defined(ESP32)
  dacWrite(DAC1, envelopeValue);
#else
  analogWrite(DAC, envelopeValue);
#endif
}

void onEncoderChanged() {
  display.draw(&adsr);
}
