#include "AdsrEnvelope.h"

#if defined(ESP32)
#include <ESP32Encoder.h>
#define DAC_BIT_WIDTH 8
#define ENCODER_A_PIN 32
#define ENCODER_B_PIN 33
#define ENCODER_BUTTON_PIN 26
void IRAM_ATTR handleSwitch();

ESP32Encoder encoder;
#else
#include <Encoder.h>
#define ENCODER_A_PIN 5
#define ENCODER_B_PIN 6
#define DAC_BIT_WIDTH 12
#define ENCODER_BUTTON_PIN 7

Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);
#endif

const int A_max = (1 << DAC_BIT_WIDTH) - 1;  // 4095 on 12 bit DAC
const int A_sustain = A_max * 0.75;
unsigned long T_attack = 100;         // Attack time in milliseconds
const unsigned long T_decay = 75;     // Decay time in milliseconds
const unsigned long T_sustain = 250;  // Sustain time in milliseconds
const unsigned long T_release = 250;  // Release time in milliseconds

int envelopeValue = 0;
unsigned long startTime = 0;

enum EncoderState {
  ATTACK_DURATION,
  ATTACK_SHAPE
};

volatile EncoderState encoderState = ATTACK_DURATION;

long encoderPosition = 0;

const byte buzzPin = 3;

AdsrEnvelope adsr(A_max, T_attack, T_decay, T_sustain, A_sustain, T_release);

void setup() {
  Serial.begin(115200);

#if defined(ESP32)
  delay(1000);  // wait for Serial on ESP32
  encoder.attachHalfQuad(ENCODER_A_PIN, ENCODER_B_PIN);
#else
  analogWriteResolution(DAC_BIT_WIDTH);  // Max out DAC resolution
#endif

  adsr.setEnvelopeStartTime(millis());

  Serial.println("Hello Envelope Generator");

  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON_PIN), handleSwitch, RISING);

  // pinMode(buzzPin, OUTPUT);
  // tone(buzzPin, 200);
}

void loop() {

    #if defined(ESP32)
    long newEncoderPosition = encoder.getCount() / 2;
    #else
    long newEncoderPosition = encoder.read() / 4;
    #endif

    if (newEncoderPosition != encoderPosition) {
        Serial.print("Encoder position: ");
        Serial.println(newEncoderPosition);

        long encoderDelta = (newEncoderPosition - encoderPosition);
        encoderPosition = newEncoderPosition;
        double attackDurationMs;
        double attackShapeFactor;
        switch (encoderState) {
        case ATTACK_DURATION:
            attackDurationMs = adsr.getAttackDurationMs();
            attackDurationMs += 50 * encoderDelta;
            if (attackDurationMs > 1000) {
                attackDurationMs = 1000;
            } else if (attackDurationMs < 0) {
                attackDurationMs = 0;
            }
            adsr.setAttackDurationMs(attackDurationMs);
            Serial.print("Attack duration: ");
            Serial.println(attackDurationMs);
            break;
        case ATTACK_SHAPE:
            attackShapeFactor = adsr.getAttackShapeFactor();
            attackShapeFactor += 0.5 * encoderDelta;
            if (attackShapeFactor > 10) {
                attackShapeFactor = 10;
            } else if (attackShapeFactor < 4) {
                attackShapeFactor = 4;
            }
            adsr.setAttackShapeFactor(attackShapeFactor);
            Serial.print("Attack shape factor: ");
            Serial.println(attackShapeFactor);
            break;
        }
   }

  unsigned long currentTime = millis();

  // For now, automatically trigger a new adsr envelope once the previous one has finished
  if(currentTime > adsr.getEnvelopeStartTime() + adsr.getEnvelopeDurationMs()) {
    adsr.setEnvelopeStartTime(currentTime);
  }

  envelopeValue = adsr.getEnvelopeValue(currentTime);
#if defined(ESP32)
  dacWrite(DAC1, envelopeValue);
#else
  analogWrite(DAC, envelopeValue);
#endif
}

#if defined(ESP32)
void IRAM_ATTR handleSwitch() {
# else
void handleSwitch() {
#endif
  // note: not okay to call Serial.println method on ESP32 here as it blocks.
  // TODO: probably need to debounce.
  encoderState = (EncoderState)((encoderState + 1) % 2);
}
