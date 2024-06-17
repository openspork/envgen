#include <Encoder.h>
#include "AdsrEnvelope.h"

#if defined(ESP32)
#define DAC_BIT_WIDTH 8
#else
#define DAC_BIT_WIDTH 12
#endif

const int A_max = (1 << DAC_BIT_WIDTH) - 1;  // 4095 on 12 bit DAC
const int A_sustain = A_max * 0.75;
unsigned long T_attack = 100;         // Attack time in milliseconds
const unsigned long T_decay = 75;     // Decay time in milliseconds
const unsigned long T_sustain = 250;  // Sustain time in milliseconds
const unsigned long T_release = 250;  // Release time in milliseconds

int envelopeValue = 0;
unsigned long startTime = 0;

// Encoder myEnc(5, 6);
const byte switchPin = 7;

long oldPosition = -999;

const byte buzzPin = 3;

AdsrEnvelope adsr(A_max, T_attack, T_decay, T_sustain, A_sustain, T_release);

void setup() {
  Serial.begin(115200);

#if defined(ESP32)
  delay(1000);  // wait for Serial on ESP32
#else
  analogWriteResolution(DAC_BIT_WIDTH);  // Max out DAC resolution
#endif

  // Initialize the envelope stage
  adsr.setEnvelopeStartTime(millis());

  Serial.println("Hello Envelope Generator");

  // pinMode(switchPin, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(switchPin), handleSwitch, RISING);

  // pinMode(buzzPin, OUTPUT);
  // tone(buzzPin, 200);
}

void loop() {

//   long newPosition = myEnc.read();
//   if (newPosition != oldPosition) {
//     // Turning knob right position goes down, but want attack to go up
//     if (newPosition < oldPosition) {
//       T_attack = T_attack + 25;

//     } else if (newPosition > oldPosition) {
//       if (T_attack >= 25) {
//         T_attack = T_attack - 25;
//       }
//     }

//     oldPosition = newPosition;
//     Serial.print("Attack change: ");
//     Serial.println(T_attack);
//   }

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

void handleSwitch() {
  Serial.println("Switch pressed");
}